#include <fstream>
#include <iostream>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>


class variableClass
{
public:
	variableClass() = default;
	variableClass(int a, std::string b, int c) : _test1(a), _test2(b), _test3(c)
	{

		_test4[0] = 444;
		_test4[1] = 333;

	}
private:
	int _test1;
	std::string _test2;
	int _test3;
	float _test4[2];
public:
	void show()
	{
		std::cout << _test1 << std::endl;
		std::cout << _test2 << std::endl;
		std::cout << _test3 << std::endl;
		std::cout << _test4[0] << std::endl;
		std::cout << _test4[1] << std::endl;
	}

protected:
	// ---------------------------------시리얼 라이즈를 수행할 객체에는 이것을 정의 하여야 한다. -----------------------------

	// 시리얼 라이즈를 위해 boost::serialization::access 클래스를 프랜드로 등록해야 된다.
	friend class boost::serialization::access;
	// 이 함수를 통해 시리얼라이즈를 수행.
	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		// boost 라이브러리에서 & 연산자를 재정의한 듯 하다.
		// 모든 멤버 변수를 & 연산자를 통해 넘겨준다.
		ar& _test3;
		ar& _test2;
		ar& _test1;
		ar& _test4;
	}
	// ---------------------------------시리얼 라이즈를 수행할 객체에는 이것을 정의 하여야 한다. -----------------------------
};

class BaseClass
{
public:
	BaseClass() = default;
	BaseClass(std::shared_ptr<variableClass> test)
	{
		_test = test;
	};

	std::shared_ptr<variableClass> _test;
public:
	void show()
	{
		_test->show();
	}

protected:
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& _test;
	}

};


int main()
{
	BaseClass data = std::make_shared<variableClass>(12312312, "한글 테스트", 112312300);
	//std::cout << "[원본 데이터]" << std::endl;
	//data.show();

	//// 주석을 풀고 확인해보세요.
	//////---------------------------- - 시리얼라이즈--------------------------------------
	std::ofstream fw("test.dat", std::ios_base::binary);
	if (fw.is_open())
	{
		boost::archive::text_oarchive outArchive(fw);

		//boost::archive::binary_oarchive outArchive(fw);

		outArchive << data;

		fw.close();
	}
	// ----------------------------- 시리얼라이즈 --------------------------------------
	data.show();

	// ----------------------------- 디시리얼라이즈 --------------------------------------
	std::ifstream fr("test.dat", std::ios_base::binary);
	if (fr.is_open())
	{

		//boost::archive::text_iarchive inArchive(fr);

		boost::archive::text_iarchive inArchive(fr);

		inArchive >> data;

		std::cout << "[읽은 데이터]" << std::endl;

		data.show();

		fr.close();
	}
	// ----------------------------- 디시리얼라이즈 --------------------------------------

	return 0;
}