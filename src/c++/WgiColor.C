#include "WgiColor.h"
#include "DebugFlag.h"
#include <Xm/XmStrDefs.h>	// for: XmNcolormap
#include <math.h>		// for: sqrt

DEBUG_FLAG(debug_wgi_color,"wgi_color");

WgiColorPlane::WgiColorPlane (WgiColorMap* map, int number_of_bits)
: priv_bits (number_of_bits), priv_map (map)
{
    map->addPlane (this);
}

void
WgiColorPlane::setLSB (int lsb)
{
    priv_lsb = lsb;
    priv_msb = lsb + bitCardinality() - 1;

    priv_mask = ((1 << bitCardinality()) - 1) << lsb;
}


//------------------------------------------------------------

String
WgiColor::rgb_name (unsigned short red, unsigned short green, 
	unsigned short blue)
{
    char buffer[20];
    sprintf (buffer, "#%04x%04x%04x", int(red), int(green), int(blue));
    return buffer;
}

WgiColor::WgiColor (unsigned char red, unsigned char green, unsigned char blue,
	const char* alias /*= NULL */)
: priv_red (red<<8), priv_green (green<<8), priv_blue (blue<<8)
{
    if (alias)
	priv_name = alias;
    else
	priv_name = rgb_name ();
}

WgiColor::WgiColor (unsigned short red, unsigned short green, 
	unsigned short blue, const char* alias /*= NULL */)
: priv_red (red), priv_green (green), priv_blue (blue)
{
    if (alias)
	priv_name = alias;
    else
	priv_name = rgb_name ();
}

WgiColor::WgiColor (const char* by_name, Display* display)
: priv_name (by_name), priv_red (0), priv_green (0), priv_blue (0)
{
    XColor exact_def;
    if (XParseColor(display, DefaultColormap (display, 
	DefaultScreen (display)), by_name, &exact_def))
    {
	priv_red = exact_def.red;
	priv_green = exact_def.green;
	priv_blue = exact_def.blue;
    }
    else 
    {
	WGI_ERROR ("WgiColor::WgiColor", 
		form("WgiColor(%s) not found\n", by_name));
    }
}

inline
double sq (double d)
{
    return d * d;
}

double
WgiColor::intensityFactor () const
{
    return sqrt (sq (.30 * red()) + sq (.59 * green()) + sq (.11 * blue()));
}

/* static */ Boolean
WgiColor::nameExists (const char* name, Display* display)
{
    XColor exact_def;
    return Boolean (XParseColor(display, DefaultColormap (display, 
	DefaultScreen (display)), name, &exact_def));
}

//------------------------------------------------------------

WgiColorMap::WgiColorMap (Display* display, int max_size)
: priv_display(display), priv_plane_card(0), priv_max_size(max_size),
  priv_freed (FALSE)
{
    Window root = XDefaultRootWindow (display);
    Visual *visual = defaultVisual ();
    Colormap original = defaultColorMap ();
    priv_cmap = XCreateColormap (display, root, visual, AllocAll);

    // Copy the colors from the Default Colormap.
    XColor color;
    for (color.pixel = 0; color.pixel < visual->map_entries; color.pixel++)
    {
	XQueryColor (display, original, &color);
	XStoreColor (display, priv_cmap, &color);
    }

    for (int i = 0; i < WGI_COLORMAP_BITS; i++)
	priv_planes[i] = NULL;
}

void
WgiColorMap::recopyUnusedColors ()
{
    Colormap original = defaultColorMap ();
    Visual* visual = defaultVisual ();

    XColor col;
    for (col.pixel = size(); col.pixel < visual->map_entries; col.pixel++)
    {
	XQueryColor (display(), original, &col);
	XStoreColor (display(), priv_cmap, &col);
    }
}

void
WgiColorMap::copyRGB ()
{
    XColor colors [WGI_COLORMAP_SIZE];
    for (int i = 0; i < size(); i++)
    {
	colors[i].pixel = i;
	colors[i].flags = DoRed | DoGreen | DoBlue;
	colors[i].red = priv_colors[i].red();
	colors[i].blue = priv_colors[i].blue();
	colors[i].green = priv_colors[i].green();
    }
    XStoreColors (display(), colorMap(), colors, size());
}

void
WgiColorMap::setWindowColorMap (Window w) const
{
    // NOTE: never use XInstallColormap ! Let the window manager do it.
    XSetWindowColormap (display(), w, colorMap());
}

