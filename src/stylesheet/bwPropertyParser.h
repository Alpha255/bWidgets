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

#pragma once

#include "bwStyleProperties.h"

struct KatanaValue;

namespace bWidgets
{
	class bwPropertyParsingFailure : public std::exception
	{
	public:
		const char* what() const noexcept override
		{
			return "could not parse value for a property";
		}
	};

	class bwPropertyParser
	{
	public:
		virtual ~bwPropertyParser() = default;

		static std::unique_ptr<bwPropertyParser> newFromPropertyType(bwStyleProperty::Type);

		virtual void parseIntoProperty(bwStyleProperty&, const KatanaValue&) const = 0;
	};

	class bwBooleanPropertyParser : public bwPropertyParser
	{
	public:
		void parseIntoProperty(bwStyleProperty&, const KatanaValue&) const override;
	};

	class bwIntegerPropertyParser : public bwPropertyParser
	{
	public:
		void parseIntoProperty(bwStyleProperty&, const KatanaValue&) const override;
	};

	class bwFloatPropertyParser : public bwPropertyParser
	{
	public:
		void parseIntoProperty(bwStyleProperty&, const KatanaValue&) const override;
	};

	class bwColorPropertyParser : public bwPropertyParser
	{
	public:
		void parseIntoProperty(bwStyleProperty&, const KatanaValue&) const override;

	private:
		bwColor parseFromFunction(const KatanaValue&) const;
		bool canParseFunction(const std::string&) const;
	};

}  // namespace bWidgets
