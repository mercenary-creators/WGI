make (AT&T Bell Laboratories) 3.0.1 04/01/94   ÍE                                                                       
.ONOBJECT  À    .OBJECT      .LCL.INCLUDE      .PFX.INCLUDE      .STD.INCLUDE      .SCAN.IGNORE      .SCAN.NULL      .SCAN.STATE  À    .SCAN.c  À  	  .SCAN.f  À  
  	.SCAN.m4  À    	.SCAN.mk  À    .SCAN.nroff  À    .SCAN.p  À    .SCAN.r  À    	.SCAN.sh  À    
.SCAN.sql  À    .SCAN.F  À    .SCAN.exec.sh  é     (AR)  ©      (F77)       ¬þ
MyWatch.h  À      headers       ÿË[Form.h  ©      
(LDFLAGS)       MWgiColor.h  ©      (YACC)   (  ¥å[()../release/include/Zfile.h ¥å[ (  ¥åY ()../release/include/CCList21.h ¥åY     ¬ý()CCList30.h ¬þ (   ¦().MAKEINIT¶$local T1 T2 T3 T4
if "$(-mam)"
	print info start
end
for T1 $(MAKE_OPTIONS)
	if T1 == "no*"
		T1 := $(T1:/no//)
		T2 :=
	elif T1 == "*=*"
		T1 := $(T1:/=.*//)
		T2 := $(T1:/[^=]*=//)
	else
		T2 := 1
	end
	if ! "$($(T1):V)"
		$(T1) := $(T2)
	end
end
if ! cctype
	cctype := $(CC:N!=*=*)
end
cctype := $(cctype:O=1:P=C) $(cctype:O>1)
if ! CC.PROBE
	CC.PROBE := $(cctype:@P=P=C)
end
.CC.PROBE. : .VIRTUAL $(CC.PROBE)
$(CC.PROBE) : .SPECIAL .DONTCARE
(AR) (AS) (CPP) (CC) (LD) : .CC.PROBE.
if CC.PROBE
	include "$(CC.PROBE)"
else
	error 1 $(cctype): C probe failed -- default assumed
	if ! CC.STDINCLUDE
		CC.STDINCLUDE = /usr/include
	end
	if ! nativepp
		nativepp = -1
	end
end
if "$(instrument)"
	if ! ( instrument.root = "$(instrument:D:N!=.:T=F)" )
		instrument.root := $(OPTDIRS:/:/ /G:X=$(instrument:B)/.:T=F:O=1)
	end
	if ! "$(instrument.root)"
		error 2 $(instrument): not found
	else
		set expandview
		instrument := $(instrument:B)
		CC.READONLY =
		CCFLAGS := $(CCFLAGS:V:N!=-g|-O*|-p*) -g
		if ! ( T1 = "$(INSTRUMENT_$(instrument))" )
			T1 := CC = $(CC:N=*=*) $(instrument:F=U)HOME=$(instrument.root) $(instrument.root)/$(instrument) $(cctype)
		end
		eval
		$(T1:V)
		end
	end
end
if "$(-mam)"
	nativepp = -1
end
CC.PROFILE := $(CC.PROFILE)
if ancestor
	if "$(*.VIEW:O=2:P=I=..)"
		let ancestor = ancestor + 1
	end
	if "$(-mam)"
		T1 = ..
		let T2 = 1
		while T2 < ancestor
			let T2 = T2 + 1
			T1 := ../$(T1)
		end
		print setv INSTALLROOT $(T1)
		T4 =
		for T3 $(ancestor_list)
			if T4
				$(T4) : $(T1)/$(T3)
				T4 =
			else
				T4 := $(T3)
			end
		end
	end
	T1 = ..
	let T2 = 0
	while T2 < ancestor
		let T2 = T2 + 1
		T4 =
		for T3 $(ancestor_list)
			if T4
				$(T4) : $(T1)/$(T3)
				T4 =
			else
				T4 := $(T3)
			end
		end
		T1 := ../$(T1)
	end
elif "$(-mam)"
	print setv INSTALLROOT $(INSTALLROOT:N=$(HOME):?$HOME?$(INSTALLROOT)?)
end
.MAMEDIT. =
if "$(INSTALLROOT:N=..*(/*))"
	.MAMROOT. := $(INSTALLROOT)
else
	.MAMROOT. := $(PWD:C@.*/src/@/src/@:C@/[^/]*@/..@G:C@/@@)
end
.SOURCE.h : $(*.SOURCE.c:N!=.) $(*.SOURCE:N!=.)
.SOURCE .SOURCE.a .SOURCE.c .SOURCE.h : .INSERT .
if CC.PREROOT && "$(CC.PREROOT:P!=I=/)"
	T1 := $(CC.PREROOT) /
	CC.STDINCLUDE := $(T1:X=$$(CC.STDINCLUDE:C@^/@@))
	CC.STDLIB := $(T1:X=$$(CC.STDLIB:C@^/@@))
end
if .PACKAGE. && ( T1 = "$(.PACKAGE.:/.*/lib&$(CC.ARCHIVE)/:A!=.TARGET:/lib\(.*\)\$(CC.ARCHIVE)/\1/)" )
	local D I L
	for L $(T1)
		LDLIBRARIES += $$(*:N=-l$(L)|*(*/)lib$(L)$(CC.ARCHIVE):??-l$(L)?)
	end
	T2 =
	for T3 $(T1)
		I := $(PACKAGE_$(T3)_INCLUDE)
		L := $(PACKAGE_$(T3)_LIB)
		if ( !I || !L )
			if ! ( T1 = "$(PACKAGE_$(T3))" )
				T1 := $(PACKAGE:/:/ /G) $(PATH:/:/ /G:D) $(OPTDIRS:/:/ /G:C%$%/$(T3)%) /usr/local
				T1 := $(T1:C%$%/include/$(T3)%:T=F:O=1:C%/include/$(T3)$%%)
			end
			if T1
				if !I
					I := $(T1)/include
				end
				if !L
					L := $(T1)/lib
				end
			end
		end
		T2 += $(T3)
		T4 =
		if "$(-mam)"
			if "$(INCLUDEDIR:B:N=$(T3))"
				T4 += P
			end
			if I && "$(I:D)" == "$(L:D)"
				print setv PACKAGE_$(T3) $(T4:N=P:?$INSTALLROOT?$(I:D)?)
				print setv PACKAGE_$(T3)_INCLUDE $PACKAGE_$(T3)/$(I:B:S)
				print setv PACKAGE_$(T3)_LIB $PACKAGE_$(T3)/$(L:B:S)
				T4 += I L
			end
		end
		if I
			.SOURCE.h : $(I)
			if "$(-mam)"
				.MAMPACKAGE. += PACKAGE_$(T3)_INCLUDE
				eval
				PACKAGE_$(T3)_INCLUDE = $(I)
				end
				if ! "$(T4:N=I)"
					print setv PACKAGE_$(T3)_INCLUDE $(T4:N=P:?$INSTALLROOT/$(I:B:S)?$(I)?)
				end
			end
		end
		if L
			.SOURCE.a : $(L)
			if "$(-mam)"
				.MAMPACKAGE. += PACKAGE_$(T3)_LIB
				eval
				PACKAGE_$(T3)_LIB = $(L)
				end
				if ! "$(T4:N=L)"
					print setv PACKAGE_$(T3)_LIB $(T4:N=P:?$INSTALLROOT/$(L:B:S)?$(L)?)
				end
			end
		end
	end
	T3 := $(*.SOURCE.h)
	.SOURCE.h : $(CC.STDINCLUDE)
	T4 := $(*.SOURCE.a)
	.SOURCE.a : $(CC.STDLIB)
	if T2
		T2 += .
		T1 := $(*.SOURCE.h:C%.*%$$(T2:C,.*,&/\&,)%)
		.SOURCE.h : .CLEAR . $(T1:P=C)
	end
else
	T3 := $(*.SOURCE.h)
	.SOURCE.h : $(CC.STDINCLUDE)
	T4 := $(*.SOURCE.a)
	.SOURCE.a : $(CC.STDLIB)
end
if "$(-mam)"
	.CC.NOSTDINCLUDE. := $(CC.STDINCLUDE:V:N!=*[()]*|$(T3:@C@ @|@G):C@ @|@G:C@^@N!=@)
	.CC.NOSTDLIB. := $(CC.STDLIB:V:N!=*[()]*|$(T4:@C@ @|@G):C@ @|@G:C@^@N!=@)
else
	.CC.NOSTDINCLUDE. := $(CC.STDINCLUDE:N!=$(T3:@C@ @|@G):C@ @|@G:C@^@N!=@)
	.CC.NOSTDLIB. := $(CC.STDLIB:N!=$(T4:@C@ @|@G):C@ @|@G:C@^@N!=@)
	if T3 = "$(CC.STDLIB:X=...:P=I=$(LIBDIR):O=1:C@/...$@@)"
		.SOURCE.a : .INSERT $(T3)
	end
end
T3 =
if ! "$(CC.DIALECT:N=ANSI)"
	stdarg.h : .SPECIAL .NULL .TERMINAL .DONTCARE .IGNORE
end
if "$(CC.REPOSITORY)"
	.PTR.PREREQS = $(!) $(!.PTR.CHECK)
	if ! "$(*.SOURCE.ptr)"
		.SOURCE.ptr : $(CC.REPOSITORY)
	end
	T3 += $$(.PTR.OPTIONS.)
end
if !nativepp
	T4 := $(cctype:@P=P=C,pp)
	if ! "$(CC.DIALECT:N=LIBPP)" || CPP != "$\(MAKEPP\)"
		(CC) : (CPP)
		if CC.ALTPP.FLAGS || CC.ALTPP.ENV
			T3 += $(CC.ALTPP.FLAGS)
			if CC.ALTPP.ENV
				.BIND : (CC)
				CC := $$(CC.ALTPP.ENV) $(CC:V)
				if "$(CC.REPOSITORY)"
					CCLD := $$(CC.ALTPP.ENV) $(CCLD:V)
				end
			end
		else
			.BIND : (CC)
			CC := $(CC:N=*=*) $$(CCC) -%C+'$(cctype)' -%p+$$(CPP)
			if "$(CC.REPOSITORY)"
				CCLD := $(CCLD:N=*=*) $$(CCC) -%C+'$(cctype)' -%p+$$(CPP)
			end
		end
		if T4
			T3 += -I-D$(T4)
		end
	end
end
if "$(CCFLAGS:N=-I-)" || nativepp && ! "$(CC.DIALECT:N=-I-)"
	T3 += $$(*:A=.SCAN.c:@?$$$(*.SOURCE.%.LCL.INCLUDE:I=$$$$(!$$$$(*:O=1):A=.LCL.INCLUDE|.STD.INCLUDE:P=D):$(.CC.NOSTDINCLUDE.):/^/-I/)??) $$(&:T=D)
	if nativepp > 0
		error 1 local include files may be ignored by the native C preprocessor
	end
else
	if ! prefixinclude
		T3 += -D:noprefix
	end
	T3 += $$(*:A=.SCAN.c:@?$$$(*.SOURCE.%.LCL.INCLUDE:I=$$$$(!$$$$(*:O=1):A=.LCL.INCLUDE:P=D):/^/-I/) -I- $$$(*.SOURCE.%.STD.INCLUDE:I=$$$$(!$$$$(*:O=1):A=.STD.INCLUDE:P=D):$(.CC.NOSTDINCLUDE.):/^/-I/)??) $$(&:T=D)
end
if "$(instrument)"
	T3 += -D_INSTRUMENT_
end
if "$(CCFLAGS:N=-[gG])"
	T3 += -D_TRACE_
end
if T3
	CCFLAGS &= $(T3:V)
end
T3 =
if "$(CC.REPOSITORY)"
	T3 += $$(CCFLAGS:N=-[D]*) $$(.PTR.PREREQS:A=.SCAN.c:@?$$$(*.SOURCE.%.LCL.INCLUDE:I=$$$$(.PTR.PREREQS:A=.LCL.INCLUDE:P=D):/^/-I/) -I- $$$(*.SOURCE.%.LCL.INCLUDE:I=$$$$(.PTR.PREREQS:A=.LCL.INCLUDE|.STD.INCLUDE:P=D):$(.CC.NOSTDINCLUDE.):/^/-I/)??) $$(&.PTR.CHECK:T=D)
	.APPEND.%.COMMAND : .PTR.CHECK .PTR.UPDATE
	T3 := $$(@:V:N=*$\(CC*(LD)\)*:@?$(T3:V)??)
end
.BIND : (FEATUREFLAGS)
FEATUREFLAGS += $$(.FEATURE.REF.)
if ! FEATURE_huh
	(FEATUREFLAGS) : .PARAMETER
end
if "$(-mam)"
	LDFLAGS &= $(T3:V)
else
	LDFLAGS &= $$(*.SOURCE.%.ARCHIVE:I=$$$(*:N=-l*:P=D):$(.CC.NOSTDLIB.):P=A:/^/-L/) $(T3:V)
end
M4FLAGS &= $$(*.SOURCE.%.M4.INCLUDE:I=$$$(!$$$(*:O=1):P=D):/^/-I/) $$(&:T=D)
for T3 .ARGS .MAIN
	T2 := $(~$(T3):V)
	eval
	.ORIGINAL$(T3). = $(T2:V)
	end
	$(T3) : .CLEAR
	for T1 $(T2)
		if "$(T1:A!=.IMMEDIATE|.TARGET)"
			T2 := $(".$(T1:F=U)":A=.IMMEDIATE|.TARGET)
			if T2
				T1 := $(T2)
			end
		end
		$(T3) : $(T1:V)
	end
end
if recurse > 0
	T1 = 0
	T2 =
	while T1 < recurse
		T2 += .SEMAPHORE
		let T1 = T1 + 1
	end
	.RECURSE.SEMAPHORE : $(T2)
end¦ (  ¥åX ()../release/include/CCList20.h ¥åX©      
(M4FLAGS)       ¥å()WgiDevice.h ¥åUÀ     ¥åY../release/include/CCList21.h        .STATE  ©     ü¶Q(FEATUREFLAGS) set cc cc -O -Qpath /usr/add-on/DFM/bin/nmake3.0.1/lib -I-D/usr/add-on/DFM/bin/nmake3.0.1/lib/probe/C/pp/835E4F4F5bincc    : set static -Bstatic : À     ¥åX../release/include/CCList20.h  À     ¥å[../release/include/MyWatch.h       ìñ'().DO.COPY/$(CMP) -s $(*:O=1) $(<) || $(CP) $(*:O=1) $(<)      ¬þCCList21.h       1()wgiDevice.h I     ¬ý()DebugFlag.h ¬þ     ¬ý()Func_obj.h ¬þ     ¬ý()Boolean.h ¬þ
     ¬ý()StringList.h ¬þ     ¬þCCList20.h   (  ¥åZ"()../release/include/StringList.h ¥åZ     ¬ý()wgiSymbol.h ¬þ     ¬ý
()Zfile.h ¬þ     ¬þStringList.h   (  
¥åW ()../release/include/WgiColor.h ¥åW     ÿËE	()Form.h ÿË[À     ¥å[../release/include/Zfile.h  ©      
(FEATURE)       ¬þ

Boolean.h  À     ¥åW../release/include/WgiColor.h       LWgi.h       ¥åUWgiWorld.h   (  ¥å[()../release/include/MyWatch.h ¥å[     IwgiDevice.h       ¬þDebugFlag.h       .ATTRIBUTE       0()WgiRubber.h L     3()wgiStruct.h J     ¬þwgiSymbol.h  ©      
(ARFLAGS)       ¬ý()CCList21.h ¬þ©      (LEXFLAGS)  À     ¥åW../release/include/Boolean.h       2()wgiRubber.h Ié     (AS)  ©      
(CCFLAGS)       ¬ý()CCList20.h ¬þ     ¬ý()C++List.h ¬þ©      (LEX)  ©      (CIAFLAGS)       ¬þZfile.h  À      .CC.PROBE.  À     ìñ'	.DO.COPY  À     ¥åV../release/include/WgiDevice.h       LWgiRubber.h       ¬þCCList30.h       ()cplus_headers ¦ (  ¥åV()../release/include/Wgi.h ¥åU (  !¥åV ()../release/include/WgiWorld.h ¥åV     ¬þ
C++List.h       ¥åUWgiDevice.h  ©      (F77FLAGS)       JwgiStruct.h  À     ¥åZ ../release/include/StringList.h  é    # (CC)  À     ¥åV../release/include/WgiWorld.h  ©      (CIADBFLAGS)  ©      (YACCFLAGS)       /()WgiColor.h MÀ     ¥åS../release/include/wgiDevice.h  À      upd_h       IwgiRubber.h  À     ¥å[../release/include/DebugFlag.h  À     ¥å\../release/include/Form.h   (  %¥åX()../release/include/Boolean.h ¥åW (  '¥åV!()../release/include/WgiDevice.h ¥åV     ¬þFunc_obj.h  À     ¥åU../release/include/wgiSymbol.h  ©      (LDLIBRARIES)  À     ¥åU../release/include/Wgi.h   (   ¦().MAKERULES.LOADlocal T
if "$(MAKE_OPTIONS:N=viewverify)"
	T := 1
else
	T := $(MAKE_OPTIONS:N=viewverify=*:/.*=//)
end
if T && ! "$(*.VIEW:O=2)"
	error $(T) viewpath not set
end
if "$(-mam)"
	.MAMROOT. := $(PWD:C@.*/src/@/src/@:C@/[^/]*@/..@G:C@/@@)
	.MAKE : .MAM.LOAD
end¦      .o   (    ()upd_hVP1=`vp | cut -d":" -f1`
for f in `vp $(UPD_DIR)/%.h`
do
	if [ "`dirname $f`" != "${VP1}/$(UPD_DIR)" ]
	then
		$(CP) -p $f ${VP1}/$(UPD_DIR)
	fi
done¦À     ¥åX../release/include/C++List.h   (  )¥åS!()../release/include/wgiDevice.h ¥åS (  +¥å\!()../release/include/DebugFlag.h ¥å[ (  -¥åU!()../release/include/wgiSymbol.h ¥åU    / ()all ¦ (  2¥åZ ()../release/include/CCList30.h ¥åYé    4 (LD)   (  5¥å\()../release/include/Form.h ¥å\    7 
()headers ¦À     ¥åY../release/include/CCList30.h  À     ¥åW../release/include/WgiRubber.h  ©      
(ASFLAGS)  ©      (CIA)       .()Wgi.h LÀ     ¥åT../release/include/wgiStruct.h  À      cplus_headers  ©      (CCLD)   (  ;¥åZ ()../release/include/Func_obj.h ¥åZÀ     ¥åT../release/include/wgiRubber.h   (  =¥åX()../release/include/C++List.h ¥åX     ¥å()WgiWorld.h ¥åUÀ     ¥åZ../release/include/Func_obj.h   (  ?¥åW!()../release/include/WgiRubber.h ¥åW©      (COATTRIBUTES)   (  A¥åU!()../release/include/wgiStruct.h ¥åT     ¬ý()MyWatch.h ¬þ (   ¦().MAKEDONE$if "$(-mam)"
	print info finish
end¦©      (M4)  é    C (CPP)   (  D¥åT!()../release/include/wgiRubber.h ¥åT    .SCAN  O PN P) P/ P3 P P O k]Qy9Fo&#x['6de P: P; O P8 PX P= P> PB ~b PS O P a}i Ph PW PR PZ O Pc  