#include "usb_api.h"

int tim_subtract(struct timeval *result, struct timeval *x, struct timeval *y);
unsigned char buf1[512];
unsigned char buf[512];
static int device_satus(libusb_device_handle *hd)  
{  
  
    int interface = 0;  
    unsigned char byte;  
    libusb_control_transfer(hd, LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS | 
	                        LIBUSB_RECIPIENT_INTERFACE,   LIBUSB_REQUEST_CLEAR_FEATURE, 
							0,interface,&byte, 1, 5000);  
    printf("status:0x%x\n", byte);    
    return 0;  
} 
int tim_subtract(struct timeval *result, struct timeval *x, struct timeval *y)
{       
    int nsec;
    if( x->tv_sec>y->tv_sec) return -1;
    if((x->tv_sec==y->tv_sec)&&(x->tv_usec>y->tv_usec)) return -1;
    result->tv_sec=(y->tv_sec - x->tv_sec);
    result->tv_usec=(y->tv_usec - x->tv_usec);
    if(result->tv_usec<0)
    { 
        result->tv_sec--;
        result->tv_usec+=1000000;
    }
    return 0;
}  

libusb_device        **gd4_devs; 
libusb_device_handle *gd4_dev_handle; 
libusb_context       *gd4_ctx = NULL; 

int gd4_usb_init() {  
    int r;
    ssize_t cnt; 
    r = libusb_init(&gd4_ctx); 
    if(r < 0) {  
        perror("Init Error\n");
        return 1;  
    }  
	//set verbosity level to 3, as suggested in the documentation  
    libusb_set_debug(gd4_ctx, LIBUSB_LOG_LEVEL_INFO); 
    cnt = libusb_get_device_list(gd4_ctx, &gd4_devs); //get the list of devices  
    if(cnt < 0) {  
        perror("Get Device Error\n"); //there was an error  
        return 1;  
    }  
    printf("%d Devices in list.\n", cnt);  
  
    gd4_dev_handle = libusb_open_device_with_vid_pid(gd4_ctx, VID, PID); 
    if(gd4_dev_handle == NULL)  
        perror("Cannot open device\n");  
    else { 
        printf("Device Opened\n");  
        libusb_free_device_list(gd4_devs, 1);  
	}
  
    if(libusb_kernel_driver_active(gd4_dev_handle, 0) == 1) { //find out if kernel driver is attached  
        printf("Kernel Driver Active\n");  
        if(libusb_detach_kernel_driver(gd4_dev_handle, 0) == 0) //detach it  
            printf("Kernel Driver Detached!\n");  
    }  
    r = libusb_claim_interface(gd4_dev_handle, 0);  
    if(r < 0) {  
        perror("Cannot Claim Interface\n");  
        return 1;  
    }  
    printf("Claimed Interface\n");  
	printf("gd4 initial finish.\n");
	return 0;
}

int usb_speed_test() {
	//start setup command
	//gd4_help();
    struct timeval start,stop,diff;
    int count=5;
    int length[512];
    int r;
    memset(buf,0x36,512);
    gettimeofday(&start,0);
    while(count--)
    {
        r= libusb_bulk_transfer(gd4_dev_handle,OUTEP, buf, 512,length, 500);
        if(r<0){
            perror("error\n");
			return 1;
        }
        else{
            printf("bulk write success\n");
        }
        //r= libusb_bulk_transfer(gd4_dev_handle, INEP, buf1, 512,length, 200);
        //if(r<0){
        //    perror("error\n");
        //}
        //else{
        //    printf("bulk read success  %s\n",buf1);
        //}
            
    }
    gettimeofday(&stop,0);
    tim_subtract(&diff,&start,&stop);
    printf("offtime:: %ld /n",diff.tv_usec+diff.tv_sec*1000000);
    device_satus(gd4_dev_handle);
    libusb_close(gd4_dev_handle); //close the device we opened  
    libusb_exit(gd4_ctx); //needs to be called to end the  
    return 0;  
} 

int usb_da_op(unsigned long channel,unsigned long value) {
    unsigned char buffer[512];
	memset(buffer,0x0,512);
	int length[512];
	int r;
	unsigned long pkthead = 0x5A5A5A5A;
	unsigned long id = 0x5A5A5A5A;
	unsigned long channel_num  = 0;
	unsigned long da_data = 0;

	channel_num = channel;
	da_data = value;

	memcpy(buffer,   &pkthead,4);
	memcpy(buffer+4, &id,4);
	memcpy(buffer+8, &channel_num,4);
	memcpy(buffer+12, &da_data,4);
	if(gd4_dev_handle != NULL) {
	    r = libusb_bulk_transfer(gd4_dev_handle, OUTEP, buffer, 512, length, 5000);
        if(r<0){
            perror("usb trans error!\n");
			return 1;
        } else {
            printf("DA setup channel %ld success.\n",channel);
			return 0;
        }
    } else {
		printf("Please Initial GD4.\n");
	    return 1;
	}
}

static unsigned long Current_AD_Channel;
static unsigned long Current_AD_Value;

int usb_ad_op(unsigned long channel) {
    unsigned char buffer[512];
	memset(buffer,0x0,512);
	int length[512];
	int i = 0;
	int r;
	char head = 0x6A;
	unsigned long pkthead = 0x6A6A6A6A;
	unsigned long id = 0x6A6A6A6A;
	unsigned long value = 0;
	unsigned long channel_num = 0;
	channel_num = channel;
	memcpy(buffer,   &pkthead,4);
	memcpy(buffer+4, &id,4);
	memcpy(buffer+8, &channel_num,4);
	if (gd4_dev_handle != NULL)
	{
	    r = libusb_bulk_transfer(gd4_dev_handle, OUTEP, buffer, 512, length, 2000);
        if(r<0){
            perror("usb trans error!\n");
			return 1;
        } else {
            printf("AD commit read command.\n");
        }
	} else {
	    printf("Please initial GD4!");
	    return 1;
	}
	usleep(10000);   //wait 100ms
	if (gd4_dev_handle != NULL)
	{
	    r = libusb_bulk_transfer(gd4_dev_handle, INEP, buffer, 512, length, 2000);
        if(r<0){
            perror("usb trans error!\n");
			return 1;
        } else {
            printf("AD read channel %ld.\n",channel);
        }
	}
	while(1) {
		if(i > 512) return 1;
	    if(buffer[i] == head) break;
	    i++;
	}
	while(1){
		if(buffer[i] == head) {
			i++;
		} else {
		    memcpy(&channel,buffer+i,4);
		    memcpy(&value,buffer+i+4,4);
		    break;
		}
	}
	Current_AD_Channel = channel;
	Current_AD_Value = value;
	printf("current AD channel is : %ld--voltage is : %ld \n",Current_AD_Channel,Current_AD_Value);
	return 0;
}

