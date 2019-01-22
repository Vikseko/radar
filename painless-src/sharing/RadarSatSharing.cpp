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


#include "../sharing/RadarSatSharing.h"
#include "../utils/Parameters.h"
#include "../clauses/ClauseManager.h"
#include "../solvers/SolverFactory.h"

RadarSatSharing::RadarSatSharing()
{
   literalPerRound = Parameters::getIntParam("shr-lit", 1500);
}

RadarSatSharing::~RadarSatSharing()
{
}

void
RadarSatSharing::doSharing(int idSharer, const vector<SolverInterface *> & from,
                           const vector<SolverInterface *> & to)
{
   sharingcount += 1;

   if(sharingcount == 10) {

      for (size_t i = 0; i < from.size(); i++) {
         int used, usedPercent, selectCount;
      
         tmp.clear();

         from[i]->getLearnedClauses(tmp);

         stats.receivedClauses += tmp.size();

         for (size_t k = 0; k < tmp.size(); k++) {
            std::vector<int> vecclause;
            std::ofstream out1("./vecclause.txt", std::ios::app);
            for(int s = 0; s < tmp[k]->size; s++)
            {
            	vecclause.push_back(tmp[k]->lits[s]);
            	    
					if (out1.is_open())
					{
    					out1 << tmp[k]->lits[s] << endl;
					}
            }
            out1 << "qwe" << endl;
			   out1.close();
            auto it = hashtable1.find(vecclause);
            if(it != hashtable1.end())
            {
               it->second++;
            }
            else
            {
               hashtable1[vecclause] = 1;
            }
            for(auto clause : hashtable1)
            {
               if(vecclause == clause.first && clause.second>1)
               {
               	  tmp[k]->repeat = false;
                  database.addClause(tmp[k]);
  
               }
            }
         }
         tmp.clear();

         for (int j = 0; j < to.size(); j++) {
            if (from[i]->id != to[j]->id) {
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
      hashtable1.clear();

   }
   else if (sharingcount < 10){
     
      for (size_t i = 0; i < from.size(); i++) {
         int used, usedPercent, selectCount;
      
         tmp.clear();

         from[i]->getLearnedClauses(tmp);

         stats.receivedClauses += tmp.size();

         for (size_t k = 0; k < tmp.size(); k++) {
         	tmp[k]->repeat = true;
            database.addClause(tmp[k]);
            std::vector<int> vecclause;
            for(int s = 0; s < tmp[k]->size; s++)
            {
               vecclause.push_back(tmp[k]->lits[s]);
            }
            auto it = hashtable1.find(vecclause);
            if(it != hashtable1.end())
            {
               it->second++;
            }
            else
            {
               hashtable1[vecclause] = 1;
            }
         }
         tmp.clear();

         used        = database.giveSelection(tmp, literalPerRound, &selectCount);
         usedPercent = (100 * used) / literalPerRound;

         stats.sharedClauses += selectCount;

         if (usedPercent < 80) {
            from[i]->increaseClauseProduction();
         }

         for (int j = 0; j < to.size(); j++) {
            if (from[i]->id != to[j]->id) {
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
   else{
   	   for (size_t i = 0; i < from.size(); i++) {
      int used, usedPercent, selectCount;
      
      tmp.clear();

      from[i]->getLearnedClauses(tmp);

      stats.receivedClauses += tmp.size();

      for (size_t k = 0; k < tmp.size(); k++) {
         database.addClause(tmp[k]);
         std::cout << tmp[k]->lits;
      }

      tmp.clear();

      used        = database.giveSelection(tmp, literalPerRound, &selectCount);
      usedPercent = (100 * used) / literalPerRound;

      stats.sharedClauses += selectCount;

      if (usedPercent < 80) {
         from[i]->increaseClauseProduction();
      }

      for (int j = 0; j < to.size(); j++) {
         if (from[i]->id != to[j]->id) {
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

SharingStatistics
RadarSatSharing::getStatistics()
{
   return stats;
}


/*                    std::ofstream out("./sharing.txt", std::ios::app);
					if (out.is_open())
					{
    				out << *tmp[k]->lits;
					}
		out << endl;
		out.close();*/