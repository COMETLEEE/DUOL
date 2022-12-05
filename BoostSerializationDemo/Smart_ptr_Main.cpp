#include <fstream>
#include <iostream>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>


class variableClass
{
public:
	variableClass() = default;
	variableClass(int a, std::string b, double c) : _test1(a), _test2(b), _test3(c)
	{}
private:
	int _test1;
	std::string _test2;
	double _test3;
public:
	void show()
	{
		std::cout << _test1 << std::endl;
		std::cout << _test2 << std::endl;
		std::cout << _test3 << std::endl;
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
		ar& _test1;
		ar& _test2;
		ar& _test3;
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
		ar&* _test;
	}

};


int main()
{
	BaseClass data = std::make_shared<variableClass>(0, "�ѱ� �׽�Ʈ", 0);
	std::cout << "[���� ������]" << std::endl;
	data.show();

	// �ּ��� Ǯ�� Ȯ���غ�����.
	// ----------------------------- �ø�������� --------------------------------------
	//std::ofstream fw("test.dat", std::ios_base::binary);
	//if (fw.is_open())
	//{
	//	boost::archive::text_oarchive outArchive(fw);

	//	//boost::archive::binary_oarchive outArchive(fw);

	//	outArchive << data;

	//	fw.close();
	//}
	// ----------------------------- �ø�������� --------------------------------------


	// ----------------------------- ��ø�������� --------------------------------------
	std::ifstream fr("test.dat", std::ios_base::binary);
	if (fr.is_open())
	{

		boost::archive::text_iarchive inArchive(fr);

		//boost::archive::binary_iarchive inArchive(fr);

		inArchive >> data;

		std::cout << "[���� ������]" << std::endl;

		data.show();

		fr.close();
	}
	// ----------------------------- ��ø�������� --------------------------------------

	return 0;
}