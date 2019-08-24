#ifndef _WGI_H_INCLUDED
#define _WGI_H_INCLUDED

#include <Map.h>
#include <stdio.h>
#include <string.h>
#include <stream.h>

#include "Boolean.h"
#include "C++List.h"
#include "Form.h"

extern "C"
{
    
#define String XtString
#define Boolean XtBoolean
#define Object XtObject

/* will put the function prototypes in the X/Motif header files */
#define NeedFunctionPrototypes 1

#ifndef SOLARIS
#ifndef _WCHAR_T_DEFINED
#define _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#endif
#endif
    
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#undef String
#undef Boolean
#undef Object
#undef NoValue	/* in Xutil.h and ONTOS class */

#include "wgiStruct.h"
#include "wgiDevice.h"
#include "wgiSymbol.h"

};

#ifdef SC201
inline int fread (void* addr, int sz, int n, FILE* fp)
{ return ::fread ((char*) addr, (unsigned int) sz, (unsigned int) n, fp); }

inline int fwrite (const void* addr, int sz, int n, FILE* fp)
{ return ::fwrite ((const char*) addr, (unsigned int) sz, (unsigned int) n, fp); }

inline int fread (void* addr, unsigned int sz, int n, FILE* fp)
{ return ::fread ((char*) addr, (unsigned int) sz, (unsigned int) n, fp); }

inline int fwrite (const void* addr, unsigned int sz, int n, FILE* fp)
{ return ::fwrite ((const char*) addr, (unsigned int) sz, (unsigned int) n, fp); }
#endif /* SC201 */

extern "C" {
   long wgi_circle (WLDcrd*, long, long, long, Pickdef, long);
   long wgi_creseg (long, long, long, long, long, long, long, long, long);
   long wgi_delseg (long, long, long, long, long*);
   long wgi_draw_segment (Segment*);
   long wgi_endseg ();
   long wgi_flush (long, WLDcrd*, Pickdef, long);
   long wgi_get_current_segment (Segment**);
   Segment* wgi_get_segment_from_iden (long iden, long level);
   long wgi_hole (long, WLDcrd*, long, long, Pickdef, long);
   long wgi_is_seglist_empty (Seglist*);
   long wgi_marker (WLDcrd*, long, long, Pickdef, long);
   long wgi_points (long, long, WLDcrd*, long, Pickdef, long);
   long wgi_polyln (long, WLDcrd*, long, long, long, Pickdef, long);
   long wgi_zebra (long, WLDcrd*, long, long, long, long, Pickdef, long);
   long wgi_polygon (long, WLDcrd*, long, long, Pickdef, long);
   long wgi_rectangle (WLDcrd*, long, long, Pickdef, long);
   long wgi_stencil (long, WLDcrd*, long, long, Pickdef, long);
   long wgi_text (WLDcrd*, char*, long, long, long, long, long, long, Pickdef,
      long);
   long wgi_calc_text_center_from_base (long wide, long high,
	long rotn, long refl, WLDcrd* base0, WLDcrd* cen);

   long wgi_set_dash_style (long style, long count, Single* array);

   long wgi_seglist_create (Seglist**, unsigned long);
   long wgi_seglist_current (Seglist*);
   long wgi_get_current_seglist (Seglist**);
   long wgi_seglist_purgeit (Seglist *list, long draw);
   long wgi_seglist_parseit (Seglist*);
   long wgi_get_current_segment (Segment**);
   long wgi_is_seglist_empty (Seglist *list);
   long wgi_segment_current (Segment*);
   long wgi_delseg (long iden, long levl, long hold, long draw, long *done);
   long wgi_draw_segment (Segment*);
   long wgi_undraw_segment (Segment*);
   long wgi_change_seglist_colors (long edge, long fill, long text);
   long wgi_change_seglist_fill_style (long fill);

   long iwgi_xfrm (long, WLDcrd*, WLDcrd*, long, long);
#ifndef SC201
   int nint (double);
#endif
   long wgi_device_init (Display *dis, Window win, long wide, long high, 
			 XFontStruct *fnt, long fore, long back);
   long wgi_device_resize (long wide, long high);
   long wgi_device_color_table (unsigned long itable[], unsigned long inumbr);
   long wgi_device_plane_table (unsigned long itable[], unsigned long inumbr);

   long wgi_init (void);
   long wgi_erase_area (long xmin, long ymin, long xmax, long ymax, long mask);
   long wgi_viewport (long iden);
   long wgi_viewport_create (long iden, long xpos, long ypos, 
			     long wide, long high);
   long wgi_viewport_delete (long iden);
   long wgi_viewport_query (long iden, Extent* world_ext, Extent* scr_ext);
   long wgi_viewport_view (long iden, WLDcrd *view);
   long wgi_viewport_zoom (long iden, WLDcrd *wldp, long mode, double frac);
   long wgi_viewport_zoom_query (long iden, WLDcrd *wldp, long mode, 
				 double frac, Extent* view_win);
   long wgi_flag (long flag, Flag valu);
   long wgi_scrwld (long iden, VDCcrd vdcp, WLDcrd *wldp);
   long wgi_scrwld_n (long nelm, VDCcrd *vdc_listp, WLDcrd *wld_listp);
   long wgi_vdcwld (long nelm, VDCcrd *vdc_listp, WLDcrd *wld_listp);
   long wgi_wldvdc (long nelm, WLDcrd *wld_listp, VDCcrd *vdc_listp);

   long iwgi_extent_inverse (Extent* xtnt);
   long iwgi_extent_combine (Extent xtin, Extent* xtio);
};

