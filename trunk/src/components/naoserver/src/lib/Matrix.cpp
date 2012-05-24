/**********************************************************************
 * 
 * Kwun Han <kwunh@cs.cmu.edu>
 * March 1997
 *
 * Michael Bowling <mhb@cs.cmu.edu>
 * 1998-2002
 *
 * Francisco Martn Rico <fmartin@gsyc.escet.urjc.es>
 * 2005
 * 
 * Determinant and inverse code is copied from mtrxmath under the GPL.
 *
 **********************************************************************/
/* LICENSE:
  =========================================================================
    CMPack'04 Source Code Release for OPEN-R SDK 1.1.5-r2 for ERS7
    Copyright (C) 2004 Multirobot Lab [Project Head: Manuela Veloso]
    School of Computer Science, Carnegie Mellon University
    All rights reserved.
  ========================================================================= */
#define  PLATFORM_LINUX
#include <cstring>
#include <assert.h>
#ifdef PLATFORM_LINUX
#include <cstdio>
#endif
#include <cstdlib>
#include <cmath>
#include "Matrix.h"
#include <stdio.h>

MatrixCM::MatrixCM(char* const init_string)
{
  str_init(init_string);
}

MatrixCM::MatrixCM(int rows, int columns)
{
  r_ = rows;
  c_ = columns;

  mat = new double[r_*c_];//(double*)malloc(r_*c_*sizeof(double));
}

MatrixCM::MatrixCM(int rows, int columns, float *m)
{
  r_ = rows;
  c_ = columns;

  mat =  new double[r_*c_];//(double*)malloc(r_*c_*sizeof(double));

  for(int i=0; i<rows*columns; i++)
    mat[i] = m[i];
}

MatrixCM::MatrixCM(int identity_size)
{
  r_ = identity_size;
  c_ = identity_size;

  mat =  new double[r_*c_];//(double*)malloc(r_*c_*sizeof(double));
  for(int i = 0; i < identity_size; i++) {
    for(int j = 0; j < identity_size; j++) {
      this->e(i,j) = i==j;
    }
  }
}

MatrixCM::MatrixCM()
{
  r_ = 0;
  c_ = 0;
  mat = 0;    
}

MatrixCM::MatrixCM(const MatrixCM& other)
{
  r_ = other.r_;
  c_ = other.c_;

  mat =  new double[r_*c_];//(double*)malloc(r_*c_*sizeof(double));

  for(int i = 0; i < r_*c_; i++) {
    mat[i] = other.mat[i];
  }
}

MatrixCM::~MatrixCM()
{
  if(mat) delete [] mat;
}

void MatrixCM::CopyData(float *data)
{
  for (double *ptr = mat; ptr < mat + r_ * c_; ptr++)
    *data++ = (float) (*ptr++);
}

void MatrixCM::CopyData(double *data)
{
  memcpy(data, mat, r_ * c_ * sizeof(double));
}

const MatrixCM& MatrixCM::operator= (const MatrixCM& other)
{
  if(this == &other) return *this;

  int i;

  if(mat) delete [] mat;
  r_ = other.r_;
  c_ = other.c_;

  mat =  new double[r_*c_];//(double*)malloc(r_*c_*sizeof(double));

  for(i = 0; i < (r_*c_); i++) {
    mat[i] = other.mat[i];
  }

  return *this;
}

const MatrixCM& MatrixCM::operator= (char* const init_string)
{
  if(mat) delete [] mat;
  str_init(init_string);

  return *this;
}

MatrixCM &MatrixCM::set(double const *data)
{
  for (int i = 0; i < r_; i++) {
    for (int j = 0; j < c_; j++) {
      e(i,j) = data[i * c_ + j];
    }
  }
  return (*this);
}

MatrixCM &MatrixCM::set(float const *data)
{
  for (int i = 0; i < r_; i++) {
    for (int j = 0; j < c_; j++) {
      e(i,j) = data[i * c_ + j];
    }
  }
  return (*this);
}

void MatrixCM::str_init(char* const init_string)
{
  char* str1 =  new char[strlen(init_string)+1];//(char*)malloc(strlen(init_string)+1);
  int tcount;
  std::string delim = " ;[]";


  strcpy(str1, init_string);

  tcount = 0;
  
  if(strtok(str1, delim.c_str())) tcount++;
  while(strtok(NULL, delim.c_str())) tcount ++;

  if(!tcount) {
    mat = 0;
    return;
  }

  mat =  new double[r_*c_];//(double*)malloc(tcount * sizeof(double));

  strcpy(str1, init_string);

  r_ = 0;

  if(strtok(str1, ";")) r_++;
  while(strtok(NULL, ";")) r_ ++;

  c_ = tcount / r_;

  strcpy(str1, init_string);
  
  mat[0] = atof(strtok(str1, delim.c_str()));
  for(int i = 1; i < tcount; i++) {
    mat[i] = atof(strtok(NULL, delim.c_str()));
  }
  delete [] str1;
}

