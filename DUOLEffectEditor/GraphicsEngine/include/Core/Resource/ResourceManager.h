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

	class TextureRenderPass;

	class ResourceManager
	{
	public:
		ResourceManager();

		~ResourceManager();
	private:
		// ���丮 Ŭ������ ���� �����..? �׷��� �ҽ��� ������ �ʾƼ� ����� �ȴ�.
		void OutputShaderErrorMessage(ID3DBlob* errorMessage, HWND hwnd, const WCHAR* shaderFileName);

		ID3D11VertexShader* GetVertexShader(std::tuple<tstring, std::string, std::string> key);

		void InsertVertexShader(std::tuple<tstring, std::string, std::string> key, ID3D11VertexShader* shader);

		ID3D11PixelShader* GetPixelShader(std::tuple<tstring, std::string, std::string> key);

		void InsertPixelShader(std::tuple<tstring, std::string, std::string> key, ID3D11PixelShader* shader);

		ID3D11GeometryShader* GetGeometryShader(std::tuple<tstring, std::string, std::string> key);

		void InsertGeometryShader(std::tuple<tstring, std::string, std::string> key, ID3D11GeometryShader* shader);

		void InsertInputLayOut(ID3D11VertexShader* key, ID3D11InputLayout* inputLayout);

		std::string D3DMacroToString(std::vector<D3D_SHADER_MACRO>& macro);

		std::string TupleToString(std::tuple<tstring, std::string, std::string>& key);
	public:
		void init();

		unsigned int InsertVBIBMesh(tstring name, VBIBMesh* mesh);

		VBIBMesh* GetVBIBMesh(unsigned int meshID);

		unsigned int GetVBIBMesh(tstring meshName);

		void* InsertTexture(tstring path);

		ID3D11ShaderResourceView* GetTexture(tstring name);

		void InsertParticleMesh(unsigned int objectID);

		void DeleteParticleMesh(unsigned int objectID);

		ParticleMesh* GetParticleMesh(unsigned int objectID);

		PassBase<RenderingData_3D>* Get3DShader(tstring name);

		PassBase<RenderingData_Particle>* GetParticleShader(tstring name);

		PassBase<std::vector<std::pair<ID3D11ShaderResourceView*, int>>>* GetTextureRenderShader(tstring name);

		ID3D11InputLayout* GetInputLayout(ID3D11VertexShader* key);

		ID3D11VertexShader* CompileVertexShader(const WCHAR* fileName, const CHAR* entryName, D3D11_INPUT_ELEMENT_DESC polygonLayout[], UINT size, std::vector<D3D_SHADER_MACRO> macro = std::vector<D3D_SHADER_MACRO>());

		ID3D11PixelShader* CompilePixelShader(const WCHAR* fileName, const CHAR* entryName, std::vector<D3D_SHADER_MACRO> macro = std::vector<D3D_SHADER_MACRO>());

		ID3D11GeometryShader* CompileGeometryShader(const WCHAR* fileName, const CHAR* entryName, bool useStreamOut, std::vector<D3D_SHADER_MACRO> macro = std::vector<D3D_SHADER_MACRO>());

	private:
		std::unordered_map<unsigned int, VBIBMesh*> _mesh_VBIB_IDs;

		std::unordered_map<tstring, unsigned int> _mesh_VBIB_ID_Maps; // Ž���� �ӵ��� string �� int�� ���̰� �ֱ� ������ �ӵ� ����� ���� ������ �ؼ� ����Ѵ�.
		// ���߿� ��ġ ��������..!!
		std::unordered_map<tstring, ID3D11ShaderResourceView*> _textureMapIDs;

		std::unordered_map<unsigned int, ParticleMesh*> _particleMapIDs;

		std::unordered_map<tstring, PassBase<RenderingData_3D>*> _3DShaderIDs; // ���̴��� ������ mesh ��ŭ ���� �� ���� ������ ������ �� �ʿ�� ������ �ϴ�.. ��Ʈ������ ��������..
		// � fx���ϰ� �׸��� � tech�� ������� �̸��� ���� �����Ѵ�.

		std::unordered_map<tstring, PassBase<RenderingData_Particle>*> _particleShaderIDs;

		std::unordered_map<tstring, PassBase<std::vector<std::pair<ID3D11ShaderResourceView*, int>>>*> _textureRenderShaderIDs;

		std::unordered_map<std::string, ID3D11VertexShader*> _vertexShaderStorage; // ���� ���̴��� ������ ������ �� �ʿ�� ������ �����ϰ� �����Ŵ� ���� ����.

		std::unordered_map<ID3D11VertexShader*, ID3D11InputLayout*> _inputLayoutStorage; // VertexShader�� ��Ʈ�̴�, VertexShader�� Ű������ ��������.

		std::unordered_map<std::string, ID3D11PixelShader*> _pixelShaderStorage; // ���� ���̴��� ������ ������ �� �ʿ�� ������ �����ϰ� �����Ŵ� ���� ����.

		std::unordered_map<std::string, ID3D11GeometryShader*> _geometryShaderStorage; // ���� ���̴��� ������ ������ �� �ʿ�� ������ �����ϰ� �����Ŵ� ���� ����.

		Factory* _factory;

		unsigned int _textureId;

		unsigned int _meshId;
	};


}
