#pragma once

#include <stack>

#include "bwRectangle.h"

namespace bWidgets
{
	class bwStyle;

	namespace bwScreenGraph
	{
		class bwScreenGraph;
		class bwNode;

		class bwDrawer
		{
		public:
			static void draw(bwScreenGraph& screen_graph, bwStyle& style);
			static void drawSubtree(bwNode& subtree_root, bwStyle& style);

		private:
			bwDrawer(bwStyle& style);

			void drawSubtreeRecursive(bwNode& subtree_root);
			void drawNode(bwNode& node);
			void pushMask(const bwNode& node);
			void popMask();

			bwStyle& style;
			std::stack<bwRectanglePixel> maskrect_stack;
		};

	}  // namespace bwScreenGraph
}  // namespace bWidgets
