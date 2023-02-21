#include "DUOLGraphicsLibrary_Direct3D11/FontEngine/FontEngine.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXHelper.h"
#include <unordered_map>
#include <d3d11.h>
#include <dwrite_3.h>

#include <algorithm>
#include "DUOLGraphicsLibrary/Core/TypeCast.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Resource/D3D11Texture.h"

namespace DUOLGraphicsLibrary
{
	///////////////////////////////////////////////////////

	struct Font::Impl
	{
	public:
		Impl(IDWriteFactory* factory, const std::wstring& fontName, const std::wstring& fontLocale, ComPtr<IDWriteFontCollection1> customFontCollection);

		~Impl();

	public:
		IDWriteTextFormat* GetIDWriteTextFormat(UINT32 fontSize, WeightOption weightOption, StyleOption styleOption);

	private:
		ComPtr<IDWriteFontCollection1> _fontCollections;

		std::unordered_map<unsigned int, ComPtr<IDWriteTextFormat>> _textFormats;

		IDWriteFactory* _factory;

		std::wstring _fontName;

		std::wstring _fontLocale;
	};

	Font::Impl::Impl(IDWriteFactory* factory, const std::wstring& fontName, const std::wstring& fontLocale, ComPtr<IDWriteFontCollection1> customFontCollection) :
		_fontCollections(customFontCollection)
		, _factory(factory)
		, _fontName(fontName)
		, _fontLocale(fontLocale)
	{
	}

	Font::Impl::~Impl()
	{
		_textFormats.clear();
		_fontCollections.Reset();
	}

	IDWriteTextFormat* Font::Impl::GetIDWriteTextFormat(UINT32 fontSize, WeightOption weightOption, StyleOption styleOption)
	{
		HRESULT hr;

		UINT32 key = 0;

		if(fontSize > 255)
		{
			fontSize = 255;
		}

		key += static_cast<UINT32>(fontSize) << 8 * 2;
		key += static_cast<UINT32>(weightOption) << 8 ;
		key += static_cast<UINT32>(styleOption);

		auto foundFont = _textFormats.find(key);

		if (foundFont == _textFormats.end())
		{
			ComPtr<IDWriteTextFormat> newFont;
			//없으므로 제작합니다.

			DWRITE_FONT_WEIGHT weight;

			if (weightOption == WeightOption::BOLD)
			{
				weight = DWRITE_FONT_WEIGHT_BOLD;
			}
			else
			{
				weight = DWRITE_FONT_WEIGHT_NORMAL;
			}

			DWRITE_FONT_STYLE style;

			if (styleOption == StyleOption::ITALIC)
			{
				style = DWRITE_FONT_STYLE_ITALIC;
			}
			else if (styleOption == StyleOption::OBLIQUE)
			{
				style = DWRITE_FONT_STYLE_OBLIQUE;
			}
			else
			{
				style = DWRITE_FONT_STYLE_NORMAL;
			}

			hr = _factory->CreateTextFormat(_fontName.c_str(), _fontCollections.Get(), weight, style, DWRITE_FONT_STRETCH_NORMAL, fontSize, _fontLocale.c_str(), newFont.ReleaseAndGetAddressOf());

			if (FAILED(hr))
			{
				hr = _factory->CreateTextFormat(_fontName.c_str(), _fontCollections.Get(), weight, style, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"en-us", newFont.ReleaseAndGetAddressOf());
			}

			auto ret = newFont.Get();
			ret->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

			_textFormats.emplace(key, newFont);

			return ret;
		}

		return foundFont->second.Get();
	}

	Font::Font(IDWriteFactory* factory, const std::wstring& fontName, const std::wstring& fontLocale, ComPtr<IDWriteFontCollection1> customFontCollection)
	{
		_pImpl = std::make_unique<Font::Impl>(factory, fontName, fontLocale, customFontCollection);
	}

