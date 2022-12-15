#pragma once
#define Mutil_Render_Count 6


class ID3D11RenderTargetView;

class ID3D11DepthStencilView;

namespace MuscleGrapics
{
	enum class MutilRenderTexture
	{
		Depth,
		Normal,
		Position,
		Albedo,
		MatDiffuse,
		MatSpecular,
		ObjectID
	};
	class RenderTexture;

	class DeferredRenderPass;

	class TextureRenderPass;

	class ObjectIDTexture;

	class RenderTarget
	{
	public:
		RenderTarget();

		~RenderTarget();

	private:
		DeferredRenderPass* _deferredRenderPass;

		TextureRenderPass* _textureRenderPass;

		RenderTexture* _deferredTexture;

		ID3D11RenderTargetView* _deferredRenderTargetView; // 디퍼드 조립을 위한 뷰

		ID3D11RenderTargetView* _renderTargetView;
		// 렌더 타겟 뷰 , 렌더 대상, 그림을 그릴 곳, 화면에 보이는 것은 아님, 백 버퍼로 사용할거얌~.
		// 최종 결과물 보여주는 용으로 사용하자~
		// 뷰포트를 통해 보여줌

		// Multi Render Target을 위한 변수들!!
		// Texture 버퍼 생성을 위한 클래스.
		static RenderTexture* _renderTexture[Mutil_Render_Count + 1]; // 다른 패스에서 필요 할 수도 있으니 static으로 만들자..
		// 화면에 출력하기 위한 클래스
		static ObjectIDTexture* _objectIDTxture;
	public:
		void BeginRender();

		void ExecuteRender();

		void OnResize();

		// 바인딩할 렌더 타겟의 갯수와 뷰를 넘긴다.
		void SetRenderTargetView(ID3D11DepthStencilView* _depthStencilView, int _num, ...);

		static RenderTexture** GetRenderTexture() { return _renderTexture; }

		ID3D11RenderTargetView* GetRenderTargetView(); // 백 버퍼.

		void PopShaderResource();

		unsigned int PickObjectID(int x, int y);

	private:
		void ClearRenderTarget(); // 모든 렌더 타겟 지우기

		void CreateDeferredTexture();

		void RenderDebugWindow(); // 디퍼드 렌더링 디버그 화면 출력.

		void RenderDeferredWindow(); // 디퍼드 렌더링 된 화면을 출력

		void Release();
	};
}

