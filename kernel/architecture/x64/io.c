#include <architecture/x64/io.h>

#include <stdint.h>

inline __attribute__((always_inline)) uint8_t io_read_b(uint16_t address) {
    uint8_t data;
    __asm__ volatile ("inb %1, %0\n":"=a"(data):"d"(address));
    return data;
}

inline __attribute__((always_inline)) void io_write_b(uint16_t address, uint8_t data) {
    __asm__ volatile ("outb %0, %1\n"::"a"(data), "d"(address));
}