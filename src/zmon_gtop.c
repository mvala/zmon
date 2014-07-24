/*  =========================================================================
    zmon_gtop - zmon gtop plugin

    Generated codec implementation for zmon_gtop
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
    zmon_gtop - zmon gtop plugin
@discuss
@end
*/

#include "../include/zmon.h"

//  Structure of our class

struct _zmon_gtop_t {
//    glibtop_cpu cpu;         //  current cpu
//    glibtop_mem memory;      //  current memory
//    glibtop_netload net;     //  network load
//    char *net_interface;     //  net interface
    zmon_gtop_proc_t *sys;   //  system proc
    zlist_t *proceses;       //  list of proce
};

//  --------------------------------------------------------------------------
//  Create a new zmon_gtop

zmon_gtop_t *
zmon_gtop_new (char *net_interface)
{
    zmon_gtop_t *self = (zmon_gtop_t *) zmalloc (sizeof (zmon_gtop_t));
    self->sys = zmon_gtop_proc_new (0);

    //    net_interface = strdup (net_interface);
    self->proceses = zlist_new ();
    glibtop_init ();
    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the zmon_gtop

void
zmon_gtop_destroy (zmon_gtop_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        zmon_gtop_t *self = *self_p;

        zmon_gtop_processes_destroy (self);
        zlist_destroy (&self->proceses);

//        free (self->net_interface);

        zmon_gtop_proc_destroy (&self->sys);
        glibtop_close ();

        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}

//  --------------------------------------------------------------------------
//  Print contents of message to stdout

void
zmon_gtop_dump (zmon_gtop_t *self)
{
    assert (self);
    zmon_gtop_proc_dump (self->sys);
    zmon_gtop_proc_t *p = (zmon_gtop_proc_t *) zlist_first (self->proceses);
    while (p) {
        zmon_gtop_proc_dump (p);
        p = (zmon_gtop_proc_t *) zlist_next (self->proceses);
    }
}

//  --------------------------------------------------------------------------
//  Publish monitoring information to pub_socket
void
zmon_gtop_publish (zmon_gtop_t *self, void *to)
{
    assert (self);
    assert (to);
    zmsg_t *msg = zmsg_new ();
//    zmsg_addstrf (msg, "\"cpu\" : \"%.2f\"", zmon_gtop_proc_cpu (self->sys));
//    zmsg_addstrf (msg, "\"mem_rss\" : \"%.0f\"", zmon_gtop_proc_mem_rss (self->sys));
    // TODO
    zmsg_send (&msg, to) ;
}

//  --------------------------------------------------------------------------
//  Destroy all subprocesses

void
zmon_gtop_processes_destroy (zmon_gtop_t *self)
{
    assert (self);
    zmon_gtop_proc_t *p;
    while ( (p = (zmon_gtop_proc_t *) zlist_pop (self->proceses))) {
        zmon_gtop_proc_destroy (&p);
    }
}

//  --------------------------------------------------------------------------
//  Update all subprocesses

void
zmon_gtop_update (zmon_gtop_t *self)
{
    assert (self);

    zclock_log ("zmon_gtop_update");
    zmon_gtop_proc_update (self->sys);
    zmon_gtop_proc_t *p = (zmon_gtop_proc_t *) zlist_first (self->proceses);
    while (p) {
        zmon_gtop_proc_update (p);
        p = (zmon_gtop_proc_t *) zlist_next (self->proceses);
    }


//    glibtop_get_cpu (&self->cpu);
//    glibtop_get_mem (&self->memory);
//    if (self->net_interface)
//        glibtop_get_netload (&self->net, self->net_interface);
//
//    /* update system monitoring */
//    zmon_gtop_proc_update (self->sys, &self->cpu, &self->memory, &self->net, 0);
//
//    /* update processes monitoring */
//    zmon_gtop_proc_t *p = (zmon_gtop_proc_t *) zlist_first (self->proceses);
//    while (p) {
//        zmon_gtop_proc_update (p, &self->cpu, &self->memory, &self->net, self->sys);
//        p = (zmon_gtop_proc_t *) zlist_next (self->proceses);
//    }
//
//    /* setting current values for total and used cpu */
//    zmon_gtop_proc_set_cpu_total_last (self->sys, self->cpu.total);
//    zmon_gtop_proc_set_cpu_used_last (self->sys, self->cpu.user + self->cpu.nice + self->cpu.sys);
}

//  --------------------------------------------------------------------------
//  Is process monitred

int
zmon_gtop_is_process (zmon_gtop_t *self, pid_t pid)
{
    assert (self);
    zmon_gtop_proc_t *p = (zmon_gtop_proc_t *) zlist_first (self->proceses);
    while (p) {
        if (zmon_gtop_proc_pid (p) == pid)
            return 1;
        p = (zmon_gtop_proc_t *) zlist_next (self->proceses);
    }
    return 0;
}

//  --------------------------------------------------------------------------
//  Add process

void
zmon_gtop_add_process (zmon_gtop_t *self, pid_t pid)
{
    assert (self);
    // TODO think of it
    if (zmon_gtop_proc_pid (self->sys) == pid) return;

    zmon_gtop_proc_t *p = zmon_gtop_proc_new (pid);
    zlist_append (self->proceses, p);
}

//  --------------------------------------------------------------------------
//  Remove Process

void
zmon_gtop_remove_process (zmon_gtop_t *self, pid_t pid)
{
    assert (self);
    if (zmon_gtop_proc_pid (self->sys) != pid) return;

    zmon_gtop_proc_t *p = (zmon_gtop_proc_t *) zlist_first (self->proceses);
    while (p) {
        if (zmon_gtop_proc_pid (p) == pid) {
            zlist_remove (self->proceses, p);
            return;
        }
        p = (zmon_gtop_proc_t *) zlist_next (self->proceses);
    }
}

//  --------------------------------------------------------------------------
//  Selftest

int
zmon_gtop_test (bool verbose)
{
    printf (" * zmon_gtop: ");
    if (verbose)
        printf ("\n");

    //  @selftest
    //  Simple create/destroy test
    zmon_gtop_t *self = zmon_gtop_new ("");
    assert (self);

    zmon_gtop_add_process (self, getpid());

    for (int i = 0; i < 4; i++) {
        zmon_gtop_update (self);
        if (verbose)
            zmon_gtop_dump (self);
        zclock_sleep (500);
    }

    zmon_gtop_destroy (&self);
    if (verbose)
        printf (" * zmon_gtop: ");

    printf ("OK\n");
    return 0;
}