void
WgiColorMap::setWidgetColorMap (Widget w) const
{
    // Don't set XmNcolormap resource of the TopLevelShell Widget !
    // If you do, the application will try to allocate colors from
    //  this Colormap when creating new Widgets and then save_colors
    //  or ids_cmap won't save these colors. Also, you will get errors
    //  when it tries to allocate colors for any Widget that
    //  is created after the resource is set, e.g.
    //
    // Warning: Cannot allocate colormap entry for "black"
    // Warning: Cannot allocate colormap entry for default background
    // Warning: Cannot allocate colormap entry for "white"

// The following 3 lines show how it should NOT be done:
//    Arg args[1];
//    XtSetArg (args[0], XmNcolormap, colorMap());
//    XtSetValues (w, args, 1);

    setWindowColorMap (XtWindow(w));
}

void
WgiColorMap::install (Boolean installPlaneTable /*= TRUE */)
{
    copyRGB ();

    if (installPlaneTable)
    {
	unsigned long table [WGI_VIRTUAL_COLORMAP_SIZE];
	for (int i = 0; i < planeCardinality(); i++)
	{
	    table[i] = plane(i)->mask ();
	}
	wgi_device_plane_table (table, planeCardinality());
    }
}

WgiColorMap::~WgiColorMap ()
{
    free ();

    // Destroy the WgiColorPlanes
    for (int i = 0; i < planeCardinality(); i++)
	delete priv_planes[i];
}

void
WgiColorMap::free ()
{
    if (!priv_freed)
    {
	XFreeColormap (display(), colorMap());
	priv_freed = TRUE;
    }
}

Boolean 
WgiColorMap::isValid (WgiColorIndex color_index) const
{
    return Boolean (color_index >= 0 && color_index < size());
}

Boolean
WgiColorMap::setColor (WgiColorIndex color_index, const WgiColor& col)
{
    if (isValid (color_index))
    {
	priv_colors[color_index] = col;
	return TRUE;
    }
    return FALSE;
}

Boolean
WgiColorMap::setColor (WgiPlaneIndex plane_index, WgiColorIndex color_index,
	const WgiColor& col)
{
    WgiColorPlane* pl = plane (plane_index);
    if (pl)
    {
	// e.g. LSB=4, color_index=2 : set colors 32-47

	WgiColorIndex start = color_index << pl->LSB();
	WgiColorIndex end = (color_index + 1) << pl->LSB();

	if (!isValid(start) || !isValid(end-1))
	    return FALSE;

	for (WgiColorIndex i = start; i < end; i++)
	{
	    setColor (i, col);
	}
	return TRUE;
    }
    else
    {
	WGI_ERROR("WgiColorMap::setColor", "invalid plane_index");
	return FALSE;
    }
}

const WgiColor& 
WgiColorMap::color (WgiPlaneIndex plane_index, WgiColorIndex color_index) const
{
    WgiColorPlane* pl = plane (plane_index);
    if (pl)   
    {
	WgiColorIndex start_index = color_index << pl->LSB();
	return color (start_index);
    }
    else
    {
	WGI_ERROR("WgiColorMap::color", "invalid plane_index");
	return color (0);
    }
}

void
WgiColorMap::swapColors (WgiColorIndex color_index1, 
	WgiColorIndex color_index2, Boolean install_it /*= TRUE */)
{
    if (color_index1 != color_index2)
    {
	WgiColor tmp = color (color_index1);
	setColor (color_index1, color (color_index2));
	setColor (color_index2, tmp);
    }
    if (install_it)
	install (FALSE);	// NOTE: plane table is already installed
}

void
WgiColorMap::addPlane (WgiColorPlane* pl)
{
    priv_planes [priv_plane_card] = pl;
    if (priv_plane_card == 0)
    {
	pl->setLSB (0);
    }
    else
	pl->setLSB (plane (priv_plane_card-1)->MSB() + 1);

    pl->priv_index = priv_plane_card++;
}

WgiPlaneMask
WgiColorMap::on () const
{
    return ~0;
// ?? 0xFF should probably be sufficient !
}

WgiPlaneMask 
WgiColorMap::on (WgiPlaneIndex index) const
{
    return plane(index)->mask();
}

WgiColorPlane*
WgiColorMap::plane (WgiPlaneIndex index) const
{
    if (index >= 0 && index < planeCardinality())
	return priv_planes[index];
    else
	return NULL;
}

int 
WgiColorMap::bitsUsed () const
{
    return planeCardinality() == 0 ? 0 : plane(planeCardinality()-1)->MSB()+1;
}

Boolean 
WgiColorMap::findColorByName (const char* color_name, WgiColorIndex* p_index,
		WgiColorIndex start_index /*= 0 */) const
{
    for (WgiColorIndex i = start_index; i < size(); i++)
    {
	if (priv_colors[i].name() == color_name)
	{
	    if (p_index) *p_index = i;
	    return TRUE;
	}
    }
    return FALSE;
}

