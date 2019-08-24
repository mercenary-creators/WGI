#include "driver.h"

Colormap cmap;

#define DRV_NUMBER_COLORS 16

static struct
{
	unsigned char r;
	unsigned char g;
	unsigned char b;

} rgb_colors[DRV_NUMBER_COLORS] = {

	{  0,   0,   0},
	{255,   0,   0},
	{  0, 255,   0},
	{  0,   0, 255},
	{150, 150,  60},
	{255,   0, 255},
	{  0, 255, 255},
	{255, 255, 255},
	{255, 255,   0},
	{255, 255,   0},
	{255, 255,   0},
	{255, 255,   0},
	{255, 255,   0},
	{255, 255,   0},
	{255, 255,   0},
	{255, 255,   0},
};

static long drv_pixel(color, indx, what)
XColor *color;
long indx;
long what;
{
	color->pixel = what;
	color->flags = (DoRed|DoGreen|DoBlue);
	color->red = (rgb_colors[indx].r << 8);
	color->blue = (rgb_colors[indx].b << 8);
	color->green = (rgb_colors[indx].g << 8);

	return (DRV_SUCCESS);
}

long drv_allocate_colormap(toplevel)
Widget toplevel;
{
	long indx;
	XColor colr;
	unsigned long planes[2];
	XColor list[DRV_NUMBER_COLORS];
	Display *disp = XtDisplay(toplevel);
	Window root = XDefaultRootWindow(disp);
   	Visual *visu = DefaultVisual(disp, DefaultScreen(disp));
	Colormap orig = DefaultColormap(disp, DefaultScreen(disp));

	cmap = XCreateColormap(disp, root, visu, AllocAll);

	for(colr.pixel = 0; colr.pixel < visu->map_entries; colr.pixel++)
	{
		XQueryColor(disp, orig, &colr);
		XStoreColor(disp, cmap, &colr);
	}

	for(indx = 0; indx < DRV_NUMBER_COLORS; indx++)
	{
		drv_pixel(&list[indx], indx, indx);
	}

	planes[0] = 0x7;
	planes[1] = 0x8;

	wgi_device_plane_table(planes, 2);

	XStoreColors(disp, cmap, list, DRV_NUMBER_COLORS);
	XSetWindowColormap(disp, XtWindow(toplevel), cmap);
/*	XInstallColormap(disp, cmap); */

	return (DRV_SUCCESS);
}
