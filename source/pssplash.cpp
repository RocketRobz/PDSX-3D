#include <3ds.h>
#include "graphic.h"
#include "pp2d/pp2d.h"
#include "sound.h"
#include "settings.h"

#include "scesplash.h"


extern int psConsoleModel;					// 0 = Playstation -> PS, 1 = PSone

extern sound *bgm_playstation;

static bool ps_music = false;
static bool ps_fadedin = false;

static int ps_logoFadeColor = 0;	// 255 when faded in
static int ps_textFadeAlpha = 0;	// 255 when faded in

void psSplashInit(void) {
	ps_music = false;
	ps_fadedin = false;

	ps_logoFadeColor = 0;	// 255 when faded in
	ps_textFadeAlpha = 0;	// 255 when faded in
}

void psSplash(void) {
	if (!ps_music) {
		bgm_playstation->play();
		ps_music = true;
	}

	ps_logoFadeColor += 8;
	if (ps_logoFadeColor >= 255) {
		ps_logoFadeColor = 255;
	}
	
	if (ps_logoFadeColor == 255) {
		ps_textFadeAlpha += 8;
		if (ps_textFadeAlpha >= 255) {
			ps_textFadeAlpha = 255;
		}
	}
}

void psGraphicDisplay(int topfb) {
	pp2d_draw_texture_blend(psSTex, 40+107, 82, RGBA8(ps_logoFadeColor, ps_logoFadeColor, ps_logoFadeColor, 255));
	pp2d_draw_texture_blend(psPTex, 40+147, 33, RGBA8(ps_logoFadeColor, ps_logoFadeColor, ps_logoFadeColor, 255));
	pp2d_draw_texture_blend(psTextTex, 40+115, 136, RGBA8(255, 255, 255, ps_textFadeAlpha));
	if (ps_logoFadeColor == 255) {
		pp2d_draw_texture(psLicensedByTextTex, 40+132, 167);
		pp2d_draw_texture(psSonyComputerEntertainmentTextTex, 40+61, 177);
		pp2d_draw_texture(psSceTextTex, 40+146, 197);
	}
}