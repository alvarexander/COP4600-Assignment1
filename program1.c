// Program 1: First Come First Serve, Round Robin, Shortest Job First - Schedulers
// COP4600 - University of Central Florida
// Group 35: Brandon Bradley,Alexander Alvarez 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

//Queue struct for Round Robin scheduler
struct rrQueue
{
	int front;
	int rear;
	int size;
	unsigned capacity;
	int * array;
};

//Create the rrQueue array
struct rrQueue * createrrQueue(unsigned capacity)
{
	struct rrQueue* rrQueue = (struct rrQueue*) malloc(sizeof(struct rrQueue));
	rrQueue->capacity = capacity;
	rrQueue->front = 0;
	rrQueue->size = 0;
	rrQueue->rear = capacity - 1;
	rrQueue->array = (int*)malloc(rrQueue->capacity * sizeof(int));
	return rrQueue;
}
//When queue size is equal to capacity, the queue is full
int isrrQueueFull(struct rrQueue * rrQueue) 
{

	return (rrQueue->size == rrQueue->capacity);

}

//When the queue size is 0, the queue is empty
int isrrQueueEmpty(struct rrQueue * rrQueue)
{
	return (rrQueue->size == 0);
}

//Adds a process to the rrQueue
void enqueue(struct rrQueue* rrQueue, int processIndex)
{
	if (isrrQueueFull(rrQueue))
	{
		return;
	}
	rrQueue->rear = (rrQueue->rear + 1) % rrQueue->capacity;
	rrQueue->array[rrQueue->rear] = processIndex;
	rrQueue->size = rrQueue->size + 1;
	//printf("%d enqueued to rrQueue\n", processIndex);

}

//Removes an item from the rrQueue
int dequeue(struct rrQueue * rrQueue)
{
	if (isrrQueueEmpty(rrQueue))
	{
		return INT_MIN;
	}

	int processIndex = rrQueue->array[rrQueue->front];
	rrQueue->front = (rrQueue->front + 1) % rrQueue->capacity;
	rrQueue->size = rrQueue->size - 1;
	return processIndex;
}

//Returns the front of rrQueue
int front(struct rrQueue * rrQueue)
{
	if (isrrQueueEmpty(rrQueue))
	{
		return INT_MIN;
	}

	return rrQueue->array[rrQueue->front];
}

//Returns the rear of rrQueue
int rear(struct rrQueue * rrQueue)
{
	if (isrrQueueEmpty(rrQueue))
	{
		return INT_MIN;
	}

	return rrQueue->array[rrQueue->rear];
}

