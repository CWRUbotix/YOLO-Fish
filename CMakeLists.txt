CMAKE_MINIMUM_REQUIRED (VERSION 3.9)

project(darkhelp_video C CXX)

SET (CMAKE_BUILD_TYPE Release)
SET (CMAKE_CXX_STANDARD 17)
SET (CMAKE_CXX_STANDARD_REQUIRED ON)

IF (WIN32)
	ADD_COMPILE_OPTIONS ( /permissive- )	# stick to C++ standards (turn off Microsoft-specific extensions)
ELSE ()
	ADD_COMPILE_OPTIONS ( -Wall -Wextra -Werror -Wno-unused-parameter )
ENDIF ()

FIND_PACKAGE (Threads	REQUIRED)
FIND_PACKAGE (OpenCV	REQUIRED)
FIND_LIBRARY (DARKHELP	darkhelp)
FIND_LIBRARY (DARKNET	darknet	)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(darkhelp_video darkhelp_video.cpp)
add_executable(darkhelp_to_roboflow darkhelp_to_roboflow.cpp)
target_link_libraries(darkhelp_video Threads::Threads ${DARKHELP} ${DARKNET} ${OpenCV_LIBS})
target_link_libraries(darkhelp_to_roboflow Threads::Threads ${DARKHELP} ${DARKNET} ${OpenCV_LIBS})