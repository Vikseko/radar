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


#include "../solvers/SolverFactory.h"
#include "../utils/Parameters.h"
#include "../utils/System.h"

void
SolverFactory::sparseDiversification(const vector<SolverInterface *> & solvers)
{
   if (solvers.size() <= 0)
      return;

   int vars = solvers[0]->getVariablesCount();

   for (int sid = 0; sid < solvers.size(); sid++) {
      for (int var = 1; var + solvers.size() < vars; var += solvers.size()) {
         solvers[sid]->setPhase(var + sid, true);
      }
   }
}

void
SolverFactory::randomDiversification(const vector<SolverInterface *> & solvers,
                                     unsigned int seed)
{
   if (solvers.size() <= 0)
      return;

   srand(seed);

   int vars = solvers[0]->getVariablesCount();

   for (int sid = 0; sid < solvers.size(); sid++) {
      for (int var = 1; var <= vars; var++) {
         solvers[sid]->setPhase(var, rand()%2 == 1);
      }
   }
}

void
SolverFactory::sparseRandomDiversification(const vector<SolverInterface *> &
                                           solvers)
{
   if (solvers.size() <= 0)
      return;

   int vars = solvers[0]->getVariablesCount();

   for (int sid = 0; sid < solvers.size(); sid++) {
      srand(sid);
      for (int var = 1; var <= vars; var++) {
         if (rand() % solvers.size() == 0) {
            solvers[sid]->setPhase(var, rand() % 2 == 1);
         }
      }
   }
}

void
SolverFactory::nativeDiversification(const vector<SolverInterface *> & solvers)
{
   for (int sid = 0; sid < solvers.size(); sid++) {
      solvers[sid]->diversify(sid);
   }
}

void
SolverFactory::binValueDiversification(const vector<SolverInterface *> &
                                       solvers)
{
   if (solvers.size() <= 0)
      return;

   int tmp = solvers.size();
   int log = 0;

   while (tmp > 0) {
      tmp >>= 1;
      log++;
   }

   int vars = solvers[0]->getVariablesCount();

   for (int sid = 0; sid < solvers.size(); sid++) {
      for (int var = 1; var < vars; var++) {
         int bit    = var % log;
         bool phase = (sid >> bit) & 1 ? true : false;

         solvers[sid]->setPhase(var, phase);
      }
   }
}

SolverInterface *
SolverFactory::createMapleSolver()
{
   int id = currentIdSolver.fetch_add(1);

   SolverInterface * solver = new Maple(id);

   solver->loadFormula(Parameters::getFilename());

   return solver;
}

void
SolverFactory::createMapleSolvers(int maxSolvers,
                                  vector<SolverInterface *> & solvers)
{
   solvers.push_back(createMapleSolver());

   double memoryUsed    = getMemoryUsed();
   int maxMemorySolvers = Parameters::getIntParam("max-memory", 58) * 1024 *
                          1024 / memoryUsed;

   if (maxSolvers > maxMemorySolvers) {
      maxSolvers = maxMemorySolvers;
   }

   for (int i = 1; i < maxSolvers; i++) {
      solvers.push_back(createMapleSolver());
   }
}

void
SolverFactory::printStats(const vector<SolverInterface *> & solvers)
{
   printf("c | ID | conflicts  | propagations |  restarts  | decisions  " \
          "| memPeak |\n");

   for (size_t i = 0; i < solvers.size(); i++) {
      SolvingStatistics stats = solvers[i]->getStatistics();

      printf("c | %2zu | %10ld | %12ld | %10ld | %10ld | %7d |\n",
             solvers[i]->id, stats.conflicts, stats.propagations,
             stats.restarts, stats.decisions, (int)stats.memPeak);
   }
}