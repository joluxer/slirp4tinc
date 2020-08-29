
# For licensing reasons, this library must be used as a shared library 
# if the associated SW is not published.

# sources
SET(src
    arg.cpp
    val.cpp
)

SET(CMAKE_INSTALL_RPATH "$ORIGIN")

# compile library
ADD_LIBRARY(arg SHARED ${src})
ADD_LIBRARY(arg_static STATIC ${src})

INSTALL(TARGETS arg LIBRARY DESTINATION lib)

