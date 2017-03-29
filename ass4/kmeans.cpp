#include <iostream>
#include <fstream>
#include "mat.h"

using namespace std;

double randomWithin(int min, int max) {
   return randMod(max-min)+min;
}

Matrix initializePoints(Matrix rawInputs, int k) {
   Matrix pointList(k, rawInputs.maxCols());
   pointList.constant(0);

   for(int i=0; i<pointList.maxCols(); i++) {
      double min = rawInputs.minCol(i);
      double max = rawInputs.maxCol(i);
      //cout << "min: " << min << endl;
      //cout << "max: " << max << endl;
      for(int j=0; j<pointList.maxRows(); j++) {
         pointList.set(j, i, randomWithin(min, max));
      }
   }

   return pointList;
}

int main()
{
   initRand();
   
   int k, loops, numCols, numRows;
   cin >> k >> loops >> numRows >> numCols;

   double inputArray[numRows*numCols];
   for(int i=0; i<numRows*numCols; i++) {
      cin >> inputArray[i];
   }

   Matrix rawInputs(numRows, numCols, inputArray);
   rawInputs.setName("rawInputs");
   rawInputs.print();

   Matrix pointList = initializePoints(rawInputs, k);
   pointList.setName("pointList");
   pointList.print();
}
