/*  =========================================================================
    zmon_gtop_proc - zmon gtop pid monitor

    Generated codec implementation for zmon_gtop_proc
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
    zmon_gtop_proc - zmon gtop pid monitor
@discuss
@end
*/

#include <glibtop/proctime.h>
#include <glibtop/procmem.h>
#include <glibtop/netlist.h>
#include <glibtop/netload.h>

#include "../include/zmon.h"

//  Structure of our class

struct _zmon_gtop_proc_t {
    pid_t pid;               //  Pid of process
    char *json;              //  Monitoring info in JSON format
    zmon_val_t *cpu;         //  CPU values
    zmon_val_t *mem;         //  MEM values
    zhash_t *nets;           //  One entry for each net interface
    int64_t last_update;     //  Time of last update in ms
};

//  --------------------------------------------------------------------------
//  Create a new zmon_gtop_proc

zmon_gtop_proc_t *
zmon_gtop_proc_new (pid_t pid)
{
    zmon_gtop_proc_t *self = (zmon_gtop_proc_t *) zmalloc (sizeof (zmon_gtop_proc_t));
    self->pid = pid;

    self->cpu = zmon_val_new ();
    self->mem = zmon_val_new ();
//    self->net = zmon_val_new ();

    self->json = NULL;

    zmon_val_set_current (self->cpu, zmalloc (sizeof (glibtop_cpu)));
    zmon_val_set_previous (self->cpu, zmalloc (sizeof (glibtop_cpu)));

    zmon_val_set_current (self->mem, zmalloc (sizeof (glibtop_mem)));
    zmon_val_set_previous (self->mem, zmalloc (sizeof (glibtop_mem)));

    self->nets = zhash_new ();

    glibtop_netlist netlist;
    char **devices;
    devices = glibtop_get_netlist(&netlist);
    for (int i=0; i<netlist.number; i++) {
        void *net_val = zmon_val_new ();
        zmon_val_set_current (net_val, zmalloc (sizeof (glibtop_netload)));
        zmon_val_set_previous (net_val, zmalloc (sizeof (glibtop_netload)));
        zhash_insert (self->nets, devices[i], net_val);
    }
    g_strfreev(devices);

    self->last_update = 0;

    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the zmon_gtop_proc

void
zmon_gtop_proc_destroy (zmon_gtop_proc_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        zmon_gtop_proc_t *self = *self_p;

        zmon_val_destroy (&self->cpu);
        zmon_val_destroy (&self->mem);

        zmon_val_t *net_val = (zmon_val_t*) zhash_first (self->nets);
        while (net_val) {
            zmon_val_destroy (&net_val);
            net_val = (zmon_val_t*) zhash_next (self->nets);
        }
        zhash_destroy (&self->nets);

        free (self->json);

        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}

//  --------------------------------------------------------------------------
//  Print contents of message to stdout

void
zmon_gtop_proc_dump (zmon_gtop_proc_t *self)
{
    assert (self);
    if (!self->pid)
        printf ("System: ");
    else
        printf ("  pid=%5d", self->pid);

    printf ("%s\n", self->json);
}

//  --------------------------------------------------------------------------
//  Fill cpu info

char *
zmon_gtop_proc_fill_cpu (zmon_gtop_proc_t *self)
{
    assert (self);

    zmon_val_switch (self->cpu);

    glibtop_cpu *cur_cpu = (glibtop_cpu *) zmon_val_get_current (self->cpu);
    glibtop_cpu *prev_cpu = (glibtop_cpu *) zmon_val_get_previous (self->cpu);

    glibtop_get_cpu (cur_cpu);


    float total_diff = (float) (cur_cpu->total - prev_cpu->total);
    float sys_diff = (float) (cur_cpu->sys - prev_cpu->sys);
    float user_diff = (float) (cur_cpu->user - prev_cpu->user);
    float nice_diff = (float) (cur_cpu->nice - prev_cpu->nice);
    float used_diff =  (float) (sys_diff + user_diff + nice_diff);

    char *ret_str = NULL;

    if (asprintf (&ret_str, "%s, \"cpu\" : { ", self->json) < 0)
        ret_str = NULL;
    free (self->json);
    self->json = ret_str;

    if (asprintf (&ret_str, "%s \"sys\" : \"%.2f\"", self->json , 100 * sys_diff / total_diff) < 0)
        ret_str = NULL;
    free (self->json);
    self->json = ret_str;

    if (asprintf (&ret_str, "%s, \"user\" : \"%.2f\"", self->json , 100 * user_diff / total_diff) < 0)
        ret_str = NULL;
    free (self->json);
    self->json = ret_str;

    if (asprintf (&ret_str, "%s, \"nice\" : \"%.2f\"", self->json , 100 * nice_diff / total_diff) < 0)
        ret_str = NULL;
    free (self->json);
    self->json = ret_str;

    if (asprintf (&ret_str, "%s, \"idle\" : \"%.2f\"", self->json , 100 * (total_diff - used_diff) / total_diff) < 0)
        ret_str = NULL;
    free (self->json);
    self->json = ret_str;

    if (asprintf (&ret_str, "%s } ", self->json) < 0)
        ret_str = NULL;
    free (self->json);
    self->json = ret_str;


    return ret_str;

}

//  --------------------------------------------------------------------------
//  Fill mem info

char *
zmon_gtop_proc_fill_mem (zmon_gtop_proc_t *self)
{
    assert (self);

    zmon_val_switch (self->mem);

    glibtop_mem *cur = (glibtop_mem *) zmon_val_get_current (self->mem);
//    glibtop_mem *prev = (glibtop_mem *) zmon_val_get_previous (self->mem);


    glibtop_get_mem (cur);

    char *ret_str = NULL;

    if (asprintf (&ret_str, "%s, \"mem\" : { ", self->json) < 0)
        ret_str = NULL;
    free (self->json);
    self->json = ret_str;

    if (asprintf (&ret_str, "%s \"total\" : \"%ld\"", self->json, cur->total) < 0)
        ret_str = NULL;
    free (self->json);
    self->json = ret_str;

    if (asprintf (&ret_str, "%s, \"used\" : \"%ld\"", self->json, cur->used) < 0)
        ret_str = NULL;
    free (self->json);
    self->json = ret_str;

    if (asprintf (&ret_str, "%s, \"buffer\" : \"%ld\"", self->json, cur->buffer) < 0)
        ret_str = NULL;
    free (self->json);
    self->json = ret_str;

    if (asprintf (&ret_str, "%s, \"cached\" : \"%ld\"", self->json, cur->cached) < 0)
        ret_str = NULL;
    free (self->json);
    self->json = ret_str;

    if (asprintf (&ret_str, "%s, \"user\" : \"%ld\"", self->json, cur->user) < 0)
        ret_str = NULL;
    free (self->json);
    self->json = ret_str;

    if (asprintf (&ret_str, "%s, \"shared\" : \"%ld\"", self->json, cur->shared) < 0)
        ret_str = NULL;
    free (self->json);
    self->json = ret_str;

    if (asprintf (&ret_str, "%s } ", self->json) < 0)
        ret_str = NULL;
    free (self->json);
    self->json = ret_str;


    return ret_str;
}

//  --------------------------------------------------------------------------
//  Fill net info

char *
zmon_gtop_proc_fill_net (zmon_gtop_proc_t *self)
{
    assert (self);

    char *ret_str = NULL;

    if (asprintf (&ret_str, "%s, \"net\" : { ", self->json) < 0)
        ret_str = NULL;
    free (self->json);
    self->json = ret_str;


    zmon_val_t *net_val = (zmon_val_t*) zhash_first (self->nets);
    while (net_val) {

        zmon_val_switch (net_val);

        glibtop_netload *cur = (glibtop_netload *) zmon_val_get_current (net_val);
        glibtop_netload *prev = (glibtop_netload *) zmon_val_get_previous (net_val);
        glibtop_get_netload (cur, zhash_cursor(self->nets));

        zclock_log ("%s cur=%ld", (char *)zhash_cursor(self->nets), cur->bytes_in - prev->bytes_in);

        if (asprintf (&ret_str, "%s \"%s\" : { ", self->json, (char *)zhash_cursor(self->nets)) < 0)
            ret_str = NULL;
        free (self->json);
        self->json = ret_str;

        if (asprintf (&ret_str, "%s \"bytes_in\" : \"%ld\"", self->json, cur->bytes_in - prev->bytes_in) < 0)
            ret_str = NULL;
        free (self->json);
        self->json = ret_str;

        if (asprintf (&ret_str, "%s, \"bytes_out\" : \"%ld\"", self->json, cur->bytes_out - prev->bytes_out) < 0)
            ret_str = NULL;
        free (self->json);
        self->json = ret_str;

        if (asprintf (&ret_str, "%s },", self->json) < 0)
            ret_str = NULL;
        free (self->json);
        self->json = ret_str;
        net_val = (zmon_val_t*) zhash_next (self->nets);
    }

    if (asprintf (&ret_str, "%s } ", self->json) < 0)
        ret_str = NULL;
    free (self->json);
    self->json = ret_str;

    return ret_str;
}



//  --------------------------------------------------------------------------
//  Updates monitoring information

void zmon_gtop_proc_update (zmon_gtop_proc_t *self)
{
    assert (self);

    zclock_log ("zmon_gtop_proc_update (pid=%d)", self->pid);

//    zmon_val_switch (self->mem);
//    zmon_val_switch (self->net);

    if (!self->pid) {

        free (self->json);

        if (asprintf (&self->json, "{ \"node\": { ") < 0)
            self->json = NULL;
        char *tmp = self->json;

        if (asprintf (&tmp, "%s \"hostname\": \"%s\",", self->json, zsys_hostname()) < 0)
            tmp = NULL;
        free (self->json);
        self->json = tmp;

        if (asprintf (&tmp, "%s \"time\": \"%ld\"", self->json, time(NULL)) < 0)
            tmp = NULL;
        free (self->json);
        self->json = tmp;

        if (asprintf (&tmp, "%s }", self->json) < 0)
            tmp = NULL;
        free (self->json);
        self->json = tmp;


        zmon_gtop_proc_fill_cpu (self);
        zmon_gtop_proc_fill_mem (self);
        zmon_gtop_proc_fill_net (self);

        if (asprintf (&tmp, "%s }", self->json) < 0)
            tmp = NULL;
        free (self->json);
        self->json = tmp;

        zmon_gtop_proc_dump(self);
    }
    else {

    }

    self->last_update = zclock_time ();

//    assert (cpu);
//    assert (memory);
//
//    float cpu_total_diff, cpu_used_diff, load;
//    float net_b_in_diff, net_b_out_diff;
//
//
//    if (!self->pid) {
//
//        // updating system monitoring info
//        if (!zmon_gtop_proc_cpu_total_last (self)) {
//            cpu_used_diff = 0;
//            cpu_total_diff = 0;
//            net_b_in_diff = 0;
//            net_b_out_diff = 0;
//
//        } else {
//            cpu_total_diff = cpu->total - zmon_gtop_proc_cpu_total_last (self);
//            cpu_used_diff = cpu->sys + cpu->user + cpu->nice  - zmon_gtop_proc_cpu_used_last (self);
//            if (net) {
//                net_b_in_diff = net->bytes_in;
//                net_b_out_diff = net->bytes_out;
//            }
//        }
//
////        self->cpu_sys = MIN (100 * cpu->sys / MAX (cpu_total_last, 1.0f), 100.0f);
////        self->cpu_user = MIN (100 * cpu->user / MAX (cpu_total_last, 1.0f), 100.0f);
////        self->cpu_nice = MIN (100 * cpu->nice / MAX (cpu_total_last, 1.0f), 100.0f);
//
//        self->cpu_usage = MIN (100 * cpu_used_diff / MAX (cpu_total_diff, 1.0f), 100.0f);
//        self->cpu_idle = 100.0f - self->cpu_usage;
//
//        self->mem_rss = (memory->used - memory->buffer - memory->cached);
//        self->mem_buffer = (memory->used - self->mem_rss - memory->cached);
//        self->mem_cache = (memory->used - memory->buffer - self->mem_rss);
//        self->mem_user = memory->user;
//        self->mem_shared = memory->shared;
//
//        self->mem_used = memory->used;
//        self->mem_free = memory->free;
//
//        if (net) {
//            self->net_bytes_in = net->bytes_in - net_b_in_diff;
//            self->net_bytes_out = net->bytes_out - net_b_out_diff;
//        }
//    } else {
//        // updating process monitoring info
//        glibtop_proc_time proctime;
//
//        glibtop_get_proc_time (&proctime, self->pid);
//        if (!zmon_gtop_proc_cpu_total_last (sys)) {
//            cpu_used_diff = 0;
//            cpu_total_diff = 0;
//        } else {
//            // updating system monitoring info
//            cpu_total_diff = cpu->total - zmon_gtop_proc_cpu_total_last (sys);
//            cpu_used_diff = proctime.rtime - zmon_gtop_proc_cpu_used_last (self);
//        }
//
//        //   load = used/total;
//        load = 100 * cpu_used_diff / MAX (cpu_total_diff, 1.0f);
//        load = MIN (load, 100.0f);
//
//        zmon_gtop_proc_set_cpu_total_last (self, cpu->total);
//        zmon_gtop_proc_set_cpu_used_last (self, proctime.rtime);
//
//        glibtop_proc_mem mem;
//        glibtop_get_proc_mem (&mem, self->pid);
//        self->cpu_usage = (float) load;
//        self->mem_rss = (float) (mem.rss);
//    }
}

//  --------------------------------------------------------------------------
//  Returns our pid

pid_t
zmon_gtop_proc_pid (zmon_gtop_proc_t *self)
{
    assert (self);
    return self->pid;
}

//  --------------------------------------------------------------------------
//  Selftest

int
zmon_gtop_proc_test (bool verbose)
{
    printf (" * zmon_gtop_proc: ");
    if (verbose)
        printf ("\n");

    //  @selftest
    //  Simple create/destroy test
    zmon_gtop_proc_t *self = zmon_gtop_proc_new (0);
    assert (self);

//    // FIXME memory leak (report bug to glibtop)
//    glibtop_init ();
//
//    glibtop_cpu cpu;
//    glibtop_mem memory;
////    glibtop_netload net;
////    char *net_interface = strdup ("wlp2s0");
//
//    for (int i = 0; i < 4; i++) {
//        glibtop_get_cpu (&cpu);
//        glibtop_get_mem (&memory);
////        glibtop_get_netload (&net, net_interface);
////        zmon_gtop_proc_update (self, &cpu, &memory, 0, 0);
//        if (verbose)
//            zmon_gtop_proc_dump (self);
//        zclock_sleep (500);
//        /* setting current values for total and used cpu */
//        zmon_gtop_proc_set_cpu_total_last (self, cpu.total);
//        zmon_gtop_proc_set_cpu_used_last (self, cpu.user + cpu.nice + cpu.sys);
//    }
//
//    glibtop_close ();

    zmon_gtop_proc_destroy (&self);

    if (verbose)
        printf (" * zmon_gtop_proc: ");

    printf ("OK\n");
    return 0;
}
