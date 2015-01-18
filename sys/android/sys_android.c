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
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/ipc.h>
//#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <errno.h>
#include <mntent.h>

#include <dlfcn.h>

#include "../../common/common.h"

#include "../android/rw_android.h"

#include <android/log.h>
#include <android_native_app_glue.h>
#include <cpu-features.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "boobs", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "boobs", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "boobs", __VA_ARGS__))

cvar_t *nostdout;

unsigned    sys_frame_time;

uid_t saved_euid;
qboolean stdin_active = true;


ANativeWindow *mainWindow = NULL;

// =======================================================================
// General routines
// =======================================================================

void Sys_ConsoleOutput (char *string)
{
    if (nostdout && nostdout->value)
        return;

    fputs(string, stdout);
}

void Sys_Printf (char *fmt, ...)
{
    va_list     argptr;
    char        text[1024];
    unsigned char       *p;

    va_start (argptr,fmt);
    vsprintf (text,fmt,argptr);
    va_end (argptr);

    if (strlen(text) > sizeof(text))
        Sys_Error("memory overwrite in Sys_Printf");

    if (nostdout && nostdout->value)
        return;

    for (p = (unsigned char *)text; *p; p++) {
        *p &= 0x7f;
        if ((*p > 128 || *p < 32) && *p != 10 && *p != 13 && *p != 9)
            printf("[%02x]", *p);
        else
            putc(*p, stdout);
    }
}

void Sys_Quit (void)
{
    CL_Shutdown();
    Com_Shutdown();
#ifndef ANDROID
    fcntl (0, F_SETFL, fcntl (0, F_GETFL, 0) & ~FNDELAY);
#endif
    _exit(0);
}

void Sys_Init(void)
{
#if id386
//  Sys_SetFPCW();
#endif
}

void Sys_Error (char *error, ...)
{
    va_list     argptr;
    char        string[1024];

// change stdin to non blocking
#ifndef ANDROID
    fcntl (0, F_SETFL, fcntl (0, F_GETFL, 0) & ~FNDELAY);
#endif

    CL_Shutdown();
    Com_Shutdown();

    va_start (argptr,error);
    vsprintf (string,error,argptr);
    va_end (argptr);
    //fprintf(stderr, "Error: %s\n", string);
    LOGE("Error: %s\n", string);

    _exit (1);

}

void Sys_Warn (char *warning, ...)
{
    va_list     argptr;
    char        string[1024];

    va_start (argptr,warning);
    vsprintf (string,warning,argptr);
    va_end (argptr);
    fprintf(stderr, "Warning: %s", string);
}

/*
============
Sys_FileTime

returns -1 if not present
============
*/
int Sys_FileTime (char *path)
{
    struct  stat    buf;

    if (stat (path,&buf) == -1)
        return -1;

    return buf.st_mtime;
}

void floating_point_exception_handler(int whatever)
{
//  Sys_Warn("floating point exception\n");
    signal(SIGFPE, floating_point_exception_handler);
}

char *Sys_ConsoleInput(void)
{
    static char text[256];
    int     len;
    fd_set  fdset;
    struct timeval timeout;

    if (!dedicated || !dedicated->value)
        return NULL;

    if (!stdin_active)
        return NULL;

    FD_ZERO(&fdset);
    FD_SET(0, &fdset); // stdin
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    if (select (1, &fdset, NULL, NULL, &timeout) == -1 || !FD_ISSET(0, &fdset))
        return NULL;

    len = read (0, text, sizeof(text));
    if (len == 0) { // eof!
        stdin_active = false;
        return NULL;
    }

    if (len < 1)
        return NULL;
    text[len-1] = 0;    // rip off the /n and terminate

    return text;
}

/*****************************************************************************/

static void *game_library;

/*
=================
Sys_UnloadGame
=================
*/
void Sys_UnloadGame (void)
{
    if (game_library)
        dlclose (game_library);
    game_library = NULL;
}

