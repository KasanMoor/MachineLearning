#!/usr/bin/env python3
# Author: Dr. Robert Heckendorn, Computer Science Department, University of Idaho, 2016
#
# ID3 decision tree algorithm
#
import sys
from math import *

## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## 
#
# IO support for reading from stdin and writing to stdout
#

# read in a classifier problem
def readProblem() :
    global Features, FeatureList, FeatureValues, Data

    Features = set()     # set of all the features including Ans
    FeatureList = []     # list of all the features including Ans
    FeatureValues = {}   # potential values of all the features, even ones not in examples
    Data = []            # example classification data

    # read number of features
    numFeatures = int(sys.stdin.readline())

    # read in features and answer which must be called: Ans
    for i in range(0, numFeatures+1) :
        line = sys.stdin.readline().strip()
        fields = line.split()
        Features.add(fields[0])
        FeatureList.append(fields[0])
        FeatureValues[fields[0]] = fields[2:] # ignore feature name and num features

    # read number of samples
    numSamples = int(sys.stdin.readline())
    
    # read in example classifications
    for line in sys.stdin.readlines() :
        fields = line.split()
        sample = {}
        for i in range(0, len(FeatureList)) :
            sample[FeatureList[i]] = fields[i]
        Data.append(sample)



# write out indented classifier tree
amountIndent = 4*" "

def printDTree(tree):
    printDTreeAux("", tree)
    
def printDTreeAux(indent, tree) :
    name = tree[0]
    d = tree[1]
    if type(d) is dict :
        for v in FeatureValues[name] :
            print(indent, name+"="+v+":")
            printDTreeAux(indent+amountIndent, d[v])
    else :
        print(indent, d)


## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## 
#
# Decision Tree:  list either [Feature, Value] or [Feature, dictionary of value:tree]
# Author: Robert B. Heckendorn, University of Idaho
# 
# The OUTPUT:
#
# ["Ans", "Yes"]
# or 
# ["Price", {"$" : tree1, "$$" : tree2, "$$$" : tree1}]
# 
# 
# DATA: a list of dictionary for each training case
# 
# [{"Ans" : "Yes", "Alt" : "Yes", "Price" : "$$" ... },
#  ... ]
# 
# FEATUREVALUES: dictionary of Feature:[list of values]
# 
# {"Ans" : ["Yes", "No"], "Alt" : ["Yes", "No"], "Price" : ["$", "$$", "$$$"] ... }
# 
# FEATURES: a set of feature labels
# set(["Ans", "Alt", "Price", ...])
# 

# 
# select(data, feature, value) -> subset of data as list of dictionaries that have feature=value
# count(data, feature, value) -> number of cases in data in which feature has value 
# entropy(data, feature) -> [0, 1], feature is generally "Ans" for ID3


# list of the items in data that have feature equal to value
def select(data, feature, value) :
    return [ item for item in data if item[feature]==value ]


# count how many items in the data have feature equal to value
def count(data, feature, value) :
    num = 0
    for d in data :
        if d[feature]==value : num+=1
    return num


# what is the entropy of a question about feature?
# sum the entropy over the possible values of the feature.
def entropy(data, feature) :
    total = len(data)
    sum = 0
    for v in FeatureValues[feature] :
        p = count(data, feature, v)/total   # real divide
        if p>0 : sum += -p * log2(p)        # if p==0 then don't add anything by definition
    return sum


# current entropy - expected entropy after getting info about feature 
# entropy(data, "Ans") - sum_{v=featurevalues} p_v * entropy(select(data, feature, v), "Ans")
def gain(data, feature) :
    total = len(data)
    sum = 0
    for v in FeatureValues[feature] :       # weighted sum of entropies
        p = count(data, feature, v)/total   # proportion of entropy to count
        print("P:", p)
        if p>0 : sum += p * entropy(select(data, feature, v), "Ans")
        if p>0 : print("E: ", entropy(select(data, feature, v), "Ans"))

    return entropy(data, "Ans") - sum


# If there only one value for the feature in the items in data return that value
# If not return None
def isOneLabel(data, feature) :
    num = len(data)
    for v in FeatureValues[feature] :
        if count(data, feature, v) == num :
            return v
    return None


# select the most popular Ans value left in the data for the constraints
# up to now.
def maxAns(data) :
    maxCnt = 0
    maxValue = None
    for v in FeatureValues["Ans"] :
        cnt = count(data, "Ans", v)
        if maxValue == None or cnt > maxCnt :
            maxCnt = cnt
            maxValue = v
    return maxValue


# this is the ID3 algorithm
def ID3BuildTree(data, availableFeatures) :
    # only one label for the Ans feature at this point?
    value = isOneLabel(data, "Ans")       
    if value != None :
        return ["Ans", value]

    # ran out of discriminating features
    elif len(availableFeatures) == 0 :
        print("***  out of features ***")
        return ["Ans", maxAns(data)]

    # pick maximum information gain
    else :
        bestFeature = None
        bestGain = None
        for feature in availableFeatures :
            g = gain(data, feature)
            print(feature, ": ", g);
            if bestGain == None or g>bestGain :
                bestGain = g
                bestFeature = feature
        print("B:", bestFeature);
        print();
            
        # recursively construct tree on return
        treeLeaves = {}   # start with empty dictionary
        for v in FeatureValues[bestFeature] :
            treeLeaves[v] = ID3BuildTree(select(data, bestFeature, v), availableFeatures - set([bestFeature]))
        return [bestFeature, treeLeaves]    # list of best feature and dictionary of trees


# do the work
def main() :
    readProblem()
    tree = ID3BuildTree(Data, Features - set(["Ans"]))
    printDTree(tree)

main()
