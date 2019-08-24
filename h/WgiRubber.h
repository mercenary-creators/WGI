#ifndef WGI_RUBBER_H
#define WGI_RUBBER_H

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubber.h -- This file contains types and definitions that are used --*/
/*--                to create rubber-band control structures.              --*/
/*--                                                                       --*/
/*--                The rubber-band control structure wgiRubberStruct_t is --*/
/*--                a C union which is composed  of similar sub-structures --*/
/*--                that contain members to define the special behavior of --*/
/*--                the rubber-band.  The sub-structures also have members --*/
/*--                for real time feed back of the rubber-band`s state and --*/
/*--                end points. These are called output members,  and they --*/
/*--                are defined by the kind of output each type of rubber- --*/
/*--                band may have:  The radius, center and angle of an arc --*/
/*--                for example.                                           --*/
/*--                                                                       --*/
/*--                All the sub-structures contain common members that are --*/
/*--                the same for each rubber-band type. They are placed at --*/
/*--                the beginning of the sub-structure and this allows the --*/
/*--                control structures to act as a generic or poly-morphic --*/
/*--                type. The most important of these is the TypeOf member --*/
/*--                at the very beginning of the sub-structure.  This is a --*/
/*--                type tag used in all the internal routines and it also --*/
/*--                defines what sub-structure members the user can set on --*/
/*--                that type. An example of how this is used:             --*/
/*--                                                                       --*/
/*--                wgiRubberStruct_t rubber;                              --*/
/*--                                                                       --*/
/*--                rubber.Common.TypeOf = wgiRubberTypeOfVector;          --*/
/*--                rubber.Common.Anchor = {WLDcrd}                        --*/
/*--                rubber.Common.Offset = {WLDcrd}                        --*/
/*--                rubber.Common.ColorI = {Color Table Index}             --*/
/*--                rubber.Common.PlaneI = {Plane Table Index}             --*/
/*--                rubber.Common.Dashed = wgiDeviceDashedDoNone;          --*/
/*--                rubber.Common.DrawOp = wgiDeviceDrawOpXorXor;          --*/
/*--                rubber.Common.UserFn = NULL;                           --*/
/*--                rubber.Common.FnData = NULL;                           --*/
/*--                                                                       --*/
/*--                Because the sub-structures are overlaid in a union and --*/
/*--                the common members of each one are at exactly the same --*/
/*--                location we could also set the common members by using --*/
/*--                the specific type of sub-structure for the rubber-band --*/
/*--                we are creating:                                       --*/
/*--                                                                       --*/
/*--                rubber.Vector.TypeOf = wgiRubberTypeOfVector;          --*/
/*--                rubber.Vector.Anchor = {WLDcrd}                        --*/
/*--                rubber.Vector.Offset = {WLDcrd}                        --*/
/*--                rubber.Vector.ColorI = {Color Table Index}             --*/
/*--                rubber.Vector.PlaneI = {Plane Table Index}             --*/
/*--                rubber.Vector.Dashed = wgiDeviceDashedDoNone;          --*/
/*--                rubber.Vector.DrawOp = wgiDeviceDrawOpXorXor;          --*/
/*--                rubber.Vector.UserFn = NULL;                           --*/
/*--                rubber.Vector.FnData = NULL;                           --*/
/*--                                                                       --*/
/*--                There are sub-structure members that are unique or are --*/
/*--                defined differently for each sub-structure type. These --*/
/*--                can only be set or examined using the specific type of --*/
/*--                sub-structure we are creating.  Here we set the Vector --*/
/*--                SnapTo mode to Manhattan:                              --*/
/*--                                                                       --*/
/*--                rubber.Vector.SnapTo = wgiRubberSnapToManh;            --*/
/*--                                                                       --*/
/*--                The user MUST SET ALL of the  common and sub-structure --*/
/*--                specific members to some meaningful values before they --*/
/*--                are used.  The rubber-banding routines will set output --*/
/*--                members.                                               --*/
/*--                                                                       --*/
/*--                In order to use the control structures they first must --*/
/*--                be pointed to by a rubber-band manager structure.  All --*/
/*--                the rubber-band functions operate on manager structure --*/
/*--                pointers.  You create a manager structure and populate --*/
/*--                the DoList and Number members:                         --*/
/*--                                                                       --*/
/*--                wgiRubberManage_t manage;                              --*/
/*--                                                                       --*/
/*--                manage.Number = 1;                                     --*/
/*--                manage.DoList = &rubber;                               --*/
/*--                                                                       --*/
/*--                Although this looks a bit odd it is really designed to --*/
/*--                manage several rubber-band control structures at once: --*/
/*--                                                                       --*/
/*--                wgiRubberStruct_t rubber_list[3];                      --*/
/*--                                                                       --*/
/*--                rubber_list[0].Vector.TypeOf = wgiRubberTypeOfVector;  --*/
/*--                rubber_list[0].Vector....... = ...                     --*/
/*--                rubber_list[1].Xhairs.TypeOf = wgiRubberTypeOfXhairs;  --*/
/*--                rubber_list[1].Xhairs....... = ...                     --*/
/*--                rubber_list[2].Circle.TypeOf = wgiRubberTypeOfCircle;  --*/
/*--                rubber_list[2].Circle....... = ...                     --*/
/*--                                                                       --*/
/*--                manage.Number = 3;                                     --*/
/*--                manage.DoList = rubber_list;                           --*/
/*--                                                                       --*/
/*--                wgi_rubber_init(&manage);                              --*/
/*--                ...                                                    --*/
/*--                                                                       --*/
/*--                Before we can draw any rubber-bands we MUST first call --*/
/*--                the initialization routine.  This sets members in both --*/
/*--                the manager and control structures it points to.  This --*/
/*--                MUST NOT be called again (with same manager) until the --*/
/*--                application is finished with the manager and calls the --*/
/*--                done routine. The done routine undraws any rubber-band --*/
/*--                on the graphics device and free`s some internal memory --*/
/*--                pointers.   The control structures still contain valid --*/
/*--                information that may be examined.                      --*/
/*--                                                                       --*/
/*--                ...                                                    --*/
/*--                wgi_rubber_done(&manage);                              --*/
/*--                                                                       --*/
/*--                circle_radius = rubber_list[2].Circle.Radius;          --*/
/*--                                                                       --*/
/*--                Rubber-bands are drawn on the graphics device when the --*/
/*--                mouse is moved in a wgi viewport.   The user must call --*/
/*--                the move routine in response to  these mouse movements --*/
/*--                to update the rubber-band`s position and graphics. The --*/
/*--                move routine needs the current wgi viewport id and the --*/
/*--                world coordinate of the mouse position. We will assume --*/
/*--                that we have these values:                             --*/
/*--                                                                       --*/
/*--                long   viewport;                                       --*/
/*--                WLDcrd position;                                       --*/
/*--                                                                       --*/
/*--                wgi_rubber_move(&manage, viewport, &position);         --*/
/*--                                                                       --*/
/*--                The last routine is the zonk routine.  There are times --*/
/*--                when you need to undraw the rubber-bands but you don`t --*/
/*--                want to call the done routine.  You may want to undraw --*/
/*--                if the mouse leaves the window for example. The second --*/
/*--                parameter is the undraw flag:                          --*/
/*--                                                                       --*/
/*--                wgi_rubber_zonk(&manage, TRUE);                        --*/
/*--                                                                       --*/
/*--                There is also a situation that needs to be handled. If --*/
/*--                the DrawOp member of a control structure is set to Xor --*/
/*--                and the application erases the rubber-band graphics by --*/
/*--                doing an erase viewport or similar function,  the next --*/
/*--                move will attempt to erase the rubber-bands by Xor'ing --*/
/*--                the graphics off.   If the graphics are not there this --*/
/*--                actually puts the graphics back during an erase!! This --*/
/*--                can be handled by calling the zonk routine with redraw --*/
/*--                flag turned off:                                       --*/
/*--                                                                       --*/
/*--                wgi_rubber_zonk(&manage, FALSE);                       --*/
/*--                                                                       --*/
/*--                The end effect is the manager  forgets that there were --*/
/*--                rubber-bands to  undraw and the next move will not try --*/
/*--                to do the undraw.                                      --*/
/*--                                                                       --*/
/*--                The common members and their function are explained in --*/
/*--                the wgiRubberCommon_t sub-structure. The sub-structure --*/
/*--                specific and output members  are explained for each of --*/
/*--                the sub-structure types.                               --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Producer: Dean S. Jones    Inspection: MPM 08/11/93    Coverage: 100% --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#if defined(__cplusplus)
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- Octants and octant locking -- Octants are the control concept used in --*/
/*-- all rubber-banding.  The space around the anchor point is broken into --*/
/*-- 8 regions, hence octants. As the variable point crosses the boundries --*/
/*-- between these regions,  the direction of behavior of the rubber-bands --*/
/*-- will change. With the anchor at the center the octants look like this --*/
/*-- sort of ( character devices are non-square )                          --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--                         \     2  |  1     /                           --*/
/*--                           \      |      /                             --*/
/*--                             \    |    /                               --*/
/*--                          3    \  |  /    0                            --*/
/*--                                 \|/                                   --*/
/*--                        ----------A----------                          --*/
/*--                                 /|\                                   --*/
/*--                          4    /  |  \    7                            --*/
/*--                             /    |    \                               --*/
/*--                           /      |      \                             --*/
/*--                         /     5  |  6     \                           --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Given one anchor and one variable point,  and a rubber band type like --*/
/*-- Bend90 for example, there could be 2 ways to draw a line: ( Anchor is --*/
/*-- represented by (A), Variable by (V) and the line by (#))              --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--       \        |    V   /                 \        #####V   /         --*/
/*--         \      |    # /                     \      #      /           --*/
/*--           \    |    #                         \    #    /             --*/
/*--             \  |  / #                           \  #  /               --*/
/*--               \|/   #                             \#/                 --*/
/*--      ----------A#####-----      or       ----------A----------        --*/
/*--               /|\                                 /|\                 --*/
/*--             /  |  \                             /  |  \               --*/
/*--           /    |    \                         /    |    \             --*/
/*--         /      |      \                     /      |      \           --*/
/*--       /        |        \                 /        |        \         --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- With the concept of octants, the line would be drawn using the second --*/
/*-- case.   The line will be drawn so as to not cross a diagonal boundry. --*/
/*-- What if the user wanted the first case ?? Some rubber-band types have --*/
/*-- a Locked member. When this is set to TRUE, as the user crosses one of --*/
/*-- the diagonal boundries the rubber band acts as if it was still in the --*/
/*-- previous octant.                                                      --*/
/*--                                                                       --*/
/*--     user moves right a bit then up and across a diagonal boundry:     --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--       \        |        /                 \        |    V   /         --*/
/*--         \      |      /                     \      |    # /           --*/
/*--           \    |    /                         \    |    #             --*/
/*--             \  |  / V                           \  |  / #             --*/
/*--               \|/   #                             \|/   #             --*/
/*--      ----------A#####-----       -->     ----------A#####-----        --*/
/*--               /|\                                 /|\                 --*/
/*--             /  |  \                             /  |  \               --*/
/*--           /    |    \                         /    |    \             --*/
/*--         /      |      \                     /      |      \           --*/
/*--       /        |        \                 /        |        \         --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- To un-set an octant lock, the user must move across a horizontal or a --*/
/*-- vertical boundry if they want the un-locked behavior.                 --*/
/*--                                                                       --*/
/*--      user moves left across boundry then right to target point:       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--       \        |        /                 \        |        /         --*/
/*--         \    V##      /                     \      ####V  /           --*/
/*--           \    #    /                         \    #    /             --*/
/*--             \  #  /                             \  #  /               --*/
/*--               \#/                                 \#/                 --*/
/*--      ----------A----------      -->      ----------A----------        --*/
/*--               /|\                                 /|\                 --*/
/*--             /  |  \                             /  |  \               --*/
/*--           /    |    \                         /    |    \             --*/
/*--         /      |      \                     /      |      \           --*/
/*--       /        |        \                 /        |        \         --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Now, with locking still enabled,  the lock will engage in as the user --*/
/*-- moves across the diagonal boundry in the other direction.             --*/
/*--                                                                       --*/
/*--        user continues moving left across the diagonal boundry:        --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--       \        |        /                 \        |        /         --*/
/*--         \      ####V  /                     \      ##########V        --*/
/*--           \    #    /                         \    #    /             --*/
/*--             \  #  /                             \  #  /               --*/
/*--               \#/                                 \#/                 --*/
/*--      ----------A----------      -->      ----------A----------        --*/
/*--               /|\                                 /|\                 --*/
/*--             /  |  \                             /  |  \               --*/
/*--           /    |    \                         /    |    \             --*/
/*--         /      |      \                     /      |      \           --*/
/*--       /        |        \                 /        |        \         --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Again, to un-set the lock they move across a horizontal or a vertical --*/
/*-- boundry:                                                              --*/
/*--                                                                       --*/
/*--    user moves below boundry to unset lock then up to target point:    --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*--       \        |        /                 \        |        /         --*/
/*--         \      |      /                     \      |      /  V        --*/
/*--           \    |    /                         \    |    /    #        --*/
/*--             \  |  /                             \  |  /      #        --*/
/*--               \|/                                 \|/        #        --*/
/*--      ----------A##########      -->      ----------A##########        --*/
/*--               /|\        #                        /|\                 --*/
/*--             /  |  \      V                      /  |  \               --*/
/*--           /    |    \                         /    |    \             --*/
/*--         /      |      \                     /      |      \           --*/
/*--       /        |        \                 /        |        \         --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#define WGI_RUBBER_LOCKIN_NOT (9) /* Value to signal undefined octant lockin */

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberTypeOf_t -- This type is used to define what the rubber-band --*/
/*--                      is and what sub-structure members we can access. --*/
/*--                      It is used internally to call routines that will --*/
/*--                      calculate and draw the rubber-band.              --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef enum
{
	wgiRubberTypeOfCommon,
	wgiRubberTypeOfXhairs,
	wgiRubberTypeOfBounds,
	wgiRubberTypeOfVector,
	wgiRubberTypeOfMitred,
	wgiRubberTypeOfBend90,
	wgiRubberTypeOfLarc90,
	wgiRubberTypeOfLarc45,
	wgiRubberTypeOfArc3pt,
	wgiRubberTypeOfCircle,
	wgiRubberTypeOfNshape,
	wgiRubberTypeOfEllipt,
	wgiRubberTypeOfCloser,
	wgiRubberTypeOfTextEx,
	wgiRubberTypeOfMyType,
	wgiRubberTypeOfLarc45Rot

} wgiRubberTypeOf_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberHairTo_t -- This type is used to define the HairTo member of --*/
/*--                      the Xhairs sub-structure.  See the sub-structure --*/
/*--                      definition for an explanation of it`s use.       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef enum
{
	wgiRubberHairToEdges,
	wgiRubberHairToConst,
	wgiRubberHairToScale

} wgiRubberHairTo_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberBaseIs_t -- This type is used to define the BaseIs member of --*/
/*--                      the Bounds and Ellipt sub-structures.    See the --*/
/*--                      sub-structure definitions  for an explanation of --*/
/*--                      it`s use.                                        --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef enum {

	wgiRubberBaseIsCorner,
	wgiRubberBaseIsCenter

} wgiRubberBaseIs_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberSnapTo_t -- This type is used to define the SnapTo member of --*/
/*--                      the Vector sub-structure.  See the sub-structure --*/
/*--                      definition for an explanation of it`s use.       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef enum {

	wgiRubberSnapToFree,
	wgiRubberSnapToSemi,
	wgiRubberSnapToManh,
	wgiRubberSnapToVert,
	wgiRubberSnapToHorz,
	wgiRubberSnapToDiag

} wgiRubberSnapTo_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberLastIs_t -- This type is used to define the LastIs member of --*/
/*--                      the Arc3pt sub-structure.  See the sub-structure --*/
/*--                      definition for an explanation of it`s use.       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef enum
{
	wgiRubberLastIsEnd,
	wgiRubberLastIsMid

} wgiRubberLastIs_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberSpokes_t -- This type is used to define the Spokes member of --*/
/*--                      various sub-structures. It defines the secondary --*/
/*--                      graphical output of a rubber-band.  Check it out --*/
/*--                      for each sub-structure (tired of that line yet?) --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef enum
{
	wgiRubberSpokesDoNot,
	wgiRubberSpokesSlice,
	wgiRubberSpokesCross,
	wgiRubberSpokesScope

} wgiRubberSpokes_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberCommon_t -- This type is a sub-structure that can be used to --*/
/*--                      access only the common data members of a control --*/
/*--                      structure. The common members are explained here --*/
/*--                      and are not repeated for each sub-structure.     --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- TypeOf: This is a type tag used to determine what kind of rubber-band --*/
/*--         we have and what sub-structure members we can access.         --*/
/*--                                                                       --*/
/*-- Dashed: This is a value used to specify the rubber-band`s dash style. --*/
/*--         See wgiDevice.h for the explanation of the various values for --*/
/*--         the wgiDeviceDashed_t type.                                   --*/
/*--                                                                       --*/
/*-- DrawOp: This is a value used to specify how the rubber-band lines are --*/
/*--         drawn and erased.  See wgiDevice.h for the explanation of the --*/
/*--         various values for the wgiDeviceDrawOp_t type.                --*/
/*--                                                                       --*/
/*-- ColorI: This is a color index into the device color table. The device --*/
/*--         color table is set with the wgi_device_color_table() routine. --*/
/*--         This defines what color the rubber-band is drawn with.        --*/
/*--                                                                       --*/
/*-- PlaneI: This is a plane index into the device plane table. The device --*/
/*--         plane table is set with the wgi_device_plane_table() routine. --*/
/*--         This defines which bit-plane is used to draw the rubber-band. --*/
/*--                                                                       --*/
/*-- WARNING: The effect of DrawOp, ColorI and PlaneI depend on the set-up --*/
/*--          of the hardware colormap, device color table, and the device --*/
/*--          plane table.  You may wish to define color planes which will --*/
/*--          allow non-destructive overlay rubber-banding.  Please review --*/
/*--          any information you have on color models.  This package does --*/
/*--          not enforce any color model, but provides a way to set plane --*/
/*--          masks and an indirect color table.                           --*/
/*--                                                                       --*/
/*-- Anchor: This is the world coordinate anchor point of the rubber-band. --*/
/*--         The rubber-band is drawn attached to, or in reference to this --*/
/*--         point.                                                        --*/
/*--                                                                       --*/
/*-- Offset: This is an offset (x, y) applied to the variable point on the --*/
/*--         rubber-band. The values are in world coordinate space.        --*/
/*--                                                                       --*/
/*-- UserFn: This is a user defined function pointer. The function will be --*/
/*--         called after the rubber-band end point calculation but before --*/
/*--         the rubber-band is drawn.  This function can be used to check --*/
/*--         that the rubber-band is within some user defined limits.  The --*/
/*--         user could reset a member such as ColorI or Dashed to signify --*/
/*--         an `out-of-bounds` condition.  The calling parameters of this --*/
/*--         function are as follows:                                      --*/
/*--                                                                       --*/
/*--         void function(                                                --*/
/*--                  void              *FnData,                           --*/
/*--                  wgiRubberStruct_t *Rubber,                           --*/
/*--                  unsigned long      MIndex,                           --*/
/*--                  WLDcrd            *MovePt);                          --*/
/*--                                                                       --*/
/*--         FnData: This is a user a defined pointer. ( Described below ) --*/
/*--                                                                       --*/
/*--         Rubber: This is a pointer to the current rubber-bands control --*/
/*--                 structure.                                            --*/
/*--                                                                       --*/
/*--         MIndex: This is the index of the current rubber-band as it is --*/
/*--                 defined in the manager DoList.                        --*/
/*--                                                                       --*/
/*--         MovePt: This is the current variable point on the rubber band --*/
/*--                 before the common member Offset is applied. The point --*/
/*--                 plus Offset can be calculated as:                     --*/
/*--                                                                       --*/
/*--                 (x) = MovePt->x + Rubber->Common.Offset.x             --*/
/*--                 (y) = MovePt->y + Rubber->Common.Offset.y             --*/
/*--                                                                       --*/
/*-- FnData: This is a user defined data pointer and is passed to the user --*/
/*--         defined UserFn function.                                      --*/
/*--                                                                       --*/
/*--         NOTE FOR C++ PROGRAMMERS:                                     --*/
/*--                                                                       --*/
/*--         There is a clever little hack that can be used here.  The C++ --*/
/*--         member functions  have a hidden ``this`` pointer as the first --*/
/*--         argument.  You can have UserFn point to a member function and --*/
/*--         FnData a pointer to ``this``.   The member function should be --*/
/*--         called with and for the correct object. You could declare the --*/
/*--         member function without the FnData argument.     Good Luck!!! --*/
/*--                                                                       --*/
/*-- WARNING: Beware of clever little hacks!!!                             --*/
/*--                                                                       --*/
/*-- WARNING: In my opinion,  users should only change members that define --*/
/*--          drawing behavior in the UserFn function.  Dashed, ColorI and --*/
/*--          PlaneI seem safe. Change anything else,  yer` on your own!!! --*/
/*--                                                                       --*/
/*-- InitFn, CalcFn, DrawFn, DoneFn:  Even though these members are common --*/
/*--         members, they are for internal use in all but the MyType sub- --*/
/*--         structure.  Check out MyType for their use.                   --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct
{
	wgiRubberTypeOf_t	TypeOf;
	wgiDeviceDashed_t	Dashed;
	wgiDeviceDrawOp_t	DrawOp;
	unsigned long		ColorI;
	unsigned long		PlaneI;
	WLDcrd			Anchor;
	WLDcrd			Offset;
	unsigned long		Changed;

	Function		UserFn;
	Function		InitFn;
	Function		CalcFn;
	Function		DrawFn;
	Function		DoneFn;
	void		       *FnData;

} wgiRubberCommon_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberXhairs_t -- This type is a sub-structure and is used to draw --*/
/*--                      a cross-hair on the graphics device.             --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE BEHAVIOR:                                --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- HairTo: This defines how the cross-hair cursor lines are drawn. It is --*/
/*--         used along with the Length member to calculate how long cross --*/
/*--         hair lines are. Legal values are:                             --*/
/*--                                                                       --*/
/*--         wgiRubberHairToEdges: Draw the cross hair to the edges of the --*/
/*--                               graphics device.                        --*/
/*--                                                                       --*/
/*--         wgiRubberHairToConst: Draw the cross hair using a pixel value --*/
/*--                               constant length.  This value is defined --*/
/*--                               by the Length member.                   --*/
/*--                                                                       --*/
/*--         wgiRubberHairToScale: Draw the cross hair using a line length --*/
/*--                               defined in world coordinate space. This --*/
/*--                               value is defined by the Length member.  --*/
/*--                                                                       --*/
/*-- Length: Like didn't we cover this above...    :-)                     --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE OUTPUT:                                  --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Output[0]: The variable point on the cross-hair.  ( Not real useful ) --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- WARNING: The Anchor member is not used.                               --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct
{
	wgiRubberTypeOf_t	TypeOf;
	wgiDeviceDashed_t	Dashed;
	wgiDeviceDrawOp_t	DrawOp;
	unsigned long		ColorI;
	unsigned long		PlaneI;
	WLDcrd			Anchor;
	WLDcrd			Offset;
	unsigned long		Changed;

	Function		UserFn;
	Function		InitFn;
	Function		CalcFn;
	Function		DrawFn;
	Function		DoneFn;
	void		       *FnData;

	WLDcrd		       *Output[1];
	WLDcrd			Buffer[2];
	VDCcrd			Mapped[2];

	double			Length;
	wgiRubberHairTo_t	HairTo;

} wgiRubberXhairs_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberBounds_t -- This type is a sub-structure and is used to draw --*/
/*--                      a bounding box on the graphics device.           --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE BEHAVIOR:                                --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Aspect: This defines a fixed aspect ratio (y/x) for the bounding box. --*/
/*--         If this value is zero the box can be of any shape. If this is --*/
/*--         set to something like (2.5/5.0), the box will always be twice --*/
/*--         as wide as it is high.                                        --*/
/*--                                                                       --*/
/*-- BaseIs: This defines where the anchor of the bounding box is.   Legal --*/
/*--         values are:                                                   --*/
/*--                                                                       --*/
/*--         wgiRubberBaseIsCorner: The anchor of the bounding box is at a --*/
/*--                                point at a corner of the box.          --*/
/*--                                                                       --*/
/*--         wgiRubberBaseIsCenter: The anchor of the bounding box is at a --*/
/*--                                point in the center of the box.        --*/
/*--                                                                       --*/
/*-- Spokes: This defines some secondary graphical output for the bounding --*/
/*--         box.  Legal values are:                                       --*/
/*--                                                                       --*/
/*--         wgiRubberSpokesCross: 2 lines are drawn from corner to corner --*/
/*--                               in the bounding box.                    --*/
/*--                                                                       --*/
/*--         wgiRubberSpokesScope: 2 lines are drawn, one a horizontal and --*/
/*--                               the other a vertical bisector.          --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE OUTPUT:                                  --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Output[0]:    Is set to NULL if there was no bounding box, else is an --*/
/*--               array of 2 world coordinates  that define the min & max --*/
/*--               corners of the bounding box.                            --*/
/*--                                                                       --*/
/*-- Output[0][0]: This is a bounding box corner,  a world coordinate with --*/
/*--               the smallest or min (x,y) values.                       --*/
/*--                                                                       --*/
/*-- Output[0][1]: This is a bounding box corner,  a world coordinate with --*/
/*--               the largest or max (x,y) values.                        --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct
{
	wgiRubberTypeOf_t	TypeOf;
	wgiDeviceDashed_t	Dashed;
	wgiDeviceDrawOp_t	DrawOp;
	unsigned long		ColorI;
	unsigned long		PlaneI;
	WLDcrd			Anchor;
	WLDcrd			Offset;
	unsigned long		Changed;

	Function		UserFn;
	Function		InitFn;
	Function		CalcFn;
	Function		DrawFn;
	Function		DoneFn;
	void		       *FnData;

	WLDcrd		       *Output[1];
	WLDcrd			Buffer[2];
	VDCcrd			Mapped[2];

	double			Aspect;
	wgiRubberBaseIs_t	BaseIs;
	wgiRubberSpokes_t	Spokes;

} wgiRubberBounds_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberVector_t -- This type is a sub-structure and is used to draw --*/
/*--                      a single line vector on the graphice device.     --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE BEHAVIOR:                                --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- SnapTo: This defines the snapping behavior of the vector line.  Legal --*/
/*--         values are:                                                   --*/
/*--                                                                       --*/
/*--         wgiRubberSnapToFree: There is no snapping and the vector will --*/
/*--                              be drawn from the anchor to the variable --*/
/*--                              point.                                   --*/
/*--                                                                       --*/
/*--         wgiRubberSnapToSemi: The vector is snapped to Semi-Manhattan. --*/
/*--                              This is a pure horizontal, vertical or a --*/
/*--                              diagonal vector.                         --*/
/*--                                                                       --*/
/*--         wgiRubberSnapToManh: The vector is snapped to Manhattan. This --*/
/*--                              is a pure horizontal or vertical vector. --*/
/*--                                                                       --*/
/*--         wgiRubberSnapToVert: The vector is snapped to a pure Vertical --*/
/*--                              line ( same x coordinate as the anchor ) --*/
/*--                                                                       --*/
/*--         wgiRubberSnapToHorz: The vector is snapped to pure Horizontal --*/
/*--                              ( same y coordinate as the anchor )      --*/
/*--                                                                       --*/
/*--         wgiRubberSnapToDiag: The vector is snapped to pure a Diagonal --*/
/*--                              line ( 45 degrees from the anchor )      --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE OUTPUT:                                  --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Output[0]:    Is set to NULL if there was no vector calculated,  else --*/
/*--               is an array of 2 world coordinates  that define the end --*/
/*--               points of the vector.                                   --*/
/*--                                                                       --*/
/*-- Output[0][0]: Is the first end point of the vector ( Same as Anchor ) --*/
/*--                                                                       --*/
/*-- Output[0][1]: Is the second end point of the vector.  This may not be --*/
/*--               the same as the variable point if snapping is done.     --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct
{
	wgiRubberTypeOf_t	TypeOf;
	wgiDeviceDashed_t	Dashed;
	wgiDeviceDrawOp_t	DrawOp;
	unsigned long		ColorI;
	unsigned long		PlaneI;
	WLDcrd			Anchor;
	WLDcrd			Offset;
	unsigned long		Changed;

	Function		UserFn;
	Function		InitFn;
	Function		CalcFn;
	Function		DrawFn;
	Function		DoneFn;
	void		       *FnData;

	WLDcrd		       *Output[1];
	WLDcrd			Buffer[2];
	VDCcrd			Mapped[2];

	wgiRubberSnapTo_t	SnapTo;

} wgiRubberVector_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberMitred_t -- This type is a sub-structure and is used to draw --*/
/*--                      a mitred rubber-band on the graphics device. The --*/
/*--                      mitred type is a group of 3 or less line segment --*/
/*--                      parts.  In the basic case, there is a horizontal --*/
/*--                      or vertical lead in, followed by a diagonal line --*/
/*--                      segment, and another horizontal or vertical line --*/
/*--                      segment. Any one of these parts may not exist if --*/
/*--                      the position of the variable point dictates that --*/
/*--                      some fringe condition exists, or the behavior is --*/
/*--                      restricted by the Locked or MaxLen members.      --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE BEHAVIOR:                                --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- MaxLen: This is the maximum length ( In world coordinate space ) that --*/
/*--         the diagonal line segment may be. If set to zero the diagonal --*/
/*--         may be of any length.  If there is a maximum length set and a --*/
/*--         user tries to stretch the diagonal,  a horizontal or vertical --*/
/*--         tail will be created.                                         --*/
/*--                                                                       --*/
/*-- Locked: See the  included description  of rubber-band octant locking. --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE OUTPUT:                                  --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Number: This is the number  of end points that define the mitred line --*/
/*--         segments. ( 3 segments would be 4 end points )                --*/
/*--                                                                       --*/
/*-- Length: This is the length ( world coordinate space ) of the diagonal --*/
/*--         line segment, or zero if none exists.                         --*/
/*--                                                                       --*/
/*-- Mitred: This is the index in the Output  array which contains the end --*/
/*--         points of the diagonal line segment. If WGI_RUBBER_MITRED_NOT --*/
/*--         then no diagonal line segment exists.                         --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Output[0]: If set to NULL there were no mitred line segments, else an --*/
/*--            array of (Number) end points that define the line, also an --*/
/*--            array of 2 end points that describe the first line segment --*/
/*--            part.                                                      --*/
/*--                                                                       --*/
/*-- Output[1]: If not set to NULL,  an array of 2 end points which define --*/
/*--            the second line segment part.                              --*/
/*--                                                                       --*/
/*-- Output[2]: If not set to NULL,  an array of 2 end points which define --*/
/*--            the third line segment part.                               --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#define WGI_RUBBER_MITRED_NOT (3) /* No diagonal line segment in mitred type */

