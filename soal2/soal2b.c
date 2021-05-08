#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

unsigned long long int r3 = 4, c3 = 6;
struct args
{
    unsigned long long int i, j;
};

unsigned long long int matrixA[4][6], matrixB[4][6], hasil[4][6];

void display(unsigned long long int matrix[r3][c3], char *Messsage)
{

    char prompt[100] = {0};
    sprintf(prompt, "\nOutput Matrix %s\n", Messsage);
    printf("%s", prompt);
    unsigned long long int i = 0, j = 0;
    while (i < r3)
    {
        j = 0;
        while (j < c3)
        {
            printf("%llu  ", matrix[i][j]);
            if (j == c3 - 1)
            {
                printf("\n");
            }
            j++;
        }
        i++;
    }
}

void *calculation(void *arg)
{
    unsigned long long int i = ((struct args *)arg)->i, j = ((struct args *)arg)->j;
    if (matrixA[i][j] == 0 || matrixB[i][j] == 0)
    {
        hasil[i][j] = 0;
    }
    else if (matrixA[i][j] < matrixB[i][j])
    {
        unsigned long long int temp = 1;
        for (unsigned long long int k = 1; k <= matrixA[i][j]; k++)
        {
            temp = temp * k;
        }
        hasil[i][j] = temp;
    }
    else if (matrixA[i][j] >= matrixB[i][j])
    {
        unsigned long long int temp = 1;
        for (unsigned long long int k = 1; k <= matrixA[i][j]; k++)
        {
            temp = temp * k;
        }
        unsigned long long int temp2 = 1;
        for (unsigned long long int l = 1; l <= matrixA[i][j] - matrixB[i][j]; l++)
        {
            temp2 = temp2 * l;
        }
        unsigned long long int hasilCalculation = temp / temp2;
        hasil[i][j] = hasilCalculation;
    }

}

int main()
{

    pthread_t tid[r3][c3];
    key_t key = 1337;
    unsigned long long int *value;
    unsigned long long int shmid = shmget(key, 96, IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    unsigned long long int *p = (unsigned long long int *)value;
    memcpy(matrixA, p, 192);

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
    char *message = "Matrix B";
    display(matrixA, message);

    char prompt[100];
    for (unsigned long long int i = 0; i < r3; ++i)
    {
        for (unsigned long long int j = 0; j < c3; ++j)
        {
            sprintf(prompt, "Masukkan B %llu %llu ", i + 1, j + 1);
            printf("%s", prompt);
            scanf("%llu", &matrixB[i][j]);
        }
        printf("\n");
    }

    unsigned long long int i = 0, j = 0;
    while (i < r3)
    {
        j = 0;
        while (j < c3)
        {
            struct args *index = (struct args *)malloc(sizeof(struct args));
            index->i = i, index->j = j;
            pthread_create(&tid[i][j], NULL, &calculation, (void *)index);
            j++;
        }
        i++;
    }

    i = 0, j = 0;

    while (i < r3)
    {
        j = 0;
        while (j < c3)
        {
            pthread_join(tid[i][j], NULL);
            j++;
        }
        i++;
    }
    char *message2 = "Hasil";
    display(hasil, message2);
}