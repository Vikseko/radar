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


#ifndef WORKING_SEQUENTIAL_WORKER_H
#define WORKING_SEQUENTIAL_WORKER_H

#include "../utils/Threading.h"
#include "../working/WorkingStrategy.h"
#include "../solvers/SolverInterface.h"

#include <vector>

using namespace std;

// Main executed by worker threads
static void * mainWorker(void * arg);

class SequentialWorker : public WorkingStrategy
{
public:
   SequentialWorker(SolverInterface * solver_);
   
   ~SequentialWorker();

   void solve (const vector<int> & cube);

   void join(WorkingStrategy * winner, SatResult res,
             const vector<int> & model);

   void setInterrupt();

   void unsetInterrupt();

   void waitInterrupt();
   
   int getDivisionVariable();

   void setPhase(const int var, const bool phase);

   void bumpVariableActivity(const int var, const int times);

   SolverInterface * solver;

protected:
   friend void * mainWorker(void * arg);

   Thread * worker;

   vector<int> actualCube;

   atomic<bool> force;
   
   atomic<bool> waitJob;

   Mutex waitInterruptLock;

   pthread_mutex_t mutexStart;
   pthread_cond_t  mutexCondStart;
};

#endif // SEQUENTIAL_WORKER_H

