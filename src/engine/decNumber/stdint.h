/* stdint.h –– some standard integer types from C99 */

#if defined(_WIN32)

// Windows: There is no stdint.h
typedef unsigned char uint8_t;
typedef char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned long long uint64_t;
typedef long long int64_t;

#else

// Linux: Include file from standard library
#include <stdint.h>

#endif
