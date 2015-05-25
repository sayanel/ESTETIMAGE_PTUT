//GLINEUR MAXIMILIEN & BAPTISTE BOURRON

#include <iostream>
#include <Eigen/Eigenvalues>

#include "ioEigen.hpp"

using namespace Eigen;


void hints(void){

  MatrixXd M;
  
  // compute the column wize mean of a data matrix
  VectorXd mean =  M.colwise().sum() / (double) M.rows(); 
  
  // some hints to center some data (with the exterior product)
  std::cout << VectorXd::Ones(M.rows())*mean.transpose() << std::endl;

  // compute some eigen vectors
  SelfAdjointEigenSolver<MatrixXd> eigensolver(M);
  std::cout << "\neigenvectors of M \n" << eigensolver.eigenvectors().rowwise().reverse() << std::endl;
  std::cout << "\neigenvalues of M : " << eigensolver.eigenvalues().colwise().reverse().transpose() << std::endl;
  
  // extract some rows or columns from a matrix
  std::cout << M.leftCols(3) << std::endl; 
  std::cout << M.topRows(3) << std::endl; 
	
}

MatrixXd centerData(MatrixXd moyM, MatrixXd M){
  MatrixXd center = M - moyM;
  return center;
}

VectorXd meanData(MatrixXd M){
  VectorXd mean =  M.colwise().sum() / (double) M.rows(); 
  return mean;
}

MatrixXd normalize(MatrixXd M){
  MatrixXd normM;
  int nbInd = M.rows();
  VectorXd sigma = M.cwiseAbs().colwise().sum() / nbInd;
  std::cout << "\n***************** sigma  *****************\n" << sigma << std::endl; 
  
  //std::cout << "sigma\n" << sigma << std::endl; 
  MatrixXd sigmaM = VectorXd::Ones(M.rows()) * sigma.transpose();
  normM = M.cwiseQuotient(sigmaM);
  return normM;
}

MatrixXd covariance(MatrixXd M1, MatrixXd M2){
  MatrixXd covM;
     covM = (M2.transpose() * M2) / (double(M1.rows() -1));
  return covM;
}

int main(int argc, char **argv)
{
  if(argc != 2){
    std::cerr << "usage : " << argv[0] << " data.mat" << std::endl;
    exit(0);
  }
  
  // load the data
  MatrixXd A;
  loadMatrix(A,argv[1]);
  std::cout << "\n ***************** A *****************\n" << A << std::endl;

  // mean of the data
  VectorXd vmoyA;
  vmoyA = meanData(A);
  std::cout << "\n ***************** vmoyA *****************\n" << vmoyA << std::endl; 

  // center the data
  MatrixXd centerA;
  MatrixXd meanA;
  meanA = VectorXd::Ones(A.rows())*vmoyA.transpose();
  centerA = centerData(meanA,A);  
  std::cout << "\n ***************** centerA *****************\n" << centerA << std::endl; 

  // normalize the data
  MatrixXd normA = normalize(centerA);
  std::cout << "\n ***************** normA *****************\n" << normA << std::endl; 

  // build the covariance matrix 
  MatrixXd covA;
  covA = covariance(centerA, normA);
  std::cout << "\n ***************** covA *****************\n" << covA << std::endl;

  // compute the eigen vectors
  SelfAdjointEigenSolver<MatrixXd> eigensolver(covA);
  MatrixXd vectorEigen = eigensolver.eigenvectors().rowwise().reverse();

  std::cout << "\n ***************** eigenvectors of covA ***************** \n" << vectorEigen << std::endl;
  std::cout << "\n ***************** eigenvalues of covA ***************** \n " << eigensolver.eigenvalues().colwise().reverse() .transpose() << std::endl;


  // keep only n dimensions
  MatrixXd T = vectorEigen.leftCols(3).transpose();
  std::cout << "\n *****************  3 best eigenvectors rows of covA ***************** \n" << T << std::endl; 
  
  // project the data
  MatrixXd At = (T * normA.transpose()).transpose();
  std::cout << "\n ***************** At ***************** \n" << At << std::endl; 
  
  //test
  VectorXd x(A.cols());
  x << 5629, 0, 50, 0.005, 5.6, 0, 0, 9, 106, 58, 184, 144, 160, 183, 13, 11, 7, 14, 9, 14; 
  std::cout << "\n ***************** x *****************\n" << x << std::endl;

  MatrixXd scale = MatrixXd::Zero(A.cols(),A.cols());
  scale.diagonal() = centerA.cwiseAbs().colwise().sum() / (double) A.rows();
  scale = scale.inverse();
  VectorXd mean =  A.colwise().sum() / (double) A.rows(); 

  std::cout << "\n ***************** mes valeurs *****************\n" << x << std::endl;
  VectorXd xprim = T * scale * (x-mean);
  std::cout << "xprim \n" << xprim.transpose() << std::endl;

  //distance from data
  VectorXd distance = (At - VectorXd::Ones(At.rows()) * xprim.transpose()).rowwise().norm();
  std::cout << "resultat: \n" << distance << std::endl;


  // project a new vector (remind to center and scale this vector)
  

  return 0;
}
