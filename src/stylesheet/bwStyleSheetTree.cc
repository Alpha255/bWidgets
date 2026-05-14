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

#include "bwStyleSheetTree.h"

namespace bWidgets
{

class bwStateProperties
{
public:
    const bwStyleProperty* lookupProperty(const std::string_view& identifier) const;
    bwStyleProperty& ensureProperty(const std::string_view& identifier,
                                    bwStyleProperty::Type type);

private:
    bwStyleProperties properties;
};

class bwStyleSheetNode
{
public:
    class bwStateProperties state_properties[int32_t(bwWidget::State::STATE_TOT)];
};

bwStyleSheetNode* bwStyleSheetTree::lookupNode(const std::string_view& name) const
{
    const auto& node_iterator = std::find_if(
        nodes.begin(), nodes.end(), [&name](auto& key_value_pair) {
            return key_value_pair.first == name;
        });
    if (node_iterator == nodes.end())
    {
        return nullptr;
    }

    return node_iterator->second;
}

bwStyleSheetNode& bwStyleSheetTree::ensureNode(const std::string_view& class_name)
{
    if (bwStyleSheetNode* node = lookupNode(class_name))
    {
        return *node;
    }

    bwStyleSheetNode* new_node = new bwStyleSheetNode;
    nodes.insert({ std::string(class_name), new_node });
    return *new_node;
}

bwStyleSheetTree::~bwStyleSheetTree()
{
    while (!nodes.empty())
    {
        auto iterator = nodes.begin();
        bwStyleSheetNode* node = iterator->second;

        nodes.erase(iterator);
        delete node;
    }
}

bwStyleProperty& bwStyleSheetTree::ensureNodeWithProperty(const std::string_view& class_name,
                                                          const bwWidget::State pseudo_state,
                                                          const std::string_view& identifier,
                                                          const bwStyleProperty::Type type)
{
    bwStyleSheetNode& node = ensureNode(class_name);
    bwStateProperties& state_properties = node.state_properties[int32_t(pseudo_state)];

    return state_properties.ensureProperty(identifier, type);
}

static const bwStyleProperty* state_properties_lookup_property(
    const std::string_view& property_name, bwStateProperties& state_properties)
{
    return state_properties.lookupProperty(property_name);
}

const bwStyleProperty* bwStyleSheetTree::resolveProperty(const std::string_view& class_name,
                                                         const std::string_view& property_name,
                                                         const bwWidget::State state)
{
    if (bwStyleSheetNode* node = lookupNode(class_name))
    {
        const bwStyleProperty* property = state_properties_lookup_property(
            property_name, node->state_properties[int32_t(state)]);

        if (!property && (state != bwWidget::State::NORMAL))
        {
            // Property for this state not set, check for STATE_NORMAL.
            property = state_properties_lookup_property(
                property_name, node->state_properties[int32_t(bwWidget::State::NORMAL)]);
        }

        return property;
    }

    return nullptr;
}

const bwStyleProperty* bwStateProperties::lookupProperty(const std::string_view& identifier) const
{
    return properties.lookup(identifier);
}

/**
 * Performs a identifier based lookup of \a property and adds it if not found.
 */
bwStyleProperty& bwStateProperties::ensureProperty(const std::string_view& identifier,
                                                   bwStyleProperty::Type type)
{
    for (auto& iter_property : properties)
    {
        if (iter_property->getIdentifier() == identifier)
        {
            return *iter_property;
        }
    }

    return properties.addProperty(identifier, type);
}

}  // namespace bWidgets
