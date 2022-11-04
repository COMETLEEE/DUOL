#pragma once
class Device
{
public:
	Device() = default;
	~Device();
private:

	UINT m_4xMsaaQuality; //msaa 확인용
	bool m_Enable4xMsaa = false;
	ID3D11Device* m_D3dDevice; // 디바이스
	ID3D11DeviceContext* m_d3dImmediateContext; // 디바이스 컨텍스트
	IDXGISwapChain* m_SwapChain; // 스왑체인(교환사슬)
	// 이중 버퍼링을 위한 스왑체인. 전면 버퍼의 포인터를 가지고 있으며 Present 함수로 전면과 후면 버퍼를 교체.
public:
	void Initialize(HWND hWnd, int Width, int height);
	void Release();

	ID3D11Device* GetDevice() { return m_D3dDevice; }
	ID3D11DeviceContext* GetDeviceContext() { return m_d3dImmediateContext; }
	IDXGISwapChain* GetSwapChain() { return m_SwapChain; }
	bool GetEnable4xMsaa() { return m_Enable4xMsaa; }
	UINT Get4xQuality() { return m_4xMsaaQuality; }
};

