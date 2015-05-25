cmake_minimum_required(VERSION 2.8)

project (Estetimage)

set(EXECUTABLE_NAME estetimage)

#c++11 support
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


find_package( OpenCV REQUIRED )
if( OpenCV_FOUND )
list( APPEND ThirdParty_LIBS ${OpenCV_LIBS} )
include_directories( ${OpenCV_INCLUDE_DIRS} )
endif( OpenCV_FOUND )




IF( SQLITE3_INCLUDE_DIR AND SQLITE3_LIBRARY_RELEASE AND SQLITE3_LIBRARY_DEBUG )
    SET(SQLITE3_FIND_QUIETLY TRUE)
ENDIF( SQLITE3_INCLUDE_DIR AND SQLITE3_LIBRARY_RELEASE AND SQLITE3_LIBRARY_DEBUG )

FIND_PATH( SQLITE3_INCLUDE_DIR sqlite3.h )

FIND_LIBRARY(SQLITE3_LIBRARY_RELEASE NAMES sqlite3 )

FIND_LIBRARY(SQLITE3_LIBRARY_DEBUG NAMES sqlite3 sqlite3d HINTS /usr/lib/debug/usr/lib/ C:/Windows/System32/ )

IF( SQLITE3_LIBRARY_RELEASE OR SQLITE3_LIBRARY_DEBUG AND SQLITE3_INCLUDE_DIR )
SET( SQLITE3_FOUND TRUE )
ENDIF( SQLITE3_LIBRARY_RELEASE OR SQLITE3_LIBRARY_DEBUG AND SQLITE3_INCLUDE_DIR )

IF( SQLITE3_LIBRARY_DEBUG AND SQLITE3_LIBRARY_RELEASE )
# if the generator supports configuration types then set
# optimized and debug libraries, or if the CMAKE_BUILD_TYPE has a value
IF( CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE )
SET( SQLITE3_LIBRARIES optimized ${SQLITE3_LIBRARY_RELEASE} debug ${SQLITE3_LIBRARY_DEBUG} )
ELSE( CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE )
    # if there are no configuration types and CMAKE_BUILD_TYPE has no value
    # then just use the release libraries
SET( SQLITE3_LIBRARIES ${SQLITE3_LIBRARY_RELEASE} )
ENDIF( CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE )
ELSEIF( SQLITE3_LIBRARY_RELEASE )
SET( SQLITE3_LIBRARIES ${SQLITE3_LIBRARY_RELEASE} )
ELSE( SQLITE3_LIBRARY_DEBUG AND SQLITE3_LIBRARY_RELEASE )
SET( SQLITE3_LIBRARIES ${SQLITE3_LIBRARY_DEBUG} )
ENDIF( SQLITE3_LIBRARY_DEBUG AND SQLITE3_LIBRARY_RELEASE )

IF( SQLITE3_FOUND )
IF( NOT SQLITE3_FIND_QUIETLY )
MESSAGE( STATUS "Found Sqlite3 header file in ${SQLITE3_INCLUDE_DIR}")
MESSAGE( STATUS "Found Sqlite3 libraries: ${SQLITE3_LIBRARIES}")
ENDIF( NOT SQLITE3_FIND_QUIETLY )
ELSE(SQLITE3_FOUND)
IF( SQLITE3_FIND_REQUIRED)
MESSAGE( FATAL_ERROR "Could not find Sqlite3" )
ELSE( SQLITE3_FIND_REQUIRED)
MESSAGE( STATUS "Optional package Sqlite3 was not found" )
ENDIF( SQLITE3_FIND_REQUIRED)
ENDIF(SQLITE3_FOUND)






##Include eigen 
INCLUDE_DIRECTORIES ("~/eigen" )
##INCLUDE_DIRECTORIES ("~/OpenCV")
find_library( SQLITE3_LIBRARY_RELEASE sqlite3 )

## Compilation du programme
add_library(estetimage_lib src/capture.cpp src/parameters.cpp   src/config.cpp src/notifications.cpp src/widget.cpp src/ioEigen.cpp src/pca.cpp src/detector/faceDetector.cpp src/detector/contourDetector.cpp src/detector/landscapeDetector.cpp src/analyse.cpp src/filter.cpp src/parametersToIndex.cpp) 


## programme estetimacge qui link avec la librairie
add_executable(estetimage src/main.cpp)



FIND_PACKAGE(JPEG REQUIRED)
IF(JPEG_FOUND)
  INCLUDE_DIRECTORIES(${JPEG_INCLUDE_DIR})
  TARGET_LINK_LIBRARIES(estetimage ${JPEG_LIBRARIES})
ENDIF()

set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl -lsqlite3")
#opencv_core opencv_calib3d opencv_imgproc opencv_video opencv_features2d opencv_highgui opencv_nonfree #opencv_objdetect

target_link_libraries(estetimage jpeg gphoto2 ${OpenCV_LIBS} -I ~/eigen)


target_link_libraries(estetimage estetimage_lib)

message(STATUS "It's all good man.")
