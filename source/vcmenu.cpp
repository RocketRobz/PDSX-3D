#include <3ds.h>
#include "graphic.h"
#include "pp2d/pp2d.h"
#include "sound.h"
#include "settings.h"

#include "vcmenu.h"
#include "scesplash.h"
#include "pssplash.h"
#include "ogpsmenu.h"
#include "psonemenu.h"


extern int topFadeAlpha;
extern bool simulationRunning;

extern int gameMode;
extern int gameModeBuffer;

extern int blackScreenDelay;

extern int psConsoleModel;					// 0 = Playstation (SCPH1001) -> PS, 1 = PSone (SCPH101)

extern sound *sfx_vcmenuselect;
extern sound *bgm_sce;
extern sound *bgm_playstation;

extern u32 hDown;
extern u32 hHeld;

extern char vertext[24];

static int vcMenu_bubblesYPos[2] = {0};
static bool vcMenu_bubbleMoveDelay = false;

static int vcMenu_screen = 0;

static int vcMenu_cursorPosition[2] = {0};

const char *vcMenu_resume = "Resume";
const char *vcMenu_restart = "Restart";
const char *vcMenu_settings = "Settings";

int vcMenu_width = 0;
int vcMenu_x = 0;

void vcMenu(void) {
	if (!vcMenu_bubbleMoveDelay) {
		vcMenu_bubblesYPos[0]--;
		if (vcMenu_bubblesYPos[0] <= -240) vcMenu_bubblesYPos[0] = 0;
	}
	vcMenu_bubbleMoveDelay = !vcMenu_bubbleMoveDelay;

	vcMenu_bubblesYPos[1]--;
	if (vcMenu_bubblesYPos[1] <= -240) vcMenu_bubblesYPos[1] = 0;

	if (hDown & KEY_UP) {
		vcMenu_cursorPosition[vcMenu_screen]--;
		sfx_vcmenuselect->stop();
		sfx_vcmenuselect->play();
	} else if (hDown & KEY_DOWN) {
		vcMenu_cursorPosition[vcMenu_screen]++;
		sfx_vcmenuselect->stop();
		sfx_vcmenuselect->play();
	}

	if (vcMenu_screen == 0) {
		if (vcMenu_cursorPosition[0] < 0) vcMenu_cursorPosition[0] = 2;
		if (vcMenu_cursorPosition[0] > 2) vcMenu_cursorPosition[0] = 0;

		if (hDown & KEY_A) {
			sfx_vcmenuselect->stop();
			sfx_vcmenuselect->play();
			switch (vcMenu_cursorPosition[0]) {
				case 0:
				default:
					simulationRunning = true;
					break;
				case 1:
					bgm_sce->stop();
					bgm_playstation->stop();
					sceInit();
					psSplashInit();
					ogPsMenuInit();
					psoneMenuInit();
					if (settings.pseudoEmulation.modeOrder == 2) {
						gameModeBuffer = 1;
					} else {
						gameModeBuffer = 0;
					}
					blackScreenDelay = 0;
					gameMode = -1;
					psConsoleModel = settings.pseudoEmulation.bios;
					simulationRunning = true;
					vcMenu_cursorPosition[0] = 0;
					break;
				case 2:
					vcMenu_screen = 1;
					break;
			}
		}

		if (hDown & KEY_B) {
			simulationRunning = true;
			vcMenu_cursorPosition[0] = 0;
		}
	} else if (vcMenu_screen == 1) {
		if (vcMenu_cursorPosition[1] < 0) vcMenu_cursorPosition[1] = 2;
		if (vcMenu_cursorPosition[1] > 2) vcMenu_cursorPosition[1] = 0;

		if (hDown & KEY_A) {
			sfx_vcmenuselect->stop();
			sfx_vcmenuselect->play();
			switch (vcMenu_cursorPosition[1]) {
				case 0:
				default:
					settings.pseudoEmulation.border++;
					break;
				case 1:
					settings.pseudoEmulation.bios++;
					break;
				case 2:
					settings.pseudoEmulation.modeOrder++;
					break;
			}
		}

		if (settings.pseudoEmulation.border > 2) settings.pseudoEmulation.border = 0;
		if (settings.pseudoEmulation.bios > 1) settings.pseudoEmulation.bios = 0;
		if (settings.pseudoEmulation.modeOrder > 2) settings.pseudoEmulation.modeOrder = 0;

		if (hDown & KEY_B) {
			vcMenu_screen = 0;
		}
	}
}

