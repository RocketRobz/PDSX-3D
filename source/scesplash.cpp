#include <3ds.h>
#include "graphic.h"
#include "pp2d/pp2d.h"
#include "sound.h"
#include "settings.h"

#include "scesplash.h"

#define CONFIG_3D_SLIDERSTATE (*(float *)0x1FF81080)


extern int gameMode;

extern int psConsoleModel;					// 0 = Playstation (SCPH1001) -> PS, 1 = PSone (SCPH101)

extern sound *bgm_sce;

static bool sce_music = false;
static int sce_fadeTime[2] = {0};
static bool sce_fadedin = false;

static int sce_bgColor = 0;	// 185 when faded in
static int sce_textFadeAlpha = 255;	// 0 when faded in

static int sce_triangle1_x = 40+97;
static int sce_triangle1_y = 57;
static int sce_triangle2_x = 40+160;
static int sce_triangle2_y = 57;

static float sce_triangle_scale = 1.00;

static float sce_triangle_depth = 0.0f;

static int sce_triangle_zoomdelay[4] = {0};
static bool sce_triangle_zoomdelay2AddDelay = false;

static bool sce_trianglesFormed = false;

static int sce_loopOnLogo = 0;

void sceInit(void) {
	sce_music = false;
	sce_fadeTime[0] = 0;
	sce_fadeTime[1] = 0;
	sce_fadedin = false;

	sce_bgColor = 0;	// 185 when faded in
	sce_textFadeAlpha = 255;	// 0 when faded in

	sce_triangle1_x = 40+97;
	sce_triangle1_y = 57;
	sce_triangle2_x = 40+160;
	sce_triangle2_y = 57;

	sce_triangle_scale = 1.00;
	
	sce_triangle_depth = 0.0f;

	sce_triangle_zoomdelay[0] = 0;
	sce_triangle_zoomdelay[1] = 0;
	sce_triangle_zoomdelay[2] = 0;
	sce_triangle_zoomdelay[3] = 0;
	sce_triangle_zoomdelay2AddDelay = false;

	sce_trianglesFormed = false;

	sce_loopOnLogo = 0;
}

void sceSplash(void) {
	if (!sce_music) {
		bgm_sce->play();
		sce_music = true;
	}

	if (!sce_fadedin && sce_loopOnLogo < 60*2) {
		if (sce_fadeTime[0] == 10) {
			if (sce_fadeTime[1] == 60) {
				sce_fadedin = true;
			} else {
				sce_fadeTime[1]++;
				sce_bgColor += 3;
			}
		} else {
			sce_fadeTime[0]++;
		}
	} else if (sce_loopOnLogo >= 60*6) {
		sce_fadedin = false;
		if (settings.pseudoEmulation.modeOrder == 1) {
			gameMode = 2;	// Go to Main Menu
		} else {
			sce_bgColor -= 10;
			if (sce_bgColor < 0) {
				sce_bgColor = 0;
				gameMode = 1;	// Go to Playstation splash
			}
		}
	} else {
		sce_bgColor = 185;

		sce_triangle_zoomdelay[0]++;
		if (sce_triangle_zoomdelay[0] == 2) {
			sce_triangle_zoomdelay[0] = 0;

			if (sce_triangle1_x != 40+136) {
				sce_triangle1_x++;
				sce_triangle_scale -= 0.015;
			}
			if (sce_triangle2_x != 40+156) {
				sce_triangle2_x--;
			}
		}

		sce_triangle_zoomdelay[1]++;
		if (sce_triangle_zoomdelay[1] == 3) {
			sce_triangle_zoomdelay[1] = 0;

			if (sce_triangle1_y != 70) {
				sce_triangle1_y++;
			}
		}

		sce_triangle_zoomdelay[2]++;
		if (sce_triangle_zoomdelay[2] == 1+sce_triangle_zoomdelay2AddDelay) {
			sce_triangle_zoomdelay[2] = 0;
			sce_triangle_zoomdelay2AddDelay = !sce_triangle_zoomdelay2AddDelay;

			if (sce_triangle2_y != 117) {
				sce_triangle2_y++;
			} else {
				sce_trianglesFormed = true;
			}
		}

		sce_triangle_zoomdelay[3]++;
		if (sce_triangle_zoomdelay[3] == 10) {
			sce_triangle_zoomdelay[3] = 0;

			if (sce_triangle2_y != 117) {
				sce_triangle2_y++;
				sce_triangle_depth += 0.2;
			}
		}
		
		if (sce_trianglesFormed) {
			sce_textFadeAlpha -= 15;
			if (sce_textFadeAlpha <= 0) sce_textFadeAlpha = 0;
		}

	}

	sce_loopOnLogo++;
}

void sceGraphicDisplay(int topfb) {
	pp2d_draw_rectangle(0, 0, 400, 240, RGBA8(sce_bgColor, sce_bgColor, sce_bgColor, 255)); // Fade in/out effect
	if (sce_fadedin) {
		offset3D[0].level = CONFIG_3D_SLIDERSTATE * -sce_triangle_depth;
		offset3D[1].level = CONFIG_3D_SLIDERSTATE * sce_triangle_depth;
		pp2d_draw_texture(sceTriangleTex, 40+97, 57);
		pp2d_draw_texture_flip(sceTriangleTex, 40+160, 57, HORIZONTAL);
		pp2d_draw_texture_scale_flip(sceTriangleTex, offset3D[topfb].level+sce_triangle2_x, sce_triangle2_y, sce_triangle_scale, sce_triangle_scale, HORIZONTAL);
		pp2d_draw_texture_scale(sceTriangleTex, offset3D[topfb].level+sce_triangle1_x, sce_triangle1_y, sce_triangle_scale, sce_triangle_scale);
		if (sce_trianglesFormed) {
			offset3D[0].level = CONFIG_3D_SLIDERSTATE * -1.0f;
			offset3D[1].level = CONFIG_3D_SLIDERSTATE * 1.0f;
			if (psConsoleModel == 1) {
				pp2d_draw_texture_part(sceLogoTex, 40+offset3D[topfb].level+110, 32, 0, 60, 100, 19);		// Sony
				pp2d_draw_texture_part(sceLogoTex, 40+offset3D[topfb].level+110, 188, 0, 80, 114, 23);	// Computer Entertainment (R)
				pp2d_draw_rectangle(40+offset3D[topfb].level+110, 32, 100, 19, RGBA8(sce_bgColor, sce_bgColor, sce_bgColor, sce_textFadeAlpha));
				pp2d_draw_rectangle(40+offset3D[topfb].level+110, 188, 114, 23, RGBA8(sce_bgColor, sce_bgColor, sce_bgColor, sce_textFadeAlpha));
			} else {
				pp2d_draw_texture_part(sceLogoTex, 40+offset3D[topfb].level+100, 29, 0, 0, 120, 24);		// Sony
				pp2d_draw_texture_part(sceLogoTex, 40+offset3D[topfb].level+100, 193, 0, 24, 120, 28);	// Computer Entertainment
				pp2d_draw_texture_part(sceLogoTex, 40+offset3D[topfb].level+170, 180, 0, 52, 12, 8);		// TM
			}
		}
	}
}