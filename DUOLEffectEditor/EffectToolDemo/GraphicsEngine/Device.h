#pragma once
class Device
{
public:
	Device() = default;
	~Device();
private:

	UINT m_4xMsaaQuality; //msaa Ȯ�ο�
	bool m_Enable4xMsaa = false;
	ID3D11Device* m_D3dDevice; // ����̽�
	ID3D11DeviceContext* m_d3dImmediateContext; // ����̽� ���ؽ�Ʈ
	IDXGISwapChain* m_SwapChain; // ����ü��(��ȯ�罽)
	// ���� ���۸��� ���� ����ü��. ���� ������ �����͸� ������ ������ Present �Լ��� ����� �ĸ� ���۸� ��ü.
public:
	void Initialize(HWND hWnd, int Width, int height);
	void Release();

	ID3D11Device* GetDevice() { return m_D3dDevice; }
	ID3D11DeviceContext* GetDeviceContext() { return m_d3dImmediateContext; }
	IDXGISwapChain* GetSwapChain() { return m_SwapChain; }
	bool GetEnable4xMsaa() { return m_Enable4xMsaa; }
	UINT Get4xQuality() { return m_4xMsaaQuality; }
};

