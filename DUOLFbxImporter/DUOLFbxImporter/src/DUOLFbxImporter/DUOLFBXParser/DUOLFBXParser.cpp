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
 * \brief Create�� ����Ѵ�.
 */
void DUOLParser::DUOLFBXParser::Initialize()
{
	// fbxManager�� �ν��Ͻ��� �����Ѵ�. 
	_fbxManager = fbxsdk::FbxManager::Create();

	fbxsdk::FbxIOSettings* ios = fbxsdk::FbxIOSettings::Create(_fbxManager, IOSROOT);
	_fbxManager->SetIOSettings(ios);

	// Scene�� ������ش�. 
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

	// Scene�� �����ͼ� �־��ش�.
	// Scene���ٰ� ���� Ǯ�����
	// ���� ���� �ִϸ��̼� �Ž��� �ε��� �� ����
	_fbxImporter->Import(_fbxScene);

	// ��ǥ���� �����´�.
	FbxAxisSystem sceneAxisSystem = _fbxScene->GetGlobalSettings().GetAxisSystem();

	// �� ���� ��ǥ���� �ٲ۴�.
	_fbxScene->GetGlobalSettings().SetAxisSystem(fbxsdk::FbxAxisSystem::DirectX);

	// �� ������ �ﰢ��ȭ �� �� �ִ� ��� ��带 �ﰢ��ȭ ��Ų��. 
	FbxGeometryConverter* geometryConverter = new FbxGeometryConverter(_fbxManager);

	geometryConverter->Triangulate(_fbxScene, true);

	// scene�� �������°� �����ϸ� ��带 ��ȸ�ؼ� ���� ����Ǿ� �ִ� Ʈ���������� ������Ʈ�� ã�ƿ�
	if (success)
	{
		ProcessNode(_fbxScene->GetRootNode());
	}
}

/**
 * \brief Ʈ�� �����̱� ������ ��Ʈ ���κ��� ��� �Լ��� ���� ������Ʈ ������ ������ �˾ƿ�
 * \param parentnode
 * \param node
 */
