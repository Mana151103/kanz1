#include<iostream>
#include<cstring>
#include"vector.h"

#ifndef __VECTORARRAY__
#define __VECTORARRAY__

class VectorArray{
 private:
  int Rows;
  Vector *Element;
 public:
  VectorArray(int rows=0, int cols=0);
  ~VectorArray(void);
  VectorArray(const VectorArray &arg);
  VectorArray &operator=(const VectorArray &arg);
  VectorArray(VectorArray &&arg);
  VectorArray &operator=(VectorArray &&arg);
  int rows(void) const;
  int cols(void) const;
  Vector operator[](int index) const;
  Vector &operator[](int index);
  VectorArray operator+(void) const;
  VectorArray operator-(void) const;
  VectorArray &operator+=(const VectorArray &rhs);
  VectorArray &operator-=(const VectorArray &rhs);
};

VectorArray operator+(const VectorArray &lhs, const VectorArray &rhs);
VectorArray operator-(const VectorArray &lhs, const VectorArray &rhs);
std::ostream &operator<<(std::ostream &os, const VectorArray &rhs);

#endif
