#include "graphic.h"

void pp2d_draw_texture_scale_flip(size_t id, int x, int y, float scaleX, float scaleY, flipType fliptype)
{
	pp2d_texture_select(id, x, y);
	pp2d_texture_flip(fliptype);
	pp2d_texture_scale(scaleX, scaleY);
	pp2d_texture_draw();		
}

void pp2d_draw_texture_scale_blend(size_t id, int x, int y, float scaleX, float scaleY, u32 color)
{
	pp2d_texture_select(id, x, y);
	pp2d_texture_blend(color);
	pp2d_texture_scale(scaleX, scaleY);
	pp2d_texture_draw();		
}

void pp2d_draw_texture_part_blend(size_t id, int x, int y, int xbegin, int ybegin, int width, int height, u32 color)
{
	pp2d_texture_select_part(id, x, y, xbegin, ybegin, width, height);
	pp2d_texture_blend(color);
	pp2d_texture_draw();		
}

void pp2d_draw_texture_part_scale(size_t id, int x, int y, int xbegin, int ybegin, int width, int height, float scaleX, float scaleY)
{
	pp2d_texture_select_part(id, x, y, xbegin, ybegin, width, height);
	pp2d_texture_scale(scaleX, scaleY);
	pp2d_texture_draw();
}

void pp2d_draw_texture_part_scale_blend(size_t id, int x, int y, int xbegin, int ybegin, int width, int height, float scaleX, float scaleY, u32 color)
{
	pp2d_texture_select_part(id, x, y, xbegin, ybegin, width, height);
	pp2d_texture_blend(color);
	pp2d_texture_scale(scaleX, scaleY);
	pp2d_texture_draw();		
}

void pp2d_draw_texture_rotate_flip_blend(size_t id, int x, int y, float angle, flipType fliptype, u32 color)
{
	pp2d_texture_select(id, x, y);
	pp2d_texture_rotate(angle);
	pp2d_texture_flip(fliptype);
	pp2d_texture_blend(color);
	pp2d_texture_draw();
}

size_t homeiconTex = 1;
size_t vcMenuBgTex = 2;
size_t vcMenuBubbleTex[2] = {3, 4};
size_t psoneBorderTex = 5;

size_t sceTriangleTex = 10;
size_t sceLogoTex = 11;

size_t psPTex = 12;
size_t psSTex = 13;
size_t psTextTex = 14;
size_t psLicensedByTextTex = 15;
size_t psSonyComputerEntertainmentTextTex = 16;
size_t psSceTextTex = 17;

size_t blueMenuBgTex = 18;
size_t blueMainMenuTextTex = 19;
size_t blueOrbsTex = 20;
size_t inkedButtonTex = 21;
size_t memCardCdTextTex = 22;
size_t gridBgTex = 23;
size_t cursorTex = 24;

void graphicsInit(void) {
	pp2d_load_texture_png(homeiconTex, "romfs:/graphics/vcmenu/home_icon.png");
	pp2d_load_texture_png(vcMenuBgTex, "romfs:/graphics/vcmenu/bg.png");
	pp2d_load_texture_png(vcMenuBubbleTex[0], "romfs:/graphics/vcmenu/Bubble 1.png");
	pp2d_load_texture_png(vcMenuBubbleTex[1], "romfs:/graphics/vcmenu/Bubble 2.png");
	pp2d_load_texture_png(psoneBorderTex, "romfs:/graphics/psone border.png");
	
	pp2d_load_texture_png(sceTriangleTex, "romfs:/graphics/sce/triangle.png");
	pp2d_load_texture_png(sceLogoTex, "romfs:/graphics/sce/logo.png");

	pp2d_load_texture_png(psPTex, "romfs:/graphics/playstation/P.png");
	pp2d_load_texture_png(psSTex, "romfs:/graphics/playstation/S.png");
	pp2d_load_texture_png(psTextTex, "romfs:/graphics/playstation/text.png");
	pp2d_load_texture_png(psLicensedByTextTex, "romfs:/graphics/playstation/text_licensedby.png");
	pp2d_load_texture_png(psSonyComputerEntertainmentTextTex, "romfs:/graphics/playstation/text_sonyComputerEntertainment.png");
	pp2d_load_texture_png(psSceTextTex, "romfs:/graphics/playstation/text_sce.png");

	pp2d_load_texture_png(blueMenuBgTex, "romfs:/graphics/mainmenu/bg_blue.png");
	pp2d_load_texture_png(blueMainMenuTextTex, "romfs:/graphics/mainmenu/text_mainmenu.png");
	pp2d_load_texture_png(blueOrbsTex, "romfs:/graphics/mainmenu/blueorbs.png");
	pp2d_load_texture_png(inkedButtonTex, "romfs:/graphics/mainmenu/inkedbutton.png");
	pp2d_load_texture_png(memCardCdTextTex, "romfs:/graphics/mainmenu/menutext.png");
	pp2d_load_texture_png(gridBgTex, "romfs:/graphics/mainmenu/bg_grid.png");
	pp2d_load_texture_png(cursorTex, "romfs:/graphics/mainmenu/cursor.png");
}
