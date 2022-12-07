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
	// ---------------------------------�ø��� ����� ������ ��ü���� �̰��� ���� �Ͽ��� �Ѵ�. -----------------------------

	// �ø��� ����� ���� boost::serialization::access Ŭ������ ������� ����ؾ� �ȴ�.
	friend class boost::serialization::access;
	// �� �Լ��� ���� �ø������� ����.
	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		// boost ���̺귯������ & �����ڸ� �������� �� �ϴ�.
		// ��� ��� ������ & �����ڸ� ���� �Ѱ��ش�.
		ar& _test3;
		ar& _test2;
		ar& _test1;
		ar& _test4;
	}
	// ---------------------------------�ø��� ����� ������ ��ü���� �̰��� ���� �Ͽ��� �Ѵ�. -----------------------------
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
	BaseClass data = std::make_shared<variableClass>(12312312, "�ѱ� �׽�Ʈ", 112312300);
	//std::cout << "[���� ������]" << std::endl;
	//data.show();

	//// �ּ��� Ǯ�� Ȯ���غ�����.
	//////---------------------------- - �ø��������--------------------------------------
	std::ofstream fw("test.dat", std::ios_base::binary);
	if (fw.is_open())
	{
		boost::archive::text_oarchive outArchive(fw);

		//boost::archive::binary_oarchive outArchive(fw);

		outArchive << data;

		fw.close();
	}
	// ----------------------------- �ø�������� --------------------------------------
	data.show();

	// ----------------------------- ��ø�������� --------------------------------------
	std::ifstream fr("test.dat", std::ios_base::binary);
	if (fr.is_open())
	{

		//boost::archive::text_iarchive inArchive(fr);

		boost::archive::text_iarchive inArchive(fr);

		inArchive >> data;

		std::cout << "[���� ������]" << std::endl;

		data.show();

		fr.close();
	}
	// ----------------------------- ��ø�������� --------------------------------------

	return 0;
}