const MatrixCM operator+ (const MatrixCM& a, const MatrixCM& b)
{
  MatrixCM out;
  m_add(out, a, b);
  return out;
}

const MatrixCM operator- (const MatrixCM& a, const MatrixCM& b)
{
  MatrixCM out;
  m_subtract(out, a, b);
  return out;
}

const MatrixCM operator* (const MatrixCM& a, const MatrixCM& b)
{
  MatrixCM out;
  m_multiply(out, a, b);

  return out;
}

const MatrixCM inverse(const MatrixCM& a)
{
  MatrixCM out;
  out = a;
  out.inverse();
  return out;
}

const MatrixCM transpose(const MatrixCM& a)
{
  MatrixCM out;
  out = a;
  out.transpose();
  return out;
}

const MatrixCM& m_multiply(MatrixCM& out, const MatrixCM& a, const MatrixCM& b)
{
  int i, j, k;

  assert(a.c_ == b.r_);

  out.resize(a.r_, b.c_);

  for(i = 0; i < out.r_*out.c_; i++)
    out.mat[i] = 0.0;

  for(i = 0; i < a.r_; i++) {
    for(j = 0; j < b.c_; j++) {
      for(k = 0; k < a.c_; k++) {
	out.e(i,j) += a.e(i,k) * b.e(k,j);
      }
    }
  }

  return out;
}

const MatrixCM& m_inverse(MatrixCM& out, const MatrixCM& in)
{
  out = in;
  out.inverse();
  return out;
}

const MatrixCM& m_add(MatrixCM& out, const MatrixCM& a, const MatrixCM& b)
{
  assert(a.r_ == b.r_ && a.c_ == b.c_);

  out.resize(a.r_, a.c_);

  for(int i = 0; i < a.r_ * a.c_ ; i++) {
    out.mat[i] = a.mat[i] + b.mat[i];
  }

  return out;
}

const MatrixCM& m_subtract(MatrixCM& out, const MatrixCM& a, const MatrixCM& b)
{
  assert(a.r_ == b.r_ && a.c_ == b.c_);

  out.resize(a.r_, a.c_);

  for(int i = 0; i < a.r_ * a.c_ ; i++) {
    out.mat[i] = a.mat[i] - b.mat[i];
  }

  return out;
}

const MatrixCM& m_transpose(MatrixCM& out, const MatrixCM& in)
{
  out.resize(in.c_, in.r_);

  for(int i = 0; i < in.r_; i++) {
    for(int j = 0; j < in.c_; j++) {
      out.e(j,i) = in.e(i,j);
    }
  }

  return out;
}

const MatrixCM& MatrixCM::transpose()
{
  if(!mat) return *this;

  double* newmat =  new double[r_*c_];//(double*)malloc(r_*c_*sizeof(double));
  
  for(int i = 0; i < r_; i++) {
    for(int j = 0; j < c_; j++) {
      newmat[j*r_+i] = e(i,j);
    }
  }

  int t;
  t = c_;
  c_ = r_;
  r_ = t;    

  delete [] mat;
  mat = newmat;

  return *this;
}

const MatrixCM& MatrixCM::identity(int size)
{
  if(mat) delete [] mat;
  r_ = size;
  c_ = size;

  mat =  new double[r_*c_];//(double*)malloc(r_*c_*sizeof(double));
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < size; j++) {
      this->e(i,j) = i==j;
    }
  }
  return *this;
}

const MatrixCM& MatrixCM::resize(int rows_cols)
{
  return resize(rows_cols,rows_cols);
}

const MatrixCM& MatrixCM::resize(int rows, int columns)
{
  if (rows == r_ && columns == c_)
    return *this;

  if(mat) delete [] mat;

  r_ = rows;
  c_ = columns;

  mat =  new double[r_*c_];//(double*)malloc(r_*c_*sizeof(double));

  return *this;
}

const MatrixCM& MatrixCM::scale(double factor)
{
  if(!mat) return *this;

  for(int i = 0; i < r_*c_; i++) {
    mat[i] *= factor;
  }
  return *this;
}

