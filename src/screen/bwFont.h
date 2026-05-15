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

#include <memory>
#include <string>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "utils/FixedNum.h"
#include "screen/bwPixmap.h"

#include "bwColor.h"
#include "bwRectangle.h"
#include "bwUtil.h"

namespace bWidgets
{
	class bwFontGlyph;
	class Pen;

	class bwFont
	{
	public:
		enum AntiAliasingMode
		{
			/** Default, pixel coverage based AA. The alpha value of a pixel is
			 * determined by how much it overlaps with filled the glyph outline. */
			NORMAL_COVERAGE,
			/* Works similar to NORMAL_COVERAGE, but gives up to 3x the horizontal
			 * resolution by addressing RGB channels separately rather than the
			 * entire pixel. A filter is used to minimize resulting color fringes,
			 * making them invisible to most people. */
			SUBPIXEL_LCD_RGB_COVERAGE,
		};

		~bwFont();

		static void initFontReading();
		static bwFont* loadFont(const std::string& name, const std::string& path);

		void render(const std::string& text, const int32_t pos_x, const int32_t pos_y);
		uint32_t calculateStringWidth(const std::string& text);

		void setFontAntiAliasingMode(AntiAliasingMode);
		void setTightPositioning(bool value);
		void setHinting(bool value);
		void setSubPixelPositioning(bool value);

		void setSize(const float size);
		int32_t getSize() const;

		const bwColor& getActiveColor() const;
		void setActiveColor(const bwColor& value);

		void setMask(const bwRectanglePixel& value);

	private:
		class FontGlyphCache
		{
			// Everything public, this nested class is private to bwFont anyway.
		public:
			void invalidate();
			void ensureUpdated(const bwFont&);
			const bwFontGlyph& getCachedGlyph(const bwFont&, const char) const;

			bool is_dirty{ true };
			std::vector<std::unique_ptr<bwFontGlyph>> cached_glyphs;

		private:
			void loadGlyphsIntoCache(const bwFont&);
		};

		bwFont() = default;

		void renderGlyph(const bwFontGlyph& glyph,
			const bwFontGlyph* previous_glyph,
			const uint32_t attr_pos,
			const uint32_t attr_texcoord,
			Pen& pen) const;

		void applyPositionBias(FixedNum<F16p16>& value) const;
		float calcSubpixelOffset(const Pen& pen, const bwFontGlyph* previous_glyph) const;
		FixedNum<F16p16> getKerningDistance(const bwFontGlyph& left, const bwFontGlyph& right) const;
		/* Accesses private members, so make it a member function. Would be better
		 * to keep freetype specific stuff out of the general bwFont class, but
		 * ignoring for now since this is just the demo app anyway. */
		FT_Int32 getFreeTypeLoadFlags() const;
		FT_Render_Mode getFreeTypeRenderFlags() const;
		bool useSubpixelPositioning() const;

		// The freetype library handle.
		static FT_Library ft_library;
		// The freetype font handle.
		FT_Face face;

		// Height in pixels.
		int32_t size{ 0 };

		bwColor active_color = bwColor::White;
		bwRectanglePixel mask{};
		AntiAliasingMode render_mode = NORMAL_COVERAGE;
		bool use_tight_positioning = false;
		bool use_hinting = false;
		bool use_subpixel_pos = false;

		FontGlyphCache cache;
	};

	class bwFontGlyph
	{
	public:
		bwFontGlyph(const uint32_t index,
			std::unique_ptr<bwPixmap>&& pixmap,
			const int32_t offset_left,
			const int32_t offset_top,
			FixedNum<F16p16> advance_width);
		bwFontGlyph() = default;

		bool is_valid = false;

		uint32_t index = 0;  // Same as freetype index

		std::unique_ptr<bwPixmap> pixmap;
		int32_t offset_left = 0, offset_top = 0;  // bitmap_left, bitmap_top
		FixedNum<F16p16> advance_width;
		int32_t pitch = 0;
	};

}  // namespace bWidgets
