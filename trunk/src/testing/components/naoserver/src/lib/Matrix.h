/* LICENSE:
  =========================================================================
    CMPack'04 Source Code Release for OPEN-R SDK 1.1.5-r2 for ERS7
    Copyright (C) 2004 Multirobot Lab [Project Head: Manuela Veloso]
    School of Computer Science, Carnegie Mellon University
    All rights reserved.
  ========================================================================= */

#ifndef __CRAPPY_MATRIX_H__
#define __CRAPPY_MATRIX_H__

#include <string>

/**********************************************************************
 * 
 * Kwun Han <kwunh@cs.cmu.edu>
 * March 1997
 *
 * Michael Bowling <mhb@cs.cmu.edu>
 * 1998-2002
 *
 * Francisco Mart�n Rico <fmartin@gsyc.escet.urjc.es>
 * 2005
 * 
 * Determinant and inverse code is copied from mtrxmath under the GPL.
 *
 **********************************************************************/

class MatrixCM {

  // errr... rows and columns

  int r_;
  int c_;

  double* mat;

  void str_init(char* const init_string);

  MatrixCM *reduce_Matrix(int cut_row, int cut_col) const;
public:
  MatrixCM(char* const init_string);
  MatrixCM(int rows, int columns);
  MatrixCM(int rows, int columns, float *m);

  // this makes an identity matrix
  MatrixCM(int identity_size);

  // standard stuffs.
  MatrixCM();
  MatrixCM(const MatrixCM& other);

  ~MatrixCM();

  MatrixCM & set(double const *data);
  MatrixCM & set(float const *data);

  void CopyData(float *data);
  void CopyData(double *data);

  double determinant() const;

  const MatrixCM& operator= (const MatrixCM& other);

  const MatrixCM& operator= (char* const init_string);

  friend const MatrixCM operator+ (const MatrixCM& a, const MatrixCM& b);
  friend const MatrixCM operator- (const MatrixCM& a, const MatrixCM& b);
  friend const MatrixCM operator* (const MatrixCM& a, const MatrixCM& b);
  friend const MatrixCM inverse(const MatrixCM& a);
  friend const MatrixCM transpose(const MatrixCM& a);

  friend const MatrixCM& m_multiply(MatrixCM& out, const MatrixCM& a,
				  const MatrixCM& b);
  friend const MatrixCM& m_inverse(MatrixCM& out, const MatrixCM& in);
  friend const MatrixCM& m_add(MatrixCM& out, const MatrixCM& a,
			     const MatrixCM& b);
  friend const MatrixCM& m_subtract(MatrixCM& out, const MatrixCM& a,
				  const MatrixCM& b);
  friend const MatrixCM& m_transpose(MatrixCM& out, const MatrixCM& in);

  const MatrixCM& transpose();
  const MatrixCM& identity(int size);
  const MatrixCM& inverse();
  const MatrixCM& resize(int rows_cols);
  const MatrixCM& resize(int row, int col);
  
  const MatrixCM& scale(double factor);

  void sete(int row, int col, double value);

  inline double& e(int row, int col) const {return mat[row*c_+col];};
  inline double& e(int row) const {return mat[row*c_];};

  int nrows() const { return r_; }
  int ncols() const { return c_; }

  void jacobi(MatrixCM *v);
  void clear();
  void print() const;
};

#endif
