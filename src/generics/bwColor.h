#pragma once

#include <stdint.h>
#include <cereal/cereal.hpp>

namespace bWidgets
{
	class bwColor
	{
	public:
		bwColor(float red, float green, float blue, float alpha = 1.0f);
		bwColor(float rgb, float alpha = 1.0f);
		bwColor(uint32_t red, uint32_t green, uint32_t blue, uint32_t alpha = 255);
		bwColor(uint32_t rgb, uint32_t alpha = 255);
		bwColor() = default;
		~bwColor() = default;
		bwColor(const bwColor&);

		bwColor& shade(float rgb_shade, float alpha_shade = 0.0f);
		bwColor& shade(uint32_t rgb_shade, uint32_t alpha_shade = 0.0f);

		void setColor(float red, float green, float blue, float alpha = 1.0f);
		void setColor(float rgb, float alpha = 1.0f);
		void setColor(const float rgba[4]);
		const float* getColor() const;
		bwColor& operator=(const float* rgb);
		bwColor& operator=(const bwColor& other_color);

		bool operator==(const bwColor& compare_color) const;
		float& operator[](const int32_t index);
		// Implicit conversion to float*
		operator const float* () const;

		const static bwColor White;

		template<class Archive>
		void serialize(Archive ar)
		{
			ar(
				CEREAL_NVP(rgba)
			);
		}
	private:
		enum class Component
		{
			RED = 0,
			GREEN = 1,
			BLUE = 2,
			ALPHA = 3,
		};

		float& operator[](const Component component)
		{
			return rgba[static_cast<int32_t>(component)];
		}

		void clamp(const Component component);

		float rgba[4]{ 0, 0, 0, 1 };
	};

}  // namespace bWidgets
