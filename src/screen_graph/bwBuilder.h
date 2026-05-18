#pragma once

#include <type_traits>
#include <assert.h>

#include "bwNode.h"
#include "bwScreenGraph.h"
#include "bwWidget.h"

namespace bWidgets
{
	namespace bwScreenGraph
	{
		/**
		 * \brief Helper class to construct screen-graphs.
		 *
		 * Use this to create all screen-graphs or sub-trees.
		 *
		 * * `addFoo` functions add a node and constructs its item (widget or layout) in-place. That means
		 *   arguments are passed to the function are forwarded to the widget or layout constructor.
		 * * `buildFoo` functions do the same but allow specifying a build-function for sub-layouts. This
		 *   is the prefered way to construct sub-layouts, because the builder manages all state. For the
		 *   caller it's a stateless way to build.
		 */

		class bwBuilder
		{
		public:
			/**
			 * \brief A callable type to build a layout.
			 *
			 * `buildFoo` functions use this to receive a callback for building their sub-layout.
			 * Can bind to a plain old function pointer, a function object (functor) or a lambda.
			 */

			template<typename _BuilderType = bwBuilder> 
			using BuildFunc = std::function<void(_BuilderType&)>;

			bwBuilder(bwLayoutNode& active_layout_node);
			bwBuilder(bwScreenGraph& screen_graph);
			virtual ~bwBuilder() = default;

			static void setLayout(bwLayoutNode& node, std::unique_ptr<bwLayoutInterface> layout);
			static void setWidget(bwWidgetNode& node, std::unique_ptr<bwWidget> widget);
			static bwWidget& addWidget(bwLayoutNode& node, std::unique_ptr<bwWidget> widget);

			void setActiveLayout(bwLayoutNode&);

			/**
			 * \brief Add and activate a child node for a layout created in-place.
			 */
			template<typename _LayoutType, typename... _Args>
			bwLayoutNode& addLayout(_Args&&... __args)
			{
				static_assert(std::is_base_of<bwLayoutInterface, _LayoutType>::value, "Should implement bwLayoutInterface");

				bwLayoutNode& new_node = addChildNode<bwLayoutNode>(_active_layout_node);
				new_node.layout = std::make_unique<_LayoutType>(std::forward<_Args>(__args)...);
				setActiveLayout(new_node);
				return new_node;
			}

			/**
			 * \brief Build a sub-layout node in-place using \a build_func.
			 *
			 * This override passes the default builder type (\ref bwBuilder) to the
			 * \a build_func.
			 */

			template<typename _LayoutType, typename... _Args>
			bwLayoutNode& buildLayout(BuildFunc<> build_func, _Args&&... __args)
			{
				return buildLayout<_LayoutType, bwBuilder>(build_func, std::forward<_Args>(__args)...);
			}

			/**
			 * \brief Build a sub-layout node in-place using \a build_func.
			 *
			 * This override passes a custom builder type to the \a build_func (must inherit from \ref
			 * bwBuilder).
			 */

			template<typename _LayoutType, typename _BuilderType, typename... _Args>
			bwLayoutNode& buildLayout(BuildFunc<_BuilderType> build_func, _Args&&... __args)
			{
				static_assert(std::is_base_of_v<bwLayoutInterface, _LayoutType>, "Should implement bwLayoutInterface");
				static_assert(std::is_base_of_v<bwBuilder, _BuilderType>, "Should inherit from bwBuilder");

				bwLayoutNode& new_node = addChildNode<bwLayoutNode>(_active_layout_node);
				new_node.layout = std::make_unique<_LayoutType>(std::forward<_Args>(__args)...);
				buildChildren(build_func, new_node);

				return new_node;
			}

			template<typename _WidgetType, typename... _Args>
			_WidgetType& addWidget(_Args&&... __args)
			{
				static_assert(std::is_base_of<bwWidget, _WidgetType>::value, "Should derrive from bwWidget");

				bwWidgetNode& new_node = addChildNode<bwWidgetNode>(_active_layout_node);
				new_node.widget = std::make_unique<_WidgetType>(std::forward<_Args>(__args)...);
				new_node.handler = new_node.widget->createHandler();
				return static_cast<_WidgetType&>(*new_node.widget);
			}

			template<typename _WidgetType, typename... _Args>
			bwContainerNode& addContainer(std::unique_ptr<bwLayoutInterface> layout, _Args&&... __args)
			{
				static_assert(std::is_base_of<bwWidget, _WidgetType>::value, "Should derrive from bwWidget");
				static_assert(std::is_base_of<bwContainerWidget, _WidgetType>::value, "Should derrive from bwContainerWidget");

				bwContainerNode& new_node = addChildNode<bwContainerNode>(_active_layout_node);

				setLayout(new_node, std::move(layout));

				new_node.widget = std::make_unique<_WidgetType>(new_node, std::forward<_Args>(__args)...);
				new_node.handler = new_node.widget->createHandler();

				setActiveLayout(new_node);
				return new_node;
			}

			/**
			 * \brief Build a container node in-place using \a build_func.
			 *
			 * This override passes the default builder type (\ref bwBuilder) to the \a build_func.
			 */

			template<typename _WidgetType, typename... _Args>
			bwContainerNode& buildContainer(BuildFunc<> build_func,
				std::unique_ptr<bwLayoutInterface> layout,
				_Args&&... __args)
			{
				return buildContainer<_WidgetType, bwBuilder>(build_func, std::move(layout), std::forward<_Args>(__args)...);
			}