void MatrixCM::print() const
{
#ifdef PLATFORM_LINUX
  int i,j;
  fprintf(stderr,"\n%dx%d\n", r_, c_);

  for(i = 0; i < r_; i++) {
    for(j = 0; j < c_; j++) {
      fprintf(stderr, "%f ", e(i,j));
    }
    fprintf(stderr, "\n");
  }
#endif
}

MatrixCM *MatrixCM::reduce_Matrix (int cut_row, int cut_column) const
{
  MatrixCM *reduced;
  int y, x, rc, rr;
  rc=rr=0;
  
  reduced = new MatrixCM(r_-1, r_-1);

  /* This function performs a fairly simple function. It
     reduces a matrix in size by one element on each dimesion
     around the coordinates sent in the cut_row and cut_column
     values.  For example:
        3x3 Matrix:  1 2 3
	             4 5 6
		     7 8 9
	is sent to this function with the cut_row == 2 and 
	cut_column == 1, the function returns the 
	2x2 Matrix: 2 3
	            8 9
  */

  for ( x=0 ; x < r_ ; x++) {
    for ( y=0 ; y < c_; y++) {
      if( x == cut_row || y == cut_column ) { }
      else {
	reduced->e(rr,rc) = e(x,y);
	rc++;
      }
    }
    if (rr != cut_row || x > cut_row ) { rr++; rc=0; }
  }

  //reduced->print();

  return reduced;
}

/* 
 * Determinant
 * Solve the determinant of a matrix
 *
 * Yes, I know this uses the Cramer's Method of finding a
 * determinate, and that Gaussian would be better.  I'm
 * looking into implementing a Gaussian function, but for
 * now this works. 
 */

double MatrixCM::determinant() const
{
  double det=0;
  MatrixCM *tmp;
  int i, sign=1;
  
  /* Do I need to explain this? */
  assert( r_ == c_ );
  
  /* This may never be used, but it's necessary for error
     checking */
  if (r_ == 1)
    return e(0,0);
  
  /* This may not be necessary, but it keeps the recursive function
     down to one less call, so it speeds things up just a bit */
  if (r_ == 2)
    return (e(0,0)*e(1,1)-e(0,1)*e(1,0));

  /* This is the recursive algorithm that does most of the computation. */
  else {
    for ( i=0 ; i < r_ ; i++, sign*=-1) {
      tmp = reduce_Matrix(0,i);
      det += sign * e(0,i) * tmp->determinant();
      delete tmp;
    }
    return det;
  }
}

#if 0
// PER 6/13/04
// This is from vision... this neds to be used instead of the above
// method... 
double CalcDeterminant(bool &ok,int n_rc,double *mat)
{
  static const int MaxRC=5; // maximum rows/cols

  static double mat_copy[MaxRC][MaxRC];

  ok = false;

  if(n_rc > MaxRC)
    return 0.0;

  // copy matrix
  for(int r=0; r<n_rc; r++){
    for(int c=0; c<n_rc; c++){
      mat_copy[r][c] = mat[r*n_rc + c];
    }
  }

  double det=1.0;

  // row_to_fix is first remaining row which is not upper
  //   triangular with a 1.0 diagonal value
  for(int row_to_fix=0; row_to_fix<n_rc; row_to_fix++){
    // if diagonal element is zero, find a row below this one that's non-zero
    //   to swap with
    if(mat_copy[row_to_fix][row_to_fix]==0.0){
      int row_to_swap;
      for(row_to_swap=row_to_fix+1; row_to_swap<n_rc; row_to_swap++){
        if(mat_copy[row_to_swap][row_to_fix]!=0.0){
          break;
        }
      }
      // uh-oh, no non-zero row to swap, implies determinate is 0
      if(row_to_swap == n_rc){
        ok = true;
        return 0.0;
      }
      // swap rows
      for(int c=0; c<n_rc; c++){
        swap(mat_copy[row_to_fix][c],mat_copy[row_to_swap][c]);
      }
      det = -det;
    }
    // diagonal element is now non-zero
    // divide by diagonal element to make it 1.0
    // can start at row_to_fix since first elements are already 0.0
    double scale = mat_copy[row_to_fix][row_to_fix];
    det *= scale;
    for(int c=row_to_fix; c<n_rc; c++){
      mat_copy[row_to_fix][c] /= scale;
    }
    // now eliminate this variable from remaining rows
    for(int row_to_clear=row_to_fix+1; row_to_clear<n_rc; row_to_clear++){
      double mult = -mat_copy[row_to_clear][row_to_fix] / mat_copy[row_to_fix][row_to_fix];
      // can start at row_to_fix since first elements are already 0.0
      for(int c=row_to_fix; c<n_rc; c++){
        mat_copy[row_to_clear][c] += mult*mat_copy[row_to_fix][c];
      }      
    }
  }

  // trace of matrix is now 1.0 and matrix is traingular so det is 1.0
  // det has factor to multiply this by to get determinant of orignal matrix
  ok = true;
  return det;
}
#endif

