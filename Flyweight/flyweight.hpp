#pragma once
#include <iostream>
#include <map>
#include <set>

template <typename T>
class flyweight_factory
{
public:
	static auto get_id_by_obj(T* obj) -> int;
	static auto get_obj_by_id(const int id) -> T*;
	static auto dereference_obj_by_id(const int id) -> void;
	static void dereference_obj_by_obj(T* obj);

	//TODO:考虑增加log计数

private:
	flyweight_factory();
	~flyweight_factory();
	 
	class flyweight
	{
	public:
		flyweight(int id, T* obj);
		explicit flyweight(T* obj): id_(-1),count_(0) { obj_ = *obj; }
		~flyweight();
		inline int get_id() const { return id_; }
		inline T* get_obj() { return &obj_; }
		bool operator<(const flyweight& b) const { return obj_ < b.obj_; }
		void decrease_count();
		inline void increase_count() { ++count_; }
		inline int get_count() const { return count_; }
	private:
		int id_;
		int count_;
		T obj_;

		//TODO:考虑添加触发器
	};
	class FLYWEIGHT_CMP
	{
	public:
		bool operator()(const flyweight* a, const flyweight* b) const { return *a < *b; }
	};

	static std::map<int, flyweight*> map_id_flyweight_;

	//如果直接以T*作为key,则需要app实现T*类型的比较函数,否则是直接按照指针地址大小比较,所以此方案不是太好
	//若以T实例作为key,虽然可以进行查找,但多保存了一份实例,不符合享元的思想
	static std::set<flyweight*, FLYWEIGHT_CMP> set_obj_flyweight_;

	//TODO:考虑使用优先向后分配的bitmap来管理id的分配
	static int obj_num_;
};


template <typename T>
std::map<int, typename flyweight_factory<T>::flyweight*> flyweight_factory<T>::map_id_flyweight_;

template <typename T>
std::set<typename flyweight_factory<T>::flyweight*, typename flyweight_factory<T>::FLYWEIGHT_CMP> flyweight_factory<T>::set_obj_flyweight_;

template <typename T>
int flyweight_factory<T>::obj_num_ = 0;


template<typename T>
inline flyweight_factory<T>::flyweight::flyweight(const int id, T* obj)
{
	id_ = id;
	count_ = 0;
	obj_ = *obj;
	std::cout << "FLYWEIGHT id:" << id_ << std::endl;
}

template<typename T>
inline flyweight_factory<T>::flyweight::~flyweight()
{
	if (id_ != -1)
	{
		std::cout << "~FLYWEIGHT id:" << id_ << std::endl;
	}
}

template <typename T>
void flyweight_factory<T>::flyweight::decrease_count()
{
	if (count_ > 0)
	{
		--count_;
	}
}

template<typename T>
auto flyweight_factory<T>::get_id_by_obj(T* obj) -> int
{
	flyweight flyweight_tmp(obj); //TODO:会增加一次临时拷贝,感觉不太好
	if (set_obj_flyweight_.find(&flyweight_tmp) == set_obj_flyweight_.end())
	{
		auto* flyweight_new = new flyweight(obj_num_, obj);
		set_obj_flyweight_.insert(flyweight_new);
		map_id_flyweight_[obj_num_] = flyweight_new;
		++obj_num_;
	}
	flyweight* tmp = *set_obj_flyweight_.find(&flyweight_tmp);
	tmp->increase_count();
	return tmp->get_id();
}

template <typename T>
auto flyweight_factory<T>::get_obj_by_id(const int id) -> T*
{
	auto it = map_id_flyweight_.find(id);
	if (it == map_id_flyweight_.end()) { return nullptr; }
	auto tmp = it->second;
	//tmp->increase_count(); //凭id取obj是不应该增加计数的,即所有app之间不应该拷贝id,而是在一开始凭临时obj去取id,之后可任意用id来取obj
	return tmp->get_obj();
}

template <typename T>
auto flyweight_factory<T>::dereference_obj_by_id(const int id) -> void
{
	auto it = map_id_flyweight_.find(id);
	if (it != map_id_flyweight_.end())
	{
		flyweight* flyweight_tmp = it->second;
		flyweight_tmp->decrease_count();
		if (flyweight_tmp->get_count() == 0)
		{
			map_id_flyweight_.erase(id);
			set_obj_flyweight_.erase(flyweight_tmp);
			delete flyweight_tmp;
		}
	}
}

template <typename T>
void flyweight_factory<T>::dereference_obj_by_obj(T* obj)
{
	flyweight flyweight_tmp(obj); //TODO:会增加一次临时拷贝,感觉不太好
	auto it = set_obj_flyweight_.find(&flyweight_tmp);
	if (it != set_obj_flyweight_.end())
	{
		flyweight* flyweight_decrease = *it;
		flyweight_decrease->decrease_count();
		if (flyweight_decrease->get_count() == 0)
		{
			map_id_flyweight_.erase(flyweight_decrease->get_id());
			set_obj_flyweight_.erase(flyweight_decrease);
			delete flyweight_decrease;
		}
	}
}

template<typename T>
inline flyweight_factory<T>::flyweight_factory()
{
}

template<typename T>
inline flyweight_factory<T>::~flyweight_factory()
{
}
