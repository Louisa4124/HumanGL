#ifndef INTERNAL_H
#define INTERNAL_H

#ifndef GLFW_PLATFORM
# define GLFW_PLATFORM 0x00050003
#endif

#ifndef GLFW_PLATFORM_X11
# define GLFW_PLATFORM_X11 0x00060004
#endif

typedef enum
{
    OK,
    KO
} rcode;

typedef enum
{
    IDLE,
    RUNNING,
    JUMPING,
    SIX_SEVENNING,
} e_state;

#endif
