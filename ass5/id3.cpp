#include <iostream>
#include <string>
#include <math.h>

#include "mat.h"
#include "rand.h"
#include "ktree.h"

using namespace std;

vector<string> features;
vector< vector<string> > featureValues;
vector< vector<string> > Data;

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
   //printVector(features);
   //print2dVector(featureValues);
   cin >> numExamples;
   for(int i=0; i<numExamples; i++){
      Data.push_back(vector<string>());
      for(int j=0; j<numFeatures+1; j++){
         cin >> input;
         Data[i].push_back(input);
      }
   }
   //print2dVector(Data);
}

int count(vector< vector<string> > data, int feature, int value)
{
   //cout << "counting: " << features[feature] << ", " << featureValues[feature][value] << endl;
   int num = 0;
   for(int d=0; d<data.size(); d++){
      if(data[d][feature] == featureValues[feature][value])
         num++;
   }
   //cout << "counted: " << num << endl;
   return num;
}

int isOneLabel(vector< vector<string> > data, int feature)
{
   int num = data.size();
   //cout << features[feature] << endl;
   //print2dVector(data);
   //cout << num << endl;
   for(int v=0; v<featureValues[feature].size(); v++){
      //cout << featureValues[feature][v] << endl;
      //cout << count(data, feature, v) << endl;
      if(count(data, feature, v) == num)
         return v;
   }
   //cout << "returning -1" << endl;
   return -1;
}

vector< vector<string> > select(vector< vector<string> > data, int feature, int value)
{
   vector< vector<string> > selection;
   for(int i=0; i<data.size(); i++){
      if(data[i][feature] == featureValues[feature][value])
         selection.push_back(data[i]);
   }
   //cout << "looking for: " << featureValues[feature][value];
   //cout << " selected: " << endl;
   //print2dVector(selection);
   return selection;
}

double entropy(vector< vector<string> > data, int feature)
{
   double total = data.size();
   double sum = 0;
   //cout << "feature: " << feature << endl;
   //cout << featureValues[feature].size() << endl;
   for(int v=0; v<featureValues[feature].size(); v++){
      double p = count(data, feature, v)/total;
      //cout << "small p: " << p << endl;
      if(p>0)
         sum += -p * log2(p);
	 //cout << "sum: " << sum << endl;
   }
   //cout << "calculated entropy: " << sum << endl;
   return sum;
}

double gain(vector< vector<string> > data, int feature)
{
   double total = data.size();
   double sum = 0;

   for(int v=0; v<featureValues[feature].size(); v++){
      double p = count(data, feature, v)/total;
      //cout << "P:" << p << endl;
      if( p>0 )
         sum += p * entropy(select(data, feature, v), features.size()-1);
      //if( p>0 )
         //cout << "E: " << entropy(select(data, feature, v), features.size()-1) << endl;
   }
   //cout << "sum: " << sum << endl;
   return entropy(data, features.size()-1) - sum;
}

int outOfFeatures(vector<string> availableFeatures){
   //cout << "checking if we are out of features" << endl;
   for(int i=0; i<availableFeatures.size(); i++){
      if(availableFeatures[i] != "")
         return 0;
   }
   return 1;
}

int maxAns(vector< vector<string> > data){
   int maxCnt = 0;
   int maxValue = -1;
   for(int v=0; v<featureValues[features.size()-1].size(); v++){
      int cnt = count(data, features.size()-1, v);
      if(maxValue == -1 || cnt > maxCnt){
         maxCnt = cnt;
	 maxValue = v;
      }
   }
   return maxValue;
}

Tree *ID3BuildTree(vector< vector<string> > data, vector<string> availableFeatures)
{
   int value = isOneLabel(data, features.size()-1);
   if(value != -1){
      //cout << "All labels are the same and value: " << featureValues[features.size()-1][value] << endl;
      Tree* temp = new Tree(featureValues[features.size()-1][value]);
      //temp->printEdge();
      return temp;
   }
   else if(outOfFeatures(availableFeatures)){
      //cout << "out of features" << endl;
      Tree *temp = new Tree(featureValues[features.size()-1][maxAns(data)]);
      //temp->printEdge();
      return temp;
   }
   else {
      //cout << "building new subtree for ";
      int bestFeature = -1;
      double bestGain = -1;
      for(int feature=0; feature<availableFeatures.size(); feature++){
         if( availableFeatures[feature] != "" ) {
            double g = gain(data, feature);
	    //cout << features[feature] << ": " << g << endl;
	    if (bestGain==-1 || g>bestGain){
	       bestGain = g;
	       bestFeature = feature;
	    }
	 }
      }
      //cout << "B:" << availableFeatures[bestFeature] << endl;
      Tree *treeLeaves = new Tree(availableFeatures[bestFeature]);
      availableFeatures[bestFeature] = "";
      for(int v=0; v<featureValues[bestFeature].size(); v++){
         treeLeaves->addChild(featureValues[bestFeature][v], ID3BuildTree(select(data, bestFeature, v), availableFeatures));
      }
      return treeLeaves;
   }
}

int main()
{
   readProblem();
   vector<string> availableFeatures = features;
   availableFeatures.erase(availableFeatures.end());
   //availableFeatures[0] = "";
   Tree *tree = ID3BuildTree(Data, availableFeatures);
   tree->printEdge();
   //Tree *DTree = ID3BuildTree();
   //DTree->printAux();
/*
   Tree* decisionTree = new Tree("decision Tree");
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
