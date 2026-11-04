#include "internal.h"
#include "utils.h"
#include <GL/glew.h>

rcode initGlew()
{
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        utils::err("Failed to initialize GLEW\n");
        return KO;
    }
    return OK;
}
