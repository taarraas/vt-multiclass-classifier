cmake_minimum_required (VERSION 2.6)
project (vt-multiclass-classifier)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CMAKE_INSTALL_PREFIX "bin/")

set(SOURCES
	BinaryClassifier.cpp
	DataIO.cpp
	main.cpp
	MulticlassClassifier.cpp)

set(HEADERS
	BinaryClassifier.h
	DataIO.h
	MulticlassClassifier.h)

add_executable(classifier ${SOURCES})

install (TARGETS classifier DESTINATION .)

source_group("Headers" FILES ${HEADERS})
source_group("Sources" FILES ${SOURCES})
	
