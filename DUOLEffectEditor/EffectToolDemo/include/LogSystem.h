#pragma once

#include "ImGuiRnedererBase.h"
#include "../Common/Imgui/imgui.h"

#define WriteLog(str,...) LogSystem::AddLog(str,__VA_ARGS__)

/// 싱글톤 객체이지만, 렌더를 하기 위해서 오브젝트에 부착 시켜야한다.

class LogSystem : public Muscle::ImGuiRnedererBase
{
public:
	LogSystem(std::shared_ptr<Muscle::GameObject> _gameObject);

	~LogSystem();

private:
	ImGuiTextBuffer     _buf;

	ImVector<int>       _lineOffsets; // Index to lines offset. We maintain this with AddLog() calls.

	static LogSystem* _instance;

	void Clear();

	void Draw();

public:
	static void AddLog(const char* fmt, ...) IM_FMTARGS(2); //IM_FMTARGS(2);

protected:
	/**
	* \brief 이 함수로 그래픽스에 넘겨줄 함수 객체를 설정하자.
	*/
	virtual void SetRenderingFunc() override;

public:
	virtual void Start() override;
};

