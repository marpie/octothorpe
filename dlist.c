/*
 *             _        _   _                           
 *            | |      | | | |                          
 *   ___   ___| |_ ___ | |_| |__   ___  _ __ _ __   ___ 
 *  / _ \ / __| __/ _ \| __| '_ \ / _ \| '__| '_ \ / _ \
 * | (_) | (__| || (_) | |_| | | | (_) | |  | |_) |  __/
 *  \___/ \___|\__\___/ \__|_| |_|\___/|_|  | .__/ \___|
 *                                          | |         
 *                                          |_|
 *
 * Written by Dennis Yurichev <dennis(a)yurichev.com>, 2013
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/3.0/.
 *
 */

#include <assert.h>
#include "dlist.h"
#include "dmalloc.h"

dlist *dlist_init()
{
    return DCALLOC(dlist, 1, "dlist");
};

void dlist_insert_at_begin(dlist *l, void *data)
{
    assert(l);
    if (l->data==NULL && l->prev==NULL && l->next==NULL)
    {
        l->data=data;
        return;
    };
    dlist *old_head=DCALLOC(dlist, 1, "dlist");
   
    old_head->next=l->next;
    old_head->data=l->data;
    old_head->prev=l;

    // fill current list head
    l->data=data;
    l->next=old_head;
    l->prev=NULL;
    //printf ("%s() after inserting\n", __func__);
    //dlist_dump(l);
};

void* dlist_get_first(dlist *l)
{
    return l->data;
};

void dlist_free(dlist *l, void (*free_fn)(void*))
{
    if (l==NULL)
        return;
    
    for (dlist *i=l; i;)
    {
        dlist *tmp=i->next;
        if (free_fn)
            (*free_fn)(i->data);
        DFREE(i);
        i=tmp;
    };
};

void dlist_unlink(dlist **lst, dlist *l)
{
    dlist *right=l->next;

    if (*lst==l) // special case: first element is to be unlinked
    {
        if (right) // there are at least two elements
        {
            right->prev=NULL;
            *lst=right;
        }
        else // only sole element is not be unlinked
            *lst=NULL; // no more elements!
    }
    else
    {
        dlist *left=l->prev;

        if (left)
        {
            assert (left->next==l); // just to be sure links were correct
            left->next=right;
        };

        if (right)
        {
            assert (right->prev==l); // just to be sure links were correct
            right->prev=left;
        };
    };

    DFREE(l);
};

void dlist_dump(dlist *l)
{
    if (l==NULL)
        return;
    printf ("%s(0x%p) prev=0x%p next=0x%p data=0x%p\n", __func__, l, l->prev, l->next, l->data);
    if (l->next)
        dlist_dump(l->next);
};

/* vim: set expandtab ts=4 sw=4 : */
