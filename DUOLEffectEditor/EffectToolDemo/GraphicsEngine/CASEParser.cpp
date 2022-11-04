#include "pch.h"
#include "CASEParser.h"
#include <algorithm>


/// MAX 랑 Blender 좌표 다름.

CASEParser::CASEParser() : m_lexer(nullptr), m_Token(0), m_TokenString(), m_data_asciiexport(0)
{
	MaxPos = { 0,0,0 };
	MinPos = { 0,0,0 };
}

CASEParser::~CASEParser()
{
	delete m_lexer;

	while (!Node.empty())
	{
		delete Node.front();
		Node.pop();
	}
}

bool CASEParser::Init()
{
	m_lexer = new ASE::CASELexer;

	return TRUE;
}

MeshData* CASEParser::Load(LPSTR p_File)
{
	/// 0) 파일을 로드한다.
	if (!m_lexer->Open(p_File))
	{
		TRACE("파일을 여는 중에 문제가 발생했습니다!");
		return FALSE;
	}
	Path.clear();
	m_ParentMesh = new MeshData();

	/// 1) 다롱이아부지김교수의 파워풀한 재귀함수 한개로 처리 끝!
	Parsing_DivergeRecursiveALL(0);

	// 부모만 텍스쳐 생성 ..
	m_ParentMesh->m_Path = Path;

	if (Mtl.empty())
		Mtl.push(new Material());
	m_ParentMesh->m_Mtl = Mtl.back();
	Mtl.pop();

	Convert_Optimize();

	ParentInit();
	m_lexer->Close();
	return m_ParentMesh;
}


