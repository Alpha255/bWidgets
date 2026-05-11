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

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

set(GLM_STATIC_LIBRARY_ENABLE ON CACHE BOOL "" FORCE)

# Disable all warnings for 3rd party libs.
if (MSVC)
    add_compile_options(/W0)
else()
    add_compile_options(-w)
endif()

add_subdirectory(gawain)
add_subdirectory(glew)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(katana-parser)

set_property(TARGET bwd_extern_gawain PROPERTY FOLDER "Extern")
set_property(TARGET bwd_extern_glew PROPERTY FOLDER "Extern")
set_property(TARGET glfw PROPERTY FOLDER "Extern")
set_property(TARGET extern_katana PROPERTY FOLDER "Extern")
if(TARGET glm_static)
    set_property(TARGET glm_static PROPERTY FOLDER "Extern")
endif()
if(TARGET glm_shared)
    set_property(TARGET glm_shared PROPERTY FOLDER "Extern")
endif()
