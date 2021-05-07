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
#include <time.h>


char mode[100];
int coba = 0;
const char *pilihanMode;
int check = 1;
void *routeF(void *arg)
{
  coba++;
  char fromfilenmamepath[300], filename[300], ekstensi[300];
  char tofilenamepath[300], pwd[300];
  char temp[300];
  char *temp2[100];
  char *temp3[100];
  char *slash, *dot;

  // Mendapatkan fullpath
  strcpy(fromfilenmamepath, (char *)arg);

  if (access(fromfilenmamepath, F_OK) != -1)
  {
    DIR *dir = opendir(fromfilenmamepath);
    if (dir)
    {
      if (strcmp(pilihanMode, "-f") == 0)
      {
        char message[100];
        sprintf(message, "File %d : Sad, gagal\n", coba);
        printf("%s", message);
      }
      check = 0;
    }
    closedir(dir);
    // Mendapatkan nama file
    int x = 0;
    strcpy(temp, fromfilenmamepath);
    slash = strtok(temp, "/");
    while (slash != NULL)
    {
      temp2[x] = slash;
      x++;
      slash = strtok(NULL, "/");
    }

    strcpy(filename, temp2[x - 1]);
    // printf("%s\n",filename);
    // Mendapatkan ekstensi
    if (strchr(filename, '.') != NULL)
    {

      dot = strtok(temp2[x - 1], ".");
      int y = 0;
      while (dot != NULL)
      {
        temp3[y] = dot;
        y++;
        dot = strtok(NULL, "/");
      }
      strcpy(ekstensi, temp3[y - 1]);

      for (int i = 0; i < strlen(ekstensi); i++)
      {
        ekstensi[i] = tolower(ekstensi[i]);
      }
      if (y <= 1)
      {
        strcpy(ekstensi, "Hidden");
      }
    }
    else
    {
      strcpy(ekstensi, "Unknown");
    }
    // printf("%s\n",ekstensi);

    // Mendapatkan current direktori
    char cwd[300];
    getcwd(cwd, sizeof(cwd));
    strcpy(tofilenamepath, cwd);
    strcat(tofilenamepath, "/");
    strcat(tofilenamepath, ekstensi);
    strcat(tofilenamepath, "/");
    strcat(tofilenamepath, filename);
    // ngecek guis
    // printf("%s -> ", fromfilenmamepath);
    // printf("%s\n", tofilenamepath);

    // Membuat direktori sesuai ekstensi

    DIR *dr2 = opendir(ekstensi);
    if (ENOENT == errno)
    {
      mkdir(ekstensi, 0775);
      closedir(dr2);
    }

    rename(fromfilenmamepath, tofilenamepath);
    // printf("%d\n", coba);
    char message[1000];
    if (strcmp(pilihanMode, "-f") == 0)
    {
      sprintf(message, "%s : Berhasil Dikategorikan\n", filename);
      printf("%s", message);
    }
  }
  else
  {
    if (strcmp(pilihanMode, "-f") == 0)
    {
      char message[100];
      sprintf(message, "File %d : Sad, gagal\n", coba);
      printf("%s", message);
    }
    check = 0;
  }
  
}

char f_name[1000];
void listdir(const char *name)
{
  DIR *dir;
  struct dirent *entry;

  if (!(dir = opendir(name)))
    return;

  while ((entry = readdir(dir)) != NULL)
  {
    if (entry->d_type == DT_DIR)
    {
      char path[1024];
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      {
        continue;
      }
      snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
      listdir(path);
    }
    else
    {
      sprintf(f_name + strlen(f_name), "%s/%s\n", name, entry->d_name);
    }
  }
  closedir(dir);
}

int main(int argc, char const *argv[])
{

  pthread_t tid[100];

  if (strcmp(argv[1], "-f") == 0)
  {
    pilihanMode = "-f";
    for (int i = 2; i < argc; i++)
    {
      pthread_create(&(tid[i - 2]), NULL, routeF, (void *)argv[i]);
      // pthread_join(tid[i - 2], NULL);
      //sleep(1);
    }
    for (int i = 2; i < argc; i++)
    {
    pthread_join(tid[i - 2], NULL);
    }
  }
  else if (strcmp(argv[1], "-d") == 0)
  {

    memset(f_name, 0, sizeof(f_name));
    listdir(argv[2]);
    pilihanMode = "-d";
    int index2 = 0;
    char *token = strtok(f_name, "\n");
    char **filenamepath = malloc(55 * sizeof(char *));
    while (token != NULL)
    {
      filenamepath[index2] = token;
      index2++;
      token = strtok(NULL, "\n");
    }
    // printf("Total File %d\n", index2);
    // for (int i = 0; i < index2; i++)
    // {
    //   printf("%s\n", filenamepath[i]);
    // }
    for (int i = 0; i < index2; i++)
    {
      pthread_create(&(tid[i]), NULL, routeF, (void *)filenamepath[i]);
      //sleep(1);
    }
    for (int i = 0; i < index2; i++)
    {
      pthread_join(tid[i], NULL);
    }
    if (check == 1)
    {
      printf("Direktori sukses disimpan!");
    }
    else if (check == 0)
    {
      printf("Yah, gagal disimpan :(");
    }
  }
  else if (strcmp(argv[1], "*") == 0)
  {
    memset(f_name, 0, sizeof(f_name));
    char currentDirectory[1337];
    getcwd(currentDirectory, sizeof(currentDirectory));
    listdir(currentDirectory);
    pilihanMode = "*";
    int index3 = 0;
    char *token = strtok(f_name, "\n");
    char **filenamepath = malloc(55 * sizeof(char *));
    while (token != NULL)
    {
      filenamepath[index3] = token;
      index3++;
      token = strtok(NULL, "\n");
    }
    printf("Total File %d\n", index3);
    // for (int i = 0; i < index3; i++)
    // {
    //   printf("%s\n", filenamepath[i]);
    // }
    for (int i = 0; i < index3; i++)
    {
      pthread_create(&(tid[i]), NULL, routeF, (void *)filenamepath[i]);
      //sleep(1);
    }
    for (int i = 0; i < index3; i++)
    {
      pthread_join(tid[i], NULL);
    }
    if (check == 1)
    {
      printf("sukses disimpan!");
    }
    else if (check == 0)
    {
      printf("Yah, ada yang gagal disimpan :(");
    }
  }


}