void
WgiColorMap::debug (FILE* fp /*= stderr*/)
{
    fprintf (fp, "=> WgiColorMap (this=0x%p, size=%d, #planes=%d)\n",
	this, size(), planeCardinality());

    for (int i = 0; i < planeCardinality(); i++)
    {
	WgiColorPlane* pl = plane(i);
	fprintf (fp, "\n==> WgiColorPlane[%d]: bits=%d, L/MSB=%d/%d, "
		"mask=0x%02x\n",
		pl->index(), pl->bitCardinality(), pl->LSB(),
		pl->MSB(), pl->mask());

	for (int j = 0; j < pl->colorCardinality(); j++)
	{
	    fprintf (fp, "[%d] ", j);

	    int start = (j << pl->LSB());
	    int end = ((j+1) << pl->LSB()) - 1;
	    if (i == 0 || j != 0)
	    {
		fprintf (fp, "%-15s %-13s", (const char*) color(start).name(),
			(const char*) color(start).rgb_name());
	    }
	    else
	    {
		fprintf (fp, "-- %-23s --", "BACKGROUND");
	    }
	    if (start == end)
		fprintf (fp, "  (%d)\n", start);
	    else
		fprintf (fp, "  (%d-%d)\n", start, end);
	}
    }
}

//============================================================

WgiVirtualColorMap::WgiVirtualColorMap (int size)
: priv_size (size > WGI_VIRTUAL_COLORMAP_SIZE ?
		WGI_VIRTUAL_COLORMAP_SIZE : size)
{
    if (size > WGI_VIRTUAL_COLORMAP_SIZE)
    {
	WGI_ERROR ("WgiVirtualColorMap::WgiVirtualColorMap",
		form("size = %d (must be <= %d)", size,
		WGI_VIRTUAL_COLORMAP_SIZE));
    }

    for (int i = 0; i < WGI_VIRTUAL_COLORMAP_SIZE; i++)
	priv_table[i] = i;
}

WgiColorIndex&
WgiVirtualColorMap::operator[] (WgiVirtualColorIndex i)
{
    if (!isValid(i))
    {
	WGI_ERROR ("WgiVirtualColorMap::operator[]", 
		form("invalid index %d", (int) i));
	i = 0;
    }

    return priv_table[i];
}

Boolean 
WgiVirtualColorMap::set (WgiVirtualColorIndex virtual_index, 
	WgiColorIndex index)
{
    if (isValid (virtual_index))
    {
	priv_table[virtual_index] = index;
	return TRUE;
    }
    return FALSE;
}

Boolean
WgiVirtualColorMap::setRange (WgiVirtualColorIndex start_index, 
	WgiVirtualColorIndex end_index, 
	WgiColorIndex start_color_table_index)
{
    if (!isValid(start_index) || !isValid(end_index))
	return FALSE;

    WgiColorIndex j = start_color_table_index;
    for (WgiVirtualColorIndex i = start_index; i <= end_index; i++, j++)
    {
	priv_table [i] = j;
    }

    return TRUE;
}

Boolean 
WgiVirtualColorMap::install () const
{
    return Boolean (wgi_device_color_table ((unsigned long*) priv_table, 
					    size ()) == WGI_SUCCESS);
}

Boolean
WgiVirtualColorMap::swapColors (WgiVirtualColorIndex c1, 
	WgiVirtualColorIndex c2, Boolean install_it /*= TRUE */)
{
    if (isValid(c1) && isValid(c2))
    {
	WgiColorIndex tmp = priv_table[c1];
	priv_table[c1] = priv_table[c2];
	priv_table[c2] = tmp;

	if (install_it)
	    return install();
	else
	    return TRUE;
    }
    return FALSE;
}

void
WgiVirtualColorMap::debug (FILE* fp /*= stderr =*/, 
			Boolean dontPrintSame /*= FALSE */)
{
    fprintf (fp, "=> WgiVirtualColorMap (this=0x%p)\n", this);
    for (int i = 0; i < 32; i++)
    {
	for (int j = 0; j < 256; j += 32)
	{
	    WgiVirtualColorIndex c = j + i;
	    if (j > 0) fprintf (fp, " ");
	    fprintf (fp, "[%3d] ", (int) c);

	    if (dontPrintSame && c == priv_table[c])
		fprintf (fp, "   ");
	    else
		fprintf (fp, "%3d", (int) priv_table[c]);
	}
	fprintf (fp, "\n");
    }
    fprintf (fp, "=> end: WgiVirtualColorMap (this=0x%p)\n", this);
}
