# cobarepo
## Soal 3
Mekanisme fork soal ini : 

[![0001-1.jpg](https://i.postimg.cc/ncZzNJDf/0001-1.jpg)](https://postimg.cc/bdCp24gm)

Full Source Code :
```c
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <wait.h>

void daemonize(){
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

    if ((chdir("/home/bagus/Documents/Modul2/soal3")) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

char *caesarchiper(char *message, int key){
    int i;
    char ch;
    for(i = 0; message[i] != '\0'; ++i){
		ch = message[i];
		if(ch >= 'a' && ch <= 'z'){
			ch = ch + key;
			if(ch > 'z'){
				ch = ch - 'z' + 'a' - 1;
			}
			message[i] = ch;
		}
		else if(ch >= 'A' && ch <= 'Z'){
			ch = ch + key;
			if(ch > 'Z'){
				ch = ch - 'Z' + 'A' - 1;
			}
			message[i] = ch;
		}
	}
    return message;
}

// 3d. Membuat kill bash script
void killer_script(char const *argv[], int pid){
    FILE *file_killer = fopen("killer.sh","w");
    // 3e. Membuat dua mode kill bash
    if(strcmp(argv[1],"-z") == 0){ 
        fprintf(file_killer,"#!/bin/bash\n killall -9 %s\n", argv[0]);
    }
    else if(strcmp(argv[1],"-x") == 0){
        fprintf(file_killer,"#!/bin/bash\n kill -15 %d\n", pid);
    }
    fprintf(file_killer,"rm killer.sh\n");
    fclose(file_killer);
}

int main(int argc, char const *argv[]) {
    daemonize();
    
    killer_script(argv,(int)getpid());

    int i, size;
    char nama_folder[100], path_gambar[250], url[100], time_temp[100];
    time_t rawtime;

    while (1) {
        time (&rawtime);
        struct tm *tmp = localtime(&rawtime);
        strftime(nama_folder,sizeof(nama_folder),"%Y-%m-%d_%X",tmp);
        pid_t child1 = fork();

        if(child1 < 0){
            exit(EXIT_FAILURE);
        }
        else if(child1 == 0){
            pid_t child2 = fork();
            int status1;
            if (child2 < 0){
                exit(EXIT_FAILURE);
            }
            if (child2 == 0){
                // 3a.Membuat folder berdasarkan timestamp
                char *arg_mkdir[] = {"mkdir",nama_folder,NULL};
                execv("/bin/mkdir",arg_mkdir);
            }
            else {
                while (wait(&status1) > 0);
                for (i=1; i<=10; i++){
                    pid_t child3 = fork();
                    if(child3 < 0){
                        exit(EXIT_FAILURE);
                    }
                    if(child3  == 0){
                        time (&rawtime);
                        tmp = localtime(&rawtime);
                        strftime(time_temp,sizeof(time_temp),"%Y-%m-%d_%X",tmp);
                        sprintf(path_gambar, "%s/%s.jpg", nama_folder, time_temp);
                        size = (int)time(NULL);
                        sprintf(url, "https://picsum.photos/%d", (size%1000)+50);
                        char *arg_wget[] = {"wget", "-bq", "-O", path_gambar, url, NULL};
                        execv("/bin/wget",arg_wget); 
                    }
                    sleep(5); 
                }

                // 3c. Membuat status.txt dan diisi dengan hasil enkripsi dari pesan menggunakan algoritma caesarchiper 
                char pesan[100] = "Download Succes";
                caesarchiper(pesan, 5);
                char path_status[150];
                sprintf(path_status,"%s/status.txt", nama_folder);
                FILE *file_status; 
                file_status = fopen(path_status,"w");
                fprintf(file_status,"%s",pesan);
                fclose(file_status);

                // 3c. Membuat zip dan menghapus directory aslinya
                char nama_zip[150];
                sprintf(nama_zip,"%s.zip",nama_folder);
                char *arg_zip[] = {"zip", "-rm", "-q", nama_zip, nama_folder, NULL};
                execv("/usr/bin/zip", arg_zip);
            }
        }
        sleep(40);
    }
}
```
### Soal 3a
Pada soal ini, diperintahkan untuk membuat folder setiap 40 detik dengan format nama sesuai dengan timestamp[YYYY-mm-dd_HH:ii:ss]. 
```c
pid_t child2 = fork();
int status1;
if (child2 < 0){
  exit(EXIT_FAILURE);
}
if (child2 == 0){
  // 3a.Membuat folder berdasarkan timestamp
  char *arg_mkdir[] = {"mkdir",nama_folder,NULL};
  execv("/bin/mkdir",arg_mkdir);
}
```
Jika diperhatikan pada source code di atas, pembuatan directory / folder (mkdir) dibuat di child prosess sedangkan yang terjadi di parent prosesnya adalah mekanisme download hingga membuat zip dan delete directory.

Cara pembuatan nama_folder :
```c
  time (&rawtime);
  struct tm *tmp = localtime(&rawtime);
  strftime(nama_folder,sizeof(nama_folder),"%Y-%m-%d_%X",tmp);
```
localtime() digunakan untuk mendapatkan waktu saat ini sesuai sistem pada tiap komputer.
%Y adalah format untuk tahun, %m adalah format untuk bulan, %d adaklah format untuk tangga %X adalah format untuk waktu sekarang (Jam:Menit:Detik)

### Soal 3b
Pada soal ini, diperintahkan untuk mendownload gambar setiap 5 detik ke setiap folder yang telah dibuat dengan format nama sesuai dengan timestamp[YYYY-mm-dd_HH:ii:ss]. 
```c
 while (wait(&status1) > 0);
  for (i=1; i<=10; i++){
    pid_t child3 = fork();
    if(child3 < 0){
    exit(EXIT_FAILURE);
  }
  if(child3  == 0){
    time (&rawtime);
    tmp = localtime(&rawtime);
    strftime(time_temp,sizeof(time_temp),"%Y-%m-%d_%X",tmp);
    sprintf(path_gambar, "%s/%s.jpg", nama_folder, time_temp);
    size = (int)time(NULL);
    sprintf(url, "https://picsum.photos/%d", (size%1000)+50);
    char *arg_wget[] = {"wget", "-bq", "-O", path_gambar, url, NULL};
    execv("/bin/wget",arg_wget); 
  }
sleep(5); 
```
``` while (wait(&status1) > 0); ``` Digunakan untuk menunggu proses pembuatan folder selesai
Di proses ini terdapat fork. Di parent proses akan mengingatkan child proses setiap 5 detik sekali untuk mendownload file, di parent proses ini juga untuk memberikan waktu jeda untuk download di setiap filenya. Sedangkan di child proses nya terdapat proses penamaan file hingga download gambar. Ketentuan download gambar adalah dengan pixel ukuran (n%1000) + 50 pixel. n merupakan detik Epoch Unix. Detik Epoch Unix adalah jumlah detik yang telah berlalu sejak 1 Januari 1970 (tengah malam UTC / GMT).

"wget -b" untuk melangsugkan proses download pada background
"wget -q" agar proses silent dan tidak mengeluarkan output pada terminal
"wget -O" untuk mendownload gambar sekaligus dengan path dan rename filenya

Output :

[![1619324066306.jpg](https://i.postimg.cc/TP4VdwXs/1619324066306.jpg)](https://postimg.cc/TynLQT1j)

### Soal 3c
Pada soal ini, diperintahkan untuk membuat status.txt dengan isi "Download Succes" yang telah dienkripsi dengan algoritma CaesarChiper key 5. Lalu kemudian zip setiap folder yang memiliki file yang telah didownload, lalu hapus directory/folder aslinya 
```c
char pesan[100] = "Download Succes";
caesarchiper(pesan, 5);
char path_status[150];
sprintf(path_status,"%s/status.txt", nama_folder);
FILE *file_status; 
file_status = fopen(path_status,"w");
fprintf(file_status,"%s",pesan);
fclose(file_status);
```
Setiap status.txt berada dalam setiap folder yang dibuat setelah proses download selesai.

Algoritma CaesarChiper
```c
char *caesarchiper(char *message, int key){
    int i;
    char ch;
    for(i = 0; message[i] != '\0'; ++i){
		ch = message[i];
		if(ch >= 'a' && ch <= 'z'){
			ch = ch + key;
			if(ch > 'z'){
				ch = ch - 'z' + 'a' - 1;
			}
			message[i] = ch;
		}
		else if(ch >= 'A' && ch <= 'Z'){
			ch = ch + key;
			if(ch > 'Z'){
				ch = ch - 'Z' + 'A' - 1;
			}
			message[i] = ch;
		}
	}
    return message;
}
```
Pada algoritma ini intinya mengubah suatu huruf menjadi huruf+key. Misal A dengan key 5 maka huruf A tersebut menjadi F. Jika huruf telah melewati z atau Z maka rumusnya 
ch = ch - z + a - 1 atau ch = ch - Z + A - 1

Bagian untuk zip dan remove filenya 
```c
char nama_zip[150];
sprintf(nama_zip,"%s.zip",nama_folder);
char *arg_zip[] = {"zip", "-rm", "-q", nama_zip, nama_folder, NULL};
execv("/usr/bin/zip", arg_zip);
```
"zip -rm" membuat zip melalui path dan langsung rename file, serta menghapus direktori aslinya
"zip -q" agar proses silent dan tidak mengeluarkan output pada terminal

Output :

[![1619324079821.jpg](https://i.postimg.cc/nL8SH0Y7/1619324079821.jpg)](https://postimg.cc/LJT3DkW6)

[![1619323956640.jpg](https://i.postimg.cc/NM215csj/1619323956640.jpg)](https://postimg.cc/TLGy4B5z)

[![1619324116320.jpg](https://i.postimg.cc/1XxDFjXc/1619324116320.jpg)](https://postimg.cc/YG34K81h)

### Soal 3d
Pada soal ini, diperintahkan untuk membuat killer bash script melalui program c ini. killer ini digunakan untuk terminate daemon prosesnya sesuai dengan argumen yang diinputkan pada argz.  
```c
killer_script(argv,(int)getpid());
```
### Soal 3e
Pada soal ini, diperintahkan untuk membuat killer bash script dalam dua mode. Mode pertama, jika argz == -z maka yang diterminate adalah semua prosesnya seketika dari parent process hingga child proses. Mode kedua, jika argz == -x maka yang diterminate adalah parent process nya saja, sedangkan child processnya tetap berlangsung. Jadi, tidak akan membuat directory lagi, namun proses hanya berhenti setelah proses download, zip, delete directory asli selesai.   
```c
void killer_script(char const *argv[], int pid){
    FILE *file_killer = fopen("killer.sh","w");
    // 3e. Membuat dua mode kill bash
    if(strcmp(argv[1],"-z") == 0){ 
        fprintf(file_killer,"#!/bin/bash\n killall -9 %s\n", argv[0]);
    }
    else if(strcmp(argv[1],"-x") == 0){
        fprintf(file_killer,"#!/bin/bash\n kill -15 %d\n", pid);
    }
    fprintf(file_killer,"rm killer.sh\n");
    fclose(file_killer);
}
```

Error pada soal no 3 :
1. Salah syntax strftime

[![1618586017899.jpg](https://i.postimg.cc/L4ZSD0QK/1618586017899.jpg)](https://postimg.cc/QVsvxSr0)

2. Pembuatan folder tidak berlangsung setiap 40s, melainkan setiap 50s atau dengan kata lain setiap seluruh proses download selesai

[![1618593087293.jpg](https://i.postimg.cc/wvyCW7Vc/1618593087293.jpg)](https://postimg.cc/TLxN12Bp)
