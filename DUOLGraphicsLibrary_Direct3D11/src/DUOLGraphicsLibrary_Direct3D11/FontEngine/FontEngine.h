#pragma once
#include "DUOLGraphicsLibrary/FontEngine/IFontEngine.h"

#include <dxgi.h>
#include <memory>

//전방선언
struct IDWriteFontCollection1;

namespace  DUOLGraphicsLibrary
{
    class FontEngine;

    /**

        @class   Font
        @brief   Font For D3D11. 폰트를 가지고있는 DirectWrite의 wrapper class.
        @details ~
        @author  KyungMin Oh

    **/
    class Font : public IFont
    {
    public:
        Font(IDWriteFactory* factory, const std::wstring& fontName);

        Font(IDWriteFactory* factory, const std::wstring& fontName, IDWriteFontCollection1* customFontCollection);

        ~Font() = default;

    private:
        struct Impl;

        std::unique_ptr<Impl> _pImpl;

        friend FontEngine;
    };

    /**
        @class   Brush
        @brief   Brush For D3D11. 폰트를 그릴때 사용할 색상, 변환정보를 갖습니다.
        @details ~
        @author  KyungMin Oh

    **/
    class Brush : public IBrush
    {
    public:
        Brush(IDWriteFactory* factory, const std::wstring& fontName);

        ~Brush();

    public:


    private:
        struct Impl;

        std::unique_ptr<Impl> _pImpl;

        friend FontEngine;
    };

    /**
        @class   FontEngine
        @brief   Font Engine For D3D11. 폰트 출력을 담당하는 DirectWrite의 wrapper class.
        @details ~
        @author  KyungMin Oh

    **/
    class FontEngine : public IFontEngine
    {
    public:
        FontEngine(IDXGISwapChain* d3dSwapchain, HWND handle);

        ~FontEngine();

    public:
        IFont* CreateFontFromTTF(const std::wstring& fontpath);

        IFont* GetFont(const std::wstring& fontpath);

        void DrawTexts(const std::wstring& text, UINT length, IFont* fontType, const Rect& rect, IBrush* brush);

        void UnloadBackbuffer();

        void Resize();

    private:
        struct Impl;

        std::unique_ptr<Impl> _pImpl;
    };


}
