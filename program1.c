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
    
      printf("SJF Called.\n");

    //i=0 -- guaranteed to be processCount from processFile function call
    int processCount = atoi(&info[0]);

    //i=1 -- guaranteed to be timeUnits
    int numTimeUnits = atoi(&info[1]);

    printf("%i Processes\n", processCount);
    printf("Using Shortest Job First (Pre)\n\n");
    //printf("Time Units is: %i\n", numTimeUnits);

    //Create the new tokenized array
    int arrayLen = 3+(3*processCount);
    char * array[arrayLen];
    int count = 0;

    array[count] = strtok(info,"\n\r");

    while(array[count] != NULL)
    {
       array[++count] = strtok(NULL,"\n\r");
    }

    for (int i = 0; i < count; i++) {

        printf("%s\n", array[i]);
    }

    //i=2 -- useAlgo, has already been processed. Ignore.
    //Quantum, has been removed for SJF.

    //i=3 -- guaranteed to be processName, and every i=3+3 for each processCount

    //i=4 -- guaranteed to be arrivalTime, and every i=4+3 for each processCount

    //i=5 -- guaranteed to be burstTime, and every i=5+3 for each processCount

    //Store Processes in new array
    //Logical representation Arr-Burst - [arrivalTime0][burstTime0][arrivalTime1][burstTime1][arrivalTimeN][burstTimeN]
    //Logical representation ProNames -  [P* i=0 n=0 ][P* i=2 n=1 ][P* i=4 n=2  ][P* i=6 n=3]
    //We need to keep track of the process names and corresponding locations for output
    //We can associate each arrivalTime with process name in a char array. So processNames[i] = processes[i/2] where i is count
    //Create the new arrays and counts
    char * processNames[processCount];
    int processes[2*processCount];
    int processesCount = 0;
    int processNameCount = 0;

    //Store the data in the new arrays
    for (int i = 4; i < arrayLen; i+=3) {

        processNames[processNameCount] = array[i-1];
        processNameCount++;
        processes[processesCount] = atoi(array[i]);
        processesCount++;
        processes[processesCount] = atoi(array[i+1]);
        processesCount++;

    }

    //TODO: REMOVE --------------------------------------
    for (int i = 0; i < (2*processCount); i++) {

          if (i % 2 == 0) {
            printf("Arrival Time: %i\n", processes[i]);
          }
          else printf("Burst Time: %i\n", processes[i]);

    }

    for (int i = 0; i < processCount; i++) {

          printf("Process Name: %s\n", processNames[i]);

    }

    ///--------------------------------------------------

    //We are ready for the SJF Preemptive Scheduler
    //Need init time at 0
    int currentTime = 0;

    //Always need the current shortest job, start at -1 until we get the first process
    int currentShortestJobIndex = -1;

    //Need to know when the first job is selected
    int selectedFirstJob = 0;

    //Need to know when all processes are finished
    int numProcessesFinished = 0;

    //Need to know if the scheduler is idle BETWEEN processes after selecting first process
    int numProcessesArrived = 0;

    //Need to calculate the wait and turnaround times

    //Scheduler paraphrase follows
    //At each time unit where the arrival time is LEQ the currentTime
    //Check for the shortest Job, check arrival time LEQ currentTime and burstTime GTR 0
    //Also check that shortest job burstTime != 0, which represents finished processes

    //Edge cases:
    //Two or more processes arrive at the same time
    //Idle time between arrival of processes
    //NOTE: Guaranteed there will not be identical burst times for SJF input (PASSED)

    while (currentTime != numTimeUnits) {

        //Set the shortest job index as max int each loop to reset and guarantee SJF
        int shortestJobIndex = INT_MAX;

        //We are checking each processes arrival time and burstTime on each iteration
        for (int i = 0; i < (2*processCount); i+=2) {

                //printf("i iterator is currently: %i\n", i);
                //printf("Current time: %i\n", currentTime);
                //printf("Arrival time of checked process: %i\n", processes[i]);

                //The arrival time is equal to the current time -- this will only occur once before selection, output Time N: STRING arrived
                if (currentTime == processes[i]) {

                    printf("Time %i: %s arrived\n", currentTime, processNames[(i/2)]);

                    selectedFirstJob = 1;

                    numProcessesArrived++;

                    //getchar();
                }

                //In this case we are idling this time unit,
                if (selectedFirstJob && numProcessesArrived == 0) {

                    //Output Time N: IDLE
                    printf("Time %i: IDLE\n", currentTime);

                    //We are done with the current time unit
                    currentTime++;

                }

                //We only start selecting once the first process arrives AND we should not be idling
                if (selectedFirstJob && numProcessesArrived > 0) {

                        //Find the shortest job in the array
                        for (int j = 0; j < (2*processCount); j+=2) {

                            //The arrival time is less than or equal to the current time, qualifies to have burstTime checked
                            if (processes[j] <= currentTime) {

                                //This burst time is the current lowest and not already finished
                                if (processes[j+1] < shortestJobIndex && processes[j+1] != 0) {

                                    //We are switching jobs or setting the shortestJobIndex for the first time
                                    if ((currentShortestJobIndex != shortestJobIndex) || currentShortestJobIndex == -1) {

                                        //Update the shortest job
                                        shortestJobIndex = j+1;

                                    }
                                }
                            }
                        }

                        //We now have the shortestJobIndex
                        //If we are switching jobs, output selection
                        if ((currentShortestJobIndex != shortestJobIndex)) {

                            //This will keep its value for the next iteration
                            currentShortestJobIndex = shortestJobIndex;

                            //Output Time N: STRING selected (burst PROCESSES[j+1])
                            printf("Time %i: %s selected (burst %i)\n", currentTime, processNames[(shortestJobIndex-1)/2], processes[shortestJobIndex]);

                            //getchar();
                        }

                        //Get the current burst time of the selected job
                        int curBurstTime = processes[shortestJobIndex];

                        //printf("The burstTime of the currently selected process is: %i\n", curBurstTime);

                        //This is the end for this process, update and output Time N: STRING finished
                        if (curBurstTime == 1) {

                            //Decrement the current processes burst time and store it in the array
                            curBurstTime--;
                            processes[shortestJobIndex] = curBurstTime;

                            //We are done with the current time unit
                            currentTime++;

                            //Remember index is burst time, so minus one to get the arrival time and i/2 for corresponding process name
                            printf("Time %i: %s finished\n", currentTime, processNames[((shortestJobIndex-1)/2)]);

                            //Increment the number of processes finished
                            numProcessesFinished++;

                            //Decrement the number of arrived processes
                            numProcessesArrived--;

                            //getchar();
                        }

                        //If the burst time is greater than zero decrement it
                        else if (curBurstTime > 0) {

                            //Decrement the current processes burst time and store it in the array
                            curBurstTime--;
                            processes[shortestJobIndex] = curBurstTime;

                            //printf("The new burstTime of the currently selected process is %i\n", processes[shortestJobIndex]);

                            //We are done with the current time unit
                            currentTime++;

                            //getchar();
                        }

                        //Scheduler is idle this time unit as ALL processes are finished, output
                        if (numProcessesFinished == processCount) {

                          //Output Time N: IDLE
                          printf("Time %i: IDLE\n", currentTime);

                          //We are done with the current time unit
                          currentTime++;

                          //getchar();

                        }
            }

        }
    }

    //SJF Scheduler is done, output
    printf("Finished at time %i\n", numTimeUnits);

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

        //Run for
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
    int i = 0, size = 0;
    
    figures processes[100]; // An array of processes based on data structure/class

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
