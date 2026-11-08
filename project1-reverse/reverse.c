#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct Line {
    char *text;
    struct Line *next;  /* next (= earlier in the file) line */
} Line;

/* Print an error message to stderr and exit with code 1 */
static void die(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

/* Print the error with the file name and exit. */
static void die_cannot_open(const char *name) {
    fprintf(stderr, "error: cannot open file '%s'\n", name);
    exit(1);
}

/*
 * Return 1 if both paths refer to the same existing file, 0 otherwise
 * If the output file does not exist yet, it cannot be the same file
 */
static int same_file(const char *a, const char *b) {
    struct stat sa, sb;
    if (strcmp(a, b) == 0)
        return 1;
    if (stat(a, &sa) != 0 || stat(b, &sb) != 0)
        return 0;
    return sa.st_dev == sb.st_dev && sa.st_ino == sb.st_ino;
}

/* Read all lines from 'in' and return them as a list, last line first */
static Line *read_lines(FILE *in) {
    Line *head = NULL;
    char *buf = NULL;
    size_t cap = 0;

    /* getline() returns the number of characters read, or -1 at EOF */
    while (getline(&buf, &cap, in) != -1) {
        Line *node = malloc(sizeof(Line));
        if (node == NULL)
            die("malloc failed");

        /* Take ownership of the buffer getline() filled in, and let
         * getline() allocate a fresh one for the next line */
        node->text = buf;
        node->next = head;
        head = node;
        buf = NULL;
        cap = 0;
    }
    free(buf);

    /* getline() also returns -1 on a read error, so check for that */
    if (ferror(in))
        die("error: failed to read input");
    return head;
}

/* Print the list (which is already in reverse order) to 'out' */
static void write_lines(FILE *out, const Line *head) {
    for (const Line *l = head; l != NULL; l = l->next) {
        size_t len = strlen(l->text);
        fputs(l->text, out);
        /* Assumption: a last line without '\n' gets one in the output */
        if (len == 0 || l->text[len - 1] != '\n')
            fputc('\n', out);
    }
}

/* Free every node of the list */
static void free_lines(Line *head) {
    while (head != NULL) {
        Line *next = head->next;
        free(head->text);
        free(head);
        head = next;
    }
}

int main(int argc, char *argv[]) {
    FILE *in = stdin;
    FILE *out = stdout;

    if (argc > 3)
        die("usage: reverse <input> <output>");

    if (argc >= 2) {
        in = fopen(argv[1], "r");
        if (in == NULL)
            die_cannot_open(argv[1]);
    }

    if (argc == 3) {
        /* Check this BEFORE opening the output with "w", because opening
         * with "w" would truncate the input file if they were the same */
        if (same_file(argv[1], argv[2]))
            die("Input and output file must differ");
        out = fopen(argv[2], "w");
        if (out == NULL)
            die_cannot_open(argv[2]);
    }

    Line *lines = read_lines(in);
    write_lines(out, lines);
    free_lines(lines);

    if (in != stdin)
        fclose(in);
    if (out != stdout) {
        if (fclose(out) != 0)
            die("error: failed to write output");
    } else if (fflush(stdout) != 0) {
        die("error: failed to write output");
    }
    return 0;
}
