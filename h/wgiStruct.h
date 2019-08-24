#ifndef WGI_HEADER
#define WGI_HEADER

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                 Some Utility Macros And Definitions                   --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef MIN
#define MIN(a,b)	(((a)<(b))?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b)	(((a)>(b))?(a):(b))
#endif

#ifndef ABS
#define	ABS(a)		(((a)>(0))?(a):-(a))
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                     WGI Return Code Definitions                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef WGI_CODE_DEFINES
#define WGI_CODE_DEFINES

#define WGI_NEGATIV	-1
#define WGI_SUCCESS	 0
#define WGI_WARNING	 1
#define WGI_PROBLEM	 2
#define WGI_PICKYES	 3

#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                Definitions For Keys To Flag Routines                  --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef WGI_FLAG_DEFINES
#define WGI_FLAG_DEFINES

#define WGI_DRAW	0
#define WGI_LEVL	1
#define WGI_CONT	2
#define WGI_ZERO	3

#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                      WGI Reflection Definitions                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef WGI_REFL_DEFINES
#define WGI_REFL_DEFINES

#define WGI_NREFL	0
#define WGI_YREFL	1
#define WGI_XREFL	2

#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                      WGI Primitive Type Definitions                   --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef WGI_PRIM_DEFINES
#define WGI_PRIM_DEFINES

#define WGI_PRIM_POLYLINE	0
#define WGI_PRIM_CIRCLE		1
#define WGI_PRIM_TEXT		2
#define WGI_PRIM_MARKER		3
#define WGI_PRIM_POLYGON	4
#define WGI_PRIM_RECTANGLE	5
#define WGI_PRIM_POINTS		6
#define WGI_PRIM_STENCIL	7
#define WGI_PRIM_HOLE		8
#define WGI_PRIM_FLUSH		9
#define WGI_PRIM_ZEBRA		10
#define WGI_PRIM_SIZE		11

#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                     WGI Marker Type Definitions                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef WGI_MARKER_DEFINES
#define WGI_MARKER_DEFINES

#define WGI_MARKER_POINT	1
#define WGI_MARKER_ARROW_LEFT	2
#define WGI_MARKER_ARROW_RIGHT	3
#define WGI_MARKER_ARROW_UP	4
#define WGI_MARKER_ARROW_DOWN	5
#define WGI_MARKER_TRIANGLE	6
#define WGI_MARKER_BOX		7

#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                       WGI Fill Mode Definitions                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef WGI_FILL_DEFINES
#define WGI_FILL_DEFINES

#define WGI_FILL_NONE		0
#define WGI_FILL_OFFS		1

#define WGI_FILL_FILL		1
#define WGI_FILL_LTOR		2
#define WGI_FILL_RTOL		4
#define WGI_FILL_DOTS		6
#define WGI_FILL_BIGRTOL	8
#define WGI_FILL_RL		10

#define WGI_FILL_SIZE		(WGI_FILL_RL+2)

#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                       WGI Zoom Mode Definitions                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef WGI_ZOOM_DEFINES
#define WGI_ZOOM_DEFINES

#define WGI_ZOOM_IN	0
#define WGI_ZOOM_OUT	1
#define WGI_ZOOM_UP	2
#define WGI_ZOOM_DOWN	3
#define WGI_ZOOM_LEFT	4
#define WGI_ZOOM_RIGHT	5

#define WGI_ZOOM_NONE	-1
/* i.e. none of the above */

#define WGI_ZOOMLEVE_MAX	10
#define WGI_ZOOMLEVE_MIN	-5

#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                   WGI Zebra Path Definitions                          --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef WGI_ZEBRA_DEFINES
#define WGI_ZEBRA_DEFINES

#define	WGI_MAX_DASH_STYLES	20
#define	WGI_ZEBRA_MAX_DASHES	10

#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                   WGI Postscript Output Definitions                   --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef WGI_POST_DEFINES
#define WGI_POST_DEFINES

#define	WGI_POST_FILENAME	0
#define	WGI_POST_PRINTCMD	1

#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--              Some Utility Typedefs To Keep Things Clear               --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef char Flag;

typedef float Single;

typedef long (*Function)();

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--           Type Definition Of World And Viewport Coordinate            --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct WLDcrd
{
	Single	x;
	Single	y;

} WLDcrd;

