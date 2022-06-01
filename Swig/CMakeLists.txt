cmake_minimum_required(VERSION 3.16)
project(simplest CXX)


## Wrapper.Python
#set(... Release)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})

## numpy.get_include() 
include_directories("/opt/conda/lib/python3.9/site-packages/numpy/core/include")

## Wrapper.Python
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})
#include (UseSWIG)

include_directories("/home/jovyan/work/PyPhysX/Swig")
add_library(LibraryName SHARED physx.cpp)

set_property(SOURCE swigConfigurationFile.i PROPERTY CPLUSPLUS ON)
swig_add_library(SWIG_fileName LANGUAGE python SOURCES swigConfigurationFile.i)
#swig_link_libraries(SWIG_fileName LibraryName ${Python3_LIBRARIES})


## 0.0. Setting... 
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUERIRED ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-as-needed -ldl -pthread ")


## 1.0. HEADERS 
include_directories("${PHYSX_ROOT_DIR}/linux/PhysX/include")    #  
include_directories("${PHYSX_ROOT_DIR}/linux/PxShared/include") # foundation


## 2.0. LIBRARIES 
link_directories("${PHYSX_ROOT_DIR}/linux/PhysXSO/bin/linux.clang/release") 

set(DUMMIES 
    PhysXExtensions 
    PhysX 
    PhysXPvdSDK 
    PhysXVehicle 
    PhysXCharacterKinematic 
    PhysXCooking 
    PhysXCommon 
    PhysXFoundation 
   )

swig_link_libraries(SWIG_fileName LibraryName ${Python3_LIBRARIES})