	IDWriteTextFormat* Font::GetIDWriteTextFormat(UINT32 fontSize, WeightOption weightOption, StyleOption styleOption)
	{
		return _pImpl->GetIDWriteTextFormat(fontSize, weightOption, styleOption);
	}

	/////////////////////////////////////////////////////
	struct Brush::Impl
	{
	public:
		Impl(ID2D1RenderTarget* renderTarget, const DUOLMath::Vector4& color);

		~Impl();

	public:
		void SetColor(const Canvas* canvas, const DUOLMath::Vector4& color);

		void CreateBrush(ID2D1RenderTarget* renderTarget, const DUOLMath::Vector4& color);

		ID2D1SolidColorBrush* GetBrush();

	private:
		ComPtr<ID2D1SolidColorBrush> _colorBrush;

		DUOLMath::Vector4 _brushColor;
	};

	Brush::Impl::Impl(ID2D1RenderTarget* renderTarget, const DUOLMath::Vector4& color) :
		_colorBrush(nullptr)
	{
		CreateBrush(renderTarget, color);
	}

	Brush::Impl::~Impl()
	{
	}

	void Brush::Impl::SetColor(const Canvas* canvas, const DUOLMath::Vector4& color)
	{
		_brushColor = color;
	}

	void Brush::Impl::CreateBrush(ID2D1RenderTarget* renderTarget, const DUOLMath::Vector4& color)
	{
		HRESULT hr;

		DUOLMath::Vector4 clampColor;
		color.Clamp(DUOLMath::Vector4{ 0.f, 0.f, 0.f, 0.f }, DUOLMath::Vector4{ 255.f, 255.f, 255.f, 255.f }, clampColor);

		D2D1_COLOR_F* castedKey = reinterpret_cast<D2D1_COLOR_F*>(&clampColor);

		hr = renderTarget->CreateSolidColorBrush(
			*castedKey,
			_colorBrush.GetAddressOf());

	}

	ID2D1SolidColorBrush* Brush::Impl::GetBrush()
	{
		return _colorBrush.Get();
	}

	Brush::Brush(ID2D1RenderTarget* renderTarget, const DUOLMath::Vector4& color)
	{
		_pImpl = std::make_unique<Impl>(renderTarget, color);
	}

	Brush::~Brush()
	{
	}

	ID2D1SolidColorBrush* Brush::GetBrush()
	{
		return _pImpl->GetBrush();
	}

	///////////////////////////////////////////////////////

	struct ComparePair
	{
		bool operator()(const std::pair<UINT32, IResource* const>& lvalue, const std::pair<UINT32, IResource* const>& rvalue)
		{
			return lvalue.first > rvalue.first;
		}
	};

	struct Canvas::Impl
	{
	public:
		Impl(ID2D1DeviceContext* context, IDXGISurface* surface);

		~Impl();

	public:
		void CreateD2D1RenderTarget(ID2D1DeviceContext* context, IDXGISurface* surface);

		void UnloadRenderTarget();

		ID2D1Bitmap1* GetRenderTarget();

		UIQueue& GetUIQueue();

		void DrawTexts(TextBox* const textBox, UINT32 orderInLayer);

		void DrawSprite(Sprite* const sprite, UINT32 orderInLayer);

	private:
		ComPtr<ID2D1Bitmap1> _renderTarget;

		UIQueue _uiQueue;
	};

	Canvas::Impl::Impl(ID2D1DeviceContext* context, IDXGISurface* surface)
	{
		CreateD2D1RenderTarget(context, surface);
		_uiQueue.Reserve(16);
	}

	Canvas::Impl::~Impl()
	{
	}

	Canvas::Canvas(ID2D1DeviceContext* context, IDXGISurface* surface)
	{
		_pImpl = std::make_unique<Impl>(context, surface);

	}

	void Canvas::DrawTexts(TextBox* const textBox, UINT32 orderInLayer)
	{
		_pImpl->DrawTexts(textBox, orderInLayer);
	}

	void Canvas::DrawSprite(Sprite* const sprite, UINT32 orderInLayer)
	{
		_pImpl->DrawSprite(sprite, orderInLayer);
	}

