/*  =========================================================================
    zmon_val - zmon values

    Generated codec header for zmon_val
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

#ifndef __ZMON_VAL_H_INCLUDED__
#define __ZMON_VAL_H_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

//  Opaque class structure
typedef struct _zmon_val_t zmon_val_t;

//  @interface
//  Create a new zmon_val
CZMQ_EXPORT zmon_val_t *
    zmon_val_new ();

//  Destroy the zmon_val
CZMQ_EXPORT void
    zmon_val_destroy (zmon_val_t **self_p);

//  Sets value of previous pointer
CZMQ_EXPORT void
    zmon_val_set_current (zmon_val_t *self, void *val);

//  Sets value of previous pointer
CZMQ_EXPORT void
    zmon_val_set_previous (zmon_val_t *self, void *val);

//  Gets current value
CZMQ_EXPORT void *
    zmon_val_get_current (zmon_val_t *self);

//  Gets previous value
CZMQ_EXPORT void *
    zmon_val_get_previous (zmon_val_t *self);

//  Switch pointers
CZMQ_EXPORT void
    zmon_val_switch (zmon_val_t *self);

//  Print contents of message to stdout
CZMQ_EXPORT void
    zmon_val_dump (zmon_val_t *self);

//  Self test of this class
CZMQ_EXPORT int
    zmon_val_test (bool verbose);
//  @end

#ifdef __cplusplus
}
#endif

#endif
