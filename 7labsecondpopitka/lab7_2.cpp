#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#define qu_MsgSize 15
using namespace std;
mqd_t mq;
/*
void sigfunc(int sig) {

  char c;

  if(sig != SIGINT)

  return;

  else {
    mq_close(mq);
    mq_unlink("/queue");
    printf("Checked ctrl+c, all resources are free now and program ended\n");
    exit (0);

  }

} */
int main()
{ 	
	struct mq_attr attr={0,10,qu_MsgSize,0,{0}};
	attr.mq_maxmsg=15;
	int status=0;
	char mydomainbuf[256];
	int s;
	//FILE *filename;
	//sem_t *sem;
	printf("PRogram is started\n");
	mq=mq_open("/queue",O_CREAT|O_RDONLY|O_NONBLOCK,0644,&attr);
	//OLD CODE//
	/*filename = fopen("text.txt", "a");
	if(filename==NULL){
	printf("Error in fopen in 1\n");
	return 1;
	}
	sem = sem_open("/sem", O_CREAT, 0644, 1);
	if(sem==SEM_FAILED){
	printf("error in sem_open in 1\n");
	return 2;
	}
	*/
	//signal(SIGINT,sigfunc); 
	ssize_t result;
	while (true)
	{	
		if(fcntl (STDOUT_FILENO, F_SETFL, O_NONBLOCK)==-1){
		printf("Error in fcntl in 1\n");
		return 3;
		}
		if (read(STDIN_FILENO, &s, 1) == 1) { break;}
		memset(mydomainbuf,'\0',256);
		result=mq_receive(mq,mydomainbuf,15,0);
		if(result>=0){
		printf("Message from qu: %s \n",mydomainbuf);
		}else{
		printf("error in mq_receive\n");
		sleep(1);
		continue;
		}
		sleep(1);
		/*for (int i = 0; i < 5;i++)
		{
			cout << "1" << flush;
			if(fputs("1",filename)==EOF){
			printf("Error in fputs in 1\n");
			return 8;			
			}
			if(fflush(filename)!=0){
			printf("Error in fflush in 1\n");
			return 9;
			}
			sleep(1);
		}
		if(sem_post(sem)==-1){
		printf("Error in sem_post in 1\n");
		return 7;
		}*/
	}

	//if(fclose(filename)!=0){
	//printf("Error in fclose in 1\n");
	//return 5;
	//}
	mq_close(mq);
	mq_unlink("/queue");
	printf("Program 2 ended\n");
	return 0;
}
