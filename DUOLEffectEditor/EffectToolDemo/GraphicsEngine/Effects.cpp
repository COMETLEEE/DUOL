#include "pch.h"
#include "Effects.h"

Effect::Effect(string _Path)
{
	//셰이더 불러오기 및 설정인데 아직 잘 이해가 안간다 todo:
	ID3D11Device* _d3dDevice = DXEngine::GetInstance()->GetD3dDevice();
	HRESULT hr;
	std::ifstream fin(_Path.c_str(), std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
		0, _d3dDevice, &m_FX));

	compiledShader.clear();

}

Effect::~Effect()
{
	ReleaseCOM(m_FX);
}


Shader::Shader(string _Path) :Effect(_Path),
m_TextureLightTech(nullptr), m_LightTech(nullptr),
m_fxEyePosW(nullptr), m_fxWorld(nullptr), m_fxWorldInvTranspose(nullptr),
m_fxWorldViewProj(nullptr), m_fxMaterial(nullptr), m_fxDiffuseMap(nullptr), m_NormalTech(nullptr),
m_fxLightWorldViewProj(nullptr)
{
	m_TextureLightTech = m_FX->GetTechniqueByName("TextureLightTech");
	m_LightTech = m_FX->GetTechniqueByName("LightTech");
	m_NormalTech = m_FX->GetTechniqueByName("NormalTech");

	m_fxEyePosW = m_FX->GetVariableByName("gEyePosW")->AsVector();


	m_fxWorld = m_FX->GetVariableByName("gWorld")->AsMatrix();
	m_fxWorldInvTranspose = m_FX->GetVariableByName("gWorldlnvTranspose")->AsMatrix();
	m_fxWorldViewProj = m_FX->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_fxLightWorldViewProj = m_FX->GetVariableByName("gLightWorldViewProj")->AsMatrix();
	m_fxBoneTM = m_FX->GetVariableByName("gBoneTransforms")->AsMatrix();

	m_fxMaterial = m_FX->GetVariableByName("gMaterial");

	m_fxDiffuseMap = m_FX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	m_fxNormalMap = m_FX->GetVariableByName("gNormalMap")->AsShaderResource();
	m_fxCubeMap = m_FX->GetVariableByName("gCubeMap")->AsShaderResource();
}
Shader::~Shader()
{
	ReleaseCOM(m_TextureLightTech)
		ReleaseCOM(m_NormalTech)
		ReleaseCOM(m_LightTech)

		ReleaseCOM(m_fxEyePosW)

		ReleaseCOM(m_fxWorld)
		ReleaseCOM(m_fxWorldInvTranspose)
		ReleaseCOM(m_fxWorldViewProj)
		ReleaseCOM(m_fxLightWorldViewProj)
		ReleaseCOM(m_fxBoneTM)

		ReleaseCOM(m_fxMaterial)
		ReleaseCOM(m_fxDiffuseMap)
		ReleaseCOM(m_fxNormalMap)
		ReleaseCOM(m_fxCubeMap)

}

void Shader::WorldViewProjUpdate(XMMATRIX& _World, XMMATRIX& _View, XMMATRIX& _Proj)
{

	XMMATRIX worldViewProj = _World * _View * _Proj; // 카메라

	XMMATRIX worldtranspose = MathHelper::InverseTranspose(_World); // 역전치
	SetWorldInvTranspose(worldtranspose);
	SetWorld(_World);
	SetWorldViewProj(worldViewProj);
}


Shader* Effects::LightFX = nullptr;
WireShader* Effects::WireFX = nullptr;
SkyShader* Effects::SkyFX = nullptr;
TextureRenderShader* Effects::TextureRenderFX = nullptr;
ParticleEffect* Effects::ParticleFX = nullptr;

ParticleEffect::ParticleEffect(string _Path) : Effect(_Path)
{
	_streamOutTech = m_FX->GetTechniqueByName("StreamOutTech");
	_drawTech = m_FX->GetTechniqueByName("DrawTech");

	_viewProj = m_FX->GetVariableByName("gViewProj")->AsMatrix();
	_gameTime = m_FX->GetVariableByName("gGameTime")->AsScalar();
	_timeStep = m_FX->GetVariableByName("gTimeStep")->AsScalar();
	_eyePosW = m_FX->GetVariableByName("gEyePosW")->AsVector();
	_emitPosW = m_FX->GetVariableByName("gEmitPosW")->AsVector();
	_emitDirW = m_FX->GetVariableByName("gEmitDirW")->AsVector();
	_texArray = m_FX->GetVariableByName("gTexArray")->AsShaderResource();
	_randomTex = m_FX->GetVariableByName("gRandomTex")->AsShaderResource();
}

