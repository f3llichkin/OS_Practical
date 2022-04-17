#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <chrono>


using namespace std;

pthread_mutex_t mutex;
struct timespec ts;
static pthread_barrier_t barrier;
typedef struct {
    int flag;
    char symbol;
}FlagSym;

void* first(void* arg) {
    int status=1;
    cout << "Start first thread\n" << endl;
    FlagSym* args = (FlagSym*)arg;
    while (args->flag == 0) {
    	if(clock_gettime(CLOCK_REALTIME, &ts)==-1){
    	printf("gettime in first error\n");
    	return (void*)15;
    	}
    	ts.tv_sec+=3;
    	if(pthread_mutex_timedlock(&mutex,&ts)==0){
    	//if(pthread_mutex_trylock(&mutex)==0){
    	
    	for(int i=0;i<5;i++){
        putchar(args->symbol);
        fflush(stdout);
        sleep(1);
        }
    	
        status=pthread_mutex_unlock(&mutex);
        
        if(status!=0){
        printf("Mutex error unlock in first\n");
        return (void*)12;
        }
        sleep(1);
        }
    }
    cout << "End first thread" << endl;
    status = pthread_barrier_wait(&barrier);
    if (status == PTHREAD_BARRIER_SERIAL_THREAD) {
   	printf("First proc wait barier checkpoint\n");
        pthread_barrier_destroy(&barrier);
        
    } else if (status != 0) {
        printf("error wait barrier in thread 1");
        return (void*)7;
    }
    
    pthread_exit((void*)1);
}

void* second(void* arg) {
    int status=1;
    cout << "Start second thread" << endl;
    FlagSym* args = (FlagSym*)arg;
    while (args->flag == 0) {
    	//if(pthread_mutex_trylock(&mutex)==0){
   	 if(clock_gettime(CLOCK_REALTIME, &ts)==-1){
    	printf("gettime in second error\n");
    	return (void*)15;
    	}
    	ts.tv_sec+=3;
    	if(pthread_mutex_timedlock(&mutex,&ts)==0){
    	for(int i=0;i<5;i++){
        putchar(args->symbol);
        fflush(stdout);
        sleep(1);
        }
         
        status=pthread_mutex_unlock(&mutex);
        
        if(status!=0){
        printf("Mutex error unlock in second\n");
        return (void*)12;
        }
        sleep(1);
        }
    }
    cout << "End second thread" << endl;
    status = pthread_barrier_wait(&barrier);
    if (status == PTHREAD_BARRIER_SERIAL_THREAD) {
        printf("Second proc wait barier checkpoint\n");
        pthread_barrier_destroy(&barrier);  
        
    } else if (status != 0) {
        printf("error wait barrier in thread 2");
        return (void*)8;
    }
    
    pthread_exit((void*)2);
}

int main() {
    pthread_t threads[2];
    FlagSym arg1;
    FlagSym arg2;
    arg1.flag = 0;
    arg1.symbol = 'X';
    arg2.flag = 0;
    arg2.symbol = 'Y';
    int error = 0;
    cout << "Program is started..." << endl;
    error=pthread_mutex_init(&mutex, NULL);
    if(error==0){
    printf("Mutex is init\n");
    }else{
    printf("Mutex is not init, error\n");
    return 9;
    }
    pthread_t id1, id2;
    error = pthread_create(&id1, NULL, first, &arg1);
    if (error != 0) {
        cout << "in First thread create error:" << error << endl;
        return 1;
    }
    error = pthread_create(&id2, NULL, second, &arg2);
    if (error != 0) {
        cout << "in Second thread create error:" << error << endl;
        return 2;
    }
    error = pthread_barrier_init(&barrier, NULL, 3);
    if (error != 0) {
        printf("main error: can't init barrier, status = %d\n", error);
        return 3;
    }
    cout << "Barier is init" << endl;
    cout << "IF u want that program will end, press Enter" << endl;
    getchar();
    cout << "Enter is pressed" << endl;
    //int* exitcode;
	
    arg1.flag = 1;
    
    arg2.flag = 1;
    
    error = pthread_barrier_wait(&barrier);
    if (error == PTHREAD_BARRIER_SERIAL_THREAD) {
        printf("Main wait barier checkpoint\n");
    	pthread_barrier_destroy(&barrier);
    	
    } else if (error != 0) {
        printf("error wait barrier in main thread with status = %d\n", error);
        return 4;
    }
    
    //error = pthread_join(id1, (void**)&exitcode);
    
    //if (error != 0){
    //cout << "IN  first thread pthread_join return error:" << strerror(error) << endl;
    //}else printf("exitcode = %p\n", exitcode);
    //error = pthread_join(id2, (void**)&exitcode);
    //if (error != 0) {
    //cout << "IN  second thread pthread_join return error:" << strerror(error) << endl;
    //}else printf("exitcode = %p\n", exitcode);
    error=pthread_mutex_destroy(&mutex);
    if(error==0){
    printf("Mutex destroyed succesful\n");
    }else{
    printf("Mutex destroyed unsuccesful\n");
    return 10;
    }
    printf("End program\n");
    return 0;
}