/// <summary>
/// 메시의 최적화를 해 준다.
/// 노말값, 텍스쳐 좌표에 따라 버텍스를 늘리고, 중첩되는것은 제거하고..
/// </summary>
bool CASEParser::Convert_Optimize()
{
	NodeInfo* _Node = Node.front();




#pragma region Matrix
	XMFLOAT4X4 _WorldMT;

	XMFLOAT3 _Row0 = _Node->m_Row0;
	XMFLOAT3 _Row1 = _Node->m_Row2;
	XMFLOAT3 _Row2 = _Node->m_Row1;
	XMFLOAT3 _Row3 = _Node->m_Row3;

	// 네거티비 스케일 해결 해야됨.
	XMVECTOR X, Y, Z;

	X = XMLoadFloat3(&_Row0);
	Y = XMLoadFloat3(&_Row1);
	Z = XMLoadFloat3(&_Row2);

	XMVECTOR a = XMVector3Dot(XMVector3Cross(X, Y), Z);
	if (a.m128_f32[0] < 0)
	{
		_Row0.x *= -1;
		_Row0.y *= -1;
		_Row0.z *= -1;

		_Row1.x *= -1;
		_Row1.y *= -1;
		_Row1.z *= -1;

		_Row2.x *= -1;
		_Row2.y *= -1;
		_Row2.z *= -1;

		for (auto& iter : _Node->faceNormal)
		{
			iter.faceNormal.x *= -1;
			iter.faceNormal.y *= -1;
			iter.faceNormal.z *= -1;
			for (auto& iter2 : iter.vertexNormal)
			{
				iter2.normal.x *= -1;
				iter2.normal.y *= -1;
				iter2.normal.z *= -1;
			}
		}
	}


	// 파서에서 받을 때 뒤집어서 받고있다...!
	_WorldMT._11 = _Row0.x; _WorldMT._12 = _Row0.y; _WorldMT._13 = _Row0.z; _WorldMT._14 = 0;
	_WorldMT._21 = _Row1.x; _WorldMT._22 = _Row1.y; _WorldMT._23 = _Row1.z; _WorldMT._24 = 0;
	_WorldMT._31 = _Row2.x; _WorldMT._32 = _Row2.y; _WorldMT._33 = _Row2.z; _WorldMT._34 = 0;
	_WorldMT._41 = _Row3.x; _WorldMT._42 = _Row3.y; _WorldMT._43 = _Row3.z; _WorldMT._44 = 1;

	XMMATRIX _XMReturnWorldTM = XMLoadFloat4x4(&_WorldMT); // 게임엔진 Mesh에 넘겨주는용도
	XMMATRIX _XMReturnLocalTM; // 게임엔진 Mesh에 넘겨주는용도
	XMMATRIX _XMWorldInversTM;
#pragma endregion
#pragma region 분할해서 넣기.
	m_OneMesh = nullptr; //작업할자식
	MeshData* _ParentTemp = nullptr;
	if (_Node->m_ParentName != "")
	{
		// 자식!!
		m_OneMesh = new MeshData();

		m_OneMesh->m_Name = _Node->m_Name;
		_ParentTemp = reinterpret_cast<MeshData*>(m_ParentMesh->FindChildren(_Node->m_ParentName));

		assert(_ParentTemp);
		m_OneMesh->SetParent(_ParentTemp);

		//로컬 = 월드TM * 부모TM-1 맞는듯..?...
		_XMReturnLocalTM = _XMReturnWorldTM * MathHelper::Inverse(_ParentTemp->m_WorldTM);
		m_OneMesh->m_WorldTM = _XMReturnWorldTM;
		m_OneMesh->m_LocalMT = _XMReturnLocalTM;


	}
	else
	{
		/// 부모!!
		m_OneMesh = new MeshData();
		m_OneMesh->m_Name = _Node->m_Name;
		m_OneMesh->SetParent(m_ParentMesh);

		m_OneMesh->m_WorldTM = _XMReturnWorldTM;
		m_OneMesh->m_LocalMT = _XMReturnWorldTM;

	}


	/// 버텍스들을 모두 로컬스페이스로 보낸다.

	//나의 역행렬..!!
	_XMWorldInversTM = MathHelper::Inverse(_XMReturnWorldTM);
	for (unsigned int i = 0; i < _Node->vertices.size(); i++)
	{
		// 버텍스 Pos
		XMVECTOR _Temp = XMLoadFloat3(&_Node->vertices[i].Pos);
		XMStoreFloat3(&_Node->vertices[i].Pos, XMVector3Transform(_Temp, _XMWorldInversTM));

	}
#pragma endregion
#pragma region Vertex_Index


	// Key tuple : vIndex, vtIndex, vNormal.x, vNormal.y, vNormal.z => 버텍스들을 순회하면서 같은 녀석이 이미 만들어졌는지 확인.
	std::map<std::tuple<int, float, float, float>, unsigned> indexMap;

	auto vertexList = _Node->vertices;
	const auto& tVertexList = _Node->Tvertex;

	std::vector<Vertex> vertexVector;
	std::vector<UINT> indexVector(_Node->indices.size() * 3);

	//const auto& faceNormals = _Node.faceNormals;
	//const auto& faceList = _Node.faceList;
	//const auto& tFaceList = _Node.tFaceList;
	// 모든 페이스를 순환시킨다.
	for (unsigned int i = 0; i < _Node->indices.size(); i++)
	{
		const auto& face = _Node->indices[i];
		int tFace[3];
		FaceNormal faceNormal;

		bool bUseTexture = false;
		bool bUseNormal = false;

		if (_Node->TFace.size() > i)
		{
			tFace[0] = _Node->TFace[i].x;
			tFace[1] = _Node->TFace[i].y;
			tFace[2] = _Node->TFace[i].z;
			bUseTexture = true;
		}

		if (_Node->faceNormal.size() > i)
		{
			faceNormal = _Node->faceNormal[i];
			auto _Temp = faceNormal.vertexNormal[1];
			faceNormal.vertexNormal[1] = faceNormal.vertexNormal[2];
			faceNormal.vertexNormal[2] = _Temp;
			bUseNormal = true;
		}


		// 페이스의 정점을 순회하면서 중복된 것은 추가하지 않고, 하나씩 버텍스를 만든다.
		for (int j = 0; j < 3; j++)
		{
			const int vertexIndex = face.m_Index[j];
			const int tVertexIndex = bUseTexture ? tFace[j] : -1;

			const XMFLOAT3 vertexNormal = bUseNormal ? faceNormal.vertexNormal[j].normal : XMFLOAT3(-1.f, -1.f, -1.f);

			const auto indexPair = std::make_tuple(vertexIndex, vertexNormal.x, vertexNormal.y, vertexNormal.z);

			const auto iter = indexMap.find(indexPair);

			if (iter == indexMap.end())
			{
				// 새로운 버텍스의 조합이다 => 만들어야할 필요가 있다.
				Vertex newVertex;
				newVertex = vertexList[vertexIndex];
				const XMFLOAT3 position = vertexList[vertexIndex].Pos;
				const XMFLOAT2 uv = bUseTexture ? XMFLOAT2{ tVertexList[tVertexIndex].x, tVertexList[tVertexIndex].y } : XMFLOAT2(-1.f, -1.f);

				newVertex.Pos = position;
				newVertex.Texture = uv;
				newVertex.Nomal = vertexNormal;

				vertexVector.push_back(newVertex);
				const UINT newVertexIndex = static_cast<UINT>(vertexVector.size() - 1);

				indexVector[i * 3 + j] = newVertexIndex;
				indexMap.insert({ indexPair, newVertexIndex });
			}
			else
			{
				// 이미 같은 위치, 노말, 텍스쳐 성분을 가지는 버텍스가 있으니 현재 인덱스 순서가 이 버텍스를 가르키도록 한다.
				indexVector[i * 3 + j] = iter->second;
			}
		}

	}

	m_OneMesh->m_vertices = vertexVector;
	m_OneMesh->m_indices.resize(_Node->indices.size());
	for (unsigned int i = 0; i < _Node->indices.size(); i++)
	{
		m_OneMesh->m_indices[i].m_Index[0] = indexVector[i * 3 + 0];
		m_OneMesh->m_indices[i].m_Index[1] = indexVector[i * 3 + 1];
		m_OneMesh->m_indices[i].m_Index[2] = indexVector[i * 3 + 2];

		int r, g, b;

		r = m_OneMesh->m_indices[i].m_Index[0];
		g = m_OneMesh->m_indices[i].m_Index[1];
		b = m_OneMesh->m_indices[i].m_Index[2];

		if (!_Node->Tvertex.empty())
		{
			//tangent Space 구하기.
			XMVECTOR p0 = XMLoadFloat3(&m_OneMesh->m_vertices[r].Pos);
			XMVECTOR p1 = XMLoadFloat3(&m_OneMesh->m_vertices[g].Pos);
			XMVECTOR p2 = XMLoadFloat3(&m_OneMesh->m_vertices[b].Pos);

			float x1 = m_OneMesh->m_vertices[g].Texture.x - m_OneMesh->m_vertices[r].Texture.x; // u1 - u0
			float y1 = m_OneMesh->m_vertices[g].Texture.y - m_OneMesh->m_vertices[r].Texture.y; // v1 - v0
			float x2 = m_OneMesh->m_vertices[b].Texture.x - m_OneMesh->m_vertices[r].Texture.x; // u2 - u0
			float y2 = m_OneMesh->m_vertices[b].Texture.y - m_OneMesh->m_vertices[r].Texture.y; // v2 - v0

			float def = (x1 * y2) - (x2 * y1);

			XMVECTOR e1 = p1 - p0;
			XMVECTOR e2 = p2 - p0;

			e1 = e1 * (1 / def);
			e2 = e2 * (1 / def);

			XMFLOAT3 t =
			{
			e1.m128_f32[0] * y2 + e2.m128_f32[0] * (-y1),
			e1.m128_f32[1] * y2 + e2.m128_f32[1] * (-y1),
			e1.m128_f32[2] * y2 + e2.m128_f32[2] * (-y1)
			};
			m_OneMesh->m_vertices[r].Tangent.x += t.x;
			m_OneMesh->m_vertices[r].Tangent.y += t.z;
			m_OneMesh->m_vertices[r].Tangent.z += t.y;

			m_OneMesh->m_vertices[g].Tangent.x += t.x;
			m_OneMesh->m_vertices[g].Tangent.y += t.z;
			m_OneMesh->m_vertices[g].Tangent.z += t.y;

			m_OneMesh->m_vertices[b].Tangent.x += t.x;
			m_OneMesh->m_vertices[b].Tangent.y += t.z;
			m_OneMesh->m_vertices[b].Tangent.z += t.y;
		}
	}


	for (auto iter : m_OneMesh->m_vertices)
	{
		XMVECTOR _Temp = XMLoadFloat3(&iter.Tangent);
		_Temp = XMVector3Normalize(_Temp);
		XMStoreFloat3(&iter.Tangent, _Temp);

	}
#pragma endregion
#pragma region Ani
	for (auto& iter : _Node->m_AniKeys)
	{
		//로컬로 변경한 매트릭스를 줄까? 포지션을 줄까? 포지션을 주자~
		iter.m_isAnim = true;

		if (!iter.m_RotKey.empty())
		{

			for (unsigned int i = 0; i < iter.m_RotKey.size(); i++)
			{

				XMVECTOR _prevVector = iter.m_RotKey[i].m_Rot;
				_prevVector = XMQuaternionRotationAxis(_prevVector, _prevVector.m128_f32[3]);
				iter.m_RotKey[i].m_Rot = _prevVector;
			}
		}


	}

		m_OneMesh->m_AniKeys = _Node->m_AniKeys;
		//여러개의 애니메이션을 넘기고 저장하기 위해서 바꿔야 한다!!! TODO!
		//if (_Node->m_isWireFrame && !m_BoneNameList.empty())
		//	m_OneMesh->SetRasterizerState(RasterizerState::m_WireframeRS);

#pragma endregion



	delete _Node;
	Node.pop();

	if (!Node.empty())
	{
		Convert_Optimize();
	}


	//나중에 수정하자.

	return TRUE;
}

