#include <filesystem>

#include "DUOLFBXParser/DUOLFBXParser.h"
#include "DUOLFBXParser/DUOLFBXData.h"

DUOLParser::DUOLFBXParser::DUOLFBXParser()
{
}

DUOLParser::DUOLFBXParser::~DUOLFBXParser()
{
	Destory();
}

std::shared_ptr<FBXModel> DUOLParser::DUOLFBXParser::LoadFBX(const std::string& path)
{
	_fbxModel = std::make_shared<FBXModel>();

	Initialize();

	LoadScene(path);

	return _fbxModel;
}

/**
 * \brief Create만 담당한다.
 */
void DUOLParser::DUOLFBXParser::Initialize()
{
	// fbxManager의 인스턴스를 생성한다. 
	_fbxManager = fbxsdk::FbxManager::Create();

	fbxsdk::FbxIOSettings* ios = fbxsdk::FbxIOSettings::Create(_fbxManager, IOSROOT);
	_fbxManager->SetIOSettings(ios);

	// Scene을 만들어준다. 
	_fbxScene = fbxsdk::FbxScene::Create(_fbxManager, "");

	_fbxImporter = fbxsdk::FbxImporter::Create(_fbxManager, "");
}

void DUOLParser::DUOLFBXParser::Destory()
{
	_fbxScene->Destroy();
	_fbxImporter->Destroy();
	_fbxManager->Destroy();
}

void DUOLParser::DUOLFBXParser::LoadScene(std::string path)
{
	bool success = _fbxImporter->Initialize(path.c_str(), -1, _fbxManager->GetIOSettings());

	// Scene을 가져와서 넣어준다.
	// Scene에다가 모델을 풀어놓음
	// 이제 본과 애니메이션 매쉬를 로드할 수 있음
	_fbxImporter->Import(_fbxScene);

	// 좌표축을 가져온다.
	FbxAxisSystem sceneAxisSystem = _fbxScene->GetGlobalSettings().GetAxisSystem();

	// 씬 내의 좌표축을 바꾼다.
	_fbxScene->GetGlobalSettings().SetAxisSystem(fbxsdk::FbxAxisSystem::DirectX);

	// 씬 내에서 삼각형화 할 수 있는 모든 노드를 삼각형화 시킨다. 
	FbxGeometryConverter* geometryConverter = new FbxGeometryConverter(_fbxManager);

	geometryConverter->Triangulate(_fbxScene, true);

	// scene을 가져오는게 성공하면 노드를 순회해서 씬에 저장되어 있는 트리구조에서 오브젝트를 찾아옴
	if (success)
	{
		ProcessNode(_fbxScene->GetRootNode());
	}
}

/**
 * \brief 트리 구조이기 때문에 루트 노드로부터 재귀 함수를 통해 오브젝트 개수와 종류를 알아옴
 * \param parentnode
 * \param node
 */
void DUOLParser::DUOLFBXParser::ProcessNode(fbxsdk::FbxNode* node)
{
	fbxsdk::FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
	const char* name = node->GetName();

	if (nodeAttribute)
	{
		// node의 타입을 받아온다.
		fbxsdk::FbxNodeAttribute::EType attributeType = nodeAttribute->GetAttributeType();

		switch (nodeAttribute->GetAttributeType())
		{
		case fbxsdk::FbxNodeAttribute::eMesh:
		{
			// Mesh를 불러온다.
			ProcessMesh(node);
			// Material을 불러온다.
			ProcessMaterial(node);
		}
		break;
		// 만약 카메라거나 라이트이면 받아올 필요없으니 리턴한다.
		case fbxsdk::FbxNodeAttribute::eLight:
		{
			return;
		}
		case fbxsdk::FbxNodeAttribute::eCamera:
		{	return;

		}
		break;
		default:
			break;
		}
	}

	int count = node->GetChildCount();

	// 재귀 구조로 모든 자식을 다 돈다. 
	for (int i = 0; i < count; i++)
	{
		ProcessNode(node->GetChild(i));
	}
}

/**
 * \brief Get Mesh Data(pos, UV, normal, tangent, binormal)
 * \param node
 */
