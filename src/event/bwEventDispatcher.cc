#include <iostream>

#include "event/bwEvent.h"
#include "event/bwEventHandler.h"
#include "screen_graph/bwNode.h"
#include "screen_graph/bwScreenGraph.h"

#include "event/bwEventDispatcher.h"

namespace bWidgets
{

bwEventDispatcher::bwEventDispatcher(bwScreenGraph::bwScreenGraph& _screen_graph)
    : screen_graph(_screen_graph), context(_screen_graph.context)
{
}

template<typename... _Args>
using HandlerFunc = void (bwScreenGraph::bwEventHandler::*)(_Args&&...);

template<typename... _Args>
static void bubbleEvent(const bwEvent& event,
                        const bwScreenGraph::bwNode& from_node,
                        HandlerFunc<_Args&&...> handler_func,
                        _Args&&... __args)
{
    for (const bwScreenGraph::bwNode* node = &from_node; node && !event.isSwallowed();
         node = node->Parent())
    {
        if (bwScreenGraph::bwEventHandler* handler = node->eventHandler())
        {
            (handler->*handler_func)(std::forward<_Args>(__args)...);
        }
    }
}

static bwScreenGraph::bwNode* findHoveredNode(const bwEvent& event, bwScreenGraph::bwNode& node)
{
    const bool is_hovered = node.isVisible() && node.Rectangle().isCoordinateInside(
                                                    event.location.x, event.location.y);

    if (is_hovered && node.Children() && node.childrenVisible())
    {
        for (auto& iter_child : *node.Children())
        {
            if (bwScreenGraph::bwNode* found_child = findHoveredNode(event, *iter_child))
            {
                return found_child;
            }
        }
    }

    return is_hovered ? &node : nullptr;
}

void bwEventDispatcher::dispatchMouseMovement(bwEvent event)
{
    if (drag_event)
    {
        drag_event->addMouseMovement(event.location);
    }

    if (bwScreenGraph::bwNode* active = context.active)
    {
        if (isDragging())
        {
            bubbleEvent<bwMouseButtonDragEvent&>(
                event, *active, &bwScreenGraph::bwEventHandler::onMouseDrag, drag_event.value());
        }
    }
    else
    {
        bwScreenGraph::bwNode* new_hovered = findHoveredNode(event, screen_graph.Root());

        if (new_hovered && (new_hovered == context.hovered))
        {
            bubbleEvent<bwEvent&>(
                event, *new_hovered, &bwScreenGraph::bwEventHandler::onMouseMove, event);
        }
        changeContextHovered(new_hovered, event);
    }
}

void bwEventDispatcher::dispatchMouseButtonPress(bwMouseButtonEvent& event)
{
    bwScreenGraph::bwNode* node =
        context.active ? context.active : findHoveredNode(event, screen_graph.Root());

    if (node)
    {
        bubbleEvent<bwMouseButtonEvent&>(
            event, *node, &bwScreenGraph::bwEventHandler::onMousePress, event);
    }
    drag_event.emplace(event.button, event.location);

    if (!context.active)
    {
        context.active = node;
    }
}

void bwEventDispatcher::dispatchMouseButtonRelease(bwMouseButtonEvent& event)
{
    if (context.active)
    {
        bubbleEvent<bwMouseButtonEvent&>(
            event, *context.active, &bwScreenGraph::bwEventHandler::onMouseRelease, event);

        if (!isDragging())
        {
            /* Even if the drag event was already sent, we may also need to send the click
             * event, so unswallow it for that purpose. */
            event.unswallow();

            bubbleEvent<bwMouseButtonEvent&>(
                event, *context.active, &bwScreenGraph::bwEventHandler::onMouseClick, event);
        }
    }

    drag_event = std::nullopt;
    context.active = nullptr;
}

void bwEventDispatcher::dispatchMouseWheelScroll(bwMouseWheelEvent& event)
{
    if (context.hovered)
    {
        bubbleEvent<bwMouseWheelEvent&>(
            event, *context.hovered, &bwScreenGraph::bwEventHandler::onMouseWheel, event);
    }
}

bool bwEventDispatcher::isDragging()
{
    return drag_event && (drag_event->drag_state == bwMouseButtonDragEvent::DRAGGING);
}

/**
 * Make \a new_hovered the new hovered widget, executing the onMouseEnter() and
 * onMouseLeave() listeners as needed.
 */
void bwEventDispatcher::changeContextHovered(bwScreenGraph::bwNode* new_hovered, bwEvent& event)
{
    bwScreenGraph::bwNode* old_hovered = context.hovered;

    if (new_hovered && (old_hovered == new_hovered))
    {
        return;
    }

    if (old_hovered)
    {
        bubbleEvent<bwEvent&>(
            event, *old_hovered, &bwScreenGraph::bwEventHandler::onMouseLeave, event);
    }

    if (new_hovered)
    {
        bubbleEvent<bwEvent&>(
            event, *new_hovered, &bwScreenGraph::bwEventHandler::onMouseEnter, event);
    }

    context.hovered = new_hovered;
}

}  // namespace bWidgets
