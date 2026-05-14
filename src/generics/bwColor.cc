#include <cassert>
#include <cmath>

#include "bwRange.h"

#include "bwColor.h"

namespace bWidgets
{

const bwColor bwColor::White = bwColor(1.0f, 1.0f, 1.0f, 1.0f);

bwColor::bwColor(float red, float green, float blue, float alpha)
{
    setColor(red, green, blue, alpha);
}
bwColor::bwColor(float rgb, float alpha)
{
    setColor(rgb, alpha);
}
bwColor::bwColor(uint32_t red, uint32_t green, uint32_t blue, uint32_t alpha)
{
    assert(red < 256);
    assert(green < 256);
    assert(blue < 256);
    assert(alpha < 256);

    setColor(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
}
bwColor::bwColor(uint32_t rgb, uint32_t alpha)
{
    assert(rgb < 256);
    assert(alpha < 256);

    setColor(rgb / 255.0f, alpha / 255.0f);
}

bwColor::bwColor(const bwColor& other)
{
    setColor(other.rgba);
}

bwColor& bwColor::shade(const float rgb_shade, float alpha_shade)
{
    (*this)[Component::RED] += rgb_shade;
    clamp(Component::RED);
    (*this)[Component::GREEN] += rgb_shade;
    clamp(Component::GREEN);
    (*this)[Component::BLUE] += rgb_shade;
    clamp(Component::BLUE);
    (*this)[Component::ALPHA] += alpha_shade;

    return *this;
}
bwColor& bwColor::shade(uint32_t rgb_shade, uint32_t alpha_shade)
{
    (*this)[Component::RED] += rgb_shade / 255.0f;
    clamp(Component::RED);
    (*this)[Component::GREEN] += rgb_shade / 255.0f;
    clamp(Component::GREEN);
    (*this)[Component::BLUE] += rgb_shade / 255.0f;
    clamp(Component::BLUE);
    (*this)[Component::ALPHA] += alpha_shade / 255.0f;
    clamp(Component::ALPHA);

    return *this;
}

void bwColor::setColor(float red, float green, float blue, float alpha)
{
    const float rgba[4] = { red, green, blue, alpha };
    setColor(rgba);
}

void bwColor::setColor(float rgb, float alpha)
{
    const float rgba[4] = { rgb, rgb, rgb, alpha };
    setColor(rgba);
}

void bwColor::setColor(const float _rgba[4])
{
    rgba[0] = _rgba[0];
    rgba[1] = _rgba[1];
    rgba[2] = _rgba[2];
    rgba[3] = _rgba[3];
}

const float* bwColor::getColor() const
{
    return &rgba[0];
}

bwColor& bwColor::operator=(const float* rgb)
{
    setColor(rgb);
    return *this;
}
bwColor& bwColor::operator=(const bwColor& other_color)
{
    setColor(other_color.rgba);
    return *this;
}

auto bwColor::operator==(const bwColor& compare_color) const -> bool
{
    return ((rgba[0] == compare_color[0]) && (rgba[1] == compare_color[1]) &&
            (rgba[2] == compare_color[2]) && (rgba[3] == compare_color[3]));
}

float& bwColor::operator[](const int32_t index)
{
    assert((index >= 0) && (index < 4));
    return rgba[index];
}

void bwColor::clamp(const Component component)
{
    bwRange<float>::clampValue((*this)[component], 0.0f, 1.0f);
}

bwColor::operator const float*() const
{
    return getColor();
}

}  // namespace bWidgets
