/*
 * A set of newlib system call stubs to be used with the arm-none-eabi Olimex board.
 * This is a hacked version of ones found online by nanoage and balau82.
 * Craig November 2014
 */

#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>
#include <stm32f10x.h>
#include "com_port.h"

#undef errno
extern int errno;

static inline unsigned int get_msp(void)
{
    unsigned int result;
    __asm volatile ("MRS %0, msp\n" : "=r" (result));
    return result;
}

unsigned int strlen(const char *s)
{
    unsigned int len = 0;

    while (s[len] != '\0')
    {
        len++;
    }

    return len;
}

/*
 * Read the Cortex-M3 Main Stack Pointer directly.
 * This replaces the need for get_msp() from core_cm3.c.
 */
/*
 * environ
 * A pointer to a list of environment variables and their values.
 * For a minimal environment, this empty list is adequate.
 */
char *__env[1] = { 0 };
char **environ = __env;

int _write(int file, char *ptr, int len);

void _exit(int status)
{
    _write(1, "exit", 4);

    while (1)
    {
        ;
    }
}

int _close(int file)
{
    return -1;
}

/*
 * execve
 * Transfer control to a new process.
 * Minimal implementation for a system without processes.
 */
int _execve(char *name, char **argv, char **env)
{
    errno = ENOMEM;
    return -1;
}

/*
 * fork
 * Create a new process.
 * Minimal implementation for a system without processes.
 */
int _fork(void)
{
    errno = EAGAIN;
    return -1;
}

/*
 * fstat
 * Status of an open file.
 * All files are treated as character special devices.
 */
int _fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

/*
 * getpid
 * Process ID.
 * Minimal implementation for a system without processes.
 */
int _getpid(void)
{
    return 1;
}

/*
 * isatty
 * Query whether the file is a terminal.
 */
int _isatty(int file)
{
    switch (file)
    {
        case STDOUT_FILENO:
        case STDERR_FILENO:
        case STDIN_FILENO:
            return 1;

        default:
            errno = EBADF;
            return 0;
    }
}

/*
 * kill
 * Send a signal.
 * Minimal implementation.
 */
int _kill(int pid, int sig)
{
    errno = EINVAL;
    return -1;
}

/*
 * link
 * Establish a new name for an existing file.
 * Minimal implementation.
 */
int _link(char *old, char *new)
{
    errno = EMLINK;
    return -1;
}

/*
 * lseek
 * Set position in a file.
 * Minimal implementation.
 */
int _lseek(int file, int ptr, int dir)
{
    return 0;
}

/*
 * sbrk
 * Increase program data space.
 * malloc() and related functions depend on this.
 *
 * This version starts the heap at _ebss, which is supplied by the linker script.
 * It checks the heap against the current stack pointer to avoid heap/stack collision.
 */
caddr_t _sbrk(int incr)
{
    extern char _ebss;          /* Defined by the linker script */
    static char *heap_end;
    char *prev_heap_end;
    char *stack;

    if (heap_end == 0)
    {
        heap_end = &_ebss;
    }

    prev_heap_end = heap_end;

    stack = (char *) get_msp();

    if (heap_end + incr > stack)
    {
        _write(STDERR_FILENO, "Heap and stack collision\n", 25);
        errno = ENOMEM;
        return (caddr_t) -1;
    }

    heap_end += incr;
    return (caddr_t) prev_heap_end;
}

/*
 * read
 * Read a character from a file.
 * This is still the starting stub version.
 * We will modify this later for scanf().
 */
int _read(int file, char *ptr, int len)
{
    int n;
    char c;

    switch (file)
    {
        case STDIN_FILENO:
            for (n = 0; n < len; n++)
            {
                c = inbyte();

                if (c == '\r')
                {
                    c = '\n';
                }

                ptr[n] = c;

                outbyte(c);

                if (c == '\n')
                {
                    return n + 1;
                }
            }

            return len;

        default:
            errno = EBADF;
            return -1;
    }
}
/*
 * stat
 * Status of a file by name.
 * Minimal implementation.
 */
int _stat(const char *filepath, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

/*
 * times
 * Timing information for current process.
 * Minimal implementation.
 */
clock_t _times(struct tms *buf)
{
    return -1;
}

/*
 * unlink
 * Remove a file's directory entry.
 * Minimal implementation.
 */
int _unlink(char *name)
{
    errno = ENOENT;
    return -1;
}

/*
 * wait
 * Wait for a child process.
 * Minimal implementation.
 */
int _wait(int *status)
{
    errno = ECHILD;
    return -1;
}

/*
 * write
 * Write characters to a file.
 * Newlib uses this for stdout and stderr.
 * Output is sent through USART using outbyte().
 */
int _write(int file, char *ptr, int len)
{
    int n;

    switch (file)
    {
        case STDOUT_FILENO:
        case STDERR_FILENO:
            for (n = 0; n < len; n++)
            {
                outbyte(*ptr++ & (u16)0x01FF);
            }
            break;

        default:
            errno = EBADF;
            return -1;
    }

    return len;
}