extern void WgiDefaultError (const char* file, int line, const char* func,
	const char* msg);
#define WGI_ERROR(func,str) WgiDefaultError(__FILE__, __LINE__, func, str);

class WgiSegList;
class WgiSegment;

typedef long WgiPlaneMask;
#define WgiAllPlanes AllPlanes
// <X11/Xlib.h>:#define AllPlanes                ((unsigned long)~0L)

typedef unsigned char WgiBoolean;
typedef long WgiSegListId;
typedef long WgiSegmentId;
typedef unsigned char WgiVirtualColorIndex;

typedef WgiSegList* WgiSegListPtr;
typedef Segprim* SegprimPtr;

#ifdef CC30
typedef Map<WgiSegListId,WgiSegListPtr> WgiSegListMap;
typedef Mapiter<WgiSegListId,WgiSegListPtr> WgiSegListMapIter;
typedef List<SegprimPtr> SegprimList;
typedef Listiter<SegprimPtr> SegprimListIter;
#else
Mapdeclare(WgiSegListId,WgiSegListPtr);
typedef Map(WgiSegListId,WgiSegListPtr) WgiSegListMap;
typedef Mapiter(WgiSegListId,WgiSegListPtr) WgiSegListMapIter;
Listdeclare(SegprimPtr);
typedef CCList(SegprimPtr) SegprimList;
typedef Listiter(SegprimPtr) SegprimListIter;
#endif

typedef unsigned char WgiPrimType;
typedef unsigned long WgiViewPortMask;
typedef unsigned long WgiSegmentLevel;
typedef unsigned char WgiPickType;
typedef unsigned long WgiPickId;
typedef struct Pickdef WgiPickDef;
typedef unsigned char WgiFillStyle;
typedef unsigned long WgiMarkerType;
typedef unsigned char WgiRadiusIndex;
typedef unsigned char WgiWidthIndex;
typedef long WgiTextWidthIndex;
typedef long WgiTextHeightIndex;

typedef Single WgiWorldCoord;
typedef WLDcrd WgiWorldLoc;
typedef long WgiReturnCode;

typedef long WgiTextWidth;
typedef long WgiTextHeight;

typedef long WgiLineWidth;
typedef long WgiSpaceFactor;
typedef long WgiRotation;
typedef long WgiReflection;
typedef struct Textdscr WgiTextDesc;
typedef long WgiTextDescIndex;

typedef long WgiTermination;
	// Termination style of WgiPrimPolyLine/WgiPrimZebra
typedef long WgiDashStyle;
	// Dash Style for WgiPrimZebra

