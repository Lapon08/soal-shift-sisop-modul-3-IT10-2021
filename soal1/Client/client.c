#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 4444

void readMessage(int new_socket)
{
    char readmessage[1024] = {0};
    memset(readmessage, 0, 1024);
    read(new_socket, readmessage, 1024);
    printf("%s", readmessage);
    memset(readmessage, 0, 1024);
}

void sendFile(int new_socket, int fsize, char *file_content)
{
    char buffer[1024] = {0};
    for (int i = 0; i < fsize; i += 1024)
    {

        sprintf(buffer, "%.*s", fsize < 1024 ? fsize : abs(fsize - i) < 1024 ? abs(fsize - 1)
                                                                             : 1024,
                file_content + i);
        send(new_socket, buffer, sizeof(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
    }
    memset(buffer, 0, sizeof(buffer));
    memset(file_content, 0, sizeof(file_content));

    printf("[+]File data sent successfully.\n");
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
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        return -1;
    }
    // login form
    while (otentikasi == 0)
    {
        char pesan[1024] = {0};
        read(sock, pesan, 1024);
        printf("%s\n", pesan);
        memset(pesan, 0, 1024);
        // input mode
        char mode[1024] = {0};
        fgets(mode, 1024, stdin);
        send(sock, mode, strlen(mode), 0);

        if (strcmp(mode, "l\n") == 0)
        {
            // bersihkan buffer
            // pesan dari server
            char pesan[1024] = {0};
            read(sock, pesan, 1024);
            printf("%s\n", pesan);
            memset(pesan, 0, 1024);
            // Masukkan kredensial
            char username[1024] = {0};
            fgets(username, 1024, stdin);
            //strtok(username,"\n");

            // kirim kredensial
            send(sock, username, strlen(username), 0);
            memset(username, 0, 1024);
            read(sock, pesan, 1024);
            printf("%s\n", pesan);
            // Masukkan kredensial
            char password[1024] = {0};
            fgets(password, 1024, stdin);
            //strtok(password,"\n");

            // kirim kredensial
            send(sock, password, strlen(password), 0);
            memset(password, 0, 1024);
            // menerima otentikasi
            char isotentikasi[2] = {0};
            read(sock, isotentikasi, 2);
            otentikasi = atoi(isotentikasi);
            memset(isotentikasi, 0, 2);
        }
        else if (strcmp(mode, "r\n") == 0)
        {
            // pesan dari server
            char pesan[1024] = {0};
            read(sock, pesan, 1024);
            printf("%s\n", pesan);
            memset(pesan, 0, 1024);
            // Masukkan kredensial
            char username[1024] = {0};
            fgets(username, 1024, stdin);
            // kirim kredensial
            send(sock, username, strlen(username), 0);
            memset(username, 0, 1024);

            // pesan dari server
            char pesan2[1024] = {0};
            read(sock, pesan2, 1024);
            printf("%s\n", pesan2);
            memset(pesan2, 0, 1024);
            // Masukkan kredensial
            char password[1024] = {0};
            fgets(password, 1024, stdin);
            // kirim kredensial
            send(sock, password, strlen(password), 0);
            memset(password, 0, 1024);
        }
        else if (strcmp(mode, "q\n") == 0)
        {

            printf("Good Bye\n");
            return 0;
        }
        memset(buffer, 0, 1024);
        memset(buff, 0, 1024);
        memset(buffer, 0, 1024);
        memset(buff, 0, 1024);
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
            memset(pesan, 0, 300);
            fgets(buffer2, 1024, stdin);
            send(sock, buffer2, strlen(buffer2), 0);
            if (strcmp(buffer2, "add\n") == 0)
            {

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

                FILE *fp;
                fp = fopen(filenamepath, "r");
                char file_length[1024] = {0};
                char file_content[1024] = {0};
                if (fp == NULL)
                {
                    //send confirmation

                    char *check = "0";
                    send(sock, check, strlen(check), 0);
                    printf("[-]Error in reading file.\n");
                }
                else
                {
                    fseek(fp, 0, SEEK_END);
                    int fsize = ftell(fp);
                    rewind(fp);
                    fread(file_content, 1, fsize, fp);
                    fclose(fp);

                    //send confirmation
                    char *check = "1";
                    send(sock, check, strlen(check), 0);

                    // send file size
                    sprintf(file_length, "%d", fsize);
                    send(sock, file_length, sizeof(file_length), 0);
                    sleep(1);

                    // send file content

                    sendFile(sock, fsize, file_content);
                }
            }
            else if (strcmp(buffer2, "delete\n") == 0)
            {
                memset(buffer2, 0, 1024);
                readMessage(sock);
                // input string
                char iniinput[100] = {0};
                fgets(iniinput, 100, stdin);
                iniinput[strcspn(iniinput, "\n")] = 0;
                send(sock, iniinput, strlen(iniinput), 0);
                memset(iniinput, 0, 100);
                char delet[10] = {0};
                memset(delet, 0, 10);

                read(sock, delet, 10);
                printf("%s", delet);
                memset(delet, 0, 10);
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
                memset(buffer2, 0, 1024);
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
                char file_length[1024] = {0}, buffer[1024] = {0}, file_content[1024] = {0}, isfileready[2];
                read(sock, isfileready, 2);
                int fileready = atoi(isfileready);
                memset(isfileready, 0, 2);
                if (fileready)
                {
                    long fsize;
                    // recieve file size
                    read(sock, file_length, 1024);
                    fsize = strtol(file_length, NULL, 0);
                    memset(file_length, 0, 1024);
                    // recieve file content
                    long i = 0;
                    while (i < fsize)
                    {
                        memset(buffer, 0, sizeof(buffer));
                        read(sock, buffer, 1024);
                        strcat(file_content, buffer);
                        i += 1024;
                    }

                    memset(buffer, 0, sizeof(buffer));

                    FILE *fp;

                    fp = fopen(inputString, "w");
                    if (fp == NULL)
                    {
                        perror("[-]Error in reading file.");
                        exit(1);
                    }
                    fprintf(fp, "%s", file_content);
                    memset(file_content, 0, sizeof(file_content));

                    fclose(fp);
                }
                else
                {
                    char message[200] = {0};
                    read(sock, message, 200);
                    printf("%s", message);
                    memset(message, 0, 200);
                }
            }
            else if (strcmp(buffer2, "quit\n") == 0)
            {
                printf("Good Bye\n");
                return 0;
            }
            memset(buffer2, 0, 1024);
        }
    }

    printf("Good Bye\n");

    return 0;
}