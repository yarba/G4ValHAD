#----------------------------------------------------------------------------
#
#  Note-1: Env.variables G4INSTALL, G4LIB, ROOTSYS need to be setup
#
#  Note-2: Maybe ROOT can be pulled in via ROOT_INCLUDE_DIR - need to double check...
#
# Usage (for local testing/development):
#
# mkdir /tests/test75-build
# cd /tests/test75-build
# cmake -DGeant4_DIR=$G4LIB/Geant4-X.Y.Z \
#       ../test75
# make
#
# Note-3: in some cases, one would also use -DGeant4_SOURCE_DIR=$G4INSTALL option,
#         but here's it's not necessary - the system will even tell about that
#


# Specify mandatory
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

#----Included in Geant4------------------------------------------------------------------
#----Standalone--------------------------------------------------------------------------

#------------------------------------------------------------------------------
# - Enforce an out-of-source builds before anything else
#
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(STATUS "Geant4 requires an out-of-source build.")
    message(STATUS "Please remove these files from ${CMAKE_BINARY_DIR} first:")
    message(STATUS "CMakeCache.txt")
    message(STATUS "CMakeFiles")
    message(STATUS "Once these files are removed, create a separate directory")
    message(STATUS "and run CMake from there")
    message(FATAL_ERROR "in-source build detected")
endif()

project(test75)
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
# setup include directories
#
include_directories(${PROJECT_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/../test23/shared/g4app/include
                    ${Geant4_INCLUDE_DIR})

#----------------------------------------------------------------------------
#
# sources and headers for this project
#
file(GLOB prj_sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB prj_headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#
#  sources from shared code
#
file(GLOB g4app_sources
  ${PROJECT_SOURCE_DIR}/../test23/shared/g4app/src/*.cc)
file(GLOB g4app_headers
  ${PROJECT_SOURCE_DIR}/../test23/shared/g4app/include/*.hh)

#
# find additional ROOT package
#
find_package(ROOT QUIET)
if(NOT ROOT_FOUND)
  message(STATUS "G4 TESTS: ROOT package not found. ")
  return()
endif()

# add option specific to a group of tests
#
if (USE_G4PUBLIC)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_G4PUBLIC=${USE_G4PUBLIC}")
endif(USE_G4PUBLIC)
if (USE_G4REF)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_G4REF=${USE_G4REF}")
endif(USE_G4REF)

#
# additional headers and spources from ROOT (if any)
#
include_directories(${PROJECT_SOURCE_DIR}/../test23/shared/rootanalysis/include
  ${ROOT_INCLUDE_DIRS})
file(GLOB rootana_sources
  ${PROJECT_SOURCE_DIR}/../test23/shared/rootanalysis/src/*.cc)
file(GLOB rootana_headers
  ${PROJECT_SOURCE_DIR}/../test23/shared/rootana/include/*.hh)

#
# finally, put all headers and sources together
#
list(APPEND sources ${prj_sources} ${g4app_sources} ${rootana_sources})
list(APPEND headers ${prj_headers} ${g4app_headers} ${rootana_headers})


#----------------------------------------------------------------------------
#
# specify what executable, and what are the sources for it...
#
add_executable(test75 test75.cc ${sources} ${headers})

#
# ... plus, the external libraries
#
target_link_libraries(test75 ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

# copy over the ctest config
#
#set(test75_SCRIPTS ctest75-gam300-Cu.config PlotGammaN.C ${PROJECT_SOURCE_DIR}/batch/*.sh)
#foreach(_script ${test75_SCRIPTS})
#   configure_file(${PROJECT_SOURCE_DIR}/${_script} ${PROJECT_BINARY_DIR}/${_script} COPYONLY)
#endforeach()

# alternative approach:
#
file(GLOB test75_FILES RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/ctest75-gam300-Cu.config
                                                      ${PROJECT_SOURCE_DIR}/test75.gamma
						      ${PROJECT_SOURCE_DIR}/*.C
						      ${PROJECT_SOURCE_DIR}/batch/*.sh
						      ${PROJECT_SOURCE_DIR}/*.sh
						      ${PROJECT_SOURCE_DIR}/g4val-upload-scripts/gen_*.sh
                                                      )
foreach(_file ${test75_FILES})
    configure_file(
       ${PROJECT_SOURCE_DIR}/${_file} ${PROJECT_BINARY_DIR}/${_file} COPYONLY
       )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS test75 DESTINATION bin)

# -------------------------------
# This is a leftover from earlier days, and it's kep here just as an example
#
### if(CMAKE_PROJECT_NAME STREQUAL Geant4)
###  find_package(Geant4 REQUIRED)
###  include(${Geant4_USE_FILE})
###  find_package(ROOT QUIET)
###  if(NOT ROOT_FOUND)
###    message(STATUS "G4 TESTS: ROOT package not found. --> test75 disabled")
###    return()
###  endif()
###  GEANT4_ADD_TEST(test75-build BUILD test75)
####  GEANT4_ADD_TEST(test75 COMMAND test75 ${PROJECT_SOURCE_DIR}/ctest75-gam300-Cu.config
####                         DEPENDS test75-build
####                         ENVIRONMENT ${GEANT4_TEST_ENVIRONMENT})
### endif()