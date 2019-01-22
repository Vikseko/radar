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

#include "painless.h"

#include "utils/Parameters.h"
#include "utils/System.h"
#include "utils/SatUtils.h"

#include "solvers/SolverFactory.h"

#include "clauses/ClauseManager.h"

#include "sharing/HordeSatSharing.h"
#include "sharing/Sharer.h"

#include "working/SequentialWorker.h"
#include "working/Portfolio.h"

#include <unistd.h>
#include <math.h>

using namespace std;


// -------------------------------------------
// Declaration of global variables
// -------------------------------------------
atomic<bool> globalEnding(false);

Sharer ** sharers = NULL;

int nSharers = 0;

WorkingStrategy * working = NULL;

SatResult finalResult = UNKNOWN;

vector<int> finalModel;


// -------------------------------------------
// Main du framework
// -------------------------------------------
int main(int argc, char ** argv)
{
   Parameters::init(argc, argv);
   
   if (Parameters::getFilename() == NULL ||
       Parameters::getBoolParam("h"))
   {
      cout << "USAGE: " << argv[0] << " [options] input.cnf" << endl;
      cout << "Options:" << endl;
      cout << "\t-c=<INT>\t\t number of cpus, default is 4." << endl;
      cout << "\t-max-memory=<INT>\t memory limit in Go, default is 60." << \
	      endl;
      cout << "\t-lbd-limit=<INT>\t LBD limit of exported clauses, default is" \
	      " 2" << endl;
      cout << "\t-shr-sleep=<INT>\t time in usecond a sharer sleep each round" \
	      ", default is 1500000 (1.5s)" << endl;
      cout << "\t-shr-lit=<INT>\t\t number of literals shared per round by " \
	      "the hordesat strategy, default is 1500" << endl;
      return 0;
   }

   Parameters::printParams();

   int cpus = Parameters::getIntParam("c", 48);

   // Create solvers
   vector<SolverInterface *> solvers;
   vector<SolverInterface *> solvers_VSIDS;
   vector<SolverInterface *> solvers_LRB;

   SolverFactory::createMapleSolvers(cpus, solvers);

   int nSolvers = solvers.size();
   
   SolverFactory::nativeDiversification(solvers);

   for (size_t i = 0; i < nSolvers; i++) {
      if (i % 2) {
         solvers_VSIDS.push_back(solvers[i]);
      } else {
         solvers_LRB.push_back(solvers[i]);
      }
   }

   SolverFactory::sparseRandomDiversification(solvers_LRB);
   SolverFactory::sparseRandomDiversification(solvers_VSIDS);

   cout << "c " << nSolvers << " solvers are used, " << solvers_LRB.size() << \
	   " with LRB and " << solvers_VSIDS.size() << " with VSIDS" << endl;

   vector<SolverInterface *> from;
   // Start sharing threads
   nSharers = nSolvers;
   sharers  = new Sharer*[nSharers];

   for (size_t i = 0; i < nSharers; i++) {
      from.clear();
      from.push_back(solvers[i]);
      sharers[i] = new Sharer(i, new HordeSatSharing(), from, solvers);
   }

   working = new Portfolio();
   for (size_t i = 0; i < nSolvers; i++) {
      working->addSlave(new SequentialWorker(solvers[i]));
   }

   // Init the management of clauses
   ClauseManager::initClauseManager();

   // Launch working
   vector<int> cube;
   working->solve(cube);

   // Wait until end or timeout
   int timeout = Parameters::getIntParam("t", -1);

   while(globalEnding == false) {
      sleep(1);

      if (timeout > 0 && getRelativeTime() >= timeout) {
         globalEnding = true;
         working->setInterrupt();
      }
   }

   // Delete sharers
   for (int i=0; i < nSharers; i++) {
      delete sharers[i];
   }
   delete sharers;

   // Print solver stats
   SolverFactory::printStats(solvers);

   // Delete working strategy
   delete working;

   // Delete shared clauses
   ClauseManager::joinClauseManager();

   cout << "c Resolution time: " << getRelativeTime() << "s" << endl;

   // Print the result and the model if SAT
   if (finalResult == SAT) {
      cout << "s SATISFIABLE" << endl;

      if (Parameters::getBoolParam("no-model") == false) {
         printModel(finalModel);
      }
   } else if (finalResult == UNSAT) {
      cout << "s UNSATISFIABLE" << endl;
   } else {
      cout << "s UNKNOWN" << endl;
   }

   return finalResult;
}
