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
	 * \brief 오브젝트마다 공통되는 contant 버퍼 구조체, 수정할 때 항상 쉐이더 코드도 같이 수정하자. 16 바이트 정렬 잊지말자.
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
		Matrix gCurrentViewProj; // 블러를 위한 것! 엥 필요없었네?
		Matrix gPrevViewProj; // 블러를 위한 것!
	};

	struct CB_PerObject_Particle
	{
		Vector3 gEmitPosW; // 방출기 좌표
		float gStartSpeed; // 파티클 출발 속도.

		Vector3 gEmitDirW; // 방출기 방향
		float gLifeTime; // 파티클이 얼마나 살아있을까?

		Vector2 gStartSize;
		UINT gEmissiveCount; // 한번에 방출하는 파티클의 수.
		float gEmissiveTime; // 방출 주기

		Vector3 gAccelW = { 0.0f, 7.8f, 0.0f }; // 가속도
		float pad1;
	};

	struct CB_PerFream_Particle
	{
		Vector3 gCameraPosW; // 카메라의 좌표
		float gGameTime; // 게임 시간

		Vector2 gScreenXY;
		float gTimeStep; // 1프레임당 시간
		float pad2;

		Matrix gViewProj;
	};
}

