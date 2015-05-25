#include "include/faceDetector.hpp"



 /** Global variables */
 String face_cascade_name = "xml/haarcascade_frontalface_alt.xml";
 String eyes_cascade_name = "xml/haarcascade_eye_tree_eyeglasses.xml";
 CascadeClassifier face_cascade;
 CascadeClassifier eyes_cascade;
 // RNG rng(12345);

 /** @function main */
 int faceDetector(Mat & image, int & isPortrait)
 {

    //-- 1. Load the cascades
    if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
    if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

    int nbPersons = 0;
    if( !image.empty() ){ 
      nbPersons = detectAndDisplayFace(image, isPortrait); 
    }
    else{
      std::cout << " --(!) No captured frame -- Break!" << std::endl; 
    }
     
   return nbPersons;
 }




void portraitOrNotPortraitThatIsTheQuestion(int & isPortrait, int rayon, int width, int height){
    //cout << "center= " << center_x << " " << center_y<<  " et rayon= " << rayon << " w= " << width << " h= " << height << endl;

    double aireImage = width * height;
    double aireFace = 3.14 * rayon * rayon;
    //cout << "aireImage: " << aireImage << " aireFace: " << aireFace << endl;  
    if(aireFace > (aireImage/20)) isPortrait = 1;
    else if(isPortrait != 1) isPortrait = 0;
}




/** @function detectAndDisplay */
int detectAndDisplayFace(Mat & frame, int & isPortrait){

  RNG rng(12345);
  vector<Rect> faces;
  vector<Rect> eyes;
  Mat frame_gray;

  cvtColor( frame, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

  for( size_t i = 0; i < faces.size(); i++ )
  {
    Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
    ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 0, 0, 255 ), 10, 8, 0 );
    Mat faceROI = frame_gray( faces[i] );
    
    

    //-- In each face, detect eyes
    eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    for( size_t j = 0; j < eyes.size(); j++ )
     {
       Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
       int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
       circle( frame, center, radius, Scalar( 0, 0, 255 ), 4, 8, 0 );
     }

     if(eyes.size()  >0) portraitOrNotPortraitThatIsTheQuestion(isPortrait, faces[i].width/2, frame.cols, frame.rows);
  }

  //imshow( window_name, frame );
  
  int nbFaces = faces.size();
  int nbEyes = eyes.size();
  //int nbPersons;

  if(nbFaces >= 4 && nbEyes > 0) return 4;
  else if(nbFaces == 1 && nbEyes > 0) return 1;
  else if(nbFaces == 2 && nbEyes > 0) return 2;
  else if(nbFaces == 3 && nbEyes > 0) return 3;
  else return 0;
  
 }