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

#include <sqlite3.h> 
#include <Eigen/Eigenvalues>

#include "ioEigen.hpp"

using namespace Eigen;
using namespace std;



static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

static int writeMatrix(void *data, int argc, char **argv, char **azColName){
   int i;
   //fprintf(stderr, "%s: ", (const char*)data);
   std::ofstream file( "data.mat", std::ios_base::app ); 
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
      
      //if(i !=2){
        char* j = argv[i];
        file << j;
        file << " ";        
      //} 



   }
   file << "\n";
   printf("\n");
   return 0;
}

int fillTheMatrixx(){
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  std::string sql;
  const char* data = "Callback function called";


  // Open database 
  char* db_path = (char*)"../db/database_images"; 

   rc = sqlite3_open(db_path, &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stdout, "Opened database successfully\n");
   }


   //lire la database ligne par ligne et les ajouter dans une matrice;
   //chaque ligne est une nouvelle ligne

    sql="SELECT nbContours, isPortrait, nbfaces, dominant_color, global_hue, global_saturation, global_lightness, var_h, var_s, var_v, mean_red, mean_green, mean_blue, var_red, var_green, var_blue FROM photo_param";
    // nbContours, isPortrait, nbfaces, dominant_color, global_hue, global_saturation, global_lightness, mean_red, mean_green, mean_blue, var_red, var_green, var_blue, var_h, var_s, var_v

   // Execute SQL statement   
  
   std::string str;
   const char * query = sql.c_str();
   //std::cout << sql << std::endl;

   rc = sqlite3_exec(db, query, writeMatrix, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Operation done successfully\n");
   }

  sqlite3_close(db);


  //saveJPG(result,"output/result.jpg");





  return 0;
}



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

int pca(int argc, char **argv){

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
 
  //mes valeurs
  //nbContours, isPortrait, nbfaces, dominant_color, global_hue, global_saturation, global_lightness, 
  //var_h, var_s, var_v, mean_red, mean_green, mean_blue, var_red, var_green, var_blue

  //pour test
  x << 20000, 7, 42, 73, 20, 17, 18, 28, 130, 175, 100, 2, 13, 4 ;
  
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



  return 0;  
}

//////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{

  fillTheMatrixx();

  //pca(argc,argv); 

  
  return 0;
}
