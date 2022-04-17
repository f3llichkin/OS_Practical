#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <semaphore.h>
using namespace std;

int main()
{ 
	FILE *filename;
	sem_t *sem;
	char symbol;
	printf("PRogram is started\n");
	filename = fopen("text.txt", "a");
	if(filename==NULL){
	printf("Error in fopen in 1\n");
	return 1;
	}
	sem = sem_open("/sem", O_CREAT, 0644, 1);
	if(sem==SEM_FAILED){
	printf("error in sem_open in 1\n");
	return 2;
	}
	
	while (true)
	{
		sem_wait(sem);
		for (int i = 0; i < 5;i++)
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
		}
		if(fcntl (STDOUT_FILENO, F_SETFL, O_NONBLOCK)==-1){
		printf("Error in fcntl in 1\n");
		return 3;
		}
		if (read(STDIN_FILENO, &symbol, 1) == 1) break;
		sleep(1); 
	}
	if(sem_close(sem)==-1){
	printf("Error in sem_close in 1\n");
	return 6;
	}
	sem_unlink("/sem");
	if(fclose(filename)!=0){
	printf("Error in fclose in 1\n");
	return 5;
	}
	printf("Program 1 ended\n");
	return 0;
}
