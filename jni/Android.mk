LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := enet
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include/

LOCAL_CPPFLAGS := -03 -ffast-math
LOCAL_SRC_FILES := ../callbacks.c  ../compress.c  ../host.c  ../list.c  ../packet.c  ../peer.c  ../protocol.c  ../server_proxy.c  ../unix.c  ../win32.c ../ikcp.c

include $(BUILD_SHARED_LIBRARY)