#include "DebugFlag.h"
#include "WgiDevice.h"

DEBUG_FLAG(debug_wgi_zoom, "wgi_zoom");

static double ZOOM_INACC = 0.000001;

#ifndef CC30
Mapimplement(WgiViewPortId,WgiViewPortP)
#endif

/* static */
const WgiVirtualExtent WgiVirtualExtent::domain
	(WgiVirtualCoordMin, WgiVirtualCoordMin,
	 WgiVirtualCoordMax, WgiVirtualCoordMax, TRUE);

inline double
max (double a, double b)
{
    return a > b ? a : b;
}

inline double
max (double a, double b, double c, double d)
{
    return max (max(a,b), max(c,d));
}

//============================================================

void
WgiDevice::useBackingStore ()
{
    XSetWindowAttributes attr;
    attr.backing_store = Always;
    XChangeWindowAttributes (display(), drawable(), CWBackingStore, &attr);
}

void
WgiDevice::view (WgiViewPortId id, const WgiWorldLoc* locs)
{
    wgi_viewport_view(id, (WLDcrd*) locs);
}

void
WgiDevice::erase_area (const WgiVirtualExtent& window, long mask)
{
    erase_area (window.LLx(), window.LLy(), window.width(), 
		window.height(), mask);
}

//============================================================

WgiViewPort::~WgiViewPort ()
{
    device()->removeViewPort(this);    
    wgi_viewport_delete(id());
}

void
WgiViewPort::view (const WgiWorldLoc& lower_left,
	const WgiWorldLoc& upper_right)
{
    priv_window.setLL(lower_left);
    priv_window.setUR(upper_right);

    device()->view (id(), priv_window);
}

void
WgiViewPort::view (const WgiWorldExtent& view_window)
{
    priv_window = view_window;
    device()->view (id(), priv_window);
}

void
WgiViewPort::view (const WLDcrd* view_window)
{
    priv_window = WgiWorldExtent (view_window);
    device()->view (id(), priv_window);
}

void
WgiViewPort::query_window (WgiWorldExtent* ext)
{
    Extent screen_extent, world_extent;

    wgi_viewport_query (id(), &world_extent, &screen_extent);
    ext->set (world_extent.xmin, world_extent.xmax,
		world_extent.ymin, world_extent.ymax, TRUE);
}

void
WgiViewPort::zoom (WgiZoomOper zoom_operation, double zoom_factor,
		const WgiWorldLoc* zoom_center)
{
    WgiWorldLoc zc = zoom_center ? *zoom_center : window().center();
    wgi_viewport_zoom (id(), &zc, zoom_operation, zoom_factor);
    query_window (&priv_window);
}

WgiWorldExtent
WgiViewPort::zoom_query (WgiZoomOper zoom_operation, double zoom_factor,
		const WgiWorldLoc* zoom_center)
{
    WgiWorldLoc zc = zoom_center ? *zoom_center : window().center();
    Extent ext;
    wgi_viewport_zoom_query (id(), &zc, zoom_operation, zoom_factor, &ext);
    return WgiWorldExtent (ext);
}

void
WgiViewPort::set (WgiDevice* device, WgiViewPortId id, 
		const WgiVirtualExtent& extent,
		const WgiWorldExtent& viewable_world)
{
    priv_id = id;
    priv_device = device;
    priv_extent = extent;
    setWorld(viewable_world);
    wgi_viewport_create (id, extent.x_min(), extent.y_min(),
	extent.width(), extent.height());
    device->addViewPort(this);    
}

WgiViewPort::WgiViewPort (WgiDevice* device, WgiViewPortId id, 
	WgiVirtualCoord xmin, WgiVirtualCoord ymin, 
	WgiVirtualCoord dx, WgiVirtualCoord dy,
	const WgiWorldExtent& viewable_world)
{
    set (device, id, WgiVirtualExtent(xmin, ymin, xmin+dx, ymin+dy,
	TRUE), viewable_world);
}

WgiViewPort::WgiViewPort (WgiDevice* device, WgiViewPortId id, 
	const WgiVirtualExtent& extent,
	const WgiWorldExtent& viewable_world)
{
    set (device, id, extent, viewable_world);
}

