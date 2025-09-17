#include <printk.h>
#include <stddef.h>
#include <stdarg.h>

int vsnprintf(char *buf, size_t size, const char *fmt, va_list ap);
void uartputc(char a);

static
int vprintk(const char *fmt, va_list ap)
{
    char buf[1024];
    int ret;
    char *p;

    ret = vsnprintf(buf, sizeof(buf), fmt, ap);

    for (p = buf; *p; p++)
        uartputc(*p);

    return ret;
}

int printk(const char *fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = vprintk(fmt, ap);
    va_end(ap);

    return ret;
}

void panic(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vprintk(fmt, ap);
    va_end(ap);

    asm ("cli");
    while (1)
        asm ("hlt");
}
