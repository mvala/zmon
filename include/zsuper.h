/*  =========================================================================
    zsuper - manager of zactors

    Generated codec header for zsuper
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

#ifndef __ZSUPER_H_INCLUDED__
#define __ZSUPER_H_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

//  Opaque class structure
typedef struct _zsuper_t zsuper_t;

//  @interface
//  Create a new zsuper
CZMQ_EXPORT zsuper_t *
    zsuper_new (int id);

//  Destroy the zsuper
CZMQ_EXPORT void
    zsuper_destroy (zsuper_t **self_p);

//  Print contents of message to stdout
CZMQ_EXPORT void
    zsuper_dump (zsuper_t *self);

//  Self test of this class
CZMQ_EXPORT int
    zsuper_test (bool verbose);
//  @end

#ifdef __cplusplus
}
#endif

#endif
