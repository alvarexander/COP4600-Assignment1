// Program 1: First Come First Serve, Round Robin, Shortest Job First - Schedulers
// COP4600 - University of Central Florida
// Group 35: Brandon Bradley, Joy Weaver, Alexander Alvarez 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int processTicks(int tick)
{
    tick+=1;
    return tick;
}

void firstcomefirstServe(char *str)
{
    return;
}

void roundRobin (char *str)
{
    return;
}

void shortestjobFirst(char *str)
{
    return;
}

void processFile(char *str)
{

}

int main (void)
{
    FILE *input_file;
    char buffer [1000];
    int flag = 0, i = 0;
    int size;

    if(buffer == NULL)
    {
        printf("Error: Memory allocation failed\n.");
        exit (1);
    }
    // Take in the input file
    input_file = fopen("processes.in", "r");

    if(input_file == NULL)
    {
        printf("Error: File missing or corrupted\n.");
        exit (1);
    }

     // Take in the text one character at a time.
    while((fscanf(input_file,"%c",&buffer[i]))!=EOF) //scanf and check EOF
    {
        i++;
    }

    fclose(input_file);

    processFile(buffer);
   
    if(flag == 0)
    {
        firstcomefirstServe(buffer);
    }

    else if(flag == 1)
    {
        roundRobin(buffer);
    }

    else
        shortestjobFirst(buffer);

    return 0;
}
