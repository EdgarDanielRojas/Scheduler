/*
 * Copyright (c) 2017 Edgar Daniel Rojas Vazquez
 *
 * File name:Process.c
 *
 * Author Edgar Daniel Rojas Vazquez
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

//!We make a typedef to facilitate declaration of process_p structures in the future
typedef struct process_p * Process;

/*! Declaration of the data structure process_p that will hold information about
 * each process that will run.
 */
struct process_p{
  int process_id;               //!< The id of the process
  int process_arrival;          //!< The arrival time of the process
  int process_burst;            //!< The cpu burst of the process
  int process_priority;         //!< The priority of the process
  int process_runtime;          //!< The total time the process has run
  int process_lastruntime;      //!< The last time the process started execution
  int process_remainingcycles;  //!< The cpu burst left in the process
};

/*! We declare an enum to facilitate the use of
 *  sort types and algorithm types we may use
 */
enum sort_type{ ID //!< Constant for ID
  , ARRIVAL //!< Constant for Arrival
  , PRIORITY //!< Constant for Priority
  , CPUBURST//!< Constant for Cpu burst
  };


/*!
* \brief Creates a process and adds it to the process list.
* \param process_list Pointer to the process list
* \param id The process id number
* \param arrival The process arrival time
* \param burst The process cpu burst
* \param priority The process priority
* \param algo Undefined input
* \details For this function to work the pointer to the process list must be
  of type GList as that is what we are using to organize our processes.
  The function takes in the information necesary for the creation of a process
  using the Process data structure. After allocating the necesary memory space
  and filling in the information we use the g_list_insert function included in the
  glib library to add it to the end of the list and return the pointer to the start of the new start
  of the list.
*/
GList * CreateProcess(GList * process_list,int id, int arrival,int burst,int priority,int algo){
    Process node = (Process)malloc(sizeof(struct process_p));
    node->process_id = id;
    node->process_arrival = arrival;
    node->process_burst = burst;
    node->process_priority = priority;
    node->process_runtime = 0;
    node->process_lastruntime = 0;
    node->process_remainingcycles = burst;
    return g_list_insert(process_list,node,-1);
}

/*!
* \brief Compares processes by ID.
*
* \param a Pointer to the data in GList a that it is comparing
* \param b Pointer to the data in GList b that it is comparing
*
  \details The function creates process pointers from the pointers it receives so that
  it may use the pointers it receives as process pointers. That way it can
  access the process_id parameter for comparison purposes.
*/
gint sortFunctionID(gpointer a,gpointer b){
  Process aa = a;
  Process bb = b;
  return aa->process_id - bb->process_id;
}

/*!
* \brief Compares processes by arrival time.
*
* \param a Pointer to the data in GList a that it is comparing
* \param b Pointer to the data in GList b that it is comparing
*
  \details The function creates process pointers from the pointers it receives so that
  it may use the pointers it receives as process pointers. That way it can
  access the process_arrival parameter for comparison purposes.
*/
gint sortFunctionArrival(gpointer a,gpointer b){
  Process aa = a;
  Process bb = b;
  return aa->process_arrival - bb->process_arrival;
}

/*!
* \brief Compares processes by priority.
*
* \param a Pointer to the data in GList a that it is comparing
* \param b Pointer to the data in GList b that it is comparing
*
* \details The function creates process pointers from the pointers it receives so that
  it may use the pointers it receives as process pointers. That way it can
  access the process_priority parameter for comparison purposes. In the case that two
  processes have the same priortiy we sort by the ID calling sortFunctionID().
*/
gint sortFunctionPriority(gpointer a,gpointer b){
  Process aa = a;
  Process bb = b;
  int compare= aa->process_priority - bb->process_priority;
  if(compare==0)
    compare=sortFunctionID(a,b);
  return compare;
}

