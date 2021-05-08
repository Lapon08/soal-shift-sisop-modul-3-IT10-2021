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

char mode[100];
int coba = 0;
const char *pilihanMode;
int check = 1;
void *routeF(void *arg)
{
  coba++;
  char fromfilenmamepath[500], filename[500], ekstensi[500];
  char tofilenamepath[500], pwd[500];
  char temp[500];
  char *temp2[500];
  char *temp3[500];
  char *slash, *dot;

  // Mendapatkan fullpath
  strcpy(fromfilenmamepath, (char *)arg);

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
  if (access(fromfilenmamepath, F_OK) != -1)
  {
    DIR *dir = opendir(fromfilenmamepath);
    if (dir)
    {
      closedir(dir);
      if (strcmp(pilihanMode, "-f") == 0)
      {
        char message[1024];
        sprintf(message, "%s : Sad, gagal\n", filename);
        printf("%s", message);
      }
      check = 0;
    }
    else
    {
      closedir(dir);
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

      // Mendapatkan current direktori
      char cwd[500];
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
      char message[1000];
      if (strcmp(pilihanMode, "-f") == 0)
      {
        sprintf(message, "%s : Berhasil Dikategorikan\n", filename);
        printf("%s", message);
      }
    }

  }
  else
  {
    if (strcmp(pilihanMode, "-f") == 0)
    {
      char message[600];
      sprintf(message, "%s : Sad, gagal\n", filename);
      printf("%s", message);
    }
    check = 0;
  }
}

char f_name[2048];
void listdir(const char *name)
{
  DIR *dir;struct dirent *entry;

  if (!(dir = opendir(name)))
  {
    check = 0;
    return;
  }
  else
  {
    while ((entry = readdir(dir)) != NULL)
    {
      if (entry->d_type == DT_DIR)
      {
        char path[2048];
        if (strcmp(entry->d_name, ".") == 0)
        {
          continue;
        }
        if (strcmp(entry->d_name, "..") == 0)
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
}

int main(int argc, char const *argv[])
{

  pthread_t tid[100];
  //  Jika mode -f
  if (strcmp(argv[1], "-f") == 0)
  {
    pilihanMode = "-f";
    int i = 2;
    while (i < argc)
    {
      pthread_create(&(tid[i - 2]), NULL, routeF, (void *)argv[i]);
      i++;
    }
    i = 2;
    while (i < argc)
    {
      pthread_join(tid[i - 2], NULL);
      i++;
    }
  }
  else if (strcmp(argv[1], "-d") == 0)
  {
    memset(f_name, 0, sizeof(f_name));
    listdir(argv[2]);
    pilihanMode = "-d";
    int index2 = 0;
    char *token = strtok(f_name, "\n");
    char **filenamepath = malloc(200 * sizeof(char *));
    while (token != NULL)
    {
      filenamepath[index2] = token;
      index2++;
      token = strtok(NULL, "\n");
    }

    int i = 0;
    while (i < index2)
    {
      pthread_create(&(tid[i]), NULL, routeF, (void *)filenamepath[i]);
      i++;
    }
    i = 0;
    while (i < index2)
    {
      pthread_join(tid[i], NULL);
      i++;
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
    char **filenamepath = malloc(200 * sizeof(char *));
    while (token != NULL)
    {
      filenamepath[index3] = token;
      index3++;
      token = strtok(NULL, "\n");
    }

    for (int i = 0; i < index3; i++)
    {
      pthread_create(&(tid[i]), NULL, routeF, (void *)filenamepath[i]);
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
