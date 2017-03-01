#include <iostream>
#include <cmath>
#include "mat.h"

using namespace std;

double slam(double x) {
   if( x<0 )
      return 0;
   else
      return 1;
}

double sigmoid(double x)
{
   return 1/(1+exp(x));
}

double randW(double x)
{
   return randPMUnit()*0.1;
}

bool train(Matrix inputs, Matrix* v, Matrix *w);

int main() 
{
   int number;
   int numInputs;
   int rows;
   int columns;
   int hiddenNodes;
   initRand();

   cin >> numInputs >> hiddenNodes >> rows >> columns;
//   cout << numInputs << endl << rows << " " << columns << endl;
   double data[rows*columns];
   for (int i=0; i<rows*columns; i++) {
      cin >> data[i];
   }

   Matrix inputs(rows, columns, data, "input");
//   inputs.print();   
   
   Matrix v(numInputs+1, hiddenNodes, "v");
   v = v.constant(0);
   v = v.map(&randW);

   Matrix w(hiddenNodes+1, columns-numInputs, "w");
   w = w.constant(0);
   w = w.map(&randW);

   v.print();
   w.print();

   int count = 0; 
   int cap = 0;
   while( train(inputs, &v, &w ) && count < cap) {
      count++; 
   }

   cin >> rows >> columns;
   double resultsRead[rows*columns];
   for (int i=0; i<rows*columns; i++) {
      cin >> resultsRead[i];
   }
   Matrix results(rows, columns, resultsRead, "testData");
   //results.print();

   Matrix x( rows, columns+1, "x" );
   x = x.constant(0);
   x = x.insert( results, 0, 0 );
   x = x.constantCol(columns, -1);
//   x.print();

   Matrix prediction = x.dot(w);
   prediction = prediction.map(slam);

   cout << "BEGIN TESTING" << endl;
   for (int i=0; i<rows; i++) {
      //results.writeLine(i);
      //prediction.writeLine(i);
      //cout << endl;
   }

   //if( count == cap )
      //cout << "cap reached" << endl;
   
}

bool train(Matrix inputs, Matrix *v, Matrix *w) 
{
   //fill y
   Matrix t( inputs , "t: desired output");
   //w->print();
   t=t.extract(0, w->maxRows()-1, t.maxRows(), w->maxCols());
   //t.print();


   Matrix x( inputs, "x" );
   x = x.extract( 0, 0, x.maxRows(), x.maxCols()-t.maxCols()+1 );
   //set the bias for each row
   for (int r=0; r<x.maxRows(); r++) {
      x.set(r, x.maxCols()-1, -1);
   }
   //x.print();

   //w->print();

   Matrix preh = x.dot(v);
   preh.setName("preh");
   //h.print();
   
   Matrix h( preh.maxRows(), preh.maxCols()+1, "h: prehh with bias" );
   h = h.constant(-1);
   h = h.insert( preh, 0, 0 );
   //h.print();

   //setup complete multiply
   ////////////////////////////////////////////////////////////////
   //forwards
   ///////////////////////////////////////////////////////////////
   Matrix y = h.dot(w);
   y.setName("y");
   //y.print();

   y = y.map(&sigmoid);
   //cout << "slam y: " << endl; y.print();
   ///////////////////////////////////////////////////////////////
   //backwards
   //////////////////////////////////////////////////////////////

   Matrix temp = y;
   Matrix dy = y.sub(t);
   dy.writeLine(0); cout << "dy y.sub(t)" << endl;
   y = temp;
   dy.setName("dy: y.sub(t)");
   //dy.print();
   dy.mult(y);
   dy.writeLine(0); cout << "dy.mult(y)" << endl;
   dy.mult(y.scalarPreSub(1));
   dy.writeLine(0); cout << "dy.mult(y.scalarPreSub(1)" << endl;
   y = temp;
   //dy.print();
   dy.writeLine(0); cout << "dy" << endl;
   
   temp = h;
   h.writeLine(0); cout << "h" << endl;
   Matrix inner1 = h.scalarPreSub(1);
   inner1.writeLine(0); cout << "inner1" << endl;
   h = temp;
   Matrix inner2 = dy.dot(w->transpose());
   inner2.writeLine(0); cout << "inner2" << endl;
   Matrix dh = h.mult(inner1.mult(inner2));
   dh.setName("dh");
   dh.writeLine(0); cout << "dh" << endl;
   return 0;

   ///////////////////////////////////////////////////////////////
   //Adjust
   //////////////////////////////////////////////////////////////
   double eta = 0.1;
   w->sub(
   

   Matrix ty = t.sub(y);
   ty.setName("ty");
   //cout << "t-y: " << endl; ty.print();
   
   Matrix zero(ty.maxRows(), ty.maxCols(), "zero");
   zero = zero.constant(0);
   if( ty.equal(zero) ) 
      return 0;

   Matrix xt = x.transpose();
   xt.setName("xt");
//   xt.print();

   Matrix d = xt.dot(ty);
   d.setName("d: xt dot t-y");
   //d.print();
   
   Matrix wt = d.scalarMult(0.1);
   wt.setName("wt: d times eta of 01");
   //wt.print();

   w->add(wt);
   //cout << "new value for w: " << endl; w->print();
   
   return 1;
}
