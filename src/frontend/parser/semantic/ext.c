#include "ext.h"
#include <stdlib.h>

void ext_def_semantic(program_manager *pm, ASTNode *ExtDef)
{
    // TODO: Implement this function

}

id_name_and_sizes *ext_dec_list_semantic(program_manager *pm, ASTNode *ExtDecList)
{
    if(alist_count(ExtDecList->children) == 1) 
    {
        return vardec_semantic(pm, alist_get(ExtDecList->children, 0));
    }
    else 
    {
        id_name_and_sizes *ins = vardec_semantic(pm, alist_get(ExtDecList->children, 0));
        id_name_and_sizes *ins2 = ext_dec_list_semantic(pm, alist_get(ExtDecList->children, 2));
        for(int i=0;i<ins2->size_count;i++) 
        {
            id_name_and_sizes_push_size(ins, ins2->sizes[i]);
        }
        id_name_and_sizes_free(ins2);
        return ins;
    }
}



