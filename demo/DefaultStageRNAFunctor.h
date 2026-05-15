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

#include <optional>

#include "bwFunctorInterface.h"

#include "rna/RNAProperty.h"

namespace bWidgets
{
	class bwWidget;
}

namespace bWidgetsDemo
{
	class DefaultStage;

	class DefaultStageRNAFunctor : public bWidgets::bwFunctorInterface
	{
	public:
		DefaultStageRNAFunctor(bWidgets::RNAProperties<DefaultStage>& props,
			DefaultStage& stage,
			const std::string& prop_name,
			const bWidgets::bwWidget& widget);
		DefaultStageRNAFunctor(bWidgets::RNAProperties<DefaultStage>& props,
			DefaultStage& stage,
			const std::string& prop_name,
			const bWidgets::bwWidget& widget,
			int32_t enum_value);

		void operator()() override;

		const std::string& getPropName() const;
		std::optional<int32_t> getEnumValue() const;

	private:
		bWidgets::RNAProperties<DefaultStage>& m_props;
		DefaultStage& m_stage;
		std::string m_prop_name;
		const bWidgets::bwWidget& m_widget;
		std::optional<int32_t> m_enum_value;
	};

}  // namespace bWidgetsDemo