WgiVirtualLoc
WgiViewPort::convert (const WgiWorldLoc& world_loc) const
{
    VDCcrd crd[1];
    convert (1, (WLDcrd*) &world_loc, crd);
    return WgiVirtualLoc (crd[0]);
}

WgiWorldLoc 
WgiViewPort::convert (const WgiVirtualLoc& virtual_loc) const
{
    WLDcrd crd[1];
    convert (1, & (VDCcrd&) virtual_loc, crd);
    return WgiWorldLoc (crd[0]);
}

WgiWorldExtent
WgiViewPort::convert (const WgiVirtualExtent& virtual_extent) const
{
    WLDcrd crd[2];
    convert (2, (const VDCcrd*) virtual_extent, crd);
    // swap y_min / y_max !
    return WgiWorldExtent (crd[0].x, crd[1].y, crd[1].x, crd[0].y, TRUE);
}

WgiVirtualExtent
WgiViewPort::convert (const WgiWorldExtent& world_extent) const
{
    VDCcrd crd[2];
    convert (2, (const WgiWorldLoc*) world_extent, crd);
    // swap y_min / y_max !
    return WgiVirtualExtent (crd[0].x, crd[1].y, crd[1].x, crd[0].y, TRUE);
}

Boolean 
WgiViewPort::safe_convert (const WgiWorldLoc& world_loc, 
		WgiVirtualLoc* virtual_loc) const
{
    double x = double(world_loc.x - window().x_min()) * 
	double(extent().width()) / double(window().width()) +
	extent().x_min();

    if (x < double(WgiVirtualCoordMin)
     || x > double(WgiVirtualCoordMax)) return FALSE;

    double y = double(window().y_max() - world_loc.y) * 
	double(extent().height()) / double(window().height()) +
	extent().y_min();

    if (y < double(WgiVirtualCoordMin)
     || y > double(WgiVirtualCoordMax)) return FALSE;

    if (virtual_loc)
    {
	virtual_loc->setX (WgiVirtualCoord (x + 0.5));
	virtual_loc->setY (WgiVirtualCoord (y + 0.5));
    }
    return TRUE;
}

Boolean 
WgiViewPort::safe_convert (const WgiWorldExtent& world_extent, 
		WgiVirtualExtent* virtual_extent) const
{
    WgiVirtualLoc ll, ur;

    if (!safe_convert (world_extent.LL(), &ll)) return FALSE;
    if (!safe_convert (world_extent.UR(), &ur)) return FALSE;

    if (virtual_extent)
    {
	virtual_extent->set (ll, ur, FALSE);	// swap y_min/y_max !
    }
    return TRUE;
}

Boolean 
WgiViewPort::convert_anyway (const WgiWorldLoc& world_loc, 
		double* virtual_x, double* virtual_y,
		const WgiWorldExtent* view_win /*= NULL */) const
{
    Boolean result = TRUE;

    if (view_win == NULL)
	view_win = & window();

    double x = double(world_loc.x - view_win->x_min()) * 
	double(extent().width()) / double(view_win->width()) +
	extent().x_min();

    if (x < double(WgiVirtualCoordMin)
     || x > double(WgiVirtualCoordMax)) result = FALSE;

    double y = double(view_win->y_max() - world_loc.y) * 
	double(extent().height()) / double(view_win->height()) +
	extent().y_min();

    if (y < double(WgiVirtualCoordMin)
     || y > double(WgiVirtualCoordMax)) result = FALSE;

    if (virtual_x)
	*virtual_x = x;
    if (virtual_y)
	*virtual_y = y;

    return result;
}

//============================================================

static double MORE_THAN_ONE = 1.0001;
	// Multiplication factor for calculating minimum scales.
	// NOTE: this should prevent rounding/inaccuracy problems.

// Minimum scale at the right (in terms of xmin of view window).
// where: scale = vw.width / vp.width = vw.height / vp.height
// NOTE: this scale prevents overflows of x coordinates on the 
//  LEFT (minimum) side of the world.

double
WgiViewPort::min_scale_at_right_xmin (WgiWorldCoord vw_xmin) const
{
    return (vw_xmin - world().x_min()) * MORE_THAN_ONE /
	(extent().x_min() - WgiVirtualExtent::domain.x_min());
}

double
WgiViewPort::min_scale_at_right_xmax (WgiWorldCoord vw_xmax) const
{
    return (vw_xmax - world().x_min()) * MORE_THAN_ONE /
	(extent().x_max() - WgiVirtualExtent::domain.x_min());
}

