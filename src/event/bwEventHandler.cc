#include <assert.h>
#include <iostream>

#include "event/bwEventHandler.h"
#include "screen_graph/bwNode.h"

namespace bWidgets
{
namespace bwScreenGraph
{

void bwEventHandler::addEventListener(bwEventHandler::EventType event_type, EventListener listener)
{
    assert(event_type < TOT_EVENT_TYPES);
    listeners[event_type].push_back(listener);
}

void bwEventHandler::onMouseMove(bwEvent&)
{
}

void bwEventHandler::onMouseEnter(bwEvent&)
{
}

void bwEventHandler::onMouseLeave(bwEvent&)
{
}

void bwEventHandler::onMousePress(bwMouseButtonEvent&)
{
}

void bwEventHandler::onMouseRelease(bwMouseButtonEvent&)
{
}

void bwEventHandler::onMouseClick(bwMouseButtonEvent&)
{
}

void bwEventHandler::onMouseDrag(bwMouseButtonDragEvent&)
{
}

void bwEventHandler::onMouseWheel(bwMouseWheelEvent&)
{
}

}  // namespace bwScreenGraph
}  // namespace bWidgets
