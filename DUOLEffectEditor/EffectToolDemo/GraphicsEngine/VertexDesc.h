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
		float	gDuration;				// 몇 초 동안 파티클 객체가 재생될 지.					

		Vector3 gEmitPosW;
		int		gMaxParticles;				// 파티클 최대 출력 사이즈.							

		float	gStartDelay[2];				// 몇 초 뒤에 파티클이 재생될 지.					
		float	gStartLifeTime[2];				// 한 파티클의 생존 시간.						

		float	gStartSpeed[2];				// 파티클 생성시 시작 속도.
		float	gStartRotation[2];			// 파티클의 시작 회전.							

		float gStartSize[4];				// 파티클의 시작 크기.							

		Vector4 gStartColor[2];			// 파티클의 시작 색상									

		float	gGravityModifier[2];			// 파티클에 가해지는 중력.						
		float gParticlePlayTime;
		float	pad4;			// 파티클에 가해지는 중력.						

		int	gisLooping;					// 반복여부.
		int	pad2[3];					// 시작인가요 ..?						
	};

	__declspec(align(16)) struct Emission
	{
		float	gEmissiveCount;			// 한번에 몇개를 방출 시킬지.
		float	gEmissiveTime;			// 다음 방출까지 걸리는 시간.
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
	 * \brief 오브젝트마다 공통되는 contant 버퍼 구조체, 수정할 때 항상 쉐이더 코드도 같이 수정하자. 16 바이트 정렬 잊지말자.
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

		Matrix gCurrentViewProj; // 블러를 위한 것! 엥 필요없었네?

		Matrix gPrevViewProj; // 블러를 위한 것!
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
		Vector3 gCameraPosW; // 카메라의 좌표
		float	pad999;

		Vector2 gScreenXY;
		float	gTimeStep; // 1프레임당 시간
		float	pad2;

		Matrix	gViewProj;
	};
}

