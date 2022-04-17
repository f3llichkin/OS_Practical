#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
int main(int argc, char* argv[])
{
	pid_t pid;
	printf("process Lab4_1 pid: %d\n", getpid());
	printf("parent Lab4_1 pid: %d\n", getppid());
	for(int i=0; i<=argc;i++){
	printf("%s\n", argv[i]);
	sleep(1);
	}
	
	
	switch(pid){
	case -1:
		exit(1);
	case 0: 
		exit(37);
	default:
		exit(0);
	}
}
	
