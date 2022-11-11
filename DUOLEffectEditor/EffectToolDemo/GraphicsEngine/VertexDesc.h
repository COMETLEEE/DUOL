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

	static constexpr UINT BasicParticleVertexSize = 5;
	static D3D11_INPUT_ELEMENT_DESC BasicParticleVertex[BasicParticleVertexSize] =
	{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"SIZE",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"AGE",      0, DXGI_FORMAT_R32_FLOAT,       0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TYPE",     0, DXGI_FORMAT_R32_UINT,        0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};



}

namespace ConstantBuffDesc
{


	/**
	 * \brief ������Ʈ���� ����Ǵ� contant ���� ����ü, ������ �� �׻� ���̴� �ڵ嵵 ���� ��������. 16 ����Ʈ ���� ��������.
	 */
	struct CB_PerObject
	{
		XMMATRIX worldViewProj;
		XMMATRIX gWorld;
		XMMATRIX gWorldInvTranspose;
	};

	struct CB_PerFream
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

	struct CB_PerObject_Particle
	{
		Vector3 gEmitPosW; // ����� ��ǥ
		float gStartSpeed; // ��ƼŬ ��� �ӵ�.

		Vector3 gEmitDirW; // ����� ����
		float gLifeTime; // ��ƼŬ�� �󸶳� ���������?

		Vector2 gStartSize;
		UINT gEmissiveCount; // �ѹ��� �����ϴ� ��ƼŬ�� ��.
		float gEmissiveTime; // ���� �ֱ�

		Vector3 gAccelW = { 0.0f, 7.8f, 0.0f }; // ���ӵ�
		float pad1;
	};

	struct CB_PerFream_Particle
	{
		Vector3 gCameraPosW; // ī�޶��� ��ǥ
		float gGameTime; // ���� �ð�

		Vector2 gScreenXY;
		float gTimeStep; // 1�����Ӵ� �ð�
		float pad2;

		Matrix gViewProj;
	};
}

