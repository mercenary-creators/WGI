make (AT&T Bell Laboratories) 3.0.1 04/01/94   UTD                                                                  'ÿËEMakefile ûìÈmakerules.mo      UPD_DIRrelease/includeT    
.ONOBJECT  À   .OBJECT      .LCL.INCLUDE      .PFX.INCLUDE      .STD.INCLUDE      .SCAN.IGNORE      .SCAN.NULL      .SCAN.STATE  À    .SCAN.cO|S|
Q|/*|*/||C|
Q|//||\\|LC|
Q|"|"|\\|LQ|
Q|'|'|\\|LQ|
Q|\\|||CS|
D| \# define %|
B| \# if|
E| \# endif|
I| \# include <%>|A.STD.INCLUDE|
I| \# include "%"|A.LCL.INCLUDE|$(prefixinclude:/0//:?M$$$(.PREFIX.INCLUDE.)|??)
I| \# pragma library "%"|A.VIRTUAL|A.ACCEPT|M.LIBRARY.$$(%)| À  	  .SCAN.f I| include '%'|
I| INCLUDE '%'| À  
  	.SCAN.m4
O|M|
I|%| À    	.SCAN.mkQ|/*|*/||CW|
Q|"|"|\\|LQ|
Q|'|'|\\|LQ|
Q|\\|||CS|
B|if|
B| \# if|
E|end|
E| \# endif|
I|include - "%"|A.DONTCARE|M$$(%)|
I|include - %|A.DONTCARE|M$$(%)|
I|include "%"|M$$(%)|
I|include %|M$$(%)|
I| \# include <%>|A.STD.INCLUDE|
I| \# include "%"|A.LCL.INCLUDE|M$$(.PREFIX.INCLUDE.)| À    .SCAN.nroffZI|.so % |
I|,so % |
I|.BP % |M$$(%:/\([\-,0-9]*\)$//)|
I|,BP % |M$$(%:/\([\-,0-9]*\)$//)| À    .SCAN.p6Q|(*|*)||C|
Q|{|}||C|
Q|"|"|\\|LQ|
I| \# include '%'| À    .SCAN.r^I| include '%'|
I| include "%"|
I| include % |
I| INCLUDE '%'|
I| INCLUDE "%"|
I| INCLUDE % | À    	.SCAN.sh'O|S|
Q|#||\\|LCW|
Q|'|'||Q|
Q|\\|||QS| À    
.SCAN.sql
Q|/*|*/||C|
Q|//||\\|LC|
Q|"|"|\\|LQ|
Q|'|'|\\|LQ|
Q|\\|||CS|
I| EXEC SQL include "%"|
I| EXEC SQL include % |M$$(%:/;.*//)|
I| \## include "%"|
I| \## include % |M$$(%:/;.*//)|
I| $ include <%> |
I| $ include "%" |
I| $ include '%' |
I| $ include % |M$$(%:/;.*//)| À    .SCAN.F-$(@.SCAN.sql)
D| \# define %|
B| \# if|
E| \# endif|
I| \# include <%>|A.STD.INCLUDE|
I| \# include "%"|A.LCL.INCLUDE|M$$(.PREFIX.INCLUDE.)|
I| include '%'|
I| INCLUDE '%'| À    .SCAN.exec.sh$(@.SCAN.sh)
I| : include@ % |OX|
I| . % |A.DONTCARE|M$$(%:C%.*[`$&].*%%:C%['"]%%:C%["']$%%)|
I| *< % |A.DONTCARE|M$$(%:C%.*[`$&].*%%:C%['"]%%:C%["']$%%)|       
MyWatch.h  À     headers        Form.h        WgiColor.h  À     ../release/include/CCList21.h  À     ../release/include/CCList20.h  À    
 ../release/include/MyWatch.h        StringList.h  À     ../release/include/Zfile.h        
Boolean.h  À     ../release/include/WgiColor.h        WgiWorld.h      .ATTRIBUTE  À     ../release/include/Boolean.h        Zfile.h  À      	.DO.COPY/$(CMP) -s $(*:O=1) $(<) || $(CP) $(*:O=1) $(<) À     ../release/include/WgiDevice.h        CCList30.h        
C++List.h       .MAIN        WgiDevice.h  À      ../release/include/StringList.h  À     ../release/include/WgiWorld.h  À      upd_hVP1=`vp | cut -d":" -f1`
for f in `vp $(UPD_DIR)/%.h`
do
	if [ "`dirname $f`" != "${VP1}/$(UPD_DIR)" ]
	then
		$(CP) -p $f ${VP1}/$(UPD_DIR)
	fi
done À     ../release/include/wgiDevice.h  À      ../release/include/DebugFlag.h  À    " ../release/include/Form.h        wgiDevice.h        DebugFlag.h        Func_obj.h  À    $ ../release/include/wgiSymbol.h        wgiSymbol.h  À    & ../release/include/Wgi.h        .o  À    ( ../release/include/C++List.h        Wgi.h  À    * ../release/include/CCList30.h  À    , ../release/include/WgiRubber.h  À    . ../release/include/wgiStruct.h        WgiRubber.h  À    0 all        wgiStruct.h  À    3 cplus_headers  À    C ../release/include/wgiRubber.h        CCList21.h        wgiRubber.h  À    E ../release/include/Func_obj.h        CCList20.h    G .SCAN  5 ,?:2 #@ #C # #" #  # #( < # # #/ #0 # #3 #7 #& #% #; #= >+ 4*$9!68B)-. #A #1 	
  