#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <3ds.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/stat.h>

#include "graphic.h"
#include "pp2d/pp2d.h"
#include "sound.h"
#include "dumpdsp.h"
#include "settings.h"

#include "scesplash.h"
#include "pssplash.h"
#include "ogpsmenu.h"
#include "psonemenu.h"
#include "vcmenu.h"

#define CONFIG_3D_SLIDERSTATE (*(float *)0x1FF81080)

bool dspfirmfound = false;

int topFadeAlpha = 0;
bool simulationRunning = true;
bool vcMenuMusicPlayed = false;

// Music and sound effects.
sound *sfx_vcmenu = NULL;
sound *sfx_vcmenuselect = NULL;
sound *bgm_sce = NULL;
sound *bgm_playstation = NULL;

int gameMode = -1;
int gameModeBuffer = 0;

int blackScreenDelay = 0;

int psConsoleModel = 0;					// 0 = Playstation (SCPH1001) -> PS, 1 = PSone (SCPH101)

u32 hDown;
u32 hHeld;

// Version numbers.
char vertext[24];

int main()
{
	aptInit();
	amInit();
	sdmcInit();
	romfsInit();
	srvInit();
	hidInit();

	snprintf(vertext, 24, "PDSX 3D v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);

	// make folders if they don't exist
	mkdir("sdmc:/3ds", 0777);	// For DSP dump

	pp2d_init();

	pp2d_set_screen_color(GFX_TOP, TRANSPARENT);
	pp2d_set_3D(1);

	Result res = 0;

	graphicsInit();
	
 	if( access( "sdmc:/3ds/dspfirm.cdc", F_OK ) != -1 ) {
		ndspInit();
		dspfirmfound = true;
	}else{
		pp2d_begin_draw(GFX_BOTTOM, GFX_LEFT);
		pp2d_draw_text(12, 16, 0.5f, 0.5f, WHITE, "Dumping DSP firm...");
		pp2d_end_draw();
		dumpDsp();
		if( access( "sdmc:/3ds/dspfirm.cdc", F_OK ) != -1 ) {
			ndspInit();
			dspfirmfound = true;
		} else {
			for (int i = 0; i < 90; i++) {
				pp2d_begin_draw(GFX_BOTTOM, GFX_LEFT);
				pp2d_draw_text(12, 16, 0.5f, 0.5f, WHITE, "DSP firm dumping failed.\n"
						"Running without sound.");
				pp2d_end_draw();
			}	
		}
	}

	// Load the sound effects if DSP is available.
	if (dspfirmfound) {
		sfx_vcmenu = new sound("romfs:/sounds/vcmenu.wav", 1, false);
		sfx_vcmenuselect = new sound("romfs:/sounds/vcmenuselect.wav", 2, false);
		bgm_sce = new sound("romfs:/sounds/sce.wav", 3, false);
		bgm_playstation = new sound("romfs:/sounds/playstation.wav", 4, false);
	}

	LoadSettings();

	if (settings.pseudoEmulation.modeOrder == 2) {
		gameModeBuffer = 1;
	} else {
		gameModeBuffer = 0;
	}

	int fadealpha = 255;
	bool fadein = true;

	// Loop as long as the status is not exit
	while(aptMainLoop()) {

		// Scan hid shared memory for input events
		hidScanInput();

		hDown = hidKeysDown();
		hHeld = hidKeysHeld();

		if (fadealpha == 0 && topFadeAlpha == 0
		&& simulationRunning && (hDown & KEY_TOUCH)) {
			simulationRunning = false;
			ndspChnSetPaused(3, true);			// Pause SCE logo sound
			ndspChnSetPaused(4, true);			// Pause Playstation logo sound
			vcMenuMusicPlayed = false;
		}

		for (int topfb = GFX_LEFT; topfb <= GFX_RIGHT; topfb++) {
			if (topfb == GFX_LEFT) pp2d_begin_draw(GFX_TOP, (gfx3dSide_t)topfb);
			else pp2d_draw_on(GFX_TOP, (gfx3dSide_t)topfb);
			switch (gameMode) {
				case 0:
				default:
					sceGraphicDisplay(topfb);
					break;
				case 1:
					psGraphicDisplay(topfb);
					break;
				case 2:
					if (psConsoleModel == 1) {
						psoneMenuGraphicDisplay(topfb);
					} else {
						ogPsMenuGraphicDisplay(topfb);
					}
					break;
			}
			if (topFadeAlpha > 0) pp2d_draw_rectangle(0, 0, 400, 240, RGBA8(0, 0, 0, topFadeAlpha)); // Fade in/out effect
			if (settings.pseudoEmulation.border == 1) {
				pp2d_draw_rectangle(0, 0, 40, 240, RGBA8(0, 0, 0, 255));
				pp2d_draw_rectangle(360, 0, 40, 240, RGBA8(0, 0, 0, 255));
			} else if (settings.pseudoEmulation.border == 2) {
				pp2d_draw_texture(psoneBorderTex, 0, 0);
				pp2d_draw_texture_flip(psoneBorderTex, 360, 0, HORIZONTAL);
			}
		}

		if (fadealpha == 0 && topFadeAlpha == 0 && simulationRunning) {
			switch (gameMode) {
				case -1:
					blackScreenDelay++;
					if (blackScreenDelay == 60) {
						gameMode = gameModeBuffer;
					}
					break;
				case 0:
				default:
					sceSplash();
					break;
				case 1:
					psSplash();
					break;
				case 2:
					if (psConsoleModel == 1) {
						psoneMenu();
					} else {
						ogPsMenu();
					}
					break;
			}
		}

		pp2d_draw_on(GFX_BOTTOM, GFX_LEFT);
		vcMenuGraphicDisplay();
		if (fadealpha > 0) pp2d_draw_rectangle(0, 0, 320, 240, RGBA8(0, 0, 0, fadealpha)); // Fade in/out effect
		pp2d_end_draw();

		if (!simulationRunning) {
			topFadeAlpha += 10;
			if (topFadeAlpha >= 191) {
				topFadeAlpha = 191;
				vcMenu();
			}
		} else {
			topFadeAlpha -= 10;
			if (topFadeAlpha <= 0) {
				if (ndspChnIsPaused(3)) ndspChnSetPaused(3, false);		// Unpause SCE logo sound
				if (ndspChnIsPaused(4)) ndspChnSetPaused(4, false);		// Unpause Playstation logo sound
				topFadeAlpha = 0;
			}
		}

		if (!simulationRunning && !vcMenuMusicPlayed) {
			sfx_vcmenu->play();
			vcMenuMusicPlayed = true;
		}

		if (fadein == true) {
			fadealpha -= 5;
			if (fadealpha < 0) {
				fadealpha = 0;
				fadein = false;
			}
		}

		//if (hDown & KEY_A) {

		//}
	}

	
	SaveSettings();

	delete sfx_vcmenu;
	delete sfx_vcmenuselect;
	delete bgm_sce;
	delete bgm_playstation;
	if (dspfirmfound) {
		ndspExit();
	}

	pp2d_exit();

	hidExit();
	srvExit();
	romfsExit();
	sdmcExit();
	aptExit();

    return 0;
}