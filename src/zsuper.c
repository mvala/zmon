/*  =========================================================================
    zsuper - manager of zactors

    Generated codec implementation for zsuper
    -------------------------------------------------------------------------
    Copyright (c) 2014 UEF SAV -- http://www.saske.sk                       
    Copyright other contributors as noted in the AUTHORS file.              
                                                                            
    This file is part of ztask, an open-source framework for running various
    tasks in parallel                                                       
                                                                            
    This is free software; you can redistribute it and/or modify it under   
    the terms of the GNU Lesser General Public License as published by the  
    Free Software Foundation; either version 3 of the License, or (at your  
    option) any later version.                                              
                                                                            
    This software is distributed in the hope that it will be useful, but    
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTA-   
    BILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General  
    Public License for more details.                                        
                                                                            
    You should have received a copy of the GNU Lesser General Public License
    along with this program. If not, see http://www.gnu.org/licenses/.      
    =========================================================================
*/

/*
@header
    zsuper - manager of zactors
@discuss
@end
*/

#include "../include/zmon.h"

//  Structure of our class

struct _zsuper_t {
    void *inbox_url;          //  inbox url
    void *outbox_url;         //  outbox url
    zactor_t *actor;          //  current actor
    zhash_t *actors;          //  list of actors
};

//  --------------------------------------------------------------------------
//  Create a new zsuper

zsuper_t *
zsuper_new (char *inbox_url, char *outbox_url)
{
    zsuper_t *self = (zsuper_t *) zmalloc (sizeof (zsuper_t));
    self->inbox_url = inbox_url;
    self->outbox_url = outbox_url;
    self->actor = NULL;
    self->actors = zhash_new ();
    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the zsuper

void
zsuper_destroy (zsuper_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        zsuper_t *self = *self_p;
        zhash_destroy (&self->actors);
        if (self->actor) zactor_destroy (&self->actor);
        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}

//  --------------------------------------------------------------------------
//  Print contents of message to stdout

void
zsuper_dump (zsuper_t *self)
{
    assert (self);
}

//  --------------------------------------------------------------------------
//  Actor
//  must call zsock_signal (pipe) when initialized
//  must listen to pipe and exit on $TERM command


static void
zsuper_engine (zsock_t *pipe, void *args)
{
    //  Do some initialization
    assert (args);

    zsuper_t *super = (zsuper_t *) args;

    zsock_signal (pipe, 0);

    zsock_t *sock_in = zsock_new (ZMQ_PULL);
    int rc = zsock_bind (sock_in, "%s", (char *) super->inbox_url);
    //  TODO return message that we could not bind
    assert (rc != -1);

    zsock_t *sock_out = NULL;
    if (super->outbox_url) {
        sock_out = zsock_new (ZMQ_PUSH);
        rc = zsock_connect (sock_out, "%s", (char *) super->outbox_url);
        assert (rc != -1);
    }

    zpoller_t *poller = zpoller_new (pipe, sock_in, NULL);

    bool terminated = false;
    while (!terminated) {
        void *which = zpoller_wait (poller, -1);
        if (which == pipe) {
            zmsg_t *msg = zmsg_recv (which);
            if (!msg)
                break;              //  Interrupted
            char *command = zmsg_popstr (msg);
            if (streq (command, "$TERM"))
                terminated = true;
            else
            if (streq (command, "ECHO"))
                zmsg_send (&msg, pipe);
            else {
                puts ("E: invalid message to actor");
                assert (false);
            }
            free (command);
            zmsg_destroy (&msg);
        }
        else
        if (which == sock_in) {
            zmsg_t *msg = zmsg_recv (which);
            if (!msg)
                break;              //  Interrupted
            // msg is just redirected to zsuper
            zmsg_send (&msg, pipe);
            zmsg_destroy (&msg);
        }

    }

    zsock_destroy (&sock_in);
    zsock_destroy (&sock_out);
}


//  --------------------------------------------------------------------------
//  Main loop

void
zsuper_loop (zsuper_t *self)
{
    assert (self);

    self->actor = zactor_new (zsuper_engine, self);

    while (!zsys_interrupted) {
        zmsg_t *msg = zmsg_recv (self->actor);
        if (!msg)
            break;
        char *command = zmsg_popstr (msg);
        char *identity = zmsg_popstr (msg);
        if (streq (command, "$TERM")) {
            break;
        }
        else
        if (streq (command, "HELLO")) {
            zhash_insert (self->actors, identity, NULL);
        }
        else
        if (streq (command, "BYE")) {
            zhash_delete (self->actors, identity);
        } else {
            puts ("E: invalid message in zsuper_loop");
            assert (false);
        }
        free (command);
        zmsg_destroy (&msg);
    }

//    zclock_log ("wk=%ld", zhash_size (self->actors));

}

//  --------------------------------------------------------------------------
//  Selftest

int
zsuper_test (bool verbose)
{
    printf (" * zsuper: ");
    if (verbose)
        printf ("\n");

    //  @selftest
    //  Simple create/destroy test
    zsuper_t *self = zsuper_new ("inproc://zsuper", NULL);
    assert (self);

    zsock_t *sock = zsock_new (ZMQ_PUSH);
    zsock_connect (sock, "inproc://zsuper");
    zstr_sendx (sock, "HELLO", "1", NULL);
    zstr_sendx (sock, "HELLO", "2", NULL);
    zstr_sendx (sock, "HELLO", "3", NULL);
    zstr_sendx (sock, "HELLO", "4", NULL);
    zstr_sendx (sock, "HELLO", "5", NULL);
    zstr_sendx (sock, "BYE", "1", NULL);

    zstr_sendx (sock, "$TERM", NULL);

    zsock_destroy (&sock);

    zsuper_loop (self);

    //  zsuper_destroy will destroy its zactor
    zsuper_destroy (&self);

    if (verbose)
        printf (" * zsuper: ");
    printf ("OK\n");
    return 0;
}
