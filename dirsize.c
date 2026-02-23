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

int calculate_size(char *path) {
    struct stat st;
    stat(path, &st);

    if (!S_ISDIR(st.st_mode)) {            
        return st.st_size;
    }

    int total = 0;

    DIR *dir = opendir(path);
    struct dirent *entry;

    while((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        
        char sub_dir[1028];
        sprintf(sub_dir, "%s/%s", path, entry->d_name);

        total += calculate_size(sub_dir);
    }

    return total;
}

int main(int argc, char *argv[]) {
    char *path = argv[1];
    if (path == NULL) path = ".";
    char *entries[1028];
    int sizes[1028];

    DIR *dir = opendir(path);
    struct dirent *entry;

    int tot_e = 0;
    while((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        entries[tot_e] = strdup(entry->d_name);

        char full_path[1028];
        sprintf(full_path, "%s/%s", path, entry->d_name);

        sizes[tot_e] = calculate_size(full_path);
        tot_e++;
    }

    for (int i = 0; i < tot_e; i++) {
        printf("name: %s, size %d bytes\n", entries[i], sizes[i]);
    }

    return 0;
}