#include "WgiWorld.h"

void
WgiWorldExtent::set (WgiWorldCoord llx, WgiWorldCoord lly, 
	WgiWorldCoord urx, WgiWorldCoord ury, Boolean inOrder)
{
    if (inOrder)
    {
	setLLx (llx);
	setLLy (lly);
	setURx (urx);
	setURy (ury);
    }
    else
    {
	if (llx < urx)
	{
	    setLLx (llx);	setURx (urx);
	}
	else
	{
	    setLLx (urx);	setURx (llx);
	}

	if (lly < ury)
	{
	    setLLy (lly);	setURy (ury);
	}
	else
	{
	    setLLy (ury);	setURy (lly);
	}
    }
}

WgiWorldExtent&
WgiWorldExtent::enlarge(WgiWorldCoord x, WgiWorldCoord y)
{
    if (x < LLx()) setLLx (x);
    if (x > URx()) setURx (x);
    if (y < LLy()) setLLy (y);
    if (y > URy()) setURy (y);
    return *this;
}

WgiWorldExtent&
WgiWorldExtent::enlarge (const Extent& ext)
{
    if (ext.xmin < LLx()) setLLx (ext.xmin);
    if (ext.xmax > URx()) setURx (ext.xmax);
    if (ext.ymin < LLy()) setLLy (ext.ymin);
    if (ext.ymax > URy()) setURy (ext.ymax);
    return *this;
}

WgiWorldExtent&
WgiWorldExtent::enlarge (const WgiWorldExtent& ext)
{
    return enlarge ((const Extent&) ext);
}

const WgiWorldLoc& 
WgiWorldExtent::center () const
{
    static WgiWorldLoc c;
    c.x = LLx() + width() / 2;
    c.y = LLy() + height() / 2;
    return c;
}

WgiWorldExtent& 
WgiWorldExtent::aspectize (double ratio)
{
    // NOTE: ratio = dy/dx

    WgiWorldLoc c = center();
    if (aspect_ratio() > ratio)
    {
	// grow horizontally
	double new_width = height() / ratio;
	setLLx (c.x - new_width / 2);
	setURx (c.x + new_width / 2);
    }
    else
    {
	// grow vertically
	double new_height = width() * ratio;
	setLLy (c.y - new_height / 2);
	setURy (c.y + new_height / 2);
    }
    return *this;
}

Boolean
WgiWorldExtent::contains (const WgiWorldLoc& loc) const
{
    if (loc.x > x_max())
	return FALSE;
    if (loc.x < x_min())
	return FALSE;
    if (loc.y > y_max())
	return FALSE;
    if (loc.y < y_min())
	return FALSE;

    return TRUE;
}
