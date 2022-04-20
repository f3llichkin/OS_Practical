#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <signal.h>
using namespace std;
 int fd;
char* addr;
sem_t* sem_read;
sem_t* sem_writer;
void sigfunc(int sig) {

  char c;

  if(sig != SIGINT)

  return;

  else {
    munmap(0,sizeof(char[256]));
    sem_close(sem_writer);
    sem_close(sem_read);
    close(fd);
    printf("Checked ctrl+c, all resources are free now and program ended\n");
    exit (0);

  }

} 
int main()
{ 	
	//shm_unlink("/Sh_memory");
	fd=shm_open("/Sh_memory", O_CREAT|O_RDWR, 0644);
	ftruncate(fd,sizeof(char[256])+1);
	//FILE *filename;
	//sem_t *sem;
	addr=(char*)mmap(0,sizeof(char[256]), PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
	sem_read=sem_open("/sem_read",O_CREAT,0644,0);
	sem_writer=sem_open("/sem_write",O_CREAT,0644,0);
	printf("PRogram is started\n");
	
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
	int s;
	int counter=0;
	signal(SIGINT,sigfunc); 
	while (true)
	{
		
		if(sem_wait(sem_writer)!=0) exit(1);		
		printf("%d)Read: %s\n",counter,addr);
		if(sem_post(sem_read)!=0) exit(1);
		counter++;
		
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
		sleep(1); 
		if(fcntl (STDOUT_FILENO, F_SETFL, O_NONBLOCK)==-1){
		printf("Error in fcntl in 1\n");
		return 3;
		}
		if (read(STDIN_FILENO, &s, 1) == 1) {  
		break;
		
		}
	}
	munmap(0,sizeof(char[256]));
	close(fd);
	if(sem_close(sem_writer)==-1){
	printf("Error in sem_close in 1\n");
	return 6;
	}
	if(sem_close(sem_read)==-1){
	printf("Error in sem_close in 1\n");
	return 6;
	}
	sem_unlink("/Sh_memory");
	sem_unlink("/sem_read");
	sem_unlink("/sem_write");
	//if(fclose(filename)!=0){
	//printf("Error in fclose in 1\n");
	//return 5;
	//}
	printf("Program 1 ended\n");
	return 0;
}