typedef struct VDCcrd
{
	short	x;
	short	y;

} VDCcrd;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                   Type Definition Of An Extent Box                    --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct Extent
{
	Single	xmin;
	Single	xmax;
	Single	ymin;
	Single	ymax;

} Extent;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                Type Definition Of A Pick Identifier                   --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct Pickdef
{
	unsigned char		type;
	unsigned long		iden;

} Pickdef;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--          Type Definition Of A Graphical Primitive Structure           --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct Segprim
{
	unsigned char		type;
	unsigned char		wide;
	unsigned char		pick;
	unsigned char		ptyp;
	unsigned short		nelm;
	unsigned long		piid;
	unsigned long		user;
	unsigned long		indx;
	struct Segprim	       *next;
	Extent			xtnt;

} Segprim;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--    Type Definition Of A Segment That Contains Graphical Primitives    --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct Segment
{
	unsigned char		visi;
	unsigned char		pick;
	unsigned char		fill;
	unsigned char		edge;
	unsigned char		text;
	unsigned char		draw;
	unsigned long		levl;
	unsigned long		iden;
	unsigned long		mask;
	struct Segprim	       *prim;
	struct Segment	       *next;
	Extent			xtnt;

} Segment;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- Mapping Is A Structure That Holds The Current Transform For A Seglist --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct Mapping
{
	long		sync;
	long		view;
	long		wide;
	long		high;
	Extent		xtnt;

} Mapping;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- An Error Printing Macro That Prints The File And Line Plus A printf() --*/
/*--               Like Message With Variable Arguments                    --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

extern void wgi_error_set();
extern long wgi_error_file();
extern void wgi_error_print();
extern void wgi_message_print();

#define WOOPS	(wgi_error_set(__LINE__,__FILE__),wgi_error_print)
#define SAYIT	(wgi_message_print)

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--          Structures And Definitions For The Free Table Code           --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#define WGI_TABLE_MAX_CHUNK	65535

extern long wgi_table_free();
extern long wgi_table_init();
extern long wgi_table_aloc();

typedef struct Tnode
{
	unsigned long indx;
	unsigned long nelm;
	struct Tnode *next;
	struct Tnode *back;

} Tnode;

typedef struct Table
{
	unsigned long sync;
	unsigned long elsz;
	unsigned long grow;
	unsigned long nelm;
	unsigned long used;

	struct Tnode *node;
	char	     *data;

} Table;

typedef struct Tbuff
{
	unsigned long size;
	unsigned long sync;
	char	     *data;

} Tbuff;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--  Type Definition Of A Seglist Which Contains Many Segments And Their  --*/
/*--                    Current Mapping And Data Space                     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct Seglist
{
	long		mask;
	Table		wldt;
	Tbuff		vdcb;
	Extent		xtnt;
	Mapping		smap;
	Segment	       *segs;
	Segment	       *last;
	Segment	       *hold;
	Segment	       *back;

} Seglist;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--           Structures And Definitions For The Picking Code             --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef enum
{
	wgiPickModeNormal,
	wgiPickModeDoPoly,
	wgiPickModeExtent
} wgiPickMode_t;

typedef struct wgiPickManage_t
{
	wgiPickMode_t		PickMode;
	long			ViewMask;
	long			Continue;
	Extent			PickRect;

	Seglist		       *SeglistP;
	Segment		       *SegmentP;
	Segprim		       *SegprimP;
	Seglist		       *LastList;

} wgiPickManage_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--           Structures And Definitions For The Viewport Code            --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#define WGI_VIEWPORT_MAX	32

typedef struct Viewport
{
	unsigned char	vused;
	unsigned char	vsync;
	unsigned char	vdraw;
	unsigned char	vclip;
	unsigned long	viden;
	unsigned long	vxpos;
	unsigned long	vypos;
	unsigned long	vwide;
	unsigned long	vhigh;
	Tbuff		vtext;
	Tbuff		vline;
	Extent		vview;
	Extent		vonly;

} Viewport;

#define WGI_VIEWPORT_ALL	(~0)

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--  External Global Variables That Match The Definitions In Globvars.c   --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

extern Table *wld_table;
extern Tbuff *vdc_tbuff;

extern Table wld_ltabl;
extern Table wld_ttabl;
extern Table txt_dscrp;

extern Function  dispatch_clip[];
extern Function  dispatch_draw[];
extern Function *dispatch_tabl[];

extern Seglist *iseglistp;
extern Segment *isegmentp;
extern Segprim *isegprimp;

extern Mapping vmap;

extern Flag iwgi_draw_flag;
extern Flag iwgi_levl_flag;
extern Flag iwgi_cont_flag;
extern Flag iwgi_zero_flag;

#define LOOK_LEVL(that, same)	((iwgi_levl_flag) ? ((that) == (same)) : (TRUE))

#endif

#define WGI_REALLOC(mptr, nelm, type)	\
	((type *)ewgi_realloc((char *)mptr, (nelm) * sizeof(type)))

#define WGI_MALLOC(nelm, type)	\
	((type *)ewgi_malloc((nelm) * sizeof(type)))

#define WGI_FREE(mptr)	\
	(ewgi_free((char *)mptr))

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