double
WgiViewPort::min_scale_at_left_xmin (WgiWorldCoord vw_xmin) const
{
    return (world().x_max() - vw_xmin) * MORE_THAN_ONE /
	(WgiVirtualExtent::domain.x_max() - extent().x_min());
}

double
WgiViewPort::min_scale_at_bottom_ymin (WgiWorldCoord vw_ymin) const
{
    return (world().y_max() - vw_ymin) * MORE_THAN_ONE /
	(extent().y_max() - WgiVirtualExtent::domain.y_min());
}

double
WgiViewPort::min_scale_at_bottom_ymax (WgiWorldCoord vw_ymax) const
{
    return (world().y_max() - vw_ymax) * MORE_THAN_ONE /
	(extent().y_min() - WgiVirtualExtent::domain.y_min());
}

double
WgiViewPort::min_scale_at_top_ymin (WgiWorldCoord vw_ymin) const
{
    return (vw_ymin - world().y_min()) * MORE_THAN_ONE /
	(WgiVirtualExtent::domain.y_max() - extent().y_max());
}

double
WgiViewPort::min_scale_at_top_ymax (WgiWorldCoord vw_ymax) const
{
    return (vw_ymax - world().y_min()) * MORE_THAN_ONE /
	(WgiVirtualExtent::domain.y_max() - extent().y_min());
}

// Minimum scale at the right (in terms of x coord. of center of view window).
// where: scale = vw.width / vp.width = vw.height / vp.height
// NOTE: this scale prevents overflows of x coordinates on the 
//  LEFT (minimum) side of the world.

double
WgiViewPort::min_scale_at_right_zx (WgiWorldCoord zx) const
{
    return (zx - world().x_min()) * MORE_THAN_ONE /
	(extent().x_min() + extent().width() / 2 
	 - WgiVirtualExtent::domain.x_min());
}

double
WgiViewPort::min_scale_at_left_zx (WgiWorldCoord zx) const
{
    return (world().x_max() - zx) * MORE_THAN_ONE /
	(WgiVirtualExtent::domain.x_max()
	 - extent().x_min() - extent().width() / 2);
}

double
WgiViewPort::min_scale_at_bottom_zy (WgiWorldCoord zy) const
{
    return (world().y_max() - zy) * MORE_THAN_ONE /
	(extent().y_min() + extent().height() / 2
	 - WgiVirtualExtent::domain.y_min());
}

double
WgiViewPort::min_scale_at_top_zy (WgiWorldCoord zy) const
{
    return (zy - world().y_min()) * MORE_THAN_ONE /
	(WgiVirtualExtent::domain.y_max()
	 - extent().y_min() - extent().height() / 2);
}

// Minimum scale in the top left corner of the world.

double
WgiViewPort::min_scale_at_top_left () const
{
    return max (min_scale_at_left_xmin (world().x_min()),
		min_scale_at_top_ymax (world().y_max()));
}

double
WgiViewPort::min_scale_at_top_right () const
{
    return max (min_scale_at_right_xmax (world().x_max()),
		min_scale_at_top_ymax (world().y_max()));
}

double
WgiViewPort::min_scale_at_bottom_left () const
{
    return max (min_scale_at_left_xmin (world().x_min()),
		min_scale_at_bottom_ymin (world().y_min()));
}

double
WgiViewPort::min_scale_at_bottom_right () const
{
    return max (min_scale_at_right_xmax (world().x_max()),
		min_scale_at_bottom_ymin (world().y_min()));
}

