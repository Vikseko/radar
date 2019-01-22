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

#ifndef BOBSAT_H
#define BOBSAT_H

#include "solvers/SolverInterface.h"
#include "working/WorkingStrategy.h"
#include "sharing/Sharer.h"

#include <vector>
#include <atomic>

using namespace std;

/// Is it the end of the search
extern atomic<bool> globalEnding;

/// Working strategy
extern WorkingStrategy * working;

/// Array of sharers
extern Sharer ** sharers;

/// Size of the array of sharers
extern int nSharers;

/// Final result
extern SatResult finalResult;

/// Model for SAT instances
extern vector<int> finalModel;

#endif
