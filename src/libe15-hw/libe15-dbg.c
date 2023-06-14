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
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "libe15-dbg.h"

weak int32_t dev_putc(int ch)
{
    return EOF;
}

static volatile int32_t last_char = 0x00;

int32_t putc_warper(int ch)
{
    last_char = ch;
    return dev_putc(ch);
}

weak int32_t dev_puts(const char *s)
{
    int32_t i = 0;
    int32_t cnt = 0;
    while (*s != 0)
    {
        i = putc_warper(*s);
        if (i == EOF)
            return EOF;
        s++;
        cnt++;
    }
    return cnt;
}

int32_t puts_warper(const char *s)
{
    int32_t cnt = dev_puts(s);
    if (cnt != EOF)
        last_char = *(s + cnt - 1);
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

int is_fmt_specifier(int ch)
{
    switch (ch)
    {
    case 'd':
    case 'i':
    case 'c':
    case 'u':
    case 'o':
    case 'x':
    case 'X':
    case 'F':
    case 'f':
    case 'E':
    case 'e':
    case 'G':
    case 'g':
    case 'A':
    case 'a':
    case 's':
        return 1;
    default:
        return 0;
    }
}

int is_specifier_attr(int ch)
{
    switch (ch)
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '.':
    case '+':
    case '-':
    case '#':
    case '*':
        return 1;
    default:
        return 0;
    }
}
int get_fmt_string(const char *start, const char **end_out)
{
    while (*start != 0)
    {
        if (is_specifier_attr(*start))
            start++;
        else if (is_fmt_specifier(*start))
        {
            *end_out = start + 1;
            return 1;
        }
        else
            return 0;
    }
    return 0;
}

#undef print
void dbg_print(int32_t level, const char *location, const char *function, const char *msg, ...)
{
    va_list args;
    va_start(args, msg);

    UNUSED(location);
    UNUSED(function);
    UNUSED(level);

    if (last_char == '\n' || last_char == '\0')
    {

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
    }

    const char *ch = msg;
    while (*ch != 0)
    {
        if (*ch == '%')
        {
            const char *ch_end = NULL;
            if (get_fmt_string(ch + 1, &ch_end))
            {
                int32_t fmt_specifier = *(ch_end - 1);
                int32_t fmt_len = ch_end - ch;
                if (fmt_len > 32)
                    goto not_a_valid_fmt;

                char fmt[32] = {'%', 0};
                memcpy(fmt + 1, ch + 1, fmt_len - 1);
                char buf[64] = {0};
                char *output = buf;

                switch (fmt_specifier)
                {
                case 'd':
                case 'i':
                case 'c':
                {
                    int32_t d = va_arg(args, int32_t);
                    snprintf(buf, 32, fmt, d);
                    break;
                }

                case 'u':
                case 'o':
                case 'x':
                case 'X':
                {
                    int32_t u = va_arg(args, uint32_t);
                    snprintf(buf, 32, fmt, u);
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
                    snprintf(buf, 32, fmt, f);
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
                dev_puts(output);
                ch = ch_end;
                continue;
            }
            else
            {
            not_a_valid_fmt:
                putc_warper(*ch);
            }
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
                putc_warper('\r');
            putc_warper(*ch);
        }
        else
            putc_warper(*ch);

        ++ch;
    }

    va_end(args);
}
