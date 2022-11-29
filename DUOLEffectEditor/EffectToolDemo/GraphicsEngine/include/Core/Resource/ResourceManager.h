#pragma once
#include <unordered_map>
#include "Core/Pass/PassBase.h"
#include "util/TypeDefine.h"
#include "Export/RenderingData.h"

/// <summary>
/// �̸� ����Ʈ�� �޽��� �����ϰ� ���ܾ���?
/// </summary>
///

namespace MuscleGrapics
{
	class VBIBMesh;

	class ParticleMesh;

	class Factory;

	class ResourceManager
	{
	public:
		ResourceManager();

		~ResourceManager();

	public:
		void init();

		unsigned int InsertVBIBMesh(tstring name, VBIBMesh* mesh);

		VBIBMesh* GetVBIBMesh(unsigned int meshID);

		unsigned int GetVBIBMesh(tstring meshName);

		void* InsertTexture(tstring path);

		ID3D11ShaderResourceView* GetTexture(tstring name);

		unsigned int InsertParticleMesh(ParticleMesh* mesh);

		ParticleMesh* GetParticleMesh(unsigned int objectID);

		PassBase<RenderingData_3D>* Get3DShader(tstring name);

		PassBase<RenderingData_Particle>* GetParticleShader(tstring name);

	private:
		std::unordered_map<unsigned int, VBIBMesh*> _mesh_VBIB_IDs;

		std::unordered_map<tstring, unsigned int> _mesh_VBIB_ID_Maps; // Ž���� �ӵ��� string �� int�� ���̰� �ֱ� ������ �ӵ� ����� ���� ������ �ؼ� ����Ѵ�.
		// ���߿� ��ġ ��������..!!

		std::unordered_map<tstring, ID3D11ShaderResourceView*> _textureMapIDs;

		std::unordered_map<unsigned int, ParticleMesh*> _particleMapIDs;

		std::unordered_map<tstring, PassBase<RenderingData_3D>*> _3DShaderIDs; // ���̴��� ������ mesh ��ŭ ���� �� ���� ������ ������ �� �ʿ�� ������ �ϴ�.. ��Ʈ������ ��������..
		// � fx���ϰ� �׸��� � tech�� ������� �̸��� ���� �����Ѵ�.

		std::unordered_map<tstring, PassBase<RenderingData_Particle>*> _particleShaderIDs;

		Factory* _factory;

		unsigned int _textureId;

		unsigned int _meshId;

		unsigned int _particleId;
	};
}