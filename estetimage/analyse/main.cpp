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

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/objdetect/objdetect.hpp"

//#include <Eigen/Dense>
#include "ImageRGB.hpp"
#include "ioJPG.hpp"
#include "exif.h"

#include "photo.hpp"

#include "detector/include/faceDetector.hpp"
#include "detector/include/contourDetector.hpp"

#include <sqlite3.h> 
#include <jpeglib.h>

//using namespace kn;
using namespace cv;
using namespace std;


static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int getPixelColorType(int H, int S, int V){
        String color;

        int icolor = 0;

        if (V < 75)
            { color = "cBLACK"; icolor = 1; }
        else if (V > 190 && S < 27)
            { color = "cWHITE"; icolor = 2; }
        else if (S < 53 && V < 185)
            { color = "cGREY"; icolor = 3; }
        else {  // Is a color
            if (H < 14)
                { color = "cRED"; icolor = 4; }
            else if (H < 25)
                { color = "cORANGE"; icolor = 5; }
            else if (H < 34)
                { color = "cYELLOW"; icolor = 6; }
            else if (H < 73)
                { color = "cGREEN"; icolor = 7; }
            else if (H < 102)
                { color = "cAQUA"; icolor = 8; }
            else if (H < 127)
                { color = "cBLUE"; icolor = 9; }
            else if (H < 149)
                { color = "cPURPLE"; icolor = 10; }
            else if (H < 175)
                { color = "cPINK"; icolor = 11; }
            else    // full circle 
                { color = "cRED";  icolor = 4; }// back to Red
        }

        // cout << "global color: " << color << "icolor: " << icolor << endl;
        return icolor;
}


int hsvrgb(Mat src, int & icolor, int & h, int & s, int & v, int & r, int & g, int & b){
  Mat dst;

  /// Load image

  if( !src.data )
    { return -1; }

  /// Separate the image in 3 places ( B, G and R )
  vector<Mat> bgr_planes;
  split( src, bgr_planes );

  /// Establish the number of bins
  int histSize = 256;

  /// Set the ranges ( for B,G,R) )
  float range[] = { 0, 256 } ;
  const float* histRange = { range };

  bool uniform = true; bool accumulate = false;

  Mat b_hist, g_hist, r_hist;

  /// Compute the histograms:
  calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

  // Draw the histograms for B, G and R
  int hist_w = 512; int hist_h = 400;
  int bin_w = cvRound( (double) hist_w/histSize );

  Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

  /// Normalize the result to [ 0, histImage.rows ]
  normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

  /// Draw for each channel
  for( int i = 1; i < histSize; i++ )
  {
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                       Scalar( 255, 0, 0), 2, 8, 0  );
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                       Scalar( 0, 255, 0), 2, 8, 0  );
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                       Scalar( 0, 0, 255), 2, 8, 0  );
  }

  /// Display
  namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
  imshow("calcHist Demo", histImage );


  Mat hsv;
  // hsv = cvCloneImage(src);
  // cvCvtColor(src, hsv, CV_BGR2HSV);
  cvtColor(src, hsv, CV_BGR2HSV);

  //imshow("HSV", hsv );

  //H = hue = teinte
  //S = saturation
  //V = value/lightness
  h=0, s=0, v=0; 

  int nb_pixels = hsv.cols * hsv.rows;
  //String colors[] = {"cBLACK", "cWHITE", "cGREY", "cRED", "cORANGE", "cYELLOW", "cGREEN", "cAQUA", "cBLUE", "cPURPLE", "cPINK", "cRED"};
  for (int i = 0; i < hsv.rows; i++)
  {
      for (int j = 0; j < hsv.cols; j++)
      {
         //HSV
         h += (int)hsv.at<Vec3b>(i,j)[0];
         s += (int)hsv.at<Vec3b>(i,j)[1];
         v += (int)hsv.at<Vec3b>(i,j)[2];

         //BGR
         r += (int)src.at<Vec3b>(i,j)[2]; 
         g += (int)src.at<Vec3b>(i,j)[1];
         b += (int)src.at<Vec3b>(i,j)[0];

          
      }
  }

  h = h/nb_pixels;
  s = s/nb_pixels;
  v = v/nb_pixels;

  r = r/nb_pixels;
  g = g/nb_pixels;
  b = b/nb_pixels;

  // cout<<"h = " << h << " s = " << s << " v = " << v << " nb_pixels = " << nb_pixels << endl;
  // cout<<"r = " << r << " g = " << g << " b = " << b << " nb_pixels = " << nb_pixels << endl;
  icolor = getPixelColorType(h,s,v);


  return 1;

}

void detectPortrait(Mat & src, int & isPortrait, int & nbPers){
    nbPers = faceDetector(src, isPortrait);
}

