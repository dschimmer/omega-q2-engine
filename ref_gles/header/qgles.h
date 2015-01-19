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
** QGLES.H
*/

#ifndef __QGLES_H__
#define __QGLES_H__

#ifdef WIN32
#include <gl/GL.h>
#include "glext.h"

// hack
typedef int GLclampx;
#else
#include <EGL/egl.h>
#include <GLES/gl.h>
#endif

qboolean QGL_Init( const char *dllname );
void     QGL_Shutdown( void );

#ifndef APIENTRY
#  define APIENTRY
#endif

extern	void ( APIENTRY * qglActiveTexture )(GLenum);
extern  void ( APIENTRY * qglAlphaFunc )(GLenum func, GLclampf ref);
extern  void ( APIENTRY * qglAlphaFuncx )(GLenum func, GLclampx ref);
extern  void ( APIENTRY * qglBindBuffer )(GLenum target, GLuint buffer);
extern  void ( APIENTRY * qglBindTexture )(GLenum target, GLuint texture);
extern  void ( APIENTRY * qglBlendFunc )(GLenum sfactor, GLenum dfactor);
extern  void ( APIENTRY * qglBufferData )(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
extern  void ( APIENTRY * qglBufferSubData )(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);
extern  void ( APIENTRY * qglClear )(GLbitfield mask);
extern  void ( APIENTRY * qglClearColor )(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
extern  void ( APIENTRY * qglClearColorx )(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);
extern  void ( APIENTRY * qglClearDepthf )(GLclampf depth);
extern  void ( APIENTRY * qglClearDepthx )(GLclampx depth);
extern  void ( APIENTRY * qglClearStencil )(GLint s);
extern	void ( APIENTRY * qglClientActiveTexture)(GLenum);
extern  void ( APIENTRY * qglClipPlanef )(GLenum plane, const GLfloat *equation);
extern  void ( APIENTRY * qglClipPlanex )(GLenum plane, const GLfixed *equation);
extern  void ( APIENTRY * qglColor4f )(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern  void ( APIENTRY * qglColor4ub )(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
extern  void ( APIENTRY * qglColor4x )(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);
extern  void ( APIENTRY * qglColorMask )(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
extern  void ( APIENTRY * qglColorPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern  void ( APIENTRY * qglCompressedTexImage2D )(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
extern  void ( APIENTRY * qglCompressedTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
extern  void ( APIENTRY * qglCopyTexImage2D )(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
extern  void ( APIENTRY * qglCopyTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern  void ( APIENTRY * qglCullFace )(GLenum mode);
extern  void ( APIENTRY * qglDeleteBuffers )(GLsizei n, const GLuint *buffers);
extern  void ( APIENTRY * qglDeleteTextures )(GLsizei n, const GLuint *textures);
extern  void ( APIENTRY * qglDepthFunc )(GLenum func);
extern  void ( APIENTRY * qglDepthMask )(GLboolean flag);
extern  void ( APIENTRY * qglDepthRangef )(GLclampf zNear, GLclampf zFar);
extern  void ( APIENTRY * qglDepthRangex )(GLclampx zNear, GLclampx zFar);
extern  void ( APIENTRY * qglDisable )(GLenum cap);
extern  void ( APIENTRY * qglDisableClientState )(GLenum array);
extern  void ( APIENTRY * qglDrawArrays )(GLenum mode, GLint first, GLsizei count);
extern  void ( APIENTRY * qglDrawElements )(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
extern  void ( APIENTRY * qglEnable )(GLenum cap);
extern  void ( APIENTRY * qglEnableClientState )(GLenum array);
extern  void ( APIENTRY * qglFinish )(void);
extern  void ( APIENTRY * qglFlush )(void);
extern  void ( APIENTRY * qglFogf )(GLenum pname, GLfloat param);
extern  void ( APIENTRY * qglFogfv )(GLenum pname, const GLfloat *params);
extern  void ( APIENTRY * qglFogx )(GLenum pname, GLfixed param);
extern  void ( APIENTRY * qglFogxv )(GLenum pname, const GLfixed *params);
extern  void ( APIENTRY * qglFrontFace )(GLenum mode);
extern  void ( APIENTRY * qglFrustumf )(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
extern  void ( APIENTRY * qglFrustumx )(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
extern  void ( APIENTRY * qglGenBuffers )(GLsizei n, GLuint *buffers);
extern  void ( APIENTRY * qglGenTextures )(GLsizei n, GLuint *textures);
extern  void ( APIENTRY * qglGetBooleanv )(GLenum pname, GLboolean *params);
extern  void ( APIENTRY * qglGetFixedv )(GLenum pname, GLfixed *params);
extern  void ( APIENTRY * qglGetFloatv )(GLenum pname, GLfloat *params);
extern  void ( APIENTRY * qglGetIntegerv )(GLenum pname, GLint *params);
extern  void ( APIENTRY * qglGetBufferParameteriv )(GLenum target, GLenum pname, GLint *params);
extern  void ( APIENTRY * qglGetClipPlanef )(GLenum plane, GLfloat *equation);
extern  void ( APIENTRY * qglGetClipPlanex )(GLenum plane, GLfixed *equation);
extern  GLenum ( APIENTRY * qglGetError )(void);
extern  void ( APIENTRY * qglGetLightfv )(GLenum light, GLenum pname, GLfloat *params);
extern  void ( APIENTRY * qglGetLightxv )(GLenum light, GLenum pname, GLfixed *params);
extern  void ( APIENTRY * qglGetMaterialfv )(GLenum face, GLenum pname, GLfloat *params);
extern  void ( APIENTRY * qglGetMaterialxv )(GLenum face, GLenum pname, GLfixed *params);
extern  void ( APIENTRY * qglGetPointerv )(GLenum pname, GLvoid* *params);
extern  const GLubyte * ( APIENTRY * qglGetString )(GLenum name);
extern  void ( APIENTRY * qglGetTexEnvfv )(GLenum target, GLenum pname, GLfloat *params);
extern  void ( APIENTRY * qglGetTexEnviv )(GLenum target, GLenum pname, GLint *params);
extern  void ( APIENTRY * qglGetTexEnvxv )(GLenum target, GLenum pname, GLfixed *params);
extern  void ( APIENTRY * qglGetTexParameterfv )(GLenum target, GLenum pname, GLfloat *params);
extern  void ( APIENTRY * qglGetTexParameteriv )(GLenum target, GLenum pname, GLint *params);
extern  void ( APIENTRY * qglGetTexParameterxv )(GLenum target, GLenum pname, GLfixed *params);
extern  void ( APIENTRY * qglHint )(GLenum target, GLenum mode);
extern  GLboolean ( APIENTRY * qglIsBuffer )(GLuint buffer);
extern  GLboolean ( APIENTRY * qglIsEnabled )(GLenum cap);
extern  GLboolean ( APIENTRY * qglIsTexture )(GLuint texture);
extern  void ( APIENTRY * qglLightf )(GLenum light, GLenum pname, GLfloat param);
extern  void ( APIENTRY * qglLightfv )(GLenum light, GLenum pname, const GLfloat *params);
extern  void ( APIENTRY * qglLightx )(GLenum light, GLenum pname, GLfixed param);
extern  void ( APIENTRY * qglLightxv )(GLenum light, GLenum pname, const GLfixed *params);
extern  void ( APIENTRY * qglLightModelf )(GLenum pname, GLfloat param);
extern  void ( APIENTRY * qglLightModelfv )(GLenum pname, const GLfloat *params);
extern  void ( APIENTRY * qglLightModelx )(GLenum pname, GLfixed param);
extern  void ( APIENTRY * qglLightModelxv )(GLenum pname, const GLfixed *params);
extern  void ( APIENTRY * qglLineWidth )(GLfloat width);
extern  void ( APIENTRY * qglLineWidthx )(GLfixed width);
extern  void ( APIENTRY * qglLoadIdentity )(void);
extern  void ( APIENTRY * qglLoadMatrixf )(const GLfloat *m);
extern  void ( APIENTRY * qglLoadMatrixx )(const GLfixed *m);
extern  void ( APIENTRY * qglLogicOp )(GLenum opcode);
extern  void ( APIENTRY * qglMaterialf )(GLenum face, GLenum pname, GLfloat param);
extern  void ( APIENTRY * qglMaterialfv )(GLenum face, GLenum pname, const GLfloat *params);
extern  void ( APIENTRY * qglMaterialx )(GLenum face, GLenum pname, GLfixed param);
extern  void ( APIENTRY * qglMaterialxv )(GLenum face, GLenum pname, const GLfixed *params);
extern  void ( APIENTRY * qglMatrixMode )(GLenum mode);
extern  void ( APIENTRY * qglMultMatrixf )(const GLfloat *m);
extern  void ( APIENTRY * qglMultMatrixx )(const GLfixed *m);
extern  void ( APIENTRY * qglMultiTexCoord4f )(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
extern  void ( APIENTRY * qglMultiTexCoord4x )(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);
extern  void ( APIENTRY * qglNormal3f )(GLfloat nx, GLfloat ny, GLfloat nz);
extern  void ( APIENTRY * qglNormal3x )(GLfixed nx, GLfixed ny, GLfixed nz); 
extern  void ( APIENTRY * qglNormalPointer )(GLenum type, GLsizei stride, const GLvoid *pointer);
extern  void ( APIENTRY * qglOrthof )(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
extern  void ( APIENTRY * qglOrthox )(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
extern  void ( APIENTRY * qglPixelStorei )(GLenum pname, GLint param);
extern  void ( APIENTRY * qglPointParameterf )(GLenum pname, GLfloat param);
extern  void ( APIENTRY * qglPointParameterfv )(GLenum pname, const GLfloat *params);
extern  void ( APIENTRY * qglPointParameterx )(GLenum pname, GLfixed param);
extern  void ( APIENTRY * qglPointParameterxv )(GLenum pname, const GLfixed *params);
extern  void ( APIENTRY * qglPointSize )(GLfloat size);
extern  void ( APIENTRY * qglPointSizex )(GLfixed size);
extern  void ( APIENTRY * qglPointSizePointerOES )(GLenum type, GLsizei stride, const GLvoid *pointer);
extern  void ( APIENTRY * qglPolygonOffset )(GLfloat factor, GLfloat units);
extern  void ( APIENTRY * qglPolygonOffsetx )(GLfixed, GLfixed units); 
extern  void ( APIENTRY * qglPopMatrix )(void);
extern  void ( APIENTRY * qglPushMatrix )(void);
extern  void ( APIENTRY * qglReadPixels )(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
extern  void ( APIENTRY * qglRotatef )(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
extern  void ( APIENTRY * qglRotatex )(GLfixed angle, GLfixed x, GLfixed y, GLfixed z);
extern  void ( APIENTRY * qglSampleCoverage )(GLclampf value, GLboolean invert);
extern  void ( APIENTRY * qglSampleCoveragex )(GLclampx value, GLboolean invert);
extern  void ( APIENTRY * qglScalef )(GLfloat x, GLfloat y, GLfloat z);
extern  void ( APIENTRY * qglScalex )(GLfixed x, GLfixed y, GLfixed z);
extern  void ( APIENTRY * qglScissor )(GLint x, GLint y, GLsizei width, GLsizei height);
extern  void ( APIENTRY * qglShadeModel )(GLenum mode);
extern  void ( APIENTRY * qglStencilFunc )(GLenum func, GLint ref, GLuint mask);
extern  void ( APIENTRY * qglStencilMask )(GLuint mask);
extern  void ( APIENTRY * qglStencilOp )(GLenum fail, GLenum zfail, GLenum zpass);
extern  void ( APIENTRY * qglTexCoordPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern  void ( APIENTRY * qglTexEnvf )(GLenum target, GLenum pname, GLfloat param);
extern  void ( APIENTRY * qglTexEnvfv )(GLenum target, GLenum pname, const GLfloat *params);
extern  void ( APIENTRY * qglTexEnvi )(GLenum target, GLenum pname, GLint param);
extern  void ( APIENTRY * qglTexEnviv )(GLenum target, GLenum pname, const GLint *params);
extern  void ( APIENTRY * qglTexEnvx )(GLenum target, GLenum pname, GLfixed param);
extern  void ( APIENTRY * qglTexEnvxv )(GLenum target, GLenum pname, const GLfixed *params);
extern  void ( APIENTRY * qglTexImage2D )(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern  void ( APIENTRY * qglTexParameterf )(GLenum target, GLenum pname, GLfloat param);
extern  void ( APIENTRY * qglTexParameterfv )(GLenum target, GLenum pname, const GLfloat *params);
extern  void ( APIENTRY * qglTexParameteri )(GLenum target, GLenum pname, GLint param);
extern  void ( APIENTRY * qglTexParameteriv )(GLenum target, GLenum pname, const GLint *params);
extern  void ( APIENTRY * qglTexParameterx )(GLenum target, GLenum pname, GLfixed param);
extern  void ( APIENTRY * qglTexParameterxv )(GLenum target, GLenum pname, const GLfixed *params);
extern  void ( APIENTRY * qglTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
extern  void ( APIENTRY * qglTranslatef )(GLfloat x, GLfloat y, GLfloat z);
extern  void ( APIENTRY * qglTranslatex )(GLfixed x, GLfixed y, GLfixed z);
extern  void ( APIENTRY * qglVertexPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern  void ( APIENTRY * qglViewport )(GLint x, GLint y, GLsizei width, GLsizei height);

#ifdef _WIN32

extern  int   ( WINAPI * qwglChoosePixelFormat )(HDC, CONST PIXELFORMATDESCRIPTOR *);
extern  int   ( WINAPI * qwglDescribePixelFormat) (HDC, int, UINT, LPPIXELFORMATDESCRIPTOR);
extern  int   ( WINAPI * qwglGetPixelFormat)(HDC);
extern  BOOL  ( WINAPI * qwglSetPixelFormat)(HDC, int, CONST PIXELFORMATDESCRIPTOR *);
extern  BOOL  ( WINAPI * qwglSwapBuffers)(HDC);

extern BOOL  ( WINAPI * qwglCopyContext)(HGLRC, HGLRC, UINT);
extern HGLRC ( WINAPI * qwglCreateContext)(HDC);
extern HGLRC ( WINAPI * qwglCreateLayerContext)(HDC, int);
extern BOOL  ( WINAPI * qwglDeleteContext)(HGLRC);
extern HGLRC ( WINAPI * qwglGetCurrentContext)(VOID);
extern HDC   ( WINAPI * qwglGetCurrentDC)(VOID);
extern PROC  ( WINAPI * qwglGetProcAddress)(LPCSTR);
extern BOOL  ( WINAPI * qwglMakeCurrent)(HDC, HGLRC);
extern BOOL  ( WINAPI * qwglShareLists)(HGLRC, HGLRC);
extern BOOL  ( WINAPI * qwglUseFontBitmaps)(HDC, DWORD, DWORD, DWORD);

extern BOOL  ( WINAPI * qwglUseFontOutlines)(HDC, DWORD, DWORD, DWORD, FLOAT,
                                           FLOAT, int, LPGLYPHMETRICSFLOAT);

extern BOOL ( WINAPI * qwglDescribeLayerPlane)(HDC, int, int, UINT,
                                            LPLAYERPLANEDESCRIPTOR);
extern int  ( WINAPI * qwglSetLayerPaletteEntries)(HDC, int, int, int,
                                                CONST COLORREF *);
extern int  ( WINAPI * qwglGetLayerPaletteEntries)(HDC, int, int, int,
                                                COLORREF *);
extern BOOL ( WINAPI * qwglRealizeLayerPalette)(HDC, int, BOOL);
extern BOOL ( WINAPI * qwglSwapLayerBuffers)(HDC, UINT);

extern BOOL ( WINAPI * qwglSwapIntervalEXT)( int interval );

extern BOOL ( WINAPI * qwglGetDeviceGammaRampEXT ) ( unsigned char *pRed, unsigned char *pGreen, unsigned char *pBlue );
extern BOOL ( WINAPI * qwglSetDeviceGammaRampEXT ) ( const unsigned char *pRed, const unsigned char *pGreen, const unsigned char *pBlue );

#endif

#ifdef __linux__

// local function in dll
extern void *qwglGetProcAddress(char *symbol);

extern void (*qgl3DfxSetPaletteEXT)(GLuint *);

#endif // linux

#endif
