#include "BlenderStage.h"
#include "builtin_widgets.h"
#include "blender_icon_defines.h"
#include "screen/bwLayout.h"
#include "bwStyleManager.h"
#include "screen_graph/bwBuilder.h"

using namespace bWidgets;

namespace bWidgetsDemo
{
	std::unique_ptr<bWidgets::bwScreenGraph::bwScreenGraph> BlenderStage::createScreenGraph(uint32_t width, uint32_t height)
	{
		auto container = std::make_unique<bwScreenGraph::bwContainerNode>();
		container->createLayout<bwColumnLayout>(true);

		return std::make_unique<bWidgets::bwScreenGraph::bwScreenGraph>(std::move(container));
	}

	void BlenderStage::buildWidgets()
	{
		bwScreenGraph::bwBuilder builder(*screen_graph);
	}
}  // namespace bWidgetsDemo