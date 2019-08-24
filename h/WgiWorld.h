#ifndef _WGIWORLD_H_INCLUDED
#define _WGIWORLD_H_INCLUDED

#include "Wgi.h"
#include "values.h"

typedef Single WgiWorldCoord;
const WgiWorldCoord WgiWorldCoordMin = - (MAXFLOAT / 2.0);
const WgiWorldCoord WgiWorldCoordMax = MAXFLOAT;

inline int operator== (const WgiWorldLoc& a, const WgiWorldLoc& b)
{
    return a.x == b.x && a.y == b.y;
}

inline WgiWorldLoc worldLoc (WgiWorldCoord x, WgiWorldCoord y)
{
    WgiWorldLoc loc;
    loc.x = x;
    loc.y = y;
    return loc;
} 

class WgiWorldExtent
{
 private:

  WLDcrd priv_coord[2];

  WgiWorldExtent* unconst () const { return (WgiWorldExtent*) this; }

 public:

  void set (WgiWorldCoord LLx, WgiWorldCoord LLy, 
	  WgiWorldCoord URx, WgiWorldCoord URy,
	  Boolean inOrder = FALSE);
    // Creates an extent covering the given area defined by a LowerLeft
    //  and an UpperRight corner.
    // If inOrder = FALSE, then it's not sure what is the LowerLeft or
    //  UpperRight corner (so that will be determined first).

  WgiWorldExtent ()
  {
    set (WgiWorldCoordMax, WgiWorldCoordMax, 
	 WgiWorldCoordMin, WgiWorldCoordMin, TRUE);
  }

  WgiWorldExtent (WgiWorldCoord LLx, WgiWorldCoord LLy, 
	WgiWorldCoord URx, WgiWorldCoord URy,
	Boolean inOrder = FALSE)
    // Creates an extent covering the given area defined by a LowerLeft
    //  and an UpperRight corner.
    // If inOrder = FALSE, then it's not sure what is the LowerLeft or
    //  UpperRight corner (so that will be determined first).
  { set(LLx, LLy, URx, URy, inOrder); }

  WgiWorldExtent (const WgiWorldLoc& LL, const WgiWorldLoc& UR, 
	Boolean inOrder = FALSE)
    // Creates an extent covering the given area defined by a LowerLeft
    //  and an UpperRight corner.
    // If inOrder = FALSE, then it's not sure what is the LowerLeft or
    //  UpperRight corner (so that will be determined first).
  { set(LL.x, LL.y, UR.x, UR.y, inOrder); }

  WgiWorldExtent (const WgiWorldLoc* corners, Boolean inOrder = FALSE)
    // Creates an extent covering the given area defined by a LowerLeft
    //  and an UpperRight corner.
    // If inOrder = FALSE, then it's not sure what is the LowerLeft or
    //  UpperRight corner (so that will be determined first).
  { set(corners[0].x, corners[0].y, corners[1].x, corners[1].y, inOrder); }

  WgiWorldExtent (const Extent& ext)
  { set(ext.xmin, ext.ymin, ext.xmax, ext.ymax, TRUE); }

  const WgiWorldLoc& LL () const { return priv_coord[0]; }
    // Returns the LowerLeft corner of this extent.
  const WgiWorldLoc& UR () const { return priv_coord[1]; }
    // Returns the UpperRight corner of this extent.
  const WgiWorldLoc& center () const;
    // Returns the center of the extent.

  const WgiWorldCoord& LLx () const { return LL().x; }
    // Returns the x coordinate of the LowerLeft corner (min. x).
  const WgiWorldCoord& LLy () const { return LL().y; }
    // Returns the y coordinate of the LowerLeft corner (min. y).
  const WgiWorldCoord& URx () const { return UR().x; }
    // Returns the x coordinate of the UpperRight corner (max. x).
  const WgiWorldCoord& URy () const { return UR().y; }
    // Returns the y coordinate of the UpperRight corner (max. y).

  const WgiWorldCoord& x_min () const { return LLx(); }
    // Same as: LLx().
  const WgiWorldCoord& y_min () const { return LLy(); }
    // Same as: LLy().
  const WgiWorldCoord& x_max () const { return URx(); }
    // Same as: URx().
  const WgiWorldCoord& y_max () const { return URy(); }
    // Same as: URy().

  WgiWorldCoord width () const
  { return URx() - LLx(); }
    // Returns the width of this extent.
  WgiWorldCoord height () const
  { return URy() - LLy(); }
    // Returns the height of this extent.

