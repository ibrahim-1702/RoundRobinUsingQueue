#include <stdio.h>
#include <stdlib.h>
#define SLICE 2   // setting slice time or time quantum to 2
#define PROCESS 4 // setting number process as 4

struct process // structure for a process
{
 int id;
 int burstTime;        // process have burst time
 int arrivalTime;      // a process arrives at a time quantum
 struct process *next; // next pointer which will point the next process to be executed in the processor
};

struct process *queueF = NULL; // ready queue's front pointer
struct process *queueR = NULL; // ready queue's rear pointer
struct process *processes[5];  // its a list of process

int isEmpty()
{
 return (queueF == NULL);
}

void enqueue(struct process *new) // enqueues a process into the ready queue
{
 if (queueF == NULL)
 {
  queueF = new;
  queueR = new;
  return;
 }
 if (queueF == queueR)
 {
  queueR = new;
  queueF->next = queueR;
  return;
 }
 queueR->next = new;
 queueR = new;
 queueR->next = NULL;
}

struct process *dequeue() // dequeues a process from the ready queue and returns the address of the process which got dequeued.
{
 struct process *p;
 if (queueF != NULL)
 {
  p = queueF;
  if (queueF == queueR)
  {
   queueF = NULL;
   queueR = NULL;
   return p;
  }
  struct process *tmp = queueF;
  queueF = tmp->next;
 }
 return p;
}

void printGanttChart(int state, int id, int end) // draws the gantt chart onto the terminal/cmdline
{
 switch (state)
 {
 case 0:
  printf("0--p%d--%d", id, end);
  break;
 default:
  printf("--p%d--%d", id, end);
  break;
 }
}

void main()
{

 int sliceTime = SLICE;                   // slice time is set to 2
 int burstTime[PROCESS] = {8, 6, 3, 2};   // declaring and assigning values to an array which has burst time of consecutive processes
 int arrivalTime[PROCESS] = {0, 1, 3, 5}; // declaring and assigning values to an array which has arrival time of consecutive processes
 int processPtr = 0;                      // this variable is used to iterate through the process array
 for (int i = 0; i < PROCESS; i++)        // populating process array with burst time and arrival time respectively
 {
  processes[i] = malloc(sizeof(struct process));
  processes[i]->burstTime = burstTime[i];
  processes[i]->arrivalTime = arrivalTime[i];
  processes[i]->id = i + 1;
 }

 int clockCycle = 0; // clockCycle is the variable keeps track of iteration of the loop each iteration is considered a cycle for this simulation of round robin process scheduling algorithm.
 do
 {
  if ((processPtr < PROCESS) /*check for AIOOB Exception*/ && processes[processPtr]->arrivalTime == clockCycle /*Checks whether current process at processPtr is arriving.*/)
  {
   // AIOOB -> "Array Index Out Of Bound"
   enqueue(processes[processPtr++]); /* if it's AT = clockCycle then we enqueue the process at processPtr into the ready queue.*/
  }
  // the front of the queue gets processed here for one clockCycle
  /*After processing the front of queue then we would need to
  dequeue the front of the queue there can be two cases while dequeuing
  case1: process at front's burst time is completed
  case2: process at front's alloted slice time has been exhausted
  */
  if (queueF->burstTime == 0) // case 1
  {
   struct process *p = dequeue(); /*just dequeuing the process as its burst time is over
                                   */
   printGanttChart(clockCycle - 1, p->id, clockCycle);
   sliceTime = SLICE; // reset sliceTime to the fixed time quantum.
  }
  else if (sliceTime == 0) // case 2
  {
   struct process *p = dequeue(); // dequeuing process as the sliceTime has been exhausted
   enqueue(p);                    // enqueuing the dequeued process as the process remaining burst time.
   printGanttChart(clockCycle - 2, p->id, clockCycle);
   sliceTime = SLICE; // reset sliceTime to the fixed time quantum.
  }

  if (!isEmpty()) // burst time, slice time gets decremented only if the ready is not empty
  {
   queueF->burstTime--;
   sliceTime--;
  }
  clockCycle++; // clock cycle gets incremented as the one iteration has been completed.
 }while(!isEmpty());
}
