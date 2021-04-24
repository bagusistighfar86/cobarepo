#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include<dirent.h>
#include<sys/wait.h>
#include<stdio.h>
#include<string.h>

//fungsi untuk mempersingkat fork
void eksekusi(char perintah[],char *arg[]){

        int status;
        pid_t child_id;
        child_id=fork();
        if(child_id==0){
            printf("%s",perintah);
            execv(perintah,arg);
        }
        else{
           ((wait(&status))>0);
        }
}


//buat menghapus string index-4 / .jpg
char* cut(char *arr){
        int n,i;
        char* cuts;
        for(i=0;arr[i]!='\0';i++);
           n=i-4+1;

        if(n<1) return NULL;

        cuts=(char*)malloc(n*sizeof(char));

        for(i=0;i<n-1;i++)
        cuts[i]=arr[i];
        cuts[i]='\0';
        return cuts;
}
int main() {
        pid_t pid1;
        int status;
        pid1 = fork();

        //child tidak berhasil dibuat
        if(pid1<0){
        exit(EXIT_FAILURE);

        }
        //child berhasil dibuat
        if(pid1==0){


        //membuat folder untuk menyimpan ekstrak
        char *argv[]={"mkdir","-p","/home/rizqi/modul2/petshop",NULL};
        eksekusi("/bin/mkdir",argv);

        //while((wait(NULL))>0);
        //unzip pets.zip lalu di extract ke folder petshop
        char *argv2[]={"unzip","-q","/home/rizqi/Downoads/pets.zip","-d","/home/rizqi/modul2/petshop",NULL};
        execv("/usr/bin/unzip",argv2);
        }

        //remove file yg ga penting
        while((wait(NULL))>0);
        printf("coba\n");

        //Membuka direktori yang telah terdapat file ekstrak
        DIR *dir=opendir("/home/rizqi/modul2/petshop");
        struct dirent *dent;
        if(dir!=NULL){
        while((dent=readdir(dir))!=NULL){
                //hidden file
                //Membandingkan string jika terdapat . maka kontinue
                if(strcmp(dent->d_name,".")==0 || strcmp(dent->d_name,"..")==0) continue;
                if(fork()==0)  continue;
                else if(dent->d_type==DT_DIR){
                        char fileName[100]="/home/rizqi/modul2/petshop/";
                        //Menggabungkan folder dengan file yang tidak penting
                        strcat(fileName,dent->d_name);
                        //Menghapus file tidak penting
                        char *argv[]={"rm","-rf",fileName,NULL};
                        eksekusi("/bin/rm",argv);
                        exit(EXIT_SUCCESS);
                }
                char *cutss=cut(dent->d_name); //menghilangkan.jpg
                //membagi string menjadi beberapa bagian dengan pemisah  _
                //untuk foto yg ada 2 pet(dipisahin _)
                char *photos;

                while(photos=strtok_r(cutss,"_",&cutss)){
                        int i=0;
                        char pet[30], pName[30], pAge[30];
                        char *ph=photos;
                        char *photo;
                        //Memisahkan string dengan pemisah ;
                        while(photo=strtok_r(ph,";",&ph)){
                                if(i==0){
                                //buat folder sesuai nama pets
                                char files[80]="/home/rizqi/modul2/petshop/";
                                strcat(files,photo);
                                char *argv[]={"mkdir","-p",files,NULL};
                                eksekusi("/bin/mkdir",argv);
                                strcpy(pet,photo);
                                }
                                if(i==1){
                                //Mengcopy isi dari arrat photo ke array  nama pets
                                strcpy(pName,photo);
                                }
                                if(i==2){
                                //Mencopy isi index 3 ke umurnya pets
                                strcpy(pAge,photo);
                                }
                                i++;
                        }
                        //nama folder setelah dipisah ke masing2 folder
                        while((wait(NULL))>0);
                        char fileku[80];
                        strcpy(fileku,"/home/rizqi/modul2/petshop/");
                        strcat(fileku,dent->d_name);


                        //file destination
                        char dest[80];
                        strcpy(dest,"/home/rizqi/modul2/petshop/");
                        strcat(dest,pet);
                        strcat(dest,"/");
                        strcat(dest,pName);
                        strcat(dest,".jpg");
                        char *argv[]={"cp",fileku,dest,NULL};
                        eksekusi("/bin/cp",argv);

                        //keterangan.txt di masing2 folder pets
                        char file[50];
                        strcpy(file,"/home/rizqi/modul2/petshop/");
                        strcat(file,pet);
                        strcat(file,"/keterangan.txt");

                        //isi file keterangan.txt
                        char ch[50];
                        strcat(ch,"nama : ");
                        strcat(ch,pName);
                        strcat(ch,"\numur: ");
                        strcat(ch,pAge);
                        strcat(ch,"tahun\n\n");


                        //buat keterangan.txt
                        FILE *fp;
                        fp=fopen(file,"a");
                        fputs(ch,fp);
                        fclose(fp);
         }
        //remove setelah pindah ke folder masing2
        char hapus[60]="/home/rizqi/modul2/petshop/";
        strcat(hapus,dent->d_name);
        char *args[]={"rm",hapus,NULL};
        execv("/bin/rm",args);

      }
      closedir(dir);
    }
  return 0;
}
