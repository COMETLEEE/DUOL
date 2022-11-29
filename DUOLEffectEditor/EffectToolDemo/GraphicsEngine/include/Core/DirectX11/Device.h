#pragma once
#include <Windows.h>

class ID3D11Device;

class ID3D11DeviceContext;

class IDXGISwapChain;

namespace MuscleGrapics
{
	class Device
	{
	public:
		Device() = default;
		~Device();
	private:

		UINT _4xMsaaQuality; //msaa 확인용

		bool _enable4xMsaa = false;

		ID3D11Device* _d3dDevice; // 디바이스

		ID3D11DeviceContext* _d3dImmediateContext; // 디바이스 컨텍스트

		IDXGISwapChain* _swapChain; // 스왑체인(교환사슬)
		// 이중 버퍼링을 위한 스왑체인. 전면 버퍼의 포인터를 가지고 있으며 Present 함수로 전면과 후면 버퍼를 교체.
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