class Wgi
{
  friend class WgiSegList;
  friend class WgiSegment;
  friend class WgiSegPrim;

 private:

  static long priv_seglist_card;
  static WgiSegListMap priv_map;

  static WgiSegmentId priv_max_id;
  static Boolean priv_create_when_read;

  /* how many were read */

  static long priv_seglist_count;
	// NOTE: not to be confused with cardinality() !
  static long priv_segment_count;
  static long priv_segprim_count [WGI_PRIM_SIZE];

 public:

  static void init () { wgi_init(); }
	// NOTE: same as wgi_init().

  static void dontZeroOutMemory ()
  { wgi_flag (WGI_ZERO, 0); }

  /* Seglist management */

  static Boolean addSegList (const WgiSegList* seglist);
  static Boolean removeSegList (const WgiSegList* seglist);

  static long cardinality () { return priv_seglist_card; }
  static WgiSegList* seglist (WgiSegListId appl_seglist_id)
  {
    if (priv_map.element (appl_seglist_id))
	return priv_map [appl_seglist_id];
    else
	return NULL;
  }
  static WgiSegListMapIter iter ()
  { return WgiSegListMapIter(priv_map); }

  static Extent allSeglistExtent ();

  /* debugging */

  static Boolean debug ();
	// Returns whether debugging is on.
  static Boolean debug (Boolean new_flag);
	// Sets the debug flag. Returns the old value of the debug flag.

  /* dump and read */

  static Boolean dump (const char* fileName);
  static Boolean read (const char* fileName);

  static Boolean create ()
	// Whether to create the WGI elements that are read
	//  by Wgi::read(). Initially is TRUE.
  { return priv_create_when_read; }

  static Boolean create (Boolean new_create)
	// Sets whether the WGI elements that are read by Wgi::read()
	//  will actually be created.
	// Returns the previous value of the flag.
  {
    Boolean old_create = priv_create_when_read;
    priv_create_when_read = new_create;
    return old_create;
  }

  static WgiSegListId maxSegmentId()
  { return priv_max_id; }

  static long segListCount ()
	// How many segment lists were read by Wgi::read().
  { return priv_seglist_count; }
  static long segmentCount ()
	// How many segments were read by Wgi::read().
  { return priv_segment_count; }
  static long segPrimCount (int type)
	// How many segment primitives of that type
	//  were read by Wgi::read().
  { return priv_segprim_count[type]; }
  static long segPrimCount ();
	// How many segment primitives (total) were read by Wgi::read().

  /* dump and read low level functions */

  static void dump (FILE* fp, unsigned char c)
  { fwrite (&c, sizeof(char), 1, fp); }
  static void read (FILE* fp, unsigned char* c)
  { fread (c, sizeof(char), 1, fp); }

  static void dump_str (FILE* fp, const char* c)
  { fwrite (c, sizeof(char), strlen(c)+1, fp); }
  static void read_str (FILE* fp, char* c)
  {
    int n = -1;
    do
    {
	fread (&c[++n], sizeof(char), 1, fp);
    }
    while (c[n] != '\0');
  }

  static void dump (FILE* fp, unsigned long c)
  { fwrite (&c, sizeof(long), 1, fp); }
  static void read (FILE* fp, unsigned long* c)
  { fread (c, sizeof(long), 1, fp); }

  static void dump (FILE* fp, long c)
  { fwrite (&c, sizeof(long), 1, fp); }
  static void read (FILE* fp, long* c)
  { fread (c, sizeof(long), 1, fp); }

  static void dump (FILE* fp, float c)
  { fwrite (&c, sizeof(float), 1, fp); }
  static void read (FILE* fp, float* c)
  { fread (c, sizeof(float), 1, fp); }

  static void dump (FILE* fp, const Pickdef& c)
  { fwrite (&c, sizeof(Pickdef), 1, fp); }
  static void read (FILE* fp, Pickdef* c)
  { fread (c, sizeof(Pickdef), 1, fp); }

  static void dump (FILE* fp, const WgiWorldLoc& loc)
  { fwrite (&loc, sizeof(WgiWorldLoc), 1, fp); }
  static void read (FILE* fp, WgiWorldLoc* loc)
  { fread (loc, sizeof(WgiWorldLoc), 1, fp); }

