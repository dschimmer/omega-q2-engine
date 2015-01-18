# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

MY_COMMON_PATH = ../../common
MY_CLIENT_PATH = ../../client
MY_SERVER_PATH = ../../server
MY_GAME_PATH = ../../game
MY_NULL_PATH = ../../null
MY_GL_PATH = ../../ref_gles
#MY_SOUND_PATH = ../../sound
MY_SOUND_PATH = ../../client
MY_SHARED_PATH = ../../shared

MY_COMMON_SRC = \
    $(MY_COMMON_PATH)/cmd.c \
    $(MY_COMMON_PATH)/cmodel.c \
    $(MY_COMMON_PATH)/common.c \
    $(MY_COMMON_PATH)/crc.c \
    $(MY_COMMON_PATH)/cvar.c \
    $(MY_COMMON_PATH)/files.c \
    $(MY_COMMON_PATH)/md4.c \
    $(MY_COMMON_PATH)/net_chan.c \
    $(MY_COMMON_PATH)/pmove.c
#    $(MY_COMMON_PATH)/pcx.c \
#    $(MY_COMMON_PATH)/tga.c

MY_CLIENT_SRC = \
    $(MY_CLIENT_PATH)/cl_cin.c \
    $(MY_CLIENT_PATH)/cl_ents.c \
    $(MY_CLIENT_PATH)/cl_fx.c \
    $(MY_CLIENT_PATH)/cl_input.c \
    $(MY_CLIENT_PATH)/cl_inv.c \
    $(MY_CLIENT_PATH)/cl_main.c \
    $(MY_CLIENT_PATH)/cl_newfx.c \
    $(MY_CLIENT_PATH)/cl_parse.c \
    $(MY_CLIENT_PATH)/cl_pred.c \
    $(MY_CLIENT_PATH)/cl_tent.c \
    $(MY_CLIENT_PATH)/cl_scrn.c \
    $(MY_CLIENT_PATH)/cl_view.c \
    $(MY_CLIENT_PATH)/console.c \
    $(MY_CLIENT_PATH)/keys.c \
    $(MY_CLIENT_PATH)/menu.c \
    $(MY_SOUND_PATH)/snd_dma.c \
    $(MY_SOUND_PATH)/snd_mem.c \
    $(MY_SOUND_PATH)/snd_mix.c \
    $(MY_CLIENT_PATH)/qmenu.c
#    $(MY_GAME_PATH)/m_flash.c
# $(SOUND_FILES) $(CLIENT_SYSTEM_FILES) $(REF_SOFT_FILES)
# $(MY_CLIENT_PATH)/cl_demo.c $(MY_CLIENT_PATH)/sbar.c $(MY_CLIENT_PATH)/screen.c $(MY_CLIENT_PATH)/view.c

MY_SERVER_SRC = \
    $(MY_SERVER_PATH)/sv_ccmds.c \
    $(MY_SERVER_PATH)/sv_ents.c \
    $(MY_SERVER_PATH)/sv_game.c \
    $(MY_SERVER_PATH)/sv_init.c \
    $(MY_SERVER_PATH)/sv_main.c \
    $(MY_SERVER_PATH)/sv_send.c \
    $(MY_SERVER_PATH)/sv_user.c \
    $(MY_SERVER_PATH)/sv_world.c
# $(MY_SERVER_PATH)/pr_cmds.c $(MY_SERVER_PATH)/pr_edict.c $(MY_SERVER_PATH)/pr_exec.c $(MY_SERVER_PATH)/sv_move.c $(MY_SERVER_PATH)/sv_phys.c $(MY_SERVER_PATH)/world.c

MY_GL_SRC = \
	$(MY_GL_PATH)/gles_draw.c \
	$(MY_GL_PATH)/gles_image.c \
	$(MY_GL_PATH)/gles_light.c \
	$(MY_GL_PATH)/gles_mesh.c \
	$(MY_GL_PATH)/gles_model.c \
	$(MY_GL_PATH)/gles_rmain.c \
	$(MY_GL_PATH)/gles_rmisc.c \
	$(MY_GL_PATH)/gles_rsurf.c \
	$(MY_GL_PATH)/gles_warp.c \
	$(MY_GL_PATH)/gles_error.c \
	gl_glx.c \
	qgl_android.c
	
#	$(MY_GL_PATH)/formats/pcx.c \
#	$(MY_GL_PATH)/formats/tga.c \
	
MY_SHARED_SRC = \
	$(MY_SHARED_PATH)/shared.c \
	$(MY_SHARED_PATH)/flash.c

