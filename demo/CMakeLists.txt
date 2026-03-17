# ***** BEGIN GPL LICENSE BLOCK *****
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, see <http://www.gnu.org/licenses/>.
#
# Original work Copyright (c) 2018 Julian Eisel
#
# ***** END GPL LICENSE BLOCK *****

set(INC
	../bwidgets
	../bwidgets/generics
	../bwidgets/styling
	../bwidgets/utils
	../bwidgets/widgets
	./utils
	screen
	window_manager
)

set(SRC
	Application.cc
	Pixmap.cc
	bwidgets_demo.cc
	File.cc

	Application.h
	Pixmap.h
	File.h
	RNAProperty.h
	RNAScreenGraphBuilder.h
	resources/blender_icon_defines.h
)

set(LIB
	bwd_gpu
	bwd_window_manager
	bwd_screen
)

set(GPU_INC
	../extern/glew/include
	../extern/glm
)

if (UNIX)
  set(OpenGL_GL_PREFERENCE GLVND)
endif()

find_package(OpenGL REQUIRED)

list(APPEND LIB ${OPENGL_LIBRARIES})

add_definitions(-DGLEW_STATIC)

add_executable(bWidgetsDemo)
target_sources(bWidgetsDemo PRIVATE ${SRC})
include_directories(${INC})
target_link_libraries(bWidgetsDemo LINK_PRIVATE ${LIB})

if(WIN32)
    add_custom_command(
        TARGET bWidgetsDemo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:zlib>
                $<TARGET_FILE_DIR:bWidgetsDemo>
    )
endif()

# --- Resource files ---

if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
	set(RESOURCES
		bfont.ttf
		blender_icons16.png
		LICENSE-bfont.ttf.txt
		classic_style.css
		flat_light.css
		flat_dark.css
	)

	set(RESOURCES_PATH ${CMAKE_CURRENT_BINARY_DIR}/resources)
	foreach(RES ${RESOURCES})
		# copy resource files to build
		add_custom_command(
			TARGET bWidgetsDemo PRE_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy
					${CMAKE_CURRENT_SOURCE_DIR}/resources/${RES}
					${RESOURCES_PATH}/${RES}
		)
	endforeach()
else()
	set(RESOURCES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/resources)
endif()
add_definitions(-DRESOURCES_PATH_STR="${RESOURCES_PATH}")

# --- subdirectories ---

add_subdirectory(extern)
add_subdirectory(gpu)
add_subdirectory(screen)
add_subdirectory(stylesheet)
add_subdirectory(utils)
add_subdirectory(window_manager)
