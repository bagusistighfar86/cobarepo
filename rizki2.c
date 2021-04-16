#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

int main (){
	pid_t child_id;
	int status;
	time_t t = time (NULL);
	char nama[100];
	struct tm *tmp = localtime(&t);
	strftime(nama,sizeof(nama),"%d-%m-%Y_%X",tmp);
	child_id = fork();

	if(child_id < 0){
		exit(EXIT_FAILURE);
	}
	if(child_id == 0){
		char *argv[] = {"mkdir",nama,NULL};
		execv("/bin/mkdir",argv);
	}
	else{
		while((wait(&status)) > 0);
		char *argv[] = {"cp","-a","/home/rizqi/Downloads/.",nama,NULL};
		execv("/bin/cp",argv);
	}
}
