#ifndef _THREADMODE_H_
#define _THREADMODE_H_

#include <stdlib.h>
#include <pthread.h>
/*
 * Default file access permissions for new files.
 */
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

struct ThreadMode {
	unsigned long mode;
	unsigned long channel;
	int fd;
	pthread_mutex_t lock;
};

struct ThreadMode * ThreadMode_alloc();
void set_ThreadMode(struct ThreadMode *fp); 
void clr_ThreadMode(struct ThreadMode *fp);

#endif  //_THREADMODE_H_

