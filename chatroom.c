#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <termios.h> // termios, TCSANOW, ECHO, ICANON
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

int find_all_users(char *room_users[], char *path, char *main_user);

int main(int argc, char *argv[]){
    char *roomname = argv[1];
    char *main_user = argv[2];
    char *room_users[1024];
    
    char room_path[1024] = "/tmp/chatroom-";
    strcat(room_path, roomname);
    
    //If room does not exist create it.
    struct stat st = {0};
    if (stat(room_path, &st) == -1) mkdir(room_path, 0777);

    //Create user FIFO.
    char user_path[1024];
    strcpy(user_path, room_path);
    strcat(user_path, "/");
    strcat(user_path, main_user);
    mkfifo(user_path, 0777);
    printf("Welcome to %s!\n", roomname);

    //Child process is used to print into every other pipe in the room.
    //Travels through each pipe in the directory. Creates another child process to write and waits until execution completes.
    int pid = fork();
    if (pid == 0) {
        while (true) {
            char raw_message[1028];
            fgets(raw_message, 1028, stdin);
            char message[1028];
            strcpy(message, "[");
            strcat(message, roomname);
            strcat(message, "] ");
            strcat(message, main_user);
            strcat(message, ": ");
            strcat(message, raw_message);
            
            // Uncomment if you want to echo your message.
            // printf("[%s] %s: %s", roomname, main_user, raw_message);
            
            int user_count = find_all_users(room_users, room_path, main_user);
            for (int i = 0; i < user_count; i++) {
                int pid_write = fork();
                if (pid_write == 0) {
                    char cur_user_path[1024];
                    strcpy(cur_user_path, room_path);
                    strcat(cur_user_path, "/");
                    strcat(cur_user_path, room_users[i]);
                    mkfifo(cur_user_path, 0777);

                    int fd = open(cur_user_path, O_WRONLY | O_NONBLOCK);
                    if (fd == -1) {
                        exit(0); 
                    }
                    write(fd, message, strlen(message));
                    close(fd);
                    exit(0);
                } else {
                    continue;
                }
            }    
            while (wait(NULL) > 0);
        }   
    //Main process is used to continiously read from users' pipe.
    } else {
        int user_fifo = open(user_path, O_RDONLY);
        while (true) {
            char message[1028];
            int n = read(user_fifo, message, 1027);
            if (n > 0) {
                message[n] = '\0';
                printf("%s", message);
            }
        }
    }

    return 0;
}

//Helper function to find all existing pipes in the room. 
//Needs to be executed before each write since another user might be joined since first creation.
int find_all_users(char *room_users[], char *path, char *main_user) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(path);
    int cur = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, main_user) == 0) continue;
        
        room_users[cur] = malloc(strlen(entry->d_name) + 1);
        strcpy(room_users[cur], entry->d_name);
        cur++;
    }
    closedir(dir);

    return cur;
}