void DUOLParser::DUOLFBXParser::ProcessMesh(FbxNode* node)
{
	// Mesh를 뽑는다.
	fbxsdk::FbxMesh* currentMesh = node->GetMesh();
	fbxsdk::FbxGeometry* geometry = node->GetGeometry();

	// Mesh를 생성해준다. 
	std::shared_ptr<DuolData::Mesh> meshinfo = std::make_shared<DuolData::Mesh>();

	// model에 Mesh를 넣어준다.
	_fbxModel->fbxMeshList.emplace_back(meshinfo);

	// 기존에 있는 geometry인지 체크한다. 
	//bool instancing = false;

	//for (int i = 0; i < _fbxGeometryList.size(); i++)
	//{
	//	// 기존에 있는 geometry이다.
	//	// 내가 보기엔 대충 씬에 같은 geometry가 여러개 있을때를 의미하는듯
	//	// 근데 Mesh 기준이 아니라 Geometry 기준인 이유를 모르겠다.
	//	// 이거 Mesh로 바꾸고(어짜피 MeshList도 있으니께) 테스트 한번 해보기
	//	if (geometry == _fbxGeometryList[i])
	//	{
	//		instancing = true;
	//		// geometry가 이미 존재하므로 처리할 필요가 없으니깐 함수를 빠져나간다.
	//		break;
	//	}
	//}

	//// GeometryList안에다가 넣어준다. 
	//_fbxGeometryList.emplace_back(geometry);

	meshinfo->nodeName = node->GetName();

	// node의 Parent 찾기
	// 근데 이게 Mesh의 부모인지 아니면 그 트리구조의 부모인지 모르겠다.
	// 만약 트리구조의 부모라면 FindMesh로 Mesh에서만 찾아야함.
	fbxsdk::FbxMesh* parentMesh = node->GetParent()->GetMesh();

	// Mesh의 부모 노드가 존재하는지 여부 판단
	if (parentMesh == nullptr)
	{
		meshinfo->isparent = false;
	}
	// 만약 부모가 있다면 이름을 가져와서 넣어준다.
	else
	{
		// 부모가 있다. 
		meshinfo->isparent = true;
		// 부모 이름을 가져와서 넣어준다.
		const char* parentName = node->GetParent()->GetName();
		meshinfo->parentName = parentName;
		// 부모 Mesh를 넣어준다. 
		std::shared_ptr<DuolData::Mesh> nodeparentmesh = FindMesh(parentName);
		meshinfo->parentMesh = nodeparentmesh;
		// 부모에 자식으로 넣어준다.
		nodeparentmesh->childList.emplace_back(meshinfo);
	}

	// Node TRS 설정
	//SetTransform(node);

	// Mesh Vertex
	// fbxsdk에서는 vertex를 controlpoint라고 한다. 
	const int vertexcount = currentMesh->GetControlPointsCount();

	// Mesh에서 삼각형 개수를 가져온다.
	// fbxsdk 에서는 face를 polygon이라고 한다.
	const int facecount = currentMesh->GetPolygonCount();

	// vertex 가 없으면 처리 X
	if (vertexcount < 1)
		return;

	meshinfo->vertexList.resize(vertexcount);

	// pos 정보를 가져온다. (축을 바꿔야함)
	fbxsdk::FbxVector4* controlpoints = currentMesh->GetControlPoints();

	// Vertex는 잘 들어감을 확인했다.
	for (int i = 0; i < vertexcount; i++)
	{
		meshinfo->vertexList[i].position.x = static_cast<float>(controlpoints[i].mData[0]);
		meshinfo->vertexList[i].position.y = static_cast<float>(controlpoints[i].mData[1]);
		meshinfo->vertexList[i].position.z = static_cast<float>(controlpoints[i].mData[2]);
	}

	// vertex 갯수
	int vertexcounter = 0;

	// 버텍스 쪼개기를 하기위한 체크 요소이다.
	std::vector<bool> isvertex(vertexcount, false);

	// face 갯수만큼 size를 지정해줌(삼각형의 개수)
	// face랑 mesh를 어떤식으로 연결할꺼냐 생각해봐야함
	meshinfo->MeshFace.resize(facecount);

	for (unsigned int i = 0; i < facecount; i++)
	{
		meshinfo->MeshFace[i] = std::make_shared <DuolData::Face>();

		std::shared_ptr<DuolData::Face> nowface = meshinfo->MeshFace[i];

		// 삼각형은 세개의 정점으로 구성되어 있음
		// 여기서 버텍스 쪼개기를 해줘야 한다. 
		for (unsigned int j = 0; j < 3; j++)
		{
			// i번째 Face에 j번째 버텍스 인덱스를 가져온다.
			int controlpointIndex = currentMesh->GetPolygonVertex(i, j);
			int uvindex = currentMesh->GetTextureUVIndex(i, j);

			// 버텍스 쪼개기
			if (isvertex[controlpointIndex] == true)
			{
				// new vertex만들기
				DuolData::Vertex vertex;

				vertex.position = meshinfo->vertexList[controlpointIndex].position;

				meshinfo->vertexList.push_back(vertex);

				controlpointIndex = meshinfo->vertexList.size() - 1;

				isvertex.push_back(true);
			}

			// 노말을 불러온다.
			GetNormal(currentMesh, meshinfo, controlpointIndex, vertexcounter);
			GetUV(currentMesh, meshinfo, controlpointIndex, vertexcounter);

			// Index를 넣어준다.
			nowface->vertexIndex[j] = controlpointIndex;

			// face에 관한걸 넣어준다. 
			//nowface->vertexNormal[j] = fbxnormal;
			//nowface->vertexUV[j] = fbxuv;
			//meshinfo->vertexList[vertexcounter].uv = fbxuv;

			vertexcounter++;
			// 이 버텍스는 이미 접근했어요 체크
			isvertex[controlpointIndex] = true;
		}

		meshinfo->indices.emplace_back(nowface->vertexIndex[0]);
		meshinfo->indices.emplace_back(nowface->vertexIndex[1]);
		meshinfo->indices.emplace_back(nowface->vertexIndex[2]);
	}
}

