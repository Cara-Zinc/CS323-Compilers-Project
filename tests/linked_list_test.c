#include <stdio.h>
#include <stdlib.h>
#include "../src/frontend/defs_all.h"

int main()
{
    // init 3 lists for local variables, global variables, and struct definitions
    list *var_list = list_new(field_def);
    list *global_list = list_new(readonly_field_def);
    list *struct_defs = list_new(struct_def);
    list *func_defs = list_new(func_def);

    // use case 1: Scrum Team Management
    // generate some variables of different types
    field_def *var1 = new(field_def);
    var1->name = "team_name";
    var1->type = TYPE_CHAR;
    var1->is_array = true;
    // say your team name is "scrum_team_management", we will have char[21]
    var1->array_size = 21;
    list_add(var_list, var1);

    field_def *var2 = new(field_def);
    var2->name = "team_size";
    var2->type = TYPE_INT;
    list_add(var_list, var2);

    field_def *var3 = new(field_def);
    var3->name = "contributions";
    var3->type = TYPE_FLOAT;
    var3->is_array = true;
    var3->array_size = 32;
    list_add(var_list, var3);

    // generate some global variables
    readonly_field_def *global1 = new(readonly_field_def);
    global1->name = "MAX_TEAM_SIZE";
    global1->type = TYPE_INT;
    int max_team_size = 3;
    global1->value = &max_team_size;

    readonly_field_def *global2 = new(readonly_field_def);
    global2->name = "MAX_CONTRIBUTIONS";
    global2->type = TYPE_FLOAT;
    float max_contributions = 1.0;
    global2->value = &max_contributions;


    // generate a struct definition for team members
    struct_def *team_member = new_struct_def("team_member");
    /*
    struct team_member {
        char[] name;
        int age;
        float contributions[32];
        void greet() {
            printf("Hello, my name is %s, I'm %d years old, and I've made %.2f contributions to the team.\n", name, age, contributions);
        }
    }
    */
    field_def *member_name = new(field_def);
    member_name->name = "name";
    member_name->type = TYPE_CHAR;
    member_name->is_array = true;
    // say your team member name is "233333333333333333333333"
    member_name->array_size = 24;

    field_def *member_age = new(field_def);
    member_age->name = "age";
    member_age->type = TYPE_INT;
    member_age->is_array = false;

    field_def *member_contributions = new(field_def);
    member_contributions->name = "contributions";
    member_contributions->type = TYPE_FLOAT;
    member_contributions->is_array = false;
    
    func_def *member_greet = new(func_def);
    member_greet->name = "greet";
    member_greet->return_type = TYPE_VOID;
    member_greet->args = list_new(field_def);
    list_add(member_greet->args, member_name);


}