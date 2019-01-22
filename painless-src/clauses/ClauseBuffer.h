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


#ifndef CLAUSES_CLAUSE_BUFFER_H
#define CLAUSES_CLAUSE_BUFFER_H

#include "../clauses/ClauseExchange.h"

#include <stdio.h>
#include <memory>
#include <atomic>
#include <vector>


using namespace std;

/// Clause buffer is a queue containning shared clauses.
class ClauseBuffer
{
public:
   /// Constructor.
   ClauseBuffer();

   /// Destructor.
   ~ClauseBuffer();

   /// Enqueue a shared clause to the buffer.
   void addClause (ClauseExchange * clause);

   /// Enqueue shared clauses to the buffer.
   void addClauses(const vector<ClauseExchange *> & clauses);

   /// Dequeue a shared clause.
   bool getClause (ClauseExchange ** clause);

   /// Dequeue shared clauses.
   void getClauses(vector<ClauseExchange *> & clauses);

   /// Return the current size of the buffer
   int size();

protected:
   typedef struct ListElement
   {
      ClauseExchange * clause;

      atomic<ListElement *> next;         

      ListElement(ClauseExchange * cls)
      {
         next   = NULL;
         clause = cls;
      }

      ~ListElement()
      {
      }
   } ListElement;

   typedef struct ListRoot
   {
      atomic<int> size;

      atomic<ListElement *> head;
      atomic<ListElement *> tail;
   } ListRoot;

   /// Root of producer/customers lists
   ListRoot buffer;
};

#endif // CLAUSES_CLAUSE_BUFFER_H
