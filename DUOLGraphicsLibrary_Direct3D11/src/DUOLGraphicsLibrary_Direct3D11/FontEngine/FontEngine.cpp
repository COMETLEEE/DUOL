#include "DUOLGraphicsLibrary_Direct3D11/FontEngine/FontEngine.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXHelper.h"
#include <unordered_map>
#include <wrl.h>
#include <d3d11.h>
#include <d2d1.h>
#include <dwrite_3.h>

namespace DUOLGraphicsLibrary
{
	// Type alias for Win32 ComPtr template
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	///////////////////////////////////////////////////////

	struct Font::Impl
	{
	public:
		Impl(IDWriteFactory* factory, const std::wstring& fontName);

		Impl(IDWriteFactory* factory, const std::wstring& fontName, IDWriteFontCollection1* customFontCollection);

	public:
		IDWriteTextFormat* GetIDWriteTextFormat();

	private:
		ComPtr<IDWriteFontCollection1> _fontCollections;

		ComPtr<IDWriteTextFormat> _textFormat;
	};

	Font::Impl::Impl(IDWriteFactory* factory, const std::wstring& fontName)
	{
		factory->CreateTextFormat(fontName.c_str(), NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 72.f, L"en-us", _textFormat.ReleaseAndGetAddressOf());
	}

	Font::Impl::Impl(IDWriteFactory* factory, const std::wstring& fontName, IDWriteFontCollection1* customFontCollection)
	{
		_fontCollections = customFontCollection;

		factory->CreateTextFormat(fontName.c_str(), customFontCollection, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 72.f, L"en-us", _textFormat.ReleaseAndGetAddressOf());
	}

	IDWriteTextFormat* Font::Impl::GetIDWriteTextFormat()
	{
		return _textFormat.Get();
	}

	Font::Font(IDWriteFactory* factory, const std::wstring& fontName)
	{
		_pImpl = std::make_unique<Font::Impl>(factory, fontName);
	}

	Font::Font(IDWriteFactory* factory, const std::wstring& fontName, IDWriteFontCollection1* customFontCollection)
	{
		_pImpl = std::make_unique<Font::Impl>(factory, fontName, customFontCollection);
	}

	///////////////////////////////////////////////////////
	struct FontEngine::Impl
	{
	public:
		Impl(IDXGISwapChain* d3dSwapchain, HWND handle);

	public:
		Font* CreateFontFromTTF(const std::wstring& fontName);

		Font* GetFont(const std::wstring& fontName);

		void DrawTexts(const std::wstring& text, UINT length, IFont* fontType, const D2D1_RECT_F& rect, IBrush* brush);

		void Resize();

		void UnloadBackBuffer();

	private:
		void CreateFactory();

		void CreateDXGIRenderTarget();

	private:
		ComPtr<IDWriteFactory5> _writeFactory;

		ComPtr<IDWriteFontSetBuilder1> _fontSetBuilder;

		ComPtr<IDWriteFontCollectionLoader> _fontCollectionLoader;

		ComPtr<IDWriteFontCollection> _fontCollection;

		ComPtr<ID2D1Factory> _d2dFactory;

		ComPtr<ID2D1RenderTarget> _backbufferRenderTarget;

		HWND _handle;

		IDXGISwapChain* _d3dSwapchain;

		std::unordered_map<std::wstring, std::unique_ptr<Font>> _fonts;
	};

	FontEngine::Impl::Impl(IDXGISwapChain* d3dSwapchain, HWND handle) :
		_d3dSwapchain(d3dSwapchain)
		, _handle(handle)
	{
		CreateFactory();
		CreateDXGIRenderTarget();
	}

	Font* FontEngine::Impl::CreateFontFromTTF(const std::wstring& fontpath)
	{
		auto ret = _fonts.find(fontpath);

		//폰트를 찾지 못했으므로 생성합니다.
		if (ret == _fonts.end())
		{
			std::unique_ptr<Font> newFont;

			ComPtr<IDWriteFontFile> fontFile;

			_writeFactory->CreateFontFileReference(fontpath.c_str(), NULL, fontFile.GetAddressOf());
			_fontSetBuilder->AddFontFile(fontFile.Get());

			ComPtr<IDWriteFontSet> fontSet;

			_fontSetBuilder->CreateFontSet(fontSet.GetAddressOf());

			IDWriteFontCollection1* fontCollection;

			_writeFactory->CreateFontCollectionFromFontSet(fontSet.Get(), &fontCollection);

			IDWriteStringList* familyName;

			fontSet->GetPropertyValues(DWRITE_FONT_PROPERTY_ID_FAMILY_NAME, &familyName);

			WCHAR name[256];
			familyName->GetString(0, name, 256);

			//todo:: 생성 실패시 추가를 하지말고 에러 뱉어낼것
			newFont = std::make_unique<Font>(_writeFactory.Get(), name, fontCollection);
			auto ret = newFont.get();

			_fonts.emplace(fontpath, std::move(newFont));

			return ret;
		}
		else
		{
			return ret->second.get();
		}
	}

