#pragma once
namespace MuscleGrapics
{
	class Depth
	{
	public:
		Depth();

		~Depth();
	private:
		// ���� ���۴� �̹��� �ڷḦ ���� �ʴ� �ؽ�ó�� ��.
		// ���� ���۴� �� �ȼ��� ���� ������ ��´�.
		ID3D11Texture2D* _depthStencilBuffer; // ���� ���ٽ� ���� , �����ϴ� ��.

		ID3D11DepthStencilView* _depthStencilView; // ���� ���ٽ� �� , �׸��� ��.

		ID3D11DepthStencilState* _depthStencilState; // ���� ���ٽ� ������Ʈ // ��� �׸���.
	public:
		void Clear();

		void OnResize();

		void Finalize();

		void OMsetDepth();

		ID3D11DepthStencilView* GetDeptStencilView();
	};
}
