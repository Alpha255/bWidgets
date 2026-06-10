#pragma once

#include <stack>

#include "bwRectangle.h"

namespace bWidgets
{
	namespace bwScreenGraph
	{
		class bwScreenGraph;
		class bwNode;

		class bwDrawer
		{
		public:
			static void draw(bwScreenGraph& screen_graph);
			static void drawSubtree(bwNode& subtree_root);

		private:

			void drawSubtreeRecursive(bwNode& subtree_root);
			void drawNode(bwNode& node);
			void pushMask(const bwNode& node);
			void popMask();

			std::stack<bwRectanglePixel> maskrect_stack;
		};

	}  // namespace bwScreenGraph
}  // namespace bWidgets
