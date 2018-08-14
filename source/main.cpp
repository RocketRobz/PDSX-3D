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

#define CONFIG_3D_SLIDERSTATE (*(float *)0x1FF81080)

bool dspfirmfound = false;

bool simulationRunning = true;
bool vcMenuMusicPlayed = false;

// Sound effects.
sound *sfx_vcmenu = NULL;
sound *sfx_vcmenuselect = NULL;
sound *sfx_sce = NULL;
sound *sfx_playstation = NULL;

int gameMode = 0;
int modeOrder = 0;

int psConsoleModel = 0;					// 0 = Playstation -> PS, 1 = PSone

// 3D offsets. (0 == Left, 1 == Right)
Offset3D offset3D[2] = {{0.0f}, {0.0f}};

// Version numbers.
char vertext[13];

int main()
{
	aptInit();
	amInit();
	sdmcInit();
	romfsInit();
	srvInit();
	hidInit();

	snprintf(vertext, 13, "PDSX 3D v%d.%d.%d by Robz8", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);

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
		sfx_vcmenu = new sound("romfs:/sounds/vcmenu.wav", 2, false);
		sfx_vcmenuselect = new sound("romfs:/sounds/vcmenuselect.wav", 2, false);
		sfx_sce = new sound("romfs:/sounds/sce.wav", 2, false);
		sfx_playstation = new sound("romfs:/sounds/playstation.wav", 2, false);
	}
	
	LoadSettings();

	int fadealpha = 255;
	bool fadein = true;
	
	// Loop as long as the status is not exit
	while(aptMainLoop()) {
		offset3D[0].logo = CONFIG_3D_SLIDERSTATE * -5.0f;
		offset3D[1].logo = CONFIG_3D_SLIDERSTATE * 5.0f;
		//offset3D[0].launchertext = CONFIG_3D_SLIDERSTATE * -3.0f;
		//offset3D[1].launchertext = CONFIG_3D_SLIDERSTATE * 3.0f;

		// Scan hid shared memory for input events
		hidScanInput();
		
		const u32 hDown = hidKeysDown();
		const u32 hHeld = hidKeysHeld();

		if (hDown & KEY_TOUCH) {
			simulationRunning = !simulationRunning;
			if (simulationRunning) {
				sceInit();
			} else {
				sfx_sce->stop();
				vcMenuMusicPlayed = false;
			}
		}

		if (simulationRunning) {
			switch (gameMode) {
				case 0:
				default:
					sceSplash();
					break;
			}
		}

		if (simulationRunning) pp2d_draw_on(GFX_BOTTOM, GFX_LEFT);
		else pp2d_begin_draw(GFX_BOTTOM, GFX_LEFT);
		const char *vc_text = "Tap the Touch Screen to go";
		const char *vc_text2 = "to the Virtual Console menu.";
		const int vc_width = pp2d_get_text_width(vc_text, 0.50, 0.50);
		const int vc_width2 = pp2d_get_text_width(vc_text, 0.50, 0.50);
		const int vc_x = (320-vc_width)/2;
		const int vc_x2 = (320-vc_width2)/2;
		pp2d_draw_text(vc_x, 120, 0.50, 0.50, WHITE, vc_text);
		pp2d_draw_text(vc_x2, 132, 0.50, 0.50, WHITE, vc_text2);
		const char *home_text = ": Return to HOME Menu";
		const int home_width = pp2d_get_text_width(home_text, 0.50, 0.50) + 16;
		const int home_x = (320-home_width)/2;
		pp2d_draw_texture(homeicontex, home_x, 219); // Draw HOME icon
		pp2d_draw_text(home_x+20, 220, 0.50, 0.50, WHITE, home_text);
		//if (fadealpha > 0) pp2d_draw_rectangle(0, 0, 320, 240, RGBA8(0, 0, 0, fadealpha)); // Fade in/out effect
		pp2d_end_draw();

		if (!simulationRunning && !vcMenuMusicPlayed) {
			sfx_vcmenu->play();
			vcMenuMusicPlayed = true;
		}

		if (fadein == true) {
			fadealpha -= 15;
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
	delete sfx_sce;
	delete sfx_playstation;
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