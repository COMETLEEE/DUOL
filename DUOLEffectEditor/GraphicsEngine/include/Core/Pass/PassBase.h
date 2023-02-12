/**

	@file      PassBase.h
	@brief     /// PassBase를 상속 받은 Pass 제작 후 리소스 매니저에 패스 추가. Renderer에서 다형적으로 Draw동작 수형.
	@details   ~
	@author    SinSeongHyeon
	@date      9.11.2022
	@copyright © SinSeongHyeon, 2022. All right reserved.

**/
#pragma once
#include <d3dcompiler.h>
#include "util/TypeDefine.h"
#include "util/STLInclude.h"
#include "Export/RenderingData.h"
#include "Core/DirectX11/DXEngine.h"
#include "Core/DirectX11/Renderer.h"
#include "util/VertexDesc.h"
#include "Core/Resource/Resource/ResourceBase.h"
#if defined(DEBUG) || defined(_DEBUG)
static uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
//static uint32 compileFlag = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#else
static uint32 compileFlag = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#endif

namespace MuscleGrapics
{
	struct ShaderLikingDesc
	{
		std::string _interfaceName;
		std::string _instanceName;
		std::string _nullInstanceName;
	};
	struct PipeLineDesc
	{
		PipeLineDesc() :_vs(nullptr), _il(nullptr), _ps(nullptr), _gs(nullptr), _cs(nullptr),
			_vsDynamicLinkageArray(nullptr), _gsDynamicLinkageArray(nullptr), _psDynamicLinkageArray(nullptr), _csDynamicLinkageArray(nullptr),
			_numVsInstance(0), _numGsInstance(0), _numPsInstance(0), _numCsInstance(0)
		{
		}

		ID3D11VertexShader* _vs;
		ID3D11InputLayout* _il;
		ID3D11GeometryShader* _gs;
		ID3D11PixelShader* _ps;
		ID3D11ComputeShader* _cs;

		ID3D11ClassInstance** _vsDynamicLinkageArray;
		ID3D11ClassInstance** _gsDynamicLinkageArray;
		ID3D11ClassInstance** _psDynamicLinkageArray;
		ID3D11ClassInstance** _csDynamicLinkageArray;

		unsigned int _numVsInstance;
		unsigned int _numGsInstance;
		unsigned int _numPsInstance;
		unsigned int _numCsInstance;

		void Clear()
		{
			_vs = nullptr;
			_il = nullptr;
			_ps = nullptr;
			_gs = nullptr;
			_cs = nullptr;

			_vsDynamicLinkageArray = nullptr;
			_gsDynamicLinkageArray = nullptr;
			_psDynamicLinkageArray = nullptr;
			_csDynamicLinkageArray = nullptr;

			_numVsInstance = 0;
			_numGsInstance = 0;
			_numPsInstance = 0;
			_numCsInstance = 0;
		}
	};

	template <typename T>
	class PassBase abstract : public ResourceBase
	{
	public:
		PassBase(D3D11_PRIMITIVE_TOPOLOGY topology);

		virtual ~PassBase();

	private:
		D3D11_PRIMITIVE_TOPOLOGY _topolgy;

		std::map<UINT, ID3D11Buffer*> _constantBuffers;

	protected:
		std::vector<PipeLineDesc> _pipeLineDescs; //

		ID3D11DeviceContext* _d3dImmediateContext;

		float _left = -1.0f;
		float _right = 1.0f;
		float _top = 1.0f;
		float _bottom = -1.0f;

	protected:
		void InsertShader(PipeLineDesc& pipeLineDesc);
		/**
		 * \brief 오버라이딩된 Draw 함수에서 실행시켜야 하는 함수들.
		 */
		void SetShader(UINT shaderIndex = 0);

		virtual void SetConstants(T& renderingData) abstract;

		void CreateConstantBuffer(UINT slot, UINT bufferSize);

		template<class DATATYPE>
		void UpdateConstantBuffer(UINT slot, DATATYPE& data);
	public:
		virtual void Draw(T& renderingData) abstract;

		void SetDrawRectangle(float left, float right, float top, float bottom); // 이 함수 정의 하자!
	};

	template <typename T>
	PassBase<T>::PassBase(D3D11_PRIMITIVE_TOPOLOGY topology) :
		_topolgy(topology), _pipeLineDescs(),
		_d3dImmediateContext(DXEngine::GetInstance()->Getd3dImmediateContext())
	{

	}

	template <typename T>
	PassBase<T>::~PassBase()
	{

		for (auto& iter : _constantBuffers)
		{
			if (iter.second)
				iter.second->Release();
		}

	}