Boolean
WgiViewPort::isValidViewWindow (const WgiWorldExtent& vw,
	WgiWorldExtent* modified_vw) const
{
    // First ensure that the view window has the same aspect ratio
    //  as the view port.
    WgiWorldExtent asp = vw;
    asp.aspectize (extent().aspect_ratio());

    // The view window must be inside the world.
    if (!world().contains (asp))
    {
DEBUG_PRINT (debug_wgi_zoom, "isValid:: contains(asp) = FALSE\n");
if (debug_wgi_zoom) debug_view_window (asp);
	return FALSE;
    }

    double x_min, y_min, x_max, y_max;
    convert_anyway (world().LL(), &x_min, &y_max, &asp);
    convert_anyway (world().UR(), &x_max, &y_min, &asp);

DEBUG_PRINT (debug_wgi_zoom, form ("world' = (%.9lG,%.9lG) (%.9lG,%.9lG)\n", x_min,
	y_min, x_max, y_max));

    Boolean ok = TRUE;
    if (x_min < WgiVirtualExtent::domain.x_min() - 0.5)
    {
DEBUG_PRINT (debug_wgi_zoom, "xmin TOO SMALL\n");
	ok = FALSE;
    }
    if (y_min < WgiVirtualExtent::domain.y_min() - 0.5)
    {
DEBUG_PRINT (debug_wgi_zoom, "ymin TOO SMALL\n");
	ok = FALSE;
    }
    if (x_max > WgiVirtualExtent::domain.x_max() + 0.5)
    {
DEBUG_PRINT (debug_wgi_zoom, "xmax TOO BIG\n");
	ok = FALSE;
    }
    if (y_max > WgiVirtualExtent::domain.y_max() + 0.5)
    {
DEBUG_PRINT (debug_wgi_zoom, "ymax TOO BIG\n");
	ok = FALSE;
    }

#ifdef SCALE_CHECK
    double scale = asp.width() / extent().width();
    // Check if this scale at this location does not
    //  cause overflow problems.
    double s = min_scale_at_right_xmin (asp.x_min());
    if (scale < s)
    {
fprintf(stderr, "scale=%.9lG right=%.9lG\n", scale, s);
	return FALSE;
    }
    s = min_scale_at_left_xmin (asp.x_min());
    if (scale < s)
    {
fprintf(stderr, "scale=%.9lG left=%.9lG\n", scale, s);
	return FALSE;
    }
    s = min_scale_at_top_ymin (asp.y_min());
    if (scale < s)
    {
fprintf(stderr, "scale=%.9lG top=%.9lG\n", scale, s);
	return FALSE;
    }
    s =  min_scale_at_bottom_ymin (asp.y_min());
    if (scale < s)
    {
fprintf(stderr, "scale=%.9lG bottom=%.9lG\n", scale, s);
	return FALSE;
    }
#endif
    if (ok == TRUE && modified_vw != NULL)
	*modified_vw = asp;

    return ok;
}

Boolean
WgiViewPort::smallestViewWindow (WgiWorldCoord zx, WgiWorldCoord zy,
	WgiWorldExtent* vw) const
{
    double scale = max (min_scale_at_right_zx (zx),
	min_scale_at_left_zx (zx),
	min_scale_at_top_zy (zy),
	min_scale_at_bottom_zy (zy));

    WgiWorldCoord width = scale * extent().width();
    WgiWorldCoord height = scale * extent().height();

    WgiWorldExtent w (zx - width / 2, zy - height / 2,
	zx + width / 2, zy + height / 2, TRUE);

    if (world().contains (w))
    {
	if (vw)
	    *vw = w;
	return TRUE;
    }

    return FALSE;
}

