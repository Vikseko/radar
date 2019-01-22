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


#ifndef UTILS_SAT_UTILS_H
#define UTILS_SAT_UTILS_H

#include "../solvers/SolverInterface.h"

/// Print the model correctly in stdout.
void printModel(vector<int> & model);

/// Load the cnf contains in the file to the solver.
bool loadFormulaToSolvers(vector<SolverInterface*> solvers,
                          const char* filename);

#endif // UTILS_SAT_UTILS_H
