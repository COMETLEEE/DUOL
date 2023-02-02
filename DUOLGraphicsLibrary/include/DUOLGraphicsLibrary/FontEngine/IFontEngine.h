#pragma once
#include "DUOLGraphicsLibrary/Export.h"
#include <string>

//���漱��
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
        @brief   Font For D3D11. ��Ʈ�� �������ִ� DirectWrite�� wrapper class.
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
        @brief   Brush For D3D11. ��Ʈ�� �׸��� ����� ����, ��ȯ������ �����ϴ�.
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
     @brief   Font Engine For D3D11. ��Ʈ ����� ����ϴ� DirectWrite�� wrapper class.
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
