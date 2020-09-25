#include <shell.h>

#include <stdbool.h>
#include <stdint.h>
#include <graphics.h>
#include <keyboard.h>
#include <string.h>
#include <memory.h>
#include <power.h>
#include <datetime.h>

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
        else if (str_cmp(input_buffer, "shutdown") == 0)
            reset_system_power(ResetShutdown);
        else if (str_cmp(input_buffer, "memory") == 0)
            print_memory_info();
        else if (str_cmp(input_buffer, "uname") == 0)
            kernel_print_string_n("Egoistic OS Kernel ver.0.1");
        else if (str_cmp(input_buffer, "date") == 0) {
            DateTime time = get_datetime();
            kernel_printf("%04u Year  %02u Month  %02u Day  %02u:%02u:%02u", time.year, time.month, time.day, time.hour,
                          time.minute, time.second);
        } else
            kernel_printf("Undefined command: %s", input_buffer);
    }
}

