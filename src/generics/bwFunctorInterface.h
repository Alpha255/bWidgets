#pragma once

#include <functional>

namespace bWidgets
{
	/**
	 * \brief Abstract function object class.
	 *
	 * We use this to avoid having to use function pointers with ugly void
	 * pointers to pass type-generic data. Instead, just derive from this
	 * interface and add needed data to the derived class.
	 *
	 * Example implementation:
	 * \code{.cc}
	 * class WidgetApplyFunctor : public bwFunctorInterface
	 * {
	 *   bwWidget& some_widget;
	 *
	 * public:
	 *   WidgetApplyFunctor(bwWidget& widget) :
	 *       some_widget(widget)
	 *   {
	 *   }
	 *
	 *   inline void operator()() override
	 *   {
	 *     some_widget.do_stuff();
	 *   }
	 * };
	 * \endcode
	 */
	class bwFunctorInterface
	{
	public:
		virtual void operator()() = 0;
		virtual ~bwFunctorInterface() = default;
	};

	class bwFunctorLambda : public bwFunctorInterface
	{
	public:
		template<class _Lambda>
		bwFunctorLambda(_Lambda&& lambda)
			: func(std::forward<_Lambda>(lambda))
		{
		}

		void operator()() override
		{
			if (func)
			{
				func();
			}
		}
	private:
		std::function<void()> func;
	};

	template<class _Widget>
	class bwFunctorLambda2 : public bwFunctorInterface
	{
	public:
		using OwnerType = _Widget;

		template<class _Lambda>
		bwFunctorLambda2(_Widget& inOwner, _Lambda&& lambda)
			: owner(inOwner)
			, func(std::forward<_Lambda>(lambda))
		{
		}

		void operator()() override
		{
			if (func)
			{
				func(owner);
			}
		}
	private:
		_Widget& owner;
		std::function<void(_Widget&)> func;
	};

	template<class _Value>
	class bwFunctorLambda3 : public bwFunctorInterface
	{
	public:
		template<class _GetValue, class _OnValueChanged>
		bwFunctorLambda3(_GetValue&& getValue, _OnValueChanged&& valueChanged)
			: getValue(std::forward<_GetValue>(getValue))
			, onValueChanged(std::forward<_OnValueChanged>(valueChanged))
		{
		}

		void operator()() override
		{
			if (getValue && onValueChanged)
			{
				onValueChanged(getValue());
			}
		}
	private:
		std::function<_Value()> getValue;
		std::function<void(_Value)> onValueChanged;
	};

}  // namespace bWidgets