  static void dump (FILE* fp, long card, const WgiWorldLoc* loc);
  static void read (FILE* fp, long card, WgiWorldLoc* loc);

  /* accessing global tables */

  static WgiWorldCoord radius (WgiRadiusIndex i)
  { return ((WLDcrd*) wld_ltabl.data) [i].x; }
  static WgiWorldCoord width (WgiWidthIndex i)
  { return ((WLDcrd*) wld_ltabl.data) [i].x; }

  static const WgiTextDesc& textDesc (WgiTextDescIndex i)
  { return ((Textdscr*) txt_dscrp.data) [i]; }
};

#define WGI_SEGLIST_LOOP(list) \
	WgiSegList* list = NULL; WgiSegListMapIter _iter = Wgi::iter(); \
	while (++_iter) if ((list = _iter.value()) != NULL)

class WgiSegList
{
  friend class Wgi;

 private:

  Seglist* priv_seglist;
  WgiSegListId priv_appl_id;

  WgiReturnCode create (WgiPlaneMask mask, WgiSegListId appl_seglist_id,
	Boolean set_current = TRUE);

 public:

  WgiSegList (FILE* fp);
  WgiSegList (Seglist* list, WgiSegListId appl_seglist_id);
  WgiSegList (WgiPlaneMask mask, WgiSegListId appl_seglist_id);
  ~WgiSegList ();

  void setCurrent ();
  static WgiSegList current ();

  Seglist* seglist () const { return priv_seglist; }
  WgiSegListId id () const { return priv_appl_id; }
  WgiPlaneMask mask () const { return seglist()->mask; }
  long cardinality () const;
  const WgiWorldLoc* worldTable () const
  { return (const WgiWorldLoc*) seglist()->wldt.data; }

  Extent extent () const
  { return seglist()->xtnt; }

  WgiReturnCode purge (Boolean undraw) const
  { return wgi_seglist_purgeit (seglist(), (long) undraw); }
	// Removes all segments from the list and undraws them first
	//  when undraw = TRUE.

	// Used to be: wgi_seglist_purgeit ().

  WgiReturnCode draw () const
  { return wgi_seglist_parseit (seglist()); }
	// Draws the Seglist.

	// Used to be: wgi_seglist_parseit ().

  Boolean isEmpty () const
  { return (Boolean) wgi_is_seglist_empty(seglist()); }

  Boolean segment (WgiSegmentId id, WgiSegmentLevel level,
		WgiSegment* seg);
	// Finds the Segment with the id in this Seglist.
	// Returns FALSE and does not affect *seg,
	//  if the Segment was not found.
	// NOTE: calles setCurrent().

  WgiReturnCode changeColors (WgiVirtualColorIndex edgeColor, 
		WgiVirtualColorIndex fillColor, WgiVirtualColorIndex textColor);
	// Changes the edge, fill and text colors of all the Segments
	//  in this list. Indices of -1 mean: do not change them.
	// NOTE: calles setCurrent().

  WgiReturnCode changeFillStyle (WgiFillStyle style);
	// Changes the fill style of all Segprims of types WGI_PRIM_POLYGON,
	//  WGI_PRIM_RECTANGLE, WGI_PRIM_CIRCLE, WGI_PRIM_STENCIL and
	//  WGI_PRIM_HOLE (of all Segments) in this list.

  void dump (FILE* fp = stdout);
};

class WgiSegment
{
  friend class Wgi;
  friend class WgiSegList;

 private:

  Segment* priv_segment;

 public:

  static WgiReturnCode create (WgiSegmentId id, WgiSegmentLevel level, 
	WgiViewPortMask mask, WgiBoolean pickable, 
	WgiVirtualColorIndex edgeColor, WgiVirtualColorIndex fillColor, 
	WgiVirtualColorIndex textColor, WgiBoolean visible, 
	WgiBoolean drawable);
  static void read (FILE* fp);

