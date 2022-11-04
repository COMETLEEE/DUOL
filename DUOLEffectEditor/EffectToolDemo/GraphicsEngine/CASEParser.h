//------------------------------------------------------------
//
// ASE Parser
// Ver 2.1.20080124
// 
// 자체적으로 데이터 구조가 있으며 
// 이것을 D3D용 특정 오브젝트로 변환하는 곳까지 커버한다
//
//------------------------------------------------------------


struct FaceNormal
{
	struct VertexNormal
	{
		unsigned index;
		XMFLOAT3 normal;
	};

	unsigned index;
	XMFLOAT3 faceNormal;
	vector<VertexNormal> vertexNormal;
};

struct NodeInfo
{
	NodeInfo() :m_Row0(), m_Row1(), m_Row2(), m_Row3()
	{
		m_IndexOffset = 0;
		m_VertexOffset = 0;
		m_Name = "";
		m_ParentName = "";
		m_AnimationName = "";

	}
	int m_IndexOffset; // static 너무 많이 쓰는듯?
	int m_VertexOffset; // static 너무 많이 쓰는듯? 구조 바꿀까?

	string m_Name;
	string m_ParentName;
	string m_AnimationName;

	XMFLOAT3 m_Row0;
	XMFLOAT3 m_Row1;
	XMFLOAT3 m_Row2;
	XMFLOAT3 m_Row3;

	vector<Vertex> vertices;
	vector<index3> indices;
	vector<FaceNormal> faceNormal;
	vector<XMFLOAT2> Tvertex;
	vector<XMFLOAT3> TFace;

	bool m_isWireFrame = false;
	vector<AnimationKey> m_AniKeys;



};
enum eObjectType
{
	eObjectType_ObjectRoot, eObjectType_Geomobject, eObjectType_Shape,
	eObjectType_Camera, eObjectType_Light, eObjectType_HelperObject,
	eObjectType_MarkerObject, eObjectType_SkinnedMesh, eObjectType_Bone,
	eObjectType_CMeshPlane, eObjectType_CBullet_Trace,
	//
	eObjectType_BasicSphere, eObjectType_Missile,
	eObjectType_MeshBillboard, eObjectType_PointSprite,			// CMesh상속..
	eObjectType_End,
};

// 교수님이 제공해주신 Lexer
#include "ASEFile.h"
#include <list>

using namespace std;

/// ASE를 파싱하기 위한 클래스. 기본적으로 주어지는 Lexer클래스를 이용한다.

/// <summary>
/// 신성현
///
/// 2021.04.06 개조
/// </summary>
class CASEParser
{
public:
	CASEParser();
	~CASEParser();

	/// 초기화, 로드
	bool Init();
	MeshData* Load(LPSTR p_File);		// ASE파일을 읽어서, 자체 리스트에 넣는다. //초기화 중인 메시를 This로 넣을것..
	bool Convert_Optimize();		// 버텍스(노말), 텍스쳐 값으로 최적화를 해 준다.
	void ParentInit();

private:
	// 토큰을 구별해내기 위한 스트링
	char m_TokenString[256];
	int m_Token;
	MeshData* m_OneMesh;
public:
	/// 기본적인 읽어들인 내용들
	int m_data_asciiexport;			// *3DSMAX_ASCIIEXPORT	200 - 맨 처음 나오는 헤더인데 무슨 의미인지는 모르겠고, 일단 예의상 읽어서 보관해준다.
	/// 한 씬에 N개의 메시가 존재할 수 있다.
	MeshData* m_ParentMesh; //부모?						// 메시 하나의 데이터가 들어갈 구조체 	
							// 메시 하나의 데이터가 들어갈 구조체 	


	vector<string> m_BoneNameList;
	queue<Material*> Mtl;
	queue<NodeInfo*> Node;
	string Path;

	XMFLOAT3 MaxPos;
	XMFLOAT3 MinPos;

private:
	//	Lexcer

	int FindNewIndex(int _Index);
	ASE::CASELexer* m_lexer;
private:
	/// 파싱을 맡게 되는 거대한 재귀함수. 여기서 모든 토큰의 처리를 한다.
	void Parsing_DivergeRecursiveALL(int depth);
	int Parsing_NumberLong();			// long을 읽어서 리턴해준다.
	float Parsing_NumberFloat();		// float
	LPSTR Parsing_String();				// string
	int Parsing_NumberInt();			// int	

};




























