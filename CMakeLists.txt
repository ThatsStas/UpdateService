CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
find_package(Git REQUIRED)
SET(projName dummy)

PROJECT(${projName})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

SET(incs
	logger.h
	configurator.h
)

SET(srcs
	main.cpp
)

SET(project_version_str "0.01")
SET(release_type		"beta")

add_definitions(-DVER_APPLICATION_VERSION_STR=\"${project_version_str}\")
add_definitions(-DVER_RELEASE_TYPE_STR=\"${release_type}\")
#add_definitions(-DVER_COMMIT_DATE_STR=\"${GIT_DATE}\")


add_executable(${projName} ${srcs} ${incs})