  WgiSegment () : priv_segment(NULL) {}
  WgiSegment (Segment* segment) : priv_segment (segment) {}
  WgiSegment (WgiSegmentId id, WgiSegmentLevel level, 
	WgiViewPortMask mask, WgiBoolean pickable, 
	WgiVirtualColorIndex edgeColor, WgiVirtualColorIndex fillColor, 
	WgiVirtualColorIndex textColor, WgiBoolean visible, 
	WgiBoolean drawable);

  void dump (FILE* fp, const WgiWorldLoc* table);
  Segment* segment () const { return priv_segment; }
  long cardinality () const;

  WgiSegmentId id () const { return segment()->iden; }
  WgiSegmentLevel level () const { return segment()->levl; }
  WgiViewPortMask mask () const { return segment()->mask; }
  WgiVirtualColorIndex fillColor () const { return segment()->fill; }
  WgiVirtualColorIndex edgeColor () const { return segment()->edge; }
  WgiVirtualColorIndex textColor () const { return segment()->text; }
  WgiBoolean drawable () const { return segment()->draw; }
  WgiBoolean pickable () const { return segment()->pick; }
  WgiBoolean visible () const { return segment()->visi; }

  static WgiReturnCode end () { return wgi_endseg(); }
	// Should be called when creating a new Segment, after the Segprims
	//  are added.
	// Used to be: wgi_endseg().

  static WgiReturnCode remove (WgiSegmentId id, WgiSegmentLevel level,
		WgiBoolean hold, WgiBoolean undraw, long* done)
  { return wgi_delseg (id, level, hold, undraw, done); }
	// If undraw=TRUE, it is undrawn before it is removed.
	// done returns whether Segments were actually deleted.
	// Used to be: wgi_delseg().

  WgiReturnCode draw () const { return wgi_draw_segment (segment()); }
  WgiReturnCode undraw () const { return wgi_undraw_segment (segment()); }

  WgiReturnCode setCurrent () const
  { return wgi_segment_current(segment()); }
	// Makes this segment the current one.
  static WgiReturnCode getCurrent (Segment** seg)
  { return wgi_get_current_segment (seg); }
  Boolean isCurrent () const;
	// Whether this segment is current.
};

class WgiSegPrim
{
  friend class Wgi;
  friend class WgiSegment;

 protected:

  Segprim* priv_segprim;

  WgiSegPrim () : priv_segprim(NULL) {}
  WgiSegPrim (Segprim* prim) : priv_segprim (prim) {}

 public:

  static void read (FILE* fp);
  Segprim* segprim () const { return priv_segprim; }
  WgiPrimType type () const { return segprim()->type; }

  void dump (FILE* fp, const WgiWorldLoc* table);

  WgiBoolean pickable () const { return segprim()->pick; }
  WgiPickId pickId () const { return segprim()->piid; }
  WgiPickType pickType () const { return segprim()->ptyp; }
};

class WgiPrimRectangle : public WgiSegPrim
{
  friend class Wgi;

 private:

 public:

  static WgiReturnCode create (const WgiWorldLoc* corners, 
	WgiFillStyle style, WgiBoolean pickable,
	const WgiPickDef& pick_def, WgiBoolean draw);
  static void read (FILE* fp);

  WgiPrimRectangle (Segprim* prim) : WgiSegPrim(prim) {}

  void dump (FILE* fp, const WgiWorldLoc* table);

  const WgiWorldLoc& corner1 (const WgiWorldLoc* table)
  { return table[segprim()->indx]; }
  const WgiWorldLoc& corner2 (const WgiWorldLoc* table)
  { return table[segprim()->indx+1]; }

  WgiFillStyle fillStyle () const { return segprim()->user; }
};

class WgiPrimMarker : public WgiSegPrim
{
  friend class Wgi;

 private:

 public:

  static WgiReturnCode create (const WgiWorldLoc& loc, WgiMarkerType type,
	WgiBoolean pickable, const WgiPickDef& pick_def, WgiBoolean draw);
  static void read (FILE* fp);

  WgiPrimMarker (Segprim* prim) : WgiSegPrim(prim) {}

  void dump (FILE* fp, const WgiWorldLoc* table);

  const WgiWorldLoc& location (const WgiWorldLoc* table)
  { return table[segprim()->indx]; }