/**
 * \brief Material Count 만큼 돌면서 node의 material을 불러온다.
 * \param node
 */
void DUOLParser::DUOLFBXParser::ProcessMaterial(FbxNode* node)
{
	// Material Count로 갯수를 불러온다. 
	const int materialcount = node->GetMaterialCount();

	for (int i = 0; i < materialcount; i++)
	{
		fbxsdk::FbxSurfaceMaterial* surfaceMaterial = node->GetMaterial(i);

		LoadMaterial(surfaceMaterial);
	}
}

void DUOLParser::DUOLFBXParser::LoadMaterial(const fbxsdk::FbxSurfaceMaterial* surfacematerial)
{
	std::shared_ptr<DuolData::Material> material = std::make_shared<DuolData::Material>();

	material->materialName = surfacematerial->GetName();

	material->albedoMap = GetTextureName(surfacematerial, fbxsdk::FbxSurfaceMaterial::sDiffuse);
	material->normalMap = GetTextureName(surfacematerial, fbxsdk::FbxSurfaceMaterial::sNormalMap);
	material->roughnessMap = GetTextureName(surfacematerial, fbxsdk::FbxSurfaceMaterial::sShininess);
	material->emissiveMap = GetTextureName(surfacematerial, fbxsdk::FbxSurfaceMaterial::sEmissive);

	if (material->albedoMap != L"")
		material->isAlbedo = true;

	if (material->normalMap != L"")
		material->isNormal = true;

	if (material->roughnessMap != L"")
		material->isRoughness = true;

	if (material->emissiveMap != L"")
		material->isEmissive = true;

	// Mesh에는 material Name만 넣어준다.
	_fbxModel->fbxMeshList.back()->materialName = surfacematerial->GetName();

	// Material은 fbxmodel이 가지고 있는다. 
	_fbxModel->fbxmaterialList.emplace_back(material);
}

std::wstring DUOLParser::DUOLFBXParser::GetTextureName(const fbxsdk::FbxSurfaceMaterial* surfacematerial, const char* materialproperty)
{
	std::string name;
	// 텍스쳐를 로드하려면 먼저 유효한 property를 검색한다
	fbxsdk::FbxProperty textureproperty = surfacematerial->FindProperty(materialproperty);

	if (textureproperty.IsValid())
	{
		// 연결되어있는 맵의 개수인듯(?)
		int count = textureproperty.GetSrcObjectCount();

		if (0 < count)
		{
			// 연결되어 있는 지정된 인덱스의 개체를 반환함
			// 기본 인덱스는 0 인듯
			fbxsdk::FbxFileTexture* texture = textureproperty.GetSrcObject<fbxsdk::FbxFileTexture>(0);
			// name을 이상하게 받아와서 경로가 이상해진다.
			if (texture)
				name = texture->GetRelativeFileName();
		}
	}

	std::wstring wname = L"";

	wname.assign(name.begin(), name.end());

	// 일단 예제는 Texture\\2048\\texturename 이렇게 나오므로 texturename이 나오게 수정이 필요하다.
	// 근데 path.filename을 쓰면 파일 이름만 뽑아준다.
	std::wstring filename = std::filesystem::path(wname).filename();

	return filename;
}

