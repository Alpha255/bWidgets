#pragma once

#include <vector>

namespace bWidgets
{

/* TODO for (2D-)polygons, we should actually use ints, not floats. Prevents
 * precision and rounding errors. */
using bwPointVec = std::vector<class bwPoint>;

class bwPolygon
{
public:
    bwPolygon() = default;
    bwPolygon(const bwPolygon& poly) = default;
    bwPolygon(const bwPointVec& vertices);
    explicit bwPolygon(const uint32_t reserve_vertex_count);

    void addVertex(class bwPoint vertex);
    void addVertex(const float x, const float y);
    void addVertex(const int32_t x, const int32_t y);
    void reserve(const uint32_t count);
    const bwPointVec& getVertices() const;

    bwPoint& operator[](const uint32_t index);

    bool isDrawable() const;

protected:
    bwPointVec vertices{};
    size_t vert_count{ 0 };
};

}  // namespace bWidgets
