#include "thread_mode.h"

struct ThreadMode * ThreadMode_alloc() /* allocate the object */
{
	struct ThreadMode	*tmode;

	if ((tmode = malloc(sizeof(struct ThreadMode))) != NULL) {
	    tmode->mode = 0;
		if (pthread_mutex_init(&tmode->lock, NULL) != 0) {
			free(tmode);
			return(NULL);
		}
		/* ... continue initialization ... */
	}
	return(tmode);
}

void set_ThreadMode(struct ThreadMode *fp) /* set mode to 1 */
{

	pthread_mutex_lock(&fp->lock);
    fp->mode = 1;
    pthread_mutex_unlock(&fp->lock);
}

void clr_ThreadMode(struct ThreadMode *fp) /* clear mode to 0 */
{

	pthread_mutex_lock(&fp->lock);
    fp->mode = 0;
    pthread_mutex_unlock(&fp->lock);
}

