TARGET = Manga2PSP
OBJS = main.o  \
	   init.o  \
	   affichage.o  \
	   touches.o  \
	   fichiers.o  \
	   gestion.o  \
	   menu.o  \
	   logo.o  \
	   mp3player.o  \
	   ioapi.o  \
	   unzip.o  	  
	    

INCDIR = 
CFLAGS = -G0 -Wall -O2 
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
STDLIBS= -losl -lpng -lz \
		-lpspsdk -lpspctrl -lpspumd -lpsprtc -lpsppower -lpspgu -lmad -lpspaudiolib -lpspaudio -lm
LIBS=$(STDLIBS)$(YOURLIBS)


EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Manga2PSP
PSP_EBOOT_ICON = ICON0.png

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