	template <typename T>
	void PassBase<T>::CreateConstantBuffer(UINT slot, UINT bufferSize/*sizeof(ConstantBuffDesc::CB_PerObject)*/)
	{
		auto device = DXEngine::GetInstance()->GetD3dDevice();

		// 정점 셰이더에 있는 행렬 상수 버퍼의 구조체를 작성합니다.
		D3D11_BUFFER_DESC matrixBufferDesc;
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = bufferSize;
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

		if (_constantBuffers.end() != _constantBuffers.find(slot))
		{
			::MessageBoxA(nullptr, "This Slot is not empty! Shader..", nullptr, MB_OK);
		}
		else
		{
			_constantBuffers[slot] = nullptr;
			// 상수 버퍼 포인터를 만들어 이 클래스에서 정점 셰이더 상수 버퍼에 접근할 수 있게 합니다.
			if (FAILED(device->CreateBuffer(&matrixBufferDesc, NULL, &_constantBuffers[slot])))
				::MessageBoxA(nullptr, "ConstatntBuffer Create Failed ! Shader..", nullptr, MB_OK);
		}
	}

	template <typename T>
	void PassBase<T>::SetDrawRectangle(float left, float right, float top, float bottom)
	{
		_left = left / DXEngine::GetInstance()->GetWidth() * 2 - 1;
		_right = right / DXEngine::GetInstance()->GetWidth() * 2 - 1;
		_top = -(top / DXEngine::GetInstance()->GetHeight() * 2 - 1);
		_bottom = -(bottom / DXEngine::GetInstance()->GetHeight() * 2 - 1);
	}

	template <typename T>
	template <class DATATYPE>
	void PassBase<T>::UpdateConstantBuffer(UINT slot, DATATYPE& data)
	{

		// 상수 버퍼의 내용을 쓸 수 있도록 잠급니다.
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		_d3dImmediateContext->Map(_constantBuffers[slot], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);


		// 상수 버퍼의 데이터에 대한 포인터를 가져옵니다.
		DATATYPE* dataPtr = static_cast<DATATYPE*>(mappedResource.pData);

		// 상수 버퍼에 행렬을 복사합니다.
		*dataPtr = data;

		// 상수 버퍼의 잠금을 풉니다.
		_d3dImmediateContext->Unmap(_constantBuffers[slot], 0);


		// 마지막으로 셰이더의 상수 버퍼를 바뀐 값으로 바꿉니다.
		_d3dImmediateContext->VSSetConstantBuffers(slot, 1, &_constantBuffers[slot]);

		_d3dImmediateContext->GSSetConstantBuffers(slot, 1, &_constantBuffers[slot]);

		_d3dImmediateContext->PSSetConstantBuffers(slot, 1, &_constantBuffers[slot]);

		_d3dImmediateContext->CSSetConstantBuffers(slot, 1, &_constantBuffers[slot]);
	}

	template <typename T>
	void PassBase<T>::InsertShader(PipeLineDesc& pipeLineDesc)
	{
		_pipeLineDescs.push_back(pipeLineDesc);

		pipeLineDesc.Clear();
	}

	template <typename T>
	void PassBase<T>::SetShader(UINT shaderIndex)
	{
		if (_pipeLineDescs.size() <= shaderIndex)
			assert(false);

		_d3dImmediateContext->IASetInputLayout(_pipeLineDescs[shaderIndex]._il);

		_d3dImmediateContext->IASetPrimitiveTopology(_topolgy);

		_d3dImmediateContext->VSSetShader(_pipeLineDescs[shaderIndex]._vs, _pipeLineDescs[shaderIndex]._vsDynamicLinkageArray, _pipeLineDescs[shaderIndex]._numVsInstance);

		_d3dImmediateContext->GSSetShader(_pipeLineDescs[shaderIndex]._gs, _pipeLineDescs[shaderIndex]._gsDynamicLinkageArray, _pipeLineDescs[shaderIndex]._numGsInstance);

		_d3dImmediateContext->PSSetShader(_pipeLineDescs[shaderIndex]._ps, _pipeLineDescs[shaderIndex]._psDynamicLinkageArray, _pipeLineDescs[shaderIndex]._numPsInstance);

		_d3dImmediateContext->CSSetShader(_pipeLineDescs[shaderIndex]._cs, _pipeLineDescs[shaderIndex]._csDynamicLinkageArray, _pipeLineDescs[shaderIndex]._numCsInstance);
	}

	using Pass_3D = PassBase<RenderingData_3D>;

	using Pass_Particle = PassBase<RenderingData_Particle>;

	using Pass_Texture = PassBase<std::vector<std::pair<ID3D11ShaderResourceView*, int>>>;
}
