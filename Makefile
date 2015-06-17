myapp=gd4

ARCH=arm
ifeq (arm,$(ARCH))
    GCC=arm-bcm2708hardfp-linux-gnueabi-gcc
    GXX=arm-bcm2708hardfp-linux-gnueabi-g++
else
    GCC=gcc
    GXX=g++
endif
VER=1.0
SRCPATH=./src
INCPATH=-I $(SRCPATH) -I /usr/local/arm/libusb-1.0.19/include
LIBPATH=-L /usr/local/arm/libusb-1.0.19/lib
LIB=-lusb-1.0
VPATH=$(SRCPATH)

CCFLAGS= -g
OBJ= main.o usb_api.o
default:$(OBJ)
	@echo ARCH=$(ARCH)
	$(GCC) $(INCPATH) $(LIBPATH) $(LIB) -lpthread $(CCFLAGS) -o $(myapp) $(OBJ)
main.o: $(SRCPATH)/main.c $(SRCPATH)/usb_api.h
	$(GCC) $(INCPATH)  $(CCFLAGS) -c $(SRCPATH)/main.c
usb_api.o: $(SRCPATH)/usb_api.c $(SRCPATH)/usb_api.h
	$(GCC) $(INCPATH)  $(CCFLAGS) -c $(SRCPATH)/usb_api.c
clean:
	@rm -rf *.o $(myapp)
install:
	@cp ./$(myapp) /nfsroot
tar:
	@rm -rf *.o $(myapp)
	tar zcvf /mnt/hgfs/share/$(myapp).tar.gz ../gd4-usb2
