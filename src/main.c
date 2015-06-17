#include "usb_api.h"

libusb_device        **gd4_devs; 
libusb_device_handle *gd4_dev_handle; 
libusb_context       *gd4_ctx; 

int main() {
    gd4_ctx = NULL;
	gd4_dev_handle = NULL;
    gd4_usb_init();
	usb_speed_test();
}