void detectContour(Mat & src, int &nbContours){
    nbContours = contourDetector(src);
}


void calculatevarianceRGBHSV(Mat & src, int mean_r, int mean_g, int mean_b, int & var_r, int & var_g, int & var_b, int mean_h, int mean_s, int mean_v, int & var_h, int & var_s, int & var_v){

  int height = src.rows;
  int width = src.cols;

  for (int i = 0; i < height; i++){
    for (int j = 0; j < width; j++){
      
      var_b += ((int)src.at<Vec3b>(i,j)[0] - mean_b) * ((int)src.at<Vec3b>(i,j)[0] - mean_b); 
      var_g += ((int)src.at<Vec3b>(i,j)[1] - mean_g) * ((int)src.at<Vec3b>(i,j)[1] - mean_g); 
      var_r += ((int)src.at<Vec3b>(i,j)[2] - mean_r) * ((int)src.at<Vec3b>(i,j)[2] - mean_r); 
      var_h += ((int)src.at<Vec3b>(i,j)[0] - mean_h) * ((int)src.at<Vec3b>(i,j)[0] - mean_h); 
      var_s += ((int)src.at<Vec3b>(i,j)[1] - mean_s) * ((int)src.at<Vec3b>(i,j)[1] - mean_s); 
      var_v += ((int)src.at<Vec3b>(i,j)[2] - mean_v) * ((int)src.at<Vec3b>(i,j)[2] - mean_v); 

    }
  }

  var_b = sqrt(fabs(var_b / (height * width)));
  var_g = sqrt(fabs(var_g / (height * width)));
  var_r = sqrt(fabs(var_r / (height * width)));
  var_h = sqrt(fabs(var_h / (height * width)));
  var_s = sqrt(fabs(var_s / (height * width)));
  var_v = sqrt(fabs(var_v / (height * width)));

}


