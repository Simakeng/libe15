/**
 * @file libe15-errors.h
 * @author Simakeng (simakeng@outlook.com)
 * @brief Error codes for libe15
 * @version 0.1
 * @date 2023-04-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdint.h>

typedef int32_t error_t;

#define ALL_OK 0


#define E_INVALID_ARGUMENT      -(90001)
#define E_INVALID_ADDRESS       -(90002)


#define E_HARDWARE_ERROR        -(60001)

/** operation try to access hardware waits too long */
#define E_HARDWARE_TIMEOUT      -(60500)
/** device is not available now */
#define E_HARDWARE_RESOURCE_BUSY     -(60304)
/** device is not found */
#define E_HARDWARE_NOTFOUND     -(60404)




