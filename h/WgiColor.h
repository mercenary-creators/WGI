#ifndef _WGICOLOR_H_INCLUDED
#define _WGICOLOR_H_INCLUDED

#include <String.h>
#include "Wgi.h"

typedef long WgiPlaneIndex;

/* same as Pixel in $MOTIF_ROOT/X11/Intrinsic.h */
typedef unsigned long WgiColorIndex;

class WgiColorPlane
{
  friend class WgiColorMap;

 private:

  WgiColorMap* priv_map;
  int priv_bits;
  WgiPlaneMask priv_mask;
  int priv_lsb;
  int priv_msb;
  WgiPlaneIndex priv_index;

  void setLSB (int lsb);

 public:

  WgiColorPlane (WgiColorMap* map, int number_of_bits);

  WgiPlaneIndex index () const { return priv_index; }
    // Returns the index of the plane.
  const WgiColorMap* map () const { return priv_map; }
  int LSB () const { return priv_lsb; }
    // Return the Least Significant Bit used by this plane.
  int MSB () const { return priv_msb; }
    // Return the Most Significant Bit used by this plane.
  WgiPlaneMask mask () const { return priv_mask; }
    // Returns the plane mask.
  int bitCardinality () const { return priv_bits; }
    // Returns the number of bits that this plane uses. 
  int colorCardinality () const { return 1 << bitCardinality(); }
    // Returns the number of colors in this plane (incl. background).
};

class WgiColor
{
 private:

  unsigned short priv_red, priv_green, priv_blue;
  String priv_name;

 public:

  WgiColor ()
  : priv_name ("<unspec>"), priv_red(0), priv_green(0), priv_blue (0)
  {}

  WgiColor (unsigned char red, unsigned char green, unsigned char blue,
	const char* alias = NULL);
    // red, green and blue are 8 bit values (max. is 255)
  WgiColor (unsigned short red, unsigned short green, unsigned short blue,
	const char* alias = NULL);
    // red, green and blue are 16 bit values (max. is 65535)
  WgiColor (const char* by_name, Display* display);

  unsigned short red () const { return priv_red; }
    // NOTE: 16 bit values (max. is 66535)
  unsigned short green () const { return priv_green; }
  unsigned short blue () const { return priv_blue; }

  const String& name () const { return priv_name; }
  String rgb_name () const
  { return rgb_name (red(), green(), blue()); }

  double intensityFactor () const;
    // This value can be used to determine whether it is a light or
    //  a dark color.
    // If the color is used as a background color, use white text
    //  when the factor is less than approx. 27500, otherwise use black text.

  static String rgb_name (unsigned short red, unsigned short green, 
	unsigned short blue);
  static Boolean nameExists (const char* name, Display* display);
    // Returns TRUE if a color with that name exists in the color database.
};

enum
{
    WGI_COLORMAP_BITS = 8,
    WGI_COLORMAP_SIZE = 1 << WGI_COLORMAP_BITS		// = 256
};

class WgiColorMap
{
  friend class WgiColorPlane;

 private:

  Colormap priv_cmap;
  WgiColor priv_colors [WGI_COLORMAP_SIZE];
  int priv_max_size;
  Boolean priv_freed;

  WgiColorPlane* priv_planes [WGI_COLORMAP_BITS];
  int priv_plane_card;
  Display* priv_display;

  void addPlane (WgiColorPlane* plane);
  void copyRGB ();

 public:

  WgiColorMap (Display* display, int max_size = WGI_COLORMAP_SIZE);
  ~WgiColorMap ();
    // NOTE: automatically frees the Colormap (if not already freed).

  Display* display () const { return priv_display; }
  Visual* defaultVisual () const { return DefaultVisual (display(), 
		DefaultScreen (display())); }
  Colormap defaultColorMap () const { return DefaultColormap (display(), 
	DefaultScreen (display())); }

  WgiColorPlane* plane (WgiPlaneIndex plane_index) const;
    // Return NULL for invalid plane_index.
  int planeCardinality () const
  { return priv_plane_card; }
  int size () const
  { return priv_max_size; }
  Boolean isValid (WgiColorIndex color_index) const;
	// Returns TRUE if color_index is valid.

  WgiPlaneMask on () const;
  WgiPlaneMask on (WgiPlaneIndex index) const;
  WgiPlaneMask off () const
  { return 0; }
  WgiPlaneMask off (WgiPlaneIndex index) const
  { return on() - on(index); }

  Boolean setColor (WgiPlaneIndex plane_index, WgiColorIndex color_index,
		const WgiColor& color);
	// NOTE: the first color on all planes with plane_index > 0,
	//  has color_index = 1, NOT 0 !!!.
	// Returns FALSE if plane_index or color_index is invalid.
  Boolean setColor (WgiColorIndex color_index, const WgiColor& color);
	// NOTE: consider using setColor (WgiPlaneIndex, ...).
	// Returns FALSE if color_index is invalid.

