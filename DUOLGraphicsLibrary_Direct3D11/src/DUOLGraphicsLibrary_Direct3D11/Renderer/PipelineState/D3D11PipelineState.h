#pragma once
#include "DUOLGraphicsLibrary/Renderer/PipelineState.h"
#include "DUOLGraphicsLibrary_Direct3D11/Direct3D11.h"
#include "DUOLGraphicsLibrary_Direct3D11/ComPtr.h"

namespace DUOLGraphicsLibrary
{
	class D3D11StateManager;
	/**

     @class   D3D11PipelineState
     @brief   D3D11 PipelineState
     @details ~
     @author  KyungMin Oh

 **/

	class D3D11PipelineState : public PipelineState
	{

	public:
		D3D11PipelineState(const UINT64& guid);

		virtual ~D3D11PipelineState() = default;

		virtual bool HasGeometryShader() abstract;

		virtual void BindPipeline(D3D11StateManager* stateManager, ID3D11DeviceContext* context) abstract;
	};
}