	void Canvas::Unload()
	{
		_pImpl->UnloadRenderTarget();
	}

	ID2D1Bitmap1* Canvas::GetRenderTarget()
	{
		return  _pImpl->GetRenderTarget();
	}

	void Canvas::CreateRenderTarget(ID2D1DeviceContext* context, IDXGISurface* surface)
	{
		_pImpl->CreateD2D1RenderTarget(context, surface);
	}

	Canvas::UIQueue& Canvas::GetUIQueue()
	{
		return _pImpl->GetUIQueue();
	}

	void Canvas::Impl::CreateD2D1RenderTarget(ID2D1DeviceContext* context, IDXGISurface* surface)
	{
		HRESULT hr;

		D2D1_BITMAP_PROPERTIES1  props =
			D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
				96,
				96);

		hr = context->CreateBitmapFromDxgiSurface(
			surface,
			&props,
			_renderTarget.GetAddressOf());

		DXThrowError(hr, "Canvas Create Error");
	}

	void Canvas::Impl::UnloadRenderTarget()
	{
		_renderTarget.Reset();
	}

	ID2D1Bitmap1* Canvas::Impl::GetRenderTarget()
	{
		return _renderTarget.Get();
	}

	Canvas::UIQueue& Canvas::Impl::GetUIQueue()
	{
		return _uiQueue;
	}

	void Canvas::Impl::DrawTexts(TextBox* const textBox, UINT32 orderInLayer)
	{
		_uiQueue.emplace(orderInLayer, textBox);
	}

	void Canvas::Impl::DrawSprite(Sprite* const sprite, UINT32 orderInLayer)
	{
		_uiQueue.emplace(orderInLayer, sprite);
		//_renderTarget->DrawBitmap();
		//_renderTarget->CreateBitmapFromWicBitmap()
	}

	///////////////////////////////////////////////////////
	struct FontEngine::Impl
	{
	public:
		Impl(IDXGIDevice* dxgiDevice, IDXGISwapChain* d3dSwapchain, HWND handle);

		~Impl();

	public:
		Font* CreateFontFromTTF(const std::wstring& fontName);

		Font* GetFont(const std::wstring& fontName);

		Canvas* CreateCanvas(const std::wstring& canvasName, CanvasRenderMode rendertype, Texture* const texture);

		void UnloadBackBuffer();

		void CreateBackbuffer();

		void DrawCanvas(ICanvas* canvas);

		void Excute();

		void RegistSprite(Texture* texture);

		void DeleteSprite(Texture* texture);

	private:
		void CreateFactory(IDXGIDevice* dxgiDevice);

		void DrawTexts(TextBox* text);

		void DrawSprite(Sprite* sprite);

	private:
		ComPtr<IDWriteFactory5> _writeFactory;

		ComPtr<IDWriteFontSetBuilder1> _fontSetBuilder;

		ComPtr<ID2D1Factory1> _d2dFactory;

		ComPtr<ID2D1Device> _d2dDevice;

		ComPtr<ID2D1DeviceContext> _d2dDeviceContext;

		std::unique_ptr<Canvas> _backbufferRenderTarget;

		std::unordered_map<long, std::unique_ptr<Brush>> _colorBrushes;

		HWND _handle;

		IDXGISwapChain* _d3dSwapchain;

		std::unordered_map<std::wstring, std::unique_ptr<Font>> _fonts;

		std::unordered_map<std::wstring, std::unique_ptr<Canvas>> _canvases;

		std::unordered_map<Texture*, ComPtr<ID2D1Bitmap1>> _sprites;

		std::vector<ICanvas*> _canvasQueue;
	};

	FontEngine::Impl::Impl(IDXGIDevice* dxgiDevice, IDXGISwapChain* d3dSwapchain, HWND handle) :
		_d3dSwapchain(d3dSwapchain)
		, _handle(handle)
	{
		CreateFactory(dxgiDevice);

		ComPtr<IDXGISurface> buffer;
		HRESULT hr = _d3dSwapchain->GetBuffer(0, IID_PPV_ARGS(buffer.GetAddressOf()));

		float dpi = GetDpiForWindow(_handle);

		//렌더타겟 생성
		_backbufferRenderTarget = std::make_unique<Canvas>(_d2dDeviceContext.Get(), buffer.Get());
	}

	FontEngine::Impl::~Impl()
	{
		_backbufferRenderTarget.reset();

		_fonts.clear();

		_d2dFactory.Reset();
		_fontSetBuilder.Reset();
		_writeFactory.Reset();
	}

	Font* FontEngine::Impl::CreateFontFromTTF(const std::wstring& fontpath)
	{
		HRESULT hr;	

		auto ret = _fonts.find(fontpath);

		//폰트를 찾지 못했으므로 생성합니다.
		if (ret == _fonts.end())
		{
			std::unique_ptr<Font> newFont;

			ComPtr<IDWriteFontFile> fontFile;

			hr = _writeFactory->CreateFontFileReference(fontpath.c_str(), NULL, fontFile.GetAddressOf());
			_fontSetBuilder.Reset();
			_writeFactory->CreateFontSetBuilder(_fontSetBuilder.GetAddressOf());
			hr = _fontSetBuilder->AddFontFile(fontFile.Get());

			ComPtr<IDWriteFontSet> fontSet;

			_fontSetBuilder->CreateFontSet(fontSet.GetAddressOf());

			IDWriteFontCollection1* fontCollection;

			_writeFactory->CreateFontCollectionFromFontSet(fontSet.Get(), &fontCollection);

			ComPtr<IDWriteStringList> familyName;

			fontSet->GetPropertyValues(DWRITE_FONT_PROPERTY_ID_FAMILY_NAME, familyName.GetAddressOf());

			WCHAR name[256];
			familyName->GetString(0, name, 256);

			WCHAR locale[256];
			familyName->GetLocaleName(0, locale, 256);

			//todo:: 생성 실패시 추가를 하지말고 에러 뱉어낼것
			newFont = std::make_unique<Font>(_writeFactory.Get(), name, locale, fontCollection);
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

		//폰트를 찾지 못했으므로 Null 반환.
		if (ret == _fonts.end())
		{
			return nullptr;
		}

		return ret->second.get();
	}

	Canvas* FontEngine::Impl::CreateCanvas(const std::wstring& canvasName, CanvasRenderMode rendertype, Texture* const texture)
	{
		if (rendertype == CanvasRenderMode::Texture)
		{
			auto ret = _canvases.find(canvasName);

			//Canvas를 찾지 못했으므로 생성합니다.
			if (ret == _canvases.end())
			{
				if(texture == nullptr)
					DXThrowError(E_FAIL, "FontManager :: DXGI RenderTarget CreateFailed. Texture Is Nullptr");

				D3D11Texture* d3dTexture = TYPE_CAST(D3D11Texture*, texture);

				//todo:: 여기서 얻은 텍스쳐는 무조건 2D여야 합니다. 에러처리 해줄것
				//todo:: 또한 텍스쳐의 type 또한 B8G8R8A8... 이여야함
				auto& nativeTex = d3dTexture->GetNativeTexture();

				ComPtr<IDXGISurface> buffer;

				HRESULT hr = nativeTex._tex2D->QueryInterface(buffer.GetAddressOf());

				//_canvases
				DXThrowError(hr, "FontManager :: DXGI RenderTarget CreateFailed");
				auto newCanvas = std::make_unique<Canvas>(_d2dDeviceContext.Get(), buffer.Get());
			}
			else
			{
				return ret->second.get();
			}
		}
		else
		{
			return _backbufferRenderTarget.get();
		}
	}

	void FontEngine::Impl::UnloadBackBuffer()
	{
		_d2dDeviceContext->SetTarget(nullptr);
		_backbufferRenderTarget->Unload();
	}

	void FontEngine::Impl::CreateFactory(IDXGIDevice* dxgiDevice)
	{
		HRESULT hr;

		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory5), reinterpret_cast<IUnknown**>(_writeFactory.GetAddressOf()));
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, _d2dFactory.GetAddressOf());


		hr = _d2dFactory->CreateDevice(dxgiDevice, _d2dDevice.GetAddressOf());
		hr = _d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, _d2dDeviceContext.GetAddressOf());

		//_writeFactory->RegisterFontCollectionLoader(_fontCollectionLoader.Get());
		//_writeFactory->CreateCustomFontCollection(_fontCollectionLoader.Get(), );
	}

	void FontEngine::Impl::DrawTexts(TextBox* text)
	{
		DUOLMath::Vector4 clampColor;
		text->_color.Clamp(DUOLMath::Vector4{ 0.f, 0.f, 0.f, 0.f }, DUOLMath::Vector4{ 255.f, 255.f, 255.f, 255.f }, clampColor);

		unsigned long r = static_cast<unsigned int>(clampColor.x);
		unsigned long g = static_cast<unsigned int>(clampColor.y);
		unsigned long b = static_cast<unsigned int>(clampColor.z);
		unsigned long a = static_cast<unsigned int>(clampColor.w);

		long key = 0;

		key += r << (8 * 3);
		key += g << (8 * 2);
		key += b << (8 * 1);
		key += a;

		auto foundBrush = _colorBrushes.find(key);

		ID2D1SolidColorBrush* brush;

		if (foundBrush == _colorBrushes.end())
		{
			//새로 만들어줍니다.
			std::unique_ptr<Brush> newBrush = std::make_unique<Brush>(_d2dDeviceContext.Get(), clampColor);
			brush = newBrush->GetBrush();
			_colorBrushes.emplace(key, std::move(newBrush));
		}
		else
		{
			brush = foundBrush->second->GetBrush();
		}

		Font* font = static_cast<Font*>(text->_fontType);

		D2D1_RECT_F d2drect;

		d2drect.bottom = text->_rect.bottom;
		d2drect.top = text->_rect.top;
		d2drect.right = text->_rect.right;
		d2drect.left = text->_rect.left;

		_d2dDeviceContext->DrawTextW(text->_text.c_str(), text->_text.size(), font->GetIDWriteTextFormat(text->_fontSize, text->_weightOption, text->_styleOption), &d2drect, brush, D2D1_DRAW_TEXT_OPTIONS_CLIP);
	}

	void FontEngine::Impl::DrawSprite(Sprite* sprite)
	{

		auto foundImage = _sprites.find(sprite->_texture);

		if (foundImage == _sprites.end())
		{
			RegistSprite(sprite->_texture);
			return;
		}

		D2D1_POINT_2F offset;

		offset.x = -sprite->_offset.x * (sprite->_rect.right - sprite->_rect.left);
		offset.y = -sprite->_offset.y * (sprite->_rect.bottom - sprite->_rect.top);

		D2D1_RECT_F rectSize;

		rectSize.left = offset.x +  sprite->_rect.left;
		rectSize.top = offset.y + sprite->_rect.top;
		rectSize.right = offset.x + sprite->_rect.right;
		rectSize.bottom = offset.y + sprite->_rect.bottom;

		_d2dDeviceContext->DrawBitmap(foundImage->second.Get(), &rectSize);
	}

	void FontEngine::Impl::CreateBackbuffer()
	{
		ComPtr<IDXGISurface> buffer;

		HRESULT hr = _d3dSwapchain->GetBuffer(0, IID_PPV_ARGS(buffer.GetAddressOf()));
		float dpi = GetDpiForWindow(_handle);

		//렌더타겟 생성
		_backbufferRenderTarget->CreateRenderTarget(_d2dDeviceContext.Get(), buffer.Get());
	}

	void FontEngine::Impl::DrawCanvas(ICanvas* canvas)
	{
		_canvasQueue.emplace_back(canvas);
	}

	void FontEngine::Impl::Excute()
	{
		HRESULT hr;
		_d2dDeviceContext->BeginDraw();

		for (auto& canvas : _canvasQueue)
		{
			Canvas* castedCanvas = static_cast<Canvas*>(canvas);

			_d2dDeviceContext->SetTarget(castedCanvas->GetRenderTarget());

			auto& uiQueue = castedCanvas->GetUIQueue();

			for (auto& ui : uiQueue.GetVector())
			{

				if (ui.second->_resourceType == IResource::ResourceType::Text)
				{
					DrawTexts(static_cast<TextBox*>(ui.second));
				}
				else if (ui.second->_resourceType == IResource::ResourceType::Sprite)
				{
					DrawSprite(static_cast<Sprite*>(ui.second));
				}
			}

			uiQueue.Clear();
		}

		hr = _d2dDeviceContext->EndDraw();
		_canvasQueue.clear();
	}

	void FontEngine::Impl::RegistSprite(Texture* texture)
	{
		auto foundImage = _sprites.find(texture);

		D3D11Texture* d3dtexture = TYPE_CAST(D3D11Texture*, texture);

		//todo::예외처리가 되지않았습니다.2d인경우만 리소스화 가능합니다.
		auto& d3dtexture2d = d3dtexture->GetNativeTexture()._tex2D;

		ComPtr<IDXGISurface> buffer;

		HRESULT hr = d3dtexture2d->QueryInterface(buffer.GetAddressOf());

		ComPtr<ID2D1Bitmap1> d2dBmp;

		if (foundImage != _sprites.end())
		{
			//hr = _d2dDeviceContext->CreateBitmapFromDxgiSurface(buffer.Get(), nullptr, foundImage->second.ReleaseAndGetAddressOf());
		}
		else
		{
			//D2D1_BITMAP_PROPERTIES1  props =
			//	D2D1::BitmapProperties1(
			//		D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			//		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			//		96,
			//		96);

			hr = _d2dDeviceContext->CreateBitmapFromDxgiSurface(buffer.Get(), nullptr, d2dBmp.GetAddressOf());

			_sprites.emplace(texture, d2dBmp);
		}
	}

	void FontEngine::Impl::DeleteSprite(Texture* texture)
	{
		_sprites.erase(texture);
	}

	DUOLGraphicsLibrary::FontEngine::FontEngine(IDXGIDevice* dxgiDevice, IDXGISwapChain* d3dSwapchain, HWND handle)
	{
		_pImpl = std::make_unique<Impl>(dxgiDevice, d3dSwapchain, handle);
	}

	FontEngine::~FontEngine()
	{
		_pImpl.reset();
	}

	IFont* FontEngine::CreateFontFromTTF(const std::wstring& fontpath)
	{
		return _pImpl->CreateFontFromTTF(fontpath);
	}

	IFont* FontEngine::GetFont(const std::wstring& fontpath)
	{
		return _pImpl->GetFont(fontpath);
	}

	ICanvas* FontEngine::CreateCanvas(const std::wstring& canvasName, CanvasRenderMode rendertype, Texture* const texture)
	{
		return _pImpl->CreateCanvas(canvasName, rendertype, texture);
	}

	void FontEngine::DrawCanvas(ICanvas* canvas)
	{
		_pImpl->DrawCanvas(canvas);
	}

	void FontEngine::Execute()
	{
		_pImpl->Excute();
	}

	bool FontEngine::DeleteCanvas(const std::wstring& canvasName)
	{
		return false;
	}

	void FontEngine::RegistSprite(Texture* texture)
	{
		_pImpl->RegistSprite(texture);
	}

	void FontEngine::DeleteSprite(Texture* texture)
	{
		_pImpl->DeleteSprite(texture);
	}

	void FontEngine::UnloadBackbuffer()
	{
		_pImpl->UnloadBackBuffer();
	}

	void FontEngine::CreateBackbuffer()
	{
		_pImpl->CreateBackbuffer();
	}
}
