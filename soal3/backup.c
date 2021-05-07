#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <pthread.h>
#include <errno.h>

struct arg_struct {
    int index;
    char arg[100];
};
pthread_mutex_t lock;

void *routeF(void* arg) {
  pthread_mutex_lock(&lock);
  int index = ((struct arg_struct*)arg)->index;
  const char *tes= ((struct arg_struct*)arg)->arg[index];
  printf("start %d\n",index);
  char fromfilenmamepath[300] ,filename[300] ,ekstensi[300];
  char tofilenamepath[300] ,pwd[300] ;
  char temp[300];
  char *temp2[100];
  char *temp3[100];
  char *slash, *dot;
  
  // ((struct args*)arg)->i;
  // Mendapatkan fullpath
  
  strcpy(fromfilenmamepath,tes);
  printf("%s -> ",fromfilenmamepath);
  printf("%d\n",index);
  printf("finisih %d\n",index);
  // if( access( fromfilenmamepath, F_OK ) != -1)
  // {
  //     DIR* dir = opendir(fromfilenmamepath);
  //     if (dir) {
  //       printf("File 2 : Sad, gagal huhu\n");
  //       pthread_exit(0);
  //     }
  //     closedir(dir);
  //     // Mendapatkan nama file
  //     int x = 0;
  //     strcpy(temp, fromfilenmamepath); /*mengopi string ke variabel copy*/
  //     slash = strtok(temp, "/"); /*split string dengan delimiter / */
  //     while(slash != NULL)
  //     {
  //         temp2[x] = slash;
  //         x++;
  //         slash = strtok(NULL, "/");
  //     }
      
  //     strcpy(filename, temp2[x - 1]);
  //     // printf("%s\n",filename);
  //     // Mendapatkan ekstensi
  //     dot = strtok(temp2[x - 1],".");
  //     int y=0;
  //     while(dot != NULL)
  //     {
  //         temp3[y] = dot;
  //         y++;
  //         dot = strtok(NULL, "/");
  //     }
  //     strcpy(ekstensi, temp3[y - 1]);
      
  //     for (int i = 0; i < strlen(ekstensi); i++){
  //           ekstensi[i] = tolower(ekstensi[i]);
  //     }
  //     if (y <=1)
  //     {
  //       strcpy(ekstensi,"Unknown");
  //     }
  //     // printf("%s\n",ekstensi);

  //     // Mendapatkan current direktori
  //     char cwd[300];
  //     getcwd(cwd, sizeof(cwd));
  //     strcpy(tofilenamepath,cwd);
  //     strcat(tofilenamepath,"/");
  //     strcat(tofilenamepath,ekstensi);
  //     strcat(tofilenamepath,"/");
  //     strcat(tofilenamepath,filename);
  //     printf("%s -> ",fromfilenmamepath);
  //     printf("%s\n",tofilenamepath);

  //     // Membuat direktori sesuai ekstensi

  //     DIR *dr2 = opendir(ekstensi);
  //     if (ENOENT == errno) {
  //       mkdir(ekstensi, 0775);
  //       closedir(dr2);
  //     }

  //     rename(fromfilenmamepath, tofilenamepath);
  //     printf("File 3 : Berhasil Dikategorikan\n");
  // }
  //   else
  // {
  //   printf("File 2 : Sad, gagal\n");
  //   pthread_exit(0);
  // }
pthread_mutex_unlock(&lock);
}

int main(int argc, char const *argv[])
{
    
    pthread_t tid[100];  
        if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }
    if(strcmp(argv[1], "-f") == 0)
    {
        for(int i=2; i<argc; i++){
          struct arg_struct *args= (struct arg_struct *)malloc(sizeof(struct arg_struct));
            args->index = i;
            strncpy(args->arg, argv[i],sizeof(argv[i]));
            //printf("%s\n",args.arg[i] );
            pthread_create(&(tid[i-2]), NULL, routeF, (void *)&args);
            //sleep(1);
        }
        for (int i = 2; i < argc; i++) {
          pthread_join(tid[i-2], NULL);
        }
    }else{
      printf("noob");
    }
    pthread_mutex_destroy(&lock);
    
}