  WgiMarkerType markerType() const { return segprim()->user; }
};

class WgiPrimCircle : public WgiSegPrim
{
  friend class Wgi;

 private:

 public:

  static WgiReturnCode create (const WgiWorldLoc& center, WgiWorldCoord radius,
	WgiFillStyle style, WgiBoolean pickable,
	const WgiPickDef& pick_def, WgiBoolean draw);
  static void read (FILE* fp);

  WgiPrimCircle (Segprim* prim) : WgiSegPrim(prim) {}

  void dump (FILE* fp, const WgiWorldLoc* table);

  const WgiWorldLoc& center (const WgiWorldLoc* table)
  { return table[segprim()->indx]; }
  WgiWorldCoord radius () const
  { return Wgi::radius (segprim()->wide); }

  WgiFillStyle fillStyle () const { return segprim()->user; }
};

class WgiPrimText : public WgiSegPrim
{
  friend class Wgi;

 private:

 public:

  static WgiReturnCode create (const WgiWorldLoc& center, const char* string,
	WgiTextHeight height, WgiTextWidth width, 
	WgiSpaceFactor spacingFactor, 
	WgiRotation rot, WgiReflection refl, WgiBoolean pickable,
	const WgiPickDef& pick_def, WgiBoolean draw);
  static void read (FILE* fp);

  WgiPrimText (Segprim* prim) : WgiSegPrim(prim) {}

  void dump (FILE* fp, const WgiWorldLoc* table);

  WgiTextDescIndex index () const
	// into: txt_dscrp[]
  { return segprim()->wide; }

  WgiWorldLoc center (const WgiWorldLoc* table);
  const WgiWorldLoc* baseline (const WgiWorldLoc* table)
  { return &table[segprim()->indx]; }

  WgiTextHeight height () const
  { return Wgi::textDesc(index()).high; }
  WgiTextWidth width () const
  { return Wgi::textDesc(index()).wide; }
  WgiSpaceFactor spacingFactor () const { return Wgi::textDesc(index()).spac; }
	// Percentage of width().
  WgiRotation rotation () const
  { return Wgi::textDesc(index()).rotn; }
  WgiReflection reflection () const
  { return Wgi::textDesc(index()).refl; }
  const char* string () const { return (const char*) segprim()->user; }

  const WgiWorldLoc& baseStart (const WgiWorldLoc* table) const
  { return table[segprim()->indx]; }
  const WgiWorldLoc& baseEnd (const WgiWorldLoc* table) const
  { return table[segprim()->indx+1]; }
};

class WgiPrimStencil : public WgiSegPrim
{
  friend class Wgi;

 private:

 public:

  static WgiReturnCode create (long elem_count, const WgiWorldLoc* coords, 
	WgiFillStyle style, WgiBoolean pickable,
	const WgiPickDef& pick_def, WgiBoolean draw);
  static void read (FILE* fp);

  WgiPrimStencil (Segprim* prim) : WgiSegPrim(prim) {}

  void dump (FILE* fp, const WgiWorldLoc* table);

  const WgiWorldLoc* coords (const WgiWorldLoc* table) const
  { return & table[segprim()->indx]; }
  long cardinality () const
  { return segprim()->nelm; }

  WgiFillStyle fillStyle () const { return segprim()->user; }
};

class WgiPrimPoints : public WgiSegPrim
{
  friend class Wgi;

 private:

 public:

  static WgiReturnCode create (long x_card, long y_card,
	const WgiWorldLoc* coords, 
	WgiBoolean pickable,
	const WgiPickDef& pick_def, WgiBoolean draw);
  static void read (FILE* fp);

  WgiPrimPoints (Segprim* prim) : WgiSegPrim(prim) {}

  void dump (FILE* fp, const WgiWorldLoc* table);

  const WgiWorldLoc* coords (const WgiWorldLoc* table) const
  { return & table[segprim()->indx]; }
  long cardinality () const
  { return segprim()->nelm; }
  long x_cardinality () const
  { return segprim()->user; }
  long y_cardinality () const
  { return cardinality() - x_cardinality(); }
};

