#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int baris=4, bk=3, kolom=6;
int matriks1[baris][bk], matriks2[bk][kolom], matriks_hasil[baris * kolom];

void* kali(void* argvar) {
    int* arg = (int*)argvar;
    int i, n = 0;

    for (i = 0; i < bk; i++) 
        n += arg[i] * arg[i + bk];

    int* p = (int*)malloc(sizeof(int));
    *p = n;
    pthread_exit(p);
}

int main() {
    key_t key = 1234;

    int shmid = shmget(key, sizeof(int) * baris * kolom, IPC_CREAT | 0666);
    int* share_hasil = (int*)shmat(shmid, NULL, 0);

    int i, j;
    printf("Matrix %d x %d :\n", baris, bk);
    for (i = 0; i < baris; i++) {
        for (j = 0; j < bk; j++) 
            scanf("%d", &matriks1[i][j]);
    }

    printf("\nMatrix %d x %d :\n", bk, kolom);
    for (i = 0; i < bk; i++) {
        for (j = 0; j < kolom; j++) 
            scanf("%d", &matriks2[i][j]);
    }

    int err, k;
    pthread_t* tid = (pthread_t*)malloc((24) * sizeof(pthread_t));

    int count = 0;
    int* arg = NULL;

    for (i = 0; i < baris; i++) {
        for (j = 0; j < kolom; j++) {
            arg = (int*)malloc((24) * sizeof(int));
            arg[0] = bk;

            for (k = 0; k < bk; k++) {
                arg[k + 1] = matriks1[i][k];
            }
            for (k = 0; k < bk; k++) {
                arg[k + bk + 1] = matriks2[k][j];
            }

            err = pthread_create(&(tid[count++]), NULL, &kali, (void*)arg);
            if (err != 0)
                printf("\n can't create thread : [%s]", strerror(err));
        }
    }

    printf("\nHasil Perkalian :\n");
    for (i = 0; i < baris * kolom; i++) {
        void* k;
        pthread_join(tid[i], &k);
        int* p = (int*)k;

        printf("%d ", *p);
        if ((i + 1) % kolom == 0) {
            printf("\n");
        }
        matriks_hasil[i] = *p;
    }

    for (i = 0; i < baris * kolom; i++) {
        share_hasil[i] = matriks_hasil[i];
    }

    shmdt(share_hasil);
    exit(0);
    return 0;
}
