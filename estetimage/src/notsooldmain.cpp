#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <iostream>
#include <iomanip>
#include <string>

//#include "opencv/highgui.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include "gphoto2/gphoto2-camera.h"

#include "include/params.h"
#include "include/parameters.hpp"
#include "include/capture.hpp"
#include "include/notifications.hpp"
#include "include/config.hpp"
#include "include/widget.hpp"
#include "include/pca.hpp"


#include "detector/include/faceDetector.hpp"
#include "detector/include/contourDetector.hpp"
#include "detector/include/globalTintDetector.hpp"

#include <sqlite3.h> 

//Compilo : 
//g++ -Wall -Wno-unused-local-typedefs main.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -lgphoto2 -lopencv_video -I ~/eigen  -o main
//ou, cd build_estetimacge ; cmake ../estetimacge ; make -j ; ./estetimacge

using namespace cv;
using namespace std;

Camera *camera;  
GPContext *context; 

 /** @function main */
 int main( int argc, const char** argv ){
    int nbContours, nbPers, isPortrait = 0;

    Mat image = imread( argv[1], 1 );
    globalTintDetector(image);


    nbPers = faceDetector(image, isPortrait);
    cout << "NbPersonnes = " << nbPers << endl;
    if(isPortrait) cout << "PORTRAIT" << endl;
    nbContours = contourDetector(image);
    cout << "NbContours = " << nbContours << endl;




    /*DISPLAY IMAGE*/
    Size size(image.cols/2, image.rows/2);
    Mat rz_image;
    //resize(image,rz_image,size);//resize image
    /// Create Window
    string source_window = "_Estetimacge_";
    namedWindow( source_window, CV_WINDOW_AUTOSIZE );
    //if(image.cols > 1000 || image.rows > 700) imshow( source_window, rz_image );
    imshow( source_window, image );

   
    imwrite( "../presentation/montagne.jpg", image );

	//int retval;
    //pca("../src/matrix/database.mat");
    //PCA pour test
    gp_camera_new (&camera);
    context = gp_context_new();

    waitKey(0); 
    return 0;
 }

