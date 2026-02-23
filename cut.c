#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <termios.h> // termios, TCSANOW, ECHO, ICANON
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    char buf[4096];
    char delimiter = '\t';
    char *fields_raw = NULL;
    int fields[512];
    char *filename = NULL;

    // Parsing logic for fields required in cut.
    //Supported formats are {-d ":"}, {--delimiter ":"}, {-f 1,2 or --fields 1,2} and {-f1,2 or --fields1,2}
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--delimiter") == 0) {
            if (i < argc - 1 && strlen(argv[i + 1]) == 1) {
                delimiter = argv[i + 1][0];
                i++;
            }
        } else if (strncmp(argv[i], "-f", 2) == 0 && strlen(argv[i]) > 2) {
            fields_raw = &argv[i][2];
            
        } else if (strncmp(argv[i], "--fields", 8) == 0 && strlen(argv[i]) > 8) {
            fields_raw = &argv[i][8];
        } else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--fields") == 0 ) {
            if (i < argc - 1) {
                fields_raw = argv[i + 1];
                i++;
            }
        } else {
            filename = argv[i];
        } 
    }
    
    int field_order_count = 0;
    if (fields_raw != NULL) {
        char *copy = strdup(fields_raw);
        char *pch = strtok(copy, ",");    
        while (pch != NULL) {
            fields[field_order_count++] = atoi(pch) - 1;
            pch = strtok(NULL, ",");
        }

    }

    //After parsing file is opened.
    FILE *fd = stdin;
    if (filename != NULL) {
        fd = fopen(filename, "r");
    }

    //Each line in file processed seperately and printed.
    //Processing works by states defined by argv and fields are stored in a buffer and printed based of fields.
    while (fgets(buf, sizeof(buf), fd)) {
        buf[strcspn(buf, "\n")] = '\0';

        if (fields_raw == NULL) {
            printf("%s\n", buf);
            continue;
        }

        char delim_str[2] = { delimiter, '\0' };
        char *copy = strdup(buf);
        char *pch = strtok(copy, delim_str);

        char *field_buf[1028] = {0};
        int file_fields_count = 0;
        while (pch != NULL) {
            field_buf[file_fields_count++] = pch;
            pch = strtok(NULL, delim_str);
        }

        bool no_print = true;
        for (int i = 0; i < field_order_count; i++) {
            if (fields[i] >= file_fields_count) continue;

            if (!no_print) { printf("%c", delimiter); }
        
            if (fields[i] < file_fields_count) {
                printf("%s", field_buf[fields[i]]);
                no_print = false;
            }
        }
        if (no_print) { printf("no field"); }
        printf("\n");
    }
    return 0;
}