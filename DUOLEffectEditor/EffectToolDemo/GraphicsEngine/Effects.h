#pragma once
#include "d3dx11Effect.h"	// effect, tech

class Effect
{
public:
	Effect(string _Path);
	virtual ~Effect();
public:
	virtual void WorldViewProjUpdate(XMMATRIX& _World, XMMATRIX& _View, XMMATRIX& _Proj) abstract;
protected:
	//common
	ID3DX11Effect* m_FX; //효과 객체 - 하나 이상의 렌더링 패스들을 캡슐화하며, 각 패스마다 정점 셰이더가 연관된다.
};

class Shader : public Effect
{
public:
	Shader(string _Path);
	~Shader() override;
private:
	//common
	//light Object
	ID3DX11EffectMatrixVariable* m_fxLightWorldViewProj; // 행렬도 이펙트에서 하네? ㅇㅇ GPU로 연산 넘겨줄라공
	ID3DX11EffectMatrixVariable* m_fxWorldViewProj; // 행렬도 이펙트에서 하네? ㅇㅇ GPU로 연산 넘겨줄라공
	ID3DX11EffectMatrixVariable* m_fxWorld;
	ID3DX11EffectMatrixVariable* m_fxWorldInvTranspose;
	ID3DX11EffectMatrixVariable* m_fxBoneTM;
	ID3DX11EffectVariable* m_fxMaterial;

	ID3DX11EffectVectorVariable* m_fxEyePosW;

	//Texture
	ID3DX11EffectShaderResourceVariable* m_fxDiffuseMap;
	ID3DX11EffectShaderResourceVariable* m_fxNormalMap;
	ID3DX11EffectShaderResourceVariable* m_fxCubeMap;
public:
	ID3DX11EffectTechnique* m_TextureLightTech; //테크란 무엇인가? 셰이더 함수를 실행시킬때 필요한것?
	ID3DX11EffectTechnique* m_NormalTech; //테크란 무엇인가? 셰이더 함수를 실행시킬때 필요한것?
	ID3DX11EffectTechnique* m_LightTech; //테크란 무엇인가? 셰이더 함수를 실행시킬때 필요한것?



	virtual void WorldViewProjUpdate(XMMATRIX& _World, XMMATRIX& _View, XMMATRIX& _Proj) override;



