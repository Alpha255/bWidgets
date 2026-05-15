#pragma once

#include <functional>
#include <list>
#include <string>

namespace bwWidgets
{
	template<class _T>
	class bwProperty
	{
	public:
		enum class NotifyFrequency
		{
			ALWAYS,
			ONCE
		};

		using GetValueFunc = std::function<const _T&()>;
		using OnValueChangedFunc = std::function<void(const _T&)>;

		template<class _Func, class... _Args>
		bwProperty(GetValueFunc&& getValueFunc, std::string optionalName = "")
			: getValue(std::move(getValueFunc))
			, lastValue(getValue())
			, name(std::move(optionalName))
		{
		}

		inline void setName(std::string newName)
		{
			name = std::move(newName);
		}

		void addObserver(OnValueChangedFunc&& onValueChanged, NotifyFrequency frequency = NotifyFrequency::ALWAYS)
		{
			observers.emplace_back({ frequency, std::move(onValueChanged) });
		}

		void notify()
		{
			if (!observers.empty())
			{
				const _T& curValue = getValue();
				if (curValue != lastValue)
				{
					for (auto it = observers.begin(); it != observers.end();)
					{
						it->onValueChanged(curValue);
						if (it->frequency == NotifyFrequency::ONCE)
						{
							it = observers.erase(it);
						}
						else
						{
							++it;
						}
					}
					lastValue = curValue;
				}
			}
		}
	protected:
		struct Observer
		{
			NotifyFrequency frequency;
			OnValueChangedFunc onValueChanged;
		};

		GetValueFunc getValue;
		_T lastValue;
		std::list<Observer> observers;
		std::string name;
	};
}