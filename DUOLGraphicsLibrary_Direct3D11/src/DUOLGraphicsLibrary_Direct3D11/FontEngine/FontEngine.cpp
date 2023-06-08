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

		if (fontSize > 255)
		{
			fontSize = 255;
		}

		key += static_cast<UINT32>(fontSize) << 8 * 2;
		key += static_cast<UINT32>(weightOption) << 8;
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
			ret->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

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

	//void DWSprite::SetTexture(Texture* texture)
	//{
	//	_texture = texture;
	//}

	//void DWSprite::SetColor(DUOLMath::Vector4& color)
	//{

	//}

	//bool DWSprite::HasEffectImage()
	//{
	//	return _hasColorTransform;
	//}

	Brush::Brush(ID2D1RenderTarget* renderTarget, const DUOLMath::Vector4& color)
	{
		_pImpl = std::make_unique<Impl>(renderTarget, color);
	}

	Brush::~Brush()
	{
	}

	void Brush::SetColor(const Canvas* canvas, const DUOLMath::Vector4& color)
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
		Impl(ID2D1DeviceContext* context, IDXGISurface* surface, Texture* texture, CanvasRenderMode rendertype);

		~Impl();

	public:
		void CreateD2D1RenderTarget(ID2D1DeviceContext* context, IDXGISurface* surface);

		void UnloadRenderTarget();

		ID2D1Bitmap1* GetRenderTarget();

		UIQueue& GetUIQueue();

		void DrawTexts(TextBox* const textBox, UINT32 orderInLayer);

		void DrawSprite(Sprite* const sprite, UINT32 orderInLayer);

		void RebuildRenderTarget(ID2D1DeviceContext* context);

		Texture* GetRenderTargetOrigin();

	private:
		ComPtr<ID2D1Bitmap1> _renderTarget;

		Texture* _renderTargetOrigin;

		UIQueue _uiQueue;

		CanvasRenderMode _canvasRenderType;
	};

	Canvas::Impl::Impl(ID2D1DeviceContext* context, IDXGISurface* surface, Texture* texture, CanvasRenderMode rendertype):
		_renderTargetOrigin(texture),
		_renderTarget(nullptr),
		_canvasRenderType(rendertype)
	{
		CreateD2D1RenderTarget(context, surface);
		_uiQueue.reserve(16);
	}

	Canvas::Impl::~Impl()
	{
		_renderTarget.Reset();
		_uiQueue.clear();
	}

	Canvas::Canvas(ID2D1DeviceContext* context, Texture* texture, IDXGISurface* surface, CanvasRenderMode rendertype)
		
	{
		_pImpl = std::make_unique<Impl>(context, surface, texture, rendertype);
	}

	Canvas::~Canvas()
	{
		_pImpl.reset();
	}

	Texture* Canvas::GetTexture()
	{
		return _pImpl->GetRenderTargetOrigin();
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

	void Canvas::RebuildRenderTarget(ID2D1DeviceContext* context)
	{
		_pImpl->RebuildRenderTarget(context);
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
		_uiQueue.emplace_back(orderInLayer, textBox);
	}

	void Canvas::Impl::DrawSprite(Sprite* const sprite, UINT32 orderInLayer)
	{
		_uiQueue.emplace_back(orderInLayer, sprite);
		//_renderTarget->DrawBitmap();
		//_renderTarget->CreateBitmapFromWicBitmap()
	}

	void Canvas::Impl::RebuildRenderTarget(ID2D1DeviceContext* context)
	{
		if(_canvasRenderType == CanvasRenderMode::Texture)
		{
			if (_renderTargetOrigin == nullptr)
				DXThrowError(E_FAIL, "FontManager :: DXGI RenderTarget CreateFailed. Texture Is Nullptr");

			D3D11Texture* d3dTexture = TYPE_CAST(D3D11Texture*, _renderTargetOrigin);

			//todo:: 여기서 얻은 텍스쳐는 무조건 2D여야 합니다. 에러처리 해줄것
			//todo:: 또한 텍스쳐의 type 또한 B8G8R8A8... 이여야함
			auto& nativeTex = d3dTexture->GetNativeTexture();

			ComPtr<IDXGISurface> buffer;

			HRESULT hr = nativeTex._tex2D->QueryInterface(buffer.GetAddressOf());

			//_canvases
			DXThrowError(hr, "FontManager :: DXGI RenderTarget CreateFailed");

			D2D1_BITMAP_PROPERTIES1  props =
				D2D1::BitmapProperties1(
					D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
					D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
					96,
					96);

			hr = context->CreateBitmapFromDxgiSurface(
				buffer.Get(),
				&props,
				_renderTarget.GetAddressOf());

			DXThrowError(hr, "Canvas Create Error");
		}

	}

	Texture* Canvas::Impl::GetRenderTargetOrigin()
	{
		return _renderTargetOrigin;
	}

	///////////////////////////////////////////////////////
	struct FontEngine::Impl
	{
		struct Bitmap
		{
			ID3D11Texture2D* _originalSource;

			ComPtr<ID2D1Bitmap1> _bitmap;
		};

	public:
		Impl(IDXGIDevice* dxgiDevice, IDXGISwapChain* d3dSwapchain, HWND handle);

		~Impl();

	public:
		Font* CreateFontFromTTF(const std::wstring& fontName);

		Font* GetFont(const std::wstring& fontName);

		Canvas* CreateCanvas(const std::wstring& canvasName, CanvasRenderMode rendertype, Texture* const texture);

		bool DeleteCanvas(const std::wstring& canvasName);

		void UnloadBackBuffer();

		void CreateBackbuffer();

		void DrawCanvas(ICanvas* canvas);

		void Excute();

		Bitmap RegistSprite(Texture* texture);

		void DeleteSprite(Texture* texture);

		void CreateColorEffectImage(Bitmap* bitmap, const DUOLMath::Vector4& color);

		void RebuildCanvases();

	private:
		void CreateFactory(IDXGIDevice* dxgiDevice);

		void DrawTexts(TextBox* text);

		void DrawSprite(Sprite* sprite);

		void SetTransform(float angle, DUOLMath::Vector2& rotation, DUOLMath::Vector2& scale, DUOLMath::Vector2& translation, DUOLMath::Vector2& pivot);

		void SetTransform(float angle, DUOLMath::Vector2& rotation, DUOLMath::Vector2& scale, DUOLMath::Vector2& translation, DUOLMath::Vector2& pivot, DUOLMath::Vector2& extraScale, DUOLMath
		                  ::Vector2& extraCenter);

		void CreateEffect();

	private:
		ComPtr<IDWriteFactory5> _writeFactory;

		ComPtr<IDWriteFontSetBuilder1> _fontSetBuilder;

		ComPtr<ID2D1Factory1> _d2dFactory;

		ComPtr<ID2D1Device> _d2dDevice;

		ComPtr<ID2D1DeviceContext> _d2dDeviceContext;

		ComPtr<ID2D1Effect> _colorEffect;

		std::unique_ptr<Canvas> _backbufferRenderTarget;

		std::unordered_map<long, std::unique_ptr<Brush>> _colorBrushes;

		HWND _handle;

		IDXGISwapChain* _d3dSwapchain;

		std::unordered_map<std::wstring, std::unique_ptr<Font>> _fonts;

		std::unordered_map<std::wstring, std::unique_ptr<Canvas>> _canvases;

		std::unordered_map<Texture*, Bitmap> _sprites;

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
		_backbufferRenderTarget = std::make_unique<Canvas>(_d2dDeviceContext.Get(), nullptr, buffer.Get(), CanvasRenderMode::Texture);

		buffer.Reset();

		CreateEffect();
	}

	FontEngine::Impl::~Impl()
	{
		_backbufferRenderTarget.reset();

		_fonts.clear();
		_canvases.clear();

		_d2dFactory.Reset();
		_fontSetBuilder.Reset();
		_writeFactory.Reset();

		_d2dDeviceContext->Flush();
		_d2dDeviceContext.Reset();
		_d2dDevice.Reset();
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

			if (hr != S_OK)
			{
				DXThrowError(hr, "Font를 찾지 못했습니다.");

				return nullptr;
			}

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
				if (texture == nullptr)
					DXThrowError(E_FAIL, "FontManager :: DXGI RenderTarget CreateFailed. Texture Is Nullptr");

				D3D11Texture* d3dTexture = TYPE_CAST(D3D11Texture*, texture);

				//todo:: 여기서 얻은 텍스쳐는 무조건 2D여야 합니다. 에러처리 해줄것
				//todo:: 또한 텍스쳐의 type 또한 B8G8R8A8... 이여야함
				auto& nativeTex = d3dTexture->GetNativeTexture();

				ComPtr<IDXGISurface> buffer;

				HRESULT hr = nativeTex._tex2D->QueryInterface(buffer.GetAddressOf());

				//_canvases
				DXThrowError(hr, "FontManager :: DXGI RenderTarget CreateFailed");
				auto newCanvas = std::make_unique<Canvas>(_d2dDeviceContext.Get(), texture, buffer.Get(), rendertype);

				auto rawPtr = newCanvas.get();

				_canvases.emplace(canvasName, std::move(newCanvas));

				return rawPtr;
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

	bool FontEngine::Impl::DeleteCanvas(const std::wstring& canvasName)
	{
		_canvases.erase(canvasName);
		return true;
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
#if defined(DEBUG) || defined(_DEBUG)
		D2D1_FACTORY_OPTIONS options;
		options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, _d2dFactory.GetAddressOf());
#else
		hr = D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			_d2dFactory.GetAddressOf()
		);
#endif

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


		//DUOLMath::Vector2 pivotPos = DUOLMath::Vector2{
		//	(offset.x * sprite->_rect.left + offset.x * sprite->_rect.right) / 2.f
		//	,(offset.y * sprite->_rect.top + offset.y * sprite->_rect.bottom) / 2.f };

		//SetTransform(text->_angle, text->_rotationXY, text->_scale, text->_translation,);

		Font* font = static_cast<Font*>(text->_fontType);

		D2D1_RECT_F rectSize;

		float pivotWidth = (text->_rect.right - text->_rect.left) * text->_pivot.x - (text->_rect.right - text->_rect.left) / 2;
		float pivotHeight = (text->_rect.bottom - text->_rect.top) * text->_pivot.y - (text->_rect.bottom - text->_rect.top) / 2;

		rectSize.left = text->_rect.left + pivotWidth;
		rectSize.top = text->_rect.top + pivotHeight;
		rectSize.right = text->_rect.right + pivotWidth;
		rectSize.bottom = text->_rect.bottom + pivotHeight;

		DUOLMath::Vector2 pivotPos = DUOLMath::Vector2{
			(text->_rect.left + (text->_rect.right - text->_rect.left) / 2)
			,(text->_rect.top + (text->_rect.bottom - text->_rect.top) / 2) };

		SetTransform(text->_angle, text->_rotationXY, text->_scale, text->_translation, pivotPos);

		_d2dDeviceContext->DrawTextW(text->_text.c_str(), text->_text.size(), font->GetIDWriteTextFormat(text->_fontSize, text->_weightOption, text->_styleOption), &rectSize, brush, D2D1_DRAW_TEXT_OPTIONS_CLIP);
	}

	void FontEngine::Impl::DrawSprite(Sprite* sprite)
	{
		auto spriteTexture = sprite->_texture;

		if (spriteTexture == nullptr)
			return;

		auto foundImage = _sprites.find(spriteTexture);

		Bitmap currentBitmap;

		if (foundImage == _sprites.end())
		{
			currentBitmap = RegistSprite(spriteTexture);
		}
		else
		{
			//bitmap check. Resize 여부를 포인터로 확인한다.
			//같은 포인터일시는 그냥 그대로 쓰고, 다르다면 다시만들어줌.
			D3D11Texture* d3dtexture = TYPE_CAST(D3D11Texture*, spriteTexture);

			if (d3dtexture->GetNativeTexture()._tex2D.Get() != foundImage->second._originalSource)
			{
				RegistSprite(spriteTexture);
			}

			currentBitmap = foundImage->second;
		}

		D2D1_RECT_F rectSize;

		float pivotWidth = (sprite->_rect.right - sprite->_rect.left) * sprite->_pivot.x - (sprite->_rect.right - sprite->_rect.left) / 2;
		float pivotHeight = (sprite->_rect.bottom - sprite->_rect.top) * sprite->_pivot.y - (sprite->_rect.bottom - sprite->_rect.top) / 2;

		rectSize.left = sprite->_rect.left + pivotWidth;
		rectSize.top = sprite->_rect.top + pivotHeight;
		rectSize.right = sprite->_rect.right + pivotWidth;
		rectSize.bottom = sprite->_rect.bottom + pivotHeight;

		float scaledWidth = sprite->_rect.right - sprite->_rect.left;
		float scaledHeight = sprite->_rect.bottom - sprite->_rect.top;

		//_d2dDeviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		auto textureExtents = sprite->_texture->GetTextureDesc()._textureExtent;
		D2D1_RECT_F sourceRect;

		sourceRect.left = textureExtents.x * sprite->_imagePivotLeftTop.x;
		sourceRect.top  = textureExtents.y * sprite->_imagePivotLeftTop.y;
		sourceRect.bottom = textureExtents.y * sprite->_imagePivotRightBottom.y;
		sourceRect.right = textureExtents.x * sprite->_imagePivotRightBottom.x;

		if (sprite->_color == DUOLMath::Vector4(1.f, 1.f, 1.f, 1.f))
		{
			DUOLMath::Vector2 pivotPos = DUOLMath::Vector2{
		(sprite->_rect.left + (scaledWidth) / 2)
		,(sprite->_rect.top + (scaledHeight) / 2) };

			SetTransform(sprite->_angle, sprite->_rotationXY, sprite->_scale, sprite->_translation, pivotPos);
			_d2dDeviceContext->DrawBitmap(currentBitmap._bitmap.Get(), &rectSize, 1, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &sourceRect);
		}
		else
		{
			DUOLMath::Vector2 pivotPos = DUOLMath::Vector2{
		(sprite->_rect.left + (scaledWidth) / 2)
			,(sprite->_rect.top + (scaledHeight) / 2) };

			//D2D1_POINT_2F offset{ pivotPos.x , pivotPos.y };
			D2D1_POINT_2F offset{ sprite->_rect.left, sprite->_rect.top };

			auto& textureExtend = spriteTexture->GetTextureDesc()._textureExtent;

			DUOLMath::Vector2 extraScl{ 1.f, 1.f };
			extraScl = DUOLMath::Vector2{ scaledWidth / textureExtend.x , scaledHeight / textureExtend.y };
			DUOLMath::Vector2 extraCenter{
			(sprite->_rect.left )
				,(sprite->_rect.top)};

			SetTransform(sprite->_angle, sprite->_rotationXY, sprite->_scale, sprite->_translation, pivotPos, extraScl, extraCenter);
			CreateColorEffectImage(&currentBitmap, sprite->_color);

			_d2dDeviceContext->DrawImage(_colorEffect.Get(), &offset, &sourceRect);
		}
	}

	void FontEngine::Impl::SetTransform(float angle, DUOLMath::Vector2& rotation, DUOLMath::Vector2& scale,
		DUOLMath::Vector2& translation, DUOLMath::Vector2& pivot)
	{
		const D2D1::Matrix3x2F scl = D2D1::Matrix3x2F::Scale(scale.x, scale.y, { pivot.x, pivot.y });
		const D2D1::Matrix3x2F skew = D2D1::Matrix3x2F::Skew(rotation.x, rotation.y, { pivot.x, pivot.y });
		const D2D1::Matrix3x2F rot = D2D1::Matrix3x2F::Rotation(angle, { pivot.x, pivot.y });
		const D2D1::Matrix3x2F trans = D2D1::Matrix3x2F::Translation(translation.x, translation.y);

		_d2dDeviceContext->SetTransform(scl * skew * rot * trans);
	}

	void FontEngine::Impl::SetTransform(float angle, DUOLMath::Vector2& rotation, DUOLMath::Vector2& scale,
	                                    DUOLMath::Vector2& translation, DUOLMath::Vector2& pivot, DUOLMath::Vector2& extraScale, DUOLMath::Vector2& extraCenter)
	{
		const D2D1::Matrix3x2F scl = D2D1::Matrix3x2F::Scale(scale.x, scale.y, { pivot.x, pivot.y });
		const D2D1::Matrix3x2F skew = D2D1::Matrix3x2F::Skew(rotation.x, rotation.y, { pivot.x, pivot.y });
		const D2D1::Matrix3x2F rot = D2D1::Matrix3x2F::Rotation(angle, { pivot.x, pivot.y });
		const D2D1::Matrix3x2F trans = D2D1::Matrix3x2F::Translation(translation.x, translation.y);
		const D2D1::Matrix3x2F extraScl = D2D1::Matrix3x2F::Scale(extraScale.x, extraScale.y, { extraCenter.x, extraCenter.y});

		_d2dDeviceContext->SetTransform(/* **/ scl * skew * rot * trans * extraScl);
	}

	void FontEngine::Impl::CreateEffect()
	{
		_d2dDeviceContext->CreateEffect(CLSID_D2D1ColorMatrix, _colorEffect.GetAddressOf());
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
			//auto debug = castedCanvas->GetRenderTarget()->GetPixelFormat();
			//D2D1_COLOR_F ;
			//_d2dDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::Beige));
			_d2dDeviceContext->Clear();

			auto& uiQueue = castedCanvas->GetUIQueue();
			std::sort(uiQueue.begin(), uiQueue.end());
			//int uiCount = uiQueue.GetVector().size();

			//for (int uiIdx = 0; uiIdx < uiCount; uiIdx++)
			//{
			//	auto& ui = uiQueue.top();

			//	if (ui.second->_resourceType == IResource::ResourceType::Text)
			//	{
			//		DrawTexts(static_cast<TextBox*>(ui.second));
			//	}
			//	else if (ui.second->_resourceType == IResource::ResourceType::Sprite)
			//	{
			//		DrawSprite(static_cast<Sprite*>(ui.second));
			//	}

			//	uiQueue.pop();
			//}

			//uiQueue.Clear();

			int uiCount = uiQueue.size();

			for (auto& ui : uiQueue)
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

			uiQueue.clear();
		}

		hr = _d2dDeviceContext->EndDraw();
		_canvasQueue.clear();
	}

	FontEngine::Impl::Bitmap FontEngine::Impl::RegistSprite(Texture* texture)
	{
		auto foundImage = _sprites.find(texture);

		D3D11Texture* d3dtexture = TYPE_CAST(D3D11Texture*, texture);

		//todo::예외처리가 되지않았습니다.2d인경우만 리소스화 가능합니다.
		auto& d3dtexture2d = d3dtexture->GetNativeTexture()._resource;

		ComPtr<IDXGISurface> buffer;

		HRESULT hr = d3dtexture2d->QueryInterface(buffer.GetAddressOf());

		ComPtr<ID2D1Bitmap1> d2dBmp;

		D2D1_BITMAP_PROPERTIES1  props =
			D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_NONE,
				D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
				96,
				96);

		if (foundImage != _sprites.end())
		{
			hr = _d2dDeviceContext->CreateBitmapFromDxgiSurface(buffer.Get(), &props, d2dBmp.GetAddressOf());

			foundImage->second._originalSource = d3dtexture->GetNativeTexture()._tex2D.Get();

			foundImage->second._bitmap.Reset();
			foundImage->second._bitmap = d2dBmp;

			return foundImage->second;
		}
		else
		{

			hr = _d2dDeviceContext->CreateBitmapFromDxgiSurface(buffer.Get(), &props, d2dBmp.GetAddressOf());

			Bitmap bitmap;

			bitmap._originalSource = d3dtexture->GetNativeTexture()._tex2D.Get();
			bitmap._bitmap = d2dBmp;

			_sprites.emplace(texture, bitmap);

			return bitmap;
		}
	}

	void FontEngine::Impl::DeleteSprite(Texture* texture)
	{
		_sprites.erase(texture);
	}

	void FontEngine::Impl::CreateColorEffectImage(Bitmap* bitmap, const DUOLMath::Vector4& color)
	{
		_colorEffect->SetInput(0, bitmap->_bitmap.Get());

		D2D1_MATRIX_5X4_F matrix = D2D1::Matrix5x4F(
			color.x, 0, 0, 0,
			0, color.y, 0, 0,
			0, 0, color.z, 0,
			0, 0, 0, color.w,
			0, 0, 0, 0
		);

		_colorEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, matrix);
	}

	void FontEngine::Impl::RebuildCanvases()
	{
		for(auto& canvas : _canvases)
		{
			canvas.second->RebuildRenderTarget(_d2dDeviceContext.Get());
		}
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
		_pImpl->DeleteCanvas(canvasName);

		return true;
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

	void FontEngine::RebuildCanvases()
	{
		_pImpl->RebuildCanvases();
	}
}