/*
=================
Sys_GetGameAPI

Loads the game dll
=================
*/
//#include "../../game/game.h"
void *Sys_GetGameAPI (void *parms)
{

    //extern game_export_t *GetGameAPI (game_import_t *import);

    void    *(*GetGameAPI) (void *);

    char    name[MAX_OSPATH];
    char    curpath[MAX_OSPATH];
    char    *path;
#ifdef __i386__
    const char *gamename = "gamei386.so";
#elif defined __alpha__
    const char *gamename = "gameaxp.so";
#elif defined ANDROID
  const char *gamename = "data/data/com.example.quake2/lib/libgame_base.so";
#else
#error Unknown arch
#endif

    setreuid(getuid(), getuid());
    setegid(getgid());

    if (game_library) {
        Com_Error (ERR_FATAL, "Sys_GetGameAPI without Sys_UnloadingGame");
        return NULL;
    }

    getcwd(curpath, sizeof(curpath));

    Com_Printf("------- Loading %s -------\n", gamename);
#if 0
    // now run through the search paths
    path = NULL;
    while (1)
    {
        path = FS_NextPath (path);
        if (!path) {
            return NULL;        // couldn't find one anywhere
        }

        sprintf (name, "%s/%s/%s", curpath, path, gamename);
        game_library = dlopen (name, RTLD_LAZY );
        if (game_library) {
            Com_Printf ("LoadLibrary (%s)\n",name);
            break;
        }
    }
#else
    game_library = dlopen(gamename, RTLD_LAZY);
#endif

    if (game_library) {
        GetGameAPI = (void *)dlsym (game_library, "GetGameAPI");
    }

    if (!GetGameAPI) {
        Sys_UnloadGame();
        return NULL;
    }

    return GetGameAPI(parms);
//  return (void *) GetGameAPI((game_import_t *)parms);
}

/*****************************************************************************/

void Sys_AppActivate (void)
{
}

void Sys_SendKeyEvents (void)
{
#ifndef DEDICATED_ONLY
    if (KBD_Update_fp)
        KBD_Update_fp();
#endif

    // grab frame time
    sys_frame_time = Sys_Milliseconds();
}

/*****************************************************************************/

char *Sys_GetClipboardData(void)
{
    return NULL;
}

void Sys_CopyProtect(void)
{
#if 0
    FILE *mnt;
    struct mntent *ent;
    char path[MAX_OSPATH];
    struct stat st;
    qboolean found_cd = false;

    static qboolean checked = false;

    if (checked)
        return;

    if ((mnt = setmntent("/etc/mtab", "r")) == NULL)
        Com_Error(ERR_FATAL, "Can't read mount table to determine mounted cd location.");

    while ((ent = getmntent(mnt)) != NULL) {
        if (strcmp(ent->mnt_type, "iso9660") == 0) {
            // found a cd file system
            found_cd = true;
            sprintf(path, "%s/%s", ent->mnt_dir, "install/data/quake2.exe");
            if (stat(path, &st) == 0) {
                // found it
                checked = true;
                endmntent(mnt);
                return;
            }
            sprintf(path, "%s/%s", ent->mnt_dir, "Install/Data/quake2.exe");
            if (stat(path, &st) == 0) {
                // found it
                checked = true;
                endmntent(mnt);
                return;
            }
            sprintf(path, "%s/%s", ent->mnt_dir, "quake2.exe");
            if (stat(path, &st) == 0) {
                // found it
                checked = true;
                endmntent(mnt);
                return;
            }
        }
    }
    endmntent(mnt);

    if (found_cd)
        Com_Error (ERR_FATAL, "Could not find a Quake2 CD in your CD drive.");
    Com_Error (ERR_FATAL, "Unable to find a mounted iso9660 file system.\n"
        "You must mount the Quake2 CD in a cdrom drive in order to play.");
#endif
}

#if 0
/*
================
Sys_MakeCodeWriteable
================
*/
void Sys_MakeCodeWriteable (unsigned long startaddr, unsigned long length)
{

    int r;
    unsigned long addr;
    int psize = getpagesize();

    addr = (startaddr & ~(psize-1)) - psize;

//  fprintf(stderr, "writable code %lx(%lx)-%lx, length=%lx\n", startaddr,
//          addr, startaddr+length, length);

    r = mprotect((char*)addr, length + startaddr - addr + psize, 7);

    if (r < 0)
            Sys_Error("Protection change failed\n");

}

#endif











#include "../../client/keys.h"


