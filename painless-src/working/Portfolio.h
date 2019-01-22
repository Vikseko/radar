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


#ifndef WORKING_PORTFOLIO_H
#define WORKING_PORTFOLIO_H

#include "../working/WorkingStrategy.h"
#include "../utils/Parameters.h"

using namespace std;

class Portfolio : public WorkingStrategy
{
public:
   Portfolio();

   ~Portfolio();

   void solve(const vector<int> & cube);

   void join(WorkingStrategy * strat, SatResult res,
             const vector<int> & model);
  
   void setInterrupt();

   void unsetInterrupt();

   void waitInterrupt();

   int getDivisionVariable();

   void setPhase(int var, bool value);

   void bumpVariableActivity(int var, int times);

protected:
   atomic<bool> strategyEnding;
};

#endif // WORKING_PORTFOLIO_H
