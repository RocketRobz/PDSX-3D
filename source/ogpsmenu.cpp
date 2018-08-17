#include <3ds.h>
#include "graphic.h"
#include "pp2d/pp2d.h"
#include "sound.h"
#include "settings.h"

#include "ogpsmenu.h"

#define CONFIG_3D_SLIDERSTATE (*(float *)0x1FF81080)


extern sound *bgm_sce;

extern u32 hDown;
extern u32 hHeld;

static bool musicStopped = false;
static int menu_musicStopWait = 0;
static bool displayMenuGraphics = false;
static int menu_cursor = 0;
static bool menu_textFade = false;

static int menu_textFadeColor = 255;	// 0 when faded out

extern int psConsoleModel;					// 0 = Playstation -> PS, 1 = PSone

void ogPsMenuInit(void) {
	musicStopped = false;
	menu_musicStopWait = 0;
	displayMenuGraphics = false;
	menu_cursor = 0;
	menu_textFade = false;

	menu_textFadeColor = 255;
}

void ogPsMenu(void) {
	if (!musicStopped) {
		menu_musicStopWait++;
		if (menu_musicStopWait == 30) {
			bgm_sce->stop();	// Stop SPE logo music
			musicStopped = true;
			displayMenuGraphics = true;
		}
	}
	
	if (displayMenuGraphics) {
		if (menu_textFade) {
			menu_textFadeColor -= 10;
			if (menu_textFadeColor < 0) {
				menu_textFadeColor = 255;
				menu_textFade = false;
			}
		} else {
			if (hDown & KEY_UP) {
				menu_cursor--;
				if (menu_cursor < 0) menu_cursor = 0;
			}

			if (hDown & KEY_DOWN) {
				menu_cursor++;
				if (menu_cursor > 1) menu_cursor = 1;
			}

			if (hDown & KEY_A) {
				menu_textFade = true;
			}
		}
	}
}

void ogPsMenuGraphicDisplay(int topfb) {
	pp2d_draw_rectangle(0, 0, 400, 240, RGBA8(0, 0, 33, 255));

	offset3D[0].level = CONFIG_3D_SLIDERSTATE * -6.0f;
	offset3D[1].level = CONFIG_3D_SLIDERSTATE * 6.0f;
	pp2d_draw_texture_part(blueMenuBgTex, 40+offset3D[topfb].level, 0, 0, 0, 320, 34);
	pp2d_draw_texture_part(blueMenuBgTex, 40+offset3D[topfb].level, 34*6, 0, 34*6, 320, 36);

	offset3D[0].level = CONFIG_3D_SLIDERSTATE * -5.0f;
	offset3D[1].level = CONFIG_3D_SLIDERSTATE * 5.0f;
	pp2d_draw_texture_part(blueMenuBgTex, 40+offset3D[topfb].level, 34, 0, 34, 320, 34);
	pp2d_draw_texture_part(blueMenuBgTex, 40+offset3D[topfb].level, 34*5, 0, 34*5, 320, 34);

	offset3D[0].level = CONFIG_3D_SLIDERSTATE * -4.0f;
	offset3D[1].level = CONFIG_3D_SLIDERSTATE * 4.0f;
	pp2d_draw_texture_part(blueMenuBgTex, 40+offset3D[topfb].level, 34*2, 0, 34*2, 320, 34);
	pp2d_draw_texture_part(blueMenuBgTex, 40+offset3D[topfb].level, 34*4, 0, 34*4, 320, 34);

	offset3D[0].level = CONFIG_3D_SLIDERSTATE * -3.0f;
	offset3D[1].level = CONFIG_3D_SLIDERSTATE * 3.0f;
	pp2d_draw_texture_part(blueMenuBgTex, 40+offset3D[topfb].level, 34*3, 0, 34*3, 320, 34);

	if (displayMenuGraphics) {
		offset3D[0].level = CONFIG_3D_SLIDERSTATE * -1.0f;
		offset3D[1].level = CONFIG_3D_SLIDERSTATE * 1.0f;
		pp2d_draw_texture(blueMainMenuTextTex, 40+offset3D[topfb].level+226, 22);

		offset3D[0].level = CONFIG_3D_SLIDERSTATE * -2.0f;
		offset3D[1].level = CONFIG_3D_SLIDERSTATE * 2.0f;
		pp2d_draw_texture_part(inkedButtonTex, 40+offset3D[topfb].level+27, 99, 0, 0, 109, 34);
		pp2d_draw_texture_part(inkedButtonTex, 40+offset3D[topfb].level+27, 150, 0, 34, 109, 34);

		// "Memory Card" text
		if (menu_cursor == 0) {
			pp2d_draw_texture_part_blend(memCardCdTextTex, 40+13, 109, 0, 0, 140, 15, RGBA8(menu_textFadeColor, menu_textFadeColor, menu_textFadeColor, 255));
		} else {
			offset3D[0].level = CONFIG_3D_SLIDERSTATE * -1.0f;
			offset3D[1].level = CONFIG_3D_SLIDERSTATE * 1.0f;
			pp2d_draw_texture_part_scale(memCardCdTextTex, 40+offset3D[topfb].level+26, 110, 0, 0, 140, 15, 0.82, 0.82);
		}

		// "CD Player" text
		if (menu_cursor == 1) {
			pp2d_draw_texture_part_blend(memCardCdTextTex, 40+32, 159, 0, 15, 140, 15, RGBA8(menu_textFadeColor, menu_textFadeColor, menu_textFadeColor, 255));
		} else {
			offset3D[0].level = CONFIG_3D_SLIDERSTATE * -1.0f;
			offset3D[1].level = CONFIG_3D_SLIDERSTATE * 1.0f;
			pp2d_draw_texture_part_scale(memCardCdTextTex, 40+offset3D[topfb].level+42, 160, 0, 15, 140, 15, 0.82, 0.82);
		}

		if (!menu_textFade) {
			offset3D[0].level = CONFIG_3D_SLIDERSTATE * 1.0f;
			offset3D[1].level = CONFIG_3D_SLIDERSTATE * -1.0f;
			if (menu_cursor == 0) {
				pp2d_draw_texture(cursorTex, 40+offset3D[topfb].level+83, 118);
			} else if (menu_cursor == 1) {
				pp2d_draw_texture(cursorTex, 40+offset3D[topfb].level+83, 168);
			}
		}
	}
}