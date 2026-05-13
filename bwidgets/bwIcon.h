#pragma once

#include "bwIconInterface.h"

namespace bWidgets
{
    class bwIcon : public bwIconInterface
    {
    public:
        enum class Type
        {
            PREVIEW,
            SVG_COLOR,
            SVG_MONO,
            BUFFER,
            IMAGE_BUFFER,
            VECTOR,
            GEOMETRY,
            EVENT,
            GPLAYER,
            BLANK
        };
    };
}
