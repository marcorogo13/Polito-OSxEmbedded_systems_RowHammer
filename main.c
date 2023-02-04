

#include <stdio.h>
#include <string.h> 
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <stdint.h>



#define MEM_SIZE 10000
#define CYCLE_N 50000
#define CYCLE_X_REFRESH 50000
#define ROUND 1000
#define HAMMER_ROUND 200        // the number of cpu cycle between 2 subsequent memory refresh

int memory_block[MEM_SIZE] = {0};


/*utility functions for timing analysis*/
struct timespec timer_start(){
    struct timespec start_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    return start_time;
}

// call this function to end a timer, returning nanoseconds elapsed as a long
long timer_end(struct timespec start_time){
    struct timespec end_time;
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    long diffInNanos = (end_time.tv_sec - start_time.tv_sec) * (long)1e9 + (end_time.tv_nsec - start_time.tv_nsec);

    return diffInNanos;
}


int main(){
    /*setting the address to hammer*/
    void *int = (int*)&memory_block[0];
    memset(memory_block, 0xffff, MEM_SIZE);
    //size_t offset1 = (rand() << 12) % MEM_SIZE;
    void* addr1 = (void*) (memory_pool+MEM_SIZE/2);
    //size_t offset2 = (rand() << 12) % MEM_SIZE;
    void *addr2 = (void*) (memory_pool+MEM_SIZE/3);


    /*utility variables*/
    int selection = 1;
    int number_of_cycles = 1;
    int cycles;
    int found_flag = 0;
    unsigned long temp = 0xFFFFFFFF; 
    
    /*CACHE/NON CACHE timing tests*/
    printf("Timing of repeated access to same memory location using the cache:\n");
    
    uint32_t value=0;
    struct timespec start = timer_start();
    for (int j = 0; j < ROUND; ++j) {
        asm volatile ("ldr %0, [%1]\n\t" : "=r" (value) : "r" (addr1) );
        asm volatile ("ldr %0, [%1]\n\t" : "=r" (value) : "r" (addr2) );
    }
    long time_taken = timer_end(start);
    printf("cache access time: %ld ns\n", time_taken/ROUND);

    printf("Timing of repeated access to same memory location flushing the cache:\n");

    uint32_t temp=0;
    start = timer_start();
    for (int j = 0; j < ROUND; ++j) {
    asm volatile(
        "ldr %2, [%0]\n\t"
        "ldr %2, [%1]\n\t"
        "dc civac, %0\n\t"
        "dc civac, %1\n\t"
        ::"r" (addr1), "r" (addr2), "r" (temp)
        );
    }
    time_taken = timer_end(start);
    printf("memory access time: %ld ns\n", time_taken/ROUND);

    /*start of actual hammer program*/
    
    do{
        printf("Select what you want to try: \n1)ZVA \n2)DC CVAC \n3)DC CIVAC \n");
        scanf("%d", &selection); 
        if(selection != 1 || selection != 2 || selection != 3){
            printf("Wrong input, Please select a correct value\n");
        }        
    } while(selection != 1 || selection != 2 || selection != 3);

    printf("Enter number of refresh cycles you want to run the test for:\n");
    scanf("%d", &number_of_cycles); 
    
    
    //The number of effective cycle for the program to run, e.g. how many refresh cycles should we test
    cycles = HAMMER_ROUND * number_of_cycles;

    switch (selection)
    {
    case 1:
        for (j = 0; j < cycles; ++j) {
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
        for (j = 0; j < cycles; ++j) {
            asm volatile(
                "str %2, [%0]\n\t"
                "str %2, [%1]\n\t"
                "dc cvac, %0\n\t"
                "dc cvac, %1\n\t"
                ::"r" (addr1), "r" (addr2), "r" (temp)
          );
        }
        break;

    case 3:
        for (j = 0; j < cycles; ++j) {
            asm volatile(
                "str %2, [%0]\n\t"
                "str %2, [%1]\n\t"
                "dc civac, %0\n\t"
                "dc civac, %1\n\t"
                ::"r" (addr1), "r" (addr2), "r" (temp)
          );
        }            
         break;
    default: 
            printf("Invalid choice.\n");
            return 1;
        break;
    }
    
    /*test to see if there are any flipped bits*/
    
    for (int i = 0; i<MEM_SIZE; i++){
        if (memory_block[i] == 0){
            printf("Found a bit flip in position : %d, \n", i);
            found_flag ++;
        }
    
    }

    if (!found_flag){
        printf("No bit-flip found :( \n");
    } else{
        printf("%d bit flips found :).\n", found_flag);
    }


    return 0;
}
