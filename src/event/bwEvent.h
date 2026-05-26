#pragma once

#include "bwDistance.h"
#include "bwPoint.h"
#include <string>

namespace bWidgets
{
	enum class KeyboardKey
	{
		NONE,
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z
	};

	enum class ModifierKey
	{
		NONE,
		SHIFT,
		LEFT_SHIFT,
		RIGHT_SHIFT,
		CTRL,
		LEFT_CTRL,
		RIGHT_CTRL,
		ALT,
		LEFT_ALT,
		RIGHT_ALT
	};

	struct bwInputKeys
	{
	};

	struct bwShortcut
	{
		ModifierKey FirstModKey = ModifierKey::NONE;
		ModifierKey SecondModKey = ModifierKey::NONE;
		KeyboardKey PrimaryKey = KeyboardKey::NONE;

		bwShortcut() = default;

		bwShortcut(KeyboardKey primary)
			: FirstModKey(ModifierKey::NONE)
			, SecondModKey(ModifierKey::NONE)
			, PrimaryKey(primary)
		{
		}

		bwShortcut(ModifierKey first, ModifierKey second, KeyboardKey primary)
			: FirstModKey(first)
			, SecondModKey(second)
			, PrimaryKey(primary) 
		{
		}

		bwShortcut(ModifierKey first, KeyboardKey primary)
			: FirstModKey(first)
			, SecondModKey(ModifierKey::NONE)
			, PrimaryKey(primary)
		{
		}

		const std::string& toString() const
		{
			return display;
		}
	private:
		std::string display;
	};

	class bwEvent
	{
	public:
		bwEvent(bwPoint _location);
		virtual ~bwEvent() = default;

		void swallow();
		void unswallow();
		bool isSwallowed() const;

		/** Where did the event happen? */
		const bwPoint location;

	private:
		bool is_swallowed{ false };
	};

	class bwMouseButtonEvent : public bwEvent
	{
	public:
		enum class Button
		{
			LEFT,
			RIGHT,
			WHEEL,

			UNKNOWN
		};

		bwMouseButtonEvent(Button _button, bwPoint _location);

		const Button button;
	};

	class bwMouseButtonDragEvent : public bwMouseButtonEvent
	{
	public:
		enum DragState
		{
			DRAGGING_BELOW_THRESHOLD,
			DRAGGING,
		};

		bwMouseButtonDragEvent(Button _button, bwPoint _location);

		void addMouseMovement(bwPoint new_location);

		const static short DRAG_THRESHOLD = 3;

		bwDistance drag_distance;
		DragState drag_state;
	};

	class bwMouseWheelEvent : public bwMouseButtonEvent
	{
	public:
		enum class Direction
		{
			UP,
			DOWN
		};

		bwMouseWheelEvent(Direction _direction, bwPoint _location);

		Direction getDirection() const;

	private:
		Direction direction;
	};

	class bwKeyEvent : public bwEvent
	{
	};

}  // namespace bWidgets