void loadImages(const int argc, char **argv, std::string & sql, sqlite3 *db){

  for(int i=1; i<argc; ++i){
     
      //load the image
      //std::cout << "loading '" << argv[i] << "' ...";
      Mat src = imread( argv[i], 1 );

      //std::cout << " done" << std::endl;

      //************ EXIF ****************************************//
      EXIFInfo exifReader;
      int parseSuccess = exifReader.parseFrom(fileToString(argv[i]));
      if(parseSuccess != PARSE_EXIF_SUCCESS){
        //exifParsingError(parseSuccess);
        //exit(0);
        std::cout << "exif parsing error : PARSE_EXIF_ERROR_NO_EXIF" << std::endl;
       
      }
      else if(exifReader.ISOSpeedRatings == 0 && exifReader.FNumber == 0 && exifReader.ExposureTime == 0){
        std::cout << "exif parsing error : ALL EXIF == 0" << std::endl;
      }
      else{ 
        /*
        std::cout << " ImageDescription : " << exifReader.ImageDescription << std::endl;
        std::cout << " wxh : " << exifReader.ImageWidth << " x " << exifReader.ImageHeight << std::endl;
        std::cout << " exposure : " << exifReader.ExposureTime << " s" << std::endl;
        std::cout << " flash : " << ((exifReader.Flash==0)?"no":"yes") << std::endl;
        std::cout << " camera : " << exifReader.Model << std::endl;
        std::cout << " ISO : " << exifReader.ISOSpeedRatings << std::endl;
        std::cout << " apperture : " << exifReader.FNumber << std::endl;
        std::cout << " MeteringMode : " << exifReader.MeteringMode << std::endl;
        std::cout << " FocusDistance: " << exifReader.SubjectDistance << " m" << std::endl;
        std::cout << std::endl;
        */      


      //************ EXIF DATA ***********************************//
      std::ostringstream ss_iso;
      ss_iso << exifReader.ISOSpeedRatings;

      std::stringstream ss_aperture;
      ss_aperture << exifReader.FNumber;

      std::ostringstream ss_shutterspeed;
      ss_shutterspeed << exifReader.ExposureTime;

      std::ostringstream ss_focus;
      ss_focus << exifReader.SubjectDistance;


      //************ H S V & R G B ********************************//
      int icolor = 0, h = 0, s = 0, v = 0, r = 0, g = 0, b = 0, var_r = 0, var_g = 0, var_b = 0, var_h = 0, var_s = 0, var_v = 0;
      hsvrgb(src, icolor, h, s, v, r, g, b);

      //************ VRIANCE **************************************//
      calculatevarianceRGBHSV(src, r, g, b, var_r, var_g, var_b, h, s, v, var_h, var_s, var_v);


      //************ CONTOURS *************************************//
      int isPortrait = 0; int nbPers = 0;
      detectPortrait(src, isPortrait, nbPers);

      //************ FACE DETECTION *******************************//
      int nbContours = 0;
      detectContour(src, nbContours);

      

      cout << "***********************************" << endl;
      cout << "***********************************" << endl;
      cout << "***********************************" << endl;
      cout << "path: " << argv[i] << endl;
      cout << "ss_iso: " << ss_iso.str() << endl;
      cout << "ss_shutterspeed: " << ss_shutterspeed.str() << endl;
      cout << "ss_aperture: " << ss_aperture.str() << endl;
      cout << "ss_focus: " << ss_focus.str() << endl;
      cout << "global color: " << icolor << endl;
      cout << "global hue: " << h << " var: " << var_h << endl;
      cout << "global saturation: " << s <<" var: " << var_s <<  endl;
      cout << "global lightness/value: " << v <<" var: " << var_v <<  endl;
      cout << "avg red: " << r << " var: " << var_r << endl;
      cout << "avg green: " << g << " var: " << var_g << endl;
      cout << "avg blue: " << b << " var: " << var_b << endl;
      cout << "Portrait (0/1): " << isPortrait << endl;
      cout << "Nombre faces: " << nbPers << endl;
      cout << "Nombre contours: " << nbContours << endl;
      cout << "***********************************" << endl;
      cout << "***********************************" << endl;
      cout << "***********************************" << endl;
      
      std::ostringstream oss_nbContours, oss_isPortrait, oss_nbPers, oss_icolor, oss_h, oss_s, oss_v, oss_r, oss_g, oss_b, oss_var_r, oss_var_g, oss_var_b, oss_var_h, oss_var_s, oss_var_v;

      sql += "INSERT INTO photo_param (path,shutterspeed,aperture,iso,focus, nbContours, isPortrait, nbfaces, dominant_color, global_hue, global_saturation, global_lightness, mean_red, mean_green, mean_blue, var_red, var_green, var_blue, var_h, var_s, var_v) VALUES ('";
        sql += argv[i];
        sql += "' , ";
        sql += ss_shutterspeed.str();
        sql += " , ";
        sql += ss_aperture.str();
        sql += " , ";
        sql += ss_iso.str();
        sql += " , ";
        sql += ss_focus.str();
        sql += ", ";
          oss_nbContours << nbContours;
        sql += oss_nbContours.str();
        sql += ", ";
          oss_isPortrait << isPortrait;
        sql += oss_isPortrait.str();
        sql += ", ";
          oss_nbPers << nbPers;
        sql += oss_nbPers.str();
        sql += ", ";
          oss_icolor << icolor;
        sql += oss_icolor.str();
        sql += ", ";
          oss_h << h;
        sql += oss_h.str();
        sql += ", ";
          oss_s << s;
        sql += oss_s.str();
        sql += ", ";
          oss_v << v;
        sql += oss_v.str();
        sql += ", ";
          oss_r<< r;
        sql += oss_r.str();
        sql += ", ";
          oss_g << g;
        sql += oss_g.str();
        sql += ", ";
          oss_b << b;
        sql += oss_b.str();
        sql += ", ";
          oss_var_r << var_r;
        sql += oss_var_r.str();
        sql += ", ";
          oss_var_g << var_g;
        sql += oss_var_g.str();
        sql += ", ";
          oss_var_b << var_b;
        sql += oss_var_b.str();
        sql += ", ";
          oss_var_h << var_h;
        sql += oss_var_h.str();
        sql += ", ";
          oss_var_s << var_s;
        sql += oss_var_s.str();
        sql += ", ";
          oss_var_v << var_v;
        sql += oss_var_v.str();
        //ctrl + mal + :
      sql+=");";
      }


    
  }


}





//////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
  if(argc < 2){
    std::cerr << "usage : " << argv[0] << " image_1.jpg ... image_n.jpg" << std::endl;
    std::cerr << "or : " << argv[0] << " dirname/*.jpg" << std::endl;
    exit(0);
  }

  sqlite3 *db;
  std::string sql;
  int rc;


  // Open database 
  char* db_path = (char*)"../db/database_images"; 

   rc = sqlite3_open(db_path, &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      //fprintf(stdout, "Opened database successfully\n");
   }

  //charge les images pour les analyser et preparer la requete sql
  loadImages(argc, argv, sql, db);

   // Execute SQL statement 
  
   std::string str;
   const char * query = sql.c_str();
   //std::cout << sql << std::endl;
   char* zErrMsg = 0;
   rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{//fprintf(stdout, "Table insert successfully\n");
   }

  sqlite3_close(db);


  //saveJPG(result,"output/result.jpg");



cvWaitKey(0);
      cout << "lel" << endl;

  return 0;
}