#include <iostream>
#include <string>

#include "mat.h"
#include "rand.h"
#include "ktree.h"

using namespace std;

vector<string> features;
vector< vector<string> > featureValues;
vector< vector<string> > data;

void printVector( vector< vector<string> > features )
{
   for(int i=0; i<features.size(); i++){
      for(int j=0; j<features[i].size(); j++){
         cout << features[i][j] << " ";
      }
      cout << endl;
   }
}

void readProblem()
{
   int numFeatures, numFeatureValues, numExamples;
   string input;
   cin >> numFeatures;
   for(int i=0; i<numFeatures+1; i++) {
      cin >> input;
      features.push_back(input);
      featureValues.push_back(vector<int>());
      cin << numFeatureValues;
      for(int j=0; j<numFeatureValues; j++) {
         cin >> input;
	 featureValues[i].push_back(input);
      }
   }
   printVector(featureValues);
}

double gain()
{
}

Tree *ID3BuildTree()
{
}

int main()
{
   readProblem();
   //Tree *DTree = ID3BuildTree();
   //DTree->printAux();

   /*Tree* decisionTree = new Tree("decision Tree");
   decisionTree->addChild("child", new Tree("here i am"));
   decisionTree->addChild("child", new Tree("here i am"));
   decisionTree->addChild("child", new Tree("here i am"));
   cout << decisionTree->numChildren() << endl;
   decisionTree->printEdge();
   Tree* secondTree = new Tree("second Tree", 
                               "first edge",
			       new Tree("child 1"),
			       "second edge",
			       new Tree("child 2"));
   secondTree->printEdge();
   */
}
