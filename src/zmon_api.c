/*  =========================================================================
    zmon_api - zmon api

    Generated codec implementation for zmon_api
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
    zmon_api - zmon api
@discuss
@end
*/

#include "../include/zmon.h"

//  Structure of our class

struct _zmon_api_t {
    int id;            // id description
};

//  --------------------------------------------------------------------------
//  Create a new zmon_api

zmon_api_t *
zmon_api_new (int id)
{
    zmon_api_t *self = (zmon_api_t *) zmalloc (sizeof (zmon_api_t));
    self->id = id;
    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the zmon_api

void
zmon_api_destroy (zmon_api_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        zmon_api_t *self = *self_p;
        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}

//  --------------------------------------------------------------------------
//  Print contents of message to stdout

void
zmon_api_dump (zmon_api_t *self)
{
    assert (self);
    printf( "id=%d\n", self->id);
}



//  --------------------------------------------------------------------------
//  Selftest

int
zmon_api_test (bool verbose)
{
    printf (" * zmon_api: ");

    //  @selftest
    //  Simple create/destroy test
    zmon_api_t *self = zmon_api_new (0);
    assert (self);
    zmon_api_destroy (&self);

    printf ("OK\n");
    return 0;
}
