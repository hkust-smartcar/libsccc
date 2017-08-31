#include <assert.h>
#include <errno.h>
#include <stdint.h>

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>

#include "libbase/syscall.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

FwriteHandler g_fwrite_handler = NULL;

void _exit(int status)
{
	while (1)
	{}
}

int _open(const char *name, int flags, int mode)
{
	return -1;
}

int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(int file)
{
	return 1;
}

int _lseek(int file, int ptr, int dir)
{
	return 0;
}

int _read(int file, char *ptr, int len)
{
	return 0;
}

int _write(int file, char *ptr, int len)
{
	// This function won't fail even if __g_fwrite_handler is not set, it will
	// simply ignore the string in such case
	return g_fwrite_handler ? g_fwrite_handler(file, ptr, len) : len;
}

int _close(int file)
{
	return -1;
}

int _link(const char *oldpath, const char *newpath)
{
	return -1;
}

int _unlink(const char *pathname)
{
	return -1;
}

caddr_t _sbrk(int incr)
{
	// Defined by the linker
	extern uint32_t _end;
	extern uint32_t __stack_end;
	static char *heap_end;
	char *prev_heap_end;

	if (heap_end == 0)
	{
		heap_end = (char*)&_end;
	}
	prev_heap_end = heap_end;
	if ((uint32_t)(heap_end + incr) > (uint32_t)&__stack_end)
	{
		// Heap and stack collision
		assert(0);
	}

	heap_end += incr;
	return (caddr_t)prev_heap_end;
}

int _getpid(void)
{
	return 1;
}

int _times(struct tms *buf)
{
	return -1;
}

int _gettimeofday(struct timeval *p, void *z)
{
	return -1;
}

#undef errno
int _kill(int pid, int sig)
{
	extern int errno;
	errno = ENOSYS;
	return -1;
}

void KeepSyscallSymbols(void)
{
	void *v;
	v = (void*)_exit;
	v = (void*)_open;
	v = (void*)_fstat;
	v = (void*)_isatty;
	v = (void*)_lseek;
	v = (void*)_read;
	v = (void*)_write;
	v = (void*)_close;
	v = (void*)_link;
	v = (void*)_unlink;
	v = (void*)_sbrk;
	v = (void*)_getpid;
	v = (void*)_kill;
	if (v)
	{
		return;
	}
	else
	{
		return;
	}
}
