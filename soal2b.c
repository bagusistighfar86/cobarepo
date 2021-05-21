#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int baris = 4, kolom = 6;

int matriks[baris][kolom], matriks_shared[baris][kolom], matriks_baru[baris][kolom];

pthread_t tid[baris * kolom];

typedef struct thread_args {
    int A, B, baris_baru, kolom_baru;
} args;

void *calculator(void *arg) {
    pthread_t id = pthread_self();
    int i;

    for (i = 0; i < baris * kolom; i++) {
        if (pthread_equal(id, tid[i])) {
            args *arg_matriks = (args *)arg;

            matriks[arg_matriks->baris_baru][arg_matriks->kolom_baru] = factorial(arg_matriks->A, arg_matriks->B);

            // printf("A:B,a:b -> res => %d:%d, %d:%d -> %d\n", arg_matriks->A,
            //        arg_matriks->B, arg_matriks->baris_baru, arg_matriks->kolom_baru,
            //        matriks[arg_matriks->baris_baru][arg_matriks->kolom_baru]);
        }
    }
}   

int factorial(int a, int b) {
    int result = 1, i;
    if (a == 0 || b == 0)
        result = 0;
    else if (a >= b)
        for (i = a-b+1; i <= a; i++) 
            result *= i;
    else if (a < b)
        for (i = 1; i <= a; i++) 
            result *= i;

    return result;
}

int main(int argc, char const *argv[]) {
    int i, j, k, err;
    args arg[baris * kolom];

    key_t key = 1234;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    int *result = (int *)shmat(shmid, NULL, 0);

    printf("Matrix A:\n");
    for (i = 0; i < baris; i++) {
        for (j = 0; j < kolom; j++) {
            matriks_shared[i][j] = result[k++];
            printf("%d ", matriks_shared[i][j]);
        }
        printf("\n");
    }

    printf("\nMatrix B:\n");
    for (i = 0; i < baris; i++) {
        for (j = 0; j < kolom; j++) {
            // printf("%d:%d ", i, j);
            scanf("%d", &matriks_baru[i][j]);
            // printf("%d ", matriks_baru[i][j]);
        }
        // printf("\n");
    }

    for (i = 0; i < baris; i++) {
        for (j = 0; j < kolom; j++) {
            arg[i * kolom + j].A = matriks_shared[i][j];
            arg[i * kolom + j].B = matriks_baru[i][j];
            arg[i * kolom + j].baris_baru = i;
            arg[i * kolom + j].kolom_baru = j;
            // printf("%d => %d %d %d %d\n", i * kolom + j, arg[i * kolom + j].A,
            //        arg[i * kolom + j].B, arg[i * kolom + j].baris_baru, arg[i * kolom + j].kolom__baru);

            err = pthread_create(&(tid[i * kolom + j]), NULL, calculator,
                                 &arg[i * kolom + j]);
            if (err) printf("Error");
        }
    }

    for (i = 0; i < baris * kolom; i++) pthread_join(tid[i], NULL);

    printf("\nResult:\n");
    for (i = 0; i < baris; i++) {
        for (j = 0; j < kolom; j++) {
            printf("%d ", matriks[i][j]);
        }
        printf("\n");
    }

    shmdt((void *)result);
    return 0;
}
