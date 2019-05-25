#pragma once
#include <iostream>
#include <vector>
#include "flyweight.hpp"

class test_flyweight
{
public:
	static auto test(int test_num, int flyweight_num) -> void;
	~test_flyweight();

private:
	class test_flyweight_data
	{
	public:
		test_flyweight_data();
		explicit test_flyweight_data(int data);
		bool operator<(const test_flyweight_data& b) const;
		auto get_data() const -> int;
	private:
		int data_;
	};

	//app��洢�����ݷ��ױ���벻���,ͨ���Ѳ�����Ҵ������õ�������ȡ����,������Ԫ��������,��ʡ�ڴ�
	int data0_; //�ױ��
	int data1_id_; //�����,����Ԫ����,����ֻ�洢��Ԫ�������Դid����

	test_flyweight(int data0, test_flyweight_data* data1);
	test_flyweight(int data0, int data1_id);
};

inline auto test_flyweight::test(const int test_num, const int flyweight_num) -> void
{
	std::vector<test_flyweight*> test_flyweights;
	auto test_id = -1;

	//add by obj
	for (auto i = 0; i < test_num; ++i) {
		const auto test = new test_flyweight_data(i % flyweight_num);
		test_flyweights.push_back(new test_flyweight(i, test));
		delete(test);
	}
	//get by id
	for (auto i = 0; i < test_num; ++i) {
		const auto test = flyweight_factory<test_flyweight_data>::get_obj_by_id(test_flyweights[i]->data1_id_);
		if(test == nullptr)
		{
			std::cout << "get by id " << i << " is null!" << std::endl;
			continue;
		}
		if (test->get_data() != i % flyweight_num) {
			std::cout << "get by id " << i << " error!" << std::endl;
			continue;
		}
	}

	//another data type
	test_id = flyweight_factory<int>::get_id_by_obj(&test_id);
	if (test_id != 0) {
		std::cout << "get by another type is not start from 0!" << std::endl;
	}

	//release by id/obj
	for (auto i = 0; i < test_num; ++i) {
		delete test_flyweights[i];
	}
	test_flyweights.clear();
}

inline test_flyweight::~test_flyweight()
{
	if (rand() % 2 == 0)
	{
		//release by id
		flyweight_factory<test_flyweight_data>::dereference_obj_by_id(data1_id_);
	}
	else
	{
		//release by obj
		const auto test = flyweight_factory<test_flyweight_data>::get_obj_by_id(data1_id_);
		flyweight_factory<test_flyweight_data>::dereference_obj_by_obj(test);
	}
}

inline test_flyweight::test_flyweight_data::test_flyweight_data()
{
	data_ = -1;
}

inline bool test_flyweight::test_flyweight_data::operator<(const test_flyweight_data& b) const
{
	return data_ < b.data_;
}

inline auto test_flyweight::test_flyweight_data::get_data() const -> int
{
	return data_;
}

inline test_flyweight::test_flyweight(const int data0, test_flyweight_data* data1) {
	data0_ = data0;
	data1_id_ = flyweight_factory<test_flyweight_data>::get_id_by_obj(data1);
}

inline test_flyweight::test_flyweight(const int data0, const int data1_id) {
	data0_ = data0;
	data1_id_ = data1_id;
}

inline test_flyweight::test_flyweight_data::test_flyweight_data(const int data) {
	data_ = data;
}
