#include <3ds.h>
#include "graphic.h"
#include "pp2d/pp2d.h"
#include "sound.h"
#include "settings.h"

#include "psonemenu.h"

#define CONFIG_3D_SLIDERSTATE (*(float *)0x1FF81080)


extern sound *bgm_sce;

extern u32 hDown;
extern u32 hHeld;

static bool oneMusicStopped = false;
static int oneMenu_musicStopWait = 0;
static bool oneDisplayMenuGraphics = false;
static int oneMenu_cursor = 0;
static bool oneMenu_textFade = false;

static int oneMenu_textFadeColor = 255;	// 0 when faded out

void psoneMenuInit(void) {
	oneMusicStopped = false;
	oneMenu_musicStopWait = 0;
	oneDisplayMenuGraphics = false;
	oneMenu_cursor = 0;
	oneMenu_textFade = false;

	oneMenu_textFadeColor = 255;
}

void psoneMenu(void) {
	if (!oneMusicStopped) {
		bgm_sce->stop();	// Stop SPE logo music
		oneMusicStopped = true;
	}

	if (oneDisplayMenuGraphics) {
		if (oneMenu_textFade) {
			oneMenu_textFadeColor -= 10;
			if (oneMenu_textFadeColor < 0) {
				oneMenu_textFadeColor = 255;
				oneMenu_textFade = false;
			}
		} else {
			if (hDown & KEY_LEFT) {
				oneMenu_cursor--;
				if (oneMenu_cursor < 0) oneMenu_cursor = 0;
			}

			if (hDown & KEY_RIGHT) {
				oneMenu_cursor++;
				if (oneMenu_cursor > 1) oneMenu_cursor = 1;
			}

			if (hDown & KEY_A) {
				oneMenu_textFade = true;
			}
		}
	} else {
		oneMenu_musicStopWait++;
		if (oneMenu_musicStopWait == 30) oneDisplayMenuGraphics = true;
	}
}

void psoneMenuGraphicDisplay(int topfb) {
	offset3D[0].level = CONFIG_3D_SLIDERSTATE * -3.0f;
	offset3D[1].level = CONFIG_3D_SLIDERSTATE * 3.0f;
	pp2d_draw_texture(gridBgTex, -20+offset3D[topfb].level, 0);
}