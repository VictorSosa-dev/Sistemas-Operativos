#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#define VALOR 500
// Generates and prints 'count' random 
// numbers in range [lower, upper]. 
void printRandoms(int lower, int upper) { 
    int i,total=0,num; 
    while(total < VALOR) { 
        num = (rand() % (upper - lower + 1)) + lower; 
        total += num;
        printf(" %d ", num);
    } 
    printf("Total :%d",total);
} 
  
// Driver code 
int main() 
{ 
    int lower = 0, upper = 2; 
  
    // Use current time as  
    // seed for random generator 
     srand(time(NULL)); 
  
    printRandoms(lower, upper); 
  
    return 0; 
} 