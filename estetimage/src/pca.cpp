#include "include/pca.hpp"

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
  char* db_path = (char*)"../estetimage/db/database_images"; 

   rc = sqlite3_open(db_path, &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stdout, "Opened database successfully\n");
   }
  
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
  //std::cout << M.leftCols(3) << std::endl; 
  //std::cout << M.topRows(3) << std::endl; 
  
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
  //std::cout << "\n***************** sigma  *****************\n" << sigma << std::endl; 
  
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

VectorXd pca(MatrixXd & A, VectorXd x, string matrix){

  /*if(argc != 2){
    std::cerr << "usage : " << argv[0] << " data.mat" << std::endl;
    exit(0);
  }*/

  // load the data
  loadMatrix(A,matrix);
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
  

  // x << 144, 160, 183, 13, 11, 7; 
  
  //std::cout << "\n ***************** x *****************\n" << x << std::endl;

  MatrixXd scale = MatrixXd::Zero(A.cols(),A.cols());
  scale.diagonal() = centerA.cwiseAbs().colwise().sum() / (double) A.rows();
  scale = scale.inverse();
  VectorXd mean =  A.colwise().sum() / (double) A.rows(); 

  //std::cout << "\n ***************** mes valeurs *****************\n" << x << std::endl;
  VectorXd xprim = T * scale * (x-mean);
  //std::cout << "xprim \n" << xprim.transpose() << std::endl;

  //distance from data
  VectorXd distance = (At - VectorXd::Ones(At.rows()) * xprim.transpose()).rowwise().norm();
  
  std::cout << "resultat: \n" << distance << std::endl;


  // project a new vector (remind to center and scale this vector)



  return distance;
}


Parameters getParametersFromInfos(Infos inf){
  Parameters p;
  
  std::ostringstream oss_nbContours, oss_isPortrait, oss_nbPers, oss_icolor, oss_h, oss_s, oss_v, oss_r, oss_g, oss_b, oss_var_r, oss_var_g, oss_var_b, oss_var_h, oss_var_s, oss_var_v;
  oss_nbContours << inf.nbContours;
  oss_icolor << inf.icolor;
  oss_r << inf.mean_r;
  oss_g << inf.mean_g;
  oss_b << inf.mean_b;
  oss_h << inf.mean_h;
  oss_s << inf.mean_s;
  oss_v << inf.mean_v;

  string sql;
  sql = "SELECT aperture, shutterspeed, iso FROM photo_param WHERE ";
  sql += "nbContours = "; sql += oss_nbContours.str();
  sql += " AND dominant_color = "; sql += oss_icolor.str();
  sql += " AND mean_red = "; sql += oss_r.str();
  sql += " AND mean_green = "; sql += oss_g.str();
  sql += " AND mean_blue = "; sql += oss_b.str();
  sql += " AND global_hue = "; sql += oss_h.str();
  sql += " AND global_saturation = "; sql += oss_s.str();
  sql += " AND global_lightness = "; sql += oss_v.str();

    char * res;
    std::vector< std::vector < std:: string > > result;
    for( int i = 0; i < 3; i++ )
      result.push_back(std::vector< std::string >());
    sqlite3 *db;
    sqlite3_stmt * stmt;
     const char * query = sql.c_str();
    if (sqlite3_open("../estetimacge/db/database_images", &db) == SQLITE_OK)
    {

      sqlite3_prepare( db, query, -1, &stmt, NULL );//preparing the statement
      sqlite3_step( stmt );//executing the statement
      while( sqlite3_column_text( stmt, 0 ) )
      {
          for( int i = 0; i < 3; i++ )
              result[i].push_back( std::string( (char *)sqlite3_column_text( stmt, i ) ) );
          sqlite3_step( stmt );

          p.aperture = stof(result[0][0]);
          p.shutterspeed = stof(result[1][0]);
          p.iso = stof(result[2][0]);
      }
    }
    else
    {
        cout << "Failed to open db\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

  return p;
}


//renvoie une struct Infos en fonction de l'index
Infos getInfosInMatrixFromIndex(MatrixXd & A, int index){
    Infos inf;

//nbCont, isPort, nbPers, icolor, h,s,v, vh, vs, vv, r,g,b, vr, vg, vb
  
    inf.nbContours = A.row(index)[0];
    inf.isPortrait = A.row(index)[1];
    inf.nbPers = A.row(index)[2];
    inf.icolor = A.row(index)[3];
    inf.mean_h = A.row(index)[4];
    inf.mean_s = A.row(index)[5];
    inf.mean_v = A.row(index)[6];
    inf.var_h = A.row(index)[7];
    inf.var_s = A.row(index)[8];
    inf.var_v = A.row(index)[9];
    inf.mean_r = A.row(index)[10];
    inf.mean_g = A.row(index)[11];
    inf.mean_b = A.row(index)[12];
    inf.var_r = A.row(index)[13];
    inf.var_g = A.row(index)[14];
    inf.var_b = A.row(index)[15];

    cout << "index = " << index << " -- inf.nbCOntours: " << inf.nbContours << endl;

    return inf;
}

//renvoie l'index de la ligne minimale
int getMin(VectorXd & result){
  float min = 1000;
  int i_min, i = 0;
  for(i = 0; i < result.size(); ++i){
    if(result[i] < min){
      min = result[i];
      i_min = i;
    } 
  }

  result[i_min] = 1000;

  cout << " min: " << min << " i_min: " << i_min << " " ;

  return i_min;
}

void getBestParameters(vector<Parameters> & best_params, MatrixXd & A, VectorXd & result){  

  for(int i = 0; i < 8; ++i){

      int index = getMin(result);
      Infos inf = getInfosInMatrixFromIndex(A, index);
      Parameters p =getParametersFromInfos(inf);
      if(p.aperture > 1 && p.shutterspeed > 0.0 && p.iso > 0 && p.aperture < 25 && p.iso < 6500 && p.shutterspeed > 0.00001){
        best_params.push_back(p);

      }
  }

}

//////////////////////////////////////////////////////////////////////////////////////////////
int getMatchesWitchPCA(VectorXd x,  vector<Parameters> & best_params)
{

  //fillTheMatrixx();
  string matrix = "matrix/data.mat";
  MatrixXd A;

  VectorXd result = pca(A, x, matrix); 

  getBestParameters(best_params, A, result);

  cout << "BEST PARAMETERS: " << endl;
  cout << "best_params size: " << best_params.size() << endl;
  for(int i = 0; i < best_params.size(); ++i){
    cout << best_params[i].aperture << " " ;
    cout << best_params[i].shutterspeed << " ";
    cout << best_params[i].iso << endl;
  }
  return 0;
}   


