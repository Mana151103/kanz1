#include<iostream>
#include<cstring>
#include<cstdlib>
#include<cmath>
#include<climits>

#ifndef __VECTOR__
#define __VECTOR__

class Vector{
 private:
  int Size;
  double *Element;
 public:
  Vector(int size=0);
  ~Vector(void);
  Vector(const Vector &arg);
  Vector &operator=(const Vector &arg);
  Vector(Vector &&arg);
  Vector &operator=(Vector &&arg);
  int size(void) const;
  double operator[](int index) const;
  double &operator[](int index);
  Vector operator+(void) const;
  Vector operator-(void) const;
  Vector &operator+=(const Vector &rhs);
  Vector &operator-=(const Vector &rhs);
  Vector operator+(const Vector &rhs) const;
  Vector operator-(const Vector &rhs) const;
};

std::ostream &operator<<(std::ostream &os, const Vector &rhs);
#endif
