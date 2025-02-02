

/*

FakeNES - A portable, Open Source NES emulator.

Distributed under the Clarified Artistic License.

netplay.c: Implementation of the NetPlay engine.

Copyright (c) 2003, Randy McDowell.
Copyright (c) 2003, Charles Bilyue'.

This is free software.  See 'LICENSE' for details.
You must read and accept the license prior to use.

*/


#include <allegro.h>


#include "misc.h"


#include "netplay.h"


int netplay_protocol = NETPLAY_PROTOCOL_TCPIP;


int netplay_server_active = FALSE;

int netplay_client_active = FALSE;


int netplay_init (void)
{
    netplay_protocol = get_config_int ("netplay", "protocol", NETPLAY_PROTOCOL_TCPIP);


    return (0);
}


void netplay_exit (void)
{
    if (netplay_server_active)
    {
        /* We should never be here... */

        netplay_close_server ();
    }


    if (netplay_client_active)
    {
        /* Or here... */

        netplay_close_client ();
    }


    set_config_int ("netplay", "protocol", netplay_protocol);
}


#ifdef USE_HAWKNL


#include <nl.h>


static NLsocket current_socket = NL_INVALID;


static void net_close (void);


static int net_open (void)
{
    nlEnable (NL_BLOCKING_IO);


    /* FakeNES sends data in little-endian format. */

    nlEnable (NL_LITTLE_ENDIAN_DATA);


    switch (netplay_protocol)
    {
        case NETPLAY_PROTOCOL_TCPIP:

            nlSelectNetwork (NL_IP);


            nlHint (NL_REUSE_ADDRESS, NL_TRUE);


            break;


        case NETPLAY_PROTOCOL_SPX:

            nlSelectNetwork (NL_IPX);


            break;


        default:

            return (1);


            break;
    }


    if (nlInit () != NL_TRUE)
    {
        return (2);
    }


    current_socket = nlOpen (NETPLAY_PORT, NL_RELIABLE_PACKETS);

    if (current_socket == NL_INVALID)
    {
        net_close ();


        return (3);
    }


    return (0);
}


static void net_close (void)
{
    if (current_socket != NL_INVALID)
    {
        nlClose (current_socket);
    }


    nlShutdown ();
}


int netplay_open_server (void)
{
    int result;


    result = net_open ();

    if (result != 0)
    {
        return ((8 + result));
    }


    if (nlListen (current_socket) != NL_TRUE)
    {
        net_close ();


        return (1);
    }


    netplay_server_active = TRUE;


    return (0);
}


void netplay_close_server (void)
{
    if (netplay_server_active)
    {
        net_close ();


        netplay_server_active = FALSE;
    }
}


int netplay_poll_server (void)
{
    NLsocket new_socket;


    new_socket = nlAcceptConnection (current_socket);

    if (new_socket != NL_INVALID)
    {
        nlClose (current_socket);


        current_socket = new_socket;


        return (TRUE);
    }


    return (FALSE);
}


int netplay_open_client (const UINT8 * address)
{
    NLaddress nl_address;


    int result;

    
    result = net_open ();

    if (result != 0)
    {
        return ((8 + result));
    }


    nlStringToAddr (address, &nl_address);


    nlSetAddrPort (&nl_address, NETPLAY_PORT);


    if (nlConnect (current_socket, &nl_address) != NL_TRUE)
    {
        net_close ();


        return (1);
    }


    netplay_client_active = TRUE;


    return (0);
}


void netplay_close_client (void)
{
    if (netplay_client_active)
    {
        net_close ();
    
    
        netplay_client_active = FALSE;
    }
}


#else


int netplay_open_server (void)
{
    return (1);
}


void netplay_close_server (void)
{
}


int netplay_open_client (const UINT8 * address)
{
    return (1);
}


void netplay_close_client (void)
{
}


int netplay_poll_server (void)
{
    return (FALSE);
}


#endif /* USE_HAWKNL */
