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

#include "blender_icon_defines.h"

#include "DefaultStageRNAFunctor.h"
#include "screen/bwIcon.h"
#include "screen/bwLayout.h"
#include "rna/RNAScreenGraphBuilder.h"

#include "builtin_widgets.h"
#include "bwStyleManager.h"
#include "screen_graph/bwBuilder.h"
#include "screen_graph/bwIterators.h"

#include "DefaultStage.h"

using namespace bWidgets;  // Less verbose

namespace bWidgetsDemo
{
#define PANEL_HEADER_HEIGHT 24

	DefaultStage::DefaultStage(uint32_t mask_width, uint32_t mask_height)
		: Stage(mask_width, mask_height)
	{
	}

	bool isUseCSSVersionToggleHidden(const bwStyle& style)
	{
		return false;
	}

	void DefaultStage::addStyleSelector(bwScreenGraph::bwLayoutNode& parent_node)
	{
	}

	void DefaultStage::useStyleCSSVersionSet(const bool use_css_version)
	{
	}

	bool DefaultStage::updateStyleButton(bwWidget& widget_iter)
	{
		return true;
	}

	void DefaultStage::updateStyleButtons()
	{
		// Deactivate style radio buttons that are not active. In future this should be handled within
		// bWidgets somehow (groups of radio buttons and a value getter maybe?).
		for (bwScreenGraph::bwNode& node : *screen_graph)
		{
			if (!node.isVisible())
			{
				continue;
			}
			if (bwWidget* widget = node.Widget())
			{
				updateStyleButton(*widget);
			}
		}
	}

	void DefaultStage::activateStyleID(bwStyle::Type type_id)
	{
	}

	void DefaultStage::updateFontAAMode(bool value)
	{
		for (bwScreenGraph::bwNode& node : screen_graph->Root())
		{
			bwWidget* widget = node.Widget();
			if (!widget)
			{
				continue;
			}
			if (auto* iter_checkbox = widget_cast<bwCheckbox>(widget))
			{
				if (iter_checkbox->apply_functor)
				{
					const auto* rna_functor = dynamic_cast<DefaultStageRNAFunctor*>(iter_checkbox->apply_functor.get());
					if (rna_functor && (rna_functor->getPropName() == "font_use_subpixel_positioning"))
					{
						iter_checkbox->hide(!value);
					}
				}
			}
		}
	}

	void DefaultStage::buildWidgets()
	{
		using namespace bwScreenGraph;
		/* Convenience */
		using RNABuilder = RNAScreenGraphBuilder<DefaultStage, DefaultStageRNAFunctor>;

		/* Build the scrollable content region (below the menu bar). */
		RNABuilder builder(*screen_graph, *this, properties);

		registerProperties(properties);

		addStyleSelector(screen_graph->Root());

		builder.addRNAWidget<bwNumberSlider>("interface_scale")
			.setMinMax(0.5f, 2.0f)
			.setValue(1.0f)
			.setText("Interface Scale: ");

		builder.addWidget<bwLabel>("Font Rendering:");

		builder.buildLayout<bwRowLayout, RNABuilder>([](RNABuilder& builder) {
			builder.addRNAWidget<bwCheckbox>("font_use_tight_positioning")
				.setLabel("Tight Positioning")
				.setState(bwWidget::State::SUNKEN);
			builder.addRNAWidget<bwCheckbox>("font_use_hinting").setLabel("Hinting");
			});

		builder.buildLayout<bwRowLayout, RNABuilder>([](RNABuilder& builder) {
			builder.addRNAWidget<bwCheckbox>("font_use_subpixels").setLabel("Subpixel Rendering");
			builder.addRNAWidget<bwCheckbox>("font_use_subpixel_positioning")
				.setLabel("Subpixel Positioning")
				.hide();
			});

#if 0
		bwBuilder testBuilder(screen_graph);
		testBuilder.buildLayout<bwRowLayout>([](bwBuilder& builder) {
			auto& use_subpixel = builder.addWidget<bwCheckbox>();
			auto& use_subpixel_positioning = builder.addWidget<bwCheckbox>()
				.setLabel("Subpixel Positioning")
				.hide();

			use_subpixel.createApplyFunctor<bwFunctorLambda>([&use_subpixel, &use_subpixel_positioning]() {
				use_subpixel_positioning.hide(!use_subpixel.isChecked());
				});

			use_subpixel.createApplyFunctor2<bwFunctorLambda2<bwCheckbox>>([&use_subpixel_positioning](bwCheckbox& checkbox) {
				use_subpixel_positioning.hide(!checkbox.isChecked());
				});

			use_subpixel.createApplyFunctor3<bwFunctorLambda3<bool>>(
				[&use_subpixel]() { return use_subpixel.isChecked(); },
				[&use_subpixel_positioning](bool isChecked) { use_subpixel_positioning.hide(!isChecked); }
			);
			});
#endif

		builder.buildContainer<bwPanel, RNABuilder>(
			[](RNABuilder& builder) {
				builder.buildLayout<bwColumnLayout>(
					[](bwBuilder& builder) {
						builder.addWidget<bwPushButton>("Translate");
						builder.addWidget<bwPushButton>("Rotate");
						builder.addWidget<bwPushButton>("Scale");
					},
					true);

				builder.addWidget<bwPushButton>("Mirror").setIcon(icon_map->getIcon(ICON_MOD_MIRROR));
			},
			std::make_unique<bwPanelLayout>(),
			"Some Testing Widgets",
			PANEL_HEADER_HEIGHT);

		builder.buildContainer<bwPanel>(
			[](bwBuilder& builder) {
				builder.buildLayout<bwRowLayout>([](bwBuilder& builder) {
					builder.addWidget<bwCheckbox>("Make Awesome");
					builder.addWidget<bwCheckbox>("Wireframes");
					});

				builder.addWidget<bwTextBox>().setText("Some Text...");

				builder.buildLayout<bwRowLayout>([](bwBuilder& builder) {
					builder.addWidget<bwLabel>()
						.setLabel("Pose Icon")
						.setIcon(icon_map->getIcon(ICON_POSE_HLT));
					builder.addWidget<bwLabel>()
						.setLabel("Normalized FCurve Icon")
						.setIcon(icon_map->getIcon(ICON_NORMALIZE_FCURVES));
					builder.addWidget<bwLabel>()
						.setLabel("Chroma Scope Icon")
						.setIcon(icon_map->getIcon(ICON_SEQ_CHROMA_SCOPE));
					});
			},
			std::make_unique<bwPanelLayout>(),
			"More Testing...",
			PANEL_HEADER_HEIGHT);
	}

	void DefaultStage::registerProperties(bWidgets::RNAProperties<DefaultStage>& properties)
	{
	}

}  // namespace bWidgetsDemo
