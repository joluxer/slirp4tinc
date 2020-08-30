
INCLUDE_DIRECTORIES(Interface ${CMAKE_CURRENT_LIST_DIR})

ADD_SUBDIRECTORY(Interface)
ADD_SUBDIRECTORY(Util)
ADD_SUBDIRECTORY(umlplug)

ADD_EXECUTABLE(slirp4tinc
    main.cpp
    SlirpHandler.cpp
)

message("libVdeSlirp = ${libVdeSlirp}")

TARGET_LINK_LIBRARIES(slirp4tinc
    arg
    umlport
    Utils
    Interfaces
    ${libVdeSlirp}
    ${libSlirp}
)

INSTALL(TARGETS slirp4tinc RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