// Alexander Alvarez
// Takes in string of required information obtained from file and simulates First Come First Serve scheduling
// Outputs "processes.out"
void firstcomefirstServe(char *info)
{
	FILE *ofp;
	int size = 0, count = 0, currTime = 0, arrived = 0, completed = 0, firstSelection = 0;

	int processCount = 0, timeUnits = 0;

	ofp = fopen("processes.out", "w");

	// The processed string info will contain the processcout, timeunits, and quantum (if applicable) in that order
	processCount = atoi(&info[0]);

	timeUnits = atoi(&info[1]);


	fprintf(ofp, "%d processes\n", processCount);
	fprintf(ofp, "Using First Come First Served\n");
	fprintf(ofp, "\n");
	size = 3 + (3 * processCount);

	char * buffer[size];

	buffer[count] = strtok(info, "\n");

	while (buffer[count] != NULL)
	{
		buffer[++count] = strtok(NULL, "\n");
	}


	char * processNames[processCount];
	int processBurst[processCount];
	int processArrival[processCount];
	int processWait[processCount];
	int processTurn[processCount];


	int burstCount = 0;
	int nameCount = 0;
	int arrivalCount = 0;
	int j, timebuff = 0;
	int waitCount = 0;
	int turnaroundCount = 0;
	int i = 0;

	for (i = 4; i < size; i += 3)
	{

		processNames[nameCount] = buffer[i - 1];
		nameCount++;
		processArrival[arrivalCount] = atoi(buffer[i]);
		arrivalCount++;
		processBurst[burstCount] = atoi(buffer[i + 1]);
		burstCount++;
		processWait[waitCount] = 0;
		waitCount++;
		processTurn[turnaroundCount] = 0;
		turnaroundCount++;

	}


	int tempArr = 0, tempBurst = 0, x = 0;
	char *tempname[100];


	// Call an O(n^2) algorithm to sort by arrival, will make processing easier
	for (i = 0; i < processCount; ++i)
	{
		for (j = 0; j < processCount - i - 1; ++i)
		{
			//BubbleSort by Arrival Time

			if (processArrival[i] > processArrival[i + 1])
			{
				tempArr = processArrival[i];
				tempBurst = processBurst[i];
				tempname[x] = processNames[i];


				processArrival[i] = processArrival[i + 1];
				processBurst[i] = processBurst[i + 1];
				processNames[i] = processNames[i + 1];

				processArrival[i + 1] = tempArr;
				processBurst[i + 1] = tempBurst;
				processNames[i + 1] = tempname[x];

				x++;
			}
		}

	}

	while (currTime <= timeUnits)
	{

		for (i = 0; i < processCount; i++)
		{
			//First arrival 
			if (currTime == processArrival[i])
			{

				fprintf(ofp, "Time %d: %s arrived\n", currTime, processNames[i]);
				firstSelection = 1;
				arrived++;
			}

			;
			if (firstSelection && arrived > 0)
			{
				if (i > 0)
					currTime -= 1;


				fprintf(ofp, "Time %d: %s selected (burst %d)\n", currTime, processNames[i], processBurst[i]);

				processWait[i] = currTime - processArrival[i];
				processTurn[i] = processBurst[i] + processWait[i];

				// Determines if process has arrived and starting x at arrived prevents duplicate printing
				if (i != processCount)
				{
					for (x = arrived; x < processCount; x++)
					{
						if (processArrival[x] < currTime + processBurst[i])
						{
							fprintf(ofp, "Time %d: %s arrived\n", processArrival[x], processNames[x]);
							arrived++;

						}

					}
				}
				// After a process has been run using fcfs the time will be increased by the total burst of the -
				//  process that was just run
				currTime += processBurst[i];
				fprintf(ofp, "Time %d: %s finished\n", currTime, processNames[i]); // Each process will run until completion
				completed++; //Increment completion counter


			}
			//If not processes have arrived run idly until one does

			if (firstSelection && arrived == 0)
			{

				fprintf(ofp, "Time %d: IDLE\n", currTime);

				currTime++;
			}
			//If all processes have been completed run idly until the time reaches the time unit

			if (completed == processCount && currTime + 1 <= timeUnits)
			{

				while (currTime + 1 <= timeUnits)
				{
					currTime++;

					fprintf(ofp, "Time %d: IDLE\n", currTime);
				}


			}

			currTime++;
		}

		fprintf(ofp, "Finished at time %d", timeUnits);
		fprintf(ofp, "\n");
		fprintf(ofp, "\n");
		for ( i = 0; i < processCount; i++)
			fprintf(ofp, "%s wait %d turnaround %d\n", processNames[i], processWait[i], processTurn[i]);

	}
	

	fclose(ofp);

	return;
}

//Brandon Bradley
//University of Central Florida

