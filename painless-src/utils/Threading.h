// -----------------------------------------------------------------------------
// Copyright (C) 2017  Ludovic LE FRIOUX
//
// PaInleSS is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
// -----------------------------------------------------------------------------

#ifndef UTILS_THREADING_H
#define UTILS_THREADING_H

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/// Mutex class.
class Mutex
{
public:
   /// Constructor.
   Mutex()
   {
      pthread_mutex_init(&mtx, NULL);
   }

   /// Destructor.
   virtual ~Mutex()
   {
      pthread_mutex_destroy(&mtx);
   }

   /// Lock the mutex.
   void lock()
   {
      pthread_mutex_lock(&mtx);
   }

   /// Unlock the mutex.
   void unlock()
   {
      pthread_mutex_unlock(&mtx);
   }

   /// Try to lock the mutex, return true if succes else false.
   bool tryLock()
   {
      return pthread_mutex_trylock(&mtx) == 0;
   }

protected:
   /// A pthread mutex.
   pthread_mutex_t mtx;
};

/// Thread class
class Thread
{
public:
   /// Constructor.
   Thread(void * (*main)(void *), void * arg)
   {
      pthread_create(&threadId, NULL, main, arg);
   }

   /// Join the thread.
   void join()
   {
      pthread_join(threadId, NULL);
   }

protected:
   /// The id of the pthread.
   pthread_t threadId;
};

#endif // UTILS_THREADING_H