	Font* FontEngine::Impl::GetFont(const std::wstring& fontName)
	{
		auto ret = _fonts.find(fontName);

		//폰트를 찾지 못했으므로 생성합니다.
		if (ret == _fonts.end())
		{
			return nullptr;
		}

		return ret->second.get();
	}

	void FontEngine::Impl::DrawTexts(const std::wstring& text, UINT length, IFont* fontType, const D2D1_RECT_F& rect,
		IBrush* brush)
	{
		HRESULT hr;

		auto inputrect = rect;

		ID2D1SolidColorBrush* _pBlackBrush;
		hr = _backbufferRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			&_pBlackBrush
		);

		Font* font = static_cast<Font*>(fontType);
		_backbufferRenderTarget->BeginDraw();

		_backbufferRenderTarget->DrawTextW(text.c_str(), length, font->_pImpl->GetIDWriteTextFormat(), &rect, _pBlackBrush);

		_backbufferRenderTarget->EndDraw();
	}

	void FontEngine::Impl::Resize()
	{
		CreateDXGIRenderTarget();
	}

	void FontEngine::Impl::UnloadBackBuffer()
	{
		_backbufferRenderTarget.Reset();
	}

	void FontEngine::Impl::CreateFactory()
	{
		HRESULT hr;

		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory5), reinterpret_cast<IUnknown**>(_writeFactory.GetAddressOf()));

		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, _d2dFactory.GetAddressOf());

		_writeFactory->CreateFontSetBuilder(_fontSetBuilder.GetAddressOf());

		//_writeFactory->RegisterFontCollectionLoader(_fontCollectionLoader.Get());
		//_writeFactory->CreateCustomFontCollection(_fontCollectionLoader.Get(), );
	}

	void FontEngine::Impl::CreateDXGIRenderTarget()
	{
		ComPtr<IDXGISurface> _buffer;

		RECT rc;
		GetClientRect(_handle, &rc);
		HRESULT hr = _d3dSwapchain->GetBuffer(0, IID_PPV_ARGS(_buffer.GetAddressOf()));

		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		float dpi = GetDpiForWindow(_handle);

		D2D1_RENDER_TARGET_PROPERTIES props =
			D2D1::RenderTargetProperties(
				D2D1_RENDER_TARGET_TYPE_DEFAULT
				, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)
				, 0.f
				, 0.f);

		//렌더타겟 생성
		hr = _d2dFactory->CreateDxgiSurfaceRenderTarget(_buffer.Get(), &props, _backbufferRenderTarget.GetAddressOf());
		DXThrowError(hr, "FontManager :: DXGI RenderTarget CreateFailed");
	}

	DUOLGraphicsLibrary::FontEngine::FontEngine(IDXGISwapChain* d3dSwapchain, HWND handle)
	{
		_pImpl = std::make_unique<Impl>(d3dSwapchain, handle);
	}

	FontEngine::~FontEngine()
	{

	}

	IFont* FontEngine::CreateFontFromTTF(const std::wstring& fontpath)
	{
		return _pImpl->CreateFontFromTTF(fontpath);
	}

	IFont* FontEngine::GetFont(const std::wstring& fontpath)
	{
		return _pImpl->GetFont(fontpath);
	}

	void FontEngine::DrawTexts(const std::wstring& text, UINT length, IFont* fontType, const Rect& rect, IBrush* brush)
	{
		D2D1_RECT_F d2drect;

		d2drect.bottom = rect.bottom;
		d2drect.top = rect.top;
		d2drect.right = rect.right;
		d2drect.left = rect.left;

		_pImpl->DrawTexts(text, length, fontType, d2drect, brush);
	}

	void FontEngine::UnloadBackbuffer()
	{
		_pImpl->UnloadBackBuffer();
	}

	void FontEngine::Resize()
	{
		_pImpl->Resize();
	}
}