ParticleEffect::~ParticleEffect()
{
	ReleaseCOM(_streamOutTech);
	ReleaseCOM(_drawTech);

	ReleaseCOM(_viewProj);
	ReleaseCOM(_gameTime);
	ReleaseCOM(_timeStep);
	ReleaseCOM(_eyePosW);
	ReleaseCOM(_emitPosW);
	ReleaseCOM(_emitDirW);
	ReleaseCOM(_texArray);
	ReleaseCOM(_randomTex);
}

void ParticleEffect::WorldViewProjUpdate(XMMATRIX& _World, XMMATRIX& _View, XMMATRIX& _Proj)
{
	SetViewProj(_View * _Proj);
	XMFLOAT3 pos = XMFLOAT3(_World.r[3].m128_f32[0], _World.r[3].m128_f32[1], _World.r[3].m128_f32[2]);
	XMFLOAT3 dir = XMFLOAT3(_World.r[2].m128_f32[0], _World.r[2].m128_f32[1], _World.r[2].m128_f32[2]);
	SetEmitPosW(pos);
	SetEmitDirW(dir);
}

Effects::Effects()
{
	assert(!LightFX);

	LightFX = new Shader("FX/Light.fxo");
	WireFX = new WireShader("FX/wire.fxo");
	SkyFX = new SkyShader("FX/Sky.fxo");
	TextureRenderFX = new TextureRenderShader("FX/RenderTexture.fxo");
	ParticleFX = new ParticleEffect("FX/BasicParticle.fxo");
}

Effects::~Effects()
{
	delete LightFX;
	delete WireFX;
	delete SkyFX;
	delete TextureRenderFX;
	delete ParticleFX;
}

SkyShader::SkyShader(string _Path) : Effect(_Path)
{
	m_SkyTech = m_FX->GetTechniqueByName("SkyTech");
	m_WorldViewProj = m_FX->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_World = m_FX->GetVariableByName("gWorld")->AsMatrix();
	m_CubeMap = m_FX->GetVariableByName("gCubeMap")->AsShaderResource();
}

SkyShader::~SkyShader()
{
	ReleaseCOM(m_SkyTech)
		ReleaseCOM(m_WorldViewProj)
		ReleaseCOM(m_World)
		ReleaseCOM(m_CubeMap)
}

void SkyShader::WorldViewProjUpdate(XMMATRIX& _World, XMMATRIX& _View, XMMATRIX& _Proj)
{
	XMMATRIX __World = _World;
	__World.r[0].m128_f32[0] = 1.0f; __World.r[0].m128_f32[1] = 0; __World.r[0].m128_f32[2] = 0;
	__World.r[1].m128_f32[0] = 0; __World.r[1].m128_f32[1] = 1.0f; __World.r[1].m128_f32[2] = 0;
	__World.r[2].m128_f32[0] = 0; __World.r[2].m128_f32[1] = 0; __World.r[2].m128_f32[2] = 1.0f;
	SetWorldViewProj(__World * _View * _Proj);
	SetWorld(__World);
}

WireShader::WireShader(string _Path) : Effect(_Path)
{
	m_WireTech = m_FX->GetTechniqueByName("ColorTech");
	m_WorldViewProj = m_FX->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_fxColor = m_FX->GetVariableByName("gColor")->AsVector();
}

WireShader::~WireShader()
{
	ReleaseCOM(m_WireTech)
		ReleaseCOM(m_WorldViewProj)
		ReleaseCOM(m_fxColor)
}

void WireShader::WorldViewProjUpdate(XMMATRIX& _World, XMMATRIX& _View, XMMATRIX& _Proj)
{
	SetWorldViewProj(_World * _View * _Proj);
}

