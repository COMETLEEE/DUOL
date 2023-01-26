#include "DUOLFBXImporter/DUOLFBXImporter.h"

#include <filesystem>
#include <map>
#include <string>

#include "DUOLFBXImporter/ParserData/DUOLFBXData.h"

//#include "Serialize/BinarySerialize.h"

bool DUOLParser::DUOLFBXParser::_isNoMaterial = false;

DUOLParser::DUOLFBXParser::DUOLFBXParser()
{
}

DUOLParser::DUOLFBXParser::~DUOLFBXParser()
{
	Destory();
}

std::shared_ptr<FBXModel> DUOLParser::DUOLFBXParser::LoadFBX(const std::string& path, const std::string& modelname)
{
	_fbxModel = std::make_shared<FBXModel>();

	_fbxModel->fileName = path;

	_fbxModel->modelName = modelname;

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

	LoadDefaultMaterial();
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

	if (!success)
	{
		FbxString error = _fbxImporter->GetStatus().GetErrorString();

		FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
		FBXSDK_printf("error model is %s\n", path.c_str());
		FBXSDK_printf("Error returned: %s\n\n", error.Buffer());
	}

	// Scene�� �����ͼ� �־��ش�.
	// Scene���ٰ� ���� Ǯ�����
	// ���� ���� �ִϸ��̼� �Ž��� �ε��� �� ����
	_fbxImporter->Import(_fbxScene);

	// ���� ũ�Ⱑ ũ�Գ��ͼ� �װ� �����ϴ°�
	// �ٵ� ��� ���������� ������ �Ǵ��� �𸣰ڴ�,,??
	// ���� ��� �׷���..
	fbxsdk::FbxSystemUnit lFbxFileSysteomUnit = _fbxScene->GetGlobalSettings().GetSystemUnit();
	fbxsdk::FbxSystemUnit lFbxOriginSystemUnit = _fbxScene->GetGlobalSettings().GetOriginalSystemUnit();
	double factor = lFbxFileSysteomUnit.GetScaleFactor();

	const fbxsdk::FbxSystemUnit::ConversionOptions IConversionOptions =
	{ true,true,true,true,true,true };
	lFbxFileSysteomUnit.m.ConvertScene(_fbxScene, IConversionOptions);
	lFbxOriginSystemUnit.m.ConvertScene(_fbxScene, IConversionOptions);

	//FbxSystemUnit::m.ConvertScene(_fbxScene, IConversionOptions);

	// ��ǥ���� �����´�.
	FbxAxisSystem sceneAxisSystem = _fbxScene->GetGlobalSettings().GetAxisSystem();

	// �� ���� ��ǥ���� �ٲ۴�.
	_fbxScene->GetGlobalSettings().SetAxisSystem(fbxsdk::FbxAxisSystem::DirectX);

	// �� ������ �ﰢ��ȭ �� �� �ִ� ��� ��带 �ﰢ��ȭ ��Ų��. 
	FbxGeometryConverter* geometryConverter = new FbxGeometryConverter(_fbxManager);

	geometryConverter->Triangulate(_fbxScene, true);

	geometryConverter->SplitMeshesPerMaterial(_fbxScene, true);

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
	// ��� Animation�� �����Ѵ�.
	// �ִϸ��̼ǵ� �ߵ���.
	LoadAnimation();

	// ��� Bone�� �����Ѵ�.
	// bone�� �ߵ���. 
	ProcessBone(node);

	// ��� Mesh�� �����Ѵ�. 
	ProcessMesh(node);
}

/**
 * \brief Get Mesh Data(pos, UV, normal, tangent, binormal)
 * \param node
 */