/*
 * inverse
 * 
 * Function to find the inverse of a matrix
 */

const MatrixCM &MatrixCM::inverse()
{
  MatrixCM *inverse;
  MatrixCM *tmp;
  double det=0;
  int row, col, sign=1;
  
  assert( r_ == c_ );

  inverse = new MatrixCM(r_,c_);

  det = determinant();

  
  assert( det != 0 );
  
  for ( row=0; row<r_; row++ ) {
    for ( col=0; col<c_; col++ ) {
      /* 
	 This looks kind of confusing.  All it does is take the 
	 inverse and multiply each square by the determinant that
	 is reduced around the spot the matrix is being reduced by
	 For Instance:

	 In a 3x3 Matrix: A B C
	                  D E F
			  G H I

	 When computing with Element B, Element B is multiplied by
	 the determinant of the 2x2 matrix: D F
	                                    G I
      */

      
      tmp = reduce_Matrix(row,col);
      if ((row+col)%2 == 0) sign = 1;
      else sign = -1;
      inverse->e(col,row) = sign * tmp->determinant();
      delete tmp;
    }
  }
  
  inverse->scale(1/det);
  *this = *inverse;

  delete inverse;

  return *this;
}

void  
MatrixCM::jacobi(MatrixCM *v)
{
  int n,r,count,p,q; 
  int ntot=10;
  double a,b,d,t1,t2,theta,t,c,s,h,tau,g1,g2;
  MatrixCM *aa;

  n = this->nrows();  
  aa = new MatrixCM(*this);
  
  for (count=0; count < ntot; count++){ //ntot is # of sweeps
    
    for(p=0; p < n-1; p++){ //sweep through aa matrix
      for(q=p+1; q < n; q++){ //zero elements via Jacobi transformation
	
	if(aa->e(p,q) != 0.){
	  a=aa->e(p,p); //from here, its same as 2x2 case as in lecture
	  b=aa->e(p,q);
	  d=aa->e(q,q);
	  
	  theta=(d-a)/(2.*b);
	  t1=-theta+sqrt(theta*theta+1.);
	  t2=-theta-sqrt(theta*theta+1.);
	  
	  if(fabs(t1) < fabs(t2))
	    t=t1;
	  else
	    t=t2;
	  
	  c=1./sqrt(1.+t*t);
	  s=c*t;
	  h=t*b;
	  aa->e(p,p) = aa->e(p,p)-h;
	  aa->e(q,q) = aa->e(q,q)+h;
	  aa->e(p,q) = 0.0;
	  aa->e(q,p) = 0.0;
	  tau=s/(1.+c);
	  
	  for(r=0; r < p; r++){
	    g1=aa->e(p,r);                                    g2=aa->e(q,r);
	    aa->e(p,r) = g1-s*(g2+tau*g1);       aa->e(q,r) = g2+s*(g1-tau*g2);
	    aa->e(r,p) = aa->e(p,r);                          aa->e(r,q) = aa->e(q,r);
	  }
	  for(r=p+1; r < q; r++){
	    g1=aa->e(p,r);                                    g2=aa->e(q,r);
	    aa->e(p,r) = g1-s*(g2+tau*g1);       aa->e(q,r) = g2+s*(g1-tau*g2);
	    aa->e(r,p) = aa->e(p,r);                          aa->e(r,q) = aa->e(q,r);
	  }
	  for(r=q+1; r < n; r++){
	    g1=aa->e(p,r);                                    g2=aa->e(q,r);
	    aa->e(p,r) = g1-s*(g2+tau*g1);       aa->e(q,r) = g2+s*(g1-tau*g2);
	    aa->e(r,p) = aa->e(p,r);                          aa->e(r,q) = aa->e(q,r);
	  }
	  
	}
      }
    }
  }

  for(p=0; p<n; p++)
    v->e(0, p) =  aa->e(p,p);
  
  delete aa;
}

void 
MatrixCM::sete(int row, int col, double value)
{
	if((row < r_) && (col < c_)) 
		mat[row*c_+col] = value;
}
void 
MatrixCM::clear()
{
	for(int i=0; i<r_; i++)
		for(int j=0; j<c_; j++)
			e(i,j) = 0.0;
}
