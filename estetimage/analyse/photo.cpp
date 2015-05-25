
#include "photo.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////
// open the file "filename" and copy the file content into a string (required for exif reader)
std::string fileToString(const std::string& filename){
  std::ifstream file(filename.c_str());//, std::ios::binary);
  if (!file) return "";
  std::string str(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
  return str;
}



//////////////////////////////////////////////////////////////////////////////////////////////
void exifParsingError(const int parseSuccess){
  switch(parseSuccess){
    case 1982 : // No JPEG markers found in buffer, possibly invalid JPEG file
      std::cout << "exif parsing error : PARSE_EXIF_ERROR_NO_JPEG" << std::endl;
    break;
    case 1983 : // No EXIF header found in JPEG file.
      std::cout << "exif parsing error : PARSE_EXIF_ERROR_NO_EXIF" << std::endl;
    break;
    case 1984 : // Byte alignment specified in EXIF file was unknown (not Motorola or Intel).
      std::cout << "exif parsing error : PARSE_EXIF_ERROR_UNKNOWN_BYTEALIGN" << std::endl;
    break;
    case 1985 : // EXIF header was found, but data was corrupted.
      std::cout << "exif parsing error : PARSE_EXIF_ERROR_CORRUPT" << std::endl;
    break;
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////
void loadImages(const int argc, char **argv, std::vector<ImageRGB8u> &images, std::vector<double> &exposure){

  for(int i=1; i<argc; ++i){
    //load the image
    ImageRGB8u imgageTmp;
    std::cout << "loading '" << argv[i] << "' ...";
    loadJPG(imgageTmp,argv[i]);
    images.push_back(imgageTmp);
    std::cout << " done" << std::endl;

    // load the exposure duration from the exif
    EXIFInfo exifReader;
    int parseSuccess = exifReader.parseFrom(fileToString(argv[i]));
    if(parseSuccess != PARSE_EXIF_SUCCESS){
      exifParsingError(parseSuccess);
      exit(0);
    }
    std::cout << " wxh : " << exifReader.ImageWidth << " x " << exifReader.ImageHeight << std::endl;
    std::cout << " exposure : " << exifReader.ExposureTime << " s" << std::endl;
    std::cout << " flash : " << ((exifReader.Flash==0)?"no":"yes") << std::endl;
    std::cout << " camera : " << exifReader.Model << std::endl;
    std::cout << " ISO : " << exifReader.ISOSpeedRatings << std::endl;
    std::cout << " apperture : " << exifReader.FNumber << std::endl;
    std::cout << std::endl;

    // update exposure
    exposure.push_back((double)exifReader.ExposureTime);
    //exposure.push_back((i+1) * 5.0);
  }
}