void
WgiViewPort::debug_view_window (const WgiWorldExtent& view_win, 
		FILE* fp /*= stderr */) const
{
    fprintf(fp, "-----------------------------------\n");
    fprintf (fp, "vw = (%.9lG,%.9lG) (%.9lG,%.9lG)\n", 
	(double) view_win.x_min(), (double) view_win.y_min(), 
	(double) view_win.x_max(), (double) view_win.y_max());

    WgiVirtualExtent ext = extent();
    fprintf (fp, "vp = (%hd,%hd) (%hd,%hd) ratio=%.9lG\n", 
	ext.x_min(), ext.y_min(), ext.x_max(),
	ext.y_max(), ext.aspect_ratio());

    // Aspectized.
    WgiWorldExtent asp = view_win;
    asp.aspectize (extent().aspect_ratio());
    fprintf (fp, "vw(asp) = (%.9lG,%.9lG) (%.9lG,%.9lG)\n", 
	(double) asp.x_min(), (double) asp.y_min(), 
	(double) asp.x_max(), (double) asp.y_max());

    fprintf (fp, "world = (%.9lG,%.9lG) (%.9lG,%.9lG)  contains=%d\n", 
	(double) world().x_min(), (double) world().y_min(), 
	(double) world().x_max(), (double) world().y_max(), 
	(int) world().contains(asp));

    double x_min, y_min, x_max, y_max;

    convert_anyway (asp.LL(), &x_min, &y_max, &asp);
    convert_anyway (asp.UR(), &x_max, &y_min, &asp);

    fprintf (fp, "vp' = (%.9lG,%.9lG) (%.9lG,%.9lG)\n", x_min,
	y_min, x_max, y_max);

    convert_anyway (world().LL(), &x_min, &y_max, &asp);
    convert_anyway (world().UR(), &x_max, &y_min, &asp);

    fprintf (fp, "world' = (%.9lG,%.9lG) (%.9lG,%.9lG)\n", x_min,
	y_min, x_max, y_max);

    if (x_min < WgiVirtualExtent::domain.x_min() - 0.5)
    {
	fprintf (fp, "xmin TOO SMALL\n");
    }
    if (y_min < WgiVirtualExtent::domain.y_min() - 0.5)
    {
	fprintf (fp, "ymin TOO SMALL\n");
    }
    if (x_max > WgiVirtualExtent::domain.x_max() + 0.5)
    {
	fprintf (fp, "xmax TOO BIG\n");
    }
    if (y_max > WgiVirtualExtent::domain.y_max() + 0.5)
    {
	fprintf (fp, "ymax TOO BIG\n");
    }
}

//============================================================

#define PRVW(str,vw) if (debug_wgi_zoom) fprintf(stderr,\
	"%s={ %.9G,%.9G %.9G,%.9G w=%.9G h=%.9G }\n",\
	str, vw->x_min(), vw->y_min(), vw->x_max(), vw->y_max(), \
	vw->width(), vw->height())

Boolean
WgiViewPort::correct_zoom_in (const WgiWorldExtent& view_win, 
		WgiWorldExtent* vw) const
{
    return smallestViewWindow (view_win.center().x, view_win.center().y, vw);
}

Boolean
WgiViewPort::correct_zoom_out (const WgiWorldExtent& view_win, 
		WgiWorldExtent* vw) const
{
    double CF = ZOOM_INACC * world().width();
    double CF2 = CF * 2.0;

    if (view_win.width() + CF2 > world().width()
     || view_win.height() + CF2 > world().height())
    {
	// View Window is larger than World: make it smaller
	WgiWorldCoord new_h, new_w;
	WgiWorldCoord new_zx = view_win.center().x;
	WgiWorldCoord new_zy = view_win.center().y;
	if (world().aspect_ratio() < view_win.aspect_ratio())
	{
	    new_h = world().height() - CF2;
	    new_w = new_h / view_win.aspect_ratio();
	    new_zy = world().center().y;
	    if (new_zx + new_w / 2 > world().x_max())
		new_zx = world().x_max() - new_w / 2 - CF;
	    else if (new_zx - new_w / 2 < world().x_min())
		new_zx = world().x_min() + new_w / 2 + CF;
	    
	}
	else
	{
	    new_w = world().width() - CF2;
	    new_h = new_w * view_win.aspect_ratio();
	    new_zx = world().center().x;
	    if (new_zy + new_h / 2 > world().y_max())
		new_zy = world().y_max() - new_h / 2 - CF;
	    else if (new_zy - new_h / 2 < world().y_min())
		new_zy = world().y_min() + new_h / 2 + CF;
	}

	*vw = WgiWorldExtent (new_zx - new_w / 2, new_zy - new_h / 2,
			new_zx + new_w / 2, new_zy + new_h / 2, TRUE);
    }
    else
	*vw = view_win;

    // if outside world extent: move it (it will fit !)

    WgiWorldCoord d = world().x_min() - vw->x_min();

    if (d > -CF)					// too far left
    {
	vw->moveRight (d + CF);
PRVW("move_right", vw);
    }
    else if ((d = vw->x_max() - world().x_max()) > -CF)	// too far right
    {
	vw->moveLeft (d + CF);
PRVW("move_left", vw);
    }
    if ((d = world().y_min() - vw->y_min()) > -CF)	// too far down
    {
	vw->moveUp (d + CF);
PRVW("move_up", vw);
    }
    else if ((d = vw->y_max() - world().y_max()) > -CF)	// too far up
    {
	vw->moveDown (d + CF);
PRVW("move_down", vw);
    }

    if (!isValidViewWindow (*vw, NULL))
    {
DEBUG_PRINT (debug_wgi_zoom, "correct_zoom_out failed\n");
	return FALSE;
    }

    return TRUE;
}