  void setLLx (WgiWorldCoord x) { priv_coord[0].x = x; }
    // Sets the x coordinate LowerLeft corner.
    //.br
    // NOTE: the user has to ensure that the UpperRight corner is not south
    //  and/or west of this new corner!
  void setLLy (WgiWorldCoord y) { priv_coord[0].y = y; }
    // Sets the y coordinate LowerLeft corner.
    //.br
    // NOTE: the user has to ensure that the UpperRight corner is not south
    //  and/or west of this new corner!
  void setURx (WgiWorldCoord x) { priv_coord[1].x = x; }
    // Sets the x coordinate UpperRight corner.
    //.br
    // NOTE: the user has to ensure that the LowerLeft corner is not north
    //  and/or east of this new corner!
  void setURy (WgiWorldCoord y) { priv_coord[1].y = y; }
    // Sets the y coordinate UpperRight corner.
    //.br
    // NOTE: the user has to ensure that the LowerLeft corner is not north
    //  and/or east of this new corner!

  void setLL (WgiWorldCoord x, WgiWorldCoord y) { setLLx (x); setLLy (y); }
    // Sets the LowerLeft corner.
    //.br
    // NOTE: the user has to ensure that the UpperRight corner is not south
    //  and/or west of this new corner!
  void setLL (const WgiWorldLoc& LL) { setLL (LL.x, LL.y); }
    // Same as previous function.

  void setUR (WgiWorldCoord x, WgiWorldCoord y) { setURx (x); setURy (y); }
    // Sets the UpperRight corner.
    //.br
    // NOTE: the user has to ensure that the LowerLeft corner is not north
    //  and/or east of this new corner!
  void setUR (const WgiWorldLoc& UR) { setUR (UR.x, UR.y); }
    // Same as previous function.

  WgiWorldExtent& enlarge (WgiWorldCoord x, WgiWorldCoord y);
    // Enlarges this extent to include (x,y).
  WgiWorldExtent& enlarge (const WgiWorldLoc& p)
  { return enlarge(p.x, p.y); }
  WgiWorldExtent& enlarge (const WgiWorldExtent& ext);
  WgiWorldExtent& enlarge (const Extent& ext);

  WgiWorldExtent& operator+= (const WgiWorldLoc& p)
  { return enlarge (p); }
  WgiWorldExtent& operator+= (const WgiWorldExtent& ext)
  { return enlarge (ext); }
  WgiWorldExtent& operator+= (const Extent& ext)
  { return enlarge (ext); }

  WgiWorldExtent& expand (WgiWorldCoord delta)
    // Expands this extent with size delta (on all sides).
    //.br
    // DECISION(S):
    //.br
    // - not checking for invalid extent
    //.br
    // - not checking for invalid delta (negative or too large)
  {
    setLLx (LLx () - delta);
    setLLy (LLy () - delta);
    setURx (URx () + delta);
    setURy (URy () + delta);
    return *this;
  } 

  double aspect_ratio () const
  { return height() / width(); }

  WgiWorldExtent& aspectize (double aspect_ratio);
  WgiWorldExtent& aspectize (double dy, double dx)
  { return aspectize (dy/dx); }
	// Grows the extent (if necessary) so that it gets the
	//  same aspect_ratio (i.e. dy/dx).
	// NOTE: the center remains the same.

  Boolean contains (const WgiWorldLoc& loc) const;
  Boolean contains (const WgiWorldExtent& ext) const
  {
    return contains (ext.LL ()) && contains (ext.UR ());
  }

  operator const WgiWorldLoc* () const { return (WgiWorldLoc*) priv_coord; }
  operator const Extent& () const
  { return * (const Extent*) priv_coord; }
  operator Extent& ()
  { return * (Extent*) priv_coord; }

  WgiWorldExtent& moveRight (WgiWorldCoord right)
  { setLLx (LLx () + right); setURx (URx () + right); return *this; }
  WgiWorldExtent& moveLeft (WgiWorldCoord left)
  { setLLx (LLx () - left); setURx (URx() - left); return *this; }
  WgiWorldExtent& moveUp (WgiWorldCoord up)
  { setLLy (LLy () + up); setURy (URy () + up); return *this; }
  WgiWorldExtent& moveDown (WgiWorldCoord down)
  { setLLy (LLy () - down); setURy (URy () - down); return *this; }
};

const WgiWorldExtent WgiInvWorldExtent (WgiWorldCoordMax,
	WgiWorldCoordMax, WgiWorldCoordMin, WgiWorldCoordMin, TRUE);
	// value: (WgiWorldCoordMax, WgiWorldCoordMax, 
	//	   WgiWorldCoordMin, WgiWorldCoordMin)

#endif /* _WGIWORLD_H_INCLUDED */
