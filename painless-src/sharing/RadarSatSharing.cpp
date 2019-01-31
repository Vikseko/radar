// -----------------------------------------------------------------------------
// Copyright (C) 2017  Ludovic LE FRIOUX
//
// This file is part of PaInleSS.
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

#include "../sharing/RadarSatSharing.h"
#include "../utils/Parameters.h"
#include "../clauses/ClauseManager.h"
#include "../solvers/SolverFactory.h"

RadarSatSharing::RadarSatSharing()
{
}

void
RadarSatSharing::doSharing(int idSharer, const vector<SolverInterface *> & from,
                         const vector<SolverInterface *> & to)
{
   sharingcount ++;   ///nomer sharinga


   if(sharingcount = 100) {    ///Esli uzhe sharing №100, to obmenivaemsya povtorami
      for (int i = 0; i < from.size(); i++) {
         tmp.clear();

         from[i]->getLearnedClauses(tmp);

         stats.receivedClauses += tmp.size();
         stats.sharedClauses   += tmp.size();
         
         /// Zanosim clauses v hashtable
         for (size_t k = 0; k < tmp.size(); k++) {  
            tmp[k]->repeat = true; 

            ///Clause for hashtable
            ClauseExchange cfh = *tmp[k];

            auto it = hashtable1.find(cfh);
            if(it != hashtable1.end())
            {
               it->second++;
            }
            else
            {
               hashtable1[cfh] = 1;
            }
         }

         tmp.clear(); /// Chistim tmp, chtobi zapolnit' povtorami


         /// Zapolnyaem tmp povtorami
         for(auto clause : hashtable1)
         {
            if(clause.second>1)
            {
                 tmp.push_back(&clause.first);
            }
         }

         /// Sam sharing
         for (size_t j = 0; j < to.size(); j++) {
            for (size_t k = 0; k < tmp.size(); k++) {
               ClauseManager::increaseClause(tmp[k], 1);
            }
            to[j]->addLearnedClauses(tmp);
         }
            
         for (size_t k = 0; k < tmp.size(); k++) {
            ClauseManager::releaseClause(tmp[k]);
         }

      }
   }
   else if(sharingcount>100){     ///Esli sharing bol'she chem №100, to ne ispolsuem hashtable i obmenivaemsya po sheme "vsyo vsem"

         if(sharingcount==101){   ///chistim hashtable, ona nam bol'she ne nuzhna
            hashtable1.clear();
         }

         for (int i = 0; i < from.size(); i++) {
         tmp.clear();

         /// Ukazivaem chto eto ne povtori
         from[i]->getLearnedClauses(tmp);
         for (size_t k = 0; k < tmp.size(); k++) {
            tmp[k]->repeat = true;
         }

         stats.receivedClauses += tmp.size();
         stats.sharedClauses   += tmp.size();

         /// Sam sharing
         for (size_t j = 0; j < to.size(); j++) {
            if (i != j) {
               for (size_t k = 0; k < tmp.size(); k++) {
                  ClauseManager::increaseClause(tmp[k], 1);
               }
               to[j]->addLearnedClauses(tmp);
            }
         }
            
         for (size_t k = 0; k < tmp.size(); k++) {
            ClauseManager::releaseClause(tmp[k]);
         }
      }
   }

   else if (sharingcount<100){ ///Esli sharing men'she chem №100, to zapolnyaem hashtable, no obmenivaemsya po sheme "vsyo vsem"
         for (int i = 0; i < from.size(); i++) {
         tmp.clear();

         from[i]->getLearnedClauses(tmp);

         stats.receivedClauses += tmp.size();
         stats.sharedClauses   += tmp.size();



         /// Zanosim clauses v hashtable
         for (size_t k = 0; k < tmp.size(); k++) {
            tmp[k]->repeat = true;

            ///Clause for hashtable
            ClauseExchange cft = *tmp{k};

            auto it = hashtable1.find(cft);
            if(it != hashtable1.end())
            {
               it->second++;
            }
            else
            {
               hashtable1[cft] = 1;
            }
         }

         /// Sam sharing
         for (size_t j = 0; j < to.size(); j++) {
            if (i != j) {
               for (size_t k = 0; k < tmp.size(); k++) {
                  ClauseManager::increaseClause(tmp[k], 1);
               }
               to[j]->addLearnedClauses(tmp);
            }
         }
            
         for (size_t k = 0; k < tmp.size(); k++) {
            ClauseManager::releaseClause(tmp[k]);
         }
      }

   }
}


