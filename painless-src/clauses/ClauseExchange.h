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


#ifndef CLAUSES_CLAUSE_EXCHANGE_H
#define CLAUSES_CLAUSE_EXCHANGE_H

#include <atomic>

using namespace std;

/// Structure of a shared clause.
typedef struct ClauseExchange
{
   /// Number of references on this clause.
   atomic<int> nbRefs;

   /// LBD value of this clause.
   int lbd;
   
   /// Id of the solver that has exported this clause.
   int from;

   /// Size of this clause.
   int size;

   /// Literals of this clause.
   int lits[0];

} ClauseExchange;

#endif // CLAUSES_CLAUSE_EXCHANGE_H
