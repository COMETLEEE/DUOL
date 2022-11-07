#pragma once
#include "DUOLGraphicsLibrary/EntityBase.h"

namespace DUOLGraphicsLibrary
{
	class RenderTarget;

 /**

     @class   RenderPass
     @brief   ���������ο� ����Ÿ�ٵ��� ���ε��ϱ����� ����Ÿ���� ����
     @details ~
     @author  KyungMin Oh

 **/
	class DUOLGRAPHICSLIBRARY_EXPORT RenderPass : public EntityBase
	{
		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::RenderPass);

	protected:
		RenderPass(const UINT64& guid):
			EntityBase(guid)
		{
			
		}

	};
}