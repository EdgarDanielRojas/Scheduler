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

void FirstCome(GList * process_list);

void NonPreemptive(GList * process_list,int type);

void Preemptive(GList * process_list,int type);

void RoundRobin(GList * process_list, int quantum);