/**
 * \brief 메시의 최적화를 해준다.
 *		노말값, 텍스쳐 좌표에 따라 곂치지 않으면 늘리고, 중첩되면 지운다.
 * \return
 */
bool DUOLParser::DUOLFBXParser::ConvertOptimize(std::shared_ptr<DuolData::Mesh>)
{
	return true;
}

std::shared_ptr<DuolData::Mesh> DUOLParser::DUOLFBXParser::FindMesh(const std::string nodename)
{
	for (auto mesh : _fbxModel->fbxMeshList)
	{
		if (mesh->nodeName == nodename)
			return mesh;
	}
	return nullptr;
}

void DUOLParser::DUOLFBXParser::GetNormal(fbxsdk::FbxMesh* mesh, std::shared_ptr<DuolData::Mesh>  meshinfo, int controlpointindex, int vertexindex)
{
	// 여기서 예외처리 해주기
	if (mesh->GetElementNormalCount() < 1)
		return;

	// 노말 획득
	const fbxsdk::FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
	int index;

#pragma region MappingMode
	// 매핑 모드
	switch (vertexNormal->GetMappingMode())
	{
		// control point mapping
	case fbxsdk::FbxGeometryElement::eByControlPoint:
	{
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			index = controlpointindex;
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			// 우리가 필요한 인덱스를 뽑아준다.
			index = vertexNormal->GetIndexArray().GetAt(controlpointindex);
		}
		break;
		default:
			break;
		}
	}
	break;
	case fbxsdk::FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			index = vertexindex;
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			// 우리가 필요한 인덱스를 뽑아준다.
			index = vertexNormal->GetIndexArray().GetAt(vertexindex);
		}
		break;
		default:
			break;
		}
	}
	break;
	default:
		break;
	}
#pragma endregion

	meshinfo->vertexList[controlpointindex].normal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);;
	meshinfo->vertexList[controlpointindex].normal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);;
	meshinfo->vertexList[controlpointindex].normal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);;
}


void DUOLParser::DUOLFBXParser::GetUV(fbxsdk::FbxMesh* mesh, std::shared_ptr<DuolData::Mesh>  meshinfo, int controlpointindex, int vertexindex)
{
	if (mesh->GetElementUVCount() < 1)
		return;

	int index;

#pragma region MapponMode
	// Layer에 있는 UV를 가져온다.
	fbxsdk::FbxGeometryElementUV* vertexuv = mesh->GetLayer(0)->GetUVs();

	// Mapping mode -> index / reference mode -> data
	switch (vertexuv->GetMappingMode())
	{
		// Surface Control Point를 위해 하나의 mapping cordinate만 존재
	case FbxGeometryElement::eByControlPoint:
	{
		// Key값의 차이이다.
		switch (vertexuv->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			// control point index를 구한다음(vertexindex) 이를 key값으로해서 direct array에 직접 uv data접근
			index = controlpointindex;
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			// index array에서 실제 index를 획득한 다음에 data를 획득한다.
			index = vertexuv->GetIndexArray().GetAt(controlpointindex);
		}
		break;
		default:
			break;
		}
	}
	break;
	// vertex가 구성하는 polygon만큼의 mapping coordinate를 가지게 된다.
	// 정확히 무슨말인지는 잘 모르겠다. 
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexuv->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			index = vertexindex;
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			index = vertexuv->GetIndexArray().GetAt(vertexindex);
		}
		break;
		default:
			break;
		}
	}
	break;
	default:
		break;
	}
#pragma endregion
	meshinfo->vertexList[controlpointindex].uv.x = static_cast<float>(vertexuv->GetDirectArray().GetAt(index).mData[0]);
	meshinfo->vertexList[controlpointindex].uv.y = 1.0f - static_cast<float>(vertexuv->GetDirectArray().GetAt(index).mData[1]);
}