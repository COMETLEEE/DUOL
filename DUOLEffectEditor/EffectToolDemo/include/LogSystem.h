#pragma once

#include "ImGuiRnedererBase.h"
#include "../Common/Imgui/imgui.h"

#define WriteLog(str,...) LogSystem::AddLog(str,__VA_ARGS__)

/// �̱��� ��ü������, ������ �ϱ� ���ؼ� ������Ʈ�� ���� ���Ѿ��Ѵ�.

class LogSystem : public Muscle::ImGuiRnedererBase
{
public:
	LogSystem(std::shared_ptr<Muscle::GameObject> _gameObject);

	~LogSystem();

private:
	ImGuiTextBuffer     _buf;

	ImVector<int>       _lineOffsets; // Index to lines offset. We maintain this with AddLog() calls.

	static LogSystem _instance;

	void Clear();

	void Draw();

	static bool _isDelete;

public:
	static void AddLog(const char* fmt, ...) IM_FMTARGS(2); //IM_FMTARGS(2);

protected:
	/**
	* \brief �� �Լ��� �׷��Ƚ��� �Ѱ��� �Լ� ��ü�� ��������.
	*/
	virtual void SetRenderingFunc() override;

public:
	virtual void Start() override;
};