void DUOLParser::DUOLFBXParser::ProcessMesh(FbxNode* node)
{
	fbxsdk::FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();

	// Mesh�϶��� ���´�.
	if (nodeAttribute && nodeAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh)
	{
		std::shared_ptr<DuolData::Mesh> meshinfo = std::make_shared<DuolData::Mesh>();

		int nodecount = node->GetNodeAttributeCount();

		meshinfo->indices.resize(nodecount);

		// ���⼭�� �ε����� ���� �־��ش�. 
		for (int i = 0; i < nodecount; i++)
		{
			fbxsdk::FbxMesh* currentMesh = (fbxsdk::FbxMesh*)node->GetNodeAttributeByIndex(i);

			// bool ������ �ּ� split�Ǵ� mesh���� Ȯ���Ѵ�. 
			LoadMesh(node, currentMesh, meshinfo, i);

			fbxsdk::FbxLayerElementMaterial* materialElement = currentMesh->GetElementMaterial(0);

			if (materialElement)
			{
				int index = materialElement->GetIndexArray().GetAt(0);

				fbxsdk::FbxSurfaceMaterial* surfaceMaterial = currentMesh->GetNode()->GetSrcObject<fbxsdk::FbxSurfaceMaterial>(index);
				std::string materialname = surfaceMaterial->GetName();

				meshinfo->materialName.emplace_back(materialname);

				if (!CleanMaterial(materialname))
				{
					if (_isNoMaterial)
						index += 1;
					// NoMaterial�� ó������ ���Ƿ� +1�� ���ش�. 
					meshinfo->materialIndex.emplace_back(index);

					LoadMaterial(surfaceMaterial);
				}
			}
			else
			{
				if (!_isNoMaterial)
				{
					_fbxModel->fbxmaterialList.emplace_back(_noMaterial);
					_isNoMaterial = true;
				}
				meshinfo->materialName.emplace_back("NoMaterial");
				// Material�� �ȵ��� Mesh�� ���� ���Ƿ� �ϳ� ������ش�.
				LoadDefaultMaterial();
			}
		}

		// model�� Mesh�� �־��ش�.
		_fbxModel->fbxMeshList.emplace_back(meshinfo);
	}

	int count = node->GetChildCount();

	// ��� ������ ��� �ڽ��� �� ����. 
	for (int i = 0; i < count; i++)
	{
		ProcessMesh(node->GetChild(i));
	}
}

void DUOLParser::DUOLFBXParser::ProcessBone(FbxNode* node)
{
	for (size_t childindex = 0; childindex < node->GetChildCount(); ++childindex)
	{
		fbxsdk::FbxNode* currNode = node->GetChild(childindex);
		// parentindex�� -1�̶�� �ϴµ� �̰� rootnode��� �ǹ��̴�.
		LoadSkeleton(currNode, childindex, -1);
	}
}

void DUOLParser::DUOLFBXParser::ProcessAnimation(FbxNode* node)
{
	// �̹� �ִϸ��̼��� �����ǰ� ���Ĵ�.
	// �׷��Ƿ� �ִϸ��̼� ũ�⸦ �ҷ��´�.
	const size_t animationCount = _fbxModel->animationClipList.size();
	std::vector<std::shared_ptr<DuolData::KeyFrame>> keyframeList;

	// �ִϸ��̼� ������ŭ �����ش�.
	// �� �ִϸ��̼ǿ��� ���� Ű �������� ������ �ִ�. 
	for (size_t count = 0; count < animationCount; ++count)
	{
		// �ð��� � Ÿ������ �޾ƿ´�
		fbxsdk::FbxTime::EMode timeMode = _fbxScene->GetGlobalSettings().GetTimeMode();

		// Ű�����Ӹ�ŭ ������ Ű�������� �������ش�. 
		for (fbxsdk::FbxLongLong frame = 0; frame < _fbxModel->animationClipList[count]->totalKeyFrame; ++frame)
		{
			std::shared_ptr<DuolData::KeyFrame> keyframeInfo = std::make_shared<DuolData::KeyFrame>();

			fbxsdk::FbxTime fbxTime = 0;
			// Time
			fbxTime.SetFrame(frame, timeMode);

			// Local Transform = �θ� bone�� global transform�� ����� * �ڽ� bone�� global transform;
			// �� �������� �ð��� local�� ����´�.
			// EvaluateGlobalTransform : ��ȯ���
			fbxsdk::FbxAMatrix localTransform = node->EvaluateGlobalTransform(fbxTime);

			DUOLMath::Matrix localTM = DUOLMath::Matrix::Identity;

			// �θ� �ִ��� üũ�Ѵ�.
			FbxNode* parent = node->GetParent();

			if (parent)
			{
				// �θ� ���̷��� �ִϸ��̼����� Ȯ���Ѵ�.
				FbxNodeAttribute* parentAttribute = parent->GetNodeAttribute();

				if (parentAttribute && parentAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton)
				{
					fbxsdk::FbxAMatrix ParentTransform = parent->EvaluateGlobalTransform(fbxTime);

					localTransform = ParentTransform.Inverse() * localTransform;

					localTM = ConvertMatrix(localTransform);
				}
				// �θ� ���̷����� �ƴϸ� �ڱ� �ڽ� �״�� ���
				else
				{
					localTM = ConvertMatrix(localTransform);

					// ������⶧���� �����ش�.
					const auto roll = -90.0f * DirectX::XM_PI / 180.0f;

					const auto pitch = 180.0f * DirectX::XM_PI / 180.0f;

					DUOLMath::Quaternion q = DirectX::XMQuaternionRotationRollPitchYaw(roll, pitch, 0.0f);

					localTM *= XMMatrixRotationQuaternion(q);
				}
			}

			DirectX::XMVECTOR localScale;
			DirectX::XMVECTOR localRot;
			DirectX::XMVECTOR localPosition;
			DirectX::XMMatrixDecompose(&localScale, &localRot, &localPosition, localTM);

			keyframeInfo->time = fbxTime.GetSecondDouble();

			keyframeInfo->localTransform = DUOLMath::Vector3(localPosition);
			keyframeInfo->localRotation = DUOLMath::Vector4(localRot);
			keyframeInfo->localScale = DUOLMath::Vector3(localScale);

			keyframeList.emplace_back(keyframeInfo);
		}
		_fbxModel->animationClipList[count]->keyframeList.emplace_back(keyframeList);
	}
}

