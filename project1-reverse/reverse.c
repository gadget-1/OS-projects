#include <stdio.h>
#include <stdlib.h>


typedef struct Line {
    char *text;
    struct Line *next;   // point to next node
} Line;

int main(int argc, char *argv[]) {
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "cannot open file\n");
        exit(1);
    }

    Line *head = NULL;
    char *buf = NULL;
    size_t cap = 0;

    while (getline(&buf, &cap, fp) != -1) {
        Line *node = malloc(sizeof(Line));   // assign memory to new nodes
        node->text = buf;
        node->next = head;
        head = node;
        buf = NULL;          //  getline assign again
        cap = 0;
    }
    free(buf);
    fclose(fp);

    // Reverse
    Line *cur = head;
    while (cur != NULL) {
        printf("%s", cur->text);
        Line *next = cur->next;
        free(cur->text);     // release memory 
        free(cur);
        cur = next;
    }
    return 0;
}
