#include <3ds.h>
#include "graphic.h"
#include "pp2d/pp2d.h"
#include "sound.h"
#include "settings.h"

#include "scesplash.h"

extern int gameMode;
extern int modeOrder;

extern int psConsoleModel;					// 0 = Playstation -> PS, 1 = PSone

extern sound *sfx_sce;

static bool sce_music = false;
static int sce_fadeTime[2] = {0};
static bool sce_fadedin = false;

static int sce_bgColor = 0;	// 185 when faded in

static int sce_triangle1_x = 40+97;
static int sce_triangle1_y = 57;
static int sce_triangle2_x = 40+160;
static int sce_triangle2_y = 57;

static float sce_triangle_scale = 1.00;

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

	sce_triangle1_x = 40+97;
	sce_triangle1_y = 57;
	sce_triangle2_x = 40+160;
	sce_triangle2_y = 57;

	sce_triangle_scale = 1.00;

	sce_triangle_zoomdelay[0] = 0;
	sce_triangle_zoomdelay[1] = 0;
	sce_triangle_zoomdelay[2] = 0;
	sce_triangle_zoomdelay[3] = 0;
	sce_triangle_zoomdelay2AddDelay = false;

	sce_trianglesFormed = false;

	sce_loopOnLogo = 0;
}

void sceGraphicDisplay(void) {
	for (int topfb = GFX_LEFT; topfb <= GFX_RIGHT; topfb++) {
		if (topfb == GFX_LEFT) pp2d_begin_draw(GFX_TOP, (gfx3dSide_t)topfb);
		else pp2d_draw_on(GFX_TOP, (gfx3dSide_t)topfb);
		pp2d_draw_rectangle(0, 0, 400, 240, RGBA8(sce_bgColor, sce_bgColor, sce_bgColor, 255)); // Fade in/out effect
		if (settings.pseudoEmulation.border == 1) {
			pp2d_draw_rectangle(0, 0, 40, 240, RGBA8(0, 0, 0, 255));
			pp2d_draw_rectangle(360, 0, 40, 240, RGBA8(0, 0, 0, 255));
		}
		if (sce_fadedin) {
			pp2d_draw_texture(sceTriangleTex, 40+97, 57);
			pp2d_draw_texture_flip(sceTriangleTex, 40+160, 57, HORIZONTAL);
			pp2d_draw_texture_scale_flip(sceTriangleTex, sce_triangle2_x, sce_triangle2_y, sce_triangle_scale, sce_triangle_scale, HORIZONTAL);
			pp2d_draw_texture_scale(sceTriangleTex, sce_triangle1_x, sce_triangle1_y, sce_triangle_scale, sce_triangle_scale);
			if (sce_trianglesFormed) {
				pp2d_draw_texture_part(sceLogoTex, 40+100, 29, 0, 0, 120, 24);		// Sony
				pp2d_draw_texture_part(sceLogoTex, 40+100, 193, 0, 24, 120, 28);	// Computer Entertainment
			}
		}
	}
	pp2d_end_draw();
}

void sceSplash(void) {
	sceGraphicDisplay();

	if (!sce_music) {
		sfx_sce->play();
		sce_music = true;
	}

	if (!sce_fadedin) {
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
			}
		}

	}

	sce_loopOnLogo++;

	/*if (sce_loopOnLogo == 60*6) {
		sce_fadedin = false;
		if (modeOrder == 1) {
			gameMode = 2;	// Go to Main Menu
		} else {
			for (int i = 185; i >= 0; i -= 10) {
				sce_bgColor = i;
				swiWaitForVBlank();
			}
			sce_bgColor = 0;
			gameMode = 1;	// Go to PSX splash
		}
	}*/
}