void DUOLParser::DUOLFBXParser::LoadAnimation()
{
	// �ϴ� ������ �� ���� Ȯ���� _ tick per frame �� �̻��ѰŰ��⵵�ϰ�?
	// �ִϸ��̼� �̸����� ��� ��� �迭
	fbxsdk::FbxArray<FbxString*> animationNameArray;
	fbxsdk::FbxDocument* document = dynamic_cast<fbxsdk::FbxDocument*>(_fbxScene);
	if (document != nullptr)
	{
		document->FillAnimStackNameArray(animationNameArray);
	}

	size_t nowAnimationCount = _fbxImporter->GetAnimStackCount();

	// �ִϸ��̼��� ������ �����Ѵ�. 
	if (nowAnimationCount <= 0)
		return;

	// �ִϸ��̼� ��ŭ ���鼭 �ִϸ��̼��� �������ְ� �־��ش�.
	for (size_t count = 0; count < nowAnimationCount; ++count)
	{
		// Get animation information
		fbxsdk::FbxAnimStack* currAnimStack = _fbxScene->GetSrcObject<fbxsdk::FbxAnimStack>(0);

		if (currAnimStack == nullptr)
			continue;

		std::shared_ptr<DuolData::AnimationClip> animationClip = std::make_shared< DuolData::AnimationClip>();

		// �ִϸ��̼� �̸� 
		fbxsdk::FbxString animStackName = currAnimStack->GetName();
		animationClip->animationName = animStackName.Buffer();

		fbxsdk::FbxTakeInfo* takeInfo = _fbxScene->GetTakeInfo(animStackName);

		// ���۽ð�, ����ð�, �ʴ� �����ӿ� ���� ����
		// �ϴ� SDK�� �޾ƿԴµ� �̰� ���� �ٲ�����ϳ� ����_ Ȯ���غ���
		// frame�� ������°��̴�. �ð��� ��������� .GetSecondDouble() �ؾ���
		fbxsdk::FbxTime startTime = takeInfo->mLocalTimeSpan.GetStart();
		fbxsdk::FbxTime endTime = takeInfo->mLocalTimeSpan.GetStop();

		// GetFrameCount : �ð��� ������ ���� �����´�.
		// �׳� �����Ӽ��� �����ö��� ����� �������� ���ϴµ��ϴ�.
		// �׳� �״�� �ҷ��� ���� : ����� �ҷ��´� GetFrameCount �Լ��� ����� �����ϰ� �����µ�
		animationClip->frameRate = (float)fbxsdk::FbxTime::GetFrameRate(_fbxScene->GetGlobalSettings().GetTimeMode());

		if (startTime.GetFrameCount(fbxsdk::FbxTime::eFrames60) < endTime.GetFrameCount(fbxsdk::FbxTime::eFrames60))
		{
			animationClip->startKeyFrame = startTime.GetFrameCount(fbxsdk::FbxTime::eFrames60);
			animationClip->endKeyFrame = endTime.GetFrameCount(fbxsdk::FbxTime::eFrames60);
			animationClip->totalKeyFrame = endTime.GetFrameCount(fbxsdk::FbxTime::eFrames60) - startTime.GetFrameCount(fbxsdk::FbxTime::eFrames60) + 1;
			animationClip->tickPerFrame = (endTime.GetSecondDouble() - startTime.GetSecondDouble()) / (animationClip->totalKeyFrame);
		}

		_fbxModel->animationClipList.emplace_back(animationClip);
	}
}