static int32_t XlateKey(int32_t key) {
    switch (key) {
        case AKEYCODE_0                :
            return '0';
        case AKEYCODE_1                :
            return '1';
        case AKEYCODE_2                :
            return '2';
        case AKEYCODE_3                :
            return '3';
        case AKEYCODE_4                :
            return '4';
        case AKEYCODE_5                :
            return '5';
        case AKEYCODE_6                :
            return '6';
        case AKEYCODE_7                :
            return '7';
        case AKEYCODE_8                :
            return '8';
        case AKEYCODE_9                :
            return '9';
//        case AKEYCODE_STAR             :
//        case AKEYCODE_POUND            :
        case AKEYCODE_DPAD_UP          :
            return K_UPARROW;
        case AKEYCODE_DPAD_DOWN        :
            return K_DOWNARROW;
        case AKEYCODE_DPAD_LEFT        :
            return K_LEFTARROW;
        case AKEYCODE_DPAD_RIGHT       :
            return K_RIGHTARROW;
//        case AKEYCODE_DPAD_CENTER      :
//        case AKEYCODE_VOLUME_UP        :
//        case AKEYCODE_VOLUME_DOWN      :
//        case AKEYCODE_POWER            :
//        case AKEYCODE_CAMERA           :
//        case AKEYCODE_CLEAR            :
        case AKEYCODE_A                :
            return 'a';
        case AKEYCODE_B                :
            return 'b';
        case AKEYCODE_C                :
            return 'c';
        case AKEYCODE_D                :
            return 'd';
        case AKEYCODE_E                :
            return 'e';
        case AKEYCODE_F                :
            return 'f';
        case AKEYCODE_G                :
            return 'g';
        case AKEYCODE_H                :
            return 'h';
        case AKEYCODE_I                :
            return 'i';
        case AKEYCODE_J                :
            return 'j';
        case AKEYCODE_K                :
            return 'k';
        case AKEYCODE_L                :
            return 'l';
        case AKEYCODE_M                :
            return 'm'; 
        case AKEYCODE_N                :
            return 'n';
        case AKEYCODE_O                :
            return 'o';
        case AKEYCODE_P                :
            return 'p';
        case AKEYCODE_Q                :
            return 'q';
        case AKEYCODE_R                :
            return 'r';
        case AKEYCODE_S                :
            return 's';
        case AKEYCODE_T                :
            return 't';
        case AKEYCODE_U                :
            return 'u';
        case AKEYCODE_V                :
            return 'v';
        case AKEYCODE_W                :
            return 'w';
        case AKEYCODE_X                :
            return 'x';
        case AKEYCODE_Y                :
            return 'y';
        case AKEYCODE_Z                :
            return 'z';
        case AKEYCODE_COMMA            :
            return ',';
        case AKEYCODE_PERIOD           :
            return '.';
        case AKEYCODE_ALT_LEFT         :
        case AKEYCODE_ALT_RIGHT        :
            return K_ALT;
        case AKEYCODE_SHIFT_LEFT       :
        case AKEYCODE_SHIFT_RIGHT      :
            return K_SHIFT;
        case AKEYCODE_TAB              :
            return K_TAB;
        case AKEYCODE_SPACE            :
            return K_SPACE;
//        case AKEYCODE_SYM              :
//        case AKEYCODE_EXPLORER         :
//        case AKEYCODE_ENVELOPE         :
        case AKEYCODE_ENTER            :
            return K_ENTER;
        case AKEYCODE_DEL              :
            return K_DEL;
        case AKEYCODE_GRAVE            :
            return '`';
        case AKEYCODE_MINUS            :
            return '-';
        case AKEYCODE_EQUALS           :
            return '=';
        case AKEYCODE_LEFT_BRACKET     :
            return '[';
        case AKEYCODE_RIGHT_BRACKET    :
            return ']';
        case AKEYCODE_BACKSLASH        :
            return '\\';
        case AKEYCODE_SEMICOLON        :
            return ';';
        case AKEYCODE_APOSTROPHE       :
            return '\'';
        case AKEYCODE_SLASH            :
            return '/';
        case AKEYCODE_AT               :
            return '@';
        case AKEYCODE_NUM              :
        case AKEYCODE_HEADSETHOOK      :
        case AKEYCODE_FOCUS            :
            return 0;
        case AKEYCODE_PLUS             :
            return '+';
        case AKEYCODE_MENU             :
            return K_ESCAPE;
//        case AKEYCODE_NOTIFICATION     :
//        case AKEYCODE_SEARCH           :
//        case AKEYCODE_MEDIA_PLAY_PAUSE :
//        case AKEYCODE_MEDIA_STOP       :
//        case AKEYCODE_MEDIA_NEXT       :
//        case AKEYCODE_MEDIA_PREVIOUS   :
//        case AKEYCODE_MEDIA_REWIND     :
//        case AKEYCODE_MEDIA_FAST_FORWARD :
//        case AKEYCODE_MUTE            :
        case AKEYCODE_PAGE_UP         :
            return K_PGUP;
        case AKEYCODE_PAGE_DOWN       :
            return K_PGDN;
//        case AKEYCODE_PICTSYMBOLS     :
//        case AKEYCODE_SWITCH_CHARSET  :
//        case AKEYCODE_BUTTON_A        :
//        case AKEYCODE_BUTTON_B        :
//        case AKEYCODE_BUTTON_C        :
//        case AKEYCODE_BUTTON_X        :
//        case AKEYCODE_BUTTON_Y        :
//        case AKEYCODE_BUTTON_Z        :
//        case AKEYCODE_BUTTON_L1       :
//        case AKEYCODE_BUTTON_R1       :
//        case AKEYCODE_BUTTON_L2       :
//        case AKEYCODE_BUTTON_R2       :
//        case AKEYCODE_BUTTON_THUMBL   :
//        case AKEYCODE_BUTTON_THUMBR   :
//        case AKEYCODE_BUTTON_START    :
//        case AKEYCODE_BUTTON_SELECT   :
//        case AKEYCODE_BUTTON_MODE     :
        case 111: //AKEYCODE_ESCAPE:
            return K_ESCAPE;
        case 113: //AKEYCODE_CTRL_LEFT
            return K_CTRL;
        case 131: //AKEYCODE_F1
            return K_F1;
        case 132: //AKEYCODE_F2
            return K_F2;
        case 133: //AKEYCODE_F3
            return K_F3;
        case 134: //AKEYCODE_F4
            return K_F4;
        case 135: //AKEYCODE_F5
            return K_F5;
        case 136: //AKEYCODE_F6
            return K_F6;
        case 137: //AKEYCODE_F7
            return K_F7;
        case 138: //AKEYCODE_F8
            return K_F8;
        case 139: //AKEYCODE_F9
            return K_F9;
        case 140: //AKEYCODE_F10
            return K_F10;
        case 141: //AKEYCODE_F11
            return K_F11;
        case 145: //AKEYCODE_F12
            return K_F12;
        default:
            Com_Printf("i don't know that key!\n");
            return 0;
    }
}


