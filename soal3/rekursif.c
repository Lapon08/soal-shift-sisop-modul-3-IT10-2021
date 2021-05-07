#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char f_name[1000];

void listdir(const char *name)
{
    DIR *dir;struct dirent *entry;

    if (!(dir = opendir(name)))
        return;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
                continue;
            }
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            listdir(path);
        } else {
            sprintf(f_name+strlen(f_name), "%s/%s\n",name, entry->d_name);

        }
    }
    closedir(dir);
    
}

int main(void) {
    memset(f_name, 0, sizeof(f_name));
    listdir(".");
    printf("%s",f_name);
    int index=0;
    char *token = strtok(f_name, "\n");
    char **filenamepath = malloc(55*sizeof(char *));
    while (token != NULL) {
        printf("%s\n", token);
        filenamepath[index] = token;
        index++;
        token = strtok(NULL, "\n");
    }
    for (int i = 0; i < index; i++)
    {
        printf("%s\n", filenamepath[i]);
    }
        
    return 0;
}