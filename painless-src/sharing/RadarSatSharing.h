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


#ifndef SHARING_RADAR_SHARING_H
#define SHARING_RADAR_SHARING_H

#include "../clauses/ClauseFilter.h"
#include "../sharing/SharingStrategy.h"
#include "../solvers/SolverInterface.h"

#include <vector>
#include <unordered_map>
#include <sstream>
#include <array>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime> 


/// This strategy is a hordesat like sharing strategy.
class RadarSatSharing : public SharingStrategy
{
public:
   /// Constructor.
   RadarSatSharing();

   /// Destructor.
   ~RadarSatSharing();

   /// This method shared clauses from the producers to the consumers.
   void doSharing(int idSharer, const vector<SolverInterface *> & from,
                  const vector<SolverInterface *> & to);

   /// Return the sharing statistics of this sharng strategy.
   SharingStatistics getStatistics();
    struct VectorHash {
      size_t operator()(const std::vector<int>& v) const {
        std::hash<int> hasher;
        size_t seed = 0;
        for (int i : v) {
            seed ^= hasher(i) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
        return seed;
      }
    };
    /// Hashtable for repeateble clauses
    typedef std::unordered_map<vector<int>, int, VectorHash> hashtable;
    hashtable hashtable1;

    /// Count for sharing with repeats
    int sharingcount = 0;

protected:
   /// Number of shared literals per round
   int literalPerRound;

   /// Filter used to store the clauses.
   ClauseFilter database;

   /// Sharing statistics.
   SharingStatistics stats;

   /// Used to manipulate clauses.
   vector<ClauseExchange *> tmp;   
};

#endif /* SHARING_HORDESAT_SHARING_H */