TextureRenderShader::TextureRenderShader(string _Path) : Effect(_Path)
{
	m_TextureRenderTech = m_FX->GetTechniqueByName("TextureRenderTech");
	m_DeferredRenderTech = m_FX->GetTechniqueByName("DeferredRenderTech");
	m_PostProcessTech = m_FX->GetTechniqueByName("PostProcessingTech");
	m_fxDiffuseMap = m_FX->GetVariableByName("gDiffuseMap")->AsShaderResource();

	m_fxDepthBuffer = m_FX->GetVariableByName("gDepthBuffer")->AsShaderResource();
	m_fxNormalBuffer = m_FX->GetVariableByName("gNormalBuffer")->AsShaderResource();
	m_fxPositionBuffer = m_FX->GetVariableByName("gPositionBuffer")->AsShaderResource();
	m_fxAlbedoBuffer = m_FX->GetVariableByName("gAlbedoBuffer")->AsShaderResource();

	m_fxMatDiffuse = m_FX->GetVariableByName("gMatDiffuse")->AsShaderResource();
	m_fxMatSpecular = m_FX->GetVariableByName("gMatSpecular")->AsShaderResource();
	m_fxMatAmbient = m_FX->GetVariableByName("gMatAmbient")->AsShaderResource();
	m_fxShadowMap = m_FX->GetVariableByName("gShadowMap")->AsShaderResource();

	m_fxDirLight = m_FX->GetVariableByName("gDirLight");
	m_fxPointLight = m_FX->GetVariableByName("gPointLight");
	m_fxSpotLight = m_FX->GetVariableByName("gSpotLight");
	m_fxEyePosW = m_FX->GetVariableByName("gEyePosW")->AsVector();
	m_fxPointCount = m_FX->GetVariableByName("gPointCount")->AsScalar();

	m_fxLightViewProj = m_FX->GetVariableByName("gLightViewProj")->AsMatrix();
	m_fxCurrentViewProj = m_FX->GetVariableByName("gCurrentViewProj")->AsMatrix();
	m_fxPrevViewProj = m_FX->GetVariableByName("gPrevViewProj")->AsMatrix();

}

TextureRenderShader::~TextureRenderShader()
{
	ReleaseCOM(m_TextureRenderTech)
		ReleaseCOM(m_DeferredRenderTech)
		ReleaseCOM(m_PostProcessTech)
		ReleaseCOM(m_fxDiffuseMap)

		ReleaseCOM(m_fxDepthBuffer)
		ReleaseCOM(m_fxNormalBuffer)
		ReleaseCOM(m_fxPositionBuffer)
		ReleaseCOM(m_fxAlbedoBuffer)

		ReleaseCOM(m_fxDirLight)
		ReleaseCOM(m_fxPointLight)
		ReleaseCOM(m_fxSpotLight)
		ReleaseCOM(m_fxEyePosW)
		ReleaseCOM(m_fxPointCount)

		ReleaseCOM(m_fxMatDiffuse)
		ReleaseCOM(m_fxMatSpecular)
		ReleaseCOM(m_fxMatAmbient)
		ReleaseCOM(m_fxShadowMap)

		ReleaseCOM(m_fxLightViewProj)
		ReleaseCOM(m_fxCurrentViewProj)
		ReleaseCOM(m_fxPrevViewProj)

}

void TextureRenderShader::WorldViewProjUpdate(XMMATRIX& _World, XMMATRIX& _View, XMMATRIX& _Proj)
{
	// 텍스처는 필요없다.

}

void TextureRenderShader::SetBuffer(ID3D11ShaderResourceView* _Depth,
	ID3D11ShaderResourceView* _Normal,
	ID3D11ShaderResourceView* _Position,
	ID3D11ShaderResourceView* _Albedo,
	ID3D11ShaderResourceView* _MatDiffuse,
	ID3D11ShaderResourceView* _MatSpecular,
	ID3D11ShaderResourceView* _MatAmbient,
	ID3D11ShaderResourceView* _ShadowMap
)
{
	m_fxDepthBuffer->SetResource(_Depth);
	m_fxNormalBuffer->SetResource(_Normal);
	m_fxPositionBuffer->SetResource(_Position);
	m_fxAlbedoBuffer->SetResource(_Albedo);

	m_fxMatDiffuse->SetResource(_MatDiffuse);
	m_fxMatSpecular->SetResource(_MatSpecular);
	m_fxMatAmbient->SetResource(_MatAmbient);

	// todo:
	m_fxShadowMap->SetResource(_ShadowMap);

}
