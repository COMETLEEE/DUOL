#pragma once
#include "DUOLGameEngine/Util/SingletonBase.h"
#include "DUOLGameEngine/Util/EngineSpecification.h"

namespace DUOLGameEngine
{
	class Scene;

	class AnimatorController;

	class AnimationClip;
}

namespace DUOLGameEngine
{
	class DUOL_GAMEENGINE_API SerializeManager : public DUOLGameEngine::SingletonBase<SerializeManager>
	{
		DECLARE_SINGLETON(SerializeManager)

		DELETE_COPY_MOVE(SerializeManager)

	public:
		virtual ~SerializeManager() override;

	private:
		DUOLCommon::tstring _path;

	private:
		/**
		 * \brief 
		 * \param engineSpec 
		 */
		void Initialize(const DUOLGameEngine::EngineSpecification& engineSpec);

		void UnInitialize();

	public:
		/**
		 * \brief Serialize scene to project path.
		 * \param scene Target scene context.
		 * \return true => success 
		 */
		bool SerializeScene(const DUOLGameEngine::Scene* scene);

		/**
		 * \brief Deserialize scene from filePath of parameter.
		 * \param filePath Scene file name.
		 * \return Pointer of created scene by deserialize.
		 */
		std::shared_ptr<DUOLGameEngine::Scene> DeserializeScene(const DUOLCommon::tstring& filePath);

		bool SerializeAnimatorController(const DUOLGameEngine::AnimatorController* animatorController);

		std::shared_ptr<DUOLGameEngine::AnimatorController> DeserializeAnimatorController(const DUOLCommon::tstring& filePath);

		bool SerializeAnimationClip(const DUOLGameEngine::AnimationClip* animationClip);

		std::shared_ptr<DUOLGameEngine::AnimationClip> DeserializeAnimationClip(const DUOLCommon::tstring& filePath);

#pragma region FRIEND_CLASS
		friend class Engine;

		friend class SceneManager;

		friend class ResourceManager;
#pragma endregion
	};
}