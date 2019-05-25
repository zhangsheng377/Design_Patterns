#pragma once
#include <vector>
#include <queue>
#include <algorithm>

namespace utils {
	class assign {
	public:
		enum allocate_mode {
			fifo,
			//TODO:增加随机分配模式
		};
		enum expand_mode {
			no_expand ,
			increase_one,
			increase_double,
		};
		
		explicit assign(const size_t initial_id_size = 30, const allocate_mode allocate = fifo, const expand_mode expand = increase_double);
		int allocate();
		void release(const size_t index);
	private:
		std::vector<bool> bitmap_;
		std::queue<size_t> fifo_;

		allocate_mode allocate_mode_;
		expand_mode expand_mode_;

		//TODO:考虑抽象为策略模式
		int allocate_fifo();
		void allocate_resize(const size_t old_size);
		void allocate_resize_fifo(const size_t old_size);
		void allocate_init();
		void allocate_init_fifo();

		void release_fifo(const size_t index);

		void expand();
		size_t expand_no_expand() const;
		size_t expand_increase_one() const;
		size_t expand_increase_double() const;
	};

	inline assign::assign(const size_t initial_id_size, const allocate_mode allocate, const expand_mode expand) {
		bitmap_.resize(initial_id_size, false);
		allocate_mode_ = allocate;
		expand_mode_ = expand;

		allocate_init();
	}

	inline int assign::allocate() {
		auto assign = -1;

		switch (allocate_mode_) {
		case fifo:
			assign = allocate_fifo();
			break;
		default:
			assign = -1;
			break;
		}

		if (assign > -1 && static_cast<size_t>(assign) < bitmap_.size()) {
			bitmap_[assign] = true;
			return assign;
		}

		return -1;
	}

	inline void assign::release(const size_t index) {
		if (index >= bitmap_.size() || !bitmap_[index]) {
			return;
		}
		
		bitmap_[index] = false;
		switch (allocate_mode_) {
		case fifo:
			release_fifo(index);
			break;
		default:
			break;
		}
	}

	inline int assign::allocate_fifo() {
		auto assign = -1;

		if (fifo_.empty()) {
			expand();
		}

		if (!fifo_.empty()) {
			assign = fifo_.front();
			fifo_.pop();
		}

		return assign;
	}

	inline void assign::allocate_resize(const size_t old_size) {
		switch (allocate_mode_) {
		case fifo:
			allocate_resize_fifo(old_size);
			break;
		default:
			break;
		}
	}

	inline void assign::allocate_resize_fifo(const size_t old_size) {
		for (auto i = old_size; i < bitmap_.size(); ++i) {
			fifo_.push(i);
		}
	}

	inline void assign::allocate_init() {
		switch (allocate_mode_) {
		case fifo:
			allocate_init_fifo();
			break;
		default:
			break;
		}
	}

	inline void assign::allocate_init_fifo() {
		for (auto i = 0; i < bitmap_.size(); ++i) {
			fifo_.push(i);
		}
	}

	inline void assign::release_fifo(const size_t index) {
		fifo_.push(index);
	}

	inline void assign::expand() {
		const auto old_size = bitmap_.size();
		auto new_size = old_size;

		switch (expand_mode_) {
		case no_expand:
			new_size = expand_no_expand();
			break;
		case increase_one:
			new_size = expand_increase_one();
			break;
		case increase_double:
			new_size = expand_increase_double();
			break;
		default:
			new_size = old_size;
			break;
		}

		bitmap_.resize(new_size, false);
		
		allocate_resize(old_size);
	}

	inline size_t assign::expand_no_expand() const {
		return bitmap_.size();
	}

	inline size_t assign::expand_increase_one() const {
		return bitmap_.size() + 1;
	}

	inline size_t assign::expand_increase_double() const {
		return bitmap_.size() * 2;
	}
}