//Function takes in a char array as input and simulates
//the round robin scheduling algorithm
//Outputs in processes.out file
void roundRobin(char *info)
{
	//For file output
	FILE *fp;
	fp = fopen("processes.out", "w");

	
	//i=0 -- guaranteed to be processCount from processFile function call
	int processCount = atoi(&info[0]);

	//i=1 -- guaranteed to be timeUnits
	int numTimeUnits = atoi(&info[1]);

	
	//printf("Number of time units is %i\n", numTimeUnits);
	//i=3 -- guaranteed to be Quantum
	int quantum = atoi(&info[6]);
	

	fprintf(fp, "%i Processes\n", processCount);
	fprintf(fp, "Using Round Robin\n");
	fprintf(fp, "Quantum %i\n\n", quantum);

	//Create the new tokenized array
	int arrayLen = 3 + (3 * processCount);
	char * array[arrayLen];
	int count = 0;

	array[count] = strtok(info, "\n\r");

	while (array[count] != NULL)
	{
		array[++count] = strtok(NULL, "\n\r");
	}

	

	//i=2 -- useRR, has already been processed. Ignore.
	//i=3 -- Quantum is present for RR.
	//i=4 -- guaranteed to be processName, and every i=4+3 for each processCount
	//i=5 -- guaranteed to be arrivalTime, and every i=5+3 for each processCount
	//i=6 -- guaranteed to be burstTime, and every i=6+3 for each processCount

	//Store Processes in new array
	//Logical representation Arr-Burst - [arrivalTime0][burstTime0][arrivalTime1][burstTime1]...[arrivalTimeN][burstTimeN]
	//Logical representation ProNames -  [P* i=0 n=0 ][P* i=2 n=1 ][P* i=4 n=2  ][P* i=6 n=3]
	//We need to keep track of the process names and corresponding locations for output
	//We can associate each arrivalTime with process name in a char array. So processNames[i] = processes[i/2] where i is index +=2
	//Create the new arrays and counts
	char * processNames[processCount];
	int processes[2 * processCount];
	int processesCount = 0;
	int processNameCount = 0;
	int i = 0;
	int j = 0;
	int q = 0;

	//We also need wait and turnaround times for each process
	//Logical representation Wait-TurnAround - [waitTime0][turnaroundTime0]...[waitTimeN][turnaroundTimeN]
	//Using this method we can track the process names in the same manner as usual
	//Turnaround time = timeFinished - timeArrived (processes[i])
	//Wait time = turnaroundTime - burstTime (processes[i+1])
	//We also need the original burstTimes since we will modify the array
	int times[2 * processCount];
	int immutableProcesses[2 * processCount];

	//Store the data in the new arrays
	for ( i = 5; i < arrayLen; i += 3) 
	{

		processNames[processNameCount] = array[i - 1];
		processNameCount++;
		processes[processesCount] = atoi(array[i]);
		immutableProcesses[processesCount] = atoi(array[i]);
		processesCount++;
		processes[processesCount] = atoi(array[i + 1]);
		immutableProcesses[processesCount] = atoi(array[i + 1]);
		processesCount++;


	}

	//We are ready for the Round Robin Scheduler
	//Need init time at 0
	int currentTime = 0;

	//Need a queue to hold the processes
	struct rrQueue * rrQueue = createrrQueue(processCount);

	//Need to know when the first process is selected
	int selectedFirstProcess = 0;

	//Need to know when all processes are finished
	int numProcessesFinished = 0;

	//Need to know if the scheduler is idle BETWEEN processes after selecting first process
	int numProcessesArrived = 0;

	//Need to know if current job is the same job as the last time unit
	int lastExecutedProcessIndex = -1;

	//Need to know if we are in the middle of executing a burst
	int executingBurst = 0;

	//Need to know the length of time we are executing burst -- and modify it
	int executingBurstTime = quantum;

	//Break from second loop after arrival checks for each time unit
	int breakForExecution = 0;

	//If a process has finished it is ok to select the next process
	int bypassExecutionForSelection = 0;

	//Less than as currentTime begins at zero
	while (currentTime < numTimeUnits) 
	{

		if (executingBurst) 
		{

			//printf("We are currently executing burst. Current time: %i\n.", currentTime);
			executingBurstTime--;
			//printf("Current executing burst time left is %i\n", executingBurstTime);
		}

		if (executingBurstTime == 0) 
		{

			executingBurst = 0;
			breakForExecution = 0;
			//printf("Executing burst has finished. Current time: %i\n", currentTime);

			//Enqueue the process again if it has not finished

			if (processes[lastExecutedProcessIndex + 1] != 0) 
			{

				enqueue(rrQueue, lastExecutedProcessIndex);

			}
		}

		//We are checking each processes arrival time and burstTime on each iteration
		for ( i = 0; i < (2 * processCount); i += 2) 
		{

			if (breakForExecution)
				break;

			//The arrival time is equal to the current time --, output Time N: STRING arrived
			//We need to account for processes that arrive at the same time
			for (j = 0; j < (2 * processCount); j += 2) 
			{

				//THE INDEX OF THE ARRIVAL TIME OF THE PROCESS IS WHAT IS STORED IN THE QUEUE
				//We need to check arrivals on each iteration of time
				//We can not select while executingBurst but we can let processes arrive and get added to the queue
				if (currentTime == processes[j] && processes[j + 1] != 0) 
				{

					int addedToQueue = 0;

					for ( q = 0; q < rrQueue->size; q++) 
					{

						if (rrQueue->array[q] == j) 
						{
							addedToQueue = 1;

							//printf("This process is already in the queue.\n");
						}
					}

					if (!addedToQueue) 
					{

						fprintf(fp, "Time %i: %s arrived\n", currentTime, processNames[(j) / 2]);

						selectedFirstProcess = 1;

						numProcessesArrived++;

						//Add the process to the queue
						//We are going to store the arrival index to stay consistent with SJF scheduler
						enqueue(rrQueue, j);

					}

				}
			}

			if (executingBurst && bypassExecutionForSelection == 0) 
			{

				//printf("Executing burst. Breaking from loop after arrival checks.\n");
				breakForExecution = 1;
				currentTime++;
				break;
			}

			//In this case we are idling this time unit BETWEEN processes as all processes
			//are not finished and we have checked all arrival times
			if (selectedFirstProcess && numProcessesArrived == 0 && numProcessesFinished != processCount && !executingBurst) 
			{

				//Output Time N: IDLE
				
				fprintf(fp, "Time %i: IDLE\n", currentTime);

				//We are done with the current time unit
				currentTime++;

				break;

			}

			//In this case we are idling at zero time unit, BEFORE any processes have arrived
			//and we have checked all arrival times
			if (!selectedFirstProcess && numProcessesArrived == 0 && numProcessesFinished != processCount && !executingBurst) 
			{

				//Output Time N: IDLE
				
				fprintf(fp, "Time %i: IDLE\n", currentTime);

				//We are done with the current time unit
				currentTime++;

				break;

			}

			//In this case we are idling, AFTER ALL processes have finished
			//and we have checked all arrival times
			if (selectedFirstProcess && numProcessesArrived == 0 && numProcessesFinished == processCount && !executingBurst) 
			{

				//Output Time N: IDLE
				fprintf(fp, "Time %i: IDLE\n", currentTime);

				//We are done with the current time unit
				currentTime++;

				break;

			}

			//We only start selecting processes once the first process arrives AND we should NOT be idling
			if (selectedFirstProcess && numProcessesArrived > 0 && executingBurst == 0) 
			{

				//Now we know we have an item(s) in the queue and it is available to execute this time unit

				//Output Time N: STRING selected (burst PROCESSES[j+1])
				
				fprintf(fp, "Time %i: %s selected (burst %i)\n", currentTime, processNames[(front(rrQueue)) / 2], processes[front(rrQueue) + 1]);

				//Reset the execution bypass for selection
				bypassExecutionForSelection = 0;

				//Get the current burst time of the selected job
				int curBurstTime = processes[front(rrQueue) + 1];

				//printf("The burstTime of the currently selected process is: %i\n", curBurstTime);

				//This is the end for this process, update and output Time N: STRING finished
				//Once we enter this burst we can check for arrivals but are otherwise locked
				if (curBurstTime <= quantum)
				{

					//Since we go by quantum, the time increment can change depending on the curBurstTime
					int variableQuantumTimeIncrease = 0;

					//This process will end at zero burst time, currentTime will increment by normal quantum
					if (curBurstTime == quantum) 
					{
						variableQuantumTimeIncrease = quantum;
					}

					//This process has less burst time left than the quantum
					else if (curBurstTime < quantum) 
					{
						variableQuantumTimeIncrease = quantum - curBurstTime;

					}

					//Set the process's burstTime to zero
					processes[front(rrQueue) + 1] = 0;

					//Save before we dequeue
					int arrivalIndex = front(rrQueue);

					//Take it off the queue, should not be added back in since its burst time is zero
					dequeue(rrQueue);

					//Set this job as the last executed
					lastExecutedProcessIndex = arrivalIndex;

					//Set flags and time for burst execution phase
					executingBurst = 1;
					executingBurstTime = variableQuantumTimeIncrease;

					//We are done with the current time unit
					currentTime++;

					//Remember arrivalIndex is actually arrival time, /2 for corresponding process name
				
					fprintf(fp, "Time %i: %s finished\n", currentTime, processNames[((arrivalIndex) / 2)]);

					//Increment the number of processes finished
					numProcessesFinished++;

					//Decrement the number of arrived processes
					numProcessesArrived--;

					//A process has finished and we need to select the next one
					bypassExecutionForSelection = 1;

					//Store turnaroundTime = FINISHED-ARR, shortestJobIndex is burstTime
					//Logical representation
					//[a][b][a][b]
					//[w][t][w][t]
					int turnaroundTime = currentTime - processes[arrivalIndex];
					times[arrivalIndex + 1] = turnaroundTime;

					//Calculate waitTime = TAT-BURST
					int waitTime = turnaroundTime - immutableProcesses[arrivalIndex + 1];
					times[arrivalIndex] = waitTime;

				}

				//If the burst time is greater than the quantum decrement it
				else if (curBurstTime > quantum) 
				{

					//Decrement the current processes burst time and store it in the array
					curBurstTime = curBurstTime - quantum;
					processes[front(rrQueue) + 1] = curBurstTime;

					//Set flags and time for burst execution phase
					executingBurst = 1;
					executingBurstTime = quantum;

					//Save this before we dequeue
					int arrivalIndex = front(rrQueue);

					//Dequeue the process
					dequeue(rrQueue);

					//Set this job as the last executed index
					lastExecutedProcessIndex = arrivalIndex;

					//printf("The new burstTime of the currently selected process is %i\n", processes[arrivalIndex+1]);

					//We are done with the current time unit
					currentTime++;

					//printf("Current time is now: %i after executing burst.\n", currentTime);

					break;


				}

				//In this case we are idling this time unit,
				if (selectedFirstProcess && numProcessesArrived == 0 && numProcessesFinished != processCount) 
				{

					//Output Time N: IDLE
			
					fprintf(fp, "Time %i: IDLE\n", currentTime);

					//We are done with the current time unit
					currentTime++;

					break;

				}

				//Scheduler is idle this time unit as ALL processes are finished, output
				else if (numProcessesFinished == processCount) 
				{

					//Output Time N: IDLE
				
					fprintf(fp, "Time %i: IDLE\n", currentTime);
					//We are done with the current time unit
					currentTime++;

					break;


				}


			}

		}
	}


	//Round Robin Scheduler is done, output
	
	fprintf(fp, "Finished at time %i\n\n", numTimeUnits);
	int t;

	for  (t = 0; t < (2 * processCount); t += 2) 
	{

		fprintf(fp, "%s wait %i turnaround %i\n", processNames[(t / 2)], times[t], times[t + 1]);
	}

	fclose(fp);

	return;

}
//Brandon Bradley
//University of Central Florida

