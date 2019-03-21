#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "libmain.h"
#include "mqtt_client.h"

#define PIDNUM 300

static pthread_t pid[PIDNUM];
void *func_thread(void *iParam)
{
    int id;
    id = *(int*)(iParam);
    mqtt_init(id);
    return 0;
}

int main(int argc, char* argv)
{
    int loop;

    for(loop = 0; loop < PIDNUM; loop++)
    {
        usleep(1000000);
        if(pthread_create(&pid[loop], PTHREAD_CREATE_JOINABLE, func_thread, &loop) != 0)
        {
            printf("pthread_create error \n");
        }
	}
    while(1){
        sleep(3);
    }
    
    for(loop = 0; loop < PIDNUM; loop ++)
    {
		pthread_cancel(pid[loop]);
		pthread_join(pid[loop],NULL);	
	}
    
    return 0;
}