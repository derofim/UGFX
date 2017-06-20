### Used to download and build SDL2.
## Use in target_include_directories: ${SDL2_FOLDER}/include 
## Use in target_link_libraries: ${SDL2_LIBRARY} 
## @note ${SDL2_LIBRARY} combines multiple libs by full path

set(SDL_NAME SDL) #@see Android build.
set(SDL_GIT_DOWNLOAD_PATH ${PROJECT_SOURCE_DIR}/thirdparty/${SDL_NAME})

set(SDL_ROOT_DIR ${SDL_GIT_DOWNLOAD_PATH}) #@see FindSDL2.cmake

set(PROJECT_BUILD_DIR ${PROJECT_SOURCE_DIR}/build)
set(SDL_BUILD_PATH ${PROJECT_BUILD_DIR}/lib)
set(SDL_BUILD_DIR ${SDL_BUILD_PATH}) #@see FindSDL2.cmake

set(GIT_REPO "https://github.com/spurious/SDL-mirror.git")
set(GIT_BRANCH "release-2.0.5")

include(ExternalProject)

find_package(SDL2 QUIET)

# Download from git without build
set(ONLY_DOWNLOAD_SDL2 "FALSE")

# Download from git with build
set(BUILD_SDL2 "FALSE")

# Force start/stop download
set(ALREADY_DOWNLOADED_SDL2 "FALSE")

# Checks for downloaded folders, may be improved by comparing with remote git
if((EXISTS "${SDL_GIT_DOWNLOAD_PATH}/include") AND (EXISTS "${SDL_GIT_DOWNLOAD_PATH}/src"))
	set(ALREADY_DOWNLOADED_SDL2 "TRUE")
endif()

# Needs full build
set(BUILD_SDL2 "TRUE")

if (NOT SDL2_FOUND)
    MESSAGE (STATUS "As ${SDL_NAME} was not found on your computer, we will fetch it from the source.")
	if(ONLY_DOWNLOAD_SDL2 STREQUAL "TRUE")
		if(ALREADY_DOWNLOADED_SDL2 STREQUAL "TRUE")
			# Build from PC folder
			MESSAGE (STATUS "${SDL_NAME} source already downloaded.")
		else()
			# Downloads in configure step. Dont use ExternalProject_Add here.
			MESSAGE (STATUS "Downloading ${SDL_NAME} without build.")
			set (git_cmd "git")
			message(STATUS "git cmd: ${git_cmd}")
			execute_process(COMMAND "git" clone -b ${GIT_BRANCH} "${GIT_REPO}" "${SDL_GIT_DOWNLOAD_PATH}"
			  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
			  RESULT_VARIABLE git_result
			  OUTPUT_VARIABLE git_out)
			message(STATUS "git result [${git_result}] out [${git_out}]")
		endif()
	else()
		if(BUILD_SDL2 STREQUAL "TRUE")
			if(ALREADY_DOWNLOADED_SDL2 STREQUAL "TRUE")
				# Build from PC folder
				MESSAGE (STATUS "As ${SDL_NAME} source already downloaded we will build it from ${SDL_GIT_DOWNLOAD_PATH}.")
				ExternalProject_Add(${SDL_NAME}
					INSTALL_DIR ${PROJECT_BUILD_DIR}
					SOURCE_DIR ${SDL_GIT_DOWNLOAD_PATH}
					CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
				)
			else()
				MESSAGE (STATUS "As ${SDL_NAME} source is not up-to date we will download and build it.")
				# Build from git
				ExternalProject_Add(${SDL_NAME}
					GIT_REPOSITORY ${GIT_REPO}
					GIT_TAG ${GIT_BRANCH}
					INSTALL_DIR ${PROJECT_BUILD_DIR}
					SOURCE_DIR ${SDL_GIT_DOWNLOAD_PATH}
					CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
				)
			endif()
		endif()
	endif()
endif()

# Used in target_include_directories
SET( SDL2_FOLDER ${SDL_GIT_DOWNLOAD_PATH} )

# Static library extention
set( LIB_EXT ${CMAKE_STATIC_LIBRARY_SUFFIX} )

# Project library files
SET( SDL2_LIBRARY ${SDL_BUILD_PATH}/SDL2${LIB_EXT} ${SDL_BUILD_PATH}/SDL2main${LIB_EXT} )