void DUOLParser::DUOLFBXParser::LoadMesh(FbxNode* node, FbxMesh* currentmesh, std::shared_ptr<DuolData::Mesh> meshinfo, int meshindex)
{
	fbxsdk::FbxGeometry* geometry = node->GetGeometry();

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

	// NodeTM �ֱ�
	fbxsdk::FbxAMatrix nodetransform = _fbxScene->GetAnimationEvaluator()->GetNodeGlobalTransform(node);

	DUOLMath::Matrix nodematrix = ConvertMatrix(nodetransform);

	const auto roll = -90.0f * DirectX::XM_PI / 180.0f;

	const auto pitch = 180.0f * DirectX::XM_PI / 180.0f;

	DUOLMath::Quaternion q = DirectX::XMQuaternionRotationRollPitchYaw(roll, pitch, 0.0f);

	nodematrix *= XMMatrixRotationQuaternion(q);

	meshinfo->nodeTM = nodematrix;

	// Mesh Vertex
	// fbxsdk������ vertex�� controlpoint��� �Ѵ�. 
	const int vertexcount = currentmesh->GetControlPointsCount();

	// Mesh���� �ﰢ�� ������ �����´�.
	// fbxsdk ������ face�� polygon�̶�� �Ѵ�.
	const int facecount = currentmesh->GetPolygonCount();

	// vertex �� ������ ó�� X
	if (vertexcount < 1)
		return;

	meshinfo->tempVertexList.resize(vertexcount);

	// position ������ �����´�. (���� �ٲ����)
	fbxsdk::FbxVector4* controlpoints = currentmesh->GetControlPoints();
	// ����ġ�� �־�����Ѵ�.
	const int deformerCount = currentmesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin);

	if (deformerCount > 0)
	{
		// ��Ű�� �޽� count�� �����Ƿ� ��Ű�� �޽��̴�.
		meshinfo->isSkinned = true;

		_fbxModel->isSkinnedAnimation = true;
	}

	// Vertex�� �� ���� Ȯ���ߴ�.
	// temp�� �־��ش�.
	for (int i = 0; i < vertexcount; i++)
	{
		meshinfo->tempVertexList[i].position.x = static_cast<float>(controlpoints[i].mData[0]);
		meshinfo->tempVertexList[i].position.y = static_cast<float>(controlpoints[i].mData[2]);
		meshinfo->tempVertexList[i].position.z = static_cast<float>(controlpoints[i].mData[1]);

		if (!meshinfo->isSkinned)
			meshinfo->tempVertexList[i].position = DUOLMath::XMVector3TransformCoord(meshinfo->tempVertexList[i].position, nodematrix);
	}

	for (int i = 0; i < deformerCount; ++i)
	{
		fbxsdk::FbxSkin* fbxSkin = static_cast<fbxsdk::FbxSkin*>(currentmesh->GetDeformer(i, fbxsdk::FbxDeformer::eSkin));

		if (fbxSkin)
		{
			// � �������� �̷����ε�?
			// �޽��� �����Ǵ� ���
			fbxsdk::FbxSkin::EType type = fbxSkin->GetSkinningType();

			if (fbxsdk::FbxSkin::eRigid == type)
			{
				// Cluster�� skinning������ �ִ� bone�� ������ �ش�.
				const int clusterCount = fbxSkin->GetClusterCount();

				for (int j = 0; j < clusterCount; ++j)
				{
					fbxsdk::FbxCluster* cluster = fbxSkin->GetCluster(j);
					if (cluster->GetLink() == nullptr)
						continue;

					// ���� ���� �������� ã�ƾ��Ѵ�.
					std::string BoneName = cluster->GetLink()->GetName();
					int boneIndex = GetBoneIndex(BoneName);

					// �ش� ���� ������ �޴� ���� �ε��� ������ ����
					const int indicesCount = cluster->GetControlPointIndicesCount();

					for (int k = 0; k < indicesCount; ++k)
					{
						// �ش� ������ ����ġ
						double weight = cluster->GetControlPointWeights()[k];

						// ��Ʈ�� ����Ʈ�� �ε���(���ؽ��� �ε���)
						int controlpointIndex = cluster->GetControlPointIndices()[k];

						// ����ġ�� �ִ� 8��(���������ֳ�?)
						// ���鼭 ����� �ְ� break�Ѵ�
						for (int weightindex = 0; weightindex < 8; ++weightindex)
						{
							if (meshinfo->tempVertexList[controlpointIndex].boneIndices[weightindex] == -1)
							{
								meshinfo->tempVertexList[controlpointIndex].boneWeight[weightindex] = weight;
								meshinfo->tempVertexList[controlpointIndex].boneIndices[weightindex] = boneIndex;

								break;
							}
						}
					}

					// ���� ���������� ���� BoneOffSetMatrix�� ���ؾ��Ѵ�.
					// FbxSdk������ ����� ���Ҽ�����.
					// GetTransformLinkMaterix�� �ش� ���� ���� Matrix�� ���
					// GetTransformMatrix�δ� �θ���� ���Ե� Matrix�� ��´�
					// �ش� �θ� ����� ������� ���ϸ� �θ� ��ı��� �ڱ� �ڽ��� ������ ����
					// ��İ��� ���� �Ǽ� ���������� ��İ��� ���� �� �ְԵȴٴµ�..
					fbxsdk::FbxAMatrix transform;
					fbxsdk::FbxAMatrix linkTransform;

					cluster->GetTransformMatrix(transform);
					cluster->GetTransformLinkMatrix(linkTransform);

					DUOLMath::Matrix boneTransform = ConvertMatrix(transform);
					DUOLMath::Matrix boneLinkTransform = ConvertMatrix(linkTransform);

					// BindPose ����� ������
					fbxsdk::FbxAMatrix geometryTransform = GetGeometryTransformation(currentmesh->GetNode());
					DUOLMath::Matrix geometryMatrix = ConvertMatrix(geometryTransform);

					DUOLMath::Matrix offsetMatrix = boneTransform * boneLinkTransform.Invert();

					DUOLMath::Vector3 translation; DUOLMath::Vector3 scale; DUOLMath::Quaternion rot;

					offsetMatrix.Decompose(scale, rot, translation);

					DUOLMath::Matrix newOffset = /*DUOLMath::Matrix::CreateScale(scale) **/ DUOLMath::Matrix::CreateFromQuaternion(rot) * DUOLMath::Matrix::CreateTranslation(translation);

					_fbxModel->fbxBoneList[boneIndex]->offsetMatrix = offsetMatrix;
					//_fbxModel->fbxBoneList[boneIndex]->offsetMatrix = newOffset;
				}
			}
		}
	}

	// Mesh���� �ﰢ�� ������ �����´�.
	// fbxsdk ������ face�� polygon�̶�� �Ѵ�.
	ConvertOptimize(currentmesh, meshinfo, meshindex);

	// ����ȭ�� �ϰ� tangent ����� ����.
	GetTangent(meshinfo, meshindex);
}

