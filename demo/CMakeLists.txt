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
	../src
	../src/generics
	../src/styling
	../src/utils
	../src/widgets
	${CMAKE_SOURCE_DIR}/demo/extern/glew/include
	resources
)

set(SRC
	bwidgets_demo.cc
	Application.cc
	DefaultStage.cc
	DefaultStageRNAFunctor.cc
	Stage.cc

	Application.h
	DefaultStage.h
	DefaultStageRNAFunctor.h
	Stage.h
	resources/blender_icon_defines.h
)

set(LIB
	bWidgets
)

add_executable(demo)
target_sources(demo PRIVATE ${SRC})
include_directories(${INC})
target_link_libraries(demo LINK_PRIVATE ${LIB})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})

if(WIN32)
	add_custom_command(
		TARGET demo POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
				$<TARGET_FILE:zlib>
				$<TARGET_FILE_DIR:demo>
	)
endif()

# --- Resource files ---

if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
	set(RESOURCES
		bfont.ttf
		Inter.ttf
		blender_icons16.png
		LICENSE-bfont.ttf.txt
		classic_style.css
		flat_light.css
		flat_dark.css
	)

	set(RESOURCES_PATH ${CMAKE_CURRENT_BINARY_DIR}/resources)
	foreach(RES ${RESOURCES})
		add_custom_command(
			TARGET demo PRE_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy
					${CMAKE_CURRENT_SOURCE_DIR}/resources/${RES}
					${RESOURCES_PATH}/${RES}
		)
	endforeach()

	add_custom_command(
		TARGET demo PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
				${CMAKE_CURRENT_SOURCE_DIR}/resources/icons_svg
				${RESOURCES_PATH}/icons_svg
	)

	add_custom_command(
		TARGET demo PRE_BUILD
		COMMAND ${CMAKE_COMMAND}
				-DICONS_SVG_DIR=${CMAKE_CURRENT_SOURCE_DIR}/resources/icons_svg
				-DOUTPUT_HEADER=${CMAKE_CURRENT_SOURCE_DIR}/resources/blender_icons_svg.h
				-P ${CMAKE_SOURCE_DIR}/src/GenerateIconsHeader.cmake
	)
else()
	set(RESOURCES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/resources)
endif()
add_definitions(-DRESOURCES_PATH_STR="${RESOURCES_PATH}")
