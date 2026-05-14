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
#include <fstream>
#include <iostream>

#include "utils/File.h"
#include "katana.h"
#include "bwPropertyParser.h"
#include "bwStyleSheetTree.h"

#include "stylesheet/bwStyleSheet.h"

namespace bWidgets
{

bwStyleSheet::bwStyleSheet(std::string filepath) : filepath(std::move(filepath))
{
    load();
}

bwStyleSheet::~bwStyleSheet()
{
    unload();
}

// TODO Right now the type of properties in the style sheet is decided based on
// the value they have set. E.g. "border-width: rgba(...)" would add a bwColor
// property to the tree. Instead we should have a global map of property types
// with the property identifier as key. Then we can check if the CSS rule is
// valid for the property.

static bwStyleProperty::Type stylesheet_property_type_get_from_katana(const KatanaValue& value)
{
    switch (value.unit)
    {
    case KATANA_VALUE_PARSER_FUNCTION: {
        std::string function_name{ value.function->name };
        if ((function_name == "rgb(") || (function_name == "rgba("))
        {
            return bwStyleProperty::Type::COLOR;
        }
        break;
    }
    case KATANA_VALUE_PX:
        return bwStyleProperty::Type::FLOAT;
    case KATANA_VALUE_IDENT: {
        // Customization to support booleans in CSS.
        const std::string ident_value{ value.string };
        assert(ident_value == "true" || ident_value == "false");
        return bwStyleProperty::Type::BOOL;
    }
    default:
        return bwStyleProperty::Type::INTEGER;
    }

    return bwStyleProperty::Type::INTEGER;
}

static void stylesheet_set_value_from_katana_value(bwStyleProperty& property,
                                                   const KatanaValue& value)
{
    std::unique_ptr<bwPropertyParser> parser(
        bwPropertyParser::newFromPropertyType(property.getType()));
    parser->parseIntoProperty(property, value);
}

static bwWidget::State stylesheet_property_state_from_katana(const KatanaSelector& selector)
{
    KatanaPseudoType pseudo_type = selector.tagHistory ? selector.tagHistory->pseudo :
                                                         KatanaPseudoEmpty;

    switch (pseudo_type)
    {
    case KatanaPseudoHover:
        return bwWidget::State::HIGHLIGHTED;
    case KatanaPseudoActive:
        return bwWidget::State::SUNKEN;
    case KatanaPseudoEmpty:
    default:
        return bwWidget::State::NORMAL;
    }
}

static void stylesheet_tree_property_ensure_from_katana(bwStyleSheetTree& tree,
                                                        const KatanaSelector& selector,
                                                        const KatanaDeclaration& declaration,
                                                        const KatanaValue& value)
{
    bwStyleProperty& property = tree.ensureNodeWithProperty(
        selector.tag->local,
        stylesheet_property_state_from_katana(selector),
        declaration.property,
        stylesheet_property_type_get_from_katana(value));
    stylesheet_set_value_from_katana_value(property, value);
}

static void stylesheet_tree_node_fill_from_katana(bwStyleSheetTree& tree,
                                                  const KatanaStyleRule& rule,
                                                  const KatanaSelector& selector)
{
    for (uint32_t declaration_idx = 0; declaration_idx < rule.declarations->length;
         declaration_idx++)
    {
        auto* declaration = (KatanaDeclaration*)rule.declarations->data[declaration_idx];

        for (uint32_t value_idx = 0; value_idx < declaration->values->length; value_idx++)
        {
            auto* value = (KatanaValue*)declaration->values->data[value_idx];
            stylesheet_tree_property_ensure_from_katana(tree, selector, *declaration, *value);
        }
    }
}

static void stylesheet_tree_fill_from_katana(bwStyleSheetTree& tree,
                                             const KatanaOutput& katana_output)
{
    for (uint32_t rule_idx = 0; rule_idx < katana_output.stylesheet->rules.length; rule_idx++)
    {
        const auto* rule = (KatanaStyleRule*)katana_output.stylesheet->rules.data[rule_idx];

        for (uint32_t selector_idx = 0; selector_idx < rule->selectors->length; selector_idx++)
        {
            auto* selector = (KatanaSelector*)rule->selectors->data[selector_idx];
            stylesheet_tree_node_fill_from_katana(tree, *rule, *selector);
        }
    }
}

void bwStyleSheet::load()
{
    File file{ filepath };
    std::string file_contents = file.readIntoString();
    KatanaOutput* katana_output = katana_parse(
        file_contents.c_str(), file_contents.length(), KatanaParserModeStylesheet);

    tree = std::make_unique<bwStyleSheetTree>();
    stylesheet_tree_fill_from_katana(*tree, *katana_output);
    katana_destroy_output(katana_output);
}

void bwStyleSheet::unload()
{
    // Nothing right now.
}

void bwStyleSheet::reload()
{
    unload();
    load();
}

void bwStyleSheet::resolveValue(const std::string_view& class_name,
                                const bwWidget::State state,
                                bwStyleProperty& property)
{
    const bwStyleProperty* property_from_tree = tree->resolveProperty(
        class_name, property.getIdentifier(), state);

    if (property_from_tree)
    {
        property.setValue(*property_from_tree);
    }
    else
    {
        property.setValueToDefault();
        //		std::cout << filepath << std::endl;
        //		std::cout << "CSS resolving error: Failed to find property \"" <<
        // property.getIdentifier() <<
        //		             "\" for " << class_name << "." << std::endl << std::endl;
    }
}

const std::string& bwStyleSheet::getFilepath() const
{
    return filepath;
}

}  // namespace bWidgets