void CASEParser::ParentInit()
{
	/// 여기에서 스키닝 벡터 넘겨주는게 좋을듯?
	/// NodeTM 과 WorldTM은 무슨 차이가 있지.?
	/// 

	/// 
	m_ParentMesh->m_BoneNameList = m_BoneNameList;

	m_ParentMesh->m_MaxPos = MaxPos;
	m_ParentMesh->m_MinPos = MinPos;

	//for (auto iter : m_BoneNameList)
	//{
	//	Mesh* _Find = reinterpret_cast<Mesh*>(m_ParentMesh->FindChildren(iter));
	//	if (_Find)
	//	{
	//		_Find->SetRasterizerState(RasterizerState::m_WireframeRS);
	//	}
	//}

	//m_ParentMesh->Optimize();
	m_BoneNameList.clear();

	MaxPos = { 0,0,0 };
	MinPos = { 0,0,0 };


}


int CASEParser::FindNewIndex(int _Index)
{
	if (m_OneMesh->m_vertices[_Index]._isDelete)
	{
		return FindNewIndex(m_OneMesh->m_vertices[_Index].m_vertexLastIndex);
	}
	else
	{
		return m_OneMesh->m_vertices[_Index].m_vertexLastIndex;
	}
}

//----------------------------------------------------------------
// 재귀 호출됨을 전제로 하는 분기 함수이다.
//
// 일단 읽고, 토큰에 따라 동작한다.
//
// 재귀를 들어가는 조건은 { 을 만날 경우이다.
// 리턴하는 조건은 '}'를 만나거나 TOKEND_END를 만났을 때이다.
//
// 더불어, 재귀를 들어가기 전 토큰을 읽어, 지금 이 함수가 어떤 모드인지를 결정한다.
//
//----------------------------------------------------------------
void CASEParser::Parsing_DivergeRecursiveALL(int depth)
{
	//----------------------------------------------------------------------
	// 지역변수들 선언
	// 재귀 돌리다가 일정 이상 루프를 도는 경우(오류)를 체크하기 위한 변수
	int i = 0;
	// 리스트에 데이터를 넣어야 할 때 쓰이는 리스트 카운터.
	int listcount = 0;		// 매우 C스러운데 다른 해결책이 없을까?
	// 이것은 현 토큰이 어떤 종류인가를 판별해준다.
	// 이것을 멤버 변수로 하고 재귀를 돌리는것은 위험하다? -> 잘못된 재귀방식이다?
	LONG nowtoken;
	//----------------------------------------------------------------------
	int FaceCount = 0;

	static int _TicksPerFrame = 0;
	static int _LastFrame = 0;
	static int _FirstFrame = 0;

	static int _BoneIndex = 0;
	/// 이것을 하면 한 개의 토큰을 읽고, 그 종류와 내용을 알 수 있다.
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		// 일단 한 개의 토큰을 읽고, 그것이 괄호 닫기가 아니라면.
		// 넘어온 토큰에 따라 처리해준다.
		switch (nowtoken)
		{
#pragma region 쓸모없는거

		case TOKEND_BLOCK_START:
			Parsing_DivergeRecursiveALL(depth++);
			break;
		case TOKENR_3DSMAX_ASCIIEXPORT:
			m_data_asciiexport = Parsing_NumberLong();
			break;
			Parsing_String();	// 그냥 m_TokenString에 읽어버리는 역할 뿐.
			break;
		case TOKENR_SCENE_ENVMAP:
		{
			// 그냥 안의 내용을 읽어버린다 }가 나올때까지
			while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END) {
			}
		}
		break;
#pragma endregion
#pragma region Scene

		case  TOKENR_SCENE:
			_TicksPerFrame = 0;
			_LastFrame = 0;
			_FirstFrame = 0;

			break;
		case  TOKENR_SCENE_FIRSTFRAME:
			_FirstFrame = Parsing_NumberInt();
			break;
		case  TOKENR_SCENE_LASTFRAME:
			_LastFrame = Parsing_NumberInt();
			break;
		case  TOKENR_SCENE_TICKSPERFRAME:
			_TicksPerFrame = Parsing_NumberInt();
			break;
#pragma endregion
#pragma region 버텍스와 인덱스, 텍스쳐
		case TOKENR_MESH_NUMVERTEX: //메쉬 버텍스 사이즈.
			Node.back()->vertices.resize(Parsing_NumberInt());
			break;
		case TOKENR_MESH_NUMFACES: //페이스 사이즈. * 3 인덱스 사이즈.
			Node.back()->indices.resize(Parsing_NumberInt());
			break;
		case TOKENR_MESH_VERTEX: //버텍스를 읽어 static 변수에 저장시킨다. 
		{
			int Index = 0;
			float x, y, z = 0;
			Index = Parsing_NumberInt();
			x = Parsing_NumberFloat();
			y = Parsing_NumberFloat();
			z = Parsing_NumberFloat();
			Node.back()->vertices[Index].Pos = XMFLOAT3(x, z, y);


			MinPos.x = min(Node.back()->vertices[Index].Pos.x, MinPos.x);
			MinPos.y = min(Node.back()->vertices[Index].Pos.y, MinPos.y);
			MinPos.z = min(Node.back()->vertices[Index].Pos.z, MinPos.z);

			MaxPos.x = max(Node.back()->vertices[Index].Pos.x, MaxPos.x);
			MaxPos.y = max(Node.back()->vertices[Index].Pos.y, MaxPos.y);
			MaxPos.z = max(Node.back()->vertices[Index].Pos.z, MaxPos.z);


			break;
		}
		case TOKENR_MESH_FACE: // 메쉬 폴리곤이 가지고 있는 버텍스의 인덱스를 저장 a,b,c
		{
			int Index, x, y, z = 0;
			Index = Parsing_NumberInt();
			Parsing_String();
			x = Parsing_NumberInt();
			Parsing_String();
			y = Parsing_NumberInt();
			Parsing_String();
			z = Parsing_NumberInt();
			Node.back()->indices[Index] = index3(x, z, y);
		}
		break;
		case TOKENR_MESH_VERTEXNORMAL: //버텍스의 노말벡터 저장.
		{
			int Index = 0;
			float x, y, z = 0;
			Index = Parsing_NumberInt();

			x = Parsing_NumberFloat();
			y = Parsing_NumberFloat();
			z = Parsing_NumberFloat();
			FaceNormal::VertexNormal Temp;
			Temp.index = Index;
			Temp.normal = XMFLOAT3(x, z, y);
			Node.back()->faceNormal.back().vertexNormal.push_back(Temp);
			break;
		}

		case TOKENR_MESH_FACENORMAL: // 없어도 됨.
		{

			FaceCount = Parsing_NumberInt();

			FaceNormal _Temp;
			_Temp.index = FaceCount;
			Node.back()->faceNormal.push_back(_Temp);

			break;
		}
		case TOKENR_MESH_NUMTVERTEX: // 텍스쳐 버텍스 사이즈 변경
			Node.back()->Tvertex.resize(Parsing_NumberInt());
			break;
		case TOKENR_MESH_NUMTVFACES: // 텍스쳐 페이스 사이즈 리사이징
			Node.back()->TFace.resize(Parsing_NumberInt());
			break;

		case TOKENR_MESH_TVERT: // 텍스쳐값. uxuy
		{
			int Index = Parsing_NumberInt();
			float ux, uy;
			ux = Parsing_NumberFloat();
			uy = Parsing_NumberFloat();
			if (Node.back()->Tvertex[Index].x == 0 && Node.back()->Tvertex[Index].y == 0)
				Node.back()->Tvertex[Index] = XMFLOAT2(ux, 1 - uy);  //1 - 로 뒤집어주기
		}
		break;
		case TOKENR_MESH_TFACE: // 텍스쳐 페이스
		{
			int Index = Parsing_NumberInt();
			float ux, uy, uz;
			ux = Parsing_NumberFloat();
			uy = Parsing_NumberFloat();
			uz = Parsing_NumberFloat();
			Node.back()->TFace[Index] = XMFLOAT3(ux, uz, uy);
			break;
		}
#pragma endregion
#pragma region 머터리얼
		case TOKENR_MATERIAL:
			Mtl.push(new Material());
			Mtl.back()->Reflect = XMFLOAT4(1, 1, 1, 1);
			break;
		case TOKENR_MATERIAL_AMBIENT:
			Mtl.back()->Ambient = XMFLOAT4(Parsing_NumberFloat(), Parsing_NumberFloat(), Parsing_NumberFloat(), 1);
			break;
		case TOKENR_MATERIAL_DIFFUSE:
			Mtl.back()->Diffuse = XMFLOAT4(Parsing_NumberFloat(), Parsing_NumberFloat(), Parsing_NumberFloat(), 1);
			break;
		case TOKENR_MATERIAL_SPECULAR:
			Mtl.back()->Specular = XMFLOAT4(Parsing_NumberFloat(), Parsing_NumberFloat(), Parsing_NumberFloat(), 1);
			break;
		case TOKENR_BITMAP:
		{

			Path = Parsing_String();
			if (Path == "None")
				Path.clear();
			break;
		}
#pragma endregion
#pragma region 노드
		case TOKENR_GEOMOBJECT:
			Node.push(new NodeInfo());
			Node.back()->m_AniKeys.clear();

			break;
		case TOKENR_HELPEROBJECT:
			Node.push(new NodeInfo());
			Node.back()->m_AniKeys.clear();

			break;
		case TOKENR_SHAPEOBJECT:
			Node.push(new NodeInfo());
			Node.back()->m_AniKeys.clear();

			break;
		case TOKENR_NODE_NAME: // 노드 이름.
			if (!Node.back()->m_AnimationName.empty())
			{
				Node.back()->m_AnimationName = Parsing_String();
			}
			else
				Node.back()->m_Name = Parsing_String();


			break;

		case TOKENR_NODE_PARENT: // 부모 이름.
			Node.back()->m_ParentName = Parsing_String();
			break;

			/// <summary>
			/// y z 바꿔서 받는중 나중에 문제 생길지도..?
		case TOKENR_TM_ROW0:
			Node.back()->m_Row0.x = Parsing_NumberFloat();
			Node.back()->m_Row0.z = Parsing_NumberFloat();
			Node.back()->m_Row0.y = Parsing_NumberFloat();
			break;
		case TOKENR_TM_ROW1:
			Node.back()->m_Row1.x = Parsing_NumberFloat();
			Node.back()->m_Row1.z = Parsing_NumberFloat();
			Node.back()->m_Row1.y = Parsing_NumberFloat();
			break;
		case TOKENR_TM_ROW2:
			Node.back()->m_Row2.x = Parsing_NumberFloat();
			Node.back()->m_Row2.z = Parsing_NumberFloat();
			Node.back()->m_Row2.y = Parsing_NumberFloat();
			break;
		case TOKENR_TM_ROW3:
			Node.back()->m_Row3.x = Parsing_NumberFloat();
			Node.back()->m_Row3.z = Parsing_NumberFloat();
			Node.back()->m_Row3.y = Parsing_NumberFloat();
			break;


#pragma endregion
#pragma region Animation
		case TOKENR_ANIMATION_NAME:
		{
			AnimationKey _newAnimation;
			Node.back()->m_AnimationName = Parsing_String();
			_newAnimation.m_Name = Node.back()->m_AnimationName;
			Node.back()->m_AniKeys.push_back(_newAnimation);
			break;
		}
		case TOKENR_TM_ANIMATION:
		{
			// 애니메이션 발견!
			break;
		}
		case TOKENR_CONTROL_POS_SAMPLE: // 애니메이션 pos
		{
			if (Node.back()->m_AniKeys.empty())
				break; // 옛날 익스포터로 출력한 애니메이션들은 걸러주자.
			int _Time = Parsing_NumberInt() / _TicksPerFrame;

			float x = Parsing_NumberFloat();
			float y = Parsing_NumberFloat();
			float z = Parsing_NumberFloat();
			PosKey _PosKey(_Time, XMFLOAT3(x, z, y));
			Node.back()->m_AniKeys.back().m_PosKey.push_back(_PosKey);

			Node.back()->m_AniKeys.back().m_LastFrame = max(Node.back()->m_AniKeys.back().m_LastFrame, _Time);
			Node.back()->m_AniKeys.back().m_FirstFrame = min(Node.back()->m_AniKeys.back().m_FirstFrame, _Time);
			break;
		}
		case TOKENR_CONTROL_ROT_SAMPLE: // 애니메이션 Rot 
		{
			if (Node.back()->m_AniKeys.empty())
				break; // 옛날 익스포터로 출력한 애니메이션들은 걸러주자.
			int _Time = Parsing_NumberInt() / _TicksPerFrame;

			float x = Parsing_NumberFloat();
			float y = Parsing_NumberFloat();
			float z = Parsing_NumberFloat();
			float w = Parsing_NumberFloat();

			XMVECTOR Quaternion = XMVectorSet(x, z, y, w);

			RotKey _RotKey(_Time, Quaternion);
			Node.back()->m_AniKeys.back().m_RotKey.push_back(_RotKey);

			Node.back()->m_AniKeys.back().m_LastFrame = max(Node.back()->m_AniKeys.back().m_LastFrame, _Time);
			Node.back()->m_AniKeys.back().m_FirstFrame = min(Node.back()->m_AniKeys.back().m_FirstFrame, _Time);
			break;
		}
		case TOKENR_WIREFRAME_COLOR:
			Node.back()->m_isWireFrame = true;
			break;
#pragma endregion
#pragma region Skinning

		case TOKENR_MESH_NUMBONE: //본이 몇개 있는지.
			break;
		case TOKENR_BONE_NAME: //본의 이름이 무엇인지
			m_BoneNameList.push_back(Parsing_String());
			break;
		case TOKENR_MESH_WVERTEXS: //생략 가능. 
			break;
		case TOKENR_MESH_WEIGHT: // n번 버텍스의 본의 가중치들.
			_BoneIndex = Parsing_NumberInt();
			break;
		case TOKENR_BONE_BLENGING_WEIGHT: // n번 본 , 가중치 값
		{
			int _Index = Parsing_NumberInt();
			float _Wegiht = Parsing_NumberFloat();

			if (Node.back()->vertices[_BoneIndex].Weights.x <= 0.0001f)
			{
				Node.back()->vertices[_BoneIndex].BoneIndex[0] = _Index;
				Node.back()->vertices[_BoneIndex].Weights.x = _Wegiht;
			}
			else if (Node.back()->vertices[_BoneIndex].Weights.y <= 0.0001f)
			{
				Node.back()->vertices[_BoneIndex].BoneIndex[1] = _Index;
				Node.back()->vertices[_BoneIndex].Weights.y = _Wegiht;
			}
			else if (Node.back()->vertices[_BoneIndex].Weights.z <= 0.0001f)
			{
				Node.back()->vertices[_BoneIndex].BoneIndex[2] = _Index;
				Node.back()->vertices[_BoneIndex].Weights.z = _Wegiht;
			}
			else
			{
				Node.back()->vertices[_BoneIndex].BoneIndex[3] = _Index;
				//4개가 넘으면? 몰루
			}

			break;
		}
#pragma endregion
#pragma region 마무으리
		case TOKEND_END:
			// 아마도 이건 파일의 끝이 나타났을때인것 같은데. while을 탈출해야 하는데?
			//AfxMessageBox("파일의 끝을 본 것 같습니다!", MB_OK, NULL);
			TRACE("TRACE: 파싱중: 파일의 끝을 봤습니다!\n");
			return;
			break;
			/// 위의 아무것도 해당하지 않을때
		default:
			// 아무것도 하지 않는다.
			break;
#pragma endregion
		}
		/// 안전 코드.
		i++;
		if (i > 1000000)
		{
			TRACE("루프를 백만번 돌았습니다!");
			return;
		}
	}		// while()
	return;
}


///----------------------------------------------------------------------
/// parsing을 위한 단위별 함수들
///----------------------------------------------------------------------

// long을 읽어서 리턴해준다.
int CASEParser::Parsing_NumberLong()
{
	LONG			token;
	LONG			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = strtoul(m_TokenString, NULL, 10);

	return			tempNumber;
}

// float
float CASEParser::Parsing_NumberFloat()
{
	LONG			token;
	float			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (float)atof(m_TokenString);

	return			tempNumber;
}

// String
LPSTR CASEParser::Parsing_String()
{
	/// ※m_TokenString ( char[255] ) 이기 때문에 CString에 넣으면 에러 날거라 생각했는데, 생각보다 CString은 잘 만들어진 것 같다. 알아서 받아들이는데?
	m_lexer->GetToken(m_TokenString);

	return m_TokenString;
}


// int
int CASEParser::Parsing_NumberInt() {

	LONG			token;
	int				tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (int)atoi(m_TokenString);

	return			tempNumber;
}

