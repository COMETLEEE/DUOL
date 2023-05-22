/**

	@file      OrderIndependentTransparency.h
	@brief     OIT 구현관련 클래스
	@details   ~ 처음에는 OIT 관리를 위한 클래스로 설계했지만, 시간이 지날수록 파티클 전체를 관리하는 클래스가 되고있다..!
	@author    SinSeongHyeon
	@date      10.01.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/

#pragma once
#include <d3d11.h>
#include <memory>
#include <queue>
#include <vector>

class ID3D11DepthStencilState;
class ID3D11ShaderResourceView;
class ID3D11RenderTargetView;
class ID3D11DepthStencilView;

constexpr int g_layerCount = 10;

namespace MuscleGrapics
{
	struct RenderingData_Particle;
	struct RenderingData_3D;
	class DXEngine;
	class RenderTexture;

	class OrderIndependentTransparency
	{
	public:
		OrderIndependentTransparency() = default;

		~OrderIndependentTransparency();

	private:
		DXEngine* _dxEngine; // 캐싱/

		ID3D11DeviceContext* _d3dContext; // 캐싱

		std::queue<std::shared_ptr<RenderingData_Particle>> _renderQueueParticle;

		std::queue<std::shared_ptr<RenderingData_3D>> _renderQueue3D;
	private:
		void Clear();

		void Draw(); // 레이어가 완성이 됐을 때..!


	public:
		void RegistRenderingData(std::queue<std::shared_ptr<RenderingData_3D>>& renderQueue_3D);  // 이런 식으로 같은 함수를 여러개 만들고 싶지 않은데...
		// 템플릿을 사용하자니 헤더에 정의를 해야되고, 상속을 위한 다형성을 사용하자니 구조를 많이 바꿔야한다.
		// 구조를 바꾸는게 맞는 것 같지만, 일단은 구현을 하자.
		// 만약 구조를 바꾼다면 RenderingData_3D,Particle 의 부모 클래스를 만들어서 다형적으로 사용해야 될 것 같다.
		// todo :

		void RegistRenderingData(std::queue<std::shared_ptr<RenderingData_Particle>>& renderQueueParticle);  // Layer를 만드는 함수.

		void CreateLayer();

		void MergeLayer(); // 레이어를 그리는 함수. 레이어를 먼저 만들고 호출 해야 한다.

		void PostProcessing();

		void Finalize();

		void OnResize();

		void SetRTV_OITLayerCreate();

		void BindingResource_SRV();

		void UnBindingResource_UAV();

		static OrderIndependentTransparency& Get();

	private:
		// 캐싱해서 사용한다.
		RenderTexture* _OIT_PixelData_Over_Color_Texture;
		RenderTexture* _OIT_PixelData_Over_Info_Texture;
		RenderTexture* _OIT_PixelData_Additive_Color_Texture;
		RenderTexture* _OIT_PixelData_Additive_Info_Texture;
	};




}

