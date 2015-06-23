#ifndef _USB_API_H_
#define _USB_API_H_

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 
#include <sys/time.h> 

#define VID   0X04b4
#define PID   0x1004  
#define OUTEP 0X02
#define INEP  0X86

int gd4_usb_init();
int usb_speed_test();
int usb_da_op(unsigned long channel,unsigned long value);
int usb_ad_op(unsigned long channel);

#endif
