/*
 * Copyright (c) 2017 Edgar Daniel Rojas Vazquez
 *
 * File name:Process.c
 *
 * Author:  Edgar Daniel Rojas Vazquez
 *
 * Purpose: Supports routines for the creation of processes
 *
 * References:
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
#include <stdlib.h>
#include <glib.h>				/*Used so we can use the GList double linked list*/
typedef struct process_p * Process;

struct process_p{
  int process_id;
  int process_arrival;
  int process_burst;
  int process_priority;
  int process_runtime;
  int process_lastruntime;
};

enum sort_type{ ID , ARRIVAL , PRIORITY , CPUBURST};

GList * CreateProcess(GList * process_list,int id, int arrival,int burst,int priority,int algo){
    Process node = (Process)malloc(sizeof(struct process_p));
    node->process_id = id;
    node->process_arrival = arrival;
    node->process_burst = burst;
    node->process_priority = priority;
    node->process_runtime = 0;
    node->process_lastruntime = 0;

    return g_list_insert(process_list,node,-1);
}

gint sortFunctionID(gpointer a,gpointer b){
  Process aa = a;
  Process bb = b;
  return aa->process_id - bb->process_id;
}

gint sortFunctionArrival(gpointer a,gpointer b){
  Process aa = a;
  Process bb = b;
  return aa->process_arrival - bb->process_arrival;
}

gint sortFunctionPriority(gpointer a,gpointer b){
  Process aa = a;
  Process bb = b;
  return aa->process_priority - bb->process_priority;
}

gint sortFunctionCpuBurst(gpointer a,gpointer b){
  Process aa = a;
  Process bb = b;
  return aa->process_burst - bb->process_burst;
}

GList * SortProcessList(GList * process_list,int sort){
  if(sort == ID)
    return g_list_sort(process_list,(GCompareFunc)sortFunctionID);
  if(sort==ARRIVAL)
    return g_list_sort(process_list,(GCompareFunc)sortFunctionArrival);
  if(sort == PRIORITY)
      return g_list_sort(process_list,(GCompareFunc)sortFunctionPriority);
  if(sort==CPUBURST)
      return g_list_sort(process_list,(GCompareFunc)sortFunctionCpuBurst);
}

void PrintProcessList(GList * process_list){
  GList *l;
  for(l=process_list;l!=NULL;l=l->next){
    Process node = l->data;
    printf("%d %d %d %d\n", node->process_id,node->process_arrival,node->process_burst,node->process_priority);
  }
}

Process copyFunction(gpointer src,gpointer data){
  Process original = src;
  Process copy = (Process)malloc(sizeof(struct process_p));
  copy->process_id = original->process_id;
  copy->process_arrival = original->process_arrival;
  copy->process_burst = original->process_burst;
  copy->process_priority = original->process_priority;
  copy->process_runtime = original->process_runtime;
  copy->process_lastruntime = original->process_lastruntime;
  return copy;
}

GList * CopyList(GList * process_list){
    return g_list_copy_deep(process_list,(GCopyFunc)copyFunction,NULL);
}

void freeNode(gpointer node){
  free(node);
}

void DestroyList(GList * process_list){
  g_list_free_full(process_list, freeNode);
}
