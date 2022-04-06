#include "types.h"
#include "stat.h"
#include "user.h"

char* substring(char*, int, int);
char buf[512];

//get a substring from the string with given position and length
char *substring(char *string, int position, int length) {
    char *p;
    int c;
 
    p = malloc(length+1);
    for (c = 0; c < length; c++) {
        *(p+c) = *(string+position);
        string++;
    }
    *(p+c) = '\0';
    return p;
}

void uniq(int fd) {
    int n, i, j;
    char *a, *b;
    
    //when read successfully from std input
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        a = 0; //initialize current line pointer
        j = 0; //initialize position index

        for (i = 0; i < n; i++) {
            if (buf[i] == '\n') {
                b = substring(buf, j, i-j); //get next line
                if (strcmp(a, b) != 0)      //compare current with next
                    printf(1, "%s\n", b);   //print line if different 
                j = i+1;
                a = b;
            }
            if (i == n-1) {
                b = substring(buf, j, n-j); //get last line
                if (strcmp(a, b) != 0)      //compare current with last
                    printf(1, "%s\n", b);   //print line if different
            }
        }
        free(b);
    }

    //when read unsuccessfully from std input
    if (n < 0) {
        printf(1, "cat: read error\n");
        exit();
    }
}

int main(int argc, char *argv[]) {
    int fd, i;
    
    //when read from pipe
    if (argc <= 1) {
        uniq(0);
        exit();
    }

    //when read from file
    for (i = 1; i < argc; i++) {
        if ((fd = open(argv[i], 0)) < 0) {
            printf(1, "uniq: cannot open %s\n", argv[1]);
            exit();
        }
        uniq(fd);
        close(fd);
    }
    exit();
}
