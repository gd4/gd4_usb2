#ifndef _USB_API_H_
#define _USB_API_H_

#include <pthread.h>
#include <fcntl.h>

struct Flags {
    unsigned long mode;
	int fd;
}

#endif  //_USB_API_H_