	void SetBoneTM(XMMATRIX& M, int size) { if (!m_fxBoneTM) return; m_fxBoneTM->SetMatrixArray(reinterpret_cast<const float*>(&M), 0, size); }
	void SetMaterial(const Material* mat) { if (!m_fxMaterial || !mat) return; m_fxMaterial->SetRawValue(mat, 0, sizeof(Material)); }
	void SetEyePosW(const XMFLOAT3& v) { if (!m_fxEyePosW) return; m_fxEyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetTexure(ID3D11ShaderResourceView* tex) { if (!m_fxDiffuseMap || !tex) return; m_fxDiffuseMap->SetResource(tex); }
	void SetNormal(ID3D11ShaderResourceView* Normal) { if (!m_fxNormalMap || !Normal) return; m_fxNormalMap->SetResource(Normal); }
	void SetCubeMap(ID3D11ShaderResourceView* tex) { if (!m_fxCubeMap || !tex) return; m_fxCubeMap->SetResource(tex); }


private:
	void SetWorldViewProj(XMMATRIX& M) { if (!m_fxWorldViewProj) return; m_fxWorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(XMMATRIX& M) { if (!m_fxWorld) return; m_fxWorld->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(XMMATRIX& M) { if (!m_fxWorldInvTranspose) return; m_fxWorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
public:
	/// <summary>
	/// 테스트용 그림자.
	/// </summary>
	/// <param name="M"></param>
	void SetLightWorldViewProj(XMMATRIX& M) { if (!m_fxLightWorldViewProj) return; m_fxLightWorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
};

class SkyShader : public Effect
{
public:
	SkyShader(string _Path);
	~SkyShader() override;
private:

public:

	virtual void WorldViewProjUpdate(XMMATRIX& _World, XMMATRIX& _View, XMMATRIX& _Proj) override;

	void SetCubeMap(ID3D11ShaderResourceView* cubemap) { m_CubeMap->SetResource(cubemap); }

	ID3DX11EffectTechnique* m_SkyTech;

	ID3DX11EffectMatrixVariable* m_WorldViewProj;
	ID3DX11EffectMatrixVariable* m_World;

	ID3DX11EffectShaderResourceVariable* m_CubeMap;

private:
	void SetWorldViewProj(CXMMATRIX M) { m_WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M) { m_World->SetMatrix(reinterpret_cast<const float*>(&M)); }

};
class WireShader : public Effect
{
public:
	WireShader(string _Path);
	~WireShader();
private:

public:

	virtual void WorldViewProjUpdate(XMMATRIX& _World, XMMATRIX& _View, XMMATRIX& _Proj) override;

	void SetColor(const XMFLOAT4& v) { if (!m_fxColor) return; m_fxColor->SetRawValue(&v, 0, sizeof(XMFLOAT4)); }

	ID3DX11EffectTechnique* m_WireTech;

	ID3DX11EffectVectorVariable* m_fxColor;
	ID3DX11EffectMatrixVariable* m_WorldViewProj;
private:
	void SetWorldViewProj(CXMMATRIX M) { m_WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
};
class TextureRenderShader : public Effect
{
public:
	TextureRenderShader(string _Path);
	~TextureRenderShader() override;
public:

	virtual void WorldViewProjUpdate(XMMATRIX& _World, XMMATRIX& _View, XMMATRIX& _Proj) override;

	void SetTexure(ID3D11ShaderResourceView* tex) { if (!m_fxDiffuseMap || !tex) return; m_fxDiffuseMap->SetResource(tex); }
	void SetBuffer(
		ID3D11ShaderResourceView* _Depth,
		ID3D11ShaderResourceView* _Normal,
		ID3D11ShaderResourceView* _Position,
		ID3D11ShaderResourceView* _Albedo,
		ID3D11ShaderResourceView* _MatDiffuse,
		ID3D11ShaderResourceView* _MatSpecular,
		ID3D11ShaderResourceView* _MatAmbient,
		ID3D11ShaderResourceView* _ShadowMap
	);

	//Light
	void SetPointCount(int& Count) { if (!m_fxPointCount) return; m_fxPointCount->SetRawValue(&Count, 0, sizeof(int)); }
	void SetLights(const DirectionalLight& lights) { if (!m_fxDirLight) return; m_fxDirLight->SetRawValue(&lights, 0, sizeof(DirectionalLight)); }
	void SetLights(const PointLight& lights, int Index) { if (!m_fxPointLight) return; m_fxPointLight->SetRawValue(&lights, 0, sizeof(PointLight) * 10); }
	void SetLights(const SpotLight& lights) { if (!m_fxSpotLight) return; m_fxSpotLight->SetRawValue(&lights, 0, sizeof(SpotLight)); }
	void SetLightViewProj(XMMATRIX& M) { if (!m_fxLightViewProj) return; m_fxLightViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetCurrentViewProj(XMMATRIX& M) { if (!m_fxCurrentViewProj) return; m_fxCurrentViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetPrevViewProj(XMMATRIX& M) { if (!m_fxPrevViewProj) return; m_fxPrevViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }

	ID3DX11EffectTechnique* m_TextureRenderTech; // 텍스처를 출력하는 테크.
	ID3DX11EffectTechnique* m_DeferredRenderTech; // 디퍼드 텍스처를 만드는 테크. 만든후에 m_TextureRenderTech로 출력하자.
	ID3DX11EffectTechnique* m_PostProcessTech; // 디퍼드 텍스처를 만드는 테크. 만든후에 m_TextureRenderTech로 출력하자.


private:
	ID3DX11EffectShaderResourceVariable* m_fxDiffuseMap;
	ID3DX11EffectShaderResourceVariable* m_fxDepthBuffer;
	ID3DX11EffectShaderResourceVariable* m_fxNormalBuffer;
	ID3DX11EffectShaderResourceVariable* m_fxPositionBuffer;
	ID3DX11EffectShaderResourceVariable* m_fxAlbedoBuffer;


	ID3DX11EffectShaderResourceVariable* m_fxMatDiffuse;
	ID3DX11EffectShaderResourceVariable* m_fxMatSpecular;
	ID3DX11EffectShaderResourceVariable* m_fxMatAmbient;

	ID3DX11EffectShaderResourceVariable* m_fxShadowMap;
	ID3DX11EffectMatrixVariable* m_fxLightViewProj;
	ID3DX11EffectMatrixVariable* m_fxCurrentViewProj;
	ID3DX11EffectMatrixVariable* m_fxPrevViewProj;

	//light
	ID3DX11EffectScalarVariable* m_fxPointCount;
	ID3DX11EffectVariable* m_fxDirLight;
	ID3DX11EffectVariable* m_fxPointLight;
	ID3DX11EffectVariable* m_fxSpotLight;
	ID3DX11EffectVectorVariable* m_fxEyePosW;
};



class ParticleEffect : public Effect
{
public:
	ParticleEffect(string _Path);
	~ParticleEffect() override;


	void SetGameTime(float f) { _gameTime->SetFloat(f); }
	void SetTimeStep(float f) { _timeStep->SetFloat(f); }

	void SetEyePosW(const XMFLOAT3& v) { _eyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }

	void SetTexArray(ID3D11ShaderResourceView* tex) { _texArray->SetResource(tex); }
	void SetRandomTex(ID3D11ShaderResourceView* tex) { _randomTex->SetResource(tex); }

	virtual void WorldViewProjUpdate(XMMATRIX& _World, XMMATRIX& _View, XMMATRIX& _Proj) override;

private:
	void SetViewProj(CXMMATRIX M) { _viewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEmitPosW(const XMFLOAT3& v) { _emitPosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetEmitDirW(const XMFLOAT3& v) { _emitDirW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }

public:
	ID3DX11EffectTechnique* _streamOutTech;
	ID3DX11EffectTechnique* _drawTech;

private:
	ID3DX11EffectMatrixVariable* _viewProj;
	ID3DX11EffectScalarVariable* _gameTime;
	ID3DX11EffectScalarVariable* _timeStep;
	ID3DX11EffectVectorVariable* _eyePosW;
	ID3DX11EffectVectorVariable* _emitPosW;
	ID3DX11EffectVectorVariable* _emitDirW;
	ID3DX11EffectShaderResourceVariable* _texArray;
	ID3DX11EffectShaderResourceVariable* _randomTex;
};




class Effects
{
public:
	Effects();
	virtual ~Effects();
public:
	static Shader* LightFX;
	static WireShader* WireFX;
	static SkyShader* SkyFX;
	static TextureRenderShader* TextureRenderFX;

	static ParticleEffect* ParticleFX;


};
