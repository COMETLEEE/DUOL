#pragma once
#define Mutil_Render_Count 7


class ID3D11RenderTargetView;

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
		MatAmbient,
		ShadowMap
	};
	class RenderTexture;

	class DeferredRenderPass;

	class TextureRenderPass;

	class RenderTarget
	{
	public:
		RenderTarget();

		~RenderTarget();

	private:
		DeferredRenderPass* _deferredRenderPass;

		TextureRenderPass* _textureRenderPass;

		RenderTexture* _deferredTexture;

		ID3D11RenderTargetView* _deferredRenderTargetView;

		ID3D11RenderTargetView* _renderTargetView;
		// 렌더 타겟 뷰 , 렌더 대상, 그림을 그릴 곳, 화면에 보이는 것은 아님, 백 버퍼로 사용할거얌~.
		// 최종 결과물 보여주는 용으로 사용하자~
		// 뷰포트를 통해 보여줌

		// Multi Render Target을 위한 변수들!!
		// Texture 버퍼 생성을 위한 클래스.
		static RenderTexture* _renderTexture[Mutil_Render_Count]; // 다른 패스에서 필요 할 수도 있으니 static으로 만들자..
		// Shader에 넘겨줄 때 배열로 넘겨줘야한다.
		ID3D11RenderTargetView* _textureRenderTargetView[Mutil_Render_Count];
		// 화면에 출력하기 위한 클래스

	public:
		void BeginRender();

		void ExecuteRender();

		void OnResize();

		void SetRenderTargetView(int _Num);

		static RenderTexture** GetRenderTexture() { return _renderTexture; }

		void PopShaderResource();

	private:
		void ClearRenderTarget(); // 모든 렌더 타겟 지우기

		void CreateDeferredTexture();

		void SetBackBufferRenderTarget(); // 텍스처를 그리는 버퍼에서 백버퍼로 전환.

		void RenderDebugWindow(); // 디퍼드 렌더링 디버그 화면 출력.

		void RenderDeferredWindow(); // 디퍼드 렌더링 된 화면을 출력

		void Release();
	};
}

