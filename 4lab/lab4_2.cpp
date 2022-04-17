#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#define link "lab1"
#define T1 "123"
#define T2 "256"
#define T3 "313"
int main(int argc, char* argv[])
{

	pid_t pid=fork();
	int error=0;
	int status;
	
	if(pid==0){
	printf("Process lab 4_2 pid: %d\n", getpid());
	error=execlp(link,T1,T2,T3,NULL);
	if(error==-1) {printf("error in execlp"); return 1; }
	}else{
	printf("parent lab4_2 pid: %d\n", getpid());	
	while(waitpid(pid, NULL, WNOHANG)==0) sleep(1);
	if(WIFEXITED(status)) printf("Child exited, status=%d\n", WEXITSTATUS(status));
	else if(WIFSIGNALED(status)) printf("killed by signal %d\n", WTERMSIG(status));
	else if(WIFSTOPPED(status)) printf("stopped by signal %d\n", WSTOPSIG(status));
	}
exit(0);
}

