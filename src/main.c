#include "usb_api.h"

#define STR_BUFFSIZE 5
libusb_device        **gd4_devs; 
libusb_device_handle *gd4_dev_handle; 
libusb_context       *gd4_ctx; 

void gd4_cmd_help() {
    printf("GD4 command:\n");
    printf("  da,[channle],[voltage] -- for example <da,1,1000> setup 1 channel valtage 1000mV.\n");
    printf("  ad,[channel] -- for example <ad,1> fetch AD 1 channel valtage.\n");
    printf("  led,[value] -- for example <led,3> led0 & led1 turn on while led2 & led3 turn off.\n");
    printf("  exit -- exit gd4.\n");
    printf("  help -- print help infomation\n");
}

int main() {
    char buffer[STR_BUFFSIZE];
	char pstr[512];
    gd4_ctx = NULL;
	gd4_dev_handle = NULL;
	unsigned long channel,value;
	channel = 1;
	value = 2000;
    gd4_usb_init();
	//usb_speed_test();
	//usb_da_op(1,3000);
	gd4_cmd_help();
    while (1) {
        printf(">");
        scanf("%s",pstr) ;    
		switch(pstr[0]) {
		case 'a':
            sscanf(pstr,"ad,%ld",&channel) ;    
			if(usb_ad_op(channel))
			    goto error;
		    //printf("%s,%d",pstr,channel);
	        break;
		case 'd':
            sscanf(pstr,"da,%ld,%ld",&channel,&value) ;    
		    //printf("%s,%ld,%ld",pstr,channel,value);
	        if(usb_da_op(channel,value))
			    goto error;
	        break;
		case 'l':
            sscanf(pstr,"led %ld",&value) ;    
		    printf("%s %ld",pstr,value);
	        break;
		case 'h':
	        gd4_cmd_help();
	        break;
		case 'e':
	        goto error;
	        break;
	    }
    }
error:
    libusb_close(gd4_dev_handle); //close the device we opened  
    libusb_exit(gd4_ctx); //needs to be called to end the  
	return 0;
}

