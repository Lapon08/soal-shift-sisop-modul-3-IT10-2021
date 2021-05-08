#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PORT 8080
char filename[20][100];
char publisherbuku[20][100];
char tahunpublishbuku[20][100];
char fullpath[20][100];
char ekstensi[20][100];
char nama[20][100];
char namafull[20][100];
int jumlahData = 0;
char caristring[20][100];
char cookie[100];
int checkLogin(char *kredensial, char *password)
{
    FILE *fptr;

    fptr = fopen("akun.txt", "r+");
    char line[256];
    char user[1024] = {0};
    sprintf(user, "%s:%s", kredensial, password);
    while (fgets(line, sizeof(line) - 1, fptr))
    {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
        if (strcmp(user, line) == 0)
        {
            strcpy(cookie, user);
            return 1;
        }
    }
    fclose(fptr);
    return 0;
}

void runningLog(char *message)
{
    FILE *fp;
    fp = fopen("running.log", "a+");
    strtok(cookie, "\n");
    fprintf(fp, "%s (%s)\n", message, cookie);
    fclose(fp);
}

void registerAkun(char *kredensial, char *password)
{

    FILE *fptr;

    fptr = fopen("akun.txt", "a+");

    fprintf(fptr, "%s:%s\n", kredensial, password);
    fclose(fptr);
}

void createFolderFile()
{
    mkdir("FILES", 0777);
}

