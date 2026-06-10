#include "paint/bwPaintEngine.h"
#include "paint/bwPainter.h"
#include "bwStyle.h"

#include "bwNode.h"
#include "bwScreenGraph.h"

#include "bwDrawer.h"

namespace bWidgets
{
	namespace bwScreenGraph
	{
		void bwDrawer::draw(bwScreenGraph& screen_graph)
		{
			bwDrawer drawer;
			drawer.drawSubtreeRecursive(screen_graph.Root());
		}

		void bwDrawer::drawSubtree(bwNode& subtree_root)
		{
			bwDrawer drawer;
			drawer.drawSubtreeRecursive(subtree_root);
		}

		void bwDrawer::drawSubtreeRecursive(bwNode& subtree_root)
		{
			const bool has_maskrect = subtree_root.MaskRectangle().has_value();

			drawNode(subtree_root);

			if (has_maskrect)
			{
				pushMask(subtree_root);
			}

			if (subtree_root.childrenVisible() && subtree_root.Children())
			{
				for (auto& child_node : *subtree_root.Children())
				{
					drawSubtreeRecursive(*child_node);
				}
			}

			if (has_maskrect)
			{
				popMask();
			}
		}

		void bwDrawer::drawNode(bwNode& node)
		{
			bwWidget* widget = node.Widget();
			if (!widget || !node.isVisible() || node.Rectangle().isEmpty())
			{
				return;
			}

			widget->draw();
		}

		void bwDrawer::pushMask(const bwNode& node)
		{
			bwRectanglePixel final_maskrect = *node.MaskRectangle();

			if (!maskrect_stack.empty())
			{
				final_maskrect.clamp(maskrect_stack.top());
			}
			maskrect_stack.push(final_maskrect);
			bwPainter::s_paint_engine->enableMask(maskrect_stack.top());
		}

		void bwDrawer::popMask()
		{
			maskrect_stack.pop();
			if (!maskrect_stack.empty())
			{
				bwPainter::s_paint_engine->enableMask(maskrect_stack.top());
			}
		}

	}  // namespace bwScreenGraph
}  // namespace bWidgets
