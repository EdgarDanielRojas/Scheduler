/*
 * Copyright (c) 2017 Edgar Daniel Rojas Vazquez
 *
 * File name: Dispatcher.c
 *
 * Author:  Edgar Daniel Rojas Vazquez
 *
 * Purpose: Supports routines for the scheduling of processes and
 *          calcultion of average waiting time.
 *
 * References:
 *          Sorting algorithms based on those seen in class and
 *          in notes from Abelardo Lopez Lagunas' slides
 *
 * Restrictions:
 *          All functions return EXIT_FAILURE in case something went
 *          wrong
 *
 * Revision history:
 *          https://github.com/EdgarDanielRojas/Scheduler
 *
 * Error handling:
 *          None
 *
 * Notes:
 *          None
 *
 */
#include <stdio.h>       /*Used for the printf function*/
#include <glib.h>				/*Used so we can use the GList double linked list*/
#include "Process.h"    /*Used for the data structures and sorting functions*/


/*!
* \brief Prints the average wait time of a list.
*
* \param process_list Head of the process list that will have the average wait time of it´s elements calculated
* \param tipo Text that contains the type of algorithm that was used on the list.
*
* \return output Prints out the type of algorithm used and the average wait time for that algorithm
*
* \details The function takes a pointer to the head of the list and using it
  loops through the whole list, calculating the wait time for each data element (process_p)
  . Once it calculates the wait time of the element it adds it to the total wait time of the
  proceses and divides it by the number of processes which was taken using a counter. g_list_length
  could also be used but it does the same thing in it´s function definition.
*/
void PrintAverageWaitTime(GList * process_list, char *tipo){
  GList *l;
  int sum=0,number=0;
  for(l=process_list;l!=NULL;l=l->next){
    Process node = l->data;
    number++;
    sum += node->process_lastruntime - node->process_arrival - node->process_runtime;
  }
  float avg = (float)sum/number;
  printf("Average wait time for %s Algorithm : %f\n",tipo,avg);
}

/*!
* \brief Adds processes to a running list.
*
* \param running_list Head of the running list to which we shall add processes.
* \param result Head of our results taken from our search function.
*
* \return GList* Returns the new head of the running list once elements are added.
*
* \details The way the function is used is that it adds elements based on arrival time.
  When it is called it is know that the result in it´s data field holds a process that
  has arrived at the time it is called. Once it's data element is added to the running list
  we use it's arrival time to compare with the rest of the elements in the result list.
  This is because more than one element can arrive at the same time. In the case that an element
  does not have a same arrival time we stop searching the list. In the case that an element does
  have the same arrival time, it is added to the list.
*/
GList * addToList(GList * running_list,GList * result){
  running_list = g_list_insert(running_list,result->data,-1);
  Process p = result->data;
  GList *l;
  result = result->next;
  for(l=result;l!=NULL;l=l->next){
    Process node = l->data;
    if(node->process_arrival == p->process_arrival)
      running_list = g_list_insert(running_list,result->data,-1);
    else
      break;
  }
  return running_list;
}

/*!
* \brief Applies the First Come First Serve algorithm to a process list.
*
* \param process_list Head of the process list containing all the processes ordered by arrival time.
*
* \return output Calls PrintAverageWaitTime().
*
* \details The function creates a copy of the process list arriving using CopyList() so
  the copy list can be manipulated without fear of changing the original values of the list.
  Once the copy is made, the algorithm calculates the time the process runs by assigning it the
  current time and making the time equal to the time plus the process' cpu burst. We can do this
  since the algorithm is sequential.
*/
void FirstCome(GList * process_list){
  GList * fc = CopyList(process_list);
  Process running;
  int time=0;
  GList *l;
  for(l=fc;l!=NULL;l=l->next){
    running = l->data;
    running->process_lastruntime = time;
    time +=running->process_burst;
  }
  PrintAverageWaitTime(fc,"FCFS");
  DestroyList(fc);
}

gint funcArrival(gpointer a,int * b){
  Process aa = a;
  return aa->process_arrival - *b;
}

