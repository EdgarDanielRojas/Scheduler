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
 typedef struct process_p * Process;

 struct process_p{
   int process_id;
   int process_arrival;
   int process_burst;
   int process_priority;
   int process_runtime;
   int process_lastruntime;
 };
 
GList * CreateProcess(GList * process_list,int id, int arrival,int burst,int priority,int algo);

gint sortFunction(gconstpointer a,gconstpointer b);

GList * SortProcessList(GList * process_list);

void PrintProcessList(GList * process_list);

Process copyFunction(gconstpointer src,gpointer data);

GList * CopyList(GList * process_list);