MY_GAME_SRC = \
	$(MY_GAME_PATH)/g_ai.c \
	$(MY_GAME_PATH)/p_client.c \
	$(MY_GAME_PATH)/g_cmds.c \
	$(MY_GAME_PATH)/g_svcmds.c \
	$(MY_GAME_PATH)/g_chase.c \
	$(MY_GAME_PATH)/g_combat.c \
	$(MY_GAME_PATH)/g_func.c \
	$(MY_GAME_PATH)/g_items.c \
	$(MY_GAME_PATH)/g_main.c \
	$(MY_GAME_PATH)/g_misc.c \
	$(MY_GAME_PATH)/g_monster.c \
	$(MY_GAME_PATH)/g_phys.c \
	$(MY_GAME_PATH)/g_save.c \
	$(MY_GAME_PATH)/g_spawn.c \
	$(MY_GAME_PATH)/g_target.c \
	$(MY_GAME_PATH)/g_trigger.c \
	$(MY_GAME_PATH)/g_turret.c \
	$(MY_GAME_PATH)/g_utils.c \
	$(MY_GAME_PATH)/g_weapon.c \
	$(MY_GAME_PATH)/m_actor.c \
	$(MY_GAME_PATH)/m_berserk.c \
	$(MY_GAME_PATH)/m_boss2.c \
	$(MY_GAME_PATH)/m_boss3.c \
	$(MY_GAME_PATH)/m_boss31.c \
	$(MY_GAME_PATH)/m_boss32.c \
	$(MY_GAME_PATH)/m_brain.c \
	$(MY_GAME_PATH)/m_chick.c \
	$(MY_GAME_PATH)/m_flipper.c \
	$(MY_GAME_PATH)/m_float.c \
	$(MY_GAME_PATH)/m_flyer.c \
	$(MY_GAME_PATH)/m_gladiator.c \
	$(MY_GAME_PATH)/m_gunner.c \
	$(MY_GAME_PATH)/m_hover.c \
	$(MY_GAME_PATH)/m_infantry.c \
	$(MY_GAME_PATH)/m_insane.c \
	$(MY_GAME_PATH)/m_medic.c \
	$(MY_GAME_PATH)/m_move.c \
	$(MY_GAME_PATH)/m_mutant.c \
	$(MY_GAME_PATH)/m_parasite.c \
	$(MY_GAME_PATH)/m_soldier.c \
	$(MY_GAME_PATH)/m_supertank.c \
	$(MY_GAME_PATH)/m_tank.c \
	$(MY_GAME_PATH)/p_hud.c \
	$(MY_GAME_PATH)/p_trail.c \
	$(MY_GAME_PATH)/p_view.c \
    $(MY_GAME_PATH)/p_weapon.c
#	$(MY_GAME_PATH)/p_weapon.c \
#	$(MY_GAME_PATH)/m_flash.c

MAIN_SRC = $(MY_NULL_PATH)/cd_null.c q_shandroid.c vid_menu.c vid_so.c snd_android.c sys_android.c glob.c net_udp.c 
#$(MY_GAME_PATH)/q_shared.c

#LOCAL_CFLAGS    := -DGAME_HARD_LINKED
LOCAL_MODULE    := quake2
LOCAL_SRC_FILES := $(MY_SHARED_SRC) $(MY_COMMON_SRC) $(MY_CLIENT_SRC) $(MY_SERVER_SRC) $(MAIN_SRC)
#LOCAL_SRC_FILES := sys_android.c
LOCAL_LDLIBS    := -lm -llog -landroid -lOpenSLES
LOCAL_STATIC_LIBRARIES := android_native_app_glue cpufeatures

include $(BUILD_SHARED_LIBRARY)

#$(call import-module,android/native_app_glue)


include $(CLEAR_VARS)
LOCAL_MODULE    := ref_gles
LOCAL_SRC_FILES := $(MY_GL_SRC) $(MY_SHARED_SRC) q_shandroid.c glob.c
LOCAL_LDLIBS    := -lm -llog -landroid -lEGL
LOCAL_STATIC_LIBRARIES := android_native_app_glue
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/ref_gles

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := game_base
LOCAL_SRC_FILES         := $(MY_GAME_SRC) $(MY_SHARED_SRC)
LOCAL_LDLIBS            := -lm -llog -landroid
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/game

include $(BUILD_SHARED_LIBRARY)




$(call import-module,android/native_app_glue)
$(call import-module,android/cpufeatures)