void DUOLParser::DUOLFBXParser::LoadMaterial(const fbxsdk::FbxSurfaceMaterial* surfacematerial)
{
	std::shared_ptr<DuolData::Material> material = std::make_shared<DuolData::Material>();

	material->materialName = surfacematerial->GetName();

	// �� �𵨿� ���Ѱ� �����´�.
	if (surfacematerial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		// Ambient Data
		material->material_Ambient.x = static_cast<float>(((fbxsdk::FbxSurfacePhong*)surfacematerial)->Ambient.Get()[0]) * 10.0f;
		material->material_Ambient.y = static_cast<float>(((fbxsdk::FbxSurfacePhong*)surfacematerial)->Ambient.Get()[1]) * 10.0f;
		material->material_Ambient.z = static_cast<float>(((fbxsdk::FbxSurfacePhong*)surfacematerial)->Ambient.Get()[2]) * 10.0f;
		material->material_Ambient.w = 1.0f;

		// Diffuse Data
		material->material_Diffuse.x = static_cast<float>(((fbxsdk::FbxSurfacePhong*)surfacematerial)->Diffuse.Get()[0]);
		material->material_Diffuse.y = static_cast<float>(((fbxsdk::FbxSurfacePhong*)surfacematerial)->Diffuse.Get()[1]);
		material->material_Diffuse.z = static_cast<float>(((fbxsdk::FbxSurfacePhong*)surfacematerial)->Diffuse.Get()[2]);
		material->material_Diffuse.w = 1.0f;

		// Specular Data
		material->material_Specular.x = static_cast<float>(((fbxsdk::FbxSurfacePhong*)surfacematerial)->Specular.Get()[0]);
		material->material_Specular.y = static_cast<float>(((fbxsdk::FbxSurfacePhong*)surfacematerial)->Specular.Get()[1]);
		material->material_Specular.z = static_cast<float>(((fbxsdk::FbxSurfacePhong*)surfacematerial)->Specular.Get()[2]);
		material->material_Specular.w = 1.0f;

		// Emissive Data
		material->material_Emissive.x = static_cast<float>(((fbxsdk::FbxSurfacePhong*)surfacematerial)->Emissive.Get()[0]);
		material->material_Emissive.y = static_cast<float>(((fbxsdk::FbxSurfacePhong*)surfacematerial)->Emissive.Get()[1]);
		material->material_Emissive.z = static_cast<float>(((fbxsdk::FbxSurfacePhong*)surfacematerial)->Emissive.Get()[2]);
		material->material_Emissive.w = 1.0f;

		// Transparecy Data (������)
		material->material_Transparency = static_cast<float>(((fbxsdk::FbxSurfacePhong*)surfacematerial)->TransparencyFactor.Get());

		//// Shininess Data
		//material->roughness = static_cast<float>(((fbxsdk::FbxSurfacePhong*)surfacematerial)->Shininess.Get());
		//// Reflectivity Data
		//material->material_Reflectivity = static_cast<float>(((fbxsdk::FbxSurfacePhong*)surfacematerial)->ReflectionFactor.Get());
		//specular
		//material->specular = static_cast<float>(((fbxsdk::FbxSurfacePhong*)surfacematerial)->SpecularFactor.Get());

		// Metallic
		material->metallic = static_cast<float>(((fbxsdk::FbxSurfacePhong*)surfacematerial)->ReflectionFactor.Get());

		float shininess = fmax(static_cast<float>(((fbxsdk::FbxSurfacePhong*)surfacematerial)->Shininess.Get()), 0.0f);
		// Shininess Data
		material->roughness = 1.f - sqrtf(shininess) / 10.f;

		//specular
		material->specular = static_cast<float>(((fbxsdk::FbxSurfacePhong*)surfacematerial)->SpecularFactor.Get());

		// Reflectivity Data
		material->material_Reflectivity = static_cast<float>(((fbxsdk::FbxSurfacePhong*)surfacematerial)->ReflectionFactor.Get());
	}

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

	// Material�� fbxmodel�� ������ �ִ´�. 
	_fbxModel->fbxmaterialList.emplace_back(material);
}

