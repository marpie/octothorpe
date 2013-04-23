#include "string_list.h"
#include "dmalloc.h"

void main()
{
    strlist_node *l=NULL;
    
    strlist_insert_at_begin (&l, DSTRDUP("1st", "string"));
    strlist_insert_at_begin (&l, DSTRDUP("2nd", "string"));
    strlist_insert_at_begin (&l, DSTRDUP("3rd", "string"));

    printf ("strlist_get_first=%s\n", strlist_get_first(l));

    strlist_free(l, dfree);
    
    dump_unfreed_blocks();
};