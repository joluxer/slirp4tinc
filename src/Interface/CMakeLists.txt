# CMakeLists.txt
#  Created on: 29.08.2020
#      Author: lode
#

ADD_LIBRARY(Interfaces STATIC
  FdSelectHandler.cpp
  Mutex.cpp
  ByteStreamDecorator.cpp
  ByteStream.cpp
)
