myapp=test

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
INCPATH=-I $(SRCPATH) -I /usr/local/arm/libusb-1.0.19/include -I /usr/local/arm/opencv-lib/include/
LIBPATH=-L /usr/local/arm/libusb-1.0.19/lib -L /usr/local/arm/opencv-lib/lib
LIB=-lusb-1.0 -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio
VPATH=$(SRCPATH)

CCFLAGS= -Wall -g
OBJ= test.o
default:$(OBJ)
	@echo ARCH=$(ARCH)
	$(GXX) $(INCPATH) $(LIBPATH) $(LIB) -lpthread $(CCFLAGS) -o $(myapp) $(OBJ)
test.o: ./src/test.cpp
	$(GXX) $(INCPATH)  $(CCFLAGS) -c $(SRCPATH)/test.cpp
clean:
	@rm -rf *.o $(myapp)
install:
	@cp ./$(myapp) /nfs
tar:
	tar cvf /mnt/hgfs/share/$(myapp).tar ../zilar
