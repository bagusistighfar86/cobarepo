#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <wait.h>
#include <dirent.h>
#include <time.h>

#define SHIFT 5
#define MAXPHOTO 10

//3a
void makeDirectory(char *name){
    char* argv[] = {"mkdir","-p",name,NULL};
    execv("/bin/mkdir", argv);
}
//3b
void downloadItems(char *name){
    pid_t child1 = fork();
    int status1;

    if(child1 < 0){
        exit(EXIT_FAILURE);
    }

    if(child1 == 0){
        time_t timeDL = time(0);
        struct tm tstruct2;
        char buf2[80];
        tstruct2 = *localtime(&timeDL);
        strftime(buf2,sizeof(buf2),"%Y-%m-%d__%H:%M:%S",&tstruct2);
        
        char dlpath[100];
        strcpy(dlpath,name);
        strcat(dlpath,"/");
        strcat(dlpath,buf2);

        unsigned long epochTime = time(NULL);
        unsigned long pixel = (epochTime % 1000) + 50;
        char address[100] = {"https://picsum.photos/"};
        char spixel[50];
        sprintf(spixel,"%lu",pixel);
        strcat(address,spixel);

        char* argv[] = {"wget","-bq",address,"-O",dlpath,NULL};
        execv("/bin/wget",argv);
    }
}
//3c
void makeStatus(char *name){
    char dir[100];
    strcpy(dir,name);
    strcat(dir,"/");
    strcat(dir,"status.txt");
    FILE *dst = fopen(dir,"w");

    if(dst == NULL){
        exit(EXIT_FAILURE);
    }

    char status[20] = {"Download Success"};
    for(int i = 0; i < strlen(status); i++){
        char ch = status[i];
        if(ch == EOF){
            break;
        }
        
        ch += SHIFT;
        fputc(ch,dst);
    }

    fclose(dst);

}

void zipFiles(char *name){
    char fileName[80];
    strcpy(fileName,name);
    strcat(fileName,".zip");
    char *argv[] = {"zip",fileName,"-rm",name,NULL};
    execv("/bin/zip",argv);
}

// void printDetails(){
//     printf("stdinput:\n\t-z : terminate all process\n\t-x : wait process to finish before terminate");
// }

//3d
void generateBashScript(char const *argv[], int programID){
    FILE *dst = fopen("killer.sh","w");
    fprintf(dst,"#!/bin/bash\n");
    if(strcmp(argv[1],"-z") == 0){
        fprintf(dst,"killall -9 soal3\n");
    }
    else if(strcmp(argv[1],"-x") == 0){
        fprintf(dst,"kill -15 %d\n",programID);
    }
    
    fprintf(dst,"rm killer.sh\n");
    fclose(dst);
}


int main(int argc, char const *argv[])
{
    
    pid_t pid,sid;

    pid = fork();

    if(pid < 0){
        exit(EXIT_FAILURE);
    }

    if(pid > 0){
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if(sid < 0){
        exit(EXIT_FAILURE);
    }

    if((chdir("/home/zaki/Documents/Sisop/Shift2/soal3/")) < 0){
        exit(EXIT_FAILURE);
    }

    generateBashScript(argv,(int)getpid());

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    while(1){
        time_t timeNow = time(0);
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime(&timeNow);
        strftime(buf,sizeof(buf),"%Y-%m-%d__%H:%M:%S",&tstruct);

        pid_t child1 = fork();
        int status1;
        
        if(child1 < 0){
            exit(EXIT_FAILURE);
        }

        if(child1 == 0){
            pid_t child2 = fork();
            int status2;
            if(child2 < 0){
                exit(EXIT_FAILURE);
            }
            if(child2 == 0){
                makeDirectory(buf);
            }
            else{
                while((wait(&status2)) > 0);
                for(int i = 0 ; i < MAXPHOTO; i++){
                    downloadItems(buf);
                    sleep(5);
                }
                makeStatus(buf);
                zipFiles(buf);
            }
        }

        sleep(40);
    }
}
