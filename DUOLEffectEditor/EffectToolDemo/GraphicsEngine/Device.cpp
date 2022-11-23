#include "pch.h"
#include "Device.h"
namespace MuscleGrapics
{
	Device::~Device()
	{
		Release();
	}
	void Device::Initialize(HWND hWnd, int Width, int height)
	{
		UINT createDeviceFlags = 0;

#if defined (_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL m_FeatureLevel; // ���� ����
		//����̽� �������̽��� DC �������̽��� �����Ѵ�.
		HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, 0, 0, D3D11_SDK_VERSION, &_d3dDevice, &m_FeatureLevel, &_d3dImmediateContext);

		if (FAILED(hr))
			MessageBox(0, L"D3DllCreateDevice Failed.", 0, 0);

		if (m_FeatureLevel != D3D_FEATURE_LEVEL_11_0)
			MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);

		//4X MSAA ǰ�� ���� ���� ����.
		hr = _d3dDevice->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM, 4, &_4xMsaaQuality);

		assert(_4xMsaaQuality > 0);

		if (FAILED(hr))
			MessageBox(0, L"4xMassQuality Failed.", 0, 0);

		// ����ü�� / ��ȯ �罽 ����. �������.
		// ����ü���� Ư���� �����ϴ� ����ü�� �ʱ�ȭ��Ų��.
		DXGI_SWAP_CHAIN_DESC m_Chaindesc;
		//1 . �ĸ� ������ ����ü
		m_Chaindesc.BufferDesc.Width = Width;
		m_Chaindesc.BufferDesc.Height = height;
		m_Chaindesc.BufferDesc.RefreshRate.Numerator = 60;
		m_Chaindesc.BufferDesc.RefreshRate.Denominator = 1;
		m_Chaindesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_Chaindesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		m_Chaindesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		// 2. ���� ǥ��ȭ�� ���Ѱ�.
		if (_enable4xMsaa)
		{
			m_Chaindesc.SampleDesc.Count = 4;
			m_Chaindesc.SampleDesc.Quality = _4xMsaaQuality - 1;
		}
		else
		{
			m_Chaindesc.SampleDesc.Count = 1;
			m_Chaindesc.SampleDesc.Quality = 0;
		}
		//3. ������ �뵵.
		m_Chaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		// 4. ������ ����. �Ϲ������� �ĸ���۸� �ϳ��� ���.
		m_Chaindesc.BufferCount = 1;
		// 5. ������ ����� ǥ���� â�� �ڵ�
		m_Chaindesc.OutputWindow = hWnd;
		// 6. â��带 ���ϸ� TRUE ��üȭ���� ���ϸ� .FALSE
		m_Chaindesc.Windowed = true;
		// 7. ��ȯ ȿ���� �����ϴ� ����ü
		m_Chaindesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		// 8. �߰����� �÷���
		m_Chaindesc.Flags = 0;
		//��ȯ �罽 ����
		IDXGIDevice* dxgiDevice = 0;
		HR(_d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));
		IDXGIAdapter* dxgiAdapter = 0;
		HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));
		// ���� IDXGIFactory �������̽��� ��� �Ǿ���.
		IDXGIFactory* dxgiFactory = 0;
		HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));
		// ���� ��ȯ �罽�� �����Ѵ�. ����ü�� ������ ���� ������..
		HR(dxgiFactory->CreateSwapChain(_d3dDevice, &m_Chaindesc, &_swapChain));
		// ȹ���� COM �������̽����� �����Ѵ�(�� ��������Ƿ�).

		dxgiDevice->Release();

		dxgiAdapter->Release();

		dxgiFactory->Release();
	}
	void Device::Release()
	{
		ReleaseCOM(_d3dDevice);

		ReleaseCOM(_d3dImmediateContext);

		ReleaseCOM(_swapChain);
	}
}

