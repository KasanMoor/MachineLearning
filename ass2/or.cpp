#include <iostream>

#include "mat.h"

using namespace std;

double slam(double x) {
   if( x<0 )
      return 0;
   else
      return 1;
}

double randW(double x)
{
   return randPMUnit();
}

bool train(Matrix inputs, Matrix *w);

int main() 
{
   int number;
   int numInputs;
   int rows;
   int columns;
   initRand();

   cin >> numInputs >> rows >> columns;
//   cout << numInputs << endl << rows << " " << columns << endl;
   double data[rows*columns];
   for (int i=0; i<rows*columns; i++) {
      cin >> data[i];
   }

   Matrix inputs(rows, columns, data, "input");
   inputs.print();   
   
   Matrix w(3, 1, "w");
   w = w.constant(0);
   w = w.map(&randW);
   w.print();

   while( train(inputs, &w ));

   cin >> rows >> columns;
   double resultsRead[rows*columns];
   for (int i=0; i<rows*columns; i++) {
      cin >> resultsRead[i];
   }
   Matrix results(rows, columns, resultsRead, "testData");
   results.print();

   Matrix x( rows, columns+1, "test x" );
   x = x.constant(0);
   x = x.insert( results, 0, 0 );
   x = x.constantCol(columns, -1);
   x.print();

   Matrix prediction = x.dot(w);
   prediction = prediction.map(slam);

   cout << endl << "BEGIN TESTING" << endl;
   for (int i=0; i<rows; i++) {
      results.writeLine(i);
      prediction.writeLine(i);
      cout << endl;
   }
   
}

bool train(Matrix inputs, Matrix *w) 
{
   //fill y
   Matrix t( inputs , "t");

   t=t.extract(0, 2, 4, 1);
   t.print();


   Matrix x( inputs, "x" );
   x.extract(0, 0, 4, 3);
   //set the bias for each row
   for (int r=0; r<x.maxRows(); r++) {
      x.set(r, x.maxCols()-1, -1);
   }
   x.print();

   w->print();

   //setup complete multiply
   Matrix y = x.dot(w);
   y.setName("y");
   y.print();

   y = y.map(&slam);
   y.print();

   Matrix ty = t.sub(y);
   ty.setName("ty");
   ty.print();
   
   Matrix zero(ty.maxRows(), ty.maxCols(), "zero");
   zero = zero.constant(0);
   if( ty.equal(zero) ) 
      return 0;

   Matrix xt = x.transpose();
   xt.setName("xt");
   xt.print();

   Matrix d = xt.dot(ty);
   d.setName("d");
   d.print();
   
   Matrix wt = d.scalarMult(0.1);
   wt.setName("wt");
   wt.print();

   w->add(wt);
   w->print();
   
   return 1;
}


