#include "pch.h"
#include "ResourceManager.h"
#include "DDSTextureLoader.h"
#include <DirectXColors.h>	// 이미 많은 부분 DX11과 겹친다.
#include "CASEParser.h"

ResourceManager::ResourceManager() :m_ASEParser(nullptr), m_Mesh(), m_Effects(), m_Factory(nullptr)
{

}
// 매핑이 되는 uv 좌표 그리드 만들어보기.
// 쉐이더 디버깅. 점 찍어보기
// 
ResourceManager::~ResourceManager()
{
	for (auto& iter : m_Mesh)
	{
		delete iter.second;
	}
	m_Mesh.clear();

	delete m_ASEParser;
	delete m_Effects;
	delete m_Factory;
	InputLayout::Finalize();
}

void ResourceManager::init()
{
	m_Factory = new Factory();
	m_ASEParser = new CASEParser();
	m_ASEParser->Init();
	m_Effects = new Effects();
	InputLayout::Initialize();


	/////// <summary>
	/////// 메쉬 생성
	/////// </summary>
	BulidInfo _Genji = {
		"Genji",
	"Resource/ASEFile/genji_blender.ASE" ,
	Effects::LightFX->m_TextureLightTech ,
	Effects::LightFX ,
	};
	m_Factory->CreateMesh(_Genji);



	BulidInfo _teapot = {
	"teapot",
	"Resource/ASEFile/teapot.ASE" ,
	Effects::LightFX->m_TextureLightTech ,
	Effects::LightFX ,
	};
	m_Factory->CreateMesh(_teapot);



	BulidInfo _Cylinder = {
	"Cylinder",
	"Resource/ASEFile/Cylinder.ASE" ,
	Effects::LightFX->m_TextureLightTech ,
	Effects::LightFX ,
	};
	m_Factory->CreateMesh(_Cylinder);



	BulidInfo _babypig = {
	"babypig",
	"Resource/ASEFile/babypig_walk_6x.ASE" ,
	Effects::LightFX->m_TextureLightTech ,
	Effects::LightFX ,
	};
	m_Factory->CreateMesh(_babypig);



	BulidInfo _Genji_max = {
	"Genji_max",
	"Resource/ASEFile/genji_max.ASE" ,
	Effects::LightFX->m_NormalTech ,
	Effects::LightFX ,
	};
	m_Factory->CreateMesh(_Genji_max);



	BulidInfo _03IK_JOE = {
	"03IK_JOE",
	"Resource/ASEFile/03IK-Joe.ASE" ,
	Effects::LightFX->m_LightTech ,
	Effects::LightFX ,
	};
	m_Factory->CreateMesh(_03IK_JOE);

	BulidInfo WorkMan = {
	"WorkMan",
	"Resource/ASEFile/WorkMan.ASE" ,
	Effects::LightFX->m_TextureLightTech ,
	Effects::LightFX ,
	};
	m_Factory->CreateMesh(WorkMan);

	m_Factory->CreateSpecialMeshs();
	///// <summary>
	///// Helper 오브젝트? 컴포넌트 생성
	///// </summary>

}

void ResourceManager::LoadTexture(string name, string path)
{

}

void ResourceManager::LoadASE(string name, string path)
{
	// 구현하다가 말았다..!!
// ASE가 필요해지면 다시 하자..
// 지금은 박스부터 나오게 하는게 우선일듯 하다.
}