void vcMenuGraphicDisplay(void) {
	pp2d_draw_texture(vcMenuBgTex, 0, 0);
	pp2d_draw_texture_flip(vcMenuBgTex, 160, 0, HORIZONTAL);

	pp2d_draw_texture(vcMenuBubbleTex[0], 0, vcMenu_bubblesYPos[0]);
	pp2d_draw_texture(vcMenuBubbleTex[0], 0, vcMenu_bubblesYPos[0]+240);
	pp2d_draw_texture(vcMenuBubbleTex[1], 0, vcMenu_bubblesYPos[1]);
	pp2d_draw_texture(vcMenuBubbleTex[1], 0, vcMenu_bubblesYPos[1]+240);

	const char *home_text = ": Return to HOME Menu";
	const int home_width = pp2d_get_text_width(home_text, 0.50, 0.50) + 16;
	const int home_x = (320-home_width)/2;
	pp2d_draw_texture(homeiconTex, home_x, 219); // Draw HOME icon
	pp2d_draw_text(home_x+20, 220, 0.50, 0.50, WHITE, home_text);

	if (simulationRunning) {
		const char *vc_text = "Tap the Touch Screen to go";
		const char *vc_text2 = "to the Virtual Console menu.";
		const int vc_width = pp2d_get_text_width(vc_text, 0.50, 0.50);
		const int vc_width2 = pp2d_get_text_width(vc_text, 0.50, 0.50);
		const int vc_x = (320-vc_width)/2;
		const int vc_x2 = (320-vc_width2)/2;
		pp2d_draw_text(vc_x, 104, 0.50, 0.50, WHITE, vc_text);
		pp2d_draw_text(vc_x2, 118, 0.50, 0.50, WHITE, vc_text2);
	} else if (topFadeAlpha == 191) {
		pp2d_draw_text(8, 8, 0.50, 0.50, WHITE, vertext);
		pp2d_draw_text(8, 24, 0.50, 0.50, WHITE, "by Robz8");

		if (vcMenu_screen == 0) {
			pp2d_draw_rectangle(0, 72+(vcMenu_cursorPosition[0]*32), 400, 32, RGBA8(255, 0, 0, 127));

			vcMenu_width = pp2d_get_text_width(vcMenu_resume, 0.75, 0.75);
			vcMenu_x = (320-vcMenu_width)/2;
			pp2d_draw_text(vcMenu_x, 76, 0.75, 0.75, WHITE, vcMenu_resume);

			vcMenu_width = pp2d_get_text_width(vcMenu_restart, 0.75, 0.75);
			vcMenu_x = (320-vcMenu_width)/2;
			pp2d_draw_text(vcMenu_x, 108, 0.75, 0.75, WHITE, vcMenu_restart);

			vcMenu_width = pp2d_get_text_width(vcMenu_settings, 0.75, 0.75);
			vcMenu_x = (320-vcMenu_width)/2;
			pp2d_draw_text(vcMenu_x, 140, 0.75, 0.75, WHITE, vcMenu_settings);
		} else if (vcMenu_screen == 1) {
			pp2d_draw_rectangle(0, 72+(vcMenu_cursorPosition[1]*32), 400, 32, RGBA8(255, 0, 0, 127));

			pp2d_draw_text(8, 76, 0.65, 0.65, WHITE, "Border");
			if (settings.pseudoEmulation.border == 0) {
				pp2d_draw_text(260, 76, 0.65, 0.65, WHITE, "None");
			} else if (settings.pseudoEmulation.border == 1) {
				pp2d_draw_text(260, 76, 0.65, 0.65, WHITE, "Black");
			} else if (settings.pseudoEmulation.border == 2) {
				pp2d_draw_text(256, 76, 0.65, 0.65, WHITE, "PSone");
			}

			pp2d_draw_text(8, 108, 0.65, 0.65, WHITE, "BIOS");
			if (settings.pseudoEmulation.bios == 0) {
				pp2d_draw_text(216, 108, 0.65, 0.65, WHITE, "SCPH1001");
			} else if (settings.pseudoEmulation.bios == 1) {
				pp2d_draw_text(224, 108, 0.65, 0.65, WHITE, "SCPH101");
			}

			pp2d_draw_text(8, 140, 0.65, 0.65, WHITE, "Screen order");
			if (settings.pseudoEmulation.modeOrder == 0) {
				pp2d_draw_text(170, 140, 0.65, 0.65, WHITE, "SCE > Playstation");
			} else if (settings.pseudoEmulation.modeOrder == 1) {
				pp2d_draw_text(170, 140, 0.65, 0.65, WHITE, "SCE > Main Menu");
			} else if (settings.pseudoEmulation.modeOrder == 2) {
				pp2d_draw_text(224, 140, 0.65, 0.65, WHITE, "Playstation");
			}
		}
	}
}