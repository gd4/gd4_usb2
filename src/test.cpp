#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 
#include<sys/time.h> 
//#include <stdio.h>  
//#include <libusb-1.0/libusb.h>  
  
#define VID 0X04b4
#define PID 0x1004  
#define OUTP 0X02
#define INP  0X86
int tim_subtract(struct timeval *result, struct timeval *x, struct timeval *y);
unsigned char buf1[512];
unsigned char buf[512];
static int device_satus(libusb_device_handle *hd)  
{  
  
    int interface = 0;  
    unsigned char byte;  
    libusb_control_transfer(hd, LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,   LIBUSB_REQUEST_CLEAR_FEATURE, 0,interface,  
           &byte, 1, 5000);  
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
int main() {  
    libusb_device **devs; 
    libusb_device_handle *dev_handle; 
    libusb_context *ctx = NULL; 
    struct   timeval   start,stop,diff;
    int r;
    int count=5000;
    int length[512];
    ssize_t cnt; 
    r = libusb_init(&ctx); 
    if(r < 0) {  
        perror("Init Error\n");
        return 1;  
    }  
     libusb_set_debug(ctx, LIBUSB_LOG_LEVEL_INFO); //set verbosity level to 3, as suggested in the documentation  
     cnt = libusb_get_device_list(ctx, &devs); //get the list of devices  
     if(cnt < 0) {  
         perror("Get Device Error\n"); //there was an error  
          return 1;  
      }  
    printf("%d Devices in list.\n", cnt);  
  
     dev_handle = libusb_open_device_with_vid_pid(ctx, VID, PID); 
    if(dev_handle == NULL)  
         perror("Cannot open device\n");  
   else  
         printf("Device Opened\n");  
       libusb_free_device_list(devs, 1);  
  
      if(libusb_kernel_driver_active(dev_handle, 0) == 1) { //find out if kernel driver is attached  
                  printf("Kernel Driver Active\n");  
                  if(libusb_detach_kernel_driver(dev_handle, 0) == 0) //detach it  
                  printf("Kernel Driver Detached!\n");  
      }  
    r = libusb_claim_interface(dev_handle, 0);  
   if(r < 0) {  
        perror("Cannot Claim Interface\n");  
        return 1;  
              }  
     printf("Claimed Interface\n");  
     memset(buf,0x36,512);
     gettimeofday(&start,0);
   while(count--)
  {
           r= libusb_bulk_transfer(dev_handle,OUTP, buf, 512,length, 200);
          if(r<0){
               perror("error\n");
                }
          else{
           // printf("bulk write success\n");
              }
          /* r= libusb_bulk_transfer(dev_handle, INP, buf1, 512,length, 200);
           if(r<0){
              perror("error\n");
             }
           else{
           // printf("bulk read success  %s\n",buf1);
             }*/
            
  }
    gettimeofday(&stop,0);
    tim_subtract(&diff,&start,&stop);
    printf("offtime:: %d /n",diff.tv_usec+diff.tv_sec*1000000);
    device_satus(dev_handle);
    libusb_close(dev_handle); //close the device we opened  
    libusb_exit(ctx); //needs to be called to end the  
    return 0;  
} 

