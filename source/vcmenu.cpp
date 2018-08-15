#include <3ds.h>
#include "graphic.h"
#include "pp2d/pp2d.h"
#include "sound.h"
#include "settings.h"

#include "vcmenu.h"
#include "scesplash.h"
#include "pssplash.h"


extern bool simulationRunning;

extern int gameMode;
extern int modeOrder;

extern int psConsoleModel;					// 0 = Playstation -> PS, 1 = PSone

extern sound *sfx_vcmenuselect;
extern sound *bgm_sce;
extern sound *bgm_playstation;

extern u32 hDown;
extern u32 hHeld;

static int vcMenu_screen = 0;

static int vcMenu_cursorPosition = 0;

const char *vcMenu_resume = "Resume";
const char *vcMenu_restart = "Restart";
const char *vcMenu_settings = "Settings";

int vcMenu_width = 0;
int vcMenu_x = 0;

void vcMenu(void) {
	if (hDown & KEY_UP) {
		vcMenu_cursorPosition--;
		sfx_vcmenuselect->stop();
		sfx_vcmenuselect->play();
	} else if (hDown & KEY_DOWN) {
		vcMenu_cursorPosition++;
		sfx_vcmenuselect->stop();
		sfx_vcmenuselect->play();
	}

	if (vcMenu_cursorPosition < 0) vcMenu_cursorPosition = 1;
	if (vcMenu_cursorPosition > 1) vcMenu_cursorPosition = 0;

	if (hDown & KEY_A) {
		switch (vcMenu_cursorPosition) {
			case 0:
			default:
				simulationRunning = true;
				ndspChnSetPaused(3, false);		// Unpause SCE logo sound
				ndspChnSetPaused(4, false);		// Unpause Playstation logo sound
				break;
			case 1:
				bgm_sce->stop();
				bgm_playstation->stop();
				sceInit();
				psSplashInit();
				if (modeOrder == 2) {
					gameMode = 1;
				} else {
					gameMode = 0;
				}
				simulationRunning = true;
				ndspChnSetPaused(3, false);
				ndspChnSetPaused(4, false);
				vcMenu_cursorPosition = 0;
				break;
		}
	}

	if (hDown & KEY_B) {
		simulationRunning = true;
		ndspChnSetPaused(3, false);		// Unpause SCE logo sound
		ndspChnSetPaused(4, false);		// Unpause Playstation logo sound
	}
}

void vcMenuGraphicDisplay(void) {
	pp2d_draw_rectangle(0, 88+(vcMenu_cursorPosition*32), 400, 32, RGBA8(255, 0, 0, 127));

	vcMenu_width = pp2d_get_text_width(vcMenu_resume, 0.75, 0.75);
	vcMenu_x = (320-vcMenu_width)/2;
	pp2d_draw_text(vcMenu_x, 92, 0.75, 0.75, WHITE, vcMenu_resume);

	vcMenu_width = pp2d_get_text_width(vcMenu_restart, 0.75, 0.75);
	vcMenu_x = (320-vcMenu_width)/2;
	pp2d_draw_text(vcMenu_x, 124, 0.75, 0.75, WHITE, vcMenu_restart);
}