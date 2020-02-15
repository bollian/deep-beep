cmake_minimum_required(VERSION 3.5)

project(deep-beep LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES pipeline.cpp data.cpp)
set(GUI_SOURCES main.cpp musicplayer.cpp musicreader.cpp qml.qrc)

find_package(Qt5 COMPONENTS Core Quick Multimedia REQUIRED)
find_package(OpenCV REQUIRED)

if(ANDROID)
    add_library(deep-beep SHARED ${GUI_SOURCES} ${SOURCES})
else()
    add_executable(deep-beep ${GUI_SOURCES} ${SOURCES})
    add_executable(imgproc imgproc.cpp ${SOURCES})
endif()

target_compile_definitions(deep-beep
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(deep-beep
    PRIVATE Qt5::Core Qt5::Quick Qt5::Multimedia ${OpenCV_LIBS})
target_link_libraries(imgproc ${OpenCV_LIBS})

# give us newer c++ features
set_property(TARGET deep-beep PROPERTY CXX_STANDARD 14)
set_property(TARGET imgproc PROPERTY CXX_STANDARD 14)
set_property(TARGET deep-beep PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET imgproc PROPERTY CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# These variables must use CACHE, otherwise QtCreator won't see them.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android" CACHE INTERNAL "")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so CACHE INTERNAL "")
#    endif()
#endif()