void DUOLParser::DUOLFBXParser::LoadDefaultMaterial()
{
	_noMaterial = std::make_shared<DuolData::Material>();

	_noMaterial->materialName = "NoMaterial";

	// Ambient Data
	_noMaterial->material_Ambient.x = 0.f;
	_noMaterial->material_Ambient.y = 0.f;
	_noMaterial->material_Ambient.z = 1.f;
	_noMaterial->material_Ambient.w = 1.0f;

	// Diffuse Data
	_noMaterial->material_Diffuse.x = 0.0005f;
	_noMaterial->material_Diffuse.y = 0.0005f;
	_noMaterial->material_Diffuse.z = 0.0005f;
	_noMaterial->material_Diffuse.w = 1.0f;

	// Specular Data
	_noMaterial->material_Specular.x = 0.005f;
	_noMaterial->material_Specular.y = 0.005f;
	_noMaterial->material_Specular.z = 0.02f;
	_noMaterial->material_Specular.w = 1.0f;

	// Emissive Data
	_noMaterial->material_Emissive.x = 0.f;
	_noMaterial->material_Emissive.y = 0.f;
	_noMaterial->material_Emissive.z = 0.f;
	_noMaterial->material_Emissive.w = 1.0f;

	// Transparecy Data (������)
	_noMaterial->material_Transparency = 0.f;


	// Metallic
	_noMaterial->metallic = 0.f;

	// Shininess Data
	_noMaterial->roughness = 0.5f;

	//specular
	_noMaterial->specular = 0.25f;

	// Reflectivity Data
	_noMaterial->material_Reflectivity = 0.f;


	_noMaterial->isAlbedo = false;
	_noMaterial->isNormal = false;
	_noMaterial->isRoughness = false;
	_noMaterial->isEmissive = false;

	// Material�� fbxmodel�� ������ �ִ´�. 
	//_fbxModel->fbxmaterialList.emplace_back(material);
}

