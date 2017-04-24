#include <iostream>
#include <cmath>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mat.h"
//#include "rand.h"

using namespace std;

Matrix sb, sw;

double randomWithin(double min, double max) {
   double f = (double)rand() / RAND_MAX;
   return min + f * (max - min);
   //return (rand() % max-min)+min;
}

Matrix initializePoints(Matrix rawInputs, int k) {
   //cout << "initializing..." << endl;
   Matrix pointList(k, rawInputs.maxCols());
   pointList.constant(0);

   for(int i=0; i<pointList.numCols(); i++) {
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

vector<int> assignPoints(Matrix distances) {
   //cout << "assigning" << endl;
   const int k = distances.numRows();
   vector<int> assignments;
   for(int i=0; i<k; i++) {
      int min = 0;
      for(int j=1; j<distances.numCols(); j++) {
         //cout << distances.get(i, min) << " >? " << distances.get(i, j) << " ";
         if(distances.get(i, min) > distances.get(i, j))
	    min = j;
      }
      //cout << endl;
      assignments.push_back(min);
   }
   return assignments;
}

void printArray(vector<int> array) {
   for(int i=0; i<array.size(); i++) {
      cout << array[i] << endl;
   }
}

Matrix setNewCenter(Matrix centerPoints, Matrix rawInputs, int row) {
   //cout << "setting new center" << endl;
   Matrix newPoint = initializePoints(rawInputs, 1);
   newPoint.setName("newPoint");
   for(int i=0; i<centerPoints.numCols(); i++) {
      centerPoints.set(row, i, newPoint.get(0, i));
   }
   //cout << "done" << endl;
   return centerPoints;
}

vector<int> assertKClouds(Matrix centerPoints,
                          vector<int> assignments) {
   //cout << "asserting" << endl;
   const int k = centerPoints.numRows();
   vector<int> asserted;
   for(int i=0; i<k; i++) {
      asserted.push_back(0);
   }
   for(int i=0; i<assignments.size(); i++) {
      int point = assignments[i];
      //cout << point << endl;
      asserted[point]=1;
   }
   return asserted;
}

Matrix generateDistances(Matrix rawInputs, Matrix centerPoints) {
   //cout << "generating Distances..." << endl;
   const int k = centerPoints.numRows();
   Matrix distances(rawInputs.numRows(), k);
   distances.constant(0);
   //cout << "rawInputs.numRows(): " << rawInputs.numRows() << endl;

   for(int i=0; i<rawInputs.numRows(); i++) {
      for(int j=0; j<k; j++) {
         //cout << "i: " << i << " j: " << j << endl;
         //double distance = centerPoints.dist2(j, i, rawInputs.transpose());
	 double distance = rawInputs.dist2(i, j, centerPoints.transpose());
         distance = sqrt(distance);
         distances.set(i, j, distance);
      }
   }
   //cout << "done" << endl;
   return distances;
}

Matrix reInitPoints(Matrix centerPoints, 
                     vector<int> assignments,
		     Matrix rawInputs) {
   //cout << "reinit" << endl;
   const int k = centerPoints.numRows();
   vector<int> asserted = assertKClouds(centerPoints, assignments);
   //cout << "asserted?" << endl; printArray(asserted);
   for(int i=0; i<k; i++) {
      //cout << "i: " << i << endl;
      while(asserted[i] == 0) {
         //cout << "retrying: " << i << endl;
         centerPoints = setNewCenter(centerPoints, rawInputs, i);
	 Matrix distances = generateDistances(rawInputs, centerPoints);
         assignments = assignPoints(distances);
         asserted = assertKClouds(centerPoints, assignments);
         //cout << "asserted?" << endl; printArray(asserted);
	 i=0;
      }
   }
   //cout << "asserted?" << endl; printArray(asserted);
   return centerPoints;
}

int sizeOfCloud(vector<int> assignments, int kValue) {
   int size = 0;
   for(int i=0; i<assignments.size(); i++) {
      if(assignments[i] == kValue)
         size++;
   }
   return size;
}

Matrix generateCloud( Matrix rawInputs, vector<int> assignments, int kValue) {
   //cout << "generating cloud" << endl;
   int size = sizeOfCloud(assignments, kValue);
   Matrix cloud(size, rawInputs.numCols());
   cloud.constant(0);
   int r=0;
   //cout << size << endl;
   for(int i=0; i<rawInputs.numRows(); i++) {
	    //cout << "i: " << i /*<< " j: " << j*/ << " r: " << r << endl;
      if(assignments[i] == kValue) {
         for(int j=0; j<rawInputs.numCols(); j++) {
	    //cout << "i: " << i << " j: " << j << " r: " << r << endl;
	    cloud.set(r, j, rawInputs.get(i, j));
	 }
         r++;
      }
   }
   return cloud;
}

double probability(Matrix subset, Matrix rawInputs){
   //cout << subset.numRows() << " " << rawInputs.numRows() << endl;
   //cout << (double)subset.numRows()/(double)rawInputs.numRows() << endl;
   return (double)subset.numRows()/(double)rawInputs.numRows();
}

Matrix SW(vector<Matrix> covariances){
   Matrix sum = covariances[0];
   for(int i=1; i<covariances.size(); i++){
      sum.add(covariances[i]);
   }
   return sum;
}

vector<double> allPointsMean(Matrix rawInputs){
   vector<double> u;
   for(int i=0; i<rawInputs.numCols(); i++){
      u.push_back(rawInputs.meanCol(i));
   }
   return u;
}

Matrix movePoints(Matrix centerPoints, 
                  Matrix rawInputs) {
   //cout << "moving points" << endl;
   const int k = centerPoints.numRows();
   //distances.print();

   //printArray(assignments); cout << "asserted?" << endl;

   // check for points with no clouds.
   //cout << "checking points..." << endl;
   Matrix distances = generateDistances(rawInputs, centerPoints);
   vector<int> assignments = assignPoints(distances);
   //cout << "first points asserted..." << endl;
   //cout << "assignments: " << endl; printArray(assignments);

   vector<Matrix> clouds;
   vector<Matrix> covariances;
   for(int i=0; i<k; i++) {
      clouds.push_back(generateCloud(rawInputs, assignments, i));
      covariances.push_back(clouds[i].cov());
      //covariances[i].print();
      covariances[i].scalarMult(probability(clouds[i], rawInputs));
      //covariances[i].print();
      //cloud.print();
      for(int j=0; j<rawInputs.numCols(); j++) {
         centerPoints.set(i, j, clouds[i].meanCol(j));
      }
   }
   sw = SW(covariances);
   sw.print();


   return centerPoints;
}

int main()
{
   //initRand();
   srand(time(NULL));
   
   int k, loops, numCols, numRows;
   cin >> k >> loops >> numRows >> numCols;

   double inputArray[numRows*numCols];
   for(int i=0; i<numRows*numCols; i++) {
      cin >> inputArray[i];
   }

   Matrix rawInputs(numRows, numCols, inputArray);
   rawInputs.setName("rawInputs");
   //rawInputs.print();

   //cout << "initializing..." << endl;
   Matrix pointList = initializePoints(rawInputs, k);
   pointList.setName("pointList");
   pointList.print();
   //cout << "generating distances" << endl;
   Matrix distances = generateDistances(rawInputs, pointList);
   distances.setName("distances");
   //cout << "assigning" << endl;
   vector<int> assignments = assignPoints(distances);
   //printArray(assignments);
   //cout << "checking" << endl;
   pointList = reInitPoints(pointList, assignments, rawInputs);

   for(int i=0; i<1; i++) {
      //cout << "loop" << endl;
      pointList = movePoints(pointList, rawInputs);
      pointList.print();
   }
}
