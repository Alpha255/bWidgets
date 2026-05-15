#pragma once

#include <iterator>

#include "bwNode.h"
#include "bwScreenGraph.h"

namespace bWidgets
{
	namespace bwScreenGraph
	{
		/**
		 * \brief Iterator for pre-order (depth-first) traversal.
		 *
		 * Although recursion based algorithms are simple to understand (much simpler
		 * than the internals of this iterator at least), iteration based algorithms
		 * are often simpler to use, more readable (no need for lambdas or separate
		 * functions) and can minimize stack usage. This iterator tries to give all of
		 * these benefits.
		 *
		 * Stack memory should not be a concern with this iterator. It only stores a
		 * few bytes for the current item. However it does allocate a bit of heap
		 * memory to keep information about visited ancestors up to the root of
		 * iteration. Even that only adds trivial overhead though.
		 */

		class bwPreOrderIterator
		{
		public:
			bwPreOrderIterator();
			bwPreOrderIterator(bwNode& node);
			~bwPreOrderIterator();

			// Delete copy but keep move constructor for now. We store a parent path
			// below which would require deep copying. So prefer move over copy.
			bwPreOrderIterator(const bwPreOrderIterator&) = delete;
			bwPreOrderIterator(bwPreOrderIterator&&) noexcept;

			bool operator!=(const bwPreOrderIterator&) const;
			bwNode& operator*();
			bwPreOrderIterator& operator++();

		private:
			void triggerIterationEnd();
			bool hasExceededLastSibling();

			union
			{
				bwNode* node;
				bwNode::ChildList::iterator node_iter;
			};

			bwNode* root = nullptr;
			bool is_root = true;
			// Ancestors up to (but excluding!) node that started iteration.
			std::list<bwNode::ChildList::iterator> ancestors;
		};

		/* bwPreOrderIterator is the default iterator (implicitly chosen when passing a
		 * node as range-expression for range-based foor loops) */
		bwPreOrderIterator begin(bwNode&);
		bwPreOrderIterator end(bwNode&);
		bwPreOrderIterator begin(bwScreenGraph& screen_graph);
		bwPreOrderIterator end(bwScreenGraph& screen_graph);

	}  // namespace bwScreenGraph
}  // namespace bWidgets
