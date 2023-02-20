#pragma once
#include <d3dcommon.h>
#include <string>
#include <vector>

#include "Core/DirectX11/geometrygenerator.h"
#include "Core/Pass/PassBase.h"


/// <summary>
/// 메쉬를 대신 생성할 클래스.
/// 생성자를 사용하지 않고 팩토리가 생성할 것.
/// 파서에서 읽은 데이터를 잘 정리해서 알아서 생성해주세용~
/// </summary>

class ID3D11ShaderResourceView;

class ID3D11Buffer;

namespace MuscleGrapics
{
	class RenderTexture;
	class ParticleMesh;

	namespace Vertex
	{
		struct BasicLight;
	}
	struct index3;

	class GeometryGenerator::MeshData;

	class VBIBMesh;

	class Factory
	{

	public:
		Factory();

		~Factory() = default;

	public:
		ID3D11ShaderResourceView* LoadTexture(std::string path);

		std::pair<ID3D11Buffer*, ID3D11Buffer*> LoadASE(std::string path);

		VBIBMesh* CreateGridMesh();

		VBIBMesh* CreateSphereMesh();

		VBIBMesh* CreateBoxMesh();

		VBIBMesh* CreateTextureMesh();

		void Bake_BRDF_PreFilter_Irradiance(std::string path, ID3D11Texture2D* texture2DBuffer, ID3D11ShaderResourceView* srv);

		ID3D11ShaderResourceView* CreateRandomTexture1DSRV();

		ID3D11ShaderResourceView* CreatePerlinNoiseTexture(float frequency/*주파수*/, int octaves/*레이어 수*/, float octaveMutiplier, std::uint32_t seed/*randSeed*/, float width, float height);

		ParticleMesh* CreateParticleMesh();

		void CompileVertexShader(PipeLineDesc& pipeLineDesc, const WCHAR* fileName, const CHAR* entryName,
			std::vector<D3D_SHADER_MACRO> macro = std::vector<D3D_SHADER_MACRO>(),
			std::vector<ShaderLikingDesc> _shaderLikingDescs = std::vector<ShaderLikingDesc>());

		void CompilePixelShader(PipeLineDesc& pipeLineDesc, const WCHAR* fileName, const CHAR* entryName,
			std::vector<D3D_SHADER_MACRO> macro = std::vector<D3D_SHADER_MACRO>(),
			std::vector<ShaderLikingDesc> _shaderLikingDescs = std::vector<ShaderLikingDesc>());

		void CompileGeometryShader(PipeLineDesc& pipeLineDesc, const WCHAR* fileName, const CHAR* entryName, bool useStreamOut,
			std::vector<D3D_SHADER_MACRO> macro = std::vector<D3D_SHADER_MACRO>(),
			std::vector<ShaderLikingDesc> _shaderLikingDescs = std::vector<ShaderLikingDesc>());

		void CompileComputeShader(PipeLineDesc& pipeLineDesc, const WCHAR* fileName, const CHAR* entryName,
			std::vector<D3D_SHADER_MACRO> macro = std::vector<D3D_SHADER_MACRO>(),
			std::vector<ShaderLikingDesc> _shaderLikingDescs = std::vector<ShaderLikingDesc>());
	private:
		void ChangeGeometry(GeometryGenerator::MeshData* _MeshData, std::vector<Vertex::BasicLight>& _vertices, std::vector<index3>& _Indices);

		void OutputShaderErrorMessage(ID3DBlob* errorMessage, HWND hwnd, const WCHAR* shaderFileName);

		bool BuildInputLayout(ID3DBlob* shaderBlob, ID3D11InputLayout** ppinputLayOut);

		bool BuildGeometryShaderAttribute(ID3DBlob* shaderBlob, std::vector<D3D11_SO_DECLARATION_ENTRY>& outputElements, std::vector<std::string>& sementicNames);

		RenderTexture* Bake_BRDFLookUpTable(unsigned int width, unsigned int height);

		RenderTexture* Bake_PreFilterMap(ID3D11Texture2D* texture2DBuffer, ID3D11ShaderResourceView* srv, unsigned width, unsigned height, unsigned mipSize);

		RenderTexture* Bake_IrradianceMap(ID3D11Texture2D* texture2DBuffer, ID3D11ShaderResourceView* srv, unsigned width, unsigned height);
	};


}
