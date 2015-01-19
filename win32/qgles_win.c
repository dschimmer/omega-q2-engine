/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
/*
** QGL_WIN.C
**
** This file implements the operating system binding of GL to QGL function
** pointers.  When doing a port of Quake2 you must implement the following
** two functions:
**
** QGL_Init() - loads libraries, assigns function pointers, etc.
** QGL_Shutdown() - unloads libraries, NULLs function pointers
*/
#include <float.h>
#include "../ref_gles/header/gles_local.h""
#include "glw_win.h"

int   ( WINAPI * qwglChoosePixelFormat )(HDC, CONST PIXELFORMATDESCRIPTOR *);
int   ( WINAPI * qwglDescribePixelFormat) (HDC, int, UINT, LPPIXELFORMATDESCRIPTOR);
int   ( WINAPI * qwglGetPixelFormat)(HDC);
BOOL  ( WINAPI * qwglSetPixelFormat)(HDC, int, CONST PIXELFORMATDESCRIPTOR *);
BOOL  ( WINAPI * qwglSwapBuffers)(HDC);

BOOL  ( WINAPI * qwglCopyContext)(HGLRC, HGLRC, UINT);
HGLRC ( WINAPI * qwglCreateContext)(HDC);
HGLRC ( WINAPI * qwglCreateLayerContext)(HDC, int);
BOOL  ( WINAPI * qwglDeleteContext)(HGLRC);
HGLRC ( WINAPI * qwglGetCurrentContext)(VOID);
HDC   ( WINAPI * qwglGetCurrentDC)(VOID);
PROC  ( WINAPI * qwglGetProcAddress)(LPCSTR);
BOOL  ( WINAPI * qwglMakeCurrent)(HDC, HGLRC);
BOOL  ( WINAPI * qwglShareLists)(HGLRC, HGLRC);
BOOL  ( WINAPI * qwglUseFontBitmaps)(HDC, DWORD, DWORD, DWORD);

BOOL  ( WINAPI * qwglUseFontOutlines)(HDC, DWORD, DWORD, DWORD, FLOAT,
                                           FLOAT, int, LPGLYPHMETRICSFLOAT);

BOOL ( WINAPI * qwglDescribeLayerPlane)(HDC, int, int, UINT,
                                            LPLAYERPLANEDESCRIPTOR);
int  ( WINAPI * qwglSetLayerPaletteEntries)(HDC, int, int, int,
                                                CONST COLORREF *);
int  ( WINAPI * qwglGetLayerPaletteEntries)(HDC, int, int, int,
                                                COLORREF *);
BOOL ( WINAPI * qwglRealizeLayerPalette)(HDC, int, BOOL);
BOOL ( WINAPI * qwglSwapLayerBuffers)(HDC, UINT);

