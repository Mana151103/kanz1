#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<cmath>
#include<vector>
#include<climits>
#include<string>
#include<cfloat>
#include<random>
#include<algorithm>
#include<sys/stat.h>
#include<map>
#include<filesystem>
#include"vectorArray.h"

#ifndef __READDATA__
#define __READDATA__

//追加：kの値
#define Hyperparameter 1

VectorArray readTestData(const std::string &file, int dim);
Vector readTestCorrect(const std::string &file);

void recordClassified(const VectorArray &data, const Vector &label);

Vector read_data(const std::string &fileName, VectorArray &data);
Vector read_dataWithoutLabel(const std::string &fileName, VectorArray &data);
void read_data2(const std::string &fileName, VectorArray &data, Vector &label, int dimension);
void read_data3(const std::string &fileName, VectorArray &data, Vector &label);

void printData(VectorArray& trainData, VectorArray& tmp_testData);
void computeDistances(Vector& testData, VectorArray& trainData, Vector& distance, Vector& index);
void selectTopK(Vector& distance, Vector& index, int k);
Vector countLabels(Vector& trainCorrect, Vector& index, int k, int numLabels);
int getMajorityLabel(Vector& labelCount, Vector& maxCount, int i);
void serchK(VectorArray& tmp_testData, VectorArray& trainData, Vector& trainCorrect, Vector& testLabel, VectorArray& minIndexArray, int i,int k, Vector& maxCount);

void printResult(VectorArray& tmp_testD, VectorArray& trainD, Vector& trainCorrect, VectorArray& minIndexArray, Vector& testLabel, int k, Vector& maxCount);
double correlationCoefficient(VectorArray& testData, Vector& testLabel, VectorArray& trainData, Vector& trainCorrect, int c);

#endif