#include <3ds.h>
#include "graphic.h"
#include "pp2d/pp2d.h"
#include "sound.h"
#include "settings.h"

#include "scesplash.h"

#define CONFIG_3D_SLIDERSTATE (*(float *)0x1FF81080)


extern int psConsoleModel;					// 0 = Playstation (SCPH1001) -> PS, 1 = PSone (SCPH101)

extern sound *bgm_playstation;

static bool ps_music = false;
static bool ps_fadedin = false;

static int ps_logoFadeAlpha = 255;	// 0 when faded in
static int ps_textFadeAlpha = 0;	// 255 when faded in

void psSplashInit(void) {
	ps_music = false;
	ps_fadedin = false;

	ps_logoFadeAlpha = 255;	// 0 when faded in
	ps_textFadeAlpha = 0;	// 255 when faded in
}

void psSplash(void) {
	if (!ps_music) {
		bgm_playstation->play();
		ps_music = true;
	}

	ps_logoFadeAlpha -= 8;
	if (ps_logoFadeAlpha <= 0) {
		ps_logoFadeAlpha = 0;
	}
	
	if (ps_logoFadeAlpha == 0) {
		ps_textFadeAlpha += 8;
		if (ps_textFadeAlpha >= 255) {
			ps_textFadeAlpha = 255;
		}
	}
}

void psGraphicDisplay(int topfb) {
	offset3D[0].level = CONFIG_3D_SLIDERSTATE * -4.0f;
	offset3D[1].level = CONFIG_3D_SLIDERSTATE * 4.0f;
	pp2d_draw_texture_part(psSTex, 40+offset3D[topfb].level+107, 82, 0, 0, 104, 6);

	offset3D[0].level = CONFIG_3D_SLIDERSTATE * -3.5f;
	offset3D[1].level = CONFIG_3D_SLIDERSTATE * 3.5f;
	pp2d_draw_texture_part(psSTex, 40+offset3D[topfb].level+107, 82+(6), 0, 6, 64, 6);

	offset3D[0].level = CONFIG_3D_SLIDERSTATE * -3.0f;
	offset3D[1].level = CONFIG_3D_SLIDERSTATE * 3.0f;
	pp2d_draw_texture_part(psSTex, 40+offset3D[topfb].level+107+64, 82+(6), 64, 6, 40, 6);
	pp2d_draw_texture_part(psSTex, 40+offset3D[topfb].level+107, 82+(6*2), 0, 6*2, 104, 6);

	offset3D[0].level = CONFIG_3D_SLIDERSTATE * -2.5f;
	offset3D[1].level = CONFIG_3D_SLIDERSTATE * 2.5f;
	pp2d_draw_texture_part(psSTex, 40+offset3D[topfb].level+107, 82+(6*3), 0, 6*3, 104, 6);

	offset3D[0].level = CONFIG_3D_SLIDERSTATE * -2.0f;
	offset3D[1].level = CONFIG_3D_SLIDERSTATE * 2.0f;
	pp2d_draw_texture_part(psSTex, 40+offset3D[topfb].level+107, 82+(6*4), 0, 6*4, 104, 6);

	offset3D[0].level = CONFIG_3D_SLIDERSTATE * -1.5f;
	offset3D[1].level = CONFIG_3D_SLIDERSTATE * 1.5f;
	pp2d_draw_texture_part(psSTex, 40+offset3D[topfb].level+107, 82+(6*5), 0, 6*5, 104, 6);
	pp2d_draw_texture_part(psSTex, 40+offset3D[topfb].level+107, 82+(6*6), 0, 6*6, 52, 6);

	offset3D[0].level = CONFIG_3D_SLIDERSTATE * -1.0f;
	offset3D[1].level = CONFIG_3D_SLIDERSTATE * 1.0f;
	pp2d_draw_texture_part(psSTex, 40+offset3D[topfb].level+107+52, 82+(6*6), 52, 6*6, 52, 6);

	offset3D[0].level = CONFIG_3D_SLIDERSTATE * -0.5f;
	offset3D[1].level = CONFIG_3D_SLIDERSTATE * 0.5f;
	pp2d_draw_texture_part(psSTex, 40+offset3D[topfb].level+107, 82+(6*7), 0, 6*7, 104, 6);
	pp2d_draw_texture_part(psPTex, 40+offset3D[topfb].level+147, 33, 0, 0, 10, 98);

	pp2d_draw_texture_part(psPTex, 40+147+9, 33, 9, 0, 28, 98);

	offset3D[0].level = CONFIG_3D_SLIDERSTATE * 0.5f;
	offset3D[1].level = CONFIG_3D_SLIDERSTATE * -0.5f;
	pp2d_draw_texture_part(psPTex, 40+offset3D[topfb].level+147+9+28, 33, 37, 0, 19, 98);

	if (psConsoleModel == 1) {
		pp2d_draw_texture_part_blend(psTextTex, 40+114, 136, 0, 20, 104, 20, RGBA8(255, 255, 255, ps_textFadeAlpha));
	} else {
		pp2d_draw_texture_part_blend(psTextTex, 40+115, 136, 0, 0, 104, 20, RGBA8(255, 255, 255, ps_textFadeAlpha));
	}
	if (ps_logoFadeAlpha == 0) {
		offset3D[0].level = CONFIG_3D_SLIDERSTATE * -2.0f;
		offset3D[1].level = CONFIG_3D_SLIDERSTATE * 2.0f;
		pp2d_draw_texture(psLicensedByTextTex, 40+offset3D[topfb].level+132, 167);
		pp2d_draw_texture(psSonyComputerEntertainmentTextTex, 40+offset3D[topfb].level+61, 177);

		offset3D[0].level = CONFIG_3D_SLIDERSTATE * -1.0f;
		offset3D[1].level = CONFIG_3D_SLIDERSTATE * 1.0f;
		pp2d_draw_texture(psSceTextTex, 40+offset3D[topfb].level+146, 197);
	}
	if (ps_logoFadeAlpha > 0) pp2d_draw_rectangle(0, 0, 400, 240, RGBA8(0, 0, 0, ps_logoFadeAlpha));
}