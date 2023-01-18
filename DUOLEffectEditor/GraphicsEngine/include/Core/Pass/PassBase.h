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


#if defined(DEBUG) || defined(_DEBUG)
//static uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
static uint32 compileFlag = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#else
static uint32 compileFlag = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#endif

namespace MuscleGrapics
{
	template <typename T>
	class PassBase abstract
	{
	public:
		PassBase(D3D11_PRIMITIVE_TOPOLOGY topology);

		virtual ~PassBase();

	private:
		std::unordered_map<unsigned int, ID3D11InputLayout*> _inputLayout;

		D3D11_PRIMITIVE_TOPOLOGY _topolgy;

		std::unordered_map<unsigned int, ID3D11VertexShader*> _vertexShader;

		std::unordered_map<unsigned int, ID3D11PixelShader*> _pixelShader;

		std::unordered_map<unsigned int, ID3D11GeometryShader*> _geometryShader;

		std::map<UINT, ID3D11Buffer*> _constantBuffers;

	protected:
		ID3D11DeviceContext* _d3dImmediateContext;

		float _left = -1.0f;
		float _right = 1.0f;
		float _top = 1.0f;
		float _bottom = -1.0f;

	protected:
		void InsertShader(ID3D11VertexShader* vertexShader, ID3D11InputLayout* inputLayout, ID3D11GeometryShader* geometryShader, ID3D11PixelShader* pixelShader, UINT index);
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
		_topolgy(topology), _inputLayout(),
		_d3dImmediateContext(DXEngine::GetInstance()->Getd3dImmediateContext()),
		_vertexShader(),
		_pixelShader(),
		_geometryShader()

	{

	}

	template <typename T>
	PassBase<T>::~PassBase()
	{
		//for (auto& iter : _inputLayout)
		//{
		//	if (iter.second)
		//		iter.second->Release();
		//}
		//for (auto& iter : _vertexShader)
		//{
		//	if (iter.second)
		//		iter.second->Release();
		//}
		//for (auto& iter : _pixelShader)
		//{
		//	if (iter.second)
		//		iter.second->Release();
		//}
		//for (auto& iter : _geometryShader)
		//{
		//	if (iter.second)
		//		iter.second->Release();
		//}

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
	}

	template <typename T>
	void PassBase<T>::InsertShader(ID3D11VertexShader* vertexShader, ID3D11InputLayout* inputLayout,
		ID3D11GeometryShader* geometryShader, ID3D11PixelShader* pixelShader, UINT index)
	{
		if (_vertexShader.find(index) != _vertexShader.end())
			assert(false);

		_vertexShader[index] = vertexShader;

		_inputLayout[index] = inputLayout;

		_geometryShader[index] = geometryShader;

		_pixelShader[index] = pixelShader;
	}

	template <typename T>
	void PassBase<T>::SetShader(UINT shaderIndex)
	{
		if (_inputLayout.find(shaderIndex) == _inputLayout.end())
			assert(false);

		_d3dImmediateContext->IASetInputLayout(_inputLayout[shaderIndex]);

		_d3dImmediateContext->IASetPrimitiveTopology(_topolgy);

		_d3dImmediateContext->VSSetShader(_vertexShader[shaderIndex], nullptr, 0);

		_d3dImmediateContext->GSSetShader(_geometryShader[shaderIndex], nullptr, 0);

		_d3dImmediateContext->PSSetShader(_pixelShader[shaderIndex], nullptr, 0);
	}
}
