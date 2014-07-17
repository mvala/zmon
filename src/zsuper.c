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
    int id;            // id description
};

//  --------------------------------------------------------------------------
//  Create a new zsuper

zsuper_t *
zsuper_new (int id)
{
    zsuper_t *self = (zsuper_t *) zmalloc (sizeof (zsuper_t));
    self->id = id;
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
    printf( "id=%d\n", self->id);
}



//  --------------------------------------------------------------------------
//  Selftest

int
zsuper_test (bool verbose)
{
    printf (" * zsuper: ");

    //  @selftest
    //  Simple create/destroy test
    zsuper_t *self = zsuper_new (0);
    assert (self);
    zsuper_destroy (&self);

    printf ("OK\n");
    return 0;
}
