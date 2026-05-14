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

auto bwPreOrderIterator::operator!=(const bwPreOrderIterator& other) const -> bool
{
    return (is_root != other.is_root) ||
           (is_root ? (node != other.node) : (node_iter != other.node_iter));
}

auto bwPreOrderIterator::operator*() -> bwNode&
{
    return is_root ? *node : **node_iter;
}

void bwPreOrderIterator::triggerIterationEnd()
{
    node_iter.~NodeIterType();
    is_root = true;
    node = nullptr;
}

auto bwPreOrderIterator::hasExceededLastSibling() -> bool
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

auto bwPreOrderIterator::operator++() -> bwPreOrderIterator&
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

auto begin(bwNode& node) -> bwPreOrderIterator
{
    return bwPreOrderIterator(node);
}
auto end(bwNode&) -> bwPreOrderIterator
{
    return {};
}
auto begin(bwScreenGraph& screen_graph) -> bwPreOrderIterator
{
    return begin(screen_graph.Root());
}
auto end(bwScreenGraph& screen_graph) -> bwPreOrderIterator
{
    return end(screen_graph.Root());
}

}  // namespace bwScreenGraph
}  // namespace bWidgets
