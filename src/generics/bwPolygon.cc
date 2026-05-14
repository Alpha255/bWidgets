#include <cmath>
#include <iostream>

#include "bwPoint.h"

#include "bwPolygon.h"

namespace bWidgets
{

bwPolygon::bwPolygon(const bwPointVec& vertices) : vertices(vertices), vert_count(vertices.size())
{
}

bwPolygon::bwPolygon(const uint32_t reserve_vertex_count)
{
    vertices.reserve(reserve_vertex_count);
}

void bwPolygon::addVertex(bwPoint vertex)
{
    vertices.push_back(vertex);
    vert_count++;
}

void bwPolygon::addVertex(const float x, const float y)
{
    addVertex(bwPoint(x, y));
}

void bwPolygon::addVertex(const int32_t x, const int32_t y)
{
    addVertex({ std::roundf(x), std::roundf(y) });
}

void bwPolygon::reserve(const uint32_t count)
{
    vertices.reserve(count);
}

const bwPointVec& bwPolygon::getVertices() const
{
    return vertices;
}

bwPoint& bwPolygon::operator[](const uint32_t index)
{
    return vertices[index];
}

bool bwPolygon::isDrawable() const
{
    return (vert_count > 0);
}

}  // namespace bWidgets
