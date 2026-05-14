#include "bwIterators.h"
#include <cassert>

namespace bWidgets
{
namespace bwScreenGraph
{

using NodeIterType = bwNode::ChildList::iterator;

bwPreOrderIterator::bwPreOrderIterator() : node(nullptr)
{
}

bwPreOrderIterator::~bwPreOrderIterator()
{
    if (!is_root)
    {
        node_iter.~NodeIterType();
    }
}

bwPreOrderIterator::bwPreOrderIterator(bwNode& node) : node(&node), root(&node)
{
}

bwPreOrderIterator::bwPreOrderIterator(bwPreOrderIterator&& other)
    : ancestors(std::move(other.ancestors))
{
    if (other.is_root)
    {
        is_root = true;
        node = other.node;
        other.node = nullptr;
    }
    else
    {
        is_root = false;
        node_iter = other.node_iter;
        other.node_iter = {};
    }
    root = other.root;
    other.root = nullptr;
}

bool bwPreOrderIterator::operator!=(const bwPreOrderIterator& other) const
{
    return (is_root != other.is_root) ||
           (is_root ? (node != other.node) : (node_iter != other.node_iter));
}

bwNode& bwPreOrderIterator::operator*()
{
    return is_root ? *node : **node_iter;
}

void bwPreOrderIterator::triggerIterationEnd()
{
    node_iter.~NodeIterType();
    is_root = true;
    node = nullptr;
}

bool bwPreOrderIterator::hasExceededLastSibling()
{
    assert(is_root == false);
    // Check if node_iter points at the parents children.end()
    if (ancestors.empty())
    {
        return !root->Children() || (node_iter == root->Children()->end());
    }
    else
    {
        return !(*ancestors.back())->Children() ||
               (node_iter == (*ancestors.back())->Children()->end());
    }
}

bwPreOrderIterator& bwPreOrderIterator::operator++()
{
    bwNode::ChildList* childs = is_root ? node->Children() : (*node_iter)->Children();

    if (!childs || childs->empty())
    {
        if (is_root)
        {
            triggerIterationEnd();
            return *this;
        }

        node_iter++;
        if (hasExceededLastSibling())
        {
            if (ancestors.empty())
            {
                triggerIterationEnd();
                return *this;
            }
            /* Go up the hierarchy until we find a parent with siblings that
             * can be visited. Once hasExceededLastSibling returns false, we
             * have a valid item that can be returned. */
            for (auto parent_iter = ancestors.rbegin(); hasExceededLastSibling();
                 ancestors.pop_back())
            {
                if (ancestors.empty() || parent_iter == ancestors.crend())
                {
                    triggerIterationEnd();
                    break;
                }

                // next candidate for the iterator is the parents next sibling.
                node_iter = *parent_iter;
                ++node_iter;
            }
        }
    }
    else
    {
        if (!is_root)
        {
            ancestors.push_back(node_iter);
            node_iter.~NodeIterType();
        }
        else
        {
            node = nullptr;
        }
        node_iter = childs->begin();
        is_root = false;
    }

    return *this;
}

bwPreOrderIterator begin(bwNode& node)
{
    return bwPreOrderIterator(node);
}
bwPreOrderIterator end(bwNode&)
{
    return {};
}
bwPreOrderIterator begin(bwScreenGraph& screen_graph)
{
    return begin(screen_graph.Root());
}
bwPreOrderIterator end(bwScreenGraph& screen_graph)
{
    return end(screen_graph.Root());
}

}  // namespace bwScreenGraph
}  // namespace bWidgets
