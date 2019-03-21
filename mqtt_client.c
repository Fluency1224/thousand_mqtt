#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <mosquitto.h>
#include <uuid/uuid.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define BUF_MAX_LEN 8196
#define BROKERIP    "172.16.23.49"
#define BROKERPORT  1883
#define UUIDBASE    1000

static void connect_callback(struct mosquitto *mosq, void *obj, int result)
{
    // mosquitto_subscribe(mosq, NULL,mycommand, 1);
    // mosquitto_subscribe(mosq, NULL,mygget, 1);
    int m_id=0;
    // mosquitto_publish(mosq, (int *)m_id, mystate, 6,"online", 1, 1);
}
static void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
    printf("topic:%s, message:%s", message->topic, (char *)message->payload);   
}
static void subscribe_callback(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
    // printf("subscribe callback\n");
}
static void log_callback(struct mosquitto *mosq, void *obj, int level, const char *str)
{   
    // printf("log:%s\n",str);
}
static void disconnect_callback(struct mosquitto *mosq, void *obj, int rc)
{
    // printf("disconnect\n");
}
static void publish_callback(struct mosquitto *mosq, void *obj, int mid)
{
    // printf("publish callback\n");
}
int  publish(struct mosquitto *mosq, char *topic,char *str)
{
    int pub_id=0;
    mosquitto_publish(mosq, (int *)&pub_id, topic, strlen(str),str, 0, 0);
    return 0;
}

int mqtt_init( int pid)
{
    int rc = -1;
    bool clean_session = false;
    struct mosquitto *mosq;
    char uuid[32];
    char msg[1024];

    sprintf(uuid,"1a2b3c4d%d",UUIDBASE + pid);
    // printf("%s\n",uuid);
    mosquitto_lib_init();
    mosq = mosquitto_new((const char*)uuid, clean_session, NULL);
    if(mosq){
        mosquitto_connect_callback_set(mosq,connect_callback);
        mosquitto_disconnect_callback_set(mosq,disconnect_callback);
        mosquitto_log_callback_set(mosq,log_callback);
        mosquitto_message_callback_set(mosq,message_callback);
        mosquitto_subscribe_callback_set(mosq,subscribe_callback);
        mosquitto_publish_callback_set(mosq,publish_callback);
        mosquitto_threaded_set(mosq,1);
        // mosquitto_will_set(mosq,mystate,7,"offline",1,1);
    	// mosquitto_will_clear(mosq);
    }else{
        printf("mosquitto new failed\n");
        return -1;
    }  	
    // mosquitto_username_pw_set(mosq,"fluency","fluency12345@");
    mosquitto_connect(mosq, "172.16.23.49", BROKERPORT, 60);
    printf("mosquitto_connect uuid %s\n", uuid);
    sprintf(msg,"{\"head\":{\"ukey\":\"Dalitek\",\"ver\":\"gw_v1.0.0\",\"session\":\"1\",\"method\": 8192,\"src\":\"%s\",\"dest\":\"12345699\",\
        \"ts\":\"2018-10-12 11:38:38\",\"code\": 0},\"body\":{\"method\": 8705,\"devId\":12345678,\"dpId\":1,\"value\":[3,2,5000,1,0,0,0,76,0,0,0,0,0,0,0,0,0,0,0,0]}}"
        ,uuid);
    while(1){
        sleep(1);
        publish(mosq, uuid, msg);
        rc = mosquitto_loop(mosq, -1, 1);
        if(rc){
            sleep(2);
            rc = mosquitto_reconnect(mosq);
            printf("mosquitto_reconnect uuid %s\n",uuid);
        }
    }
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return 0;
}


