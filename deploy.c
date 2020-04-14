#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <assert.h>

#define MAX_BUFFER_SIZE 50

const char COMMAND_LINE_ARGUMENT[] = "qemu-system-x86_64 -m 8G -smp 4 -drive if=pflash,format=raw,file=./OVMF.fd -drive file=fat:rw:./fs,format=raw";

void copy_file(const char *in, const char *out) {
    int in_fd = open(in, O_RDONLY);
    assert(in_fd != -1);
    int out_fd = open(out, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    assert(out_fd != -1);

    struct stat st;
    stat(in, &st);
    int count = sendfile(out_fd, in_fd, NULL, st.st_size);

    printf("%d bytes were written to %s from %s\n", count, out, in);
}

static char *command_list[MAX_BUFFER_SIZE];

int parse_command(const char *command_string) {
    char command_buffer[MAX_BUFFER_SIZE];
    int command_y = 0, command_buffer_pointer = 0;
    for (int i = 0;; ++i) {
        if (command_string[i] == ' ' || command_string[i] == '\0') {
            char *command = malloc(sizeof(char) * command_buffer_pointer);
            command_list[command_y++] = command;
            strncpy(command, command_buffer, command_buffer_pointer);
            command_buffer_pointer = 0;
            if (command_string[i] == '\0') break;
            continue;
        }
        command_buffer[command_buffer_pointer++] = command_string[i];
    }
    return command_y;
}

int execute_command(const char *command_line_argument) {
    puts(COMMAND_LINE_ARGUMENT);
    parse_command(command_line_argument);
    errno = 0;
    execvp(command_list[0], command_list);
    if (errno != 0)
        perror(strerror(errno));
    return -1;
}

int main(int argc, char **argv) {
    if (argc <= 2) {
        printf("Usage: %s [[EXEFILE] [OUTPUTFILE]]...\n", argv[0]);
        return -1;
    }
    for (int i = 0; i < argc - 1; i += 2)
        copy_file(argv[1 + i], argv[2 + i]);

    return execute_command(COMMAND_LINE_ARGUMENT);
}