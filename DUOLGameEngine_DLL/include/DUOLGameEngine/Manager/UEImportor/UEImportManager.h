#pragma once
#include "DUOLGameEngine/Util/SingletonBase.h"

namespace DUOLGameEngine
{
	class Scene;
}

namespace DUOLGameEngine
{
	/**
	 * \brief Unreal Engine 에서 인스펙트 중인 하이어라키 복사 파일을 Import 합니다.
	 */
	class UEImportManager : public DUOLGameEngine::SingletonBase<DUOLGameEngine::UEImportManager>
	{
		DECLARE_SINGLETON(UEImportManager)

		DELETE_COPY_MOVE(UEImportManager)

	public:
		void Initialize();

		void UnInitialize();

		virtual ~UEImportManager() override;

		void Import(DUOLGameEngine::Scene* scene, const DUOLCommon::tstring& filePath);
	};
}