#pragma once
namespace MuscleGrapics
{
	class Device
	{
	public:
		Device() = default;
		~Device();
	private:

		UINT _4xMsaaQuality; //msaa Ȯ�ο�

		bool _enable4xMsaa = false;

		ID3D11Device* _d3dDevice; // ����̽�

		ID3D11DeviceContext* _d3dImmediateContext; // ����̽� ���ؽ�Ʈ

		IDXGISwapChain* _swapChain; // ����ü��(��ȯ�罽)
		// ���� ���۸��� ���� ����ü��. ���� ������ �����͸� ������ ������ Present �Լ��� ����� �ĸ� ���۸� ��ü.
	public:
		void Initialize(HWND hWnd, int Width, int height);

		void Release();

		ID3D11Device* GetDevice() { return _d3dDevice; }

		ID3D11DeviceContext* GetDeviceContext() { return _d3dImmediateContext; }

		IDXGISwapChain* GetSwapChain() { return _swapChain; }

		bool GetEnable4xMsaa() { return _enable4xMsaa; }

		UINT Get4xQuality() { return _4xMsaaQuality; }
	};
}