void ( APIENTRY * qglActiveTexture )(GLenum);
void ( APIENTRY * qglAlphaFunc )(GLenum func, GLclampf ref);
void ( APIENTRY * qglAlphaFuncx )(GLenum func, GLclampx ref);
void ( APIENTRY * qglBindBuffer )( GLenum target, GLuint buffer);
void ( APIENTRY * qglBindTexture )(GLenum target, GLuint texture);
void ( APIENTRY * qglBlendFunc )(GLenum sfactor, GLenum dfactor);
void ( APIENTRY * qglBufferData )(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
void ( APIENTRY * qglBufferSubData )(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
void ( APIENTRY * qglClear )(GLbitfield mask);
void ( APIENTRY * qglClearColor )(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void ( APIENTRY * qglClearColorx )(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);
void ( APIENTRY * qglClearDepthf )(GLclampf depth);
void ( APIENTRY * qglClearDepthx )(GLclampx depth);
void ( APIENTRY * qglClearStencil )(GLint s);
void ( APIENTRY * qglClientActiveTexture )(GLenum texture);
void ( APIENTRY * qglClipPlanef )(GLenum plane, const GLfloat *equation);
void ( APIENTRY * qglClipPlanex )(GLenum plane, const GLfixed *equation);
void ( APIENTRY * qglColor4f )(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void ( APIENTRY * qglColor4ub )(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
void ( APIENTRY * qglColor4x )(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);
void ( APIENTRY * qglColorMask )(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
void ( APIENTRY * qglColorPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void ( APIENTRY * qglCompressedTexImage2D )(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
void ( APIENTRY * qglCompressedTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
void ( APIENTRY * qglCopyTexImage2D )(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void ( APIENTRY * qglCopyTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void ( APIENTRY * qglCullFace )(GLenum mode);
void ( APIENTRY * qglDeleteBuffers )(GLsizei n, const GLuint *buffers);
void ( APIENTRY * qglDeleteTextures )(GLsizei n, const GLuint *textures);
void ( APIENTRY * qglDepthFunc )(GLenum func);
void ( APIENTRY * qglDepthMask )(GLboolean flag);
void ( APIENTRY * qglDepthRangef )(GLclampf zNear, GLclampf zFar);
void ( APIENTRY * qglDepthRangex )(GLclampx zNear, GLclampx zFar);
void ( APIENTRY * qglDisable )(GLenum cap);
void ( APIENTRY * qglDisableClientState )(GLenum array);
void ( APIENTRY * qglDrawArrays )(GLenum mode, GLint first, GLsizei count);
void ( APIENTRY * qglDrawElements )(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
void ( APIENTRY * qglEnable )(GLenum cap);
void ( APIENTRY * qglEnableClientState )(GLenum array);
void ( APIENTRY * qglFinish )(void);
void ( APIENTRY * qglFlush )(void);
void ( APIENTRY * qglFogf )(GLenum pname, GLfloat param);
void ( APIENTRY * qglFogfv )(GLenum pname, const GLfloat *params);
void ( APIENTRY * qglFogx )(GLenum pname, GLfixed param);
void ( APIENTRY * qglFogxv )(GLenum pname, const GLfixed *params);
void ( APIENTRY * qglFrontFace )(GLenum mode);
void ( APIENTRY * qglFrustumf )(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
void ( APIENTRY * qglFrustumx )(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
void ( APIENTRY * qglGenBuffers )(GLsizei n, GLuint *buffers);
void ( APIENTRY * qglGenTextures )(GLsizei n, GLuint *textures);
void ( APIENTRY * qglGetBooleanv )(GLenum pname, GLboolean *params);
void ( APIENTRY * qglGetFixedv )(GLenum pname, GLfixed *params);
void ( APIENTRY * qglGetFloatv )(GLenum pname, GLfloat *params);
void ( APIENTRY * qglGetIntegerv )(GLenum pname, GLint *params);
void ( APIENTRY * qglGetBufferParameteriv )(GLenum target, GLenum pname, GLint *params);
void ( APIENTRY * qglGetClipPlanef )(GLenum plane, GLfloat *equation);
void ( APIENTRY * qglGetClipPlanex )(GLenum plane, GLfixed *equation);
GLenum ( APIENTRY * qglGetError )(void);
void ( APIENTRY * qglGetLightfv )(GLenum light, GLenum pname, GLfloat *params);
void ( APIENTRY * qglGetLightxv )(GLenum light, GLenum pname, GLfixed *params);
void ( APIENTRY * qglGetMaterialfv )(GLenum face, GLenum pname, GLfloat *params);
void ( APIENTRY * qglGetMaterialxv )(GLenum face, GLenum pname, GLfixed *params);
void ( APIENTRY * qglGetPointerv )(GLenum pname, GLvoid* *params);
const GLubyte * ( APIENTRY * qglGetString )(GLenum name);
void ( APIENTRY * qglGetTexEnvfv )(GLenum target, GLenum pname, GLfloat *params);
void ( APIENTRY * qglGetTexEnviv )(GLenum target, GLenum pname, GLint *params);
void ( APIENTRY * qglGetTexEnvxv )(GLenum target, GLenum pname, GLfixed *params);
void ( APIENTRY * qglGetTexParameterfv )(GLenum target, GLenum pname, GLfloat *params);
void ( APIENTRY * qglGetTexParameteriv )(GLenum target, GLenum pname, GLint *params);
void ( APIENTRY * qglGetTexParameterxv )(GLenum target, GLenum pname, GLfixed *params);
void ( APIENTRY * qglHint )(GLenum target, GLenum mode);
GLboolean ( APIENTRY * qglIsBuffer )(GLuint buffer);
GLboolean ( APIENTRY * qglIsEnabled )(GLenum cap);
GLboolean ( APIENTRY * qglIsTexture )(GLuint texture);
void ( APIENTRY * qglLightf )(GLenum light, GLenum pname, GLfloat param);
void ( APIENTRY * qglLightfv )(GLenum light, GLenum pname, const GLfloat *params);
void ( APIENTRY * qglLightx )(GLenum light, GLenum pname, GLfixed param);
void ( APIENTRY * qglLightxv )(GLenum light, GLenum pname, const GLfixed *params);
void ( APIENTRY * qglLightModelf )(GLenum pname, GLfloat param);
void ( APIENTRY * qglLightModelfv )(GLenum pname, const GLfloat *params);
void ( APIENTRY * qglLightModelx )(GLenum pname, GLfixed param);
void ( APIENTRY * qglLightModelxv )(GLenum pname, const GLfixed *params);
void ( APIENTRY * qglLineWidth )(GLfloat width);
void ( APIENTRY * qglLineWidthx )(GLfixed width);
void ( APIENTRY * qglLoadIdentity )(void);
void ( APIENTRY * qglLoadMatrixf )(const GLfloat *m);
void ( APIENTRY * qglLoadMatrixx )(const GLfixed *m);
void ( APIENTRY * qglLogicOp )(GLenum opcode);
void ( APIENTRY * qglMaterialf )(GLenum face, GLenum pname, GLfloat param);
void ( APIENTRY * qglMaterialfv )(GLenum face, GLenum pname, const GLfloat *params);
void ( APIENTRY * qglMaterialx )(GLenum face, GLenum pname, GLfixed param);
void ( APIENTRY * qglMaterialxv )(GLenum face, GLenum pname, const GLfixed *params);
void ( APIENTRY * qglMatrixMode )(GLenum mode);
void ( APIENTRY * qglMultMatrixf )(const GLfloat *m);
void ( APIENTRY * qglMultMatrixx )(const GLfixed *m);
void ( APIENTRY * qglMultiTexCoord4f )(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
void ( APIENTRY * qglMultiTexCoord4x )(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);
void ( APIENTRY * qglNormal3f )(GLfloat nx, GLfloat ny, GLfloat nz);
void ( APIENTRY * qglNormal3x )(GLfixed nx, GLfixed ny, GLfixed nz);
void ( APIENTRY * qglNormalPointer )(GLenum type, GLsizei stride, const GLvoid *pointer);
void ( APIENTRY * qglOrthof )(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
void ( APIENTRY * qglOrthox )(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
void ( APIENTRY * qglPixelStorei )(GLenum pname, GLint param);
void ( APIENTRY * qglPointParameterf )(GLenum pname, GLfloat param);
void ( APIENTRY * qglPointParameterfv )(GLenum pname, const GLfloat *params);
void ( APIENTRY * qglPointParameterx )(GLenum pname, GLfixed param);
void ( APIENTRY * qglPointParameterxv )(GLenum pname, const GLfixed *params);
void ( APIENTRY * qglPointSize )(GLfloat size);
void ( APIENTRY * qglPointSizex )(GLfixed size);
void ( APIENTRY * qglPointSizePointerOES )(GLenum type, GLsizei stride, const GLvoid *pointer);
void ( APIENTRY * qglPolygonOffset )(GLfloat factor, GLfloat units);
void ( APIENTRY * qglPolygonOffsetx )(GLfixed factor, GLfixed units);
void ( APIENTRY * qglPopMatrix )(void);
void ( APIENTRY * qglPushMatrix )(void);
void ( APIENTRY * qglReadPixels )(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
void ( APIENTRY * qglRotatef )(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void ( APIENTRY * qglRotatex )(GLfixed angle, GLfixed x, GLfixed y, GLfixed z);
void ( APIENTRY * qglSampleCoverage )(GLclampf value, GLboolean invert);
void ( APIENTRY * qglSampleCoveragex )(GLclampx value, GLboolean invert);
void ( APIENTRY * qglScalef )(GLfloat x, GLfloat y, GLfloat z);
void ( APIENTRY * qglScalex )(GLfixed x, GLfixed y, GLfixed z);
void ( APIENTRY * qglScissor )(GLint x, GLint y, GLsizei width, GLsizei height);
void ( APIENTRY * qglShadeModel )(GLenum mode);
void ( APIENTRY * qglStencilFunc )(GLenum func, GLint ref, GLuint mask);
void ( APIENTRY * qglStencilMask )(GLuint mask);
void ( APIENTRY * qglStencilOp )(GLenum fail, GLenum zfail, GLenum zpass);
void ( APIENTRY * qglTexCoordPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void ( APIENTRY * qglTexEnvf )(GLenum target, GLenum pname, GLfloat param);
void ( APIENTRY * qglTexEnvfv )(GLenum target, GLenum pname, const GLfloat *params);
void ( APIENTRY * qglTexEnvi )(GLenum target, GLenum pname, GLint param);
void ( APIENTRY * qglTexEnviv )(GLenum target, GLenum pname, const GLint *params);
void ( APIENTRY * qglTexEnvx )(GLenum target, GLenum pname, GLfixed param);
void ( APIENTRY * qglTexEnvxv )(GLenum target, GLenum pname, const GLfixed *params);
void ( APIENTRY * qglTexImage2D )(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void ( APIENTRY * qglTexParameterf )(GLenum target, GLenum pname, GLfloat param);
void ( APIENTRY * qglTexParameterfv )(GLenum target, GLenum pname, const GLfloat *params);
void ( APIENTRY * qglTexParameteri )(GLenum target, GLenum pname, GLint param);
void ( APIENTRY * qglTexParameteriv )(GLenum target, GLenum pname, const GLint *params);
void ( APIENTRY * qglTexParameterx )(GLenum target, GLenum pname, GLfixed param);
void ( APIENTRY * qglTexParameterxv )(GLenum target, GLenum pname, const GLfixed *params);
void ( APIENTRY * qglTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
void ( APIENTRY * qglTranslatef )(GLfloat x, GLfloat y, GLfloat z);
void ( APIENTRY * qglTranslatex )(GLfixed x, GLfixed y, GLfixed z);
void ( APIENTRY * qglVertexPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void ( APIENTRY * qglViewport )(GLint x, GLint y, GLsizei width, GLsizei height);

void ( APIENTRY * qglLockArraysEXT)( int, int);
void ( APIENTRY * qglUnlockArraysEXT) ( void );

BOOL ( WINAPI * qwglSwapIntervalEXT)( int interval );
BOOL ( WINAPI * qwglGetDeviceGammaRampEXT)( unsigned char *, unsigned char *, unsigned char * );
BOOL ( WINAPI * qwglSetDeviceGammaRampEXT)( const unsigned char *, const unsigned char *, const unsigned char * );
void ( APIENTRY * qglPointParameterfEXT)( GLenum param, GLfloat value );
void ( APIENTRY * qglPointParameterfvEXT)( GLenum param, const GLfloat *value );
void ( APIENTRY * qglColorTableEXT)( int, int, int, int, int, const void * );
void ( APIENTRY * qglSelectTextureSGIS)( GLenum );
void ( APIENTRY * qglMTexCoord2fSGIS)( GLenum, GLfloat, GLfloat );
void ( APIENTRY * qglActiveTextureARB) ( GLenum );
void ( APIENTRY * qglClientActiveTextureARB) ( GLenum );


static void ( APIENTRY * dllActiveTexture )(GLenum texture);
static void ( APIENTRY * dllAlphaFunc )(GLenum func, GLclampf ref);
static void ( APIENTRY * dllAlphaFuncx )(GLenum func, GLclampx ref);
static void ( APIENTRY * dllBindBuffer )(GLenum target, GLuint buffer);
static void ( APIENTRY * dllBindTexture )(GLenum target, GLuint texture);
static void ( APIENTRY * dllBlendFunc )(GLenum sfactor, GLenum dfactor);
static void ( APIENTRY * dllBufferData )(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
static void ( APIENTRY * dllBufferSubData )(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
static void ( APIENTRY * dllClear )(GLbitfield mask);
static void ( APIENTRY * dllClearColor )(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
static void ( APIENTRY * dllClearColorx )(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);
static void ( APIENTRY * dllClearDepthf )(GLclampf depth);
static void ( APIENTRY * dllClearDepthx )(GLclampx depth);
static void ( APIENTRY * dllClearStencil )(GLint s);
static void ( APIENTRY * dllClientActiveTexture )(GLenum texture);
static void ( APIENTRY * dllClipPlanef )(GLenum plane, const GLfloat *equation);
static void ( APIENTRY * dllClipPlanex )(GLenum plane, const GLfixed *equation);
static void ( APIENTRY * dllColor4f )(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
static void ( APIENTRY * dllColor4ub )(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
static void ( APIENTRY * dllColor4x )( GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);
static void ( APIENTRY * dllColorMask )(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
static void ( APIENTRY * dllColorPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
static void ( APIENTRY * dllCompressedTexImage2D )(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
static void ( APIENTRY * dllCompressedTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
static void ( APIENTRY * dllCopyTexImage2D )(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
static void ( APIENTRY * dllCopyTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
static void ( APIENTRY * dllCullFace )(GLenum mode);
static void ( APIENTRY * dllDeleteBuffers )(GLsizei n, const GLuint *buffers);
static void ( APIENTRY * dllDeleteTextures )(GLsizei n, const GLuint *textures);
static void ( APIENTRY * dllDepthFunc )(GLenum func);
static void ( APIENTRY * dllDepthMask )(GLboolean flag);
static void ( APIENTRY * dllDepthRangef )(GLclampf zNear, GLclampf zFar);
static void ( APIENTRY * dllDepthRangex )(GLclampx zNear, GLclampx zFar);
static void ( APIENTRY * dllDisable )(GLenum cap);
static void ( APIENTRY * dllDisableClientState )(GLenum array);
static void ( APIENTRY * dllDrawArrays )(GLenum mode, GLint first, GLsizei count);
static void ( APIENTRY * dllDrawElements )(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
static void ( APIENTRY * dllEnable )(GLenum cap);
static void ( APIENTRY * dllEnableClientState )(GLenum array);
static void ( APIENTRY * dllFinish )(void);
static void ( APIENTRY * dllFlush )(void);
static void ( APIENTRY * dllFogf )(GLenum pname, GLfloat param);
static void ( APIENTRY * dllFogfv )(GLenum pname, const GLfloat *params);
static void ( APIENTRY * dllFogx )(GLenum pname, GLfixed param);
static void ( APIENTRY * dllFogxv )(GLenum pname, const GLfixed *params);
static void ( APIENTRY * dllFrontFace )(GLenum mode);
static void ( APIENTRY * dllFrustumf )(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
static void ( APIENTRY * dllFrustumx )(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
static void ( APIENTRY * dllGenBuffers )(GLsizei n, GLuint *buffers);
static void ( APIENTRY * dllGenTextures )(GLsizei n, GLuint *textures);
static void ( APIENTRY * dllGetBufferParameteriv )(GLenum target, GLenum pname, GLint *params);
static void ( APIENTRY * dllGetBooleanv )(GLenum pname, GLboolean *params);
static void ( APIENTRY * dllGetFixedv )(GLenum pname, GLfixed *params);
static void ( APIENTRY * dllGetFloatv )(GLenum pname, GLfloat *params);
static void ( APIENTRY * dllGetIntegerv )(GLenum pname, GLint *params);
static void ( APIENTRY * dllGetClipPlanef )(GLenum plane, GLfloat *equation);
static void ( APIENTRY * dllGetClipPlanex )(GLenum plane, GLfixed *equation);
GLenum ( APIENTRY * dllGetError )(void);
static void ( APIENTRY * dllGetLightfv )(GLenum light, GLenum pname, GLfloat *params);
static void ( APIENTRY * dllGetLightiv )(GLenum light, GLenum pname, GLint *params);
static void ( APIENTRY * dllGetLightxv )(GLenum light, GLenum pname, GLfixed *params);
static void ( APIENTRY * dllGetMaterialfv )(GLenum face, GLenum pname, GLfloat *params);
static void ( APIENTRY * dllGetMaterialxv )(GLenum face, GLenum pname, GLfixed *params);
static void ( APIENTRY * dllGetPointerv )(GLenum pname, GLvoid* *params);
const GLubyte * ( APIENTRY * dllGetString )(GLenum name);
static void ( APIENTRY * dllGetTexEnvfv )(GLenum target, GLenum pname, GLfloat *params);
static void ( APIENTRY * dllGetTexEnviv )(GLenum target, GLenum pname, GLint *params);
static void ( APIENTRY * dllGetTexEnvxv )(GLenum target, GLenum pname, GLfixed *params);
static void ( APIENTRY * dllGetTexParameterfv )(GLenum target, GLenum pname, GLfloat *params);
static void ( APIENTRY * dllGetTexParameteriv )(GLenum target, GLenum pname, GLint *params);
static void ( APIENTRY * dllGetTexParameterxv )(GLenum target, GLenum pname, GLfixed *params);
static void ( APIENTRY * dllHint )(GLenum target, GLenum mode);
GLboolean ( APIENTRY * dllIsBuffer )(GLuint buffer);
GLboolean ( APIENTRY * dllIsEnabled )(GLenum cap);
GLboolean ( APIENTRY * dllIsTexture )(GLuint texture);
static void ( APIENTRY * dllLightf )(GLenum light, GLenum pname, GLfloat param);
static void ( APIENTRY * dllLightfv )(GLenum light, GLenum pname, const GLfloat *params);
static void ( APIENTRY * dllLightx )(GLenum light, GLenum pname, GLfixed param);
static void ( APIENTRY * dllLightxv )(GLenum light, GLenum pname, const GLfixed *params);
static void ( APIENTRY * dllLightModelf )(GLenum pname, GLfloat param);
static void ( APIENTRY * dllLightModelfv )(GLenum pname, const GLfloat *params);
static void ( APIENTRY * dllLightModelx )(GLenum pname, GLfixed param);
static void ( APIENTRY * dllLightModelxv )(GLenum pname, const GLfixed *params);
static void ( APIENTRY * dllLineWidth )(GLfloat width);
static void ( APIENTRY * dllLineWidthx )(GLfixed width);
static void ( APIENTRY * dllLoadIdentity )(void);
static void ( APIENTRY * dllLoadMatrixf )(const GLfloat *m);
static void ( APIENTRY * dllLoadMatrixx )(const GLfixed *m);
static void ( APIENTRY * dllLogicOp )(GLenum opcode);
static void ( APIENTRY * dllMaterialf )(GLenum face, GLenum pname, GLfloat param);
static void ( APIENTRY * dllMaterialfv )(GLenum face, GLenum pname, const GLfloat *params);
static void ( APIENTRY * dllMaterialx )(GLenum face, GLenum pname, GLfixed param);
static void ( APIENTRY * dllMaterialxv )(GLenum face, GLenum pname, const GLfixed *params);
static void ( APIENTRY * dllMatrixMode )(GLenum mode);
static void ( APIENTRY * dllMultMatrixf )(const GLfloat *m);
static void ( APIENTRY * dllMultMatrixx )(const GLfixed *m);
static void ( APIENTRY * dllMultiTexCoord4f )(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
static void ( APIENTRY * dllMultiTexCoord4x )(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);
static void ( APIENTRY * dllNormal3f )(GLfloat nx, GLfloat ny, GLfloat nz);
static void ( APIENTRY * dllNormal3x )(GLfixed nx, GLfixed ny, GLfixed nz);
static void ( APIENTRY * dllNormalPointer )(GLenum type, GLsizei stride, const GLvoid *pointer);
static void ( APIENTRY * dllOrthof )(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
static void ( APIENTRY * dllOrthox )(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
static void ( APIENTRY * dllPixelStorei )(GLenum pname, GLint param);
static void ( APIENTRY * dllPointParameterf )(GLenum pname, GLfloat param);
static void ( APIENTRY * dllPointParameterfv )(GLenum pname, const GLfloat *params);
static void ( APIENTRY * dllPointParameterx )(GLenum pname, GLfixed param);
static void ( APIENTRY * dllPointParameterxv )(GLenum pname, const GLfixed *params);
static void ( APIENTRY * dllPointSize )(GLfloat size);
static void ( APIENTRY * dllPointSizex )(GLfixed size);
static void ( APIENTRY * dllPointSizePointerOES )(GLenum type, GLsizei stride, const GLvoid *pointer);
static void ( APIENTRY * dllPolygonOffset )(GLfloat factor, GLfloat units);
static void ( APIENTRY * dllPolygonOffsetx )(GLfixed factor, GLfixed units);
static void ( APIENTRY * dllPopMatrix )(void);
static void ( APIENTRY * dllPushMatrix )(void);
static void ( APIENTRY * dllReadPixels )(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
static void ( APIENTRY * dllRotatef )(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
static void ( APIENTRY * dllRotatex )(GLfixed angle, GLfixed x, GLfixed y, GLfixed z);
static void ( APIENTRY * dllSampleCoverage )(GLclampf value, GLboolean invert);
static void ( APIENTRY * dllSampleCoveragex )(GLclampx value, GLboolean invert);
static void ( APIENTRY * dllScalef )(GLfloat x, GLfloat y, GLfloat z);
static void ( APIENTRY * dllScalex )(GLfixed x, GLfixed y, GLfixed z);
static void ( APIENTRY * dllScissor )(GLint x, GLint y, GLsizei width, GLsizei height);
static void ( APIENTRY * dllShadeModel )(GLenum mode);
static void ( APIENTRY * dllStencilFunc )(GLenum func, GLint ref, GLuint mask);
static void ( APIENTRY * dllStencilMask )(GLuint mask);
static void ( APIENTRY * dllStencilOp )(GLenum fail, GLenum zfail, GLenum zpass);
static void ( APIENTRY * dllTexCoordPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
static void ( APIENTRY * dllTexEnvf )(GLenum target, GLenum pname, GLfloat param);
static void ( APIENTRY * dllTexEnvfv )(GLenum target, GLenum pname, const GLfloat *params);
static void ( APIENTRY * dllTexEnvi )(GLenum target, GLenum pname, GLint param);
static void ( APIENTRY * dllTexEnviv )(GLenum target, GLenum pname, const GLint *params);
static void ( APIENTRY * dllTexEnvx )(GLenum target, GLenum pname, GLfixed param);
static void ( APIENTRY * dllTexEnvxv )(GLenum target, GLenum pname, const GLfixed *params);
static void ( APIENTRY * dllTexImage2D )(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
static void ( APIENTRY * dllTexParameterf )(GLenum target, GLenum pname, GLfloat param);
static void ( APIENTRY * dllTexParameterfv )(GLenum target, GLenum pname, const GLfloat *params);
static void ( APIENTRY * dllTexParameteri )(GLenum target, GLenum pname, GLint param);
static void ( APIENTRY * dllTexParameteriv )(GLenum target, GLenum pname, const GLint *params);
static void ( APIENTRY * dllTexParameterx )(GLenum target, GLenum pname, GLfixed param);
static void ( APIENTRY * dllTexParameterxv )(GLenum target, GLenum pname, const GLfixed *params);
static void ( APIENTRY * dllTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
static void ( APIENTRY * dllTranslatef )(GLfloat x, GLfloat y, GLfloat z);
static void ( APIENTRY * dllTranslatex )(GLfixed x, GLfixed y, GLfixed z);
static void ( APIENTRY * dllVertexPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
static void ( APIENTRY * dllViewport )(GLint x, GLint y, GLsizei width, GLsizei height);

static void APIENTRY logActiveTexture(GLenum texture)
{
    fprintf(glw_state.log_fp, "glActiveTexture( 0x%x )\n", texture);
    dllActiveTexture( texture );
}

static void APIENTRY logAlphaFunc(GLenum func, GLclampf ref)
{
    fprintf( glw_state.log_fp, "glAlphaFunc( 0x%x, %f )\n", func, ref );
    dllAlphaFunc( func, ref );
}

static void APIENTRY logAlphaFuncx(GLenum func, GLclampx ref)
{
    fprintf( glw_state.log_fp, "glAlphaFuncx( 0x%x, %d )\n", func, ref );
    dllAlphaFuncx( func, ref );
}

static void APIENTRY logBindBuffer(GLenum target, GLuint buffer)
{
    fprintf( glw_state.log_fp, "glBindBuffer( 0x%x, %u )\n", target, buffer );
    dllBindBuffer( target, buffer );
}

static void APIENTRY logBindTexture(GLenum target, GLuint texture)
{
    fprintf( glw_state.log_fp, "glBindTexture( 0x%x, %u )\n", target, texture );
    dllBindTexture( target, texture );
}

static void APIENTRY logBlendFunc(GLenum sfactor, GLenum dfactor)
{
    fprintf( glw_state.log_fp, "glBlendFunc( 0x%x, 0x%x )\n", sfactor, dfactor );
    dllBlendFunc( sfactor, dfactor );
}

// XXX: glBufferData, glBufferSubData

static void APIENTRY logClear(GLbitfield mask)
{
    fprintf( glw_state.log_fp, "glClear\n" );
    dllClear( mask );
}

static void APIENTRY logClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    fprintf( glw_state.log_fp, "glClearColor\n" );
    dllClearColor( red, green, blue, alpha );
}

static void APIENTRY logClearColorx(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha)
{
    fprintf( glw_state.log_fp, "glClearColorx\n" );
    dllClearColorx( red, green, blue, alpha );
}

static void APIENTRY logClearDepthf(GLclampf depth)
{
    fprintf( glw_state.log_fp, "glClearDepthf\n" );
    dllClearDepthf( depth );
}

static void APIENTRY logClearDepthx(GLclampx depth)
{
    fprintf( glw_state.log_fp, "glClearDepthx\n" );
    dllClearDepthx( depth );
}

static void APIENTRY logClearStencil(GLint s)
{
    fprintf( glw_state.log_fp, "glClearStencil\n" );
    dllClearStencil( s );
}

static void APIENTRY logClientActiveTexture(GLenum texture)
{
    fprintf(glw_state.log_fp, "glActiveTexture( 0x%x", texture);
    dllActiveTexture(texture);
}

static void APIENTRY logClipPlanef(GLenum plane, const GLfloat *equation)
{
    fprintf( glw_state.log_fp, "glClipPlanef\n" );
    dllClipPlanef( plane, equation );
}

static void APIENTRY logClipPlanex(GLenum plane, const GLfixed *equation)
{
    fprintf( glw_state.log_fp, "glClipPlanex\n" );
    dllClipPlanex( plane, equation );
}

#define SIG( x ) fprintf( glw_state.log_fp, x "\n" )

static void APIENTRY logColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    fprintf( glw_state.log_fp, "glColor4f( %f,%f,%f,%f )\n", red, green, blue, alpha );
    dllColor4f( red, green, blue, alpha );
}

static void APIENTRY logColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
    SIG( "glColor4ub" );
    dllColor4ub( red, green, blue, alpha );
}

static void APIENTRY logColor4x(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)
{
    fprintf( glw_state.log_fp, "glColor4x( %d,%d,%d,%d )\n", red, green, blue, alpha );
    dllColor4x( red, green, blue, alpha );
}

static void APIENTRY logColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
    SIG( "glColorMask" );
    dllColorMask( red, green, blue, alpha );
}

static void APIENTRY logColorPointer(GLint size, GLenum type, GLsizei stride, const void *pointer)
{
    SIG( "glColorPointer" );
    dllColorPointer( size, type, stride, pointer );
}

// XXX: glCompressedTexImage2D && glCompressedTexSubImage2D

static void APIENTRY logCopyTexImage2D(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
    SIG( "glCopyTexImage2D" );
    dllCopyTexImage2D( target, level, internalFormat, x, y, width, height, border );
}

static void APIENTRY logCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
    SIG( "glCopyTexSubImage2D" );
    dllCopyTexSubImage2D( target, level, xoffset, yoffset, x, y, width, height );
}

static void APIENTRY logCullFace(GLenum mode)
{
    SIG( "glCullFace" );
    dllCullFace( mode );
}

static void APIENTRY logDeleteBuffers(GLsizei n, const GLuint *buffers)
{
    SIG( "glDeleteBuffers" );
    dllDeleteBuffers( n, buffers );
}

static void APIENTRY logDeleteTextures(GLsizei n, const GLuint *textures)
{
    SIG( "glDeleteTextures" );
    dllDeleteTextures( n, textures );
}

static void APIENTRY logDepthFunc(GLenum func)
{
    SIG( "glDepthFunc" );
    dllDepthFunc( func );
}

static void APIENTRY logDepthMask(GLboolean flag)
{
    SIG( "glDepthMask" );
    dllDepthMask( flag );
}

static void APIENTRY logDepthRangef(GLclampf zNear, GLclampf zFar)
{
    SIG( "glDepthRangef" );
    dllDepthRangef( zNear, zFar );
}

static void APIENTRY logDepthRangex(GLclampx zNear, GLclampx zFar)
{
    SIG( "glDepthRangex" );
    dllDepthRangex( zNear, zFar );
}

static void APIENTRY logDisable(GLenum cap)
{
    fprintf( glw_state.log_fp, "glDisable( 0x%x )\n", cap );
    dllDisable( cap );
}

static void APIENTRY logDisableClientState(GLenum array)
{
    SIG( "glDisableClientState" );
    dllDisableClientState( array );
}

static void APIENTRY logDrawArrays(GLenum mode, GLint first, GLsizei count)
{
    SIG( "glDrawArrays" );
    dllDrawArrays( mode, first, count );
}

static void APIENTRY logDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices)
{
    SIG( "glDrawElements" );
    dllDrawElements( mode, count, type, indices );
}

static void APIENTRY logEnable(GLenum cap)
{
    fprintf( glw_state.log_fp, "glEnable( 0x%x )\n", cap );
    dllEnable( cap );
}

static void APIENTRY logEnableClientState(GLenum array)
{
    SIG( "glEnableClientState" );
    dllEnableClientState( array );
}

static void APIENTRY logFinish(void)
{
    SIG( "glFinish" );
    dllFinish();
}

static void APIENTRY logFlush(void)
{
    SIG( "glFlush" );
    dllFlush();
}

static void APIENTRY logFogf(GLenum pname, GLfloat param)
{
    SIG( "glFogf" );
    dllFogf( pname, param );
}

static void APIENTRY logFogfv(GLenum pname, const GLfloat *params)
{
    SIG( "glFogfv" );
    dllFogfv( pname, params );
}

static void APIENTRY logFogx(GLenum pname, GLfixed param)
{
    SIG( "glFogx" );
    dllFogx( pname, param );
}

static void APIENTRY logFogxv(GLenum pname, const GLfixed *params)
{
    SIG( "glFogxv" );
    dllFogxv( pname, params );
}

static void APIENTRY logFrontFace(GLenum mode)
{
    SIG( "glFrontFace" );
    dllFrontFace( mode );
}

static void APIENTRY logFrustumf(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
    SIG("glFrustumf");
    dllFrustumf(left, right, bottom, top, zNear, zFar);
}

static void APIENTRY logFrustumx(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
    SIG("glFrustumx");
    dllFrustumx(left, right, bottom, top, zNear, zFar);
}

static void APIENTRY logGenBuffers(GLsizei n, GLuint *buffers)
{
    SIG( "glGenBuffers" );
    dllGenBuffers( n, buffers );
}

static void APIENTRY logGenTextures(GLsizei n, GLuint *textures)
{
    SIG( "glGenTextures" );
    dllGenTextures( n, textures );
}

static void APIENTRY logGetBooleanv(GLenum pname, GLboolean *params)
{
    SIG( "glGetBooleanv" );
    dllGetBooleanv( pname, params );
}

static void APIENTRY logGetFixedv(GLenum pname, GLfixed *params)
{
    SIG( "glGetFixedv" );
    dllGetFixedv( pname, params );
}

static void APIENTRY logGetFloatv(GLenum pname, GLfloat *params)
{
    SIG( "glGetFloatv" );
    dllGetFloatv( pname, params );
}

static void APIENTRY logGetIntegerv(GLenum pname, GLint *params)
{
    SIG( "glGetIntegerv" );
    dllGetIntegerv( pname, params );
}

// XXX: glGetBufferParameteriv

static void APIENTRY logGetClipPlanef(GLenum plane, GLfloat *equation)
{
    SIG( "glGetClipPlanef" );
    dllGetClipPlanef( plane, equation );
}

static void APIENTRY logGetClipPlanex(GLenum plane, GLfixed *equation)
{
    SIG( "glGetClipPlanex" );
    dllGetClipPlanex( plane, equation );
}

static GLenum APIENTRY logGetError(void)
{
    SIG( "glGetError" );
    return dllGetError();
}

static void APIENTRY logGetLightfv(GLenum light, GLenum pname, GLfloat *params)
{
    SIG( "glGetLightfv" );
    dllGetLightfv( light, pname, params );
}

static void APIENTRY logGetLightxv(GLenum light, GLenum pname, GLfixed *params)
{
    SIG( "glGetLightxv" );
    dllGetLightxv( light, pname, params );
}

static void APIENTRY logGetMaterialfv(GLenum face, GLenum pname, GLfloat *params)
{
    SIG( "glGetMaterialfv" );
    dllGetMaterialfv( face, pname, params );
}

static void APIENTRY logGetMaterialxv(GLenum face, GLenum pname, GLfixed *params)
{
    SIG( "glGetMaterialxv" );
    dllGetMaterialxv( face, pname, params );
}

static void APIENTRY logGetPointerv(GLenum pname, GLvoid* *params)
{
    SIG( "glGetPointerv" );
    dllGetPointerv( pname, params );
}

static const GLubyte * APIENTRY logGetString(GLenum name)
{
    SIG( "glGetString" );
    return dllGetString( name );
}

static void APIENTRY logGetTexEnvfv(GLenum target, GLenum pname, GLfloat *params)
{
    SIG( "glGetTexEnvfv" );
    dllGetTexEnvfv( target, pname, params );
}

static void APIENTRY logGetTexEnviv(GLenum target, GLenum pname, GLint *params)
{
    SIG( "glGetTexEnviv" );
    dllGetTexEnviv( target, pname, params );
}

static void APIENTRY logGetTexEnvxv(GLenum target, GLenum pname, GLfixed *params)
{
    SIG( "glGetTexEnvxv" );
    dllGetTexEnvxv( target, pname, params );
}

static void APIENTRY logGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
{
    SIG( "glGetTexParameterfv" );
    dllGetTexParameterfv( target, pname, params );
}

static void APIENTRY logGetTexParameteriv(GLenum target, GLenum pname, GLint *params)
{
    SIG( "glGetTexParameteriv" );
    dllGetTexParameteriv( target, pname, params );
}

static void APIENTRY logGetTexParameterxv(GLenum target, GLenum pname, GLfixed *params)
{
    SIG( "glGetTexParameterxv" );
    dllGetTexParameterxv( target, pname, params );
}

static void APIENTRY logHint(GLenum target, GLenum mode)
{
    fprintf( glw_state.log_fp, "glHint( 0x%x, 0x%x )\n", target, mode );
    dllHint( target, mode );
}

static GLboolean APIENTRY logIsBuffer(GLuint buffer)
{
    SIG( "glIsBuffer" );
    return dllIsBuffer( buffer );
}

static GLboolean APIENTRY logIsEnabled(GLenum cap)
{
    SIG( "glIsEnabled" );
    return dllIsEnabled( cap );
}

static GLboolean APIENTRY logIsTexture(GLuint texture)
{
    SIG( "glIsTexture" );
    return dllIsTexture( texture );
}

static void APIENTRY logLightf(GLenum light, GLenum pname, GLfloat param)
{
    SIG( "glLightf" );
    dllLightf( light, pname, param );
}

static void APIENTRY logLightfv(GLenum light, GLenum pname, const GLfloat *params)
{
    SIG( "glLightfv" );
    dllLightfv( light, pname, params );
}

static void APIENTRY logLightx(GLenum light, GLenum pname, GLfixed param)
{
    SIG( "glLightx" );
    dllLightx( light, pname, param );
}

static void APIENTRY logLightxv(GLenum light, GLenum pname, const GLfixed *params)
{
    SIG( "glLightxv" );
    dllLightxv( light, pname, params );
}

static void APIENTRY logLightModelf(GLenum pname, GLfloat param)
{
    SIG( "glLightModelf" );
    dllLightModelf( pname, param );
}

static void APIENTRY logLightModelfv(GLenum pname, const GLfloat *params)
{
    SIG( "glLightModelfv" );
    dllLightModelfv( pname, params );
}

static void APIENTRY logLightModelx(GLenum pname, GLfixed param)
{
    SIG( "glLightModelx" );
    dllLightModelx( pname, param );
}

static void APIENTRY logLightModelxv(GLenum pname, const GLfixed *params)
{
    SIG( "glLightModelxv" );
    dllLightModelxv( pname, params );
}

static void APIENTRY logLineWidth(GLfloat width)
{
    SIG( "glLineWidth" );
    dllLineWidth( width );
}

static void APIENTRY logLineWidthx(GLfixed width)
{
    SIG( "glLineWidthx" );
    dllLineWidthx( width );
}

static void APIENTRY logLoadIdentity(void)
{
    SIG( "glLoadIdentity" );
    dllLoadIdentity();
}

static void APIENTRY logLoadMatrixf(const GLfloat *m)
{
    SIG( "glLoadMatrixf" );
    dllLoadMatrixf( m );
}

static void APIENTRY logLoadMatrixx(const GLfixed *m)
{
    SIG( "glLoadMatrixx" );
    dllLoadMatrixx( m );
}

static void APIENTRY logLogicOp(GLenum opcode)
{
    SIG( "glLogicOp" );
    dllLogicOp( opcode );
}

static void APIENTRY logMaterialf(GLenum face, GLenum pname, GLfloat param)
{
    SIG( "glMaterialf" );
    dllMaterialf( face, pname, param );
}

static void APIENTRY logMaterialfv(GLenum face, GLenum pname, const GLfloat *params)
{
    SIG( "glMaterialfv" );
    dllMaterialfv( face, pname, params );
}

static void APIENTRY logMaterialx(GLenum face, GLenum pname, GLfixed param)
{
    SIG( "glMaterialx" );
    dllMaterialx( face, pname, param );
}

static void APIENTRY logMaterialxv(GLenum face, GLenum pname, const GLfixed *params)
{
    SIG( "glMaterialxv" );
    dllMaterialxv( face, pname, params );
}

static void APIENTRY logMatrixMode(GLenum mode)
{
    SIG( "glMatrixMode" );
    dllMatrixMode( mode );
}

static void APIENTRY logMultMatrixf(const GLfloat *m)
{
    SIG( "glMultMatrixf" );
    dllMultMatrixf( m );
}

static void APIENTRY logMultMatrixx(const GLfixed *m)
{
    SIG( "glMultMatrixx" );
    dllMultMatrixx( m );
}

static void APIENTRY logMultiTexCoord4f(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
    SIG( "logMultiTexCoord4f" );
    dllMultiTexCoord4f( target, s, t, r, q );
}

static void APIENTRY logMultiTexCoord4x(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q)
{
    SIG( "logMultiTexCoord4x" );
    dllMultiTexCoord4x( target, s, t, r, q );
}

static void APIENTRY logNormal3f(GLfloat nx, GLfloat ny, GLfloat nz)
{
    SIG( "glNormal3f" );
    dllNormal3f( nx, ny, nz );
}

static void APIENTRY logNormal3x(GLfixed nx, GLfixed ny, GLfixed nz)
{
    SIG( "glNormal3x" );
    dllNormal3x( nx, ny, nz );
}

static void APIENTRY logNormalPointer(GLenum type, GLsizei stride, const void *pointer)
{
    SIG( "glNormalPointer" );
    dllNormalPointer( type, stride, pointer );
}

static void APIENTRY logOrthof(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
    SIG("glOrthof");
    dllOrthof(left, right, bottom, top, zNear, zFar);
}

static void APIENTRY logOrthox(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
    SIG("glOrthox");
    dllOrthox(left, right, bottom, top, zNear, zFar);
}

static void APIENTRY logPixelStorei(GLenum pname, GLint param)
{
    SIG( "glPixelStorei" );
    dllPixelStorei( pname, param );
}

static void APIENTRY logPointSize(GLfloat size)
{
    SIG( "glPointSize" );
    dllPointSize( size );
}

static void APIENTRY logPointSizex(GLfixed size)
{
    SIG( "glPointSizex" );
    dllPointSizex( size );
}

static void APIENTRY logPointSizePointerOES(GLenum type, GLsizei stride, const GLvoid *pointer)
{
    SIG( "glPointSizePointerOES" );
    dllPointSizePointerOES( type, stride, pointer );
}

static void APIENTRY logPolygonOffset(GLfloat factor, GLfloat units)
{
    SIG( "glPolygonOffset" );
    dllPolygonOffset( factor, units );
}


static void APIENTRY logPolygonOffsetx(GLfixed factor, GLfixed units)
{
    SIG( "glPolygonOffsetx" );
    dllPolygonOffsetx( factor, units );
}

static void APIENTRY logPopMatrix(void)
{
    SIG( "glPopMatrix" );
    dllPopMatrix();
}

static void APIENTRY logPushMatrix(void)
{
    SIG( "glPushMatrix" );
    dllPushMatrix();
}

static void APIENTRY logReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels)
{
    SIG( "glReadPixels" );
    dllReadPixels( x, y, width, height, format, type, pixels );
}

static void APIENTRY logRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    SIG( "glRotatef" );
    dllRotatef( angle, x, y, z );
}

static void APIENTRY logRotatex(GLfixed angle, GLfixed x, GLfixed y, GLfixed z)
{
    SIG( "glRotatex" );
    dllRotatex( angle, x, y, z );
}

// XXX: glSampleCoverage && glSampleCoveragex

static void APIENTRY logScalef(GLfloat x, GLfloat y, GLfloat z)
{
    SIG( "glScalef" );
    dllScalef( x, y, z );
}

static void APIENTRY logScalex(GLfixed x, GLfixed y, GLfixed z)
{
    SIG( "glScalex" );
    dllScalex( x, y, z );
}

static void APIENTRY logScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
    SIG( "glScissor" );
    dllScissor( x, y, width, height );
}

static void APIENTRY logShadeModel(GLenum mode)
{
    SIG( "glShadeModel" );
    dllShadeModel( mode );
}

static void APIENTRY logStencilFunc(GLenum func, GLint ref, GLuint mask)
{
    SIG( "glStencilFunc" );
    dllStencilFunc( func, ref, mask );
}

static void APIENTRY logStencilMask(GLuint mask)
{
    SIG( "glStencilMask" );
    dllStencilMask( mask );
}

static void APIENTRY logStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
    SIG( "glStencilOp" );
    dllStencilOp( fail, zfail, zpass );
}

static void APIENTRY logTexCoordPointer(GLint size, GLenum type, GLsizei stride, const void *pointer)
{
    SIG( "glTexCoordPointer" );
    dllTexCoordPointer( size, type, stride, pointer );
}

static void APIENTRY logTexEnvf(GLenum target, GLenum pname, GLfloat param)
{
    fprintf( glw_state.log_fp, "glTexEnvf( 0x%x, 0x%x, %f )\n", target, pname, param );
    dllTexEnvf( target, pname, param );
}

static void APIENTRY logTexEnvfv(GLenum target, GLenum pname, const GLfloat *params)
{
    SIG( "glTexEnvfv" );
    dllTexEnvfv( target, pname, params );
}

static void APIENTRY logTexEnvi(GLenum target, GLenum pname, GLint param)
{
    fprintf( glw_state.log_fp, "glTexEnvi( 0x%x, 0x%x, 0x%x )\n", target, pname, param );
    dllTexEnvi( target, pname, param );
}

static void APIENTRY logTexEnviv(GLenum target, GLenum pname, const GLint *params)
{
    SIG( "glTexEnviv" );
    dllTexEnviv( target, pname, params );
}

static void APIENTRY logTexEnvx(GLenum target, GLenum pname, GLfixed param)
{
    fprintf( glw_state.log_fp, "glTexEnvx( 0x%x, 0x%x, %d )\n", target, pname, param );
    dllTexEnvx( target, pname, param );
}

static void APIENTRY logTexEnvxv(GLenum target, GLenum pname, const GLfixed *params)
{
    SIG( "glTexEnvxv" );
    dllTexEnvxv( target, pname, params );
}

static void APIENTRY logTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
{
    SIG( "glTexImage2D" );
    dllTexImage2D( target, level, internalformat, width, height, border, format, type, pixels );
}

static void APIENTRY logTexParameterf(GLenum target, GLenum pname, GLfloat param)
{
    fprintf( glw_state.log_fp, "glTexParameterf( 0x%x, 0x%x, %f )\n", target, pname, param );
    dllTexParameterf( target, pname, param );
}

static void APIENTRY logTexParameterfv(GLenum target, GLenum pname, const GLfloat *params)
{
    SIG( "glTexParameterfv" );
    dllTexParameterfv( target, pname, params );
}

static void APIENTRY logTexParameteri(GLenum target, GLenum pname, GLint param)
{
    fprintf( glw_state.log_fp, "glTexParameteri( 0x%x, 0x%x, 0x%x )\n", target, pname, param );
    dllTexParameteri( target, pname, param );
}

static void APIENTRY logTexParameteriv(GLenum target, GLenum pname, const GLint *params)
{
    SIG( "glTexParameteriv" );
    dllTexParameteriv( target, pname, params );
}


static void APIENTRY logTexParameterx(GLenum target, GLenum pname, GLfixed param)
{
    fprintf( glw_state.log_fp, "glTexParameterx( 0x%x, 0x%x, %d )\n", target, pname, param );
    dllTexParameterx( target, pname, param );
}

static void APIENTRY logTexParameterxv(GLenum target, GLenum pname, const GLfixed *params)
{
    SIG( "glTexParameterxv" );
    dllTexParameterxv( target, pname, params );
}

static void APIENTRY logTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
{
    SIG( "glTexSubImage2D" );
    dllTexSubImage2D( target, level, xoffset, yoffset, width, height, format, type, pixels );
}

static void APIENTRY logTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
    SIG( "glTranslatef" );
    dllTranslatef( x, y, z );
}

static void APIENTRY logTranslatex(GLfixed x, GLfixed y, GLfixed z)
{
    SIG( "glTranslatex" );
    dllTranslatex( x, y, z );
}

static void APIENTRY logVertexPointer(GLint size, GLenum type, GLsizei stride, const void *pointer)
{
    SIG( "glVertexPointer" );
    dllVertexPointer( size, type, stride, pointer );
}

static void APIENTRY logViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    SIG( "glViewport" );
    dllViewport( x, y, width, height );
}

/*
** QGL_Shutdown
**
** Unloads the specified DLL then nulls out all the proc pointers.
*/
void QGL_Shutdown( void )
{
    if ( glw_state.hinstOpenGL )
    {
        FreeLibrary( glw_state.hinstOpenGL );
        glw_state.hinstOpenGL = NULL;
    }

    glw_state.hinstOpenGL = NULL;

    qglActiveTexture             = NULL;
    qglAlphaFunc                 = NULL;
    qglAlphaFuncx                = NULL;
    qglBindBuffer                = NULL;
    qglBindTexture               = NULL;
    qglBlendFunc                 = NULL;
    qglBufferData                = NULL;
    qglBufferSubData             = NULL;
    qglClear                     = NULL;
    qglClearColor                = NULL;
    qglClearColorx               = NULL;
    qglClearDepthf               = NULL;
    qglClearDepthx               = NULL;
    qglClearStencil              = NULL;
    qglClientActiveTexture       = NULL;
    qglClipPlanef                = NULL;
    qglClipPlanex                = NULL;
    qglColor4f                   = NULL;
    qglColor4ub                  = NULL;
    qglColor4x                   = NULL;
    qglColorMask                 = NULL;
    qglColorPointer              = NULL;
    qglCompressedTexImage2D      = NULL;
    qglCompressedTexSubImage2D   = NULL;
    qglCopyTexImage2D            = NULL;
    qglCopyTexSubImage2D         = NULL;
    qglCullFace                  = NULL;
    qglDeleteBuffers             = NULL;
    qglDeleteTextures            = NULL;
    qglDepthFunc                 = NULL;
    qglDepthMask                 = NULL;
    qglDepthRangef               = NULL;
    qglDepthRangex               = NULL;
    qglDisable                   = NULL;
    qglDisableClientState        = NULL;
    qglDrawArrays                = NULL;
    qglDrawElements              = NULL;
    qglEnable                    = NULL;
    qglEnableClientState         = NULL;
    qglFinish                    = NULL;
    qglFlush                     = NULL;
    qglFogf                      = NULL;
    qglFogfv                     = NULL;
    qglFogx                      = NULL;
    qglFogxv                     = NULL;
    qglFrontFace                 = NULL;
    qglFrustumf                  = NULL;
    qglFrustumx                  = NULL;
    qglGenBuffers                = NULL;
    qglGenTextures               = NULL;
    qglGetBooleanv               = NULL;
    qglGetFixedv                 = NULL;
    qglGetFloatv                 = NULL;
    qglGetIntegerv               = NULL;
    qglGetBufferParameteriv      = NULL;
    qglGetClipPlanef             = NULL;
    qglGetClipPlanex             = NULL;
    qglGetError                  = NULL;
    qglGetLightfv                = NULL;
    qglGetLightxv                = NULL;
    qglGetMaterialfv             = NULL;
    qglGetMaterialxv             = NULL;
    qglGetPointerv               = NULL;
    qglGetString                 = NULL;
    qglGetTexEnvfv               = NULL;
    qglGetTexEnviv               = NULL;
    qglGetTexEnvxv               = NULL;
    qglGetTexParameterfv         = NULL;
    qglGetTexParameteriv         = NULL;
    qglGetTexParameterxv         = NULL;
    qglHint                      = NULL;
    qglIsBuffer                  = NULL;
    qglIsEnabled                 = NULL;
    qglIsTexture                 = NULL;
    qglLightf                    = NULL;
    qglLightfv                   = NULL;
    qglLightx                    = NULL;
    qglLightxv                   = NULL;
    qglLightModelf               = NULL;
    qglLightModelfv              = NULL;
    qglLightModelx               = NULL;
    qglLightModelxv              = NULL;
    qglLineWidth                 = NULL;
    qglLineWidthx                = NULL;
    qglLoadIdentity              = NULL;
    qglLoadMatrixf               = NULL;
    qglLoadMatrixx               = NULL;
    qglLogicOp                   = NULL;
    qglMaterialf                 = NULL;
    qglMaterialfv                = NULL;
    qglMaterialx                 = NULL;
    qglMaterialxv                = NULL;
    qglMatrixMode                = NULL;
    qglMultMatrixf               = NULL;
    qglMultMatrixx               = NULL;
    qglMultiTexCoord4f           = NULL;
    qglMultiTexCoord4x           = NULL;
    qglNormal3f                  = NULL;
    qglNormal3x                  = NULL;
    qglNormalPointer             = NULL;
    qglOrthof                    = NULL;
    qglOrthox                    = NULL;
    qglPixelStorei               = NULL;
    qglPointParameterf           = NULL;
    qglPointParameterx           = NULL;
    qglPointSize                 = NULL;
    qglPointSizex                = NULL;
    qglPointSizePointerOES       = NULL;
    qglPolygonOffset             = NULL;
    qglPolygonOffsetx            = NULL;
    qglPopMatrix                 = NULL;
    qglPushMatrix                = NULL;
    qglReadPixels                = NULL;
    qglRotatef                   = NULL;
    qglRotatex                   = NULL;
    qglSampleCoverage            = NULL;
    qglSampleCoveragex           = NULL;
    qglScalef                    = NULL;
    qglScalex                    = NULL;
    qglScissor                   = NULL;
    qglShadeModel                = NULL;
    qglStencilFunc               = NULL;
    qglStencilMask               = NULL;
    qglStencilOp                 = NULL;
    qglTexCoordPointer           = NULL;
    qglTexEnvf                   = NULL;
    qglTexEnvfv                  = NULL;
    qglTexEnvi                   = NULL;
    qglTexEnviv                  = NULL;
    qglTexEnvx                   = NULL;
    qglTexEnvxv                  = NULL;
    qglTexImage2D                = NULL;
    qglTexParameterf             = NULL;
    qglTexParameterfv            = NULL;
    qglTexParameteri             = NULL;
    qglTexParameteriv            = NULL;
    qglTexParameterx             = NULL;
    qglTexParameterxv            = NULL;
    qglTexSubImage2D             = NULL;
    qglTranslatef                = NULL;
    qglTranslatex                = NULL;
    qglVertexPointer             = NULL;
    qglViewport                  = NULL;

    qwglCopyContext              = NULL;
    qwglCreateContext            = NULL;
    qwglCreateLayerContext       = NULL;
    qwglDeleteContext            = NULL;
    qwglDescribeLayerPlane       = NULL;
    qwglGetCurrentContext        = NULL;
    qwglGetCurrentDC             = NULL;
    qwglGetLayerPaletteEntries   = NULL;
    qwglGetProcAddress           = NULL;
    qwglMakeCurrent              = NULL;
    qwglRealizeLayerPalette      = NULL;
    qwglSetLayerPaletteEntries   = NULL;
    qwglShareLists               = NULL;
    qwglSwapLayerBuffers         = NULL;
    qwglUseFontBitmaps           = NULL;
    qwglUseFontOutlines          = NULL;

    qwglChoosePixelFormat        = NULL;
    qwglDescribePixelFormat      = NULL;
    qwglGetPixelFormat           = NULL;
    qwglSetPixelFormat           = NULL;
    qwglSwapBuffers              = NULL;

    qwglSwapIntervalEXT = NULL;

    qwglGetDeviceGammaRampEXT = NULL;
    qwglSetDeviceGammaRampEXT = NULL;
}

#   pragma warning (disable : 4113 4133 4047 )
#   define GPA( a ) GetProcAddress( glw_state.hinstOpenGL, a )

/*
** QGL_Init
**
** This is responsible for binding our qgl function pointers to
** the appropriate GL stuff.  In Windows this means doing a
** LoadLibrary and a bunch of calls to GetProcAddress.  On other
** operating systems we need to do the right thing, whatever that
** might be.
**
*/
qboolean QGL_Init( const char *dllname )
{
    // update 3Dfx gamma irrespective of underlying DLL
    {
        char envbuffer[1024];
        float g;

        g = 2.00 * ( 0.8 - ( vid_gamma->value - 0.5 ) ) + 1.0F;
        Com_sprintf( envbuffer, sizeof(envbuffer), "SSTV2_GAMMA=%f", g );
        putenv( envbuffer );
        Com_sprintf( envbuffer, sizeof(envbuffer), "SST_GAMMA=%f", g );
        putenv( envbuffer );
    }

    if ( ( glw_state.hinstOpenGL = LoadLibrary( dllname ) ) == 0 )
    {
        char *buf = NULL;

        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &buf, 0, NULL);
        ri.Con_Printf( PRINT_ALL, "%s\n", buf );
        return false;
    }

    gl_config.allow_cds = true;

    qglActiveTexture             =  dllActiveTexture             = GPA( "glActiveTexture" );
    qglAlphaFunc                 =  dllAlphaFunc                 = GPA( "glAlphaFunc" );
    qglAlphaFuncx                =  dllAlphaFuncx                = GPA( "glAlphaFuncx" );
    qglBindBuffer                =  dllBindBuffer                = GPA( "glBindBuffer" );
    qglBindTexture               =  dllBindTexture               = GPA( "glBindTexture" );
    qglBlendFunc                 =  dllBlendFunc                 = GPA( "glBlendFunc" );
    qglBufferData                =  dllBufferData                = GPA( "glBufferData" );
    qglBufferSubData             =  dllBufferSubData             = GPA( "glBufferSubData" );
    qglClear                     =  dllClear                     = GPA( "glClear" );
    qglClearColor                =  dllClearColor                = GPA( "glClearColor" );
    qglClearColorx               =  dllClearColorx               = GPA( "glClearColorx" );
    qglClearDepthf               =  dllClearDepthf               = GPA( "glClearDepthf" );
    qglClearDepthx               =  dllClearDepthx               = GPA( "glClearDepthx" );
    qglClearStencil              =  dllClearStencil              = GPA( "glClearStencil" );
    qglClientActiveTexture       =  dllClientActiveTexture       = GPA( "glClientActiveTexture" );
    qglClipPlanef                =  dllClipPlanef                = GPA( "glClipPlanef" );
    qglClipPlanex                =  dllClipPlanex                = GPA( "glClipPlanex" );
    qglColor4f                   =  dllColor4f                   = GPA( "glColor4f" );
    qglColor4ub                  =  dllColor4ub                  = GPA( "glColor4ub" );
    qglColor4x                   =  dllColor4x                   = GPA( "glColor4x" );
    qglColorMask                 =  dllColorMask                 = GPA( "glColorMask" );
    qglColorPointer              =  dllColorPointer              = GPA( "glColorPointer" );
    qglCompressedTexImage2D      =  dllCompressedTexImage2D      = GPA( "glCompressedTexImage2D" );
    qglCompressedTexSubImage2D   =  dllCompressedTexSubImage2D   = GPA( "glCompressedTexSubImage2D" );
    qglCopyTexImage2D            =  dllCopyTexImage2D            = GPA( "glCopyTexImage2D" );
    qglCopyTexSubImage2D         =  dllCopyTexSubImage2D         = GPA( "glCopyTexSubImage2D" );
    qglCullFace                  =  dllCullFace                  = GPA( "glCullFace" );
    qglDeleteBuffers             =  dllDeleteBuffers             = GPA( "glDeleteBuffers" );
    qglDeleteTextures            =  dllDeleteTextures            = GPA( "glDeleteTextures" );
    qglDepthFunc                 =  dllDepthFunc                 = GPA( "glDepthFunc" );
    qglDepthMask                 =  dllDepthMask                 = GPA( "glDepthMask" );
    qglDepthRangef               =  dllDepthRangef               = GPA( "glDepthRangef" );
    qglDepthRangex               =  dllDepthRangex               = GPA( "glDepthRangex" );
    qglDisable                   =  dllDisable                   = GPA( "glDisable" );
    qglDisableClientState        =  dllDisableClientState        = GPA( "glDisableClientState" );
    qglDrawArrays                =  dllDrawArrays                = GPA( "glDrawArrays" );
    qglDrawElements              =  dllDrawElements              = GPA( "glDrawElements" );
    qglEnable                    =  dllEnable                    = GPA( "glEnable" );
    qglEnableClientState         =  dllEnableClientState         = GPA( "glEnableClientState" );
    qglFinish                    =  dllFinish                    = GPA( "glFinish" );
    qglFlush                     =  dllFlush                     = GPA( "glFlush" );
    qglFogf                      =  dllFogf                      = GPA( "glFogf" );
    qglFogfv                     =  dllFogfv                     = GPA( "glFogfv" );
    qglFogx                      =  dllFogx                      = GPA( "glFogx" );
    qglFogxv                     =  dllFogxv                     = GPA( "glFogxv" );
    qglFrontFace                 =  dllFrontFace                 = GPA( "glFrontFace" );
    qglFrustumf                  =  dllFrustumf                  = GPA( "glFrustumf" );
    qglFrustumx                  =  dllFrustumx                  = GPA( "glFrustumx" );
    qglGenBuffers                =  dllGenBuffers                = GPA( "glGenBuffers" );
    qglGenTextures               =  dllGenTextures               = GPA( "glGenTextures" );
    qglGetBooleanv               =  dllGetBooleanv               = GPA( "glGetBooleanv" );
    qglGetFixedv                 =  dllGetFixedv                 = GPA( "glGetFixedv" );
    qglGetFloatv                 =  dllGetFloatv                 = GPA( "glGetFloatv" );
    qglGetIntegerv               =  dllGetIntegerv               = GPA( "glGetIntegerv" );
    qglGetBufferParameteriv      =  dllGetBufferParameteriv      = GPA( "glGetBufferParameteriv" );
    qglGetClipPlanef             =  dllGetClipPlanef             = GPA( "glGetClipPlanef" );
    qglGetClipPlanex             =  dllGetClipPlanex             = GPA( "glGetClipPlanex" );
    qglGetError                  =  dllGetError                  = GPA( "glGetError" );
    qglGetLightfv                =  dllGetLightfv                = GPA( "glGetLightfv" );
    qglGetLightxv                =  dllGetLightxv                = GPA( "glGetLightxv" );
    qglGetMaterialfv             =  dllGetMaterialfv             = GPA( "glGetMaterialfv" );
    qglGetMaterialxv             =  dllGetMaterialxv             = GPA( "glGetMaterialxv" );
    qglGetPointerv               =  dllGetPointerv               = GPA( "glGetPointerv" );
    qglGetString                 =  dllGetString                 = GPA( "glGetString" );
    qglGetTexEnvfv               =  dllGetTexEnvfv               = GPA( "glGetTexEnvfv" );
    qglGetTexEnviv               =  dllGetTexEnviv               = GPA( "glGetTexEnviv" );
    qglGetTexEnvxv               =  dllGetTexEnvxv               = GPA( "glGetTexEnvxv" );
    qglGetTexParameterfv         =  dllGetTexParameterfv         = GPA( "glGetTexParameterfv" );
    qglGetTexParameteriv         =  dllGetTexParameteriv         = GPA( "glGetTexParameteriv" );
    qglGetTexParameterxv         =  dllGetTexParameterxv         = GPA( "glGetTexParameterxv" );
    qglHint                      =  dllHint                      = GPA( "glHint" );
    qglIsBuffer                  =  dllIsBuffer                  = GPA( "glIsBuffer" );
    qglIsEnabled                 =  dllIsEnabled                 = GPA( "glIsEnabled" );
    qglIsTexture                 =  dllIsTexture                 = GPA( "glIsTexture" );
    qglLightf                    =  dllLightf                    = GPA( "glLightf" );
    qglLightfv                   =  dllLightfv                   = GPA( "glLightfv" );
    qglLightx                    =  dllLightx                    = GPA( "glLightx" );
    qglLightxv                   =  dllLightxv                   = GPA( "glLightxv" );
    qglLightModelf               =  dllLightModelf               = GPA( "glLightModelf" );
    qglLightModelfv              =  dllLightModelfv              = GPA( "glLightModelfv" );
    qglLightModelx               =  dllLightModelx               = GPA( "glLightModelx" );
    qglLightModelxv              =  dllLightModelxv              = GPA( "glLightModelxv" );
    qglLineWidth                 =  dllLineWidth                 = GPA( "glLineWidth" );
    qglLineWidthx                =  dllLineWidthx                = GPA( "glLineWidthx" );
    qglLoadIdentity              =  dllLoadIdentity              = GPA( "glLoadIdentity" );
    qglLoadMatrixf               =  dllLoadMatrixf               = GPA( "glLoadMatrixf" );
    qglLoadMatrixx               =  dllLoadMatrixx               = GPA( "glLoadMatrixx" );
    qglLogicOp                   =  dllLogicOp                   = GPA( "glLogicOp" );
    qglMaterialf                 =  dllMaterialf                 = GPA( "glMaterialf" );
    qglMaterialfv                =  dllMaterialfv                = GPA( "glMaterialfv" );
    qglMaterialx                 =  dllMaterialx                 = GPA( "glMaterialx" );
    qglMaterialxv                =  dllMaterialxv                = GPA( "glMaterialxv" );
    qglMatrixMode                =  dllMatrixMode                = GPA( "glMatrixMode" );
    qglMultMatrixf               =  dllMultMatrixf               = GPA( "glMultMatrixf" );
    qglMultMatrixx               =  dllMultMatrixx               = GPA( "glMultMatrixx" );
    qglMultiTexCoord4f           =  dllMultiTexCoord4f           = GPA( "glMultiTexCoord4f" );
    qglMultiTexCoord4x           =  dllMultiTexCoord4x           = GPA( "glMultiTexCoord4x" );
    qglNormal3f                  =  dllNormal3f                  = GPA( "glNormal3f" );
    qglNormal3x                  =  dllNormal3x                  = GPA( "glNormal3x" );
    qglNormalPointer             =  dllNormalPointer             = GPA( "glNormalPointer" );
    qglOrthof                    =  dllOrthof                    = GPA( "glOrthof" );
    qglOrthox                    =  dllOrthox                    = GPA( "glOrthox" );
    qglPixelStorei               =  dllPixelStorei               = GPA( "glPixelStorei" );
    qglPointParameterf           =  dllPointParameterf           = GPA( "glPointParameterf" );
    qglPointParameterfv          =  dllPointParameterfv          = GPA( "glPointParameterfv" );
    qglPointParameterx           =  dllPointParameterx           = GPA( "glPointParameterx" );
    qglPointParameterxv          =  dllPointParameterxv          = GPA( "glPointParameterxv" );
    qglPointSize                 =  dllPointSize                 = GPA( "glPointSize" );
    qglPointSizex                =  dllPointSizex                = GPA( "glPointSizex" );
    qglPointSizePointerOES       =  dllPointSizePointerOES       = GPA( "glPointSizePointerOES" );
    qglPolygonOffset             =  dllPolygonOffset             = GPA( "glPolygonOffset" );
    qglPolygonOffsetx            =  dllPolygonOffsetx            = GPA( "glPolygonOffsetx" );
    qglPopMatrix                 =  dllPopMatrix                 = GPA( "glPopMatrix" );
    qglPushMatrix                =  dllPushMatrix                = GPA( "glPushMatrix" );
    qglReadPixels                =  dllReadPixels                = GPA( "glReadPixels" );
    qglRotatef                   =  dllRotatef                   = GPA( "glRotatef" );
    qglRotatex                   =  dllRotatex                   = GPA( "glRotatex" );
    qglSampleCoverage            =  dllSampleCoverage            = GPA( "glSampleCoverage" );
    qglSampleCoveragex           =  dllSampleCoveragex           = GPA( "glSampleCoveragex" );
    qglScalef                    =  dllScalef                    = GPA( "glScalef" );
    qglScalex                    =  dllScalex                    = GPA( "glScalex" );
    qglScissor                   =  dllScissor                   = GPA( "glScissor" );
    qglShadeModel                =  dllShadeModel                = GPA( "glShadeModel" );
    qglStencilFunc               =  dllStencilFunc               = GPA( "glStencilFunc" );
    qglStencilMask               =  dllStencilMask               = GPA( "glStencilMask" );
    qglStencilOp                 =  dllStencilOp                 = GPA( "glStencilOp" );
    qglTexCoordPointer           =  dllTexCoordPointer           = GPA( "glTexCoordPointer" );
    qglTexEnvf                   =  dllTexEnvf                   = GPA( "glTexEnvf" );
    qglTexEnvfv                  =  dllTexEnvfv                  = GPA( "glTexEnvfv" );
    qglTexEnvi                   =  dllTexEnvi                   = GPA( "glTexEnvi" );
    qglTexEnviv                  =  dllTexEnviv                  = GPA( "glTexEnviv" );
    qglTexEnvx                   =  dllTexEnvx                   = GPA( "glTexEnvx" );
    qglTexEnvxv                  =  dllTexEnvxv                  = GPA( "glTexEnvxv" );
    qglTexImage2D                =  dllTexImage2D                = GPA( "glTexImage2D" );
    qglTexParameterf             =  dllTexParameterf             = GPA( "glTexParameterf" );
    qglTexParameterfv            =  dllTexParameterfv            = GPA( "glTexParameterfv" );
    qglTexParameteri             =  dllTexParameteri             = GPA( "glTexParameteri" );
    qglTexParameteriv            =  dllTexParameteriv            = GPA( "glTexParameteriv" );
    qglTexParameterx             =  dllTexParameterx             = GPA( "glTexParameterx" );
    qglTexParameterxv            =  dllTexParameterxv            = GPA( "glTexParameterxv" );
    qglTexSubImage2D             =  dllTexSubImage2D             = GPA( "glTexSubImage2D" );
    qglTranslatef                =  dllTranslatef                = GPA( "glTranslatef" );
    qglTranslatex                =  dllTranslatex                = GPA( "glTranslatex" );
    qglVertexPointer             =  dllVertexPointer             = GPA( "glVertexPointer" );
    qglViewport                  =  dllViewport                  = GPA( "glViewport" );

    qwglCopyContext              = GPA( "wglCopyContext" );
    qwglCreateContext            = GPA( "wglCreateContext" );
    qwglCreateLayerContext       = GPA( "wglCreateLayerContext" );
    qwglDeleteContext            = GPA( "wglDeleteContext" );
    qwglDescribeLayerPlane       = GPA( "wglDescribeLayerPlane" );
    qwglGetCurrentContext        = GPA( "wglGetCurrentContext" );
    qwglGetCurrentDC             = GPA( "wglGetCurrentDC" );
    qwglGetLayerPaletteEntries   = GPA( "wglGetLayerPaletteEntries" );
    qwglGetProcAddress           = GPA( "wglGetProcAddress" );
    qwglMakeCurrent              = GPA( "wglMakeCurrent" );
    qwglRealizeLayerPalette      = GPA( "wglRealizeLayerPalette" );
    qwglSetLayerPaletteEntries   = GPA( "wglSetLayerPaletteEntries" );
    qwglShareLists               = GPA( "wglShareLists" );
    qwglSwapLayerBuffers         = GPA( "wglSwapLayerBuffers" );
    qwglUseFontBitmaps           = GPA( "wglUseFontBitmapsA" );
    qwglUseFontOutlines          = GPA( "wglUseFontOutlinesA" );

    qwglChoosePixelFormat        = GPA( "wglChoosePixelFormat" );
    qwglDescribePixelFormat      = GPA( "wglDescribePixelFormat" );
    qwglGetPixelFormat           = GPA( "wglGetPixelFormat" );
    qwglSetPixelFormat           = GPA( "wglSetPixelFormat" );
    qwglSwapBuffers              = GPA( "wglSwapBuffers" );

    qwglSwapIntervalEXT = 0;
    qglPointParameterfEXT = 0;
    qglPointParameterfvEXT = 0;
    qglColorTableEXT = 0;
    qglSelectTextureSGIS = 0;
    qglMTexCoord2fSGIS = 0;

    return true;
}

void GLimp_EnableLogging( qboolean enable )
{
    if ( enable )
    {
        if ( !glw_state.log_fp )
        {
            struct tm *newtime;
            time_t aclock;
            char buffer[1024];

            time( &aclock );
            newtime = localtime( &aclock );

            asctime( newtime );

            Com_sprintf( buffer, sizeof(buffer), "%s/gl.log", ri.FS_Gamedir() );
            glw_state.log_fp = fopen( buffer, "wt" );

            fprintf( glw_state.log_fp, "%s\n", asctime( newtime ) );
        }

        qglActiveTexture             =  logActiveTexture             ;
        qglAlphaFunc                 =  logAlphaFunc                 ;
        qglAlphaFuncx                =  logAlphaFuncx                ;
        qglBindBuffer                =  logBindBuffer                ;
        qglBindTexture               =  logBindTexture               ;
        qglBlendFunc                 =  logBlendFunc                 ;
        qglBufferData                =  logBufferData                ;
        qglBufferSubData             =  logBufferSubData             ;
        qglClear                     =  logClear                     ;
        qglClearColor                =  logClearColor                ;
        qglClearColorx               =  logClearColorx               ;
        qglClearDepthf               =  logClearDepthf               ;
        qglClearDepthx               =  logClearDepthx               ;
        qglClearStencil              =  logClearStencil              ;
        qglClientActiveTexture       =  logClientActiveTexture       ;
        qglClipPlanef                =  logClipPlanef                ;
        qglClipPlanex                =  logClipPlanex                ;
        qglColor4f                   =  logColor4f                   ;
        qglColor4ub                  =  logColor4ub                  ;
        qglColor4x                   =  logColor4x                   ;
        qglColorMask                 =  logColorMask                 ;
        qglColorPointer              =  logColorPointer              ;
        qglCompressedTexImage2D      =  logCompressedTexImage2D      ;
        qglCompressedTexSubImage2D   =  logCompressedTexSubImage2D   ;
        qglCopyTexImage2D            =  logCopyTexImage2D            ;
        qglCopyTexSubImage2D         =  logCopyTexSubImage2D         ;
        qglCullFace                  =  logCullFace                  ;
        qglDeleteBuffers             =  logDeleteBuffers             ;
        qglDeleteTextures            =  logDeleteTextures            ;
        qglDepthFunc                 =  logDepthFunc                 ;
        qglDepthMask                 =  logDepthMask                 ;
        qglDepthRangef               =  logDepthRangef               ;
        qglDepthRangex               =  logDepthRangex               ;
        qglDisable                   =  logDisable                   ;
        qglDisableClientState        =  logDisableClientState        ;
        qglDrawArrays                =  logDrawArrays                ;
        qglDrawElements              =  logDrawElements              ;
        qglEnable                    =  logEnable                    ;
        qglEnableClientState         =  logEnableClientState         ;
        qglFinish                    =  logFinish                    ;
        qglFlush                     =  logFlush                     ;
        qglFogf                      =  logFogf                      ;
        qglFogfv                     =  logFogfv                     ;
        qglFogx                      =  logFogx                      ;
        qglFogxv                     =  logFogxv                     ;
        qglFrontFace                 =  logFrontFace                 ;
        qglFrustumf                  =  logFrustumf                  ;
        qglFrustumx                  =  logFrustumx                  ;
        qglGenBuffers                =  logGenBuffers                ;
        qglGenTextures               =  logGenTextures               ;
        qglGetBooleanv               =  logGetBooleanv               ;
        qglGetFixedv                 =  logGetFixedv                 ;
        qglGetFloatv                 =  logGetFloatv                 ;
        qglGetIntegerv               =  logGetIntegerv               ;
        qglGetBufferParameteriv      =  logGetBufferParameteriv      ;
        qglGetClipPlanef             =  logGetClipPlanef             ;
        qglGetClipPlanex             =  logGetClipPlanex             ;
        qglGetError                  =  logGetError                  ;
        qglGetLightfv                =  logGetLightfv                ;
        qglGetLightxv                =  logGetLightxv                ;
        qglGetMaterialfv             =  logGetMaterialfv             ;
        qglGetMaterialxv             =  logGetMaterialxv             ;
        qglGetPointerv               =  logGetPointerv               ;
        qglGetString                 =  logGetString                 ;
        qglGetTexEnvfv               =  logGetTexEnvfv               ;
        qglGetTexEnviv               =  logGetTexEnviv               ;
        qglGetTexEnvxv               =  logGetTexEnvxv               ;
        qglGetTexParameterfv         =  logGetTexParameterfv         ;
        qglGetTexParameteriv         =  logGetTexParameteriv         ;
        qglGetTexParameterxv         =  logGetTexParameterxv         ;
        qglHint                      =  logHint                      ;
        qglIsBuffer                  =  logIsBuffer                  ;
        qglIsEnabled                 =  logIsEnabled                 ;
        qglIsTexture                 =  logIsTexture                 ;
        qglLightf                    =  logLightf                    ;
        qglLightfv                   =  logLightfv                   ;
        qglLightx                    =  logLightx                    ;
        qglLightxv                   =  logLightxv                   ;
        qglLightModelf               =  logLightModelf               ;
        qglLightModelfv              =  logLightModelfv              ;
        qglLightModelx               =  logLightModelx               ;
        qglLightModelxv              =  logLightModelxv              ;
        qglLineWidth                 =  logLineWidth                 ;
        qglLineWidthx                =  logLineWidthx                ;
        qglLoadIdentity              =  logLoadIdentity              ;
        qglLoadMatrixf               =  logLoadMatrixf               ;
        qglLoadMatrixx               =  logLoadMatrixx               ;
        qglLogicOp                   =  logLogicOp                   ;
        qglMaterialf                 =  logMaterialf                 ;
        qglMaterialfv                =  logMaterialfv                ;
        qglMaterialx                 =  logMaterialx                 ;
        qglMaterialxv                =  logMaterialxv                ;
        qglMatrixMode                =  logMatrixMode                ;
        qglMultMatrixf               =  logMultMatrixf               ;
        qglMultMatrixx               =  logMultMatrixx               ;
        qglMultiTexCoord4f           =  logMultiTexCoord4f           ;
        qglMultiTexCoord4x           =  logMultiTexCoord4x           ;
        qglNormal3f                  =  logNormal3f                  ;
        qglNormal3x                  =  logNormal3x                  ;
        qglNormalPointer             =  logNormalPointer             ;
        qglOrthof                    =  logOrthof                    ;
        qglOrthox                    =  logOrthox                    ;
        qglPixelStorei               =  logPixelStorei               ;
        qglPointParameterf           =  logPointParameterf           ;
        qglPointParameterfv          =  logPointParameterfv          ;
        qglPointParameterx           =  logPointParameterx           ;
        qglPointParameterxv          =  logPointParameterxv          ;
        qglPointSize                 =  logPointSize                 ;
        qglPointSizex                =  logPointSizex                ;
        qglPointSizePointerOES       =  logPointSizePointerOES       ;
        qglPolygonOffset             =  logPolygonOffset             ;
        qglPolygonOffsetx            =  logPolygonOffsetx            ;
        qglPopMatrix                 =  logPopMatrix                 ;
        qglPushMatrix                =  logPushMatrix                ;
        qglReadPixels                =  logReadPixels                ;
        qglRotatef                   =  logRotatef                   ;
        qglRotatex                   =  logRotatex                   ;
        qglSampleCoverage            =  logSampleCoverage            ;
        qglSampleCoveragex           =  logSampleCoveragex           ;
        qglScalef                    =  logScalef                    ;
        qglScalex                    =  logScalex                    ;
        qglScissor                   =  logScissor                   ;
        qglShadeModel                =  logShadeModel                ;
        qglStencilFunc               =  logStencilFunc               ;
        qglStencilMask               =  logStencilMask               ;
        qglStencilOp                 =  logStencilOp                 ;
        qglTexCoordPointer           =  logTexCoordPointer           ;
        qglTexEnvf                   =  logTexEnvf                   ;
        qglTexEnvfv                  =  logTexEnvfv                  ;
        qglTexEnvi                   =  logTexEnvi                   ;
        qglTexEnviv                  =  logTexEnviv                  ;
        qglTexEnvx                   =  logTexEnvx                   ;
        qglTexEnvxv                  =  logTexEnvxv                  ;
        qglTexImage2D                =  logTexImage2D                ;
        qglTexParameterf             =  logTexParameterf             ;
        qglTexParameterfv            =  logTexParameterfv            ;
        qglTexParameteri             =  logTexParameteri             ;
        qglTexParameteriv            =  logTexParameteriv            ;
        qglTexParameterx             =  logTexParameterx             ;
        qglTexParameterxv            =  logTexParameterxv            ;
        qglTexSubImage2D             =  logTexSubImage2D             ;
        qglTranslatef                =  logTranslatef                ;
        qglTranslatex                =  logTranslatex                ;
        qglVertexPointer             =  logVertexPointer             ;
        qglViewport                  =  logViewport                  ;
    } else {
        qglActiveTexture             =  dllActiveTexture             ;
        qglAlphaFunc                 =  dllAlphaFunc                 ;
        qglAlphaFuncx                =  dllAlphaFuncx                ;
        qglBindBuffer                =  dllBindBuffer                ;
        qglBindTexture               =  dllBindTexture               ;
        qglBlendFunc                 =  dllBlendFunc                 ;
        qglBufferData                =  dllBufferData                ;
        qglBufferSubData             =  dllBufferSubData             ;
        qglClear                     =  dllClear                     ;
        qglClearColor                =  dllClearColor                ;
        qglClearColorx               =  dllClearColorx               ;
        qglClearDepthf               =  dllClearDepthf               ;
        qglClearDepthx               =  dllClearDepthx               ;
        qglClearStencil              =  dllClearStencil              ;
        qglClientActiveTexture       =  dllClientActiveTexture       ;
        qglClipPlanef                =  dllClipPlanef                ;
        qglClipPlanex                =  dllClipPlanex                ;
        qglColor4f                   =  dllColor4f                   ;
        qglColor4ub                  =  dllColor4ub                  ;
        qglColor4x                   =  dllColor4x                   ;
        qglColorMask                 =  dllColorMask                 ;
        qglColorPointer              =  dllColorPointer              ;
        qglCompressedTexImage2D      =  dllCompressedTexImage2D      ;
        qglCompressedTexSubImage2D   =  dllCompressedTexSubImage2D   ;
        qglCopyTexImage2D            =  dllCopyTexImage2D            ;
        qglCopyTexSubImage2D         =  dllCopyTexSubImage2D         ;
        qglCullFace                  =  dllCullFace                  ;
        qglDeleteBuffers             =  dllDeleteBuffers             ;
        qglDeleteTextures            =  dllDeleteTextures            ;
        qglDepthFunc                 =  dllDepthFunc                 ;
        qglDepthMask                 =  dllDepthMask                 ;
        qglDepthRangef               =  dllDepthRangef               ;
        qglDepthRangex               =  dllDepthRangex               ;
        qglDisable                   =  dllDisable                   ;
        qglDisableClientState        =  dllDisableClientState        ;
        qglDrawArrays                =  dllDrawArrays                ;
        qglDrawElements              =  dllDrawElements              ;
        qglEnable                    =  dllEnable                    ;
        qglEnableClientState         =  dllEnableClientState         ;
        qglFinish                    =  dllFinish                    ;
        qglFlush                     =  dllFlush                     ;
        qglFogf                      =  dllFogf                      ;
        qglFogfv                     =  dllFogfv                     ;
        qglFogx                      =  dllFogx                      ;
        qglFogxv                     =  dllFogxv                     ;
        qglFrontFace                 =  dllFrontFace                 ;
        qglFrustumf                  =  dllFrustumf                  ;
        qglFrustumx                  =  dllFrustumx                  ;
        qglGenBuffers                =  dllGenBuffers                ;
        qglGenTextures               =  dllGenTextures               ;
        qglGetBooleanv               =  dllGetBooleanv               ;
        qglGetFixedv                 =  dllGetFixedv                 ;
        qglGetFloatv                 =  dllGetFloatv                 ;
        qglGetIntegerv               =  dllGetIntegerv               ;
        qglGetBufferParameteriv      =  dllGetBufferParameteriv      ;
        qglGetClipPlanef             =  dllGetClipPlanef             ;
        qglGetClipPlanex             =  dllGetClipPlanex             ;
        qglGetError                  =  dllGetError                  ;
        qglGetLightfv                =  dllGetLightfv                ;
        qglGetLightxv                =  dllGetLightxv                ;
        qglGetMaterialfv             =  dllGetMaterialfv             ;
        qglGetMaterialxv             =  dllGetMaterialxv             ;
        qglGetPointerv               =  dllGetPointerv               ;
        qglGetString                 =  dllGetString                 ;
        qglGetTexEnvfv               =  dllGetTexEnvfv               ;
        qglGetTexEnviv               =  dllGetTexEnviv               ;
        qglGetTexEnvxv               =  dllGetTexEnvxv               ;
        qglGetTexParameterfv         =  dllGetTexParameterfv         ;
        qglGetTexParameteriv         =  dllGetTexParameteriv         ;
        qglGetTexParameterxv         =  dllGetTexParameterxv         ;
        qglHint                      =  dllHint                      ;
        qglIsBuffer                  =  dllIsBuffer                  ;
        qglIsEnabled                 =  dllIsEnabled                 ;
        qglIsTexture                 =  dllIsTexture                 ;
        qglLightf                    =  dllLightf                    ;
        qglLightfv                   =  dllLightfv                   ;
        qglLightx                    =  dllLightx                    ;
        qglLightxv                   =  dllLightxv                   ;
        qglLightModelf               =  dllLightModelf               ;
        qglLightModelfv              =  dllLightModelfv              ;
        qglLightModelx               =  dllLightModelx               ;
        qglLightModelxv              =  dllLightModelxv              ;
        qglLineWidth                 =  dllLineWidth                 ;
        qglLineWidthx                =  dllLineWidthx                ;
        qglLoadIdentity              =  dllLoadIdentity              ;
        qglLoadMatrixf               =  dllLoadMatrixf               ;
        qglLoadMatrixx               =  dllLoadMatrixx               ;
        qglLogicOp                   =  dllLogicOp                   ;
        qglMaterialf                 =  dllMaterialf                 ;
        qglMaterialfv                =  dllMaterialfv                ;
        qglMaterialx                 =  dllMaterialx                 ;
        qglMaterialxv                =  dllMaterialxv                ;
        qglMatrixMode                =  dllMatrixMode                ;
        qglMultMatrixf               =  dllMultMatrixf               ;
        qglMultMatrixx               =  dllMultMatrixx               ;
        qglMultiTexCoord4f           =  dllMultiTexCoord4f           ;
        qglMultiTexCoord4x           =  dllMultiTexCoord4x           ;
        qglNormal3f                  =  dllNormal3f                  ;
        qglNormal3x                  =  dllNormal3x                  ;
        qglNormalPointer             =  dllNormalPointer             ;
        qglOrthof                    =  dllOrthof                    ;
        qglOrthox                    =  dllOrthox                    ;
        qglPixelStorei               =  dllPixelStorei               ;
        qglPointParameterf           =  dllPointParameterf           ;
        qglPointParameterfv          =  dllPointParameterfv          ;
        qglPointParameterx           =  dllPointParameterx           ;
        qglPointParameterxv          =  dllPointParameterxv          ;
        qglPointSize                 =  dllPointSize                 ;
        qglPointSizex                =  dllPointSizex                ;
        qglPointSizePointerOES       =  dllPointSizePointerOES       ;
        qglPolygonOffset             =  dllPolygonOffset             ;
        qglPolygonOffsetx            =  dllPolygonOffsetx            ;
        qglPopMatrix                 =  dllPopMatrix                 ;
        qglPushMatrix                =  dllPushMatrix                ;
        qglReadPixels                =  dllReadPixels                ;
        qglRotatef                   =  dllRotatef                   ;
        qglRotatex                   =  dllRotatex                   ;
        qglSampleCoverage            =  dllSampleCoverage            ;
        qglSampleCoveragex           =  dllSampleCoveragex           ;
        qglScalef                    =  dllScalef                    ;
        qglScalex                    =  dllScalex                    ;
        qglScissor                   =  dllScissor                   ;
        qglShadeModel                =  dllShadeModel                ;
        qglStencilFunc               =  dllStencilFunc               ;
        qglStencilMask               =  dllStencilMask               ;
        qglStencilOp                 =  dllStencilOp                 ;
        qglTexCoordPointer           =  dllTexCoordPointer           ;
        qglTexEnvf                   =  dllTexEnvf                   ;
        qglTexEnvfv                  =  dllTexEnvfv                  ;
        qglTexEnvi                   =  dllTexEnvi                   ;
        qglTexEnviv                  =  dllTexEnviv                  ;
        qglTexEnvx                   =  dllTexEnvx                   ;
        qglTexEnvxv                  =  dllTexEnvxv                  ;
        qglTexImage2D                =  dllTexImage2D                ;
        qglTexParameterf             =  dllTexParameterf             ;
        qglTexParameterfv            =  dllTexParameterfv            ;
        qglTexParameteri             =  dllTexParameteri             ;
        qglTexParameteriv            =  dllTexParameteriv            ;
        qglTexParameterx             =  dllTexParameterx             ;
        qglTexParameterxv            =  dllTexParameterxv            ;
        qglTexSubImage2D             =  dllTexSubImage2D             ;
        qglTranslatef                =  dllTranslatef                ;
        qglTranslatex                =  dllTranslatex                ;
        qglVertexPointer             =  dllVertexPointer             ;
        qglViewport                  =  dllViewport                  ;
    }
}


void GLimp_LogNewFrame( void )
{
    fprintf( glw_state.log_fp, "*** R_BeginFrame ***\n" );
}

#pragma warning (default : 4113 4133 4047 )



