#include "server_proxy.h"
#include "stdio.h"
#include "enet/enet.h"
#include <string.h>

static ENetHost* g_enet_host = NULL;
static Callback g_log = NULL;
const static unsigned int MAX_NETWORK_DELAY = 9999;
static short g_enable_kcp = 0;


void debug_log(const char * str)
{
    if (g_log)
    {
        g_log(str);
    }
}

void initialize()
{
    enet_initialize();
}

intptr_t connect_server(const char* ip, short port)
{
    ENetAddress addr;
    enet_address_set_host(&addr, ip);
    addr.port = port;
    if (g_enet_host == NULL)
    {
        g_enet_host = enet_host_create(NULL, 32, 1, 0, 0);
    }
    if (g_enet_host == NULL)
    {
        return NULL;
    }
    return enet_host_connect(g_enet_host, &addr, 2, 0);
}

void send_data_reliable(intptr_t ppeer, const char *data, uint32_t length)
{
    ENetPeer *peer = (ENetPeer*)(ppeer);
    if (peer != NULL)
    {
        ENetPacketFlag flag = ENET_PACKET_FLAG_KCP;
        if(!g_enable_kcp)
        {
            flag = ENET_PACKET_FLAG_RELIABLE;
        }
        ENetPacket *pkt = enet_packet_create(data, length, flag);
        enet_peer_send(peer, 0, pkt);
    }
}

void send_data_unreliable(intptr_t ppeer, const char *data, uint32_t length)
{
    ENetPeer *peer = (ENetPeer*)(ppeer);
    if (peer != NULL)
    {
        ENetPacket *pkt = enet_packet_create(data, length, 0);
        enet_peer_send(peer, 1, pkt);
    }
}

void free_data(intptr_t data)
{
    free(data);
}

int tick(intptr_t *peer, intptr_t *buf, uint32_t *length)
{
    if (!g_enet_host) return 0;
    ENetEvent event;
    while (enet_host_service(g_enet_host, &event, 100) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
        {
            *peer = event.peer;
            return 1;
        }
        break;
        case ENET_EVENT_TYPE_DISCONNECT:
        {
            *peer = event.peer;
            return 2;
        }
        break;
        case ENET_EVENT_TYPE_RECEIVE:
        {
            *peer = event.peer;
            *length = event.packet->dataLength;
            *buf = malloc(*length);
            memcpy(*buf, event.packet->data, event.packet->dataLength);
            enet_packet_destroy(event.packet);
            return 3;
        }
        break;
        default:
            break;
        }
    }
    return 0;
}


void set_callback(Callback h)
{
    g_log = h;
}

void close_peer(intptr_t p)
{
    ENetPeer *peer = p;
    if (peer)
    {
        enet_peer_disconnect_now(peer, 0);
    }
    enet_host_destroy(g_enet_host);
    g_enet_host = NULL;
}

void set_peer_time_out(intptr_t p, uint32_t timeout_limit, uint32_t timeout_minimum, uint32_t timeout_maximum)
{
    ENetPeer *peer = p;
    if (peer)
    {
        enet_peer_timeout(peer, timeout_limit, timeout_minimum, timeout_maximum);
    }
}

unsigned int get_network_delay(intptr_t p)
{
    ENetPeer *peer = p;
    if (peer && (peer->state == ENET_PEER_STATE_CONNECTED || peer->state == ENET_PEER_STATE_DISCONNECT_LATER))
    {
        return peer->roundTripTime / 2;
    }
    return MAX_NETWORK_DELAY;
}

void set_kcp_enable(short enable)
{
    g_enable_kcp = enable;
}