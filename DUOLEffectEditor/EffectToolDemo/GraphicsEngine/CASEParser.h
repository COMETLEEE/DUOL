//------------------------------------------------------------
//
// ASE Parser
// Ver 2.1.20080124
// 
// ��ü������ ������ ������ ������ 
// �̰��� D3D�� Ư�� ������Ʈ�� ��ȯ�ϴ� ������ Ŀ���Ѵ�
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
	int m_IndexOffset; // static �ʹ� ���� ���µ�?
	int m_VertexOffset; // static �ʹ� ���� ���µ�? ���� �ٲܱ�?

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
	eObjectType_MeshBillboard, eObjectType_PointSprite,			// CMesh���..
	eObjectType_End,
};

// �������� �������ֽ� Lexer
#include "ASEFile.h"
#include <list>

using namespace std;

/// ASE�� �Ľ��ϱ� ���� Ŭ����. �⺻������ �־����� LexerŬ������ �̿��Ѵ�.

/// <summary>
/// �ż���
///
/// 2021.04.06 ����
/// </summary>
class CASEParser
{
public:
	CASEParser();
	~CASEParser();

	/// �ʱ�ȭ, �ε�
	bool Init();
	MeshData* Load(LPSTR p_File);		// ASE������ �о, ��ü ����Ʈ�� �ִ´�. //�ʱ�ȭ ���� �޽ø� This�� ������..
	bool Convert_Optimize();		// ���ؽ�(�븻), �ؽ��� ������ ����ȭ�� �� �ش�.
	void ParentInit();

private:
	// ��ū�� �����س��� ���� ��Ʈ��
	char m_TokenString[256];
	int m_Token;
	MeshData* m_OneMesh;
public:
	/// �⺻���� �о���� �����
	int m_data_asciiexport;			// *3DSMAX_ASCIIEXPORT	200 - �� ó�� ������ ����ε� ���� �ǹ������� �𸣰ڰ�, �ϴ� ���ǻ� �о �������ش�.
	/// �� ���� N���� �޽ð� ������ �� �ִ�.
	MeshData* m_ParentMesh; //�θ�?						// �޽� �ϳ��� �����Ͱ� �� ����ü 	
							// �޽� �ϳ��� �����Ͱ� �� ����ü 	


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
	/// �Ľ��� �ð� �Ǵ� �Ŵ��� ����Լ�. ���⼭ ��� ��ū�� ó���� �Ѵ�.
	void Parsing_DivergeRecursiveALL(int depth);
	int Parsing_NumberLong();			// long�� �о �������ش�.
	float Parsing_NumberFloat();		// float
	LPSTR Parsing_String();				// string
	int Parsing_NumberInt();			// int	

};




























