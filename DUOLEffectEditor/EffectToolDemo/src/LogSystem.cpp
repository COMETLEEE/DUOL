#include "LogSystem.h"

LogSystem* LogSystem::_instance(nullptr);

LogSystem::LogSystem(std::shared_ptr<Muscle::GameObject> _gameObject) : ImGuiRnedererBase(_gameObject)
{
	if (_instance)
		assert(false);

	_instance = this;

	Clear();

	_lineOffsets.push_back(0);
}
LogSystem::~LogSystem()
{
	_instance = nullptr;

	Clear();
}
void LogSystem::Clear()
{
	_buf.clear();
	_lineOffsets.clear();
}

void LogSystem::AddLog(const char* fmt, ...)
{
 	int old_size = _instance->_buf.size();

	va_list args;

	va_start(args, fmt);

	_instance->_buf.appendfv(fmt, args);

	va_end(args);

	for (int new_size = _instance->_buf.size(); old_size < new_size; old_size++)
		if (_instance->_buf[old_size] == '\n')
			_instance->_lineOffsets.push_back(old_size + 1);
}

void LogSystem::Draw()
{

	bool clear = ImGui::Button("Clear");

	ImGui::SameLine();

	bool copy = ImGui::Button("Copy");

	ImGui::SameLine();

	ImGui::Separator();

	if (ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
	{
		if (clear)
			Clear();
		if (copy)
			ImGui::LogToClipboard();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		const char* buf = _buf.begin();

		const char* buf_end = _buf.end();

		ImGuiListClipper clipper;

		clipper.Begin(_lineOffsets.Size);

		while (clipper.Step())
		{
			for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
			{
				const char* line_start = buf + _lineOffsets[line_no];

				const char* line_end = (line_no + 1 < _lineOffsets.Size) ? (buf + _lineOffsets[line_no + 1] - 1) : buf_end;

				ImGui::TextUnformatted(line_start, line_end);
			}
		}
		clipper.End();

		ImGui::PopStyleVar();

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);
	}
	ImGui::EndChild();
}

void LogSystem::SetRenderingFunc()
{
	auto temp = [&]()
	{
		ImGui::Begin("__Log__");

		_instance->Draw();

		ImGui::End();
	};
	_renderingFunc = temp;
}

void LogSystem::Start()
{
	SetRenderingFunc();
}