void DUOLParser::DUOLFBXParser::ProcessNode(fbxsdk::FbxNode* node)
{
	fbxsdk::FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
	const char* name = node->GetName();

	if (nodeAttribute)
	{
		// node�� Ÿ���� �޾ƿ´�.
		fbxsdk::FbxNodeAttribute::EType attributeType = nodeAttribute->GetAttributeType();

		switch (nodeAttribute->GetAttributeType())
		{
		case fbxsdk::FbxNodeAttribute::eMesh:
		{
			// Mesh�� �ҷ��´�.
			ProcessMesh(node);
			// Material�� �ҷ��´�.
			ProcessMaterial(node);
		}
		break;
		// ���� ī�޶�ų� ����Ʈ�̸� �޾ƿ� �ʿ������ �����Ѵ�.
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

	// ��� ������ ��� �ڽ��� �� ����. 
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
	// Mesh�� �̴´�.
	fbxsdk::FbxMesh* currentMesh = node->GetMesh();
	fbxsdk::FbxGeometry* geometry = node->GetGeometry();

	// Mesh�� �������ش�. 
	std::shared_ptr<DuolData::Mesh> meshinfo = std::make_shared<DuolData::Mesh>();

	// model�� Mesh�� �־��ش�.
	_fbxModel->fbxMeshList.emplace_back(meshinfo);

	// ������ �ִ� geometry���� üũ�Ѵ�. 
	//bool instancing = false;

	//for (int i = 0; i < _fbxGeometryList.size(); i++)
	//{
	//	// ������ �ִ� geometry�̴�.
	//	// ���� ���⿣ ���� ���� ���� geometry�� ������ �������� �ǹ��ϴµ�
	//	// �ٵ� Mesh ������ �ƴ϶� Geometry ������ ������ �𸣰ڴ�.
	//	// �̰� Mesh�� �ٲٰ�(��¥�� MeshList�� �����ϲ�) �׽�Ʈ �ѹ� �غ���
	//	if (geometry == _fbxGeometryList[i])
	//	{
	//		instancing = true;
	//		// geometry�� �̹� �����ϹǷ� ó���� �ʿ䰡 �����ϱ� �Լ��� ����������.
	//		break;
	//	}
	//}

	//// GeometryList�ȿ��ٰ� �־��ش�. 
	//_fbxGeometryList.emplace_back(geometry);

	meshinfo->nodeName = node->GetName();

	// node�� Parent ã��
	// �ٵ� �̰� Mesh�� �θ����� �ƴϸ� �� Ʈ�������� �θ����� �𸣰ڴ�.
	// ���� Ʈ�������� �θ��� FindMesh�� Mesh������ ã�ƾ���.
	fbxsdk::FbxMesh* parentMesh = node->GetParent()->GetMesh();

	// Mesh�� �θ� ��尡 �����ϴ��� ���� �Ǵ�
	if (parentMesh == nullptr)
	{
		meshinfo->isparent = false;
	}
	// ���� �θ� �ִٸ� �̸��� �����ͼ� �־��ش�.
	else
	{
		// �θ� �ִ�. 
		meshinfo->isparent = true;
		// �θ� �̸��� �����ͼ� �־��ش�.
		const char* parentName = node->GetParent()->GetName();
		meshinfo->parentName = parentName;
		// �θ� Mesh�� �־��ش�. 
		std::shared_ptr<DuolData::Mesh> nodeparentmesh = FindMesh(parentName);
		meshinfo->parentMesh = nodeparentmesh;
		// �θ� �ڽ����� �־��ش�.
		nodeparentmesh->childList.emplace_back(meshinfo);
	}

	// Node TRS ����
	//SetTransform(node);

	// Mesh Vertex
	// fbxsdk������ vertex�� controlpoint��� �Ѵ�. 
	const int vertexcount = currentMesh->GetControlPointsCount();

	// Mesh���� �ﰢ�� ������ �����´�.
	// fbxsdk ������ face�� polygon�̶�� �Ѵ�.
	const int facecount = currentMesh->GetPolygonCount();

	// vertex �� ������ ó�� X
	if (vertexcount < 1)
		return;

	meshinfo->vertexList.resize(vertexcount);

	// pos ������ �����´�. (���� �ٲ����)
	fbxsdk::FbxVector4* controlpoints = currentMesh->GetControlPoints();

	// Vertex�� �� ���� Ȯ���ߴ�.
	for (int i = 0; i < vertexcount; i++)
	{
		meshinfo->vertexList[i].position.x = static_cast<float>(controlpoints[i].mData[0]);
		meshinfo->vertexList[i].position.y = static_cast<float>(controlpoints[i].mData[1]);
		meshinfo->vertexList[i].position.z = static_cast<float>(controlpoints[i].mData[2]);
	}

	// vertex ����
	int vertexcounter = 0;

	// ���ؽ� �ɰ��⸦ �ϱ����� üũ ����̴�.
	std::vector<bool> isvertex(vertexcount, false);

	// face ������ŭ size�� ��������(�ﰢ���� ����)
	// face�� mesh�� ������� �����Ҳ��� �����غ�����
	meshinfo->MeshFace.resize(facecount);

	for (unsigned int i = 0; i < facecount; i++)
	{
		meshinfo->MeshFace[i] = std::make_shared <DuolData::Face>();

		std::shared_ptr<DuolData::Face> nowface = meshinfo->MeshFace[i];

		// �ﰢ���� ������ �������� �����Ǿ� ����
		// ���⼭ ���ؽ� �ɰ��⸦ ����� �Ѵ�. 
		for (unsigned int j = 0; j < 3; j++)
		{
			// i��° Face�� j��° ���ؽ� �ε����� �����´�.
			int controlpointIndex = currentMesh->GetPolygonVertex(i, j);
			int uvindex = currentMesh->GetTextureUVIndex(i, j);

			// ���ؽ� �ɰ���
			if (isvertex[controlpointIndex] == true)
			{
				// new vertex�����
				DuolData::Vertex vertex;

				vertex.position = meshinfo->vertexList[controlpointIndex].position;

				meshinfo->vertexList.push_back(vertex);

				controlpointIndex = meshinfo->vertexList.size() - 1;

				isvertex.push_back(true);
			}

			// �븻�� �ҷ��´�.
			GetNormal(currentMesh, meshinfo, controlpointIndex, vertexcounter);
			GetUV(currentMesh, meshinfo, controlpointIndex, vertexcounter);

			// Index�� �־��ش�.
			nowface->vertexIndex[j] = controlpointIndex;

			// face�� ���Ѱ� �־��ش�. 
			//nowface->vertexNormal[j] = fbxnormal;
			//nowface->vertexUV[j] = fbxuv;
			//meshinfo->vertexList[vertexcounter].uv = fbxuv;

			vertexcounter++;
			// �� ���ؽ��� �̹� �����߾�� üũ
			isvertex[controlpointIndex] = true;
		}

		meshinfo->indices.emplace_back(nowface->vertexIndex[0]);
		meshinfo->indices.emplace_back(nowface->vertexIndex[1]);
		meshinfo->indices.emplace_back(nowface->vertexIndex[2]);
	}
}

/**
 * \brief Material Count ��ŭ ���鼭 node�� material�� �ҷ��´�.
 * \param node
 */
void DUOLParser::DUOLFBXParser::ProcessMaterial(FbxNode* node)
{
	// Material Count�� ������ �ҷ��´�. 
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

	// Mesh���� material Name�� �־��ش�.
	_fbxModel->fbxMeshList.back()->materialName = surfacematerial->GetName();

	// Material�� fbxmodel�� ������ �ִ´�. 
	_fbxModel->fbxmaterialList.emplace_back(material);
}

std::wstring DUOLParser::DUOLFBXParser::GetTextureName(const fbxsdk::FbxSurfaceMaterial* surfacematerial, const char* materialproperty)
{
	std::string name;
	// �ؽ��ĸ� �ε��Ϸ��� ���� ��ȿ�� property�� �˻��Ѵ�
	fbxsdk::FbxProperty textureproperty = surfacematerial->FindProperty(materialproperty);

	if (textureproperty.IsValid())
	{
		// ����Ǿ��ִ� ���� �����ε�(?)
		int count = textureproperty.GetSrcObjectCount();

		if (0 < count)
		{
			// ����Ǿ� �ִ� ������ �ε����� ��ü�� ��ȯ��
			// �⺻ �ε����� 0 �ε�
			fbxsdk::FbxFileTexture* texture = textureproperty.GetSrcObject<fbxsdk::FbxFileTexture>(0);
			// name�� �̻��ϰ� �޾ƿͼ� ��ΰ� �̻�������.
			if (texture)
				name = texture->GetRelativeFileName();
		}
	}

	std::wstring wname = L"";

	wname.assign(name.begin(), name.end());

	// �ϴ� ������ Texture\\2048\\texturename �̷��� �����Ƿ� texturename�� ������ ������ �ʿ��ϴ�.
	// �ٵ� path.filename�� ���� ���� �̸��� �̾��ش�.
	std::wstring filename = std::filesystem::path(wname).filename();

	return filename;
}

/**
 * \brief �޽��� ����ȭ�� ���ش�.
 *		�븻��, �ؽ��� ��ǥ�� ���� ��ġ�� ������ �ø���, ��ø�Ǹ� �����.
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
	// ���⼭ ����ó�� ���ֱ�
	if (mesh->GetElementNormalCount() < 1)
		return;

	// �븻 ȹ��
	const fbxsdk::FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
	int index;

#pragma region MappingMode
	// ���� ���
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
			// �츮�� �ʿ��� �ε����� �̾��ش�.
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
			// �츮�� �ʿ��� �ε����� �̾��ش�.
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
	// Layer�� �ִ� UV�� �����´�.
	fbxsdk::FbxGeometryElementUV* vertexuv = mesh->GetLayer(0)->GetUVs();

	// Mapping mode -> index / reference mode -> data
	switch (vertexuv->GetMappingMode())
	{
		// Surface Control Point�� ���� �ϳ��� mapping cordinate�� ����
	case FbxGeometryElement::eByControlPoint:
	{
		// Key���� �����̴�.
		switch (vertexuv->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			// control point index�� ���Ѵ���(vertexindex) �̸� key�������ؼ� direct array�� ���� uv data����
			index = controlpointindex;
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			// index array���� ���� index�� ȹ���� ������ data�� ȹ���Ѵ�.
			index = vertexuv->GetIndexArray().GetAt(controlpointindex);
		}
		break;
		default:
			break;
		}
	}
	break;
	// vertex�� �����ϴ� polygon��ŭ�� mapping coordinate�� ������ �ȴ�.
	// ��Ȯ�� ������������ �� �𸣰ڴ�. 
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