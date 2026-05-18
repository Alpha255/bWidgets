#include "BlenderStage.h"
#include "builtin_widgets.h"
#include "blender_icon_defines.h"
#include "screen/bwLayout.h"
#include "bwStyleManager.h"
#include "screen_graph/bwBuilder.h"
#include "widgets/bwMenuBar.h"
#include "widgets/bwMenuButton.h"

using namespace bWidgets;

namespace bWidgetsDemo
{
	std::unique_ptr<bwScreenGraph::bwScreenGraph> BlenderStage::createScreenGraph(uint32_t width, uint32_t height)
	{
		auto rootContainer = std::make_unique<bwScreenGraph::bwContainerNode>();
		auto graph = std::make_unique<bwScreenGraph::bwScreenGraph>(std::move(rootContainer));

		bwScreenGraph::bwBuilder builder(*graph);
		builder.buildRootContainer<bwMenuBar>([](bwScreenGraph::bwBuilder& builder) {
			builder.addWidget<bwMenuButton>("File");
		},
		std::make_unique<bwColumnLayout>(true),
		width);

		return graph;
		// add menu bar layout
		// add split view layout
	}

	void BlenderStage::buildWidgets()
	{
		//bwScreenGraph::bwBuilder builder(*screen_graph);

		//// --- Menu Bar ---
		//auto& menu_row = builder.addLayout<bwRowLayout>(false);

		//auto& file_menu_node = builder.addContainer<bwPopupMenu>(
		//	std::make_unique<bwColumnLayout>(), "File");
		//auto& file_menu = static_cast<bwPopupMenu&>(*file_menu_node.Widget());
		//file_menu.addAction("New")
		//	.addAction("Open")
		//	.addSeparator()
		//	.addAction("Save")
		//	.addAction("Save As...")
		//	.addSeparator()
		//	.addAction("Exit");
		//builder.setActiveLayout(menu_row);

		//auto& edit_menu_node = builder.addContainer<bwPopupMenu>(
		//	std::make_unique<bwColumnLayout>(), "Edit");
		//auto& edit_menu = static_cast<bwPopupMenu&>(*edit_menu_node.Widget());
		//edit_menu.addAction("Undo")
		//	.addAction("Redo")
		//	.addSeparator()
		//	.addAction("Cut")
		//	.addAction("Copy")
		//	.addAction("Paste");
		//builder.setActiveLayout(menu_row);
	}
}  // namespace bWidgetsDemo
