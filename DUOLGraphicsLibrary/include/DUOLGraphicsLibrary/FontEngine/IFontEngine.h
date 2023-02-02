#pragma once
#include "DUOLGraphicsLibrary/Export.h"
#include <string>

//전방선언
struct IDWriteFactory;
struct IDWriteFontCollection1;

namespace  DUOLGraphicsLibrary
{
    class IFontEngine;

    struct Rect
    {
        long    left;
        long    top;
        long    right;
        long    bottom;
    };

    /**

        @class   IFont
        @brief   Font For D3D11. 폰트를 가지고있는 DirectWrite의 wrapper class.
        @details ~
        @author  KyungMin Oh

    **/
    class DUOLGRAPHICSLIBRARY_EXPORT IFont
    {
    public:
        IFont() = default;
        ~IFont() = default;

    };

    /**
        @class   IBrush
        @brief   Brush For D3D11. 폰트를 그릴때 사용할 색상, 변환정보를 갖습니다.
        @details ~
        @author  KyungMin Oh

    **/
    class IBrush
    {
    public:
        IBrush(IDWriteFactory* factory, const std::wstring& fontName);

        ~IBrush() = default;
    };

 /**
     @class   IFontEngine
     @brief   Font Engine For D3D11. 폰트 출력을 담당하는 DirectWrite의 wrapper class.
     @details ~
     @author  KyungMin Oh

 **/
	class DUOLGRAPHICSLIBRARY_EXPORT IFontEngine
	{
	public:
        IFontEngine() = default;

        ~IFontEngine() = default;

	public:
		virtual IFont* CreateFontFromTTF(const std::wstring& fontpath) abstract;

        virtual IFont* GetFont(const std::wstring& fontpath) abstract;

        virtual void DrawTexts(const std::wstring& text, unsigned int length, IFont* fontType,const Rect& rect, IBrush* brush) abstract;
	};


}
