#include <iostream>

#include "mat.h"
#include "rand.h"
#include "ktree.h"

using namespace std;

int main()
{
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
}