typedef struct
{
	wgiRubberTypeOf_t	TypeOf;
	wgiDeviceDashed_t	Dashed;
	wgiDeviceDrawOp_t	DrawOp;
	unsigned long		ColorI;
	unsigned long		PlaneI;
	WLDcrd			Anchor;
	WLDcrd			Offset;
	unsigned long		Changed;

	Function		UserFn;
	Function		InitFn;
	Function		CalcFn;
	Function		DrawFn;
	Function		DoneFn;
	void		       *FnData;

	WLDcrd		       *Output[3];
	WLDcrd			Buffer[4];
	VDCcrd			Mapped[4];

	unsigned long		Mitred;
	unsigned long		Number;
	unsigned long		Locked;
	unsigned long		LockIn;
	double			Length;
	double			MaxLen;

	unsigned long		PrevOct;

} wgiRubberMitred_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberBend90_t -- This type is a sub-structure and is used to draw --*/
/*--                      line with a 90 degree bend, made up of 2 or less --*/
/*--                      line segment parts.                              --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE BEHAVIOR:                                --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Locked: See the  included description  of rubber-band octant locking. --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE OUTPUT:                                  --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Number: This is the number of end points that define the line. If set --*/
/*--         to zero there was no line.                                    --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Output[0]: If set to NULL there is no line, else an array of (Number) --*/
/*--            end points that define the line, also an array of 2 points --*/
/*--            which define the first line segment part.                  --*/
/*--                                                                       --*/
/*-- Output[1]: If not set to NULL,  an array of 2 end points which define --*/
/*--            the second line segment part.                              --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct
{
	wgiRubberTypeOf_t	TypeOf;
	wgiDeviceDashed_t	Dashed;
	wgiDeviceDrawOp_t	DrawOp;
	unsigned long		ColorI;
	unsigned long		PlaneI;
	WLDcrd			Anchor;
	WLDcrd			Offset;
	unsigned long		Changed;

	Function		UserFn;
	Function		InitFn;
	Function		CalcFn;
	Function		DrawFn;
	Function		DoneFn;
	void		       *FnData;

	WLDcrd		       *Output[2];
	WLDcrd			Buffer[3];
	VDCcrd			Mapped[3];
	unsigned long		Number;

	unsigned long		Locked;
	unsigned long		LockIn;

	unsigned long		PrevOct;

} wgiRubberBend90_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberLarc90_t -- This type is a sub-structure and is used to draw --*/
/*--                      lines connected by a 90 degree arc. In the basic --*/
/*--                      case, there is a horizontal or vertical lead in, --*/
/*--                      then a 90 degree arc,  and another horizontal or --*/
/*--                      vertical line.  Any of these parts may not exist --*/
/*--                      if the position  of the variable  point dictates --*/
/*--                      some fringe condition, or the if the behavior is --*/
/*--                      restricted by the Locked or MaxRad members.      --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE BEHAVIOR:                                --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- MaxRad: This is the  maximum radius ( world coordinate space ) of the --*/
/*--         90 degree arc.  If set to zero the  arc may be of any radius. --*/
/*--         If there is a  maximum set and the user  tries to stretch the --*/
/*--         arc,  a horizontal or vertical tail will be created.          --*/
/*--                                                                       --*/
/*-- Locked: See the  included description  of rubber-band octant locking. --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE OUTPUT:                                  --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Radius: This is the radius of the arc ( world coordinate space ), and --*/
/*--         is set to zero if no arc exists.                              --*/
/*--                                                                       --*/
/*-- StartA: This is the angle ( floating point ) from the anchor point to --*/
/*--         the start point of the arc.  This value is always positive or --*/
/*--         counter clockwise from the 3 o`clock position.                --*/
/*--                                                                       --*/
/*-- SweepA: This is the angle ( floating point ) from the  start point of --*/
/*--         the arc to the end point of the arc.  Is positive for counter --*/
/*--         clockwise, negative for clockwise. ( always 90.0 or -90.0 )   --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Output[0]: This is an array of 2 end points which define the first or --*/
/*--            lead in line segment. If NULL there was no lead in line.   --*/
/*--                                                                       --*/
/*-- Output[1]: This is an array of 3 points that,  along with the angles, --*/
/*--            define the arc. Output[1][0] is the start point of the arc --*/
/*--            Output[1][1] is the center or the arc, and Output[1][2] is --*/
/*--            the end point of the arc. If set to NULL there was no arc. --*/
/*--                                                                       --*/
/*-- Output[2]: This is an array of 2 end points that define the last line --*/
/*--            segment. If set to NULL there was no last line.            --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct
{
	wgiRubberTypeOf_t	TypeOf;
	wgiDeviceDashed_t	Dashed;
	wgiDeviceDrawOp_t	DrawOp;
	unsigned long		ColorI;
	unsigned long		PlaneI;
	WLDcrd			Anchor;
	WLDcrd			Offset;
	unsigned long		Changed;

	Function		UserFn;
	Function		InitFn;
	Function		CalcFn;
	Function		DrawFn;
	Function		DoneFn;
	void		       *FnData;

	WLDcrd		       *Output[3];
	WLDcrd			Buffer[7];
	VDCcrd			Mapped[7];

	unsigned long		Locked;
	unsigned long		LockIn;
	double			MaxRad;
	double			Radius;
	double			StartA;
	double			SweepA;

	unsigned long		PrevOct;

} wgiRubberLarc90_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberLarc45_t -- This type is a sub-structure and is used to draw --*/
/*--                      lines connected by a 45 degree arc. In the basic --*/
/*--                      case, there is a horizontal or vertical lead in, --*/
/*--                      then a 45 degree arc, then a diagonal line.  Any --*/
/*--                      of these parts may  not exist if the position of --*/
/*--                      the variable  point dictates a fringe condition, --*/
/*--                      or if the behavior is restricted by MaxRad.      --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE BEHAVIOR:                                --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- MaxRad: This is the  maximum radius ( world coordinate space ) of the --*/
/*--         45 degree arc.  If set to zero the  arc may be of any radius. --*/
/*--         If there is a  maximum set and the user  tries to stretch the --*/
/*--         arc,  diagonal tail will be created.                          --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE OUTPUT:                                  --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Radius: This is the radius of the arc ( world coordinate space ), and --*/
/*--         is set to zero if no arc exists.                              --*/
/*--                                                                       --*/
/*-- StartA: This is the angle ( floating point ) from the anchor point to --*/
/*--         the start point of the arc.  This value is always positive or --*/
/*--         counter clockwise from the 3 o`clock position.                --*/
/*--                                                                       --*/
/*-- SweepA: This is the angle ( floating point ) from the  start point of --*/
/*--         the arc to the end point of the arc.  Is positive for counter --*/
/*--         clockwise, negative for clockwise. ( always 45.0 or -45.0 )   --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Output[0]: This is an array of 2 end points which define the first or --*/
/*--            lead in line segment. If NULL there was no lead in line.   --*/
/*--                                                                       --*/
/*-- Output[1]: This is an array of 3 points that,  along with the angles, --*/
/*--            define the arc. Output[1][0] is the start point of the arc --*/
/*--            Output[1][1] is the center or the arc, and Output[1][2] is --*/
/*--            the end point of the arc. If set to NULL there was no arc. --*/
/*--                                                                       --*/
/*-- Output[2]: This is an array of 2 end points that define the last line --*/
/*--            segment. If set to NULL there was no last line.            --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct
{
	wgiRubberTypeOf_t	TypeOf;
	wgiDeviceDashed_t	Dashed;
	wgiDeviceDrawOp_t	DrawOp;
	unsigned long		ColorI;
	unsigned long		PlaneI;
	WLDcrd			Anchor;
	WLDcrd			Offset;
	unsigned long		Changed;

	Function		UserFn;
	Function		InitFn;
	Function		CalcFn;
	Function		DrawFn;
	Function		DoneFn;
	void		       *FnData;

	WLDcrd		       *Output[3];
	WLDcrd			Buffer[7];
	VDCcrd			Mapped[7];

	double			MaxRad;
	double			Radius;
	double			StartA;
	double			SweepA;

} wgiRubberLarc45_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberLarc45Rot_t --                                               --*/
/*--                      This type is a sub-structure and is used to draw --*/
/*--                      lines connected by a 45 degree arc. In the basic --*/
/*--                      case, there is a diagonal lead in, then a 45     --*/
/*--                      degree arc, then a vertical or horizontal line.  --*/
/*--                                                                       --*/
/*--                  Any of these parts may  not exist if the position of --*/
/*--                  the variable  point dictates a fringe condition,     --*/
/*--                  or if the behavior is restricted by MaxRad.          --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE BEHAVIOR:                                --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- MaxRad: This is the  maximum radius ( world coordinate space ) of the --*/
/*--         45 degree arc.  If set to zero the  arc may be of any radius. --*/
/*--         If there is a  maximum set and the user  tries to stretch the --*/
/*--         arc,  diagonal tail will be created.                          --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE OUTPUT:                                  --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Radius: This is the radius of the arc ( world coordinate space ), and --*/
/*--         is set to zero if no arc exists.                              --*/
/*--                                                                       --*/
/*-- StartA: This is the angle ( floating point ) from the anchor point to --*/
/*--         the start point of the arc.  This value is always positive or --*/
/*--         counter clockwise from the 3 o`clock position.                --*/
/*--                                                                       --*/
/*-- SweepA: This is the angle ( floating point ) from the  start point of --*/
/*--         the arc to the end point of the arc.  Is positive for counter --*/
/*--         clockwise, negative for clockwise. ( always 45.0 or -45.0 )   --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Output[0]: This is an array of 2 end points which define the first or --*/
/*--            lead in line segment. If NULL there was no lead in line.   --*/
/*--                                                                       --*/
/*-- Output[1]: This is an array of 3 points that,  along with the angles, --*/
/*--            define the arc. Output[1][0] is the start point of the arc --*/
/*--            Output[1][1] is the center or the arc, and Output[1][2] is --*/
/*--            the end point of the arc. If set to NULL there was no arc. --*/
/*--                                                                       --*/
/*-- Output[2]: This is an array of 2 end points that define the last line --*/
/*--            segment. If set to NULL there was no last line.            --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct
{
	wgiRubberTypeOf_t	TypeOf;
	wgiDeviceDashed_t	Dashed;
	wgiDeviceDrawOp_t	DrawOp;
	unsigned long		ColorI;
	unsigned long		PlaneI;
	WLDcrd			Anchor;
	WLDcrd			Offset;
	unsigned long		Changed;

	Function		UserFn;
	Function		InitFn;
	Function		CalcFn;
	Function		DrawFn;
	Function		DoneFn;
	void		       *FnData;

	WLDcrd		       *Output[3];
	WLDcrd			Buffer[7];
	VDCcrd			Mapped[7];

	double			MaxRad;
	double			Radius;
	double			StartA;
	double			SweepA;

} wgiRubberLarc45Rot_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberArc3pt_t -- This type is a sub-structure and is used to draw --*/
/*--                      a 3 point arc. The arc is defined by 2 fixed and --*/
/*--                      one variable point,  which can be either the end --*/
/*--                      or mid point on the arc.  There may be no arc if --*/
/*--                      the position of the variable point would cause a --*/
/*--                      fringe condition ( 3 collinear points ). In this --*/
/*--                      case a line may be generated.                    --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE BEHAVIOR:                                --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Second: This is the second fixed point of a 3 point arc.  It`s use is --*/
/*--         defined by the LastIs member.                                 --*/
/*--                                                                       --*/
/*-- LastIs: This defines if the Second point is an end point or mid point --*/
/*--         point on the arc. Legal values are:                           --*/
/*--                                                                       --*/
/*--         wgiRubberLastIsEnd: Second point is the 3 point arc end point --*/
/*--                             and the variable point is the mid point.  --*/
/*--                                                                       --*/
/*--         wgiRubberLastIsMid: Second point is the 3 point arc mid point --*/
/*--                             and the variable point is the end point.  --*/
/*--                                                                       --*/
/*-- Spokes: This defines some secondary  graphical output for the 3 point --*/
/*--         arc.  Legal values are:                                       --*/
/*--                                                                       --*/
/*--         wgiRubberSpokesScope:  Lines are drawn from the center of the --*/
/*--                                arc to the start, mid, and end points. --*/
/*--                                                                       --*/
/*--         wgiRubberSpokesSlice:  Lines are drawn from the center of the --*/
/*--                                arc to the start and end points.       --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE OUTPUT:                                  --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Radius: This is the radius of the arc ( world coordinate space ), and --*/
/*--         is set to zero if no arc exists.                              --*/
/*--                                                                       --*/
/*-- StartA: This is the angle ( floating point ) from the anchor point to --*/
/*--         the start point of the arc.  This value is always positive or --*/
/*--         counter clockwise from the 3 o`clock position.                --*/
/*--                                                                       --*/
/*-- SweepA: This is the angle ( floating point ) from the  start point of --*/
/*--         the arc to the end point of the arc.  Is positive for counter --*/
/*--         clockwise, negative for clockwise.                            --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Output[0]: This is an array of 3 points that,  along with the angles, --*/
/*--            define the arc. Output[0][0] is the start point of the arc --*/
/*--            Output[0][1] is the center or the arc, and Output[0][2] is --*/
/*--            the end point of the arc. If set to NULL there was no arc. --*/
/*--                                                                       --*/
/*-- Output[1]: If there was no arc and is not NULL, defines a line in the --*/
/*--            case where all 3 points are colliear. An array of 2 points --*/
/*--            that define the end points of this line.                   --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct
{
	wgiRubberTypeOf_t	TypeOf;
	wgiDeviceDashed_t	Dashed;
	wgiDeviceDrawOp_t	DrawOp;
	unsigned long		ColorI;
	unsigned long		PlaneI;
	WLDcrd			Anchor;
	WLDcrd			Offset;
	unsigned long		Changed;

	Function		UserFn;
	Function		InitFn;
	Function		CalcFn;
	Function		DrawFn;
	Function		DoneFn;
	void		       *FnData;

	WLDcrd		       *Output[2];
	WLDcrd			Buffer[6];
	VDCcrd			Mapped[6];

	WLDcrd			Second;
	double			Radius;
	double			StartA;
	double			SweepA;
	wgiRubberLastIs_t	LastIs;
	wgiRubberSpokes_t	Spokes;

} wgiRubberArc3pt_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberCircle_t -- This type is a sub-structure and is used to draw --*/
/*--                      a circle. The anchor is the center of the circle --*/
/*--                      and the variable point is on the perimeter.      --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE BEHAVIOR:                                --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Spokes: This defines some secondary  graphical output for the circle. --*/
/*--         Legal values are:                                             --*/
/*--                                                                       --*/
/*--         wgiRubberSpokesSlice:  A line is drawn from the center of the --*/
/*--                                circle to the variable point.          --*/
/*--                                                                       --*/
/*--         wgiRubberSpokesCross:  Lines are drawn at right angles to one --*/
/*--                                another that bisect the circle with an --*/
/*--                                end point at the variable point.       --*/
/*--                                                                       --*/
/*--         wgiRubberSpokesScope:  Horizontal and vertical lines that bi- --*/
/*--                                sect the circle are drawn.             --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE OUTPUT:                                  --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Radius: This is the radius of the circle  ( world coordinate space )  --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Output[0]: If NULL then there is no circle, else an array of 2 points --*/
/*--            that define the circle.  The first is the center point and --*/
/*--            second is a point on the perimeter. ( really just anchor & --*/
/*--            the variable point )                                       --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct
{
	wgiRubberTypeOf_t	TypeOf;
	wgiDeviceDashed_t	Dashed;
	wgiDeviceDrawOp_t	DrawOp;
	unsigned long		ColorI;
	unsigned long		PlaneI;
	WLDcrd			Anchor;
	WLDcrd			Offset;
	unsigned long		Changed;

	Function		UserFn;
	Function		InitFn;
	Function		CalcFn;
	Function		DrawFn;
	Function		DoneFn;
	void		       *FnData;

	WLDcrd		       *Output[1];
	WLDcrd			Buffer[3];
	VDCcrd			Mapped[3];

	double			Radius;
	wgiRubberSpokes_t	Spokes;

} wgiRubberCircle_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberNshape_t -- This type is a sub-structure and is used to draw --*/
/*--                      an N sided polygon.  The anchor is at the center --*/
/*--                      and a vertex is on the variable point, with each --*/
/*--                      following vertex point distributed evenly, being --*/
/*--                      rotated about the anchor on a 360 degree arc.    --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE BEHAVIOR:                                --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Nsides: This is the number of sides of the N sided polygon.  Can be a --*/
/*--         value between WGI_RUBBER_NSHAPE_MIN and WGI_RUBBER_NSHAPE_MAX --*/
/*--         3 sides defines a triangle, 4 sides is a square, and so on... --*/
/*--                                                                       --*/
/*-- Spokes: This defines some secondary  graphical output for the nshape. --*/
/*--         Legal values are:                                             --*/
/*--                                                                       --*/
/*--         wgiRubberSpokesSlice: Lines are drawn from the anchor to each --*/
/*--                               vertex point on the nshape polygon.     --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE OUTPUT:                                  --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Output[0]: If NULL then there was no nshape polygon, else an array of --*/
/*--            (Nsides + 1) end points that define the vertex list of the --*/
/*--            nshape polygon. ( The extra end point is a last or closing --*/
/*--            point and is the same at the first point )                 --*/
/*--                                                                       --*/
/*-- Output[0..Nsides]: Each entry in the output array can also be used as --*/
/*--            an array of 2 end points which define each line segment of --*/
/*--            the nshape polygon.                                        --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef WGI_RUBBER_NSHAPE_MIN
#define WGI_RUBBER_NSHAPE_MIN	3
#endif
#ifndef WGI_RUBBER_NSHAPE_MAX
#define WGI_RUBBER_NSHAPE_MAX	8
#endif

typedef struct
{
	wgiRubberTypeOf_t	TypeOf;
	wgiDeviceDashed_t	Dashed;
	wgiDeviceDrawOp_t	DrawOp;
	unsigned long		ColorI;
	unsigned long		PlaneI;
	WLDcrd			Anchor;
	WLDcrd			Offset;
	unsigned long		Changed;

	Function		UserFn;
	Function		InitFn;
	Function		CalcFn;
	Function		DrawFn;
	Function		DoneFn;
	void		       *FnData;

	WLDcrd		       *Output[WGI_RUBBER_NSHAPE_MAX];
	WLDcrd			Buffer[WGI_RUBBER_NSHAPE_MAX+2];
	VDCcrd			Mapped[WGI_RUBBER_NSHAPE_MAX+2];

	unsigned long		Nsides;
	wgiRubberSpokes_t	Spokes;

} wgiRubberNshape_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberEllipt_t -- This type is a sub-structure and is used to draw --*/
/*--                      an ellipse inside a bounding box on the graphics --*/
/*--                      device. We can define a non-rotated ellipse just --*/
/*--                      with the center and major and minor axis values, --*/
/*--                      which we (you) can derive from the bounding box. --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE BEHAVIOR:                                --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Aspect: This defines a fixed aspect ratio (y/x) for the bounding box. --*/
/*--         If this value is zero the box can be of any shape. If this is --*/
/*--         set to something like (2.5/5.0), the box will always be twice --*/
/*--         as wide as it is high, as will the ellipse.                   --*/
/*--                                                                       --*/
/*-- BaseIs: This defines where the anchor of the bounding box is.   Legal --*/
/*--         values are:                                                   --*/
/*--                                                                       --*/
/*--         wgiRubberBaseIsCorner: The anchor of the bounding box is at a --*/
/*--                                point at a corner of the box.          --*/
/*--                                                                       --*/
/*--         wgiRubberBaseIsCenter: The anchor of the bounding box is at a --*/
/*--                                point in the center of the box.        --*/
/*--                                                                       --*/
/*-- Spokes: This defines some secondary graphical output for the bounding --*/
/*--         box.  Legal values are:                                       --*/
/*--                                                                       --*/
/*--         wgiRubberSpokesCross: 2 lines are drawn from corner to corner --*/
/*--                               in the bounding box.                    --*/
/*--                                                                       --*/
/*--         wgiRubberSpokesScope: 2 lines are drawn, one a horizontal and --*/
/*--                               the other a vertical bisector.          --*/
/*--                                                                       --*/
/*-- DoRect: This defines some secondary graphical output for the ellipse. --*/
/*--         If set to TRUE, the bounding box will be drawn.               --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE OUTPUT:                                  --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Output[0]:    Is set to NULL if there was no bounding box, else is an --*/
/*--               array of 2 world coordinates  that define the min & max --*/
/*--               corners of the bounding box.                            --*/
/*--                                                                       --*/
/*-- Output[0][0]: This is a bounding box corner,  a world coordinate with --*/
/*--               the smallest or min (x,y) values.                       --*/
/*--                                                                       --*/
/*-- Output[0][1]: This is a bounding box corner,  a world coordinate with --*/
/*--               the largest or max (x,y) values.                        --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct
{
	wgiRubberTypeOf_t	TypeOf;
	wgiDeviceDashed_t	Dashed;
	wgiDeviceDrawOp_t	DrawOp;
	unsigned long		ColorI;
	unsigned long		PlaneI;
	WLDcrd			Anchor;
	WLDcrd			Offset;
	unsigned long		Changed;

	Function		UserFn;
	Function		InitFn;
	Function		CalcFn;
	Function		DrawFn;
	Function		DoneFn;
	void		       *FnData;

	WLDcrd		       *Output[1];
	WLDcrd			Buffer[2];
	VDCcrd			Mapped[2];

	long			DoRect;
	double			Aspect;
	wgiRubberSpokes_t	Spokes;
	wgiRubberBaseIs_t	BaseIs;

} wgiRubberEllipt_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberCloser_t -- This type is a sub-structure and is used to draw --*/
/*--                      a line from the variable point to the closest of --*/
/*--                      a number of user supplied points.                --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE BEHAVIOR:                                --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Points: This is a pointer to an array of user defined end points ( In --*/
/*--         world coordinate space ).  This array must be a valid address --*/
/*--         while rubber banding, ( heap or global memory ) unless DoCopy --*/
/*--         has been set.                                                 --*/
/*--                                                                       --*/
/*-- Number: The number of valid end points contained in the Points array. --*/
/*--                                                                       --*/
/*-- DoCopy: If set to TRUE,  local memory will be allocated and the array --*/
/*--         will be copied.  The Points array may then point to temporary --*/
/*--         or stack memory.                                              --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE OUTPUT:                                  --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Closer: This is the index in the Points array which was found to have --*/
/*--         the closest point to the variable point.                      --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Output[0]: If NULL then there was no closest point,  else an array of --*/
/*--            2 end points which define the line from the variable point --*/
/*--            to the closest point to the variable point.                --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- WARNING: The Anchor member is not used. This type can be though of as --*/
/*--          having a variable anchor list.                               --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct
{
	wgiRubberTypeOf_t	TypeOf;
	wgiDeviceDashed_t	Dashed;
	wgiDeviceDrawOp_t	DrawOp;
	unsigned long		ColorI;
	unsigned long		PlaneI;
	WLDcrd			Anchor;
	WLDcrd			Offset;
	unsigned long		Changed;

	Function		UserFn;
	Function		InitFn;
	Function		CalcFn;
	Function		DrawFn;
	Function		DoneFn;
	void		       *FnData;

	WLDcrd		       *Output[1];
	WLDcrd			Buffer[2];
	VDCcrd			Mapped[2];

	unsigned long		Number;
	unsigned long		Closer;
	unsigned long		DoCopy;
	WLDcrd		       *Points;

} wgiRubberCloser_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberTextEx_t -- This type is a sub-structure and is used to draw --*/
/*--                      a rubber-banded text string constrained inside a --*/
/*--                      bounding box on the graphics device. This box is --*/
/*--                      defined as the text extents.                     --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE BEHAVIOR:                                --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- TextRe: This is a pointer to an array of character and represents the --*/
/*--         text string to be drawn.   This array must be a valid address --*/
/*--         while rubber banding, ( heap or global memory ) unless DoCopy --*/
/*--         has been set.                                                 --*/
/*--                                                                       --*/
/*-- DoCopy: If set to TRUE,  local memory will be allocated and the array --*/
/*--         will be copied.  The TextRe array may then point to temporary --*/
/*--         or stack memory.                                              --*/
/*--                                                                       --*/
/*-- Aspect: This defines a fixed aspect ratio (y/x) for the bounding box. --*/
/*--         If this value is zero the box can be of any shape. If this is --*/
/*--         set to something like (2.5/5.0), the box will always be twice --*/
/*--         as wide as it is high, as will the text string.               --*/
/*--                                                                       --*/
/*-- Spaces: This is the amount of space between characters. It is defined --*/
/*--         as a percentage of the width of each character. ( 0.333 means --*/
/*--         the spacing between  characters is (1/3) of the width of each --*/
/*--         character )                                                   --*/
/*--                                                                       --*/
/*-- DoRect: This defines secondary graphical output for the rubber-banded --*/
/*--         text string.  If set to TRUE, the bounding box will be drawn. --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- THE FOLLOWING MEMBERS DEFINE OUTPUT:                                  --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- ChWide: This is the character width.   ( in world coordinate space )  --*/
/*--                                                                       --*/
/*-- ChHigh: This is the character height.  ( in world coordinate space )  --*/
/*--                                                                       --*/
/*-- Mirror: This is the reflection value of the text string. The user may --*/
/*--         reflect the text by moving the variable point across the axis --*/
/*--         of the anchor point.  See wgiDevice for an explanation of the --*/
/*--         various values for the wgiDeviceMirror_t type.                --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- Output[0]:    If set to NULL there was no text string drawn,  else an --*/
/*--               array of 2 world coordinates  that define the min & max --*/
/*--               corners of the bounding box.                            --*/
/*--                                                                       --*/
/*-- Output[0][0]: This is a bounding box corner,  a world coordinate with --*/
/*--               the smallest or min (x,y) values.                       --*/
/*--                                                                       --*/
/*-- Output[0][1]: This is a bounding box corner,  a world coordinate with --*/
/*--               the largest or max (x,y) values.                        --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct wgiStroke_t
{
	unsigned long		Number;
	unsigned long		Origin;
	struct wgiStroke_t     *Stroke;

} wgiStroke_t;

typedef struct
{
	wgiRubberTypeOf_t	TypeOf;
	wgiDeviceDashed_t	Dashed;
	wgiDeviceDrawOp_t	DrawOp;
	unsigned long		ColorI;
	unsigned long		PlaneI;
	WLDcrd			Anchor;
	WLDcrd			Offset;
	unsigned long		Changed;

	Function		UserFn;
	Function		InitFn;
	Function		CalcFn;
	Function		DrawFn;
	Function		DoneFn;
	void		       *FnData;

	WLDcrd		       *Output[2];
	WLDcrd			Buffer[3];
	VDCcrd			Mapped[3];

	WLDcrd		       *Native;
	VDCcrd		       *Scaled;
	wgiStroke_t	       *Stroke;
	char		       *TextRe;

	double			ChWide;
	double			ChHigh;
	double			Aspect;
	double			Spaces;
	long			DoRect;
	unsigned long		DoCopy;
	unsigned long		Number;
	unsigned long		StrLen;
	wgiDeviceMirror_t	Mirror;

} wgiRubberTextEx_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberMyType_t -- This type is a user defined sub-structure. It is --*/
/*--                      a way to hook user defined functions and data to --*/
/*--                      the rubber-band API.  This sub-structure defines --*/
/*--                      no members for behavior or output.  What it does --*/
/*--                      contain are 4 user defined function pointers and --*/
/*--                      a generic data pointer.  These can be used to do --*/
/*--                      whatever you want. The functions are:            --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- InitFn: This function (pointer) is called to initialize internal data --*/
/*--         data structures.  The call is made from the wgi_rubber_init() --*/
/*--         function. The calling arguments are as follows:               --*/
/*--                                                                       --*/
/*--         long function(                                                --*/
/*--                  void              *FnData,                           --*/
/*--                  wgiRubberStruct_t *Rubber,                           --*/
/*--                  unsigned long      MIndex)                           --*/
/*--                                                                       --*/
/*--         FnData: This is a user a defined pointer ( see Common above ) --*/
/*--                                                                       --*/
/*--         Rubber: This is a pointer to the current rubber-bands control --*/
/*--                 structure.                                            --*/
/*--                                                                       --*/
/*--         MIndex: This is the index of the current rubber-band as it is --*/
/*--                 defined in the manager DoList.                        --*/
/*--                                                                       --*/
/*-- CalcFn: This function (pointer) is called to do internal calculations --*/
/*--         and store them in preparation for the drawing functions. This --*/
/*--         is the function to map the world coordinate representation to --*/
/*--         the viewport coordinate representation.  This call is made by --*/
/*--         the wgi_rubber_move() function.  The calling arguments are as --*/
/*--         follows:                                                      --*/
/*--                                                                       --*/
/*--         long function(                                                --*/
/*--                  void              *FnData,                           --*/
/*--                  wgiRubberStruct_t *Rubber,                           --*/
/*--                  double		 x1, y1,                           --*/
/*--                  double		 x2, y2,                           --*/
/*--                                                                       --*/
/*--         FnData: This is a user a defined pointer ( see Common above ) --*/
/*--                                                                       --*/
/*--         Rubber: This is a pointer to the current rubber-bands control --*/
/*--                 structure.                                            --*/
/*--                                                                       --*/
/*--         x1, y1: This is the (x,y) world coordinate of the rubber-band --*/
/*--                 Anchor point.                                         --*/
/*--                                                                       --*/
/*--         x2, y2: This is the (x,y) world coordinate of the rubber-band --*/
/*--                 variable point, after the Offset has been applied.    --*/
/*--                                                                       --*/
/*-- DrawFn: This function (pointer) is called to do  the draw and un-draw --*/
/*--         of the rubber-band graphics.  The user should use the defined --*/
/*--         ( in wgiDevice.h ) device drawing routines. This call is made --*/
/*--         by wgi_rubber_move(), wgi_rubber_done() and wgi_rubber_zonk() --*/
/*--         functions. The calling arguments are as follows:              --*/
/*--                                                                       --*/
/*--         long function(                                                --*/
/*--                  void              *FnData,                           --*/
/*--                  wgiRubberStruct_t *Rubber,                           --*/
/*--                  unsigned long      DoDraw);                          --*/
/*--                                                                       --*/
/*--         FnData: This is a user a defined pointer ( see Common above ) --*/
/*--                                                                       --*/
/*--         Rubber: This is a pointer to the current rubber-bands control --*/
/*--                 structure.                                            --*/
/*--                                                                       --*/
/*--         DoDraw: This flag is set to TRUE when we are drawing graphics --*/
/*--                 and FALSE when we are to un-draw graphics. This value --*/
/*--                 should be passed to wgi_device_rubber_drawop() to set --*/
/*--                 the drawing flags in the graphics device.             --*/
/*--                                                                       --*/
/*-- DoneFn: This function (pointer) is  called to  terminate any internal --*/
/*--         data structures.  The call is made from the wgi_rubber_done() --*/
/*--         function. The calling arguments are as follows:               --*/
/*--                                                                       --*/
/*--         long function(                                                --*/
/*--                  void              *FnData,                           --*/
/*--                  wgiRubberStruct_t *Rubber,                           --*/
/*--                  unsigned long      MIndex)                           --*/
/*--                                                                       --*/
/*--         FnData: This is a user a defined pointer ( see Common above ) --*/
/*--                                                                       --*/
/*--         Rubber: This is a pointer to the current rubber-bands control --*/
/*--                 structure.                                            --*/
/*--                                                                       --*/
/*--         MIndex: This is the index of the current rubber-band as it is --*/
/*--                 defined in the manager DoList.                        --*/
/*--                                                                       --*/
/*--                                                                       --*/
/*-- If it`s not entirely clear how to use this,   I will include a simple --*/
/*-- example.  Following is a simple rubber-band line that also calculates --*/
/*-- it`s linear length:                                                   --*/
/*--                                                                       --*/
/*-- typedef struct                                                        --*/
/*-- {                                                                     --*/
/*--         WLDcrd        Buffer[2];                                      --*/
/*--         VDCcrd        Mapped[2];                                      --*/
/*--         double        Length;                                         --*/
/*--                                                                       --*/
/*-- } MyStruct;                                                           --*/
/*--                                                                       --*/
/*-- long my_init_function(                                                --*/
/*--                  void              *FnData,                           --*/
/*--                  wgiRubberStruct_t *Rubber,                           --*/
/*--                  unsigned long      MIndex)                           --*/
/*-- {                                                                     --*/
/*--         MyStruct *data;                                               --*/
/*--                                                                       --*/
/*--         if ((data = (MyStruct *)malloc(sizeof(MyStruct))) == NULL)    --*/
/*--         {                                                             --*/
/*--                 fprintf(stderr, "Goofed rubber entry %d \n", MIndex); --*/
/*--                 Rubber->MyType.MyData = NULL;                         --*/
/*--                                                                       --*/
/*--                 return (WGI_PROBLEM);                                 --*/
/*--         }                                                             --*/
/*--                                                                       --*/
/*--         data->Length = 0;                                             --*/
/*--                                                                       --*/
/*--         Rubber->MyType.MyData = (void *)data;                         --*/
/*--                                                                       --*/
/*--         return (WGI_SUCCESS);                                         --*/
/*-- }                                                                     --*/
/*--                                                                       --*/
/*-- long my_calc_function(                                                --*/
/*--                  void              *FnData,                           --*/
/*--                  wgiRubberStruct_t *Rubber,                           --*/
/*--                  double		 x1, y1,                           --*/
/*--                  double		 x2, y2,                           --*/
/*-- {                                                                     --*/
/*--         double dx = (x2 - x1);                                        --*/
/*--         double dy = (y2 - y1);                                        --*/
/*--                                                                       --*/
/*--         MyStruct *data = (MyStruct *)Rubber->MyType.MyData;           --*/
/*--                                                                       --*/
/*--         data->Length = sqrt((dx * dx)+(dy * dy));                     --*/
/*--                                                                       --*/
/*--         data->Buffer[0].x = x1;                                       --*/
/*--         data->Buffer[0].y = y1;                                       --*/
/*--         data->Buffer[1].x = x2;                                       --*/
/*--         data->Buffer[1].y = y2;                                       --*/
/*--                                                                       --*/
/*--         wgi_wldvdc(2, data->Buffer, data->Mapped);                    --*/
/*--                                                                       --*/
/*--         return (WGI_SUCCESS);                                         --*/
/*-- }                                                                     --*/
/*--                                                                       --*/
/*-- long my_draw_function(                                                --*/
/*--                  void              *FnData,                           --*/
/*--                  wgiRubberStruct_t *Rubber,                           --*/
/*--                  unsigned long      DoDraw);                          --*/
/*-- {                                                                     --*/
/*--         MyStruct *data = (MyStruct *)Rubber->MyType.MyData;           --*/
/*--                                                                       --*/
/*--         wgi_device_rubber_drawop(DoDraw,                              --*/
/*--                 Rubber->MyType.ColorI, Rubber->MyType.PlaneI,         --*/
/*--                 Rubber->MyType.DrawOp, Rubber->MyType.Dashed);        --*/
/*--                                                                       --*/
/*--         wgi_device_rubber_vector(                                     --*/
/*--                 data->Mapped[0].x, data->Mapped[0].y,                 --*/
/*--                 data->Mapped[1].x, data->Mapped[1].y);                --*/
/*--                                                                       --*/
/*--         return (WGI_SUCCESS);                                         --*/
/*-- }                                                                     --*/
/*--                                                                       --*/
/*-- long my_done_function(                                                --*/
/*--                  void              *FnData,                           --*/
/*--                  wgiRubberStruct_t *Rubber,                           --*/
/*--                  unsigned long      MIndex)                           --*/
/*-- {                                                                     --*/
/*--         free(Rubber->MyType.MyData);                                  --*/
/*--                                                                       --*/
/*--         Rubber->MyType.MyData = NULL;                                 --*/
/*--                                                                       --*/
/*--         return (WGI_SUCCESS);                                         --*/
/*-- }                                                                     --*/
/*--                                                                       --*/
/*-- ...                                                                   --*/
/*-- wgiRubberStruct_t rubber;                                             --*/
/*-- ...                                                                   --*/
/*-- rubber.MyType.TypeOf = wgiRubberTypeOfMyType;                         --*/
/*-- ...                                                                   --*/
/*-- rubber.MyType.InitFn = (Function)my_init_function;                    --*/
/*-- rubber.MyType.CalcFn = (Function)my_calc_function;                    --*/
/*-- rubber.MyType.DrawFn = (Function)my_draw_function;                    --*/
/*-- rubber.MyType.DoneFn = (Function)my_done_function;                    --*/
/*-- ...                                                                   --*/
/*--                                                                       --*/
/*-- `Nuff said...                                                         --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct
{
	wgiRubberTypeOf_t	TypeOf;
	wgiDeviceDashed_t	Dashed;
	wgiDeviceDrawOp_t	DrawOp;
	unsigned long		ColorI;
	unsigned long		PlaneI;
	WLDcrd			Anchor;
	WLDcrd			Offset;
	unsigned long		Changed;

	Function		UserFn;
	Function		InitFn;
	Function		CalcFn;
	Function		DrawFn;
	Function		DoneFn;
	void		       *FnData;

	void		       *MyData;

} wgiRubberMyType_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberStruct_t -- This type is the rubber-bands control structure, --*/
/*--                      ( really a union ) that contains a sub-structure --*/
/*--                      for each type of rubber-banding we can do plus a --*/
/*--                      Common part  used to access common sub-structure --*/
/*--                      members. The size of a union is the size of it`s --*/
/*--                      largest member,   and members overlay each other --*/
/*--                      in memory. This allows use to use this structure --*/
/*--                      as a polymorphic type.                           --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef union
{
	wgiRubberCommon_t	Common;

	wgiRubberXhairs_t	Xhairs;
	wgiRubberBounds_t	Bounds;
	wgiRubberVector_t	Vector;
	wgiRubberMitred_t	Mitred;
	wgiRubberBend90_t	Bend90;
	wgiRubberLarc90_t	Larc90;
	wgiRubberLarc45_t	Larc45;
	wgiRubberArc3pt_t	Arc3pt;
	wgiRubberCircle_t	Circle;
	wgiRubberNshape_t	Nshape;
	wgiRubberEllipt_t	Ellipt;
	wgiRubberCloser_t	Closer;
	wgiRubberTextEx_t	TextEx;
	wgiRubberMyType_t	MyType;
	wgiRubberLarc45_t	Larc45Rot;

} wgiRubberStruct_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- wgiRubberManage_t -- This type is the rubber-bands manager structure. --*/
/*--                      You should already know (from above) what to use --*/
/*--                      Number and DoList for. The other members are for --*/
/*--                      internal use only.                               --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

typedef struct
{
	long			UnDraw;
	unsigned long		Number;
	unsigned long		MyView;
	wgiRubberStruct_t      *DoList;
	unsigned long		Changed;

} wgiRubberManage_t;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- The rubber-band interface routines. Should be self-documenting... :-) --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#if defined(__cplusplus) || defined(__STDC__)

extern long wgi_rubber_init(
	wgiRubberManage_t      *manager_pointer);

extern long wgi_rubber_move(
	wgiRubberManage_t      *manager_pointer,
	long			wgi_viewport_id,
	WLDcrd		       *cursor_position);

extern long wgi_rubber_zonk(
	wgiRubberManage_t      *manager_pointer,
	long			undraw_graphics);

extern long wgi_rubber_done(
	wgiRubberManage_t      *manager_pointer);

#else

extern long wgi_rubber_init(/* SEE PARAMETERS ABOVE */);
extern long wgi_rubber_move(/* SEE PARAMETERS ABOVE */);
extern long wgi_rubber_zonk(/* SEE PARAMETERS ABOVE */);
extern long wgi_rubber_done(/* SEE PARAMETERS ABOVE */);

#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*--                                                                       --*/
/*-- The End                                                               --*/
/*--                                                                       --*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#if defined(__cplusplus)
}; /* extern "C" */
#endif

#endif /* WGI_RUBBER_H -- Do not add anything after this line !!! */
