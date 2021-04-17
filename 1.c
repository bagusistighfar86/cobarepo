#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <wait.h>
#include <time.h>
#include <string.h>
#include <dirent.h>

void move(char *basepath,char *destination){
	char path[1000];
	pid_t child_id;
	int status;
	struct dirent *dp;
	DIR *dir = opendir(basepath);
	if(!dir){
		return;
	}
	while((dp = readdir(dir)) != NULL){
		if(strcmp(dp->d_name,".") != 0 && strcmp(dp->d_name,"..") !=0){
			strcpy(path,basepath);
			strcat(path,"/");
			strcat(path,dp->d_name);
			child_id = fork();
			if(child_id < 0){
				exit(EXIT_FAILURE);
			}
			if(child_id == 0){
				char *argv[]= {"mv",path,destination,NULL};
				execv("/bin/mv",argv);
			}
		}
	}
	closedir(dir);
}


int main(){
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
	if((chdir("/home/rizqi/modul2")) < 0){
		exit(EXIT_FAILURE);
	}
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	while(1){
		time_t t = time (NULL);
		struct tm *tmp = localtime(&t);
		if(tmp->tm_mon+1 == 4 && tmp->tm_mday == 9 && tmp->tm_hour == 16 && tmp->tm_min == 22 && tmp->tm_sec == 0){
			pid_t child_id1,child_id2,child_id3,child_id4,child_id5;
			pid_t child_id2a,child_id3a,child_id4a,child_id5a;
			pid_t child_id5b;
			int status;
			child_id1 = fork();
			if(child_id1 < 0){
				exit(EXIT_FAILURE);
			}
			if(child_id1 == 0){
				char *argv[] = {"mkdir","/home/rizqi/modul2/Musyik","/home/rizqi/modul2/Fylm","/home/rizqi/modul2/Pyoto",NULL};
				execv("/bin/mkdir",argv);
			}
			else{
				while ((wait(&status)) > 0);
				child_id2 = fork();
				if(child_id2 < 0){
					exit(EXIT_FAILURE);
				}
				if(child_id2 == 0){
					child_id2a = fork();
					if(child_id2a < 0){
						exit(EXIT_FAILURE);
					}
					if(child_id2a == 0){
						//download musik
						char *argv[] = {"wget","-q","--no-check-certificate","https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download","-O","/home/rizqi/modul2/Musik_For_Stevany.zip",NULL};
						execv("/bin/wget",argv);
					}
					else{
						while((wait(&status)) > 0);
						//unzip musik
						char *argv[] = {"unzip","-q","/home/rizqi/modul2/Musik_For_Stevany.zip","-d","/home/rizqi/modul2",NULL};
						execv("/bin/unzip",argv);
					}
				}
				else{
					while((wait(&status)) > 0);
					child_id3 = fork();
					if(child_id3 < 0){
						exit(EXIT_FAILURE);
					}
					if(child_id3 == 0){
						child_id3a = fork();
						if(child_id3a < 0){
							exit(EXIT_FAILURE);
						}
						if(child_id3a == 0){
							//download foto
							char *argv[] = {"wget","-q","--no-check-certificate","https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download","-O","/home/rizqi/modul2/Foto_For_Stevany.zip",NULL};
							execv("/bin/wget",argv);
						}
						else{
							while ((wait(&status)) > 0);
							//unzip foto
							char *argv[] = {"unzip","-q","/home/rizqi/modul2/Foto_For_Stevany.zip","-d","/home/rizqi/modul2",NULL};
							execv("/bin/unzip",argv);
						}
					}
					else{
						while ((wait(&status)) > 0);
						child_id4 = fork();
						if(child_id4 < 0){
							exit(EXIT_FAILURE);
						}
						if(child_id4 == 0){
							child_id4a = fork();
							if(child_id4a < 0){
								exit(EXIT_FAILURE);
							}
							if(child_id4a == 0){
								//download film
								char *argv[] = {"wget","-q","--no-check-certificate","https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download","-O","/home/rizqi/modul2/Film_For_Stevany.zip",NULL};
								execv("/bin/wget",argv);
							}
							else{
								while((wait(&status)) > 0);
								//unzip film
								char *argv[] = {"unzip","-q","/home/rizqi/modul2/Film_For_Stevany.zip","-d","/home/rizqi/modul2",NULL};
								execv("/bin/unzip",argv);
							}
						}
						else{
							while((wait(&status)) > 0);
							child_id5 = fork();
							if(child_id5 < 0){
								exit(EXIT_FAILURE);
							}
							if(child_id5 == 0){
								child_id5a = fork();
								if(child_id5a < 0){
									exit(EXIT_FAILURE);
								}
								if(child_id5a == 0){
									//pindah MUSIK ke Musyik
									//char *argv[] = {"mv","/home/rizqi/modul2/MUSIK/*","/home/rizqi/modul2/Musyik",NULL};
									//execv("/bin/mv",argv);
									move("/home/rizqi/modul2/MUSIK","/home/rizqi/modul2/Musyik");
									_exit(1);
								}
								else{
									while((wait(&status)) > 0);
									child_id5b = fork();
									if(child_id5b < 0){
										exit(EXIT_FAILURE);
									}
									if(child_id5b == 0){
										//pindah FOTO ke Pyoto
										//char *argv[] = {"mv","/home/rizqi/modul2/FOTO/*","/home/rizqi/modul2/Pyoto",NULL};
										//execv("/bin/mv",argv);
										move("/home/rizqi/modul2/FOTO","/home/rizqi/modul2/Pyoto");
										_exit(1);
									}
									else{
										while((wait(&status)) > 0);
										//pindah FILM ke Fylm
										//char *argv[] = {"mv","/home/rizqi/modul2/FILM/*","/home/rizqi/modul2/Fylm",NULL};
										//execv("/bin/mv",argv);
										move("/home/rizqi/modul2/FILM","/home/rizqi/modul2/Fylm");
										_exit(1);
									}
								}
							}
						}
					}
				}
			}
		}
		else if (tmp->tm_mon+1 == 4 && tmp->tm_mday == 9 && tmp->tm_hour == 22 && tmp->tm_min == 22 && tmp->tm_sec == 0){
			pid_t child_id6;
			int status2;
			child_id6 = fork();
			if(child_id6 < 0){
				exit(EXIT_FAILURE);
			}
			if(child_id6 == 0){
				char *argv[] = {"zip","-rmq","Lopyu_Stevany","Musyik","Pyoto","Fylm",NULL};
				execv("/bin/zip",argv);
			}
			else{
				while((wait(&status2)) > 0);
				char *argv[] = {"rm","-r","/home/rizqi/modul2/MUSIK","/home/rizqi/modul2/FOTO","/home/rizqi/modul2/FILM",NULL};
				execv("/bin/rm",argv);
			}
		}
		sleep(1);
	}
}
