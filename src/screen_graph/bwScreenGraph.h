#pragma once

#include "screen_graph/bwContext.h"
#include "event/bwEventDispatcher.h"

namespace bWidgets
{
namespace bwScreenGraph
{

class bwNode;
class bwLayoutNode;

class bwScreenGraph
{
public:
    template<typename _NodeType>
    bwScreenGraph(std::unique_ptr<_NodeType> _root_node)
        : event_dispatcher(*this), root_node(std::move(_root_node))
    {
    }

    auto Root() const -> bwLayoutNode&
    {
        return *root_node;
    }

    /** The context describing the state of this screen-graph */
    bwContext context;
    bwEventDispatcher event_dispatcher;

private:
    std::unique_ptr<bwLayoutNode> root_node;
};

}  // namespace bwScreenGraph
}  // namespace bWidgets