class WgiPrimPolyLine : public WgiSegPrim
{
  friend class Wgi;

 private:

 public:

  static WgiReturnCode create (long card, 
	const WgiWorldLoc* coords, WgiLineWidth width, WgiTermination term,
	WgiBoolean pickable,
	const WgiPickDef& pick_def, WgiBoolean draw);
  static void read (FILE* fp);

  WgiPrimPolyLine (Segprim* prim) : WgiSegPrim(prim) {}

  void dump (FILE* fp, const WgiWorldLoc* table);

  const WgiWorldLoc* coords (const WgiWorldLoc* table) const
  { return & table[segprim()->indx]; }
  long cardinality () const
  { return segprim()->nelm; }
  WgiTermination termination () const
  { return segprim()->user; }
  WgiLineWidth width () const
  { return Wgi::width (segprim()->wide); }
};

class WgiPrimZebra : public WgiSegPrim
{
  friend class Wgi;

 private:

 public:

  static WgiReturnCode create (long card, 
	const WgiWorldLoc* coords, WgiLineWidth width, WgiTermination term,
	WgiDashStyle dash, WgiBoolean pickable,
	const WgiPickDef& pick_def, WgiBoolean draw);
  static void read (FILE* fp);

  static WgiReturnCode setDashStyle (WgiDashStyle style, long dash_count,
	Single* dash_length_array);

  WgiPrimZebra (Segprim* prim) : WgiSegPrim(prim) {}

  void dump (FILE* fp, const WgiWorldLoc* table);

  const WgiWorldLoc* coords (const WgiWorldLoc* table) const
  { return & table[segprim()->indx]; }
  long cardinality () const
  { return segprim()->nelm; }
  WgiTermination termination () const
  { return segprim()->user % 2; }
  WgiDashStyle dashStyle () const
  { return segprim()->user / 2; }
  WgiLineWidth width () const
  { return Wgi::width (segprim()->wide); }
};

class WgiPrimPolygon : public WgiSegPrim
{
  friend class Wgi;

 private:

 public:

  static WgiReturnCode create (long card, 
	const WgiWorldLoc* coords, WgiFillStyle style, 
	WgiBoolean pickable,
	const WgiPickDef& pick_def, WgiBoolean draw);
  static void read (FILE* fp);

  WgiPrimPolygon (Segprim* prim) : WgiSegPrim(prim) {}

  void dump (FILE* fp, const WgiWorldLoc* table);

  const WgiWorldLoc* coords (const WgiWorldLoc* table) const
  { return & table[segprim()->indx]; }
  long cardinality () const
  { return segprim()->nelm; }

  WgiFillStyle fillStyle () const { return segprim()->user; }
};

class WgiPrimHole : public WgiSegPrim
{
  friend class Wgi;

 private:

 public:

  static WgiReturnCode create (long card, 
	const WgiWorldLoc* coords, WgiFillStyle style,
	WgiBoolean pickable,
	const WgiPickDef& pick_def, WgiBoolean draw);
  static void read (FILE* fp);

  WgiPrimHole (Segprim* prim) : WgiSegPrim(prim) {}

  void dump (FILE* fp, const WgiWorldLoc* table);

  const WgiWorldLoc* coords (const WgiWorldLoc* table) const
  { return & table[segprim()->indx]; }
  long cardinality () const
  { return segprim()->nelm; }

  WgiFillStyle fillStyle () const { return segprim()->user; }
};

class WgiPrimFlush : public WgiSegPrim
{
  friend class Wgi;

 private:

 public:

  static WgiReturnCode create (long card, 
	const WgiWorldLoc* coords, 
	const WgiPickDef& pick_def, WgiBoolean draw);
  static void read (FILE* fp);

  WgiPrimFlush (Segprim* prim) : WgiSegPrim(prim) {}

  void dump (FILE* fp, const WgiWorldLoc* table);

  const WgiWorldLoc* coords (const WgiWorldLoc* table) const
  { return & table[segprim()->indx]; }
  long cardinality () const
  { return segprim()->nelm; }
};

#endif /* _WGI_H_INCLUDED */
