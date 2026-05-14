/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Original work Copyright (c) 2018 Julian Eisel
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#include <cassert>
#include <string>

#include "bwColor.h"

#include "katana.h"

#include "bwPropertyParser.h"

namespace bWidgets
{

std::unique_ptr<bwPropertyParser> bwPropertyParser::newFromPropertyType(
    bwStyleProperty::Type type)
{
    switch (type)
    {
    case bwStyleProperty::Type::BOOL:
        return std::make_unique<bwBooleanPropertyParser>();
    case bwStyleProperty::Type::INTEGER:
        return std::make_unique<bwIntegerPropertyParser>();
    case bwStyleProperty::Type::FLOAT:
        return std::make_unique<bwFloatPropertyParser>();
    case bwStyleProperty::Type::COLOR:
        return std::make_unique<bwColorPropertyParser>();
    default:
        return (assert(0), nullptr);
    }
}

void bwBooleanPropertyParser::parseIntoProperty(bwStyleProperty& dest_property,
                                                const KatanaValue& value) const
{
    const std::string ident_value{ value.string };

    if (ident_value == "true")
    {
        dest_property.setValue(true);
    }
    else if (ident_value == "false")
    {
        dest_property.setValue(false);
    }
    else
    {
        throw bwPropertyParsingFailure();
    }
}

void bwIntegerPropertyParser::parseIntoProperty(bwStyleProperty& dest_property,
                                                const KatanaValue& value) const
{
    dest_property.setValue((int)value.fValue);  // iValue is not valid
}

void bwFloatPropertyParser::parseIntoProperty(bwStyleProperty& dest_property,
                                              const KatanaValue& value) const
{
    dest_property.setValue((float)value.fValue);
}

bool bwColorPropertyParser::canParseFunction(const std::string& function_name) const
{
    return (function_name == "rgb(") || (function_name == "rgba(");
}

bwColor bwColorPropertyParser::parseFromFunction(const KatanaValue& value) const
{
    if (!canParseFunction(value.function->name))
    {
        throw bwPropertyParsingFailure();
    }

    {
        bwColor color;

        for (unsigned int i = 0, color_index = 0; i < value.function->args->length; i++)
        {
            auto* arg_value = (KatanaValue*)value.function->args->data[i];

            if (arg_value->unit == KATANA_VALUE_PARSER_OPERATOR)
            {
                continue;
            }
            assert(arg_value->unit == KATANA_VALUE_NUMBER);
            color[color_index++] = float(arg_value->fValue / 255.0f);
        }

        return color;
    }
}

void bwColorPropertyParser::parseIntoProperty(bwStyleProperty& dest_property,
                                              const KatanaValue& value) const
{
    switch (value.unit)
    {
    case KATANA_VALUE_PARSER_FUNCTION:
        dest_property.setValue(parseFromFunction(value));
        break;
    default:
        throw bwPropertyParsingFailure();
    }
}

}  // namespace bWidgets
