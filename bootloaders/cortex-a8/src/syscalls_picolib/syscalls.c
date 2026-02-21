#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/times.h>
#include <errno.h>
#include <stdio.h>

#include "uart.h"

// Defining stdout, stdin and stderr
static int stdout_putc(char c, FILE *file) {
    (void) file;
    while(uart_is_busy());
    uart_write_char(c);
    return c;
}

static int stdout_getc(FILE *file) {
    char c;
    (void) file;
    while(uart_new_data());
    c = uart_read_char();
    return (int) c;
}

static int stdout_flush(FILE *file) {
    (void) file;
    return 0;
}

static FILE __stdio = FDEV_SETUP_STREAM(stdout_putc, stdout_getc, stdout_flush, _FDEV_SETUP_RW);

FILE *const stdout = &__stdio;
FILE *const stdin = &__stdio;
FILE *const stderr = &__stdio;


// Syscalls
int close(int fd) {
    (void) fd;
    return -1;
}

void _exit(int status) {
    (void) status;
    while(1);
}

int fstat(int fd, struct stat *statbuf) {
    (void) fd;
    statbuf->st_mode = S_IFCHR;
    return 0;
}

int getentropy(void *buffer, size_t length) {
    (void) buffer;
    (void) length;
    return -1;
}

int gettimeofday(struct timeval * restrict tv, void * restrict tz) {
    (void) tv;
    (void) tz;
    errno = EINVAL;
    return -1;
}

off_t lseek(int fd, off_t offset, int whence) {
    (void) fd;
    (void) offset;
    (void) whence;
    return 0;
}

int open(char *path, int flags, ...) {
    (void) path;
    (void) flags;
    return -1;
}

ssize_t read(int fd, void *buf, size_t count) {
    (void) fd;
    (void) buf;
    (void) count;
    return 0;
}

void* sbrk(intptr_t incr) {
	extern char ld_heap_origin;     // set by linker
	extern char ld_heap_end;        // set by linker

	static char *heap_end = &ld_heap_origin;
	char *prev_heap_end;

	prev_heap_end = heap_end;
	heap_end += incr;

	if( heap_end > &ld_heap_end) {
        errno = ENOMEM;
		_exit(ENOMEM);
	}

	return (void*) prev_heap_end;
}

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset) {
    (void) how;
    (void) set;
    (void) oldset;
    return 0;
}

int stat(const char *path, struct stat *statbuf) {
    (void) path;
    statbuf->st_mode = S_IFCHR;
    return 0;
}

clock_t times(struct tms *buf) {
    (void) buf;
    return -1;
}

int unlink(const char *path) {
    (void) path;
    errno = ENOENT;
    return -1;
}

ssize_t write(int fd, const void *buf, size_t count) {
    (void) fd;

    for(size_t i = 0; i < count; i++)
    {
        while(uart_is_busy());
        uart_write_char(((char*) buf)[i]);
    }
    return count;
}