static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
 //   struct engine* engine = (struct engine*)app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY) {
        int32_t action = AKeyEvent_getAction(event);
        int32_t key = AKeyEvent_getKeyCode(event);
        int32_t xlate = XlateKey(key);
        int32_t flags = AKeyEvent_getFlags(event);
        int32_t repeat = AKeyEvent_getRepeatCount(event);
//        engine->animating = 1;
//        engine->state.x = AMotionEvent_getX(event, 0);
//        engine->state.y = AMotionEvent_getY(event, 0);
        Com_Printf("Key pressed, %d %d %d %x %d\n", action, key, xlate, flags, repeat);
        Key_Event(xlate, action == AKEY_EVENT_ACTION_DOWN, Sys_Milliseconds());
        return 1;
    }
    return 0;
}




static qboolean engineReady = false;


static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    //struct engine* engine = (struct engine*)app->userData;
    Com_Printf("engine_handle_cmd %d", cmd);

    switch (cmd) {
        case APP_CMD_SAVE_STATE:
#if 0
            // The system has asked us to save our current state.  Do so.
            engine->app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state*)engine->app->savedState) = engine->state;
            engine->app->savedStateSize = sizeof(struct saved_state);
#endif
            break;
        case APP_CMD_INIT_WINDOW:
            Com_Printf( "APP_CMD_INIT_WINDOW: Window: %p", app->window);
            Com_Printf( "App Dirs: %s, %s", app->activity->internalDataPath, app->activity->externalDataPath);
            mainWindow = app->window;
            Com_Init();
            engineReady = true;
#if 0
            // The window is being shown, get it ready.
            if (engine->app->window != NULL) {
                engine_init_display(engine);
                engine_draw_frame(engine);
            }
#endif
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
//            engine_term_display(engine);
            break;
        case APP_CMD_GAINED_FOCUS:
#if 0
            // When our app gains focus, we start monitoring the accelerometer.
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                        engine->accelerometerSensor, (1000L/60)*1000);
            }
