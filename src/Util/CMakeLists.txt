# CMakeLists.txt
#  Created on: 29.08.2020
#      Author: lode
#

ADD_LIBRARY(Utils STATIC
    PosixSemaphore.cpp
    HexdumpToByteStream.cpp
    PrintfToStream.cpp
    FdIoStream.cpp
)

TARGET_LINK_LIBRARIES(Utils
    Interfaces
    pthread
)
