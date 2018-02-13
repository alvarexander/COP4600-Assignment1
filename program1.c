// Program 1: First Come First Serve, Round Robin, Shortest Job First - Schedulers
// COP4600 - University of Central Florida
// Group 35: Brandon Bradley, Joy Weaver, Alexander Alvarez 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int tick(int time_tick, int flag, int burst)
{
    if(flag == 0)
    {
         time_tick+=1;
         return time_tick;
    }
    
    if(flag == 1)
    {
        time_tick+=burst;
        return time_tick;       
    }
   
}
// Alexander Alvarez
void firstcomefirstServe(char *info)
{
   
    return;
}
// Joy
void roundRobin (char *info)
{
    return;
}
// Brandon
void shortestjobFirst(char *info)
{
    return;
}

char processFile(char *buffer, int size)
{
    int i, x = 0, y = 0, flag = 0;
    char infobuff[500], info[500];

    //parse out comments    
    for(i = 0; i < size; i++)
    {
        if(buffer[i] == '#')
        {
            while(buffer[i] != '\n')
            {
                i++;
            }
        }

        // Take out the comments and store in strong
           infobuff[x++] = buffer[i];

        // Parse the string for only relevant information
        // ProcessCount
        if(buffer[i] == 't' && buffer[i+1] == ' ')
        {
            info[y++] = buffer[i+2];
            info[y++] = '\n';
        } 
        // Arrival Time
        if (buffer [i] == 'l' && buffer[i+1] == ' ')
        {
            info[y++] = buffer[i+2];
            info[y++] = '\n';
        }
        //Quantum
        if(buffer[i] == 'm' && buffer[i+1] == ' ')
        {
            info[y++] = buffer[i+2];
            info[y++] = '\n';
        }
        // Use
        if(buffer[i] == 'e'&& buffer[i-1] == 's' && buffer[i+1] == ' ')
        {
            info[y++] = buffer[i+2];
            info[y++] = '\n';

            if(info[y-2] == 's')
                flag = 1;
            else if(info[y-2] == 'f')
                flag = 2;
        }
        //Process name
        if(buffer[i] == 'e' && buffer[i-1] == 'm' && buffer[i+1] == ' ')
        {
            i+=2;
            while(buffer[i] != ' ')
            {
                info[y++] = buffer[i++];

            }
            info[y++] = '\n';
        }

        if(buffer[i] == 'r'&& buffer[i-1] == 'o' && buffer[i+1] == ' ')
        {
             info[y++] = buffer[i+2];
             info[y++] = buffer[i+3];
             info[y++] = '\n';
        }    
    }
    
    if(flag == 0)
        roundRobin(info);
    
    else if(flag == 1)
        shortestjobFirst(info);
    
    else
        firstcomefirstServe(info);



   // for(i = 0; i < strlen(info);i++)
   // printf("%c", info[i]);
    

    return *info;
}

int main (void)
{
    FILE *input_file;
    char buffer [1000];
    int flag = 0, i = 0;
    int size = 0;

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

    size = strlen(buffer);
   
    processFile(buffer,size);
   

    return 0;
}
