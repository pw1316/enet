#!/bin/bash
#
# Windows 32-bit/64-bit

# Copyright (C) polynation games ltd - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Christopher Redden, December 2013

# 62 Bit Version


gcc -O3 -o Plugins/x86_64/enet.dll -m64 -shared \
	callbacks.c  compress.c  host.c  list.c  packet.c  peer.c  protocol.c  server_proxy.c  unix.c  win32.c \
    -I./include/ \
    -lWs2_32 -lwinmm -static-libgcc -static-libstdc++

read -n 1