/*!
* \brief Compares processes by cpu burst.
*
* \param a Pointer to the data in GList a that it is comparing
* \param b Pointer to the data in GList b that it is comparing
*
* \details The function creates process pointers from the pointers it receives so that
  it may use the pointers it receives as process pointers. That way it can
  access the process_burst parameter for comparison purposes. In the case that two
  processes have the same priortiy we sort by the ID calling sortFunctionID().
*/
gint sortFunctionCpuBurst(gpointer a,gpointer b){
  Process aa = a;
  Process bb = b;
  int compare= aa->process_remainingcycles - bb->process_remainingcycles;
  if(compare==0)
    compare=sortFunctionID(a,b);
  return compare;
  return compare;
}

/*!
* \brief Sorts a process list.
*
* \param process_list Pointer to the GList that's the head of the list
* \param sort Integer value representing what type of sort to apply
*
* \details This function receives the process list to sort and the type of
  sort to apply based on the enum sort_type. It returns the new head of the list
  using the function g_list_sort which takes in the current head of the list and
  a comparison function which can be any of the following depending on the
  sort type we want to use: sortFunctionID() , sortFunctionArrival() , sortFunctionPriority() or sortFunctionCpuBurst().
*/
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

/*!
* \brief Prints a process list.
*
* \param process_list Pointer to the GList that's the head of the list
*
* \details This function receives the process list head
  and using another GList pointer it goes one by  one and extracts the
  data (a process) from the pointer and prints out the process information.
  This inludes process_id , process_arrival , process_burst, process_priority, process_runtime, process_lastruntime and process_remainingcycles.
  It stops once it gets to the NULL GList node.
*/
void PrintProcessList(GList * process_list){
  GList *l;
  for(l=process_list;l!=NULL;l=l->next){
    Process node = l->data;
    printf("%d %d %d %d %d %d %d\n", node->process_id,node->process_arrival,node->process_burst,node->process_priority,node->process_runtime,node->process_lastruntime,node->process_remainingcycles);
  }
}

/*!
* \brief Copies a process.
*
* \param src Pointer to data that needs to be copied
* \param data User data (not used)
*
* \details This function takes the src pointer and uses it to create a pointer
  to a process so we can read it's properties. Next we allocate memory for a new
  process structure and copy all the information from the original to the copy.
  Finally we return the copy.
*/
Process copyFunction(gpointer src,gpointer data){
  Process original = src;
  Process copy = (Process)malloc(sizeof(struct process_p));
  copy->process_id = original->process_id;
  copy->process_arrival = original->process_arrival;
  copy->process_burst = original->process_burst;
  copy->process_priority = original->process_priority;
  copy->process_runtime = original->process_runtime;
  copy->process_lastruntime = original->process_lastruntime;
  copy->process_remainingcycles = original->process_remainingcycles;
  return copy;
}

/*!
* \brief Copies a process list.
*
* \param process_list Pointer to the head of the process list to be copied
*
* \details This function returns the head of a new process list completely
  identical to the input process list using the function g_list_copy_deep.
  This function takes as parameters the head of the list to be copied, a
  copy function and user data. The copy function is to be applied to every data
  pointer in the GList list and is called copyFunction().
*/
GList * CopyList(GList * process_list){
    return g_list_copy_deep(process_list,(GCopyFunc)copyFunction,NULL);
}

/*!
* \brief Frees memory of a process.
*
* \param node Pointer to the data in a GList
*
* \details This function frees or deallocates memory used by a given memory address
  (pointer) using the free() function.
*/
void freeNode(gpointer node){
  free(node);
}

/*!
* \brief Frees memory of a process list.
*
* \param process_list Pointer to the head of the process list to be copied
*
* \details This function frees or deallocates memory used by a process list
  using the g_list_free_full() function.This function not only deallocates memory
  for the GList but also for the data inside the GList. The function freeNode is called
  for every GList so that the data in it may be deallocated as well.
*/
void DestroyList(GList * process_list){
  g_list_free_full(process_list, freeNode);
}
