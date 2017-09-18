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

void PrintAverageWaitTime(GList * process_list){
  GList *l;
  int sum=0,number=0;
  for(l=process_list;l!=NULL;l=l->next){
    Process node = l->data;
    number++;
    sum += node->process_lastruntime - node->process_arrival - node->process_runtime;
  }
  float avg = (float)sum/number;
  printf("Average wait time for FCFS Algorithm : %f\n",avg);
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
  PrintAverageWaitTime(fc);
  //PrintProcessList(fc);
}
