#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <chrono>
#include <netdb.h>
#include <sys/utsname.h>
#include <fcntl.h>
#define YPMAXDOMAIN 64
using namespace std;
//dev by f3llichkin//Савёлов Александр
pthread_mutex_t mutex;

static pthread_barrier_t barrier;

int pipefd[2];
typedef struct {
    int flag;
    char symbol;
}FlagSym;

void* first(void* arg) {
    const char* domain="test.domain.com";
    int status=1;
    cout << "Start first thread\n" << endl;
    FlagSym* args = (FlagSym*)arg;
    char mydomainbuf[256];//buffer array
    char* mydomain;
    while (args->flag == 0) {
    	status=pthread_mutex_lock(&mutex);
    	if(status!=0){
    	printf("Mutex is not lock in first, error\n");
    	return (void*)9;
    	}
    	//create message//
    	status=getdomainname(mydomainbuf,(int)sizeof(mydomain));   
    	if(status!=0){
    	printf("Error in getdomain\n");
    	return (void*)10;
    	}else printf("getdomain got domain\n");
    	mydomain = strdup(mydomainbuf);
    	if(write(pipefd[1],mydomain,strlen(mydomain))!=strlen(mydomain)){
    	printf("Error in write\n");
    	}
    	sleep(1);
    	//old code//
    	/*for(int i=0;i<5;i++){
        putchar(args->symbol);
        fflush(stdout);
        sleep(1);
        }*/
        
        status=pthread_mutex_unlock(&mutex);
        if(status!=0){
        printf("   Mutex error unlock in first\n");
        return (void*)12;
        }
        sleep(1);
        
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
    char buf[256];//
    while (args->flag == 0) {
    	status=pthread_mutex_lock(&mutex);
    	if(status!=0){
    	printf("Mutex is not lock in second, error\n");
    	return (void*)14;
    	}
    	
    	//reading message//
    	
    	if((status=read(pipefd[0],buf,255))>=0){
    	buf[status]=0;
    	printf("read %d bytes from the pipe, domain name is- %s;\n ",status,buf);
    	}else printf("try read empty or fully pipe\n");
    	//old code//
    	/*for(int i=0;i<5;i++){
        putchar(args->symbol);
        fflush(stdout);
        sleep(1);
        }*/
        
        status=pthread_mutex_unlock(&mutex);
        if(status!=0){
        printf("Mutex error unlock in second\n");
        return (void*)12;
        }
        sleep(1);
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
//dev by f3llichkin//Савёлов Александр
int main() {
    pthread_t threads[2];
    FlagSym arg1;
    FlagSym arg2;
    arg1.flag = 0;
    arg1.symbol = 'X';
    arg2.flag = 0;
    arg2.symbol = 'Y';
    int error = 0;
    //int pipe2_flags = 0;
    //pipe2_flags |= O_NONBLOCK;
    cout << "Program is started..." << endl;
    error=pthread_mutex_init(&mutex, NULL);
    if(error==0){
    printf("Mutex is init\n");
    }else{
    printf("Mutex is not init, error\n");
    return 9;
    }
    error=pipe(pipefd);
    if(error==0) cout << "pipe function is success create a chanel" << error << endl;
    else{
    cout << "pipe function is not create, error" << error << endl;
    return 16;
    }
    error=fcntl(pipefd[0],F_SETFL,O_NONBLOCK);
    if(error==-1){
    printf("Error in main, fcntl, pipefd[0]\n");
    return 22;
    }else printf("fcntl for pipefd[0] success\n");
    error=fcntl(pipefd[1],F_SETFL,O_NONBLOCK);
    if(error==-1){
    printf("Error in main, fcntl, pipefd[1]\n");
    return 23;
    }else printf("fcntl for pipefd[1] success\n");
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
  //dev by f3llichkin//Савёлов Александр  
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
    error=close(pipefd[0]);
    if(error==0) cout << "pipefd[0] is destroyed" << error << endl;
    else{
    cout << "error in pipefd[0] destroy" << error << endl;
    return 17;
    }
    error=close(pipefd[1]);
    if(error==0) cout << "pipefd[1] is destroyed" << error << endl;
    else{
    cout << "error in pipefd[1] destroy" << error << endl;
    return 18;
    }
    printf("End program\n");
    return 0;
}//dev by f3llichkin//Савёлов Александр
