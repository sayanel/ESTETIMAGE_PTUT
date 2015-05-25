#include "include/capture.hpp"


int capture (Camera *camera, GPContext *context, const char *filename) {

 int fd, retval;
 CameraFile *file;
 CameraFilePath camera_file_path;

 // this was done in the libphoto2 example code, but doesn't seem to be necessary
 // NOP: This gets overridden in the library to /capt0000.jpg
 //snprintf(camera_file_path.folder, 1024, "/");
 //snprintf(camera_file_path.name, 128, "foo.jpg");

 // take a shot
 retval = gp_camera_capture(camera, GP_CAPTURE_IMAGE, &camera_file_path, context);
 
 if (retval) {
  // do some error handling, probably return from this function
  cout <<"error gp_camera_capture" << endl;
 }

 printf("Pathname on the camera: %s/%s\n", camera_file_path.folder, camera_file_path.name);

 fd = open(filename, O_CREAT | O_WRONLY, 0644);
 // create new CameraFile object from a file descriptor
 retval = gp_file_new_from_fd(&file, fd);
 
 if (retval) {
  // error handling
 }

 // copy picture from camera
 
 retval = gp_camera_file_get(camera, camera_file_path.folder, camera_file_path.name,
   GP_FILE_TYPE_NORMAL, file, context);
 
 if (retval) {
  // error handling
 }

 printf("Deleting\n");
 // remove picture from camera memory
 retval = gp_camera_file_delete(camera, camera_file_path.folder, camera_file_path.name,
   context);
 
 if (retval) {
  // error handling
 }

 // free CameraFile object
 gp_file_free(file);

 // Code from here waits for camera to complete everything.
 // Trying to take two successive captures without waiting
 // will result in the camera getting randomly stuck.
 int waittime = 100;
 CameraEventType type;
 void *data;

 printf("Wait for events from camera\n");
 while(1) {
  retval = gp_camera_wait_for_event(camera, waittime, &type, &data, context);
  if(type == GP_EVENT_TIMEOUT) {
   break;
  }
  else if (type == GP_EVENT_CAPTURE_COMPLETE) {
   printf("Capture completed\n");
   waittime = 100;
  }
  else if (type != GP_EVENT_UNKNOWN) {
   printf("Unexpected event received from camera: %d\n", (int)type);
  }
 }
 return 0;
}







 /** @function main */
 
 int takephotos( int argc, const char** argv, Camera *camera, GPContext *context){
    //int nbContours, nbPers, isPortrait = 0;
    
    //Mat image = imread( argv[1], 1 );

    //nbPers = faceDetector(image, isPortrait);
    //cout << "NbPersonnes = " << nbPers << endl;
    //if(isPortrait) cout << "PORTRAIT" << endl;
    //nbContours = contourDetector(image);
    //cout << "NbContours = " << nbContours << endl;

    //detectAndDisplayLandscape(image);

 



    // Size size(image.cols/2, image.rows/2);
    // Mat rz_image;
    // resize(image,rz_image,size);//resize image
    /// Create Window
    // string source_window = "_Estetimacge_";
    // namedWindow( source_window, CV_WINDOW_AUTOSIZE );
    // imshow( source_window, image );
    //imshow( source_window, rz_image );

  //int retval;
    //pca("../src/matrix/database.mat");
    //PCA pour test

   /* 


    gp_camera_new (&camera);
    context = gp_context_new();

    printf("Camera init. Can take more than 10 seconds depending on the "
  "memory card's contents (remove card from camera to speed up).\n");
 int ret = gp_camera_init(camera, context);
 if (ret < GP_OK) {
  printf("No camera auto detected.\n");
  gp_camera_free(camera);
  return 1;
 }

 // take 10 shots
 char filename[256];
 int const nShots = 1;
 int nShotsDB = 0;
 int i;

cv::Mat img[10];

int decalagex = 30;
int decalagey = 30;




 for (i = 1; i <= nShots; i++) {
  snprintf(filename, 256, "shot-%04d.jpg", i);
  printf("Capturing to file %s\n", filename);
  print_parameters(camera, context);
  update_parameters(camera, context, aperture_tab[2],shutterspeed_tab[10],iso_tab[0]);

  // set_config_value_string(camera,"whitebalance","Auto",context);
  // set_config_value_string(camera,"meteringmode","Center-weighted average",context);
  // set_config_value_string(camera,"focusmode","AI Focus",context);
  // set_config_value_string(camera,"eoszoom","1",context);
  // set_config_value_string(camera,"manualfocusdrive","Near 1",context);
  // set_config_value_string(camera,"eoszoomposition","5,5",context);
  // set_config_value_string(camera,"whitebalanceadjusta","15",context);
  // set_config_value_string(camera,"whitebalanceadjustb","9",context);
  //
  //set_config_value_string(camera,"afdistance ","Auto",context);
  //set_config_value_string(camera,"exposurecompensation ","2",context);
  //set_config_value_string(camera,"iso","Auto",context);
  
  
  capture(camera, context, filename);
  print_parameters(camera, context);
  img[i] = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
 */
    /* comments car pas de capture 

  cv::resize(img[i], img[i], cvSize(640, 480));
  //DETECTON DE PORTRAIT FORCEE !
  //Mat image = imread( argv[1], 1 );
  nbPers = faceDetector(img[i], isPortrait);
  //nbPers = faceDetector(img[i], isPortrait);
  cout << "NbPersonnes = " << nbPers << endl;
  if(isPortrait) { 
    cout << "PORTRAIT" << endl;
    nShotsDB = 6;

  }
  //nbContours = contourDetector(img[i]);
  cout << "NbContours = " << nbContours << endl;
 
  cvNamedWindow (filename, CV_WINDOW_AUTOSIZE);
  cv::resize(img[i], img[i], cvSize(320, 240));
  cvMoveWindow(filename, decalagex, decalagey);
  decalagex +=321;
  if(i%3 == 0){ decalagey += 241; decalagex = 30; }
  
  //Mat image = imread( filename, 1 );
  
  

  cv::imshow(filename, img[i]);



  //cvShowImage (filename, img[i]);
 }*/


// DEUXIEME PRISE DE VUE APRES DETECTION

/*
nShotsDB = 6;
for (i = 1; i <= nShotsDB; i++) {
  snprintf(filename, 256, "shot-%04d.jpg", i);
  std::cout << "__________________________________________________" << std::endl;
  print_parameters(camera, context);
  //update_parameters(camera, context, aperture_tab[i*3],shutterspeed_tab[20-i],iso_tab[0]);
  std::cout << "Initialisation ..." << std::endl;
  switch(i){ 

    case 1:
        std::cout << "Initialisation premiere photo" << std::endl;
        update_parameters(camera, context, aperture_tab[2],shutterspeed_tab[27],iso_tab[0]);
        set_config_value_string(camera,"whitebalance", whitebalance_tab[0],context);
        set_config_value_string(camera,"aperture", aperture_tab[2],context);
        set_config_value_string(camera,"shutterspeed", shutterspeed_tab[27],context);
        break;
    case 2:
        std::cout << "Initialisation 2eme photo" << std::endl;
        update_parameters(camera, context, aperture_tab[4],shutterspeed_tab[22],iso_tab[0]);
        set_config_value_string(camera,"whitebalance", whitebalance_tab[1],context);
        set_config_value_string(camera,"aperture", aperture_tab[4],context);
        set_config_value_string(camera,"shutterspeed", shutterspeed_tab[22],context);
        break;
    case 3:
        std::cout << "Initialisation 3eme photo" << std::endl;
        update_parameters(camera, context, aperture_tab[5],shutterspeed_tab[20],iso_tab[0]);
        set_config_value_string(camera,"whitebalance", whitebalance_tab[0],context);
        set_config_value_string(camera,"aperture", aperture_tab[5],context);
        set_config_value_string(camera,"shutterspeed", shutterspeed_tab[20],context);
        break;
    case 4:
        std::cout << "Initialisation 4eme photo" << std::endl;
        update_parameters(camera, context, aperture_tab[10],shutterspeed_tab[16],iso_tab[0]);
        set_config_value_string(camera,"whitebalance", whitebalance_tab[3],context);
        set_config_value_string(camera,"aperture", aperture_tab[10],context);
        set_config_value_string(camera,"shutterspeed", shutterspeed_tab[16],context);
        break;
    case 5:
        std::cout << "Initialisation 5eme photo" << std::endl;
        update_parameters(camera, context, aperture_tab[13],shutterspeed_tab[12],iso_tab[0]);
        set_config_value_string(camera,"whitebalance", whitebalance_tab[0],context);
        set_config_value_string(camera,"aperture", aperture_tab[13],context);
        set_config_value_string(camera,"shutterspeed", shutterspeed_tab[12],context);
        break;
    case 6:
        std::cout << "Initialisation 6eme photo" << std::endl;
        update_parameters(camera, context, aperture_tab[16],shutterspeed_tab[6],iso_tab[0]);
        set_config_value_string(camera,"whitebalance", whitebalance_tab[5],context);
        set_config_value_string(camera,"aperture", aperture_tab[16],context);
        set_config_value_string(camera,"shutterspeed", shutterspeed_tab[6],context);
        break;


    default:
        break;
  }

  //set_config_value_string(camera,"whitebalance", whitebalance_tab[i],context);
  //set_config_value_string(camera,"aperture", whitebalance_tab[i],context);
  //std::cout << "modif FOCUS" << std::endl;
  //set_config_value_string(camera,"manualfocusdrive", manualfocusdrive_tab[i+2],context);

  //set_config_value_string(camera,"whitebalance","Auto",context);
  // set_config_value_string(camera,"meteringmode","Center-weighted average",context);
  // set_config_value_string(camera,"focusmode","AI Focus",context);
  // set_config_value_string(camera,"eoszoom","1",context);
  // set_config_value_string(camera,"manualfocusdrive","Near 1",context);
  //set_config_value_string(camera,"eoszoomposition","5,5",context);
  // set_config_value_string(camera,"whitebalanceadjusta","15",context);
  // set_config_value_string(camera,"whitebalanceadjustb","9",context);
  //
  //set_config_value_string(camera,"afdistance ","Auto",context);
  //set_config_value_string(camera,"exposurecompensation ","2",context);
  //set_config_value_string(camera,"iso","Auto",context);
  
  
  capture(camera, context, filename);
  printf("Capturing to file %s\n", filename);
  print_parameters(camera, context);
  img[i] = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
 

  cvNamedWindow (filename, CV_WINDOW_AUTOSIZE);
  cv::resize(img[i], img[i], cvSize(320, 240));
  cvMoveWindow(filename, decalagex, decalagey);
  decalagex +=321;
  if(i%3 == 0){ decalagey += 241; decalagex = 30; }
  
  //Mat image = imread( filename, 1 );
  
  

  cv::imshow(filename, img[i]);



  //cvShowImage (filename, img[i]);
 }

 // FIN DEUXIEME PRISE DE VUE




 // close camera
 gp_camera_unref(camera);
 gp_context_unref(context);

//END OF GPHOTO PART
 //STARTING OPENCV

  

  
  
  
  cvWaitKey(0);
  cvDestroyAllWindows();
  for(i = 0; i < nShots; i++){
    img[i].release();
    //cvReleaseImage(&img[i]);

    waitKey(0); 
    return 0;


    */
 }