myapp=zilar

ARCH=x86
ifeq (arm,$(ARCH))
    GCC=arm-bcm2708hardfp-linux-gnueabi-gcc
    GXX=arm-bcm2708hardfp-linux-gnueabi-g++
    INCPATH=-I $(SRCPATH) -I /usr/local/arm/libusb-1.0.19/include
    LIBPATH=-L /usr/local/arm/libusb-1.0.19/lib
else
    GCC=gcc
    GXX=g++
    INCPATH=-I $(SRCPATH) -I /usr/local/include
    LIBPATH=-L /usr/local/lib
endif
VER=1.0
SRCPATH=./src
LIB=-lusb-1.0
VPATH=$(SRCPATH)

CCFLAGS= -g
OBJ= main.o usb_api.o thread_mode.o
default:$(OBJ)
	@echo ARCH=$(ARCH)
	$(GCC) $(INCPATH) $(LIBPATH) $(LIB) -lpthread $(CCFLAGS) -o $(myapp) $(OBJ)
main.o: $(SRCPATH)/main.c $(SRCPATH)/gd4.h $(SRCPATH)/usb_api.h $(SRCPATH)/thread_mode.h
	$(GCC) $(INCPATH)  $(CCFLAGS) -c $(SRCPATH)/main.c
usb_api.o: $(SRCPATH)/usb_api.c $(SRCPATH)/usb_api.h
	$(GCC) $(INCPATH)  $(CCFLAGS) -c $(SRCPATH)/usb_api.c
thread_mode.o: $(SRCPATH)/thread_mode.c $(SRCPATH)/thread_mode.h
	$(GCC) $(INCPATH)  $(CCFLAGS) -c $(SRCPATH)/thread_mode.c
clean:
	@rm -rf *.o $(myapp)
install:
	@cp ./$(myapp) /nfsroot
tar:
	@rm -rf *.o $(myapp)
	tar zcvf /mnt/hgfs/share/$(myapp).tar.gz ../gd4-usb2
