#pragma once
#include <unordered_map>
#include "Core/Pass/PassBase.h"
#include "util/TypeDefine.h"
#include "Export/RenderingData.h"
/// <summary>
/// 미리 이펙트와 메쉬를 생성하고 땡겨쓸듯?
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

		ID3D11ShaderResourceView* GetNoiseMap(std::tuple<float, int, float> key);

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

		std::unordered_map<tstring, unsigned int> _mesh_VBIB_ID_Maps; // 탐색의 속도가 string 과 int의 차이가 있기 때문에 속도 향상을 위해 매핑을 해서 사용한다.
		// 나중에 위치 수정하자..!!
		std::unordered_map<tstring, ID3D11ShaderResourceView*> _textureMapIDs;

		// Frequency 값과 Octaves, OctaaveMutiplier 값을 키값으로 가진다.
		std::map<std::tuple<float, int,float>, std::shared_ptr<ID3D11ShaderResourceView>> _perlineNoiseMaps; // 어째서 노이즈 맵만 shared_ptr을 사용하는가! refcount가 필요하다!! 하지만 직접 만드는 것은 낭비같으니 이 친구만 sharedptr을 사용하겠다!
		// 왜냐하면! 레퍼런스 카운트를 참조해 해제를 해줄 필요가 있기 때문이다!

		std::unordered_map<unsigned int, ParticleMesh*> _particleMapIDs;

		std::unordered_map<tstring, PassBase<RenderingData_3D>*> _3DShaderIDs; // 쉐이더는 종류가 mesh 만큼 많을 것 같지 않으니 매핑을 할 필요는 없을듯 하다.. 스트링으로 저장하자..
		// 어떤 fx파일과 그리고 어떤 tech를 사용할지 이름에 따라 저장한다.

		std::unordered_map<tstring, PassBase<RenderingData_Particle>*> _particleShaderIDs;

		std::unordered_map<tstring, PassBase<std::vector<std::pair<ID3D11ShaderResourceView*, int>>>*> _textureRenderShaderIDs;

		std::unordered_map<std::string, ID3D11VertexShader*> _vertexShaderStorage; // 같은 셰이더를 여러번 컴파일 할 필요는 없으니 저장하고 같은거는 꺼내 쓰자.

		std::unordered_map<ID3D11VertexShader*, ID3D11InputLayout*> _inputLayoutStorage; // VertexShader와 세트이니, VertexShader를 키값으로 저장하자.

		std::unordered_map<std::string, ID3D11PixelShader*> _pixelShaderStorage; // 같은 셰이더를 여러번 컴파일 할 필요는 없으니 저장하고 같은거는 꺼내 쓰자.

		std::unordered_map<std::string, ID3D11GeometryShader*> _geometryShaderStorage; // 같은 셰이더를 여러번 컴파일 할 필요는 없으니 저장하고 같은거는 꺼내 쓰자.


		Factory* _factory;

		unsigned int _textureId;

		unsigned int _meshId;
	};


}
