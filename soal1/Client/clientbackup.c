#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080

void send_file(FILE *fp, int sockfd)
{
    int n;
    char data[1024] = {0};
    fgets(data, 1024, fp);
    //while (fgets(data, 1024, fp) != NULL)
    //{
    if (send(sockfd, data, sizeof(data), 0) == -1)
    {
        perror("[-]Error in sending file.");
        exit(1);
    }
    bzero(data, 1024);
    //}
}
void write_file(int sockfd){
  int n;
  FILE *fp;
  char *filename = "recv.txt";
  char buffer[1024];

  fp = fopen(filename, "w+");
  while (1) {
    n = read(sockfd, buffer, 1024);
    if (n <= 0){
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, 1024);
  }
  return;
}
void readMessage(int new_socket)
{
    char readmessage[1024] = {0};
    memset(readmessage, 0, 1024);
    // tahun publikasi
    read(new_socket, readmessage, 1024);
    printf("%s", readmessage);
    memset(readmessage, 0, 1024);
}

int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char buff[1024] = {0};

    int otentikasi = 0;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    // login form
    while (otentikasi == 0)
    {
        // selamat datang
        read(sock, buffer, 1024);
        printf("%s\n", buffer);
        memset(buffer, 0, 1024);
        // input mode
        fgets(buff, 1024, stdin);
        send(sock, buff, strlen(buff), 0);

        if (strcmp(buff, "l\n") == 0)
        {
            // bersihkan buffer
            memset(buffer, 0, 1024);
            memset(buff, 0, 1024);
            // pesan dari server
            read(sock, buffer, 1024);
            printf("%s\n", buffer);
            memset(buffer, 0, 1024);
            // Masukkan kredensial
            fgets(buff, 1024, stdin);
            // kirim kredensial
            send(sock, buff, strlen(buff), 0);
            memset(buff, 0, 1024);
            // menerima otentikasi
            char isotentikasi[2] = {0};
            read(sock, isotentikasi, 2);
            otentikasi = atoi(isotentikasi);
            memset(isotentikasi, 0, 2);
        }
        else if (strcmp(buff, "r\n") == 0)
        {
            // bersihkan buffer
            memset(buffer, 0, 1024);
            memset(buff, 0, 1024);
            // pesan dari server
            read(sock, buffer, 1024);
            printf("%s\n", buffer);
            memset(buffer, 0, 1024);
            // Masukkan kredensial
            fgets(buff, 1024, stdin);
            // kirim kredensial
            send(sock, buff, strlen(buff), 0);
            memset(buff, 0, 1024);
            read(sock, buffer, 1024);
            printf("%s\n", buffer);
            memset(buffer, 0, 1024);
        }
        else if (strcmp(buff, "q\n") == 0)
        {
            printf("Good Bye\n");
            return 0;
        }
        if (otentikasi)
        {
            int inputcommand = 1;
            while (inputcommand)
            {
                char pesan[300] = {0};
                char buffer2[1024] = {0};
                memset(buffer2, 0, 1024);
                read(sock, pesan, 300);
                printf("%s", pesan);
                memset(buffer2, 0, 1024);
                fgets(buffer2, 1024, stdin);
                send(sock, buffer2, strlen(buffer2), 0);
                if (strcmp(buffer2, "add\n") == 0)
                {
                    FILE *fp;
                    memset(buffer2, 0, 1024);
                    // publisher
                    read(sock, buffer2, 1024);
                    printf("%s", buffer2);
                    memset(buffer2, 0, 1024);
                    fgets(buffer2, 1024, stdin);
                    send(sock, buffer2, strlen(buffer2), 0);
                    memset(buffer2, 0, 1024);
                    // tahun publikasi
                    read(sock, buffer2, 1024);
                    printf("%s", buffer2);
                    memset(buffer2, 0, 1024);
                    fgets(buffer2, 1024, stdin);
                    send(sock, buffer2, strlen(buffer2), 0);
                    memset(buffer2, 0, 1024);
                    // filepath
                    read(sock, buffer2, 1024);
                    printf("%s", buffer2);
                    memset(buffer2, 0, 1024);
                    char filenamepath[1024] = {0};

                    fgets(buffer2, 1024, stdin);
                    strcpy(filenamepath, buffer2);
                    memset(buffer2, 0, 1024);
                    strtok(filenamepath, "\n");
                    send(sock, filenamepath, strlen(filenamepath), 0);

                    //buffer2[strcspn(buffer2, "\n")] = 0;
                    // printf("%stes",filenamepath);

                    //printf("the file was sent successfully");
                    // fp = fopen(filenamepath, "r");
                    // if (fp == NULL)
                    // {
                    //     perror("[-]Error in reading file.");
                    //     return 0;
                    // }
                    // int n;
                    // char data[1024] = {0};

                    // while (fgets(data, 1024, fp) != NULL)
                    // {
                    //     if (send(sock, data, sizeof(data), 0) == -1)
                    //     {
                    //         perror("[-]Error in sending file.");
                    //         exit(1);
                    //     }
                    // }
                    // bzero(data, 1024);

                    //send_file(fp, sock);
                    //printf("[+]File data sent successfully.\n");
                }
                else if (strcmp(buffer2, "delete\n") == 0)
                {
                    readMessage(sock);
                    // input string
                    char inputString[100] = {0};
                    fgets(inputString, 100, stdin);
                    inputString[strcspn(inputString, "\n")] = 0;
                    send(sock, inputString, strlen(inputString), 0);
                    memset(inputString, 0, 100);
                    char see[1024] = {0};
                    memset(see, 0, 1024);
                    // Read Database
                    read(sock, see, 1024);
                    printf("%s", see);
                    memset(see, 0, 1024);
                }
                else if (strcmp(buffer2, "see\n") == 0)
                {
                    memset(buffer2, 0, 1024);
                    char see[1024] = {0};
                    memset(see, 0, 1024);
                    // Read Database
                    read(sock, see, 1024);
                    printf("%s", see);
                    memset(see, 0, 1024);
                    //memset(buffer2, 0, 1024);
                }
                else if (strcmp(buffer2, "find\n") == 0)
                {
                    readMessage(sock);
                    // input string
                    char inputString[100] = {0};
                    fgets(inputString, 100, stdin);
                    inputString[strcspn(inputString, "\n")] = 0;
                    send(sock, inputString, strlen(inputString), 0);
                    memset(inputString, 0, 100);
                    char see[1024] = {0};
                    memset(see, 0, 1024);
                    // Read Database
                    read(sock, see, 1024);
                    printf("%s", see);
                    memset(see, 0, 1024);
                }
                else if (strcmp(buffer2, "download\n") == 0)
                {
                    readMessage(sock);
                    // input string
                    char inputString[100] = {0};
                    fgets(inputString, 100, stdin);
                    inputString[strcspn(inputString, "\n")] = 0;
                    send(sock, inputString, strlen(inputString), 0);
                    //memset(inputString, 0, 100);
                    char file_length[1024] = {0};
                    char buffer[1024] = {0};
                    char file_content[1024] = {0};

                    long fsize;
                    // recieve file size
                    read(sock,file_length,01024);
                    fsize = strtol(file_length,NULL,0);

                    // recieve file content
                    for (long i = 0; i < __FD_SETSIZE; i+=1024)
                    {
                        memset(buffer,0,sizeof(buffer));
                        read(sock,buffer,1024);
                        strcat(file_content,buffer);
                    }

                    FILE *fp;
                    fp = fopen(inputString,"w");
                    fprintf(fp, "%s",file_content);
                    fclose(fp);
                    

                    write_file(sock);
                }
                else if (strcmp(buffer2, "quit\n") == 0)
                {
                    printf("Good Bye\n");
                    return 0;
                }
            }
        }
    }

    printf("Good Bye\n");
    // valread = read( sock , buffer, 1024);

    return 0;
}