/**
 * @file libe15-dbg.h
 * @author Simakeng (simakeng@outlook.com)
 * @brief Debugging functions
 * @version 0.1
 * @date 2023-04-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdint.h>
#include <enviroment.h>

// If you want to disable all the debug messages,
// uncomment the line below this will save you some ROM space
// #define CONFIG_OMIT_MESSAGE 1


// If you dont want to print the location of the function call
// uncomment the line below
// #define CONFIG_OMIT_LOCATION 1

// if you dont want to print the function name
// uncomment the line below
// #define CONFIG_OMIT_FUNCTION_NAME 1

// if you dont want to print the message level
// uncomment the line below
// #define CONFIG_OMIT_LEVEL 1


/**
 * @brief Print an error message
 *!@warning to use this function you must implement dev_putc
 * @param level The level of the message
 * @param location The location where this function is called
 * @param function The function name of the caller function
 * @param msg The message to print
 * @param ... The arguments to the message
 */
void print(int32_t level, const char *location, const char *function, const char *msg, ...);

/**
 * @brief put a char into an output device
 * default behaviour is to do nothing
 *!@warning this function must be implemented by the user
 * @param ch ASCII char code
 * @return int32_t ch: success, EOF: failure
 */
weak int32_t dev_putc(int ch);

/**
 * @brief put a string into an output device
 * default behaviour is call dev_putc on each char
 * @param s The string to print
 * @return int32_t num of char puted, EOF: failure
 */
weak int32_t dev_puts(const char *s);

typedef enum
{
    LDEBUG,
    LINFO,
    LWARNING,
    LERROR,
    LFATAL
} debug_msg_level_e;
#define DBG_TRANSLATE_LINE(line) #line

#define DBG_TRANSLATE_LOCATION(file, line) file ":" DBG_TRANSLATE_LINE(line)

#ifdef CONFIG_OMIT_MESSAGE
#define print(code) ((void)0)
#else
#define print(level, ...) print(level, DBG_TRANSLATE_LOCATION(__FILE__, __LINE__), __FUNCTION__, __VA_ARGS__)
#endif

#define debug(...) print(LDEBUG, __VA_ARGS__)

#define error(...) print(LERROR, __VA_ARGS__)
