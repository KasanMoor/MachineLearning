#include <iostream>
#include <string>

#include "mat.h"
#include "rand.h"
#include "ktree.h"

using namespace std;

vector<string> features;
vector< vector<string> > featureValues;
vector< vector<string> > data;

void print2dVector( vector< vector<string> > features )
{
   for(int i=0; i<features.size(); i++){
      for(int j=0; j<features[i].size(); j++){
         cout << features[i][j] << " ";
      }
      cout << endl;
   }
}

void printVector(vector<string> features)
{
   for(int i=0; i<features.size(); i++) {
      cout << features[i] << " ";
   }
   cout << endl;
}

void readProblem()
{
   int numFeatures, numFeatureValues, numExamples;
   string input;
   cin >> numFeatures;
   for(int i=0; i<numFeatures+1; i++) {
      cin >> input;
      features.push_back(input);
      featureValues.push_back(vector<string>());
      cin >> numFeatureValues;
      for(int j=0; j<numFeatureValues; j++) {
         cin >> input;
	 featureValues[i].push_back(input);
      }
   }
   printVector(features);
   print2dVector(featureValues);
   cin >> numExamples;
   for(int i=0; i<numExamples; i++){
      data.push_back(vector<string>());
      for(int j=0; j<numFeatures+1; j++){
         cin >> input;
         data[i].push_back(input);
      }
   }
   print2dVector(data);
}

int count(int feature, int value)
{
   int num = 0;
   for(int d=0; d<data.size(); d++){
      if(data[d][feature] == featureValues[feature][value])
         num++;
   }
   return num;
}
/*
int isOneLabel(vector< vector<string> > feature)
{
   int num = data.size();
   for(int i=0; i<featureValues[feature].size(); i++){
      if(count(feature, i) == num)
         return i;
   }
   return -1;
}
*/
vector< vector<string> > select(int feature, int value)
{
   vector< vector<string> > selection;
   for(int i=0; i<data.size(); i++){
      if(data[i][feature] == featureValues[feature][value])
         selection.push_back(data[i]);
   }
   return selection;
}

double entropy(vector< vector<string> > dataSubset, int feature)
{
   int total = dataSubset.size();
   double sum = 0;
   for(int v=0; v<featureValues[feature].size(); v++){
      double p = count(feature, v)/total;
      if(p>0)
         sum += -p * log2(p);
   }
}

double gain(int feature)
{
   int total = data.size();
   double sum = 0;
   for(int v=0; v<featureValues[feature].size(); v++){
      double p = count(feature, v)/total;
      cout << "P:" << p << endl;
      if( p>0 )
         sum += p * entropy(select(feature, v), features.size());
      if( p>0 )
         cout << "E: ", entropy(select(feature, v), features.size());
   }
}

Tree *ID3BuildTree(vector<string> availableFeatures)
{
   //int value = isOneLabel(features.size()+1);
   //else
      int bestFeature = -1;
      double bestGain = -1;
      for(int feature=0; feature<availableFeatures.size()-1; feature++){
         double g = gain(feature);
	 cout << features[feature] << ": " << g << endl;
	 if (bestGain==-1 || g>bestGain){
	    bestGain = g;
	    bestFeature = feature;
	 }
      }
      cout << "B:" << features[bestFeature] << endl;
}

int main()
{
   readProblem();
   Tree *tree = ID3BuildTree(features);
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
