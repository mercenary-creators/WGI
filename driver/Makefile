WGI_OS_MAJOR_VERSION	:COMMAND:
	uname -r | cut -c1

CCFLAGS		+=	-DWGI_OS_MAJOR_VERSION=$(WGI_OS_MAJOR_VERSION)

if "$(WGI_OS_MAJOR_VERSION:N=5)"
/*
MOTIF	=	-L/usr/dt/lib -L/usr/openwin/lib -lXm -lXt -lX11 \
		-lnsl -lgen -lsocket

.SOURCE.h	:	/usr/dt/include /usr/openwin/share/include	\
			../release/include
*/
MOTIF	=	-L/opt4/ICS/Motif/usr/lib -lXm -lXt -lX11 \
		-lnsl -lgen -lsocket dlsym.o

.SOURCE.h	:	/opt4/ICS/Motif/usr/include	\
			../release/include
else
MOTIF	=	-L/usr/add-on/motif1.2.2/usr/lib -lXm -lXt -lX11
.SOURCE.h	:						\
			/usr/add-on/motif1.2.2/usr/include	\
			../release/include
end

FILES		=			\
			driver.o	\
			colors.o	\
			menuio.o	\
			dlsym.o		\
			allocate.o

all	:	compile link

compile	:	$(FILES)

link	:	../bin/driver

../bin/driver : $(FILES) ../release/lib/3.0/libwgi.a
	$(CC) -Bstatic $(*) -o $(<) $(MOTIF) -lm
