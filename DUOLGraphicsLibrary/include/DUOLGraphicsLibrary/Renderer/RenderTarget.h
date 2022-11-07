#pragma once
#include <DUOLMath/DUOLMath.h>
#include "DUOLGraphicsLibrary/EntityBase.h"

namespace DUOLGraphicsLibrary
{
 /**

     @class   RenderTarget
     @brief   파이프라인의 OutMerge라인에 바인딩 될 렌더 타겟(g-buffer)
     @details ~
     @author  KyungMin Oh

 **/
	class DUOLGRAPHICSLIBRARY_EXPORT RenderTarget : public EntityBase
	{
		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::RenderTarget);

	protected:
		RenderTarget(const UINT64& guid):
			EntityBase(guid)
		{
			
		}
	
	public:
		virtual DUOLMath::Vector2 GetResolution() const abstract;
		
		virtual int GetNumberOfRenderTargets() abstract;

		virtual bool IsDepthStencil() const abstract;

		virtual bool IsColor() const abstract;

		virtual bool IsRenderContext() { return false; }
	};
}

