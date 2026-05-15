#include <cassert>

#include "bwBuilder.h"

namespace bWidgets
{
	namespace bwScreenGraph
	{
		bwBuilder::bwBuilder(bwLayoutNode& active_layout_node) : _active_layout_node(active_layout_node)
		{
		}

		bwBuilder::bwBuilder(bwScreenGraph& screen_graph) : bwBuilder(screen_graph.Root())
		{
		}

		void bwBuilder::setLayout(bwLayoutNode& node, std::unique_ptr<bwLayoutInterface> layout)
		{
			node.layout = std::move(layout);
		}

		void bwBuilder::setWidget(bwWidgetNode& node, std::unique_ptr<bwWidget> widget)
		{
			node.widget = std::move(widget);
			node.handler = node.widget->createHandler();
		}

		bwWidget& bwBuilder::addWidget(bwLayoutNode& node, std::unique_ptr<bwWidget> widget)
		{
			bwWidgetNode& node_ref = addChildNode<bwWidgetNode>(node);
			setWidget(node_ref, std::move(widget));
			return *node_ref.widget;
		}

		/**
		 * \brief Activate a layout node.
		 *
		 * Adding further items will add them as children to this node, until a
		 * different layout node is activated.
		 */
		void bwBuilder::setActiveLayout(bwLayoutNode& node)
		{
			_active_layout_node = node;
		}

	}  // namespace bwScreenGraph
}  // namespace bWidgets
