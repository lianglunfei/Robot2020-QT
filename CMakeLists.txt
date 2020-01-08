cmake_minimum_required(VERSION 3.0)
project(RobotControlSystem)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(QT_DIR D:/Qt/Qt5.12.0)
SET(QT_VERSION 5.12.0)
SET(QT_ARCH mingw73_64)

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

add_definitions(-DNOMINMAX)

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


file(GLOB_RECURSE SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS *.cpp)
#同理，查找当前文件夹中所有的头文件
file(GLOB_RECURSE HEAD_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS *.h)


#添加资源文件
SET(RCC_FILES robot.qrc)
SET(UI_FILES robot.qrc)
SET(QRC_FILES robot.qrc)

#add_executable(${PROJECT_NAME} ${SRC_FILES} )

add_executable(RobotControlSystem ${DIRS_SRCS} ${HEAD_FILES} ${RCC_FILES})

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
    #    find_package(DbgHelp REQUIRED)
    MESSAGE(STATUS "find library ${ZLG_CAN_LIBRARY}")
    MESSAGE(STATUS "find library ${CX_CAN_LIBRARY}")
    MESSAGE(STATUS "find library ${GC_CAN_LIBRARY}")
    target_link_libraries(${PROJECT_NAME}
            ${ZLG_CAN_LIBRARY}
            ${CX_CAN_LIBRARY}
            ${GC_CAN_LIBRARY}
            DbgHelp)

ELSEIF (APPLE)
    MESSAGE(STATUS "Now is Apple systens.")
ELSEIF (UNIX)
    MESSAGE(STATUS "Now is UNIX-like.")
ENDIF ()
