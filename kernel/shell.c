#include <shell.h>

#include <stdbool.h>
#include <stdint.h>
#include <graphics.h>
#include <keyboard.h>
#include <string.h>
#include <memory.h>

#define MAX_INPUT_BUFFER 256

char input_buffer[MAX_INPUT_BUFFER];

void shell() {
    while (true) {
        uint16_t input_buffer_index = 0;
        kernel_printf("\nshell > ");
        while ((input_buffer[input_buffer_index++] = get_char()) != '\n' && input_buffer_index < MAX_INPUT_BUFFER)
            kernel_print_char(input_buffer[input_buffer_index - 1]);
        input_buffer[input_buffer_index - 1] = '\0';
        kernel_print_char('\n');
        if (str_cmp(input_buffer, "exit") == 0)
            break;
        else if (str_cmp(input_buffer, "shutdown") == 0);
        else if (str_cmp(input_buffer, "memory") == 0)
            print_physical_memory_usage();
        else
            kernel_printf("Undefined command: %s", input_buffer);
    }
}

