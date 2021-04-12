#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

int check(char file[]){
  FILE * fp;
  
  if (fp = fopen (file, "r")){
    fclose(fp);
    return 1;
  }
  return 0;
}

int main() {
  pid_t pid, sid;        // Variabel untuk menyimpan PID

  pid = fork();     // Menyimpan PID dari Child Process

  /* Keluar saat fork gagal
  * (nilai variabel pid < 0) */
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  /* Keluar saat fork berhasil
  * (nilai variabel pid adalah PID dari child process) */
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/home/bagus/Documents/Modul2")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
   	
  while (1) {
    int flag;
    char isi[100];
    FILE * file;
    
    flag = check("error.txt");
    if (flag){
      file = fopen("error.txt","r");
      fgets(isi,100,file);
      fclose(file);
    }

    int c=1;
    char str[15];
    while(1){
     sprintf(str,"error.log.%d",c);
     if (check(str)){
       c+=1;
     }
     else
      break;
    }
    
    file = fopen(str,"w");
    fputs(isi, file);
    fclose(file);  
  
    file = fopen("error.txt","w");
    fputs("",file);	
    sleep(10);
  }
}
