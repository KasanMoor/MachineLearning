#include <iostream>
#include <cmath>
#include <math.h>
#include "mat.h"

Matrix calculateMeans(Matrix inputs);
Matrix standardDeviation(Matrix inputs);
Matrix centerData(Matrix inputs,
                  Matrix stdDev,
		  Matrix means );
Matrix uncenterData(Matrix compressedData,
                  Matrix stdDev,
		  Matrix means );
Matrix covariate( Matrix centeredData );
Matrix covariate( Matrix centeredData );
Matrix componentAnalysis( Matrix eigenVectors,
                          Matrix eigenValues );

using namespace std;

int main() 
{
   int rows;
   int columns;
   int k;

   cin >> k >> rows >> columns;
//   cout << numInputs << endl << rows << " " << columns << endl;
   double data[rows*columns];
   for (int i=0; i<rows*columns; i++) {
      cin >> data[i];
   }

   Matrix inputs(rows, columns, data, "input");
   inputs.setName("raw input");
   //inputs.print();
   
   Matrix means = calculateMeans(inputs);
   means.setName("Column Means");
   //means.print();
   Matrix stdDev = standardDeviation(inputs);
   stdDev.setName("Standard Deviation");
   //stdDev.print();
   Matrix centeredData = centerData(inputs, stdDev, means);
   centeredData.setName("Centered Data");
   //centeredData.print();
   //Matrix covariance = covariate( centeredData );
   //covariance.setName("Covariance matrix");
   //covariance.print();

   Matrix covariance = centeredData.cov();
   covariance.setName("Library Covariance");
   //covariance.print();
   Matrix eigenVectors = covariance;
   eigenVectors.setName("EigenVectors in rows");
   Matrix eigenValues = eigenVectors.eigenSystem();
   eigenValues.setName("EigenValues");
   eigenVectors.print();
   eigenValues.print();

   Matrix ogEigenVectors = eigenVectors;
   eigenVectors.shorten(k);
   eigenVectors.transposeSelf();
   Matrix compressedData = centeredData.dot(eigenVectors);
   compressedData.setName("Compressed Data");
   compressedData.print();
   ////////////////////compression done
   eigenVectors.transposeSelf();
   Matrix decompressedData = compressedData.dot(eigenVectors);
   decompressedData.setName("Decompressed Data");
   decompressedData = uncenterData( decompressedData,
                                           stdDev,
					   means );
   decompressedData.print();
   /////////////////////decompression done
   Matrix componentMatrix = componentAnalysis( ogEigenVectors, 
   					       eigenValues );
   componentMatrix.print();
}

Matrix componentAnalysis( Matrix eigenVectors,
                          Matrix eigenValues )
{
   Matrix componentMatrix = eigenVectors;
   eigenVectors.transposeSelf();
   //eigenVectors.print();
   //eigenValues.print();
   for( int i=0; i<eigenVectors.maxRows(); i++ ) {
      for( int j=0; j<eigenVectors.maxCols(); j++ ) {
         double set = eigenVectors.get(i, j) 
	             * sqrt(eigenValues.get(0, j));
         componentMatrix.set(i, j, set );
      }
   }
   componentMatrix.transposeSelf();
   componentMatrix.setName("Component Matrix");
   return componentMatrix;
}

Matrix uncenterData(Matrix compressedData,
                    Matrix stdDev,
		    Matrix means )
{
   for( int i=0; i<compressedData.maxRows(); i++ ) {
      for( int j=0; j<compressedData.maxCols(); j++ ) {
         double centered = ( compressedData.get(i, j)*stdDev.get(0, j) )
	                    + means.get(0, j);
         compressedData.set(i, j, centered);
      }
   }
   return compressedData;
}

Matrix covariate( Matrix centeredData )
{
   Matrix covariance = centeredData;
   centeredData = centeredData.transpose();

   double r = covariance.maxRows();
   for( int i=0; i<centeredData.maxRows(); i++ ) {
      for( int j=0; j<centeredData.maxCols(); j++ ) {
         //cout << 1/r << endl;
         double set = 1/r*centeredData.get(i, j);
	 //cout << set << endl;
         centeredData.set(i, j, set);
      }
   }
   return centeredData.dot(covariance);
}

Matrix centerData(Matrix inputs,
                  Matrix stdDev,
		  Matrix means )
{
   Matrix centeredData(inputs.maxRows(), 
                       inputs.maxCols(), 
		       "Centered Data");
   centeredData.constant(0);
   for( int i=0; i<inputs.maxRows(); i++ ) {
      for( int j=0; j<inputs.maxCols(); j++ ) {
         double centered = ( inputs.get(i, j)-means.get(0, j) ) 
	                   / stdDev.get(0, j);
         centeredData.set(i, j, centered);
      }
   }
   return centeredData;
}

Matrix standardDeviation(Matrix inputs)
{
   Matrix stdDev(1, inputs.maxCols(), "Standard Deviation");
   stdDev.constant(0);
   for( int i=0; i<inputs.maxCols(); i++ ) {
      stdDev.set(0, i, inputs.stddevCol(i));
   }
   return stdDev;
}

Matrix calculateMeans(Matrix inputs)
{
   Matrix mean(1, inputs.maxCols(), "Column means");
   mean.constant(0);
   for( int i=0; i<inputs.maxCols(); i++ ) {
      mean.set(0, i, inputs.meanCol(i));
   }
   return mean;
}
