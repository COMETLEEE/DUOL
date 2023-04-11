#pragma once
/**

	@file      Inspector.h
	@brief     이펙트 에디터 매니저, 게임 매니저와 유사.
	@details   여러가지 인스펙터나 ObjectView나 다른 윈도우에서 공통적으로 사용하는 변수, 함수를 관리하는 매니저로 만들 것이다.
	@author    SinSeongHyeon
	@date      8.11.2022
	@copyright © SinSeongHyeon, 2022. All right reserved.

**/
#include <memory>
#include "StringHelper.h"
#include "../../DUOLMath/include/DUOLMath/DUOLMath.h"

class MoveTool;

namespace Muscle
{
	class GameObject;
	class ParticleRenderer;
}
namespace MuscleGrapics
{
	struct RenderingData_Particle;
}
class EffectEditorManager
{
public:
	EffectEditorManager(const EffectEditorManager&) = delete;

	EffectEditorManager(EffectEditorManager&&) = delete;

	EffectEditorManager& operator=(const EffectEditorManager&) = delete;

protected:
	EffectEditorManager();

	~EffectEditorManager() = default;

public:
	static EffectEditorManager& Get() { return _instance; };

private:
	static EffectEditorManager _instance;

	std::shared_ptr<Muscle::GameObject> _selectedObject; // 현재 선택된 게임 오브젝트.

	std::shared_ptr<Muscle::ParticleRenderer> _selectedParticle; // 현재 선택된 게임 오브젝트의 파티클 데이터. 다이나믹 캐스트의 비용을 아끼기 위해 캐싱한다.

	std::unique_ptr<MuscleGrapics::RenderingData_Particle> _lastChangedParticleData; // 마지막으로 수정된 파티클 데이터. 현재와 값이 변했는지 확인하기 위함 ex) new 할 때 값이 변했으면 저장을 수행한다.
	// 인스펙터가 아니라 이곳에 나둔 이유는 나중에 저장 같은 기능을 수행할 때도 비교하기 위해서다.

	DUOLCommon::tstring _savedPath; // 저장된 경로.

	std::shared_ptr<Muscle::GameObject> _moveToolParent;

	std::shared_ptr<MoveTool> _moveTool;

	DUOLMath::Vector3 _prePos;

	DUOLMath::Vector3 _preScale;

	DUOLMath::Vector3 _preRotate;

	std::string _preName;

public:
	void CreateMoveTool();

	void MouseEventUpdate();

	void NewParticle(); // 새로운 파티클을 만드는 함수.

	void SaveParticle(); // 현재 선택된 파티클을 세이브 한다.

	void SaveAsParticle(); // 다른 이름으로 저장한다.

	void LoadParticle(); // 파티클을 로드 한다.

	void LoadPreviousVersionParticle(); // 이전 버전의 파티클을 로드 한다.

	void UpdateAllFileToLastestVersionInFolder(); // 폴더 안에 모든 .dfx 파일을 최신 버전으로 업데이트 한다.
	
	const std::shared_ptr<Muscle::GameObject>& GetSelectedObject();

	const std::shared_ptr<Muscle::ParticleRenderer>& GetSelectedParticle();

	void SetSavedPath(DUOLCommon::tstring savedPath) { _savedPath = savedPath; }

	const DUOLCommon::tstring GetSavedPath() { return _savedPath; }

	void CheckChangedData_Update(MuscleGrapics::RenderingData_Particle& paritcleData);

private:
	void ConvertRendererToRenderingData(const std::shared_ptr<Muscle::ParticleRenderer>& parent, std::vector<MuscleGrapics::RenderingData_Particle>& renderingDatas);

private: // 커맨드로 따로 분리할 함수.
	void SelectObject(const std::shared_ptr<Muscle::GameObject>& object); // 인스펙터 창에 보여주기위해 오브젝트를 선택하는 함수.

public:
	friend class SelectObjectCommand;
	friend class ObjectTranslateCommand;
	friend class ObjectRotateCommand;
	friend class ObjectScaleCommand;
};

