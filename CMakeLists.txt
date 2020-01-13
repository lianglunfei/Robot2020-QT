cmake_minimum_required(VERSION 3.0)
project(RobotControlSystem)
set(CMAKE_VERBOSE_MAKEFILE OFF)
SET(QT_USE_QTMAIN TRUE)

set(QT_DIR D:/Qt/Qt5.12.0)
SET(QT_VERSION 5.12.0)
SET(QT_ARCH mingw73_64)
#set(CMAKE_EXE_LINKER_FLAGS -static)

# 自动调用moc，uic，rcc处理qt的扩展部分
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 设置Qt5的cmake模块所在目录，如果不设置将使用系统提供的版本
# QT_DIR和QT_VERSION是指定了qt安装目录和版本的环境变量
# 如果你使用了系统的cmake，那么会优先使用系统提供模块，因为cmake会优先搜索CMAKE_SYSTEM_PREFIX_PATH
# 如果你不想让cmake优先搜索系统目录（会导致编译使用系统安装的qt而不是我们配置的），需要提示find_package命令
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT_DIR}/${QT_VERSION}/${QT_ARCH}/lib/cmake)
MESSAGE(STATUS ${CMAKE_PREFIX_PATH})

#add_definitions(-DNOMINMAX)

set(CMAKE_BUILD_TYPE Release)
message("Generated with config types: ${CMAKE_CONFIGURATION_TYPES}")



find_package(Qt5 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
        Quick
        Qml
        Gamepad
        PrintSupport)



aux_source_directory(. DIRS_SRCS)
include_directories(AFTER libs/cx libs/gc libs/zlg)

#添加资源文件
file(GLOB_RECURSE SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS *.cpp)
file(GLOB_RECURSE HEAD_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS *.h)
FILE(GLOB_RECURSE UI_FILES "./*.ui")
SET(RCC_FILES logo.rc)
SET(QRC_FILES robot.qrc)
set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")

#add_executable(${PROJECT_NAME} ${SRC_FILES} )

add_executable(RobotControlSystem WIN32 ${DIRS_SRCS} ${HEAD_FILES} ${QRC_FILES} ${RCC_FILES} ${UI_FILES})




# set up a mapping so that the Release configuration for the Qt imported target is
# used in the COVERAGE CMake configuration.

get_target_property(QtCore_location Qt5::Core LOCATION)

MESSAGE(${QtCore_location})

set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")
set_target_properties(Qt5::Gui PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")
set_target_properties(Qt5::Widgets PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")
set_target_properties(Qt5::Quick PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")
set_target_properties(Qt5::Qml PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")
set_target_properties(Qt5::Gamepad PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")
set_target_properties(Qt5::PrintSupport PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")

#if(${CMAKE_CXX_COMPILER} MATCHES ".*/(g\\+\\+)(\\..*)")
#    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "-mwindows")
#elseif(${CMAKE_CXX_COMPILER} MATCHES ".*/(cl)(\\..*)")
#    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#endif()

target_link_libraries(${PROJECT_NAME} Qt5::Core
        Qt5::Widgets
        Qt5::Quick
        Qt5::Gui
        Qt5::Qml
        Qt5::Gamepad
        Qt5::PrintSupport)


IF (WIN32)
    MESSAGE(STATUS "WIN32 ")

    #    find_library(libs/cx ControlCAN REQUIRED)

    find_library(GC_CAN_LIBRARY ECanVci64 libs/gc REQUIRED)
    find_library(ZLG_CAN_LIBRARY ControlCAN libs/zlg REQUIRED)
    find_library(CX_CAN_LIBRARY ControlCAN libs/cx REQUIRED)
    find_library(WINDOWS_LIB qwindows libs REQUIRED)
    #    find_package(DbgHelp REQUIRED)
    MESSAGE(STATUS "find library ${ZLG_CAN_LIBRARY}")
    MESSAGE(STATUS "find library ${CX_CAN_LIBRARY}")
    MESSAGE(STATUS "find library ${GC_CAN_LIBRARY}")
    target_link_libraries(${PROJECT_NAME}
            ${ZLG_CAN_LIBRARY}
            ${CX_CAN_LIBRARY}
            ${GC_CAN_LIBRARY}
            DbgHelp
            )

ELSEIF (APPLE)
    MESSAGE(STATUS "Now is Apple systens.")
ELSEIF (UNIX)
    MESSAGE(STATUS "Now is UNIX-like.")
ENDIF ()
