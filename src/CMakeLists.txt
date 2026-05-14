set(INC
	.
	generics
	styling
	styling/styles
	utils
	widgets
	${CMAKE_SOURCE_DIR}/demo
	${CMAKE_SOURCE_DIR}/demo/extern/glew/include
	${CMAKE_SOURCE_DIR}/demo/extern/glm
	${CMAKE_SOURCE_DIR}/demo/extern/gawain
	${CMAKE_SOURCE_DIR}/demo/extern/katana-parser/src
	${CMAKE_SOURCE_DIR}/demo/resources
	${CMAKE_SOURCE_DIR}/Submodules/freetype/include
	${CMAKE_SOURCE_DIR}/Submodules/nanosvg/src
	${CMAKE_SOURCE_DIR}/Submodules/magic_enum/include
)

set(SRC
	# event
	event/bwEvent.cc
	event/bwEventDispatcher.cc
	event/bwEventHandler.cc
	event/bwEvent.h
	event/bwEventDispatcher.h
	event/bwEventHandler.h

	# generics
	generics/bwColor.cc
	generics/bwGradient.cc
	generics/bwPoint.cc
	generics/bwPolygon.cc
	generics/bwColor.h
	generics/bwDistance.h
	generics/bwFunctorInterface.h
	generics/bwGradient.h
	generics/bwPoint.h
	generics/bwPolygon.h
	generics/bwRange.h
	generics/bwRectangle.h

	# paint
	paint/bwPainter.cc
	paint/GawainPaintEngine.cc
	paint/GawainPaintEngine.h
	paint/bwPaintEngine.h
	paint/bwPainter.h

	# paint/gpu
	paint/gpu/gpu.cc
	paint/gpu/gpu_matrix.cc
	paint/gpu/GPUShader.cc
	paint/gpu/GPU.h
	paint/gpu/GPUShader.h
	paint/gpu/GPU_glfw.h
	paint/gpu/gpu_intern.h

	# rna
	rna/RNAProperty.h
	rna/RNAScreenGraphBuilder.h

	# screen
	screen/bwFont.cc
	screen/bwIcon.cc
	screen/bwLayout.cc
	screen/bwPixmap.cc
	screen/bwFont.h
	screen/bwIcon.h
	screen/bwIconInterface.h
	screen/bwLayout.h
	screen/bwPixmap.h
	screen/bwLayoutInterface.h

	# screen_graph
	screen_graph/bwBuilder.cc
	screen_graph/bwDrawer.cc
	screen_graph/bwIterators.cc
	screen_graph/bwBuilder.h
	screen_graph/bwDrawer.h
	screen_graph/bwIterators.h
	screen_graph/bwNode.h
	screen_graph/bwScreenGraph.h
	screen_graph/bwContext.h

	# styling
	styling/bwStyle.cc
	styling/bwStyleCSS.cc
	styling/bwStyleManager.cc
	styling/bwStyleProperties.cc
	styling/bwWidgetBaseStyle.cc
	styling/styles/bwStyleClassic.cc
	styling/styles/bwStyleFlatGrey.cc
	styling/styles/bwStyleFlatDark.cc
	styling/styles/bwStyleFlatLight.cc
	styling/bwPreferences.h
	styling/bwStyle.h
	styling/bwStyleCSS.h
	styling/bwStyleManager.h
	styling/bwStyleProperties.h
	styling/bwWidgetBaseStyle.h
	styling/styles/bwStyleClassic.h
	styling/styles/bwStyleFlatDark.h
	styling/styles/bwStyleFlatGrey.h
	styling/styles/bwStyleFlatLight.h

	# stylesheet
	stylesheet/bwPropertyParser.cc
	stylesheet/bwStyleSheet.cc
	stylesheet/bwStyleSheetTree.cc
	stylesheet/bwPropertyParser.h
	stylesheet/bwStyleSheet.h
	stylesheet/bwStyleSheetTree.h

	# utils
	utils/File.cc
	utils/File.h
	utils/bwUtil.h
	utils/FixedNum.h

	# widgets
	widgets/bwAbstractButton.cc
	widgets/bwCheckbox.cc
	widgets/bwContainerWidget.cc
	widgets/bwLabel.cc
	widgets/bwPopupMenu.cc
	widgets/bwNumberSlider.cc
	widgets/bwPanel.cc
	widgets/bwPushButton.cc
	widgets/bwRadioButton.cc
	widgets/bwScrollBar.cc
	widgets/bwScrollView.cc
	widgets/bwTextBox.cc
	widgets/bwWidget.cc
	widgets/bwSplitView.cc
	widgets/builtin_widgets.h
	widgets/bwAbstractButton.h
	widgets/bwCheckbox.h
	widgets/bwContainerWidget.h
	widgets/bwLabel.h
	widgets/bwPopupMenu.h
	widgets/bwNumberSlider.h
	widgets/bwPanel.h
	widgets/bwPushButton.h
	widgets/bwRadioButton.h
	widgets/bwScrollBar.h
	widgets/bwScrollView.h
	widgets/bwTextBox.h
	widgets/bwWidget.h
	widgets/bwSplitView.h

	# window_manager
	window_manager/bwEvent.cc
	window_manager/bwEventManager.cc
	window_manager/bwWindow.cc
	window_manager/bwWindowManager.cc
	window_manager/bwEvent.h
	window_manager/bwEventManager.h
	window_manager/bwWindow.h
	window_manager/bwWindowManager.h
)

set(LIB
	bwd_extern_gawain
	bwd_extern_glew
	glfw
	freetype
	zlib
	libpng
	extern_katana
)

if (UNIX)
	set(OpenGL_GL_PREFERENCE GLVND)
endif()

find_package(OpenGL REQUIRED)
list(APPEND LIB ${OPENGL_LIBRARIES})

add_definitions(-DGLEW_STATIC)

add_library(bWidgets STATIC)
target_sources(bWidgets PRIVATE ${SRC})
include_directories(${INC})
target_link_libraries(bWidgets ${LIB})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})

# ----- shaders -----

if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
	set(SHADERS
		pixel_alpha_mask_texture_uniform_color_frag.glsl
		subpixel_alpha_mask_texture_uniform_color_frag.glsl
		texture_frag.glsl
		texture_vert.glsl
		smooth_color_frag.glsl
		smooth_color_vert.glsl
		uniform_color_vert.glsl
		uniform_color_frag.glsl
	)

	set(SHADERS_PATH ${CMAKE_CURRENT_BINARY_DIR}/shaders)
	foreach(SHADER ${SHADERS})
		add_custom_command(
			TARGET bWidgets PRE_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy
					${CMAKE_CURRENT_SOURCE_DIR}/paint/gpu/shaders/${SHADER}
					${SHADERS_PATH}/${SHADER}
		)
	endforeach()
else()
	set(SHADERS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/paint/gpu/shaders)
endif()
add_definitions(-DSHADERS_PATH_STR="${SHADERS_PATH}")
