#include "header/gles_local.h"

void _check_gl_error(const char *file, int line)
 {
        GLenum err = qglGetError();
 
        while(err != GL_NO_ERROR) {
                char *error;
 
                switch(err) {
                        case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
                        case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
                        case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
                        case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
//                        case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
                }
 
                Com_Printf("GL_%s, %s, %ld", error, file, line);
                err = qglGetError();
        }
}