			/**
			 * \brief Build a container node in-place using \a build_func.
			 *
			 * This override passes a custom builder type to the \a build_func (must inherit from \ref
			 * bwBuilder).
			 *
			 * Only creates widget in-place, layout still needs to be created by the
			 * caller. Otherwise, we'd have to do perfect forwarding of parameters to
			 * multiple constructors. This can be done (e.g. see
			 * https://cpptruths.blogspot.com/2012/06/perfect-forwarding-of-parameter-groups.html)
			 * but in the end, just letting the caller create the layout is less
			 * verbose and easier to understand.
			 *
			 * \param build_func: Callback to create the subtree for this container.
			 */

			template<typename _WidgetType, typename _BuilderType, typename... _Args>
			bwContainerNode& buildContainer(BuildFunc<_BuilderType> build_func,
				std::unique_ptr<bwLayoutInterface> layout,
				_Args&&... __args)
			{
				static_assert(std::is_base_of<bwWidget, _WidgetType>::value, "Should derrive from bwWidget");
				static_assert(std::is_base_of<bwContainerWidget, _WidgetType>::value, "Should derrive from bwContainerWidget");
				static_assert(std::is_base_of_v<bwBuilder, _BuilderType>, "Should inherit from bwBuilder");

				bwContainerNode& new_node = addChildNode<bwContainerNode>(_active_layout_node);
				setLayout(new_node, std::move(layout));
				new_node.widget = std::make_unique<_WidgetType>(new_node, std::forward<_Args>(__args)...);
				new_node.handler = new_node.widget->createHandler();
				buildChildren(build_func, new_node);

				return new_node;
			}

			template<typename _WidgetType, typename... _Args>
			bwContainerNode& buildRootContainer(BuildFunc<> build_func,
				std::unique_ptr<bwLayoutInterface> layout,
				_Args&&... __args)
			{
				return buildRootContainer<_WidgetType, bwBuilder>(build_func, std::move(layout), std::forward<_Args>(__args)...);
			}

			template<typename _WidgetType, typename _BuilderType, typename... _Args>
			bwContainerNode& buildRootContainer(BuildFunc<_BuilderType> build_func,
				std::unique_ptr<bwLayoutInterface> layout,
				_Args&&... __args)
			{
				static_assert(std::is_base_of<bwWidget, _WidgetType>::value, "Should derrive from bwWidget");
				static_assert(std::is_base_of<bwContainerWidget, _WidgetType>::value, "Should derrive from bwContainerWidget");
				static_assert(std::is_base_of_v<bwBuilder, _BuilderType>, "Should inherit from bwBuilder");
				//assert(std::is_base_of<bwContainerNode, decltype(_active_layout_node)>::value);

				bwContainerNode& layout_node = static_cast<bwContainerNode&>(_active_layout_node.get());
				setLayout(layout_node, std::move(layout));
				layout_node.widget = std::make_unique<_WidgetType>(layout_node, std::forward<_Args>(__args)...);
				layout_node.handler = layout_node.widget->createHandler();
				buildChildren(build_func, layout_node);

				return layout_node;
			}

			/**
			 * \brief Add child node for a widget created in-place.
			 *
			 * Simplifies creating and appending widgets. The arguments \a __args are
			 * forwarded to the widget constructor; a new node for it is added as child
			 * to \a node.
			 *
			 * Without `emplaceWidget()`:
			 * \code
			 * using bWidgets::bwScreenGraph;
			 *
			 * auto& widget = static_cast<bwLabel&>(Builder::addWidget<bwLabel>(
			 *                    screen_graph, std::make_unique<bwLabel>("Foo", 0, 10)));
			 * widget.foo();
			 * // ...
			 * \endcode
			 *
			 * With `emplaceWidget()`:
			 * \code
			 * using bWidgets::bwScreenGraph;
			 *
			 * auto& widget = Builder::emplaceWidget<bwLabel>(screen_graph, "Foo", 0, 10);
			 * widget.foo();
			 * // ...
			 * \endcode
			 */

			template<typename _WidgetType, typename... _Args>
			static _WidgetType& emplaceWidget(bwLayoutNode& node, _Args&&... __args)
			{
				static_assert(std::is_base_of<bwWidget, _WidgetType>::value, "Should derrive from bwWidget");

				bwWidgetNode& new_node = addChildNode<bwWidgetNode>(node);
				new_node.widget = std::make_unique<_WidgetType>(std::forward<_Args>(__args)...);
				new_node.handler = new_node.widget->createHandler();
				return static_cast<_WidgetType&>(*new_node.widget);
			}

		private:
			template<typename _NodeType> static _NodeType& addChildNode(bwLayoutNode& parent_node)
			{
				static_assert(std::is_base_of<bwNode, _NodeType>::value, "Should derrive from bwScreenGraph::bwNode");

				parent_node.children.push_back(std::make_unique<_NodeType>());
				bwNode& ref = *parent_node.children.back();
				ref.parent = &parent_node;

				return dynamic_cast<_NodeType&>(ref);
			}

			template<typename _BuilderType = bwBuilder>
			void buildChildren(BuildFunc<_BuilderType> build_func, bwLayoutNode& new_node)
			{
				bwLayoutNode& old_active_layout = _active_layout_node;
				setActiveLayout(new_node);
				build_func(static_cast<_BuilderType&>(*this));
				setActiveLayout(old_active_layout);
			}

			/* Never null, so use a assignable reference. */
			std::reference_wrapper<bwLayoutNode> _active_layout_node;
		};

	}  // namespace bwScreenGraph
}  // namespace bWidgets