void send_file(FILE *fp, int sockfd)
{
    int n;
    char data[1024] = {0};

    while (fgets(data, 1024, fp) != NULL)
    {
        if (send(sockfd, data, sizeof(data), 0) == -1)
        {
            perror("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, 1024);
    }
}

void addtoDatabase(int new_socket, char *publisher, char *tahun_publikasi, char *filenamepath)
{

    char tmp[1024] = {0};
    strcpy(tmp, "FILES/");
    strcat(tmp, filenamepath);
    printf("%s",tmp);
    char file_length[1024] = {0};
    char buffer[1024] = {0};
    char file_content[1024] = {0};
    long fsize;
    // recieve file size
    read(new_socket, file_length, 1024);
    fsize = strtol(file_length, NULL, 0);

    // recieve file content
    for (long i = 0; i < fsize; i += 1024)
    {
        memset(buffer, 0, sizeof(buffer));
        read(new_socket, buffer, 1024);
        strcat(file_content, buffer);
    }

    FILE *fpr;
    fpr = fopen(tmp, "w");
    fprintf(fpr, "%s", file_content);
    fclose(fpr);

    FILE *fp;
    fp = fopen("files.tsv", "a+");
    fprintf(fp, "%s\t%s\t%s\n", publisher, tahun_publikasi, tmp);
    fclose(fp);
    char message[30] = "Tambah: ";
    strcat(message, filenamepath);
    runningLog(message);
}

void readDatabase()
{
    char line[50][256] = {0};
    FILE *fp;
    fp = fopen("files.tsv", "r");
    int i = 0;
    jumlahData = 0;
    printf("masuk\n");
    char *temp = calloc(300, sizeof(char));
    char *temp2 = calloc(300, sizeof(char));

    while (fgets(line[i], sizeof(line), fp))
    {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
        char *token = strtok(line[i], "\t");
        // while (token != NULL)
        // {
        strcpy(publisherbuku[i], token);
        token = strtok(NULL, "\t");
        strcpy(tahunpublishbuku[i], token);
        token = strtok(NULL, "\t");
        strcpy(fullpath[i], token);

        strcpy(temp, fullpath[i]);
        char *token2 = strtok(temp, ".");
        strcpy(temp2, fullpath[i]);
        token2 = strtok(NULL, ".");
        strcpy(ekstensi[i], token2);

        char *token3 = strtok(temp, "/");
        token2 = strtok(NULL, "/");
        strcpy(nama[i], token2);
        strtok(nama[i], "\n");
        strtok(ekstensi[i], "\n");
        // free(temp);
        // free(temp2);
        //}
        strcpy(namafull[i], nama[i]);
        strcat(namafull[i], ".");
        strcat(namafull[i], ekstensi[i]);
        //printf("%d -> %s -> %s -> %s -> %s -> %s -> %s", i, namafull[i], line[i], publisherbuku[i], tahunpublishbuku[i], fullpath[i], ekstensi[i]);
        i++;
        jumlahData++;
    }
    // memset(line, 0, sizeof(line[0][0]) * 50 * 256);
    // memset(publisherbuku, 0, sizeof(publisherbuku[0][0]) * 20 * 100);
    // memset(tahunpublishbuku, 0, sizeof(tahunpublishbuku[0][0]) * 20 * 100);
    // memset(nama, 0, sizeof(nama[0][0]) * 20 * 100);
    // memset(ekstensi, 0, sizeof(ekstensi[0][0]) * 20 * 100);
    // memset(fullpath, 0, sizeof(fullpath[0][0]) * 20 * 100);
    fclose(fp);
}

void seeDatabase(int new_socket)
{
    char see[1024] = {0};
    int i = 0;
    int pos = 0;

    for (i = 0; i < jumlahData; i++)
    {
        pos += sprintf(&see[pos], "Nama: %s\nPublisher: %s\nTahun publishing: %s\nEkstensi File : %s\nFilepath : %s\n", nama[i], publisherbuku[i], tahunpublishbuku[i], ekstensi[i], fullpath[i]);
    }
    //sprintf(strlen(&see[pos]), "Nama: %s\nPublisher: %s\nTahun publishing: %s\nEkstensi File : %sFilepath : %s\n", nama[i], publisherbuku[i], tahunpublishbuku[i], ekstensi[i], fullpath[i]);

    send(new_socket, see, 1024, 0);
    memset(see, 0, 1024);
}
void sendMessage(int new_socket, char *message)
{
    send(new_socket, message, strlen(message), 0);
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
void deleteFile(int new_socket, char *findthisfile)
{
    printf("%s\n", findthisfile);
    char see[1024] = {0};
    int i = 0;
    //int pos = 0;
    int checkfile = 0;
    int position = 0;
    for (int i = 0; i < jumlahData; i++)
    {
        if (strcmp(namafull[i], findthisfile) == 0)
        {
            //pos += sprintf(&see[pos], "Nama: %s\nPublisher: %s\nTahun publishing: %s\nEkstensi File : %sFilepath : %s\n", nama[i], publisherbuku[i], tahunpublishbuku[i], ekstensi[i], fullpath[i]);
            checkfile = 1;
            position = i;
        }
    }
    if (checkfile)
    {
        char tmp[100] = "FILES/old-";
        char tmp2[100] = "FILES/";
        strcat(tmp,findthisfile);
        strcat(tmp2,findthisfile);
        // rename file
        rename(tmp2,tmp);



        FILE *fp;
        fp = fopen("files.tsv", "w");

        // tulis ulang database
        for (int i = 0; i < jumlahData; i++)
        {
            if (i == position)
            {
                continue;
            }
            else
            {
                fprintf(fp, "%s\t%s\t%s", publisherbuku[i], tahunpublishbuku[i], fullpath[i]);
            }
        }
        // pesan delete berhasil
        char pesan[150] = {0};
        sprintf(pesan, "Hapus : %s", namafull[position]);
        runningLog(pesan);
        fclose(fp);
        char *message = "Berhasil\n";
        send(new_socket, message, strlen(message), 0);
        memset(see, 0, 1024);
    }
    else
    {
        char *message = "Gagal\n";
        send(new_socket, message, strlen(message), 0);
    }
}
void findstring(int new_socket, char *findthisstring)
{
    printf("%s\n", findthisstring);
    char *p;
    char see[1024] = {0};
    int i = 0;
    int pos = 0;
    for (int i = 0; i < jumlahData; i++)
    {
        p = strstr(namafull[i], findthisstring);
        if (p)
        {
            pos += sprintf(&see[pos], "Nama: %s\nPublisher: %s\nTahun publishing: %s\nEkstensi File : %s\nFilepath : %s\n", nama[i], publisherbuku[i], tahunpublishbuku[i], ekstensi[i], fullpath[i]);
        }
    }

    send(new_socket, see, 1024, 0);
    memset(see, 0, 1024);
}
int checkDownloadfile(int new_socket, char *findthisfile)
{
    char *p;
    char see[1024] = {0};
    int i = 0;
    int pos = 0;
    int checkfile = 0;
    int position = 0;
    for (int i = 0; i < jumlahData; i++)
    {
        if (strcmp(namafull[i], findthisfile) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char buff[1024] = {0};
    char kredensial[1024] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    createFolderFile();
    while (1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        // login form
        int otentikasi = 0;
        while (otentikasi == 0)
        {
            sendMessage(new_socket, "Server : Masukkan Mode: Login -> l , Register -> r , Quit -> q");
            char mode[1024] = {0};
            valread = read(new_socket, mode, 1024);
            // login

            if (strcmp(mode, "l\n") == 0)
            {
                memset(mode, 0, 1024);
                // Id
                char *message = "[Login]Id: ";
                send(new_socket, message, strlen(message), 0);
                char kredensial[1024] = {0};
                read(new_socket, kredensial, 1024);
                //printf("%s", kredensial);
                kredensial[strcspn(kredensial, "\n")] = 0;
                // Password
                message = "[Login]Password: ";
                send(new_socket, message, strlen(message), 0);
                char password[1024] = {0};
                read(new_socket, password, 1024);

                otentikasi = checkLogin(kredensial, password);
                memset(kredensial, 0, 1024);
                char isotentikasi[2];
                sprintf(isotentikasi, "%d", otentikasi);
                // kirim otentikasi
                send(new_socket, isotentikasi, strlen(isotentikasi), 0);
                memset(isotentikasi, 0, 2);
                memset(kredensial, 0, 1024);
                memset(password, 0, 1024);
                memset(buffer, 0, 1024);
            }
            else if (strcmp(mode, "r\n") == 0)
            {
                memset(mode, 0, 1024);
                // Id
                char *message = "[Register]Id: ";
                send(new_socket, message, strlen(message), 0);
                char kredensial[1024] = {0};
                read(new_socket, kredensial, 1024);
                //printf("%s", kredensial);
                kredensial[strcspn(kredensial, "\n")] = 0;
                // Password
                message = "[Register]Password: ";
                send(new_socket, message, strlen(message), 0);
                char password[1024] = {0};
                read(new_socket, password, 1024);
                password[strcspn(password, "\n")] = 0;
                registerAkun(kredensial, password);

                memset(kredensial, 0, 1024);
                memset(password, 0, 1024);
                //sendMessage(new_socket, "Server :[REGISTER] Register Berhasil Dilakukan");
            }
            else if (strcmp(mode, "q\n") == 0)
            {
                break;
            }
            memset(buffer, 0, 1024);
            memset(mode, 0, 1024);
        }
        if (otentikasi)
        {
            int inputcommand = 1;
            while (inputcommand)
            {
                char *lohe = " Server:[APP] command : add, download, delete, find, see, quit\n";
                send(new_socket, lohe, strlen(lohe), 0);
                char buffer2[1024] = {0};
                read(new_socket, buffer2, 1024);
                if (strcmp(buffer2, "add\n") == 0)
                {
                    memset(buffer2, 0, 1024);
                    // publisher
                    char *message = "Publisher: ";
                    send(new_socket, message, strlen(message), 0);
                    char publisher[1024] = {0};
                    read(new_socket, publisher, 1024);
                    //printf("%s", publisher);
                    publisher[strcspn(publisher, "\n")] = 0;
                    // tahun publikasi
                    message = "Tahun Publikasi: ";
                    send(new_socket, message, strlen(message), 0);
                    char tahun_publikasi[1024] = {0};
                    read(new_socket, tahun_publikasi, 1024);
                    tahun_publikasi[strcspn(tahun_publikasi, "\n")] = 0;
                    // tahun publikasi
                    message = "Filepath: ";
                    send(new_socket, message, strlen(message), 0);
                    char temp[1024] = {0};
                    char filenamepath[1024] = {0};
                    char *temp2[100];
                    char *slash;
                    int x;
                    read(new_socket, temp, 1024);
                    slash = strtok(temp, "/");
                    while (slash != NULL)
                    {
                        temp2[x] = slash;
                        x++;
                        slash = strtok(NULL, "/");
                    }
                    strcpy(filenamepath, temp2[x - 1]);
                    memset(temp, 0, 1024);
                    // int n;
                    // FILE *fp;
                    // char bismillah[1024] = {0};

                    // fp = fopen(filenamepath, "w+");
                    // fflush(fp);
                    // while (1)
                    // {

                    //     n = recv(new_socket, bismillah, 1024, 0);
                    //     if (n <= 0)
                    //     {
                    //         break;
                    //     }
                    //     fprintf(fp, "%s", bismillah);
                    //     printf("%s", bismillah);
                    // }
                    // bzero(buffer, 1024);

                    // fclose(fp);

                    //strtok(filenamepath,"\n");
                    //printf("%s -> %s -> %s", publisher, tahun_publikasi, filenamepath);
                    //write_file(new_socket, filenamepath);
                    // memset(publisher, 0, 1024);
                    // memset(tahun_publikasi, 0, 1024);
                    // memset(filenamepath, 0, 1024);

                    addtoDatabase(new_socket, publisher, tahun_publikasi, filenamepath);
                }
                else if (strcmp(buffer2, "delete\n") == 0)
                {
                    memset(buffer2, 0, 1024);
                    readDatabase();
                    sendMessage(new_socket, "[APP] File to Delete: ");
                    char findthisfile[100] = {0};
                    // testing
                    read(new_socket, findthisfile, 100);
                    deleteFile(new_socket, findthisfile);
                    memset(findthisfile, 0, 100);
                }
                else if (strcmp(buffer2, "see\n") == 0)
                {
                    memset(buffer2, 0, 1024);
                    readDatabase();
                    seeDatabase(new_socket);
                }
                else if (strcmp(buffer2, "find\n") == 0)
                {
                    memset(buffer2, 0, 1024);
                    readDatabase();
                    sendMessage(new_socket, "[APP] String: ");
                    char findthisstring[100] = {0};
                    // testing
                    read(new_socket, findthisstring, 100);

                    findstring(new_socket, findthisstring);
                    memset(findthisstring, 0, 100);
                }
                else if (strcmp(buffer2, "download\n") == 0)
                {
                    int filecheck = 0;
                    memset(buffer2, 0, 1024);
                    readDatabase();
                    sendMessage(new_socket, "[APP] File To Download: ");
                    char findthisstring[100] = {0};
                    // testing
                    read(new_socket, findthisstring, 100);
                    strtok(findthisstring, "\n");
                    printf("%s\n", findthisstring);
                    filecheck = checkDownloadfile(new_socket, findthisstring);

                    char isfileready[2];
                    sprintf(isfileready, "%d", filecheck);
                    // kirim otentikasi
                    send(new_socket, isfileready, strlen(isfileready), 0);
                    memset(isfileready, 0, 2);
                    if (filecheck)
                    {
                        printf("masuk yuk");
                        char file_content[1024] = {0};
                        char buffer[1024] = {0};
                        char file_length[1024] = {0};
                        FILE *fp;
                        char pathfile[100] = "FILES/";
                        strcat(pathfile, findthisstring);

                        fp = fopen(pathfile, "r");
                        if (fp == NULL)
                        {
                            perror("[-]Error in reading file.");
                            exit(1);
                        }

                        fseek(fp, 0, SEEK_END);
                        long fsize = ftell(fp);
                        rewind(fp);
                        fread(file_content, 1, fsize, fp);
                        fclose(fp);

                        // send file size
                        sprintf(file_length, "%ld", fsize);
                        send(new_socket, file_length, sizeof(file_length), 0);
                        sleep(1);

                        // send file content

                        for (long i = 0; i < fsize; i += 1024)
                        {
                            memset(buffer, 0, sizeof(buffer));
                            sprintf(buffer, "%.*s", fsize < 1024 ? fsize : abs(fsize - i) < 1024 ? abs(fsize - 1)
                                                                                                 : 1024,
                                    file_content + i);
                            send(new_socket, buffer, sizeof(buffer), 0);
                        }

                        printf("[+]File data sent successfully.\n");
                    }
                    else
                    {
                        char tidak[200] = " Gagal \n";
                        send(new_socket, tidak, 200, 0);
                    }

                    memset(findthisstring, 0, 100);
                }
                else if (strcmp(buffer2, "quit\n") == 0)
                {
                    break;
                }
            }
        }
    }
}
