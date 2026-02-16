#include "syscalls.h"
#include "uart.h"

char *__env[1] = { 0 };
char **environ = __env;
int errno;

void _exit(int status) {
    while(1);
}

int _close(int file) {
    (void) file;
    return -1;
}

int _execve(char *name, char **argv, char **env) {
    (void) name;
    (void) argv;
    (void) env;
    errno = ENOMEM;
    return -1;
}

int _fork(void) {
    errno = EAGAIN;
    return -1;
}

int _fstat(int file, struct stat *st) {
    (void) file;
    st->st_mode = S_IFCHR;
    return 0;
}

int _getpid(void) {
    return 1;
}

int _isatty(int file) {
    (void) file;
    return 1;
}

int _kill(int pid, int sig) {
    (void) pid;
    (void) sig;
    errno = EINVAL;
    return -1;
}

int _link(char *old, char *new) {
    (void) old;
    (void) new;
    errno = EMLINK;
    return -1;
}

int _lseek(int file, int ptr, int dir)
{
    (void) file;
    (void) ptr;
    (void) dir;
    return 0;
}

int _open(const char *name, int flags, int mode) {
    (void) name;
    (void) flags;
    (void) mode;
    return -1;
}

int _read(int file, char *ptr, int len)
{
    (void)file;
    (void) ptr;
    (void) len;
    return 0;
}

caddr_t _sbrk(int incr) {
	extern char ld_heap_origin;   // set by linker
	extern char ld_heap_end;     // set by linker

	static char *heap_end = &ld_heap_origin;
	char *prev_heap_end;

	prev_heap_end = heap_end;
	heap_end += incr;

	if( heap_end > &ld_heap_end) {
        errno = ENOMEM;
		_exit(ENOMEM);
	}

	return (caddr_t) prev_heap_end;
}

int _stat(char *file, struct stat *st)
{
    (void) file;
    st->st_mode = S_IFCHR;
    return 0;
}

int _times(struct tms *buf)
{
    (void) buf;
    return -1;
}

int _unlink(char *name)
{
    (void) name;
    errno = ENOENT;
    return -1;
}

int _wait(int *status)
{
    (void) status;
    errno = ECHILD;
    return -1;
}

int _write(int file, char *ptr, int len)
{
    (void) file;

    for(int i = 0; i < len; i++)
    {
        while(uart_is_busy());
        uart_write_char(ptr[i]);
    }
    return len;
}





















