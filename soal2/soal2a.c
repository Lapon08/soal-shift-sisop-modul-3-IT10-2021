
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

int r1 = 4, c1 =3;
int r2 = 3, c2 = 6;
int r3 =4,c3 =6;


// function to display the matrix
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

int main() {
   int first[r1][c1];
   int second[r2][c2];
   int result[r3][c3];

    // matrix 1

   for (int i = 0; i < r1; ++i) {
      for (int j = 0; j < c1; ++j) {
         printf("Enter a%d%d: ", i + 1, j + 1);
         scanf("%d", &first[i][j]);
      }
      printf("\n");
   }

    // matrix 2

   for (int i = 0; i < r2; ++i) {
      for (int j = 0; j < c2; ++j) {
         printf("Enter a%d%d: ", i + 1, j + 1);
         scanf("%d", &second[i][j]);
      }
      printf("\n");
   }

   // initiate
   for (int i = 0; i < r1; ++i) {
      for (int j = 0; j < c2; ++j) {
         result[i][j] = 0;
      }
   }

   // Multiplying 
   for (int i = 0; i < r1; ++i) {
      for (int j = 0; j < c2; ++j) {
         for (int k = 0; k < c1; ++k) {
            result[i][j] += first[i][k] * second[k][j];
         }
      }
   }

   display(result);
    key_t key = 1337;
    int *value;

    int shmid = shmget(key, sizeof(result), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    int* p = (int *)value;

    memcpy(p, result, 96);

    shmdt(value);

}