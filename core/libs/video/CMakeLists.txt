#
# Copyright (c) 2010-2019, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

include_directories($<TARGET_PROPERTY:Qt5::Widgets,INTERFACE_INCLUDE_DIRECTORIES>
                    $<TARGET_PROPERTY:Qt5::Core,INTERFACE_INCLUDE_DIRECTORIES>

                    $<TARGET_PROPERTY:KF5::I18n,INTERFACE_INCLUDE_DIRECTORIES>
                    $<TARGET_PROPERTY:KF5::ConfigCore,INTERFACE_INCLUDE_DIRECTORIES>
                    $<TARGET_PROPERTY:KF5::Service,INTERFACE_INCLUDE_DIRECTORIES>
)

set(libvideotools_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/manager/vidslidesettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/manager/vidslidethread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/manager/vidslidetask.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/player/vidplayerdlg.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/player/mediaplayerview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/player/slidevideo.cpp
)

add_library(videotools_src OBJECT ${libvideotools_SRCS})