  const WgiColor& color (WgiColorIndex index) const
  { return priv_colors[index]; }
  const WgiColor& color (WgiPlaneIndex plane_index, 
		WgiColorIndex color_index) const;
	// NOTE: the first color on all planes with plane_index > 0,
	//  has color_index = 1, NOT 0 !!!.

  void swapColors (WgiColorIndex color_index1, WgiColorIndex color_index2,
		Boolean install_it = TRUE);
    // Swaps the colors with those indices.
    // If install_it = FALSE, it will not call install() and therefore
    //  not show up on your screen.

  void install (Boolean installPlaneTable = TRUE);
    // If installPlaneTable, the plane table will be installed
    //  in WGI (with wgi_device_plane_table).
    // When no plane table is installed, the default table is
    //  filled with 255 (i.e. 0xFF).

    // NOTE: if you want the window manager to automatically install
    //  the Colormap whenever the cursor enters the application,
    //  you must also call the setWidgetColorMap() or setWindowColorMap()
    //  function.

  void setWindowColorMap (Window w) const;
    // Sets the Colormap resource of the Window
    //  (with XSetWindowColorMap).
    // See also: setWidgetColorMap().
  void setWidgetColorMap (Widget w) const;
    // Calls: setWindowColorMap (XtWindow(w)); (see above)
    // Use Uapp::rootWidget() in Uwindows applications.

  Boolean findColorByName (const char* color_name, WgiColorIndex* p_index,
		WgiColorIndex start_index = 0) const;
    // Finds the color with the given name starting at the color
    //  with start_index, and returns its WgiColorIndex in p_index
    //  if it finds it.
    // Returns FALSE and does not alter p_index, when it is not found.

  void free ();
    // Frees the Colormap. Automatically uninstalls it (and
    //  changes the Colormap resource back to the default
    //  colormap for all Windows that use it (I'm not sure)).

  int bitsUsed () const;
  int colorsUsed () const { return 1 << bitsUsed(); }

  Colormap colorMap () const { return priv_cmap; }

  void recopyUnusedColors ();
    // Re-copies the colors that are not set (indices: size() thru 
    //  WGI_COLORMAP_SIZE) from the standard Colormap.

    // This is useful when applications have allocated more shared colors
    //  in the standard Colormap after this WgiColorMap has been created.
    // In that case, we want to add those allocated colors to this Colormap,
    //  to prevent color flashing.

  void debug (FILE* fp = stderr);
};

/********************************************************************\
**
**	WgiVirtualColorMap defines how the virtual WGI color indices
**	 relate to the color entries in a WgiColorMap.
**	(i.e. maps WgiVirtualColorIndex -> WgiColorIndex (i.e. X Pixel)).
**
**	If no WgiVirtualColorMap is installed, the default mapping
**	 will be: 0-255 -> 0-255
**
\********************************************************************/

enum
{
    WGI_VIRTUAL_COLORMAP_SIZE = 256
};

class WgiVirtualColorMap
{
 private:

  WgiColorIndex priv_table [WGI_VIRTUAL_COLORMAP_SIZE];
  int priv_size;

 public:

  WgiVirtualColorMap (int size = WGI_VIRTUAL_COLORMAP_SIZE);

  int size () const { return priv_size; }
  Boolean isValid (WgiVirtualColorIndex i) const
  { return Boolean (i >=0 && i < size()); }
  WgiColorIndex& operator[] (WgiVirtualColorIndex i);
    // NOTE: returns element 0 (and prints an error) if the index is not valid.
  Boolean set (WgiVirtualColorIndex virtual_index, WgiColorIndex index);
    // Sets entry virtual_index to value index.
    // Returns FALSE and does not set it if virtual_index is invalid.

  Boolean setRange (WgiVirtualColorIndex start_index, 
		 WgiVirtualColorIndex end_index, 
		 WgiColorIndex start_color_table_index);
    // E.g. start_index = 0, end_index = 15, 
    //  start_color_table_index = 16 will set:
    //  [0] to 16, [1] to 17, ..., [15] to 31.

    // Returns FALSE if any of the WgiVirtualColorIndices is invalid.

  Boolean install () const;

  Boolean swapColors (WgiVirtualColorIndex c1, WgiVirtualColorIndex c2, 
		Boolean install_it = TRUE);
    // Swaps the colors with those indices.
    // If install_it = FALSE, it will not call install().

    // Returns FALSE when a WgiVirtualColorIndex is invalid (or when 
    //  install_it = TRUE and install() fails).

  void debug (FILE* fp = stderr, Boolean dontPrintSame = FALSE);
    // If dontPrintSame = TRUE, the entries for which entry[i] == i
    //  are not printed.
};

#endif /* _WGICOLOR_H_INCLUDED */
