#include <stdio.h>
#include <string.h> 
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>



#define MEM_SIZE = 10000;
#define CYCLE_N = 50000;

int memory_block[MEM_SIZE] = {0};



int main(){

    void *int = (int*)&memory_block[0];

    int selection = 1;
    int found_flag = 0;



    unsigned long temp = 0xFFFFFFFF; 

    printf("Select the cache invalidating instruction: \n 1)ZVA \n2)DC CVAC \n3)DC CIVAC \n");

    scanf("%d", &selection); 

    switch (selection)
    {
    case 1:
        for (j = 0; j < HAMMER_ROUND; ++j) {
            asm volatile(
                "str %2, [%0]\n\t"
                "str %2, [%1]\n\t"
                "dc zva, %0\n\t"
                "dc zva, %1\n\t"
                ::"r" (addr1), "r" (addr2), "r" (temp)
          );
        }
        break;
    

    case 2:
        for (j = 0; j < HAMMER_ROUND; ++j) {
            asm volatile(
                "str %2, [%0]\n\t"
                "str %2, [%1]\n\t"
                "dc zva, %0\n\t"
                "dc zva, %1\n\t"
                ::"r" (addr1), "r" (addr2), "r" (temp)
          );
        }
        break;

    case 3:
         break;
    default:
        break;
    }
    
    
    for (int i = 0; i<MEM_SIZE; i++){
        if (memory_block[i] == 1){
            printf("Found a bit flip in position : %d, \n", i);
            found_flag = 1;
        }
    
    }

    if (!found_flag){
        printf("No bit flip found :( \n");
    }


    return 0;
}
