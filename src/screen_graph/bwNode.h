#pragma once

#include <list>
#include <assert.h>

#include "bwContainerWidget.h"
#include "screen/bwLayoutInterface.h"
#include "bwWidget.h"

namespace bWidgets
{
	namespace bwScreenGraph
	{
		class bwEventHandler;

		/**
		 * \brief The base data-structure for a screen-graph node
		 *
		 * Screen-graph nodes are the atomical components of a screen-graph, which is
		 * key for the bWidgets design. If you're not familiar with our screen-graph
		 * concept, you should really have a look at the
		 * [bWidgets design overview](md_docs_bWidgets_design_overview.html).
		 *
		 * There are three kinds of screen-graph elements/nodes:
		 * * Widget nodes (node representing a simple widget)
		 * * Layout nodes (node with children aligned in specified layout)
		 * * Container nodes (combination of both, widget with children aligned in
		 *   layout - i.e. panel)
		 *
		 * To ensure separation of concerns, the screen-graph node must not have any
		 * functionality of its own (no non-trivial member functions). Any functionality
		 * must be implemented using friend classes and functions. For example to build
		 * a screen-graph and the necessary nodes, a screen-graph builder class should
		 * be used and be declared as friend of the `Node` class.
		 * Having to declare those helpers as friends may turn out to an annoyance
		 * with small benefits. In that case we should just make data public.
		 */

		class bwNode
		{
			friend class bwBuilder;

		public:
			using ChildList = std::list<std::unique_ptr<bwNode>>;
			using ChildIterator = ChildList::iterator;

			bwNode() = default;
			virtual ~bwNode() = default;

			virtual const ChildList* Children() const
			{
				return nullptr;
			}
			virtual ChildList* Children()
			{
				return nullptr;
			}

			virtual bool childrenVisible() const
			{
				return true;
			}

			virtual bwLayoutInterface* Layout() const
			{
				return nullptr;
			}

			virtual bwWidget* Widget() const
			{
				return nullptr;
			}

			bwNode* Parent() const
			{
				return parent;
			}

			bwEventHandler* eventHandler() const
			{
				return handler.get();
			}

			virtual bwRectanglePixel Rectangle() const = 0;
			virtual std::optional<bwRectanglePixel> MaskRectangle() const = 0;
			virtual bool isVisible() const = 0;

		protected:
			bwNode* parent{ nullptr };
			std::unique_ptr<bwEventHandler> handler{ nullptr };
		};

		/**
		 * \brief Node for aligning children to a specific layout.
		 */
		class bwLayoutNode : virtual public bwNode
		{
			friend class bwBuilder;

		public:
			const ChildList* Children() const override
			{
				return &children;
			}
			ChildList* Children() override
			{
				return &children;
			}

			bwLayoutInterface* Layout() const override
			{
				return layout.get();
			}

			bwRectanglePixel Rectangle() const override
			{
				assert(layout);
				return layout->getRectangle();
			}

			std::optional<bwRectanglePixel> MaskRectangle() const override
			{
				return std::nullopt;
			}

			bool isVisible() const override
			{
				return true;
			}

			void setLayout(std::unique_ptr<bwLayoutInterface> inLayout)
			{
				layout = std::move(inLayout);
			}

			template<class Layout, typename... Args>
			Layout& createLayout(Args&&... args)
			{
				static_assert(std::is_base_of<bwLayoutInterface, Layout>::value, "should be a valid layout type");

				assert(layout == nullptr);
				layout = std::make_unique<Layout>(std::forward<Args>(args)...);
				return static_cast<Layout&>(*layout);
			}

		private:
			std::unique_ptr<bwLayoutInterface> layout;
			ChildList children;
		};

		/**
		 * \brief Node representing a single widget with no children.
		 */
		class bwWidgetNode : virtual public bwNode
		{
			friend class bwBuilder;

		public:
			bwWidget* Widget() const override
			{
				assert(widget);
				return &*widget;
			}

			bwRectanglePixel Rectangle() const override
			{
				assert(widget);
				return widget->rectangle;
			}

			std::optional<bwRectanglePixel> MaskRectangle() const override
			{
				return std::nullopt;
			}

			bool isVisible() const override
			{
				assert(widget);
				return widget->isHidden() == false;
			}

			void setWidget(std::unique_ptr<bwWidget> inWidget)
			{
				widget = std::move(inWidget);
				handler = widget->createHandler();
			}

			template<class Widget, typename... Args>
			Widget& createWidget(Args&&... args)
			{
				static_assert(std::is_base_of<bwWidget, Widget>::value, "should be a valid widget type");

				assert(widget == nullptr);
				widget = std::make_unique<Widget>(std::forward<Args>(args)...);
				handler = widget->createHandler();
				return static_cast<Widget&>(*widget);
			}
		private:
			std::unique_ptr<bwWidget> widget;
		};

		/**
		 * \brief Node representing a widget with children.
		 *
		 * Note virtual inheritance of LayoutNode and WidgetNode, required to solve
		 * diamond problems.
		 */
		class bwContainerNode : public bwLayoutNode, public bwWidgetNode
		{
		public:
			const ChildList* Children() const override
			{
				return bwLayoutNode::Children();
			}
			ChildList* Children() override
			{
				return bwLayoutNode::Children();
			}

			bwLayoutInterface* Layout() const override
			{
				return bwLayoutNode::Layout();
			}

			bwWidget* Widget() const override
			{
				return bwWidgetNode::Widget();
			}

			bwContainerWidget& ContainerWidget() const
			{
				return static_cast<bwContainerWidget&>(*Widget());
			}

			bwRectanglePixel Rectangle() const override
			{
				return bwWidgetNode::Rectangle();
			}
			bwRectanglePixel ContentRectangle() const
			{
				return bwLayoutNode::Rectangle();
			}

			std::optional<bwRectanglePixel> MaskRectangle() const override
			{
				return ContainerWidget().getMaskRectangle();
			}

			bool isVisible() const override
			{
				return bwWidgetNode::isVisible();
			}

			bool childrenVisible() const override
			{
				return ContainerWidget().childrenVisible();
			}
		};

	}  // namespace bwScreenGraph
}  // namespace bWidgets
