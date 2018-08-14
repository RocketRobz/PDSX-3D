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

size_t homeicontex = 1;

size_t sceTriangleTex = 2;
size_t sceLogoTex = 3;

void graphicsInit(void) {
	pp2d_load_texture_png(homeicontex, "romfs:/graphics/vcmenu/home_icon.png");
	
	pp2d_load_texture_png(sceTriangleTex, "romfs:/graphics/sce/triangle.png");
	pp2d_load_texture_png(sceLogoTex, "romfs:/graphics/sce/logo (OG PS1).png");
}
