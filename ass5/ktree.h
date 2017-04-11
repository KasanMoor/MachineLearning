#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

// // // // // // // // // // // // // // // // // // // // 
// 
// Class Tree: super simple tree class with labeled nodes and edges.
// 
class Tree {
private:
    std::string name;
    std::vector<Tree *> children;
    std::vector<std::string> edgeNames;

// constructors
public:
    Tree();                                                            // empty tree
    Tree(std::string n);                                                     // named root node
    Tree(std::string n, std::string ln, Tree *c);                                  // named root node and edge name + child
    Tree(std::string n, std::string leftn, Tree *leftc, std::string rightn, Tree *rightc);  // named root and two children
    ~Tree();

private:
    void printAux(bool edge, int level);                  

public:
    bool isLeaf() { return children.size()==0; }                          // is this a leaf (no children)
    int numChildren() { return children.size(); }                         // how many children
    Tree *addChild(std::string ln, Tree *c);                                    // add a child with name and tree
    Tree *addChild(std::string leftn, Tree *leftc, std::string  rightn, Tree *rightc); // add children with names and trees
    void printEdge() { printAux(true, 0); }                               // print with edge names
    void print() { printAux(false, 0); }                                  // print tree without edge names
};
