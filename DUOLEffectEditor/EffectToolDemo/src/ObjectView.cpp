#include "ObjectView.h"
#include "../Common/Imgui/imgui.h"
#include "ParticleObjectManager.h"
#include "EffectEditorManager.h"
#include "LogSystem.h"
#include "ObjectManager.h"
#include "IGameEngine.h"
#include "Commands.h"

constexpr  ImGuiTreeNodeFlags BASE_FLAGS = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;

ObjectView::ObjectView(std::shared_ptr<Muscle::GameObject> _gameObject) :
	ImGuiRnedererBase(_gameObject),
	_parents(nullptr),
	_child(nullptr)
{
}

void ObjectView::ItemRowsBackground(float lineHeight = -1.0f, const ImColor& color = ImColor(200, 200, 200, 64))
{
	auto* drawList = ImGui::GetWindowDrawList();

	const auto& style = ImGui::GetStyle();

	if (lineHeight < 0)
		lineHeight = ImGui::GetTextLineHeight();

	lineHeight += style.ItemSpacing.y;

	float scrollOffsetH = ImGui::GetScrollX();

	float scrollOffsetV = ImGui::GetScrollY();

	float scrolledOutLines = floorf(scrollOffsetV / lineHeight);

	scrollOffsetV -= lineHeight * scrolledOutLines;

	ImVec2 clipRectMin(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);

	ImVec2 clipRectMax(clipRectMin.x + ImGui::GetWindowWidth(), clipRectMin.y + ImGui::GetWindowHeight());

	if (ImGui::GetScrollMaxX() > 0)
		clipRectMax.y -= style.ScrollbarSize;

	drawList->PushClipRect(clipRectMin, clipRectMax);

	bool isOdd = (static_cast<int>(scrolledOutLines) % 2) == 0;

	float yMin = clipRectMin.y - scrollOffsetV + ImGui::GetCursorPosY();

	float yMax = clipRectMax.y - scrollOffsetV + lineHeight;

	float xMin = clipRectMin.x + scrollOffsetH + ImGui::GetWindowContentRegionMin().x;

	float xMax = clipRectMin.x + scrollOffsetH + ImGui::GetWindowContentRegionMax().x;

	for (float y = yMin; y < yMax; y += lineHeight, isOdd = !isOdd)
	{
		if (isOdd)
		{
			drawList->AddRectFilled({ xMin, y - style.ItemSpacing.y }, { xMax, y + lineHeight }, color);
		}
	}

	drawList->PopClipRect();
}

void ObjectView::DrawTree_AllObject()
{
	static int selection_mask = (1 << 2);
	int node_clicked = -1;

	auto& objects = ParticleObjectManager::Get().GetParticleObjects();

	_parents = nullptr;

	_child = nullptr;

	for (auto& iter : objects)
	{
		if (iter.second->GetParent()) continue;
		if (!iter.second->GetIsEnable()) continue;
		ShowObject(iter.second);
	}
	if (_child || _parents)
	{
		EXCUTE(new SetParentsCommand(_child, _parents));
	}

}

void ObjectView::ShowObject(const std::shared_ptr<Muscle::GameObject>& gameObject)
{
	ImGuiTreeNodeFlags node_flags = BASE_FLAGS;
	if (!gameObject->GetIsEnable()) return;


	if (gameObject->GetChildrens().empty())
	{
		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		ImGui::TreeNodeEx(gameObject.get(), node_flags, gameObject->GetName().c_str());

		if (ImGui::BeginDragDropSource()) // 드래그 관련.
		{
			UINT n = gameObject->GetObjectID();
			ImGui::SetDragDropPayload("_NODE", &n, sizeof(UINT));

			ImGui::Text("This is a drag and drop source");

			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemClicked())
		{
			EXCUTE(new SelectObjectCommand(gameObject));
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_NODE"))
			{
				if (_child || _parents)
				{

				}
				else
				{
					IM_ASSERT(payload->DataSize == sizeof(UINT));
					UINT objectID = *(const UINT*)payload->Data;
					WriteLog("child : %d \n", objectID);
					WriteLog("parents : %d \n", gameObject->GetObjectID());
					_parents = gameObject;
					_child = Muscle::IGameEngine::Get()->GetObjManager()->GetGameObject(objectID);
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
	else
	{
		bool node_open = ImGui::TreeNodeEx(gameObject.get(), node_flags, gameObject->GetName().c_str());

		if (ImGui::BeginDragDropSource()) // 드래그 관련.
		{
			UINT n = gameObject->GetObjectID();
			ImGui::SetDragDropPayload("_NODE", &n, sizeof(UINT));

			ImGui::Text("This is a drag and drop source");

			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemClicked())
		{
			EXCUTE(new SelectObjectCommand(gameObject));
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_NODE"))
			{
				if (_child || _parents)
				{

				}
				else
				{
					IM_ASSERT(payload->DataSize == sizeof(UINT));
					UINT objectID = *(const UINT*)payload->Data;
					WriteLog("child : %d \n", objectID);
					WriteLog("parents : %d \n", gameObject->GetObjectID());
					_parents = gameObject;
					_child = Muscle::IGameEngine::Get()->GetObjManager()->GetGameObject(objectID);
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (node_open)
		{
			if (!gameObject->GetChildrens().empty())
			{
				for (int i = 0; i < gameObject->GetChildrens().size(); i++)
					ShowObject(gameObject->GetChildrens()[i]);
			}
			ImGui::TreePop();
		}


	}




}


void ObjectView::SetRenderingFunc()
{
	auto temp = [&]()
	{

		ImGui::Begin("ObjectView");
		ItemRowsBackground();
		DrawTree_AllObject();
		ImGui::End();


	};
	_renderingFunc = temp;
}

void ObjectView::Start()
{
	SetRenderingFunc();
}
