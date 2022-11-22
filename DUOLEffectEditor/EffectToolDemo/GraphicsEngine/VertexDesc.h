#pragma once
#include "d3d11.h"

namespace VertexDesc
{
	static constexpr UINT BasicVertexSize = 1;
	static D3D11_INPUT_ELEMENT_DESC BasicVertex[BasicVertexSize] =
	{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	static constexpr UINT DeferredVertexSize = 2;
	static D3D11_INPUT_ELEMENT_DESC DeferredVertexDesc[DeferredVertexSize] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV",    0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	static constexpr UINT BasicLightVertexSize = 7;
	static D3D11_INPUT_ELEMENT_DESC BasicLightVertex[BasicLightVertexSize] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"WEIGHT",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEINDEX",    0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 60,
			D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 72,
			D3D11_INPUT_PER_VERTEX_DATA, 0}

	};

	static constexpr UINT BasicParticleVertexSize = 6;
	static D3D11_INPUT_ELEMENT_DESC BasicParticleVertex[BasicParticleVertexSize] =
	{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"SIZE",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"AGE",      0, DXGI_FORMAT_R32_FLOAT,       0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TYPE",     0, DXGI_FORMAT_R32_UINT,        0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"SV_VertexID",     0, DXGI_FORMAT_R32_UINT,        0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	static constexpr UINT StreamOutParticleVertexSize = 6;
	static D3D11_INPUT_ELEMENT_DESC StreamOutParticleVertex[StreamOutParticleVertexSize] =
	{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"SIZE",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"AGE",      0, DXGI_FORMAT_R32_FLOAT,       0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TYPE",     0, DXGI_FORMAT_R32_UINT,        0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"VERTEXID",     0, DXGI_FORMAT_R32_UINT,        0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};


}

namespace ConstantBuffDesc
{
	__declspec(align(16)) struct CommonInfo
	{
		Vector3 gEmitDirW;
		float	gDuration;				// �� �� ���� ��ƼŬ ��ü�� ����� ��.					

		Vector3 gEmitPosW;
		int		gMaxParticles;				// ��ƼŬ �ִ� ��� ������.							

		float	gStartDelay[2];				// �� �� �ڿ� ��ƼŬ�� ����� ��.					
		float	gStartLifeTime[2];				// �� ��ƼŬ�� ���� �ð�.						

		float	gStartSpeed[2];				// ��ƼŬ ������ ���� �ӵ�.
		float	gStartRotation[2];			// ��ƼŬ�� ���� ȸ��.							

		float gStartSize[4];				// ��ƼŬ�� ���� ũ��.							

		Vector4 gStartColor[2];			// ��ƼŬ�� ���� ����									

		float	gGravityModifier[2];			// ��ƼŬ�� �������� �߷�.						
		float gParticlePlayTime;
		float	pad4;			// ��ƼŬ�� �������� �߷�.						

		int	gisLooping;					// �ݺ�����.
		int	pad2[3];					// �����ΰ��� ..?						
	};

	__declspec(align(16)) struct Emission
	{
		float	gEmissiveCount;			// �ѹ��� ��� ���� ��ų��.
		float	gEmissiveTime;			// ���� ������� �ɸ��� �ð�.
		Vector2 pad5;
	};

	__declspec(align(16)) struct Shape
	{

	};
	__declspec(align(16)) struct Color_over_Lifetime
	{
		Vector4 gStartColor;

		Vector4 gEndColor;
	};

	__declspec(align(16)) struct Velocity_over_Lifetime
	{
		Vector3 gVelocity;
		float pad;
	};
	/**
	 * \brief ������Ʈ���� ����Ǵ� contant ���� ����ü, ������ �� �׻� ���̴� �ڵ嵵 ���� ��������. 16 ����Ʈ ���� ��������.
	 */
	__declspec(align(16)) struct CB_PerObject
	{
		XMMATRIX worldViewProj;

		XMMATRIX gWorld;

		XMMATRIX gWorldInvTranspose;
	};

	__declspec(align(16)) struct CB_PerFream
	{
		DirectionalLight gDirLight;

		PointLight gPointLight[10];

		SpotLight gSpotLight;

		XMFLOAT3 gEyePosW;
		int gPointCount;

		Matrix gLightViewProj;

		Matrix gCurrentViewProj; // ���� ���� ��! �� �ʿ������?

		Matrix gPrevViewProj; // ���� ���� ��!
	};

	__declspec(align(16)) struct CB_PerObject_Particle
	{
		CommonInfo _commonInfo;

		Emission _emission;

		Velocity_over_Lifetime _velocityoverLifetime;

		Color_over_Lifetime _coloroverLifetime;
	};

	__declspec(align(16)) struct CB_PerFream_Particle
	{
		Vector3 gCameraPosW; // ī�޶��� ��ǥ
		float	pad999;

		Vector2 gScreenXY;
		float	gTimeStep; // 1�����Ӵ� �ð�
		float	pad2;

		Matrix	gViewProj;
	};
}

