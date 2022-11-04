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
	//쿼터니언인듯?
	int m_Time;
	XMVECTOR m_Rot; //XMQuaternionRatationAxis 함수 사용해서 쿼터니언으로 변경 해줘야함.
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
	// 누구인가
	bool m_isAnim = false;
	// 누구를 움직일까
	void* m_GameObject = nullptr;

	// 몇 프레임까지 있을까용
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