void NonPreemptive(GList * process_list, int type){
  GList * np = CopyList(process_list);
  GList * runningList = NULL;
  GList * result = NULL;
  Process running;
  int time=0;
  char *string;
  running = np->data;
  //runningList = g_list_insert(runningList,running,-1);
  do{
    result = g_list_find_custom(np,&time,(GCompareFunc)funcArrival);
    if(result!=NULL){
      runningList = addToList(runningList,result);
      if(type == PRIORITY)
        runningList = SortProcessList(runningList,PRIORITY);
      else if(type == CPUBURST)
        runningList = SortProcessList(runningList,CPUBURST);
    }
    if(running->process_burst == time - running->process_lastruntime){
      runningList = g_list_remove(runningList,running);
      if(runningList!=NULL){
        running = runningList->data;
        running->process_lastruntime = time;
      }
    }
    time++;
  }while(runningList!=NULL);
  if(type == PRIORITY)
    string = "NonPreemptive Priority";
  else
    string = "NonPreemptive SJF";
  PrintAverageWaitTime(np,string);
  DestroyList(np);
  DestroyList(runningList);
}

void Preemptive(GList * process_list, int type){
  GList * p = CopyList(process_list);
  GList * runningList = NULL;
  GList * result = NULL;
  Process running;
  int time=0;
  int timeRunning=0;
  char *string;
  running = p->data;
  //runningList = g_list_insert(runningList,running,-1);
  do{
    //printf("Time %d\n",time);
    if(running->process_remainingcycles ==  0){
      //printf("Removal of process at time %d\n",time);
      //PrintProcessList(runningList);
      runningList = g_list_remove(runningList,running);
      if(runningList!=NULL){
        running = runningList->data;
        running->process_lastruntime = time;
        timeRunning=0;
        //printf("Current process running %d\n",running->process_id);
      }
      else{
        running=NULL;
      }
    }
    result = g_list_find_custom(p,&time,(GCompareFunc)funcArrival);
    if(result!=NULL){
      //printf("New process introduced at time %d\n",time);
      runningList = addToList(runningList,result);
      if(type == PRIORITY)
        runningList = SortProcessList(runningList,PRIORITY);
      else if(type == CPUBURST)
        runningList = SortProcessList(runningList,CPUBURST);

      //PrintProcessList(runningList);
      if(running == NULL){
        running = runningList->data;
        running->process_lastruntime = time;
        timeRunning=0;
      }
      else if(running != runningList->data){
        int processtime = running->process_runtime;
        running->process_runtime = processtime + timeRunning;
        //printf("Change of process at time %d\n",time);
        running = runningList->data;
        running->process_lastruntime = time;
        timeRunning=0;
        //printf("Current process running %d\n",running->process_id);
      }
    }

    time++;
    timeRunning++;
    if(running != NULL)
      running->process_remainingcycles = running->process_remainingcycles -1;
  }while(runningList!=NULL);
  //PrintProcessList(p);
  if(type == PRIORITY)
    string = "Preemptive Priority";
  else
    string = "Preemptive SJF";

  //PrintProcessList(p);
  PrintAverageWaitTime(p,string);
  DestroyList(p);
  DestroyList(runningList);
}

void RoundRobin(GList * process_list, int quantum){
  GList * rr = CopyList(process_list);
  GList * runningList = NULL;
  GList * result = NULL;
  Process running;
  int time=0;
  int timeRunning=0;
  running = rr->data;
  //runningList = g_list_insert(runningList,running,-1);
  do{
    //printf("Time %d\n",time);
    if(running->process_remainingcycles ==  0){
      //printf("Removal of process at time %d\n",time);
      //PrintProcessList(runningList);
      runningList = g_list_remove(runningList,running);
      if(runningList!=NULL){
        running = runningList->data;
        running->process_lastruntime = time;
        timeRunning=0;
        //printf("Current process running %d\n",running->process_id);
      }
      else{
        running = NULL;
      }
    }
    if(timeRunning == quantum){
      running->process_runtime = running->process_runtime + timeRunning;
      runningList = g_list_remove(runningList,running);
      runningList = g_list_insert(runningList,running,-1);
      //printf("Quantum reached, processed moved to back of the line %d\n",time);
      if(runningList!=NULL){
        running = runningList->data;
        running->process_lastruntime = time;
        timeRunning=0;
        //printf("Current process running %d\n",running->process_id);
      }
    }
    result = g_list_find_custom(rr,&time,(GCompareFunc)funcArrival);
    if(result!=NULL){
      //printf("New process introduced at time %d\n",time);
      runningList = addToList(runningList,result);
      if(running == NULL){
        running = runningList->data;
        timeRunning=0;
        running->process_lastruntime = time;
      }
      //runningList = g_list_insert(runningList,result->data,-1);
      //PrintProcessList(runningList);
    }
    time++;
    timeRunning++;
    if(running != NULL)
      running->process_remainingcycles = running->process_remainingcycles -1;
  }while(runningList!=NULL);
  //PrintProcessList(rr);
  PrintAverageWaitTime(rr,"Round Robin");
  DestroyList(rr);
  DestroyList(runningList);
}
