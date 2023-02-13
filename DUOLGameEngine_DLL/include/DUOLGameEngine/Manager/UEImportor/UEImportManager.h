#pragma once
#include "DUOLGameEngine/Util/SingletonBase.h"

namespace DUOLGameEngine
{
	class Scene;
}

namespace DUOLGameEngine
{
	/**
	 * \brief Unreal Engine ���� �ν���Ʈ ���� ���̾��Ű ���� ������ Import �մϴ�.
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