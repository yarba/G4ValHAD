#----------------------------------------------------------------------------
#
#  Note-1: Env.variables G4INSTALL, ROOTSYS need to be setup
#
# Usage (for local testing/development):
#
# mkdir /tests/ctests_integration/test47-build
# cd /tests/ctests_integration/test47-build
# cmake -DCMAKE_PREFIX_PATH=$G4INSTALL \
#       -DCMAKE_CXX_COMPILER=g++ \
#       ../test47
# make
#
# Note-2 : To activate interface to FLUKA.CERN use flags -DG4_USE_FLUKA=1
#          BEAR IN MIND to setup FLUKA.CERN as described in 
#          examples/extended/hadronic/FlukaCern/FlukaInterface/README.md
#


# Specify mandatory
#
cmake_minimum_required(VERSION 3.16...3.27)

#----Included in Geant4------------------------------------------------------------------
#----Standalone--------------------------------------------------------------------------

#------------------------------------------------------------------------------
# - Enforce an out-of-source builds before anything else
#
if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    message(STATUS "Geant4 requires an out-of-source build.")
    message(STATUS "Please remove these files from ${CMAKE_CURRENT_BINARY_DIR} first:")
    message(STATUS "CMakeCache.txt")
    message(STATUS "CMakeFiles")
    message(STATUS "Once these files are removed, create a separate directory")
    message(STATUS "and run CMake from there")
    message(FATAL_ERROR "in-source build detected")
endif()

project(test47)
# message("Project Name: ${CMAKE_PROJECT_NAME}")

#----------------------------------------------------------------------------
# Find Geant4 package, no UI and Vis drivers activated
#
find_package(Geant4 REQUIRED)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Check whether FLUKA should be used or not
set(G4_USE_FLUKA OFF CACHE BOOL "Using FLUKA")
if(G4_USE_FLUKA)
  message(STATUS "G4_USE_FLUKA=ON : Using FLUKA interface for building ${PROJECT_SOURCE_DIR}")
  add_definitions(-DG4_USE_FLUKA)
  find_package(FLUKAInterface REQUIRED)
  if(FLUKAInterface_FOUND)
    message(STATUS "FLUKA cmake module was found : ${CMAKE_MODULE_PATH}")
  else()
    message(FATAL_ERROR "FLUKA cmake module was NOT found! Please add one.")
  endif()
else()
  message(STATUS "G4_USE_FLUKA=OFF : NOT using FLUKA interface for building ${PROJECT_SOURCE_DIR}. \n \
  If ever you want to use the FLUKA interface, please repeat cmake command with -DG4_USE_FLUKA=1")
endif()

#----------------------------------------------------------------------------
# setup include directories
#
include_directories(${PROJECT_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/../test23/shared/g4app/include
		    ${FLUKAInterface_INCLUDE_DIR}
                    ${Geant4_INCLUDE_DIR})

#----------------------------------------------------------------------------
#
# sources and headers for this project
#
file(GLOB prj_sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB prj_headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#  sources from shared code
#
file(GLOB g4app_sources
  ${PROJECT_SOURCE_DIR}/../test23/shared/g4app/src/*.cc)
file(GLOB g4app_headers
  ${PROJECT_SOURCE_DIR}/../test23/shared/g4app/include/*.hh)

# find additional ROOT package
#
find_package(ROOT QUIET)
if(NOT ROOT_FOUND)
  message(STATUS "G4 TESTS: ROOT package not found --> test47 disabled ")
  return()
endif()

# additional headers and spources from ROOT (if any)
#
include_directories(${PROJECT_SOURCE_DIR}/../test23/shared/rootanalysis/include
  ${ROOT_INCLUDE_DIRS})
file(GLOB rootana_sources
  ${PROJECT_SOURCE_DIR}/../test23/shared/rootanalysis/src/*.cc)
file(GLOB rootana_headers
  ${PROJECT_SOURCE_DIR}/../test23/shared/rootana/include/*.hh)

# finally, put all headers and sources together
# NOTE: NO need to explicitly add anythings from FlukaCern example/interface
#
list(APPEND sources ${prj_sources} ${g4app_sources} ${rootana_sources})
list(APPEND headers ${prj_headers} ${g4app_headers} ${rootana_headers})

#----------------------------------------------------------------------------
#
# specify what executable, and what are the sources for it...
#
add_executable(test47 test47.cc ${sources} ${headers})

# ... plus, the external libraries
#
target_link_libraries(test47 ${Geant4_LIBRARIES} ${FLUKAInterface_LIBRARIES} ${ROOT_LIBRARIES})

# copy over the ctest config
#
#set(test47_SCRIPTS ctest47-itep-proton.config)
#foreach(_script ${test47_SCRIPTS})
#   configure_file(${PROJECT_SOURCE_DIR}/${_script} ${PROJECT_BINARY_DIR}/${_script} COPYONLY)
#endforeach()

# alternative approach:
#
file(GLOB test47_FILES RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/ctest47-itep-proton.config
                                                      ${PROJECT_SOURCE_DIR}/test47*.itep
                                                      ${PROJECT_SOURCE_DIR}/test47*.itep.Mu2e
						      ${PROJECT_SOURCE_DIR}/*.C
						      ${PROJECT_SOURCE_DIR}/batch/*.sh
						      ${PROJECT_SOURCE_DIR}/*.sh
						      ${PROJECT_SOURCE_DIR}/g4val-upload-scripts/gen_*.sh
                                                      )
foreach(_file ${test47_FILES})
    configure_file(
       ${PROJECT_SOURCE_DIR}/${_file} ${PROJECT_BINARY_DIR}/${_file} COPYONLY
       )
endforeach()
