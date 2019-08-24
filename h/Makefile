all	:	headers cplus_headers upd_h

UPD_DIR = release/include

upd_h	:	.FORCE
	VP1=`vp | cut -d":" -f1`
	for f in `vp $(UPD_DIR)/%.h`
	do
		if [ "`dirname $f`" != "${VP1}/$(UPD_DIR)" ]
		then
			$(CP) -p $f ${VP1}/$(UPD_DIR)
		fi
	done	

headers	:					\
	../release/include/wgiDevice.h	\
	../release/include/wgiRubber.h	\
	../release/include/wgiStruct.h	\
	../release/include/wgiSymbol.h

cplus_headers :					\
	../release/include/Wgi.h \
	../release/include/WgiWorld.h \
	../release/include/WgiDevice.h \
	../release/include/WgiRubber.h \
	../release/include/WgiColor.h \
	../release/include/Boolean.h \
	../release/include/C++List.h \
	../release/include/CCList20.h \
	../release/include/CCList21.h \
	../release/include/CCList30.h \
	../release/include/Func_obj.h \
	../release/include/StringList.h \
	../release/include/MyWatch.h \
	../release/include/Zfile.h \
	../release/include/DebugFlag.h \
	../release/include/Form.h

../release/include/wgiDevice.h :COPY: wgiDevice.h
../release/include/wgiRubber.h :COPY: wgiRubber.h
../release/include/wgiStruct.h :COPY: wgiStruct.h
../release/include/wgiSymbol.h :COPY: wgiSymbol.h

../release/include/Wgi.h	:COPY: Wgi.h
../release/include/WgiWorld.h	:COPY: WgiWorld.h
../release/include/WgiDevice.h	:COPY: WgiDevice.h
../release/include/WgiRubber.h	:COPY: WgiRubber.h
../release/include/WgiColor.h	:COPY: WgiColor.h

../release/include/Boolean.h	:COPY: Boolean.h
../release/include/C++List.h	:COPY: C++List.h
../release/include/CCList20.h	:COPY: CCList20.h
../release/include/CCList21.h	:COPY: CCList21.h
../release/include/CCList30.h	:COPY: CCList30.h
../release/include/Func_obj.h	:COPY: Func_obj.h
../release/include/StringList.h	:COPY: StringList.h

../release/include/MyWatch.h	:COPY: MyWatch.h
../release/include/Zfile.h	:COPY: Zfile.h
../release/include/DebugFlag.h	:COPY: DebugFlag.h
../release/include/Form.h	:COPY: Form.h
