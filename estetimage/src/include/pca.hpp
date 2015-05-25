#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <cstring>
#include <vector>
#include <ctime>
#include <fstream>
#include <math.h>
#include <fcntl.h>

#include <string>
#include <sqlite3.h> 
#include <Eigen/Eigenvalues>

#include "ioEigen.hpp"
#include "parameters.hpp"
#include "analyse.hpp"

using namespace Eigen;
using namespace std;



static int callback(void *data, int argc, char **argv, char **azColName);

static int writeMatrix(void *data, int argc, char **argv, char **azColName);

int fillTheMatrixx();

void hints(void);

MatrixXd centerData(MatrixXd moyM, MatrixXd M);

VectorXd meanData(MatrixXd M);

MatrixXd normalize(MatrixXd M);

MatrixXd covariance(MatrixXd M1, MatrixXd M2);

VectorXd pca(MatrixXd & A, VectorXd x, string matrix);

Parameters getParametersFromInfos(Infos inf);

Infos getInfosInMatrixFromIndex(MatrixXd & A, int index);

void getBestParameters(vector<Parameters> & best_params, MatrixXd & A, VectorXd & result);

int getMatchesWitchPCA(VectorXd x, vector<Parameters> & best_params);
