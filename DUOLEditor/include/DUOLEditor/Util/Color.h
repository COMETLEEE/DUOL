/**

    @file      Color.h
    @brief     it is color.
    @details   ~
    @author    COMETLEE
    @date      16.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLMath/DUOLMath.h"

namespace DUOLEditor
{
	/**
	 * \brief represent r, g, b, a 4 components color.
	 */
	struct Color
    {
        Color(float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f);
        
        float r;

        float g;

        float b;

        float a;

        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color White;
        static const Color Black;
        static const Color Gray;
        static const Color Yellow;
        static const Color Cyan;
        static const Color Magenta;

        bool operator == (const Color& other) const;

        bool operator != (const Color& other) const;
    };
}