#endif
            engineReady = true;
            break;
        case APP_CMD_LOST_FOCUS:
#if 0
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
            }
            // Also stop animating.
            engine->animating = 0;
            engine_draw_frame(engine);
#endif
            engineReady = false;
            break;
    }
}


/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {
    //struct engine engine;
    Sys_Printf("Hello baby!\n");
    AndroidCpuFamily family = android_getCpuFamily();
    
    switch (family) {
        case ANDROID_CPU_FAMILY_ARM:
            Com_Printf("CPU Family is ARM\n");
            break;
        case ANDROID_CPU_FAMILY_X86:
            Com_Printf("CPU Family is X86\n");
            break;
        case ANDROID_CPU_FAMILY_MIPS:
            Com_Printf("CPU Family is MIPS\n");
            break;
        case ANDROID_CPU_FAMILY_ARM64:
            Com_Printf("CPU Family is ARM64\n");
            break;
        case ANDROID_CPU_FAMILY_X86_64:
            Com_Printf("CPU Family is X86_64\n");
            break;
        case ANDROID_CPU_FAMILY_MIPS64:
            Com_Printf("CPU Family is MIPS64\n");
            break;
        default:
            Com_Printf("Unknown");
    }
    
    uint64_t features = android_getCpuFeatures();
    
    if (features & ANDROID_CPU_ARM_FEATURE_ARMv7) {
        Com_Printf("Feature ARMv7\n");
    }
    
    if (features & ANDROID_CPU_ARM_FEATURE_VFPv3) {
        Com_Printf("Feature VFPv3\n");
    }
    
    if (features & ANDROID_CPU_ARM_FEATURE_NEON) {
        Com_Printf("Feature NEON\n");
    }
    
    int cpuCount = android_getCpuCount();
    
    Com_Printf("%d CPUs\n", cpuCount);

    // Make sure glue isn't stripped.
    app_dummy();
#if 0
    memset(&engine, 0, sizeof(engine));
    state->userData = &engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    engine.app = state;

    // Prepare to monitor accelerometer
    engine.sensorManager = ASensorManager_getInstance();
    engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
            ASENSOR_TYPE_ACCELEROMETER);
    engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
            state->looper, LOOPER_ID_USER, NULL, NULL);

    if (state->savedState != NULL) {
        // We are starting with a previous saved state; restore from it.
        engine.state = *(struct saved_state*)state->savedState;
    }
#endif
state->onAppCmd = engine_handle_cmd;
state->onInputEvent = engine_handle_input;

     mainWindow = state->window;

    // loop waiting for stuff to do.

        int     time, oldtime, newtime;

    // go back to real user for config loads
    saved_euid = geteuid();
    seteuid(getuid());

    //Com_Init();

    oldtime = Sys_Milliseconds ();

    while (1) {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        // XXX while ((ident=ALooper_pollAll(/*engine.animating ? 0 : -1*/ 0, NULL, &events, (void**)&source)) >= 0) {
        while ((ident=ALooper_pollAll(engineReady ? 0 : -1, NULL, &events, (void**)&source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(state, source);
            }
#if 0
            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER) {
                if (engine.accelerometerSensor != NULL) {
                    ASensorEvent event;
                    while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
                            &event, 1) > 0) {
                        LOGI("accelerometer: x=%f y=%f z=%f",
                                event.acceleration.x, event.acceleration.y,
                                event.acceleration.z);
                    }
                }
            }
#endif
            // Check if we are exiting.
            if (state->destroyRequested != 0) {
                //engine_term_display(&engine);
                return;
            }
        }
#if 0
        if (engine.animating) {
            // Done with events; draw next animation frame.
            engine.state.angle += .01f;
            if (engine.state.angle > 1) {
                engine.state.angle = 0;
            }

            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            engine_draw_frame(&engine);
        }
#endif

        if (engineReady) {
            do {
                newtime = Sys_Milliseconds ();
                time = newtime - oldtime;
            } while (time < 1);

            Com_Frame(time);
            oldtime = newtime;
        }
    }
}

# if 0
#include <jni.h>

#include <android/native_activity.h>
#include <android/log.h>



void ANativeActivity_onCreate(ANativeActivity *activity, void *savedState, size_t savedStateSize) {
    __android_log_print(ANDROID_LOG_INFO, "boobs", "Testing");
}
#endif