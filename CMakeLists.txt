# $Id: CMakeLists.txt 100923 2016-11-03 10:50:34Z gcosmo $

#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(MuImag)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Add external libraries
#
#link_libraries("/home/cihenp/g4proj/MuImag/include/libPrimaryHits.so")
include_directories("/home/cihenp/g4proj/MuImag/include")
set(PrimaryHits_LIBRARIES "/home/cihenp/g4proj/MuImag/lib/libPrimaryHits.so")
#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include ${Geant4_INCLUDE_DIR})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

find_package(ROOT)
include_directories(${ROOT_INCLUDE_DIR})

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(MuImag MuImag.cc ${sources} ${headers})
target_link_libraries(MuImag ${Geant4_LIBRARIES})
target_link_libraries(MuImag ${ROOT_LIBRARIES})
target_link_libraries(MuImag ${PrimaryHits_LIBRARIES})
#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B4a. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(MuImag_SCRIPTS
  MuImag.out
  MuImag.in
  gui.mac
  init_vis.mac
  run1.mac
  run2.mac
  vis.mac
  )

foreach(_script ${MuImag_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS MuImag DESTINATION bin)
