#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

int r3 =4,c3 =6;
struct args {
  int i,j;
  
};

int matrixA[4][6],matrixB[4][6],hasil[4][6];

void display(int result[r3][c3]) {

   printf("\nOutput Matrix:\n");
   for (int i = 0; i < r3; ++i) {
      for (int j = 0; j < c3; ++j) {
         printf("%d  ", result[i][j]);
         if (j == c3 - 1)
            printf("\n");
      }
   }
}

void *calculation(void* arg) {
    int i = ((struct args*)arg)->i,j = ((struct args*)arg)->j;
    printf("%d vs %d\n",i,j);
    if (matrixA[i][j] == 0 || matrixB[i][j]==0)
    {
        hasil[i][j] = 0;
    }
    else if ( matrixA[i][j] >= matrixB[i][j] )
    {
        int temp=1;
        for (int k = 1; k <= matrixA[i][j]; k++)
        {
            temp = temp * k;
        }
        int temp2=1;
        for (int l = 1; l <= matrixA[i][j]-matrixB[i][j]; l++)
        {
            temp2 = temp2 *l;
        }
        int hasilCalculation = temp/temp2;
        hasil[i][j] =hasilCalculation;
    }else if (matrixA[i][j] < matrixB[i][j])
    {
        int temp=1;
        for (int k = 1; k <=matrixA[i][j]; k++)
        {
            temp = temp * k;
        }
        hasil[i][j] = temp;
    }
    
}


int main(){
    
    pthread_t tid[r3][c3];
    key_t key = 1337;
    int *value;
    int shmid = shmget(key, 96, IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    int* p = (int *)value;
    memcpy(matrixA, p, 96);

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
    
    display(matrixA);

    for (int i = 0; i < r3; ++i) {
      for (int j = 0; j < c3; ++j) {
         printf("Enter a%d%d: ", i + 1, j + 1);
         scanf("%d", &matrixB[i][j]);
      }
      printf("\n");
   }

    for (int i = 0; i < r3; i++) {
        for (int j = 0; j < c3; j++) {
            struct args *index = (struct args *)malloc(sizeof(struct args));
            index->i = i,index->j = j;
            pthread_create(&tid[i][j], NULL, &calculation, (void *)index);
            
        }
  }
    for (int i = 0; i < r3; i++) {
        for (int j = 0; j < c3; j++) {
            pthread_join(tid[i][j], NULL);
        }
    }

  display(hasil);

}