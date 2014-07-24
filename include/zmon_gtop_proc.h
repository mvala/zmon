/*  =========================================================================
    zmon_gtop_proc - zmon gtop pid monitor

    Generated codec header for zmon_gtop_proc
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

#ifndef __ZMON_GTOP_PROC_H_INCLUDED__
#define __ZMON_GTOP_PROC_H_INCLUDED__

#include <glibtop/mem.h>
#include <glibtop/cpu.h>
#include <glibtop/netload.h>

#ifdef __cplusplus
extern "C" {
#endif

//  Opaque class structure
typedef struct _zmon_gtop_proc_t zmon_gtop_proc_t;

//  @interface
//  Create a new zmon_gtop_proc
CZMQ_EXPORT zmon_gtop_proc_t *
    zmon_gtop_proc_new (pid_t pid);

//  Destroy the zmon_gtop_proc
CZMQ_EXPORT void
    zmon_gtop_proc_destroy (zmon_gtop_proc_t **self_p);

//  Updates monitoring information
CZMQ_EXPORT void
    zmon_gtop_proc_update (zmon_gtop_proc_t *self);

//  Fill cpu info
CZMQ_EXPORT char *
    zmon_gtop_proc_fill_cpu (zmon_gtop_proc_t *self);

//  Fill mem info
CZMQ_EXPORT char *
    zmon_gtop_proc_fill_mem (zmon_gtop_proc_t *self);

//  Fill net info
CZMQ_EXPORT char *
    zmon_gtop_proc_fill_net (zmon_gtop_proc_t *self);


//  Returns pid
CZMQ_EXPORT pid_t
    zmon_gtop_proc_pid (zmon_gtop_proc_t *self);

////  Returns CPU used last
//CZMQ_EXPORT guint64
//    zmon_gtop_proc_cpu_used_last(zmon_gtop_proc_t *self);
//
////  Sets CPU used last
//CZMQ_EXPORT void
//    zmon_gtop_proc_set_cpu_used_last(zmon_gtop_proc_t *self, guint64 cpu_used_last);
//
////  Returns CPU total last
//CZMQ_EXPORT guint64
//    zmon_gtop_proc_cpu_total_last(zmon_gtop_proc_t *self);
//
////  Sets CPU total last
//CZMQ_EXPORT void
//    zmon_gtop_proc_set_cpu_total_last(zmon_gtop_proc_t *self, guint64 cpu_total_last);
//
////  Returns CPU usage
//CZMQ_EXPORT float
//    zmon_gtop_proc_cpu(zmon_gtop_proc_t *self);
//
////  Returns RSS mem
//CZMQ_EXPORT float
//    zmon_gtop_proc_mem_rss(zmon_gtop_proc_t *self);

//  Print contents of message to stdout
CZMQ_EXPORT void
    zmon_gtop_proc_dump (zmon_gtop_proc_t *self);

//  Self test of this class
CZMQ_EXPORT int
    zmon_gtop_proc_test (bool verbose);
//  @end

#ifdef __cplusplus
}
#endif

#endif
