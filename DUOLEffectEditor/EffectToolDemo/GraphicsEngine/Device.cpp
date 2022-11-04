#include "pch.h"
#include "Device.h"

Device::~Device()
{
	Release();
}

void Device::Initialize(HWND hWnd, int Width, int height)
{
	/// 여기부터 초기화 시작입니다
	/// 엔진..

	UINT createDeviceFlags = 0;
#if defined (_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL m_FeatureLevel; // 구성 수준
	//디바이스 인터페이스와 DC 인터페이스를 생성한다.
	HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, 0, 0, D3D11_SDK_VERSION, &m_D3dDevice, &m_FeatureLevel, &m_d3dImmediateContext);
	if (FAILED(hr))
	{
		MessageBox(0, L"D3DllCreateDevice Failed.", 0, 0);
		return;
	}
	if (m_FeatureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return;
	}


	//4X MSAA 품질 수준 지원 점검. 무조건 지원 하는듯.
	hr = m_D3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality);
	assert(m_4xMsaaQuality > 0);

	if (FAILED(hr))
	{
		MessageBox(0, L"4xMassQuality Failed.", 0, 0);
		return;
	}

	// 스왑체인 / 교환 사슬 설정. 더블버퍼.
	// 스왑체인의 특성을 서술하는 구조체를 초기화시킨다.
	DXGI_SWAP_CHAIN_DESC m_Chaindesc;

	//1 . 후면 버퍼의 구조체
	m_Chaindesc.BufferDesc.Width = Width;
	m_Chaindesc.BufferDesc.Height = height;
	m_Chaindesc.BufferDesc.RefreshRate.Numerator = 60;
	m_Chaindesc.BufferDesc.RefreshRate.Denominator = 1;
	m_Chaindesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_Chaindesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	m_Chaindesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 4XMSAA를 사용하는가?
	// 2. 다중 표본화를 위한것.
	if (m_Enable4xMsaa)
	{
		m_Chaindesc.SampleDesc.Count = 4;
		m_Chaindesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	else
	{
		m_Chaindesc.SampleDesc.Count = 1;
		m_Chaindesc.SampleDesc.Quality = 0;
	}

	//3. 버퍼의 용도.
	m_Chaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// 4. 버퍼의 갯수. 일반적으로 후면버퍼를 하나만 사용.
	m_Chaindesc.BufferCount = 1;
	// 5. 렌더링 결과를 표시할 창의 핸들
	m_Chaindesc.OutputWindow = hWnd;
	// 6. 창모드를 원하면 TRUE 전체화면을 원하면 .FALSE
	m_Chaindesc.Windowed = true;
	// 7. 교환 효과를 서술하는 구조체
	m_Chaindesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	// 8. 추가적인 플래그
	m_Chaindesc.Flags = 0;

	//교환 사슬 생성
	IDXGIDevice* dxgiDevice = 0;
	HR(m_D3dDevice->QueryInterface(__uuidof(IDXGIDevice),
		(void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter),
		(void**)&dxgiAdapter));

	// 드디어 IDXGIFactory 인터페이스를 얻게 되었다.
	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory),
		(void**)&dxgiFactory));

	// 이제 교환 사슬을 생성한다. 스왑체인 생성을 위한 과정들..
	HR(dxgiFactory->CreateSwapChain(m_D3dDevice, &m_Chaindesc, &m_SwapChain));

	// 획득한 COM 인터페이스들을 해제한다(다 사용했으므로).
	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();
}

void Device::Release()
{
	ReleaseCOM(m_D3dDevice)
		ReleaseCOM(m_d3dImmediateContext)
		ReleaseCOM(m_SwapChain)
}