//Function takes in a char array as input and simulates
//the sho rtest job first scheduling algorithm
//Outputs in processes.out file
void shortestjobFirst(char *info)
{
	//For file output
	FILE *fp;
	fp = fopen("processes.out", "w");

	//i=0 -- guaranteed to be processCount from processFile function call
	int processCount = atoi(&info[0]);

	//i=1 -- guaranteed to be timeUnits
	int numTimeUnits = atoi(&info[1]);

	
	fprintf(fp, "%i Processes\n", processCount);
	fprintf(fp, "Using Shortest Job First (Pre)\n\n");

	//Create the new tokenized array
	int arrayLen = 3 + (3 * processCount);
	char * array[arrayLen];
	int count = 0;

	array[count] = strtok(info, "\n\r");

	while (array[count] != NULL)
	{
		array[++count] = strtok(NULL, "\n\r");
	}
	int j = 0;

	//i=2 -- useSJF, has already been processed. Ignore.
	//Quantum, has been removed for SJF.
	//i=3 -- guaranteed to be processName, and every i=3+3 for each processCount
	//i=4 -- guaranteed to be arrivalTime, and every i=4+3 for each processCount
	//i=5 -- guaranteed to be burstTime, and every i=5+3 for each processCount

	//Store Processes in new array
	//Logical representation Arr-Burst - [arrivalTime0][burstTime0][arrivalTime1][burstTime1]...[arrivalTimeN][burstTimeN]
	//Logical representation ProNames -  [P* i=0 n=0 ][P* i=2 n=1 ][P* i=4 n=2  ][P* i=6 n=3]
	//We need to keep track of the process names and corresponding locations for output
	//We can associate each arrivalTime with process name in a char array. So processNames[i] = processes[i/2] where i is index +=2
	//Create the new arrays and counts
	char * processNames[processCount];
	int processes[2 * processCount];
	int processesCount = 0;
	int processNameCount = 0;

	//We also need wait and turnaround times for each process
	//Logical representation Wait-TurnAround - [waitTime0][turnaroundTime0]...[waitTimeN][turnaroundTimeN]
	//Using this method we can track the process names in the same manner as usual
	//Turnaround time = timeFinished - timeArrived (processes[i])
	//Wait time = turnaroundTime - burstTime (processes[i+1])
	//We also need the original burstTimes since we will modify the array
	int times[2 * processCount];
	int immutableProcesses[2 * processCount];
	int i = 0;

	//Store the data in the new arrays
	for (i = 4; i < arrayLen; i += 3) {

		processNames[processNameCount] = array[i - 1];
		processNameCount++;
		processes[processesCount] = atoi(array[i]);
		immutableProcesses[processesCount] = atoi(array[i]);
		processesCount++;
		processes[processesCount] = atoi(array[i + 1]);
		immutableProcesses[processesCount] = atoi(array[i + 1]);
		processesCount++;


	}

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

	//Need to know if we have switched jobs
	int switchedJobs = 0;

	//Need to know if current job is the same job as the last time unit
	int lastExecutedJobIndex = -1;

	//Edge cases:
	//Two or more processes arrive at the same time (PASSED)
	//Idle time between arrival of processes (PASSED)
	//NOTE: Guaranteed there will not be identical burst times for SJF input (PASSED)
	//Only one process (PASSED)
	//Process does not arrive at time zero (PASSED)
	//Process name does not begin with a letter, or is a number (PASSED)

	//Less than as currentTime begins at zero
	while (currentTime < numTimeUnits) {

		//Set the shortest job index and lowest burst time as max int each loop to reset and guarantee SJF
		int shortestJobIndex = INT_MAX;
		int currentLowestBurstTime = INT_MAX;

		//We are checking each processes arrival time and burstTime on each iteration
		for (i = 0; i < (2 * processCount); i += 2) 
		{

			//The arrival time is equal to the current time -- this will only occur once before selection, output Time N: STRING arrived
			//We need to account for processes that arrive at the same time
			for (i = 0; i < (2 * processCount); i += 2) 
			{

				if (currentTime == processes[i]) 
				{

					
					fprintf(fp, "Time %i: %s arrived\n", currentTime, processNames[(i / 2)]);

					selectedFirstJob = 1;

					numProcessesArrived++;

				}
			}

			//In this case we are idling this time unit BETWEEN processes as all processes
			//are not finished and we have checked all arrival times
			if (selectedFirstJob && numProcessesArrived == 0 && numProcessesFinished != processCount) 
			{

				//Output Time N: IDLE
				fprintf(fp, "Time %i: IDLE\n", currentTime);

				//We are done with the current time unit
				currentTime++;

				break;

			}

			//In this case we are idling at zero time unit, BEFORE any processes have arrived
			//and we have checked all arrival times
			if (!selectedFirstJob && numProcessesArrived == 0 && numProcessesFinished != processCount) 
			{

				//Output Time N: IDLE
				//printf("Time %i: IDLE\n", currentTime);
				fprintf(fp, "Time %i: IDLE\n", currentTime);

				//We are done with the current time unit
				currentTime++;

				break;

			}

			//In this case we are idling, AFTER ALL processes have finished
			//and we have checked all arrival times
			if (selectedFirstJob && numProcessesArrived == 0 && numProcessesFinished == processCount) 
			{

				//Output Time N: IDLE
				fprintf(fp, "Time %i: IDLE\n", currentTime);

				//We are done with the current time unit
				currentTime++;

				break;

			}

			//We only start selecting processes once the first process arrives AND we should NOT be idling
			if (selectedFirstJob && numProcessesArrived > 0) 
			{
				
				//Find the shortest job in the array
				for (j = 0; j < (2 * processCount); j += 2) 
				{

					//The arrival time is LEQ to the current time, so it qualifies to have burstTime checked
					if (processes[j] <= currentTime)
					{

						//This process's burst time has the current lowest burstTime and has not already finished
						if (processes[j + 1] < currentLowestBurstTime && processes[j + 1] != 0)
						{

					

							//We know right now that this is the currentShortestJobIndex
							currentShortestJobIndex = j + 1;

							//We are switching jobs or setting the shortestJobIndex for the first time
							if ((currentShortestJobIndex != shortestJobIndex) || currentShortestJobIndex == -1) 
							{

								//Update the shortest job
								shortestJobIndex = j + 1;

								//Update the currentLowestBurstTime
								currentLowestBurstTime = processes[j + 1];

								//Need to know if we ACTUALLY switched jobs or just found the same shortest job from last iteration
								//If the lastExecutedJobIndex is not the shortestJobIndex or -1(initial), then we have switched
								if (lastExecutedJobIndex != shortestJobIndex || lastExecutedJobIndex == -1) 
								{

									switchedJobs = 1;

								}

								else {

									switchedJobs = 0;
								}

								//printf("The new current lowest burst time is %i\n", currentLowestBurstTime);
							}
						}
					}
				}

				//We now have the shortestJobIndex
				//If we are switching jobs, output selection
				//THen we need to change this check, we can use a flag
				if (switchedJobs) {

					//This will keep its value for the next iteration
					currentShortestJobIndex = shortestJobIndex;

					//Reset the switched jobs flag
					switchedJobs = 0;

					//Output Time N: STRING selected (burst PROCESSES[j+1])
					//printf("Time %i: %s selected (burst %i)\n", currentTime, processNames[(shortestJobIndex-1)/2], processes[shortestJobIndex]);
					fprintf(fp, "Time %i: %s selected (burst %i)\n", currentTime, processNames[(shortestJobIndex - 1) / 2], processes[shortestJobIndex]);

					//Don't break unless we burn the time unit

				}

				//Get the current burst time of the selected job
				int curBurstTime = processes[shortestJobIndex];

				//printf("The burstTime of the currently selected process is: %i\n", curBurstTime);

				//This is the end for this process, update and output Time N: STRING finished
				if (curBurstTime == 1) {

					//Decrement the current processes burst time and store it in the array
					curBurstTime--;
					processes[shortestJobIndex] = curBurstTime;
					currentLowestBurstTime = curBurstTime;

					//Set this job as the last executed
					lastExecutedJobIndex = shortestJobIndex;

					//We are done with the current time unit
					currentTime++;

					//Remember shortestJobIndex is actually burst time, so minus one to get the arrival time and /2 for corresponding process name
					//printf("Time %i: %s finished\n", currentTime, processNames[((shortestJobIndex-1)/2)]);
					fprintf(fp, "Time %i: %s finished\n", currentTime, processNames[((shortestJobIndex - 1) / 2)]);

					//Increment the number of processes finished
					numProcessesFinished++;

					//Decrement the number of arrived processes
					numProcessesArrived--;

					//Don't break since we still want to select a new process this time unit

					//Store turnaroundTime = FINISHED-ARR, shortestJobIndex is burstTime
					//Logical representation
					//[a][b][a][b]
					//[w][t][w][t]
					int turnaroundTime = currentTime - processes[(shortestJobIndex - 1)];
					times[shortestJobIndex] = turnaroundTime;

					//Calculate waitTime = TAT-BURST
					int waitTime = turnaroundTime - immutableProcesses[(shortestJobIndex)];
					times[shortestJobIndex - 1] = waitTime;

				}

				//If the burst time is greater than zero decrement it
				else if (curBurstTime > 0) {

					//Decrement the current processes burst time and store it in the array
					curBurstTime--;
					processes[shortestJobIndex] = curBurstTime;
					currentLowestBurstTime = curBurstTime;

					//Set this job as the last executed
					lastExecutedJobIndex = shortestJobIndex;

					//printf("The new burstTime of the currently selected process is %i\n", processes[shortestJobIndex]);

					//We are done with the current time unit
					currentTime++;

					break;


				}

				//In this case we are idling this time unit,
				if (selectedFirstJob && numProcessesArrived == 0 && numProcessesFinished != processCount) {

					//Output Time N: IDLE
					//printf("Time %i: IDLE\n", currentTime);
					fprintf(fp, "Time %i: IDLE\n", currentTime);

					//We are done with the current time unit
					currentTime++;

					break;

				}

				//Scheduler is idle this time unit as ALL processes are finished, output
				else if (numProcessesFinished == processCount) {

					//Output Time N: IDLE
					//printf("Time %i: IDLE\n", currentTime);
					fprintf(fp, "Time %i: IDLE\n", currentTime);
					//We are done with the current time unit
					currentTime++;

					break;


				}
			}
		}
	}

	//SJF Scheduler is done, output
	//printf("Finished at time %i\n\n", numTimeUnits);
	fprintf(fp, "Finished at time %i\n\n", numTimeUnits);
	int t;

	for (t = 0; t < (2 * processCount); t += 2) {

		//printf("%s wait %i turnaround %i\n", processNames[(t/2)], times[t], times[t+1]);
		fprintf(fp, "%s wait %i turnaround %i\n", processNames[(t / 2)], times[t], times[t + 1]);
	}

	fclose(fp);

	return;
}
//Alexander Alvarez
// This takes a string containing all the characters found in "processes.in" and -
// parses out appropriate information and stores in into a string one line at a time -
// deliniated by a new line character
//Alexander Alvarez
// This takes a string containing all the characters found in "processes.in" and -
// parses out appropriate information and stores in into a string one line at a time -
// deliniated by a new line character
char processFile(char *buffer, int size)
{
	int i, x = 0, y = 0, flag = 0, increment = 0;
	char infobuff[500], info[500];


	//parse out comments    
	for (i = 0; i < size; i++)
	{
		if (buffer[i] == '#')
		{
			while (buffer[i] != '\n')
			{
				i++;
			}
		}

		// Take out the comments and store in string
		infobuff[x++] = buffer[i];

		// Parse the string for only relevant information
		// ProcessCount & Burst
		if (buffer[i] == 't' && buffer[i + 1] == ' ')
		{
			i += 2;
			while (buffer[i] != ' ' && isdigit(buffer[i]))
			{
				info[y++] = buffer[i++];
			}
			info[y++] = '\n';
		}
		// Arrival Time
		if (buffer[i] == 'l' && buffer[i + 1] == ' ' || buffer[i] == 'l' && buffer[i + 1] == '\n')
		{
			i += 2;
			while (buffer[i] != ' ' && buffer[i] != '\n')
			{
				info[y++] = buffer[i++];
			}
			info[y++] = '\n';
		}
		//Quantum
		if (buffer[i] == 'm' && buffer[i + 1] == ' ')
		{
			i += 2;
			while (buffer[i] != ' ')
			{
				info[y++] = buffer[i++];
			}
			info[y++] = '\n';
		}
		// Use
		if (buffer[i] == 'e'&& buffer[i - 1] == 's' && buffer[i + 1] == ' ')
		{
			info[y++] = buffer[i + 2];

			info[y++] = '\n';

			if (info[y - 2] == 's')
				flag = 1;
			else if (info[y - 2] == 'f')
				flag = 2;
		}
		//Process name
		if (buffer[i] == 'e' && buffer[i - 1] == 'm' && buffer[i + 1] == ' ')
		{
			i += 2;
			while (buffer[i] != ' ')
			{
				info[y++] = buffer[i++];

			}
			info[y++] = '\n';
		}

		//Run for
		if (buffer[i] == 'r'&& buffer[i - 1] == 'o' && buffer[i + 1] == ' ')
		{
			i += 2;
			while (buffer[i] != ' ')
			{
				info[y++] = buffer[i++];

			}
			info[y++] = '\n';
		}
	}


	//for(i = 0; i < strlen(info);i++)
	//   printf("[%d] = %c\n",i, info[i]);

	if (flag == 0)
		roundRobin(info);

	else if (flag == 1)
		shortestjobFirst(info);

	else
		firstcomefirstServe(info);

	return *info;
}

int main(void)
{
	FILE *input_file;
	char buffer[1000];
	int i = 0, size = 0;


	if (buffer == NULL)
	{
		printf("Error: Memory allocation failed\n.");
		exit(1);
	}
	// Take in the input file
	input_file = fopen("processes.in", "r");

	if (input_file == NULL)
	{
		printf("Error: File 'processes.in' missing or not in the same directory\n.");
		exit(1);
	}
	// Take in the text one character at a time.
	while ((fscanf(input_file, "%c", &buffer[i])) != EOF) //scanf and check EOF
	{
		i++;
	}

	fclose(input_file);

	size = strlen(buffer);

	processFile(buffer, size);

	return 0;
}
