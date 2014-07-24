/*  =========================================================================
    zmon_val - zmon values

    Generated codec implementation for zmon_val
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
    zmon_val - zmon values
@discuss
@end
*/

#include "../include/zmon.h"

//  Structure of our class

struct _zmon_val_t {
    void *current;           //  Current pointer of values
    void *previous;          //  Previous pointer of values
};

//  --------------------------------------------------------------------------
//  Create a new zmon_val

zmon_val_t *
zmon_val_new ()
{
    zmon_val_t *self = (zmon_val_t *) zmalloc (sizeof (zmon_val_t));
    self->current = NULL;
    self->previous = NULL;
    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the zmon_val
//  Note: zmon_val is destroying pointers

void
zmon_val_destroy (zmon_val_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        zmon_val_t *self = *self_p;

        free (self->current);
        free (self->previous);

        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}

//  --------------------------------------------------------------------------
//  Print contents of message to stdout

void
zmon_val_dump (zmon_val_t *self)
{
    assert (self);
}

//  --------------------------------------------------------------------------
//  Sets value of current pointer

void
zmon_val_set_current (zmon_val_t *self, void *val)
{
    assert (self);
    self->current = val;
}

//  --------------------------------------------------------------------------
//  Sets value of previous pointer

void
zmon_val_set_previous (zmon_val_t *self, void *val)
{
    assert (self);
    self->previous = val;
}

//  --------------------------------------------------------------------------
//  Gets current value
void *
zmon_val_get_current (zmon_val_t *self)
{
    assert (self);
    return self->current;
}

//  --------------------------------------------------------------------------
//  Gets previous value

void *
zmon_val_get_previous (zmon_val_t *self)
{
    assert (self);
    return self->previous;
}

//  --------------------------------------------------------------------------
//  Switch pointers

void
zmon_val_switch (zmon_val_t *self)
{
    assert (self);
    void *tmp = self->previous;
    self->previous = self->current;
    self->current = tmp;
}


//  --------------------------------------------------------------------------
//  Selftest

int
zmon_val_test (bool verbose)
{
    printf (" * zmon_val: ");

    //  @selftest
    //  Simple create/destroy test
    zmon_val_t *self = zmon_val_new ();
    assert (self);

//    void *my_val = zmalloc (sizeof (glibtop_cpu));
//    assert (!zmon_val_set (self, my_val));
//
//    void *my_val2 = zmalloc (sizeof (glibtop_mem));
//    assert (zmon_val_set (self, my_val2) == my_val);
//    assert (zmon_val_set (self, my_val) == my_val2);

    zmon_val_destroy (&self);

    printf ("OK\n");
    return 0;
}
