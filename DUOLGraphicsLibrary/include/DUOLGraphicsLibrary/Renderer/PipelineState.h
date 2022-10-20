#pragma once
#include "DUOLGraphicsLibrary/EntityBase.h"
#include "DUOLGraphicsLibrary/PipelineStateFlags.h"

namespace DUOLGraphicsLibrary
{
 /**

     @class   PipelineState
     @brief   그래픽스 파이프라인에 바인딩될 PSO
     @details ~
     @author  KyungMin Oh

 **/
	class DUOLGRAPHICSLIBRARY_EXPORT PipelineState : public EntityBase
	{
		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::PipelineState);

	};
}