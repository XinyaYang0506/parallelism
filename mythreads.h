/*
 * Wrapper functions for part of the pthread libraries. Rather than return
 * error codes, any failures abort the program with a message to stderr.
 * 
 * mutex_init
 * mutex_lock
 * mutex_unlock
 * thread_create
 * thread_join
 *
 * Must be linked with -lpthread
 *
 * Mutex and thread routines adapted from similar routines by Remzi
 * Arpaci-Dusseau.
 *
 * Created by Jerod Weinman, 8 Oct 2012
 * Revised 11 August 2014
 * Further simplified 11 December 2018
 */
#ifndef __MYTHREADS_H__
#define __MYTHREADS_H__

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void 
mutex_init (pthread_mutex_t *mutex)
{
  int rc = pthread_mutex_init (mutex,NULL);

  if (rc) {
      fprintf (stderr, "Error initializing mutex: %s\n", strerror(rc));
      exit (EXIT_FAILURE);
    }
}

void
mutex_lock (pthread_mutex_t *m)
{
  int rc = pthread_mutex_lock (m);

  if (rc) {
    fprintf (stderr, "Error locking mutex: %s\n", strerror(rc));
    exit (EXIT_FAILURE);
  }
}
                                                                                
void
mutex_unlock (pthread_mutex_t *m)
{
  int rc = pthread_mutex_unlock (m);

  if (rc) {
    fprintf (stderr, "Error unlocking mutex: %s\n", strerror(rc));
    exit (EXIT_FAILURE);
  }
}
                                                                                
void
thread_create (pthread_t *thread, void *(*start_routine)(void*), void *arg)
{
  int rc = pthread_create (thread, NULL, start_routine, arg);

  if (rc) {
    fprintf (stderr,"Error creating thread: %s\n", strerror(rc));
    exit (EXIT_FAILURE);
  }
}

void
thread_join (pthread_t thread)
{
  int rc = pthread_join (thread, NULL);

  if (rc) {
    fprintf(stderr,"Error joining thread: %s\n", strerror(rc));
    exit(EXIT_FAILURE);
  }
}

#endif // __MYTHREADS_H__
