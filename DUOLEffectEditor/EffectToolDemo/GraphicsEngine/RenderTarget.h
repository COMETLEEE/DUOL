#pragma once
#define Mutil_Render_Count 7

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
class RenderTarget
{
public:
	RenderTarget();

	~RenderTarget();

private:
	Display* m_DeferredWindow;
	RenderTexture* m_DeferredTexture;
	ID3D11RenderTargetView* m_DeferredRenderTargetView;

	ID3D11RenderTargetView* m_RenderTargetView;
	// 렌더 타겟 뷰 , 렌더 대상, 그림을 그릴 곳, 화면에 보이는 것은 아님, 백 버퍼로 사용할거얌~.
	// 최종 결과물 보여주는 용으로 사용하자~
	// 뷰포트를 통해 보여줌


	// Multi Render Target을 위한 변수들!!
	// Texture 버퍼 생성을 위한 클래스.
	static RenderTexture* m_RederTexture[Mutil_Render_Count]; // 다른 패스에서 필요 할 수도 있으니 static으로 만들자..
	// Shader에 넘겨줄 때 배열로 넘겨줘야한다.
	ID3D11RenderTargetView* m_TextureRenderTargetView[Mutil_Render_Count];
	// 화면에 출력하기 위한 클래스
	Display* m_DebugWindows[Mutil_Render_Count];

public:
	void BeginRender();
	void EndRender();
	void OnResize();
	void SetRenderTargetView(int _Num);
	static RenderTexture** GetRenderTexture() { return m_RederTexture; }
	void PopShaderResource();

private:
	void ClearRenderTarget(); // 모든 렌더 타겟 지우기
	void CreateDeferredTexture();
	void SetBackBufferRenderTarget(); // 텍스처를 그리는 버퍼에서 백버퍼로 전환.
	void RenderDebugWindow(); // 디퍼드 렌더링 디버그 화면 출력.
	void RenderDeferredWindow(); // 디퍼드 렌더링 된 화면을 출력
	void Release();
};

