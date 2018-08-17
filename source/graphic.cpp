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
size_t vcMenuBubbleTex = 3;
size_t psoneBorderTex = 4;

size_t sceTriangleTex = 5;
size_t sceLogoTex = 6;

size_t psPTex = 7;
size_t psSTex = 8;
size_t psTextTex = 9;
size_t psLicensedByTextTex = 10;
size_t psSonyComputerEntertainmentTextTex = 11;
size_t psSceTextTex = 12;

size_t blueMenuBgTex = 13;
size_t blueMainMenuTextTex = 14;
size_t inkedButtonTex = 15;
size_t memCardCdTextTex = 16;
size_t cursorTex = 17;

void graphicsInit(void) {
	pp2d_load_texture_png(homeiconTex, "romfs:/graphics/vcmenu/home_icon.png");
	pp2d_load_texture_png(vcMenuBgTex, "romfs:/graphics/vcmenu/bg.png");
	pp2d_load_texture_png(vcMenuBubbleTex, "romfs:/graphics/vcmenu/Bubble 1.png");
	pp2d_load_texture_png(psoneBorderTex, "romfs:/graphics/psone border.png");
	
	pp2d_load_texture_png(sceTriangleTex, "romfs:/graphics/sce/triangle.png");
	pp2d_load_texture_png(sceLogoTex, "romfs:/graphics/sce/logo (OG PS1).png");

	pp2d_load_texture_png(psPTex, "romfs:/graphics/playstation/P.png");
	pp2d_load_texture_png(psSTex, "romfs:/graphics/playstation/S.png");
	pp2d_load_texture_png(psTextTex, "romfs:/graphics/playstation/text.png");
	pp2d_load_texture_png(psLicensedByTextTex, "romfs:/graphics/playstation/text_licensedby.png");
	pp2d_load_texture_png(psSonyComputerEntertainmentTextTex, "romfs:/graphics/playstation/text_sonyComputerEntertainment.png");
	pp2d_load_texture_png(psSceTextTex, "romfs:/graphics/playstation/text_sce.png");

	pp2d_load_texture_png(blueMenuBgTex, "romfs:/graphics/mainmenu/bg_blue.png");
	pp2d_load_texture_png(blueMainMenuTextTex, "romfs:/graphics/mainmenu/text_mainmenu.png");
	pp2d_load_texture_png(inkedButtonTex, "romfs:/graphics/mainmenu/inkedbutton.png");
	pp2d_load_texture_png(memCardCdTextTex, "romfs:/graphics/mainmenu/menutext.png");
	pp2d_load_texture_png(cursorTex, "romfs:/graphics/mainmenu/cursor.png");
}
