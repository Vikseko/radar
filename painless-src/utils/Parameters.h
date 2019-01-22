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

#ifndef UTILS_PARAMETERS_H
#define UTILS_PARAMETERS_H

#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <unordered_map>

using namespace std;

static unordered_map<string, string> parameters;
static char * filename;

/// Class to manage the parameters
class Parameters
{
public:
   /// Init the parameters.
   static void init(int argc, char ** argv)
   {
      for (int i = 1; i < argc; i++) {
         char * current = argv[i];

         if (current[0] != '-' && filename == NULL) {
            filename = current;
            continue;
         }

         char * equalPtr = strchr(current, '=');

         if (equalPtr == NULL) {
            parameters[current + 1];
         } else {
            *equalPtr = 0;

            char * left  = current + 1;
            char * right = equalPtr + 1;

            parameters[left] = right;
         }
      }
   }

   /// Get the input cnf filename.
   static char * getFilename()
   {
      static char * returnPtr = filename;
      return returnPtr;
   }

   /// Print all parameters.
   static void printParams()
   {
      printf("c filename %s\n", filename);

      cout << "c ";

      for (auto & pair : parameters) {
         if (pair.second.empty()) {
            cout << pair.first << ", ";
         } else {
            cout << pair.first << "=" << pair.second << ", ";
         }
      }

      cout << endl;
   }

   /// Return true if the parameters is set otherwise false.
   static bool getBoolParam(const string & name)
   {
      return parameters.find(name) != parameters.end();
   }

   /// Get the string value of a parameters with a default value.
   static const string getParam(const string & name,
                                const string & defaultValue)
   {
      if (getBoolParam(name))
         return parameters[name];

      return defaultValue;
   }

   /// Get the string value of a parameter.
   static const string getParam(const string & name)
   {
      return getParam(name, "");
   }

   /// Get the int value of a parameter with a default value.
   static int getIntParam(const string & name, int defaultValue)
   {
      if (getBoolParam(name))
         return atoi(parameters[name].c_str());

      return defaultValue;
   }
};

#endif // UTILS_PARAMETERS_H