void DUOLParser::DUOLFBXParser::LoadSkeleton(fbxsdk::FbxNode* node, int nowindex, int parentindex)
{
	fbxsdk::FbxNodeAttribute* attribute = node->GetNodeAttribute();

	if (attribute && attribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton)
	{
		std::shared_ptr<DuolData::Bone> boneInfo = std::make_shared<DuolData::Bone>();

		boneInfo->boneName = node->GetName();

		// ��Ʈ��尡 �ڲ� 1�� ����. �׷��� �׳� Ȯ���غ���� ��ó�� ������ ���� ��Ʈ����̹Ƿ�
		// ����� 0�̸� -1�� ������ �־��ش�. 
		if (_fbxModel->fbxBoneList.size() == 0)
			boneInfo->parentIndex = -1;

		if (parentindex == _fbxModel->fbxBoneList.size())
			boneInfo->parentIndex = parentindex - 1;
		else
			boneInfo->parentIndex = parentindex;

		// Mesh Node�� ���ȱ� ������ ���鵵 ��������Ѵ�.
		// ���� �������� �𸣰ڴ� �������鼭 Ȯ���غ���
		fbxsdk::FbxAMatrix nodeTransform = _fbxScene->GetAnimationEvaluator()->GetNodeGlobalTransform(node);
		//FbxAMatrix nodeTransform = node->EvaluateGlobalTransform(fbxsdk::FbxTime(0));

		DUOLMath::Matrix nodeMatrix = ConvertMatrix(nodeTransform);

		const auto roll = -90.0f * DirectX::XM_PI / 180.0f;

		const auto pitch = 180.0f * DirectX::XM_PI / 180.0f;

		DUOLMath::Quaternion q = DirectX::XMQuaternionRotationRollPitchYaw(roll, pitch, 0.0f);

		nodeMatrix *= XMMatrixRotationQuaternion(q);

		boneInfo->nodeMatrix = nodeMatrix;

		_fbxModel->fbxBoneList.emplace_back(boneInfo);

		// Skeleton�̸� �ִϸ��̼��� �����ش�.
		ProcessAnimation(node);
	}

	size_t childCount = node->GetChildCount();

	for (size_t childcount = 0; childcount < node->GetChildCount(); ++childcount)
	{
		LoadSkeleton(node->GetChild(childcount), _fbxModel->fbxBoneList.size(), nowindex);
	}
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
void DUOLParser::DUOLFBXParser::ConvertOptimize(fbxsdk::FbxMesh* currentMesh, std::shared_ptr<DuolData::Mesh> meshinfo, int meshindex)
{
	int faceCount = currentMesh->GetPolygonCount();

	// face�� ������ return ��Ų��.
	if (faceCount == 0)
		return;

	// vertex ����
	int vertexcounter = 0;
	int vertexindex[3];

	// �ϴ� ���ؽ��� üũ�ϱ����� �����
	// std::vector<bool> isvertex(faceCount * 3, false);
	bool isWeldVertex = false;

	// key : uv, normal value : controlpoint
	// Ű���� �������� �ּ� vertex üũ���Ѵ�. 
	std::map<std::tuple<size_t, float, float, float, float, float>, size_t> vertexCheckMap;

	// face��ŭ �ϴ� �����ش�.
	for (size_t facenum = 0; facenum < faceCount; ++facenum)
	{
		// �ﰢ���� ������ �������� �����Ǿ� ����
		// ���⼭ ���ؽ� �ɰ��⸦ ����� �Ѵ�. 
		for (size_t vertexcount = 0; vertexcount < 3; ++vertexcount)
		{
			// i��° Face�� j��° ���ؽ� �ε����� �����´�.
			int controlpointIndex = currentMesh->GetPolygonVertex(facenum, vertexcount);

			vertexindex[vertexcount] = controlpointIndex;

			// ������ �ִ� ������ üũ�ϱ� ����
			DUOLMath::Vector3 tempNormal = GetNormal(currentMesh, controlpointIndex, vertexcounter);
			// UV�� ���ϴ°� �ƴ϶� UV Index�� ���Ѵ�. 
			DUOLMath::Vector2 tempUV = GetUV(currentMesh, controlpointIndex, vertexcounter);

			// ��¥�� controlpoint�� �������̴�. �ű⼭ �ɰ��ų� �ɰ��� �ʰų��� �����Ѵ�.
			// vector2�� find�� �ȵǴµ�
			const auto tupleValue = std::make_tuple(controlpointIndex, tempUV.x, tempUV.y, tempNormal.x, tempNormal.y, tempNormal.z);
			const auto iter = vertexCheckMap.find(tupleValue);

#pragma region split vertex
			// map�� ������
			// ������ ������ �� �־��ְ� �������ش�.
			if (iter == vertexCheckMap.end())
			{
				DuolData::Vertex vertex;

				vertex.position = meshinfo->tempVertexList[controlpointIndex].position;
				vertex.uv = tempUV;
				vertex.normal = tempNormal;

				for (int weightidx = 0; weightidx < 8; ++weightidx)
				{
					vertex.boneWeight[weightidx] = meshinfo->tempVertexList[controlpointIndex].boneWeight[weightidx];
					vertex.boneIndices[weightidx] = meshinfo->tempVertexList[controlpointIndex].boneIndices[weightidx];
				}

				meshinfo->vertexList.push_back(vertex);

				controlpointIndex = meshinfo->vertexList.size() - 1;

				vertexindex[vertexcount] = controlpointIndex;

				vertexCheckMap.insert(std::make_pair(tupleValue, controlpointIndex));
			}
#pragma endregion
			// ���� �ִ� �������?
			else
			{
				vertexindex[vertexcount] = iter->second;
			}
			vertexcounter++;

		}
		meshinfo->indices[meshindex].emplace_back(vertexindex[0]);
		meshinfo->indices[meshindex].emplace_back(vertexindex[2]);
		meshinfo->indices[meshindex].emplace_back(vertexindex[1]);
	}
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

DUOLMath::Vector3 DUOLParser::DUOLFBXParser::GetNormal(fbxsdk::FbxMesh* mesh, int controlpointindex, int vertexindex)
{
	DUOLMath::Vector3 normal;

	// ���⼭ ����ó�� ���ֱ�
	if (mesh->GetElementNormalCount() < 1)
		return DUOLMath::Vector3::Zero;

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

	if (!_fbxModel->isSkinnedAnimation)
	{
		normal.x = static_cast<float>(-vertexNormal->GetDirectArray().GetAt(index).mData[0]);;
		normal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);;
		normal.z = static_cast<float>(-vertexNormal->GetDirectArray().GetAt(index).mData[1]);;
	}
	else
	{
		normal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);;
		normal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);;
		normal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);;
	}
	return normal;
}

