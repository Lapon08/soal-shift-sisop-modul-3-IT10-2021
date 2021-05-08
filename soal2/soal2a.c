
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

unsigned long long int r1 = 4, c1 = 3;
unsigned long long int r2 = 3, c2 = 6;
unsigned long long int r3 = 4, c3 = 6;

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

unsigned long long int main()
{
   unsigned long long int first[r1][c1], second[r2][c2], result[r3][c3];

   // matrix 1
   char prompt[100], prompt2[100];
   char prompt3[100], prompt4[100];

   sprintf(prompt, "Matrix ke A\n");
   printf("%s", prompt);
   for (unsigned long long int i = 0; i < r1; ++i)
   {
      for (unsigned long long int j = 0; j < c1; ++j)
      {
         sprintf(prompt2, "Masukkan A %llu %llu ", i + 1, j + 1);
         printf("%s", prompt2);
         scanf("%llu", &first[i][j]);
      }
      printf("\n");
   }

   // matrix 2
   sprintf(prompt, "Matrix ke B\n");
   printf("%s", prompt);
   for (unsigned long long int i = 0; i < r2; ++i)
   {
      for (unsigned long long int j = 0; j < c2; ++j)
      {
         sprintf(prompt4, "Masukkan B %llu %llu ", i + 1, j + 1);
         printf("%s", prompt4);
         scanf("%llu", &second[i][j]);
      }
      printf("\n");
   }

   // initiate
   unsigned long long int i = 0, j = 0;

   while (i < r3)
   {
      j = 0;
      while (j < c3)
      {
         result[i][j] = 0;
         j++;
      }
      i++;
   }

   // Multiplying
   unsigned long long int sum = 0;
   char prompt5[100];unsigned long long int k =0;
   i = 0, j = 0;
   sprintf(prompt5, "\nMenghitung...\n");
   printf("%s", prompt5);
   while (i < r1)
   {
      j = 0;
      while (j < c2)
      {
         k = 0;
         while (k < c1)
         {
            sum += first[i][k] * second[k][j];
            k++;
         }
         result[i][j] = sum;
         sum = 0;
         j++;
      }
      i++;
   }

   char *message = "Hasil";
   display(result, message);
   key_t key = 1337;
   unsigned long long int *value;

   unsigned long long int shmid = shmget(key, sizeof(result), IPC_CREAT | 0666);
   value = shmat(shmid, NULL, 0);

   unsigned long long int *p = (unsigned long long int *)value;

   memcpy(p, result, 192);

   shmdt(value);
}