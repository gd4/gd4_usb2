/*****************************************************************************
 * desc: GD4 linux software 
 * authoe: woodwind
 * date: 2015/08/02
 * email:zhaop@ccede.com
 ****************************************************************************/
#include "usb_api.h"
#include "gd4.h"
#include "thread_mode.h"

#define STR_BUFFSIZE 5
libusb_device        **gd4_devs; 
libusb_device_handle *gd4_dev_handle; 
libusb_context       *gd4_ctx; 

void gd4_cmd_help() {
    printf("GD4 command:\n");
    printf("  da   syntax: da,[channle],[voltage]\n");
    printf("       -- for example <da,1,1000> setup 1 channel valtage 1000mV.\n");
    printf("  ad   syntax: ad,[channel]:\n");
    printf("       -- for example <ad,1> fetch AD 1 channel valtage.\n");
    printf("  led  syntax: led,[value]\n");
    printf("       -- for example <led,3> led0 & led1 turn on while led2 & led3 turn off.\n");
    printf("  mode syntax: mode [value]\n");
    printf("       -- set up work mode.\n");
    printf("       -- for example mode ad 1.\n");
    printf("       -- value = 0 setup AD single sample work mode .\n");
    printf("       -- value = 1 setup AD continue sample work mode .\n");
    printf("  exit syntax: exit\n");
    printf("       -- exit gd4.\n");
    printf("  help syntax: help\n");
    printf("       -- print help infomation\n");
}

void * ad_continue_samp_thread(void * arg) {
    struct ThreadMode *samp_flags = arg; 
	char content[10240]; 
	usb_ad_mode(samp_flags->mode,samp_flags->channel);
    printf("start continue samp.\n\r>");
    while(1) {
	    if(samp_flags->mode == 1) {
            usb_ad_read(samp_flags->channel,samp_flags->fd); 
		}else {
	        usb_ad_mode(samp_flags->mode,samp_flags->channel);
			usb_clr();
			printf("samp thread exit.\n");
            pthread_exit(samp_flags);
		}
	}
}

int main() {
    char buffer[STR_BUFFSIZE];
	char pstr[512];
	char content[10240];
    gd4_ctx = NULL;
	gd4_dev_handle = NULL;
	unsigned long channel,value;
    unsigned long mode = 0;
	channel = 1;
	value = 2000;
	mode = 0;
    struct ThreadMode * tmode;	
	pthread_t tid;
    gd4_usb_init();
	//usb_speed_test();
	//usb_da_op(1,3000);
	tmode = ThreadMode_alloc();
	if(tmode != NULL) {
	    if ((tmode->fd = creat("samp.dat", FILE_MODE)) < 0)
	        printf("creat error\n");
	}

	gd4_cmd_help();
    while (1) {
        printf("\n\r>");
        scanf("%s",pstr) ;    
		switch(pstr[0]) {
		case 'a':
            sscanf(pstr,"ad,%ld",&channel) ;    
			if(mode == 0) {
			    if(usb_ad_op(channel))
			        goto error;
		    }else{
	            usb_ad_mode(tmode->mode,tmode->channel);
				usleep(1000);
                pthread_create(&tid,NULL,ad_continue_samp_thread,(void*)tmode);
				//while(1) {
                //    usb_ad_read(content,tmode->channel,tmode->fd); 
				//}
				
			}
		    //printf("%s,%d",pstr,channel);
	        break;
		case 'd':
            sscanf(pstr,"da,%ld,%ld",&channel,&value) ;    
		    //printf("%s,%ld,%ld",pstr,channel,value);
	        if(usb_da_op(channel,value))
			    goto error;
	        break;
		case 'm':
            sscanf(pstr,"mode,%ld",&mode) ;    
			if(mode == 1) {
			    set_ThreadMode(tmode);
			}else{
			    clr_ThreadMode(tmode);
			}
		    printf("mode:%ld\n\r",tmode->mode);
	        break;
		case 'l':
            sscanf(pstr,"led %ld",&value) ;    
		    printf("led: %ld\n\r",value);
	        break;
		case 'h':
	        gd4_cmd_help();
	        break;
		case 'e':
			clr_ThreadMode(tmode);
	        goto error;
	        break;
	    }
    }
error:
    libusb_close(gd4_dev_handle); //close the device we opened  
    libusb_exit(gd4_ctx); //needs to be called to end the  
	return 0;
}