DUOLMath::Vector2 DUOLParser::DUOLFBXParser::GetUV(fbxsdk::FbxMesh* mesh, int controlpointindex, int vertexindex)
{
	DUOLMath::Vector2 uv;

	if (mesh->GetElementUVCount() < 1)
		return DUOLMath::Vector2::Zero;

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

	uv.x = static_cast<float>(vertexuv->GetDirectArray().GetAt(index).mData[0]);
	uv.y = 1.0f - static_cast<float>(vertexuv->GetDirectArray().GetAt(index).mData[1]);

	// mirror�ϸ� ������ ���ܼ� �־��ذ�.
	if (uv.x < 0)
		uv.x = 1 + uv.x;

	return uv;
}

/**
 * \brief tangent�� ���� �־��ش�.
 * \param mesh
 * \param meshinfo
 * \param controlpointindex
 * \param vertexindex
 */
void DUOLParser::DUOLFBXParser::GetTangent(std::shared_ptr<DuolData::Mesh>  meshinfo, int meshindex)
{
	DUOLMath::Vector3 tangent;

	// polygon�� �ﰢ������ �̷���� �ִ�.
	for (size_t i = 0; i < meshinfo->indices[meshindex].size(); i += 3)
	{
		int vertexindex0 = meshinfo->indices[meshindex][i];
		int vertexindex1 = meshinfo->indices[meshindex][i + 1];
		int vertexindex2 = meshinfo->indices[meshindex][i + 2];

		// ������ �޽� �ﰢ���� ����
		DUOLMath::Vector3 ep1 = meshinfo->vertexList[vertexindex1].position - meshinfo->vertexList[vertexindex0].position;
		DUOLMath::Vector3 ep2 = meshinfo->vertexList[vertexindex2].position - meshinfo->vertexList[vertexindex0].position;

		// �ؽ�ó ��ǥ������ ����
		DUOLMath::Vector2 uv1 = meshinfo->vertexList[vertexindex1].uv - meshinfo->vertexList[vertexindex0].uv;
		DUOLMath::Vector2 uv2 = meshinfo->vertexList[vertexindex2].uv - meshinfo->vertexList[vertexindex0].uv;

		float det = (uv1.x * uv2.y) - (uv2.x * uv1.y);

		if (det != 0.f)
		{
			det = 1.0f / det;
		}

		tangent = (ep1 * uv2.y - ep2 * uv1.y) * det;

		meshinfo->vertexList[vertexindex0].tangent += tangent;
		meshinfo->vertexList[vertexindex1].tangent += tangent;
		meshinfo->vertexList[vertexindex2].tangent += tangent;
	}

	for (size_t i = 0; i < meshinfo->vertexList.size(); ++i)
	{
		meshinfo->vertexList[i].tangent.Normalize();
	}
}

int DUOLParser::DUOLFBXParser::GetBoneIndex(std::string bonename)
{
	for (size_t count = 0; count < _fbxModel->fbxBoneList.size(); ++count)
	{
		if (_fbxModel->fbxBoneList[count]->boneName == bonename)
		{
			return count;
		}
	}
	return -1;
}


/**
 * \brief node�� TM�� ������Ѱ� (��������̴�)
 * \param node
 * \return
 */
fbxsdk::FbxAMatrix  DUOLParser::DUOLFBXParser::GetGeometryTransformation(fbxsdk::FbxNode* node)
{
	if (!node)
		throw std::exception("Null for mesh geometry");

	const fbxsdk::FbxVector4 translation = node->GetGeometricTranslation(FbxNode::eSourcePivot);
	const fbxsdk::FbxVector4 rotation = node->GetGeometricRotation(FbxNode::eSourcePivot);
	const fbxsdk::FbxVector4 scale = node->GetGeometricScaling(FbxNode::eSourcePivot);

	return fbxsdk::FbxAMatrix(translation, rotation, scale);
}

bool DUOLParser::DUOLFBXParser::CleanMaterial(std::string& materialname)
{
	for (auto material : _fbxModel->fbxmaterialList)
	{
		if (materialname == material->materialName)
			return true;
	}
	return false;
}

//void DUOLParser::DUOLFBXParser::DecomposeMatrix(DUOLMath::Matrix nodetm)
//{
//	
//}
