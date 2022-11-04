#pragma once
struct PosKey
{
	PosKey(int _Time, XMFLOAT3 _Pos) :m_Time(_Time), m_Pos(_Pos) {}
	int m_Time;
	XMFLOAT3 m_Pos;
};

struct RotKey
{
	RotKey(int _Time, XMVECTOR _Rot) :m_Time(_Time), m_Rot(_Rot) {}
	//���ʹϾ��ε�?
	int m_Time;
	XMVECTOR m_Rot; //XMQuaternionRatationAxis �Լ� ����ؼ� ���ʹϾ����� ���� �������.
};

struct ScalKey
{
	ScalKey(int _Time, XMFLOAT3 _Scal) :m_Time(_Time), m_Scal(_Scal) {}
	int m_Time;
	XMFLOAT3 m_Scal;
};

struct AnimationKey
{
	string m_Name;
	// �����ΰ�
	bool m_isAnim = false;
	// ������ �����ϱ�
	void* m_GameObject = nullptr;

	// �� �����ӱ��� �������
	int m_LastFrame = 0;
	int m_FirstFrame = 0;


	vector<PosKey> m_PosKey;
	vector<RotKey> m_RotKey;
	vector<ScalKey> m_ScalKey;



	void clear()
	{
		m_Name = "";
		m_GameObject = nullptr;
		m_isAnim = false;
		m_PosKey.clear();
		m_RotKey.clear();
		m_ScalKey.clear();
	}
};

