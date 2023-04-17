/**
 * @file libe15-dbg.c
 * @author Simakeng (simakeng@outlook.com)
 * @brief Debugging functions
 * @version 0.1
 * @date 2023-04-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdarg.h>
#include <stdio.h>
#include "libe15-dbg.h"

weak int32_t dev_putc(int ch)
{
    return EOF;
}

weak int32_t dev_puts(const char *s)
{
    int32_t i = 0;
    int32_t cnt = 0;
    while (*s != 0)
    {
        i = dev_putc(*s);
        if (i == EOF)
            return EOF;
        s++;
        cnt++;
    }
    return cnt;
}

const char *translate_level(int32_t level)
{
    switch (level)
    {
    case LDEBUG:
        return "DEBUG";
    case LINFO:
        return "INFO ";
    case LWARNING:
        return "WARN ";
    case LERROR:
        return "ERROR";
    case LFATAL:
        return "FATAL";
    default:
        return "UNKNO";
    }
}

#undef print
void print(int32_t level, const char *location, const char *function, const char *msg, ...)
{
    va_list args;
    va_start(args, msg);

    UNUSED(location);
    UNUSED(function);
    UNUSED(level);

#ifndef CONFIG_OMIT_LEVEL
    dev_puts("[");
    dev_puts(translate_level(level));
    dev_puts("] ");
#endif

#ifndef CONFIG_OMIT_LOCATION
    dev_puts("[");
    dev_puts(location);
    dev_puts("] ");
#endif

#ifndef CONFIG_OMIT_FUNCTION_NAME
    dev_puts("[");
    dev_puts(function);
    dev_puts("] ");
#endif

    const char *ch = msg;
    while (*ch != 0)
    {
        if (*ch == '%')
        {
            int32_t fmt_specifier = *(ch + 1);
            char spec[] = {'%', fmt_specifier, 0};
            char buf[32] = {0};
            char *output = buf;
            switch (fmt_specifier)
            {
            case 'd':
            case 'i':
            case 'c':
            {
                int32_t d = va_arg(args, int32_t);
                snprintf(buf, 32, spec, d);
                break;
            }

            case 'u':
            case 'o':
            case 'x':
            case 'X':
            {
                int32_t u = va_arg(args, uint32_t);
                snprintf(buf, 32, spec, u);
                break;
            }

            case 'F':
            case 'f':
            case 'E':
            case 'e':
            case 'G':
            case 'g':
            case 'A':
            case 'a':
            {
                double f = va_arg(args, double);
                snprintf(buf, 32, spec, f);
                break;
            }

            case 's':
            {
                output = va_arg(args, char *);
                break;
            }

            case '%':
            {
                buf[0] = '%';
                buf[1] = 0;
                break;
            }
            default:
                buf[0] = '%';
                buf[1] = fmt_specifier;
                buf[2] = 0;
                break;
            }
            ++ch;
            dev_puts(output);
        }
        // The intention is to add a '\r' character before the '\n' character.
        // If the 'ch' pointer is pointing to the first character of the msg
        // string, or the previous character is not '\r', then we add it.
        else if (*ch == '\n')
        {
            // Because '||' is a short-circuit operator, if the first condition
            // is true, the second one will not be evaluated. and if the first
            // condition is false, then we can safely access the previous
            // character.
            if (ch == msg || *(ch - 1) != '\r')
                dev_putc('\r');
            dev_putc(*ch);
        }
        else
            dev_putc(*ch);
        ++ch;
    }

    va_end(args);
}
