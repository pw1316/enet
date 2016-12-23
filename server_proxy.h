#pragma once
#include <stdint.h>
#ifdef _WIN32
#define PM_DLL_EXPORT __declspec(dllexport)
#else
#define PM_DLL_EXPORT
#endif

typedef int( *Callback)(const char *text);

PM_DLL_EXPORT void set_callback(Callback handler);

PM_DLL_EXPORT void initialize();
PM_DLL_EXPORT intptr_t connect_server(const char *ip, short port);
PM_DLL_EXPORT void send_data(intptr_t peer, const char *data, uint32_t length);
PM_DLL_EXPORT int tick(intptr_t *peer, intptr_t *buf, uint32_t *length);

PM_DLL_EXPORT void free_data(intptr_t data);

PM_DLL_EXPORT void close_peer(intptr_t peer);

PM_DLL_EXPORT void set_peer_time_out(intptr_t peer, uint32_t timeout_limit, uint32_t timeout_minimum, uint32_t timeout_maximum);
