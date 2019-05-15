//spencer jackson
//tikloo toolkit
 
//tk_test.c unit tests on functions that I don't trust myself on.

#include<string.h>
#include"tikloo/tk.h"
#include"tikloo/tk_test.h"

void test(uint8_t pass, char* testname)
{
    printf("%s: %s\n",testname,pass?"pass":"FAILED!");
}

uint16_t listlen(uint16_t *list)
{
    uint16_t i;
    for(i=0;list[i];i++);
    return i;
}

uint8_t listcontains(uint16_t *list, uint16_t n)
{
    uint16_t i;
    for(i=0;list[i];i++)
        if(list[i]==n)
            return 1;
    return 0;
}
uint16_t listindex(uint16_t *list, uint16_t n)
{
    uint16_t i;
    for(i=0;list[i];i++)
        if(list[i]==n)
            return i;
    return (uint16_t)-1;//it shouldn't ever get here
}

uint8_t listcompare(uint16_t *list1, uint16_t *list2)
{
    uint16_t i;
    printf("list missing: ");
    for(i=0;list1[i];i++)
        if(!listcontains(list2,list1[i]))
            printf("%i, ",list1[i]);
    printf("\nlist has extra: ");
    for(i=0;list2[i];i++)
        if(!listcontains(list1,list2[i]))
            printf("%i, ",list2[i]);
    printf("\n");
    return 0;
}

void testlists()
{
    uint16_t list[40] = {48,1,2,3,4,5,6,7,8,9,10,11,40,41,53,99,17,23,88,55,56,54,52,51,49,0,0,0,0,0,0,0,0,0,0,0,0,0};
    uint16_t list2[40];
   
    memcpy(list2,list,sizeof(uint16_t)*40);

    printf("list len %i\n",listlen(list));

    test(listcontains(list,12)-1,"Not in list");
    tk_addtolist(list,12);
    test(listcontains(list,12),"Now in list");

    tk_addtolist(list,12);
    test(listcontains(list,12),"Added again");

    tk_removefromlist(list,12);
    test(listcontains(list,12)-1,"Removed");

    tk_insertinlist(list,12,12);
    test(listcontains(list,12),"Inserted");

    tk_removefromlist(list,12);
    test(listcontains(list,12)-1,"Removed again"); 

    tk_insertinlist(list,12,14);
    printf("list len %i\n",listlen(list));
    printf("index %i\n",listindex(list,12));
    tk_insertinlist(list,12,14);
    printf("list len %i\n",listlen(list));
    printf("index %i\n",listindex(list,12));
    test(listcontains(list,12),"Inserted 2x");
    tk_removefromlist(list,12);
    printf("list len %i\n",listlen(list));
    printf("index %i\n",listindex(list,12));
    test(listcontains(list,12)-1,"Removed again");
    tk_removefromlist(list,12);
    printf("list len %i\n",listlen(list));
    test(listcontains(list,12)-1,"Removed second time");

    tk_insertinlist(list,12,14);
    printf("index %i\n",listindex(list,12));
    tk_insertinlist(list,12,18);
    printf("index %i\n",listindex(list,12));
    test(listcontains(list,12),"Inserted 2x (different spots)");
    listcompare(list,list2);
    tk_removefromlist(list,12);
    test(listcontains(list,12)-1,"Removed again");

    tk_insertinlist(list,12,19);
    printf("index %i\n",listindex(list,12));
    tk_insertinlist(list,12,2);
    printf("index %i\n",listindex(list,12));
    test(listcontains(list,12),"Inserted 2x (different spots)");
    tk_removefromlist(list,12);
    test(listcontains(list,12)-1,"Removed again");

    tk_insertinlist(list,12,0);
    printf("index %i\n",listindex(list,12));
    test(listcontains(list,12),"Inserted to 0");
    tk_removefromlist(list,12);
    test(listcontains(list,12)-1,"Removed again");

    tk_insertinlist(list,12,listlen(list));
    printf("index %i\n",listindex(list,12));
    test(listcontains(list,12),"Inserted to end");
    tk_removefromlist(list,12);
    test(listcontains(list,12)-1,"Removed again"); 
}

int main()
{ 
    testlists();
    return 0; 
}