enum VwAnchor
{
    ANCHOR_LEFT,
    ANCHOR_RIGHT,
    ANCHOR_TOP,
    ANCHOR_BOTTOM,
    ANCHOR_MID
};

Boolean
WgiViewPort::correct_zoom_move (const WgiWorldExtent& view_win, 
		WgiWorldExtent* vw) const
{
    *vw = view_win;
    VwAnchor hor = ANCHOR_MID, vert = ANCHOR_MID;
    double CF = ZOOM_INACC * world().width();

    int loop_count = 0;
    do
    {
	if (++loop_count > 4) return FALSE;

if (debug_wgi_zoom) debug_view_window (*vw);
PRVW("zoom_move", vw);
	double d = world().x_min() - vw->x_min();
	if (d > -CF)					   // too far left
	{
		vw->moveRight (d + CF);
		hor = ANCHOR_LEFT;
PRVW("move_right", vw);
	}
	else if ((d = vw->x_max() - world().x_max()) > -CF)  // too far right
	{
		vw->moveLeft (d + CF);
		hor = ANCHOR_RIGHT;
PRVW("move_left", vw);
	}

	if ((d = world().y_min() - vw->y_min()) > -CF)	   // too far down
	{
		vw->moveUp (d + CF);
		vert = ANCHOR_BOTTOM;
PRVW("move_up", vw);
	}
	else if ((d = vw->y_max() - world().y_max()) > -CF)  // too far up
	{
		vw->moveDown (d + CF);
		vert = ANCHOR_TOP;
PRVW("move_down", vw);
	}

	// At this point we should have moved the view window back in
	//  the world extent.
	// Now we check for overflow errors.

	// Calculate the virtual extent of the world if vw were the
	//  view window.
	double x_min, y_min, x_max, y_max;
	convert_anyway (world().LL(), &x_min, &y_max, vw);
	convert_anyway (world().UR(), &x_max, &y_min, vw);

	Boolean left_overflow = x_min < WgiVirtualExtent::domain.x_min();
	Boolean right_overflow = x_max > WgiVirtualExtent::domain.x_max();
	Boolean bottom_overflow = y_min < WgiVirtualExtent::domain.y_min();
	Boolean top_overflow = y_max > WgiVirtualExtent::domain.y_max();

	if (left_overflow || right_overflow || bottom_overflow || top_overflow)
	{
	    double hor_scale, vert_scale;
	    switch (hor)
	    {
		case ANCHOR_LEFT: 
		    hor_scale = min_scale_at_left_xmin (world().x_min());
		break;
		case ANCHOR_RIGHT:
		    hor_scale = min_scale_at_right_xmax (world().x_max());
		break;
		case ANCHOR_MID:
		    hor_scale = max (min_scale_at_left_zx (vw->center().x),
				min_scale_at_right_zx (vw->center().x));
		break;
	    }
	    switch (vert)
	    {
		case ANCHOR_TOP: 
		    vert_scale = min_scale_at_top_ymax (world().y_max());
		break;
		case ANCHOR_BOTTOM:
		    vert_scale = min_scale_at_bottom_ymin (world().y_min());
		break;
		case ANCHOR_MID:
		    vert_scale = max (min_scale_at_top_zy (vw->center().y),
				min_scale_at_bottom_zy (vw->center().y));
		break;
	    }
	    double scale = max (hor_scale, vert_scale);
	    double width = scale * extent().width();
	    double height = scale * extent().height();
if (debug_wgi_zoom) fprintf(stderr,"ov=%s%s%s%s,anch=%d,%d,scale=%.9lG,"
	"w=%.9lG,h=%.9lG\n",
	left_overflow ? "L" : "", right_overflow ? "R" : "",
	bottom_overflow ? "B" : "", top_overflow ? "T" : "",
	hor, vert, scale, width, height);

	    WgiWorldCoord x, y;
	    switch (hor)
	    {
		case ANCHOR_LEFT:   x = world().x_min() + CF; break;
		case ANCHOR_RIGHT:  x = world().x_max() - width - CF; break;
		case ANCHOR_MID:    x = vw->center().x - width / 2.0; break;
	    }
	    switch (vert)
	    {
		case ANCHOR_TOP:    y = world().y_max() - height - CF; break;
		case ANCHOR_BOTTOM: y = world().y_min() + CF; break;
		case ANCHOR_MID:    y = vw->center().y - height / 2.0; break;
	    }

	    vw->set (x, y, x+width, y+height, TRUE);
	}
    }
    while (!isValidViewWindow (*vw, vw));
PRVW("after", vw);

    return TRUE;
}

