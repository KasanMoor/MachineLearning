#ifndef MATH
#define MATH

// // // // // // // // // // // // // // // // 
//
// Some simple matrix operators
//
// These routines are self contained except for random number
// generation.   Comment these out if you want completely stand-alone
// functionality.  Uses some code from Numerical Recipes (see comments).
// This code is not designed to be blindingly fast but rather just get the job done.
//
// Author: Robert B. Heckendorn, University of Idaho, 2016

#include <string>
#include "rand.h"

class Matrix;

// // // // // // // // // // // // // // // // 
//
// class MatrixRowIter
//
class MatrixRowIter {
private:
    Matrix *mat;
    int r;
    Matrix *arow;
    bool more;

public:
    MatrixRowIter(Matrix *mat);
    ~MatrixRowIter();

public:
    Matrix *rowBegin();
    Matrix *rowNext();
    bool rowNotEnd();
    int row();
};





// // // // // // // // // // // // // // // // 
//
// class Matrix
//
class Matrix {
friend class MatrixRowIter;
private:
    bool defined;           // does it have rows and cols defined
    int maxr, maxc;
    double **m;             // the data
    std::string name;       // the name of the matrix or ""

// constructors
public:
    void allocate(int r, int c, std::string namex);
    Matrix(std::string namex="");
    Matrix(int r, int c, std::string namex="");
    Matrix(int r, int c, double *data, std::string namex="");
    Matrix(const Matrix &other, std::string namex="");         // copy constructor
    Matrix(Matrix *other);                                     // just for convenience
    ~Matrix();
    Matrix &operator=(const Matrix &other);
    void checkBounds(int r, int c, std::string msg) const;
    void assertDefined(std::string msg) const;
    void assertSquare(std::string msg) const;
    void assertSize(int r, int c, std::string msg) const;
    void assertOtherLhs(const Matrix &other, std::string msg) const;
    void assertRowsEqual(const Matrix &other, std::string msg) const;
    void assertColsEqual(const Matrix &other, std::string msg) const;
    void assertOtherSizeMatch(const Matrix &other, std::string msg) const;

// accessors
public: 
    int maxRows() const { return maxr; }
    int maxCols() const { return maxc; }
    double get(int r, int c);
    double set(int r, int c, double v);
    void setName(std::string newName);
    const std::string &getName(const std::string &defaultName="") const;
    void narrow(int newc);        // remove columns (without proper deallocation)
    void shorten(int newr);       // remove rows (without proper deallocation)

public: 
    bool equal(const Matrix &other) const;       // are the two matrices equal?
    bool nearEqual(double epsilon, const Matrix &other) const; // matrices nearly equal?
    int countGreater(const Matrix &other) const; // count number of elements >
    void argMax(int &r, int &c) const;           // what location is the largest
    void argMin(int &r, int &c) const;           // what location is the smallest
    double max() const;                          // minimum in whole array
    double min() const;                          // maximum in whole array
    double mean() const;                         // mean of whole array
    double maxCol(int c) const;                  // maximum in a column
    double minCol(int c) const;                  // minimum in a column
    double meanCol(int c) const;                 // mean in a column
    double stddevCol(int c) const;               // standard deviation in a column
    double dist2(const Matrix &other) const;           // *SQUARE* of distance between two matrices
    double dot(int r, int c, const Matrix &other) const;  // dot of row of this with col of other -> double 
    double dist2(int r, int c, const Matrix &other) const;  // *SQUARE* of distance between row of this with col of other

    // element by element operators (modifies self)
    Matrix &abs();
    Matrix &add(const Matrix &other);
    Matrix &sub(const Matrix &other);
    Matrix &mult(const Matrix &other);
    Matrix &div(const Matrix &other);

    Matrix &swap(Matrix &other);    // also modifies other
    Matrix &vecDivide(const Matrix &other, double defaultValue);
    Matrix &rowInc(int r);
    Matrix &rowAdd(int r, const Matrix &other);

    // scalar operators
    Matrix &constant(double x);            // this can be used to zero a matrix
    Matrix &constantDiagonal(double x);    // this can be used to set the diagonal to a constant
    Matrix &constantCol(int c, double x);  // this can be used to zero a column
    Matrix &identity();                    // convert to an identity matrix  (must be square)
    Matrix &scalarMult(double x);          // multiply all elements by x
    Matrix &scalarAdd(double x);           // add to all elements x
    Matrix &scalarPreSub(double x);        // NOTE: this is x - self   not   self - x, can be used to negate
    Matrix &scalarPostSub(double x);       // NOTE: this is self - x
    Matrix &addRowVector(const Matrix &other);  // self[r] + other for each row r
    Matrix &subRowVector(const Matrix &other);  // self[r] - other for each row r
    
    // min/max normalization by columns
    Matrix &normalize();                   // normalize and return array of min and max of each col
    void normalize(Matrix &minMax);        // normalize based on an array of min and max for each col

    // mapping functions
    Matrix &map(double (*f)(double x));    // apply given function to all elements
    Matrix &mapCol(int c, double (*f)(double x));  // apply given function to all elements in col c
    Matrix &mapIndex(double (*f)(int r, int c, double x)); // apply function to elements + index

    // random initialization
    Matrix &randCol(int c, double min, double max);  // random reals in a column
    Matrix &rand(double min, double max);            // random reals in range 
    Matrix &rand(int min, int max);                  // random integers in range

    // insertion and extraction
    Matrix &extract(int minr, int minc, int sizer, int sizec, Matrix &out);  // see extract below
    Matrix &insert(const Matrix &other, int minr, int minc);

    // input/output
    void print(std::string msg="");
    void printSize(std::string msg="");
    void write();
    void writeLine(int r);
    void read();

    // the following CONSTRUCT TO NEW MATRIX for the answer  (BEWARE MEMORY LEAKS!)
    Matrix &extract(int minr, int minc, int sizer, int sizec);
    Matrix &transpose();                    // classic transpose into new matrix (see transposeSelf below)
    Matrix &dot(const Matrix &other);       // classic matrix multiply, inner product
    Matrix &dotT(const Matrix &other);      // classic matrix multiply self * Transpose(other)
    Matrix &Tdot(const Matrix &other);      // classic matrix multiply Transpose(self) * other
    Matrix &meanVec();                      // creates a row vector of means of columns
    Matrix &cov();                          // covariance matrix (BIASED covariance)

    // alternation versions of operaters that do not create new matrices
    Matrix &transposeSelf();                // transpose in place of SQUARE MATRIX

    // special operators
    void swapRows(int i, int j);            // utility to swap two rows
    int *LU();                              // LU decomposition in place
    Matrix &solve(Matrix &B);               // solve Ax = B returns solutions and inverse
    Matrix &inverse();                      // replace with inverse

    // eigen systems
    void tridiagonalize(double *&d, double *&e);
    Matrix &eigenSystem();
    };

#endif
