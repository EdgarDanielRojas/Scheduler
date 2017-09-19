/*
 * Copyright (c) 2017 Edgar Daniel Rojas Vazquez
 *
 * File name: Dispatcher.c
 *
 * Author:  Edgar Daniel Rojas Vazquez
 *
 * Purpose: Supports routines for the creation of processes and their scheduling
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
 *          August 29 7:49 2017 -- File created
 *
 * Error handling:
 *          None
 *
 * Notes:
 *          Not as complicated as it seems, supports the six scheduling algorithms and
 *          the creation of processes for easy handling.
 *
 * $Id$
 */
#include <stdio.h>
#include <glib.h>				/*Used so we can use the GList double linked list*/
#include "Process.h"

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
      runningList = g_list_insert(runningList,result->data,-1);
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
    }
    result = g_list_find_custom(p,&time,(GCompareFunc)funcArrival);
    if(result!=NULL){
      //printf("New process introduced at time %d\n",time);
      runningList = g_list_insert(runningList,result->data,-1);
      if(type == PRIORITY)
        runningList = SortProcessList(runningList,PRIORITY);
      else if(type == CPUBURST)
        runningList = SortProcessList(runningList,CPUBURST);

      //PrintProcessList(runningList);
      if(running != runningList->data){
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
    running->process_remainingcycles = running->process_remainingcycles -1;
  }while(runningList!=NULL);
  //PrintProcessList(p);
  if(type == PRIORITY)
    string = "Preemptive Priority";
  else
    string = "Preemptive SJF";
  PrintAverageWaitTime(p,string);
  DestroyList(p);
  DestroyList(runningList);
}
