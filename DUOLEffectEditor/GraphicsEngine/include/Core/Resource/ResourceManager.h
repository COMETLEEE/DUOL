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
	class ResourceBase;

	class Factory;

	class TextureRenderPass;

	class ResourceManager
	{
	public:
		ResourceManager();

		~ResourceManager();
	private:
		template<class T>
		TypeUID GetTypeKey();

		std::string D3DMacroToString(std::vector<D3D_SHADER_MACRO>& macro);

		std::string TupleToString(std::tuple<tstring, std::string, std::string>& key);

	public:
		void init();

		void* LoadTexture(tstring path);

		void* LoadTexture(std::string path);

		ID3D11ShaderResourceView* GetNoiseMap(std::tuple<float, int, float> key);

		void CreateParticleMesh(std::string name);

		void CompileVertexShader(PipeLineDesc& pipeLineDesc, const WCHAR* fileName, const CHAR* entryName,
		std::vector<D3D_SHADER_MACRO> macro = std::vector<D3D_SHADER_MACRO>(),
			std::vector<ShaderLikingDesc> _shaderLikingDescs = std::vector<ShaderLikingDesc>());

		void CompilePixelShader(PipeLineDesc& pipeLineDesc, const WCHAR* fileName, const CHAR* entryName,
		std::vector<D3D_SHADER_MACRO> macro = std::vector<D3D_SHADER_MACRO>(),
			std::vector<ShaderLikingDesc> _shaderLikingDescs = std::vector<ShaderLikingDesc>());

		void CompileGeometryShader(PipeLineDesc& pipeLineDesc, const WCHAR* fileName, const CHAR* entryName,
		bool useStreamOut, std::vector<D3D_SHADER_MACRO> macro = std::vector<D3D_SHADER_MACRO>(),
			std::vector<ShaderLikingDesc> _shaderLikingDescs = std::vector<ShaderLikingDesc>());

		void CompileComputeShader(PipeLineDesc& pipeLineDesc, const WCHAR* fileName, const CHAR* entryName,
		std::vector<D3D_SHADER_MACRO> macro = std::vector<D3D_SHADER_MACRO>(),
			std::vector<ShaderLikingDesc> _shaderLikingDescs = std::vector<ShaderLikingDesc>());
		// �ٸ� ���ҽ���� �ٸ��� �ؽ��ĸ� ���� �Լ��� ���� ������ �ؽ��Ĵ� �����ÿ� �ٽ� �ε带 ������ϱ� �����̴�..!
		ID3D11ShaderResourceView* GetTexture(std::string name);

		ID3D11ShaderResourceView* GetTexture(tstring name);

	private:
		std::unordered_map<std::string, std::pair<unsigned int, ID3D11ClassInstance*>> _shaderClassInstanceStorage; // ��� ���̴��� ��Ʋ�� ���� �̸��� Ŭ������ ���ٰ� �����Ѵ�.

		std::unordered_map<TypeUID, std::unordered_map<std::string, ResourceBase*>> _resourceStorage;

	public:
		template<class resourceType>
		void AddResource(const std::string& resourceName, resourceType* resource);

		template<class resourceType>
		resourceType* GetResource(const std::string& resourceName);

		template<class resourceType>
		void SubResource(const std::string& resourceName);

	private:
		Factory* _factory;

		unsigned int _textureId;

		unsigned int _meshId;
	};

	template <class T>
	TypeUID ResourceManager::GetTypeKey()
	{
		static T* TypeKeyObject = nullptr;

		return (TypeUID)(&TypeKeyObject);
	}

	template <class resourceType>
	void ResourceManager::AddResource(const std::string& resourceName, resourceType* resource)
	{
		auto& targetStructure = _resourceStorage[GetTypeKey<resourceType>()];

		auto result = targetStructure.find(resourceName);

		if (result != targetStructure.end())
			assert(false);

		static_assert(std::is_base_of_v<ResourceBase, resourceType>);

		targetStructure.insert({ resourceName, resource });
	}

	template <class resourceType>
	resourceType* ResourceManager::GetResource(const std::string& resourceName)
	{
		auto& targetStructure = _resourceStorage[GetTypeKey<resourceType>()];

		auto result = targetStructure.find(resourceName);

		if (result == targetStructure.end())
			return nullptr;

		return reinterpret_cast<resourceType*>(result->second); //Dynamic Cast�� ������� �ʴ� ����
		// �̹� Ű ���� ���� ���͸��� �����߱� ����.
	}

	template <class resourceType>
	void ResourceManager::SubResource(const std::string& resourceName)
	{
		auto& targetStructure = _resourceStorage[GetTypeKey<resourceType>()];

		auto result = targetStructure.find(resourceName);

		if (result != targetStructure.end())
		{
			delete result->second;
			targetStructure.erase(result);
		}
	}
}
