/*  =========================================================================
    zmon_gtop - zmon gtop plugin

    Generated codec header for zmon_gtop
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

#ifndef __ZMON_GTOP_H_INCLUDED__
#define __ZMON_GTOP_H_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

//  Opaque class structure
typedef struct _zmon_gtop_t zmon_gtop_t;

//  @interface
//  Create a new zmon_gtop
CZMQ_EXPORT zmon_gtop_t *
    zmon_gtop_new ();

//  Destroy the zmon_gtop
CZMQ_EXPORT void
    zmon_gtop_destroy (zmon_gtop_t **self_p);

//  Print contents of message to stdout
CZMQ_EXPORT void
    zmon_gtop_dump (zmon_gtop_t *self);

//  Publish monitoring information to pub_socket
CZMQ_EXPORT void
    zmon_gtop_publish (zmon_gtop_t *self, void *to);

//  Do we monitor process
CZMQ_EXPORT int
   zmon_gtop_is_process(zmon_gtop_t *self, pid_t pid);

//  Add process
CZMQ_EXPORT void
    zmon_gtop_add_process(zmon_gtop_t *self, pid_t pid);

//  Remove process
CZMQ_EXPORT void
    zmon_gtop_remove_process(zmon_gtop_t *self, pid_t pid);

//  Destroy all subprocesses
CZMQ_EXPORT void
    zmon_gtop_processes_destroy(zmon_gtop_t *self);

//   Update monitoring information
CZMQ_EXPORT void
    zmon_gtop_update(zmon_gtop_t *self);

//  Self test of this class
CZMQ_EXPORT int
    zmon_gtop_test (bool verbose);
//  @end

#ifdef __cplusplus
}
#endif

#endif