//============================================================

void 
WgiDevice::addViewPort (WgiViewPort* vp)
{
    priv_map[vp->id()] = vp;
}

void 
WgiDevice::removeViewPort (WgiViewPort* vp)
{
    priv_map[vp->id()] = NULL;
}

WgiViewPortId
WgiDevice::whichViewPort (const WgiVirtualLoc& loc) const
{
    if (this == NULL)
	return WgiInvViewPortId;

    WGI_VIEWPORT_LOOP (this, vp)
    {
	if (vp->contains (loc))
	    return vp->id ();
    }
    return WgiInvViewPortId;
}

WgiVirtualLoc::WgiVirtualLoc ()
{
  priv_coord.x = WgiInvVirtualLoc.x();
  priv_coord.y = WgiInvVirtualLoc.y();
}

WgiViewPortId
WgiVirtualLoc::viewPort (WgiDevice* device) const
{
    return device->whichViewPort (priv_coord);
}

WgiWorldLoc
WgiVirtualLoc::worldLoc (WgiDevice* device, WgiViewPortId* vp) const
{
    WgiWorldLoc world_loc = ::worldLoc (0, 0);
    WgiViewPortId id = viewPort(device);
    if (vp) *vp = id;

    if (id != WgiInvViewPortId)
	wgi_scrwld (id, priv_coord, &world_loc);

    return world_loc;
}

//------------------------------------------------------------

WgiVirtualLoc
WgiVirtualExtent::center () const
{
    return WgiVirtualLoc (LLx() + width() / 2,
			  LLy() + height() / 2);
}

Boolean
WgiVirtualExtent::contains (const WgiVirtualLoc& loc) const
{
    if (loc.x() > x_max())
	return FALSE;
    if (loc.x() < x_min())
	return FALSE;
    if (loc.y() > y_max())
	return FALSE;
    if (loc.y() < y_min())
	return FALSE;

    return TRUE;
}

void
WgiVirtualExtent::set (WgiVirtualCoord llx, WgiVirtualCoord lly, 
	WgiVirtualCoord urx, WgiVirtualCoord ury, Boolean inOrder)
{
    if (inOrder)
    {
	_LLx() = llx;
	_LLy() = lly;
	_URx() = urx;
	_URy() = ury;
    }
    else
    {
	if (llx < urx)
	{
	    _LLx() = llx;	_URx() = urx;
	}
	else
	{
	    _LLx() = urx;	_URx() = llx;
	}

	if (lly < ury)
	{
	    _LLy() = lly;	_URy() = ury;
	}
	else
	{
	    _LLy() = ury;	_URy() = lly;
	}
    }
}

WgiVirtualExtent&
WgiVirtualExtent::enlarge (WgiVirtualCoord x, WgiVirtualCoord y)
{
    if (x < LLx()) _LLx() = x;
    if (x > URx()) _URx() = x;
    if (y < LLy()) _LLy() = y;
    if (y > URy()) _URy() = y;
    return *this;
}

WgiVirtualExtent&
WgiVirtualExtent::expand_rel (double expand_factor)
	// Grows the extent by multiplying the width and height with the
	//  expand_factor. The center and aspect ratio are not affected.
{
    WgiVirtualLoc c = center();
    WgiVirtualCoord llx = WgiVirtualCoord 
				(c.x() - width() * expand_factor + 0.5);
    WgiVirtualCoord urx = WgiVirtualCoord 
				(c.x() + width() * expand_factor + 0.5);
    WgiVirtualCoord lly = WgiVirtualCoord 
				(c.y() - height() * expand_factor + 0.5);
    WgiVirtualCoord ury = WgiVirtualCoord 
				(c.y() + height() * expand_factor + 0.5);
    set (llx, lly, urx, ury, TRUE);
    return *this;
}

