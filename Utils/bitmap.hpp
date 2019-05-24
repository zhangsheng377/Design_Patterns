#pragma once
#include <vector>
#include <queue>

namespace utils {
	class  bitmap {
	public:
		enum assign_mode {
			fifo,
		};
		enum expand_mode {
			no_expand ,
			increase_one,
			increase_double,
		};
		
		explicit bitmap(const size_t initial_id_size = 30, const assign_mode assign = fifo, const expand_mode expand = increase_double);
		int assign();
		void release(const size_t index);
	private:
		std::vector<bool> bitmap_;
		std::queue<size_t> fifo_;

		assign_mode assign_mode_;
		expand_mode expand_mode_;

		int assign_fifo();
		void release_fifo(const size_t index);
	};

	inline bitmap::bitmap(const size_t initial_id_size, const assign_mode assign, const expand_mode expand) {
		bitmap_.resize(initial_id_size, false);
		for (size_t i = 0; i < initial_id_size; ++i) {
			fifo_.push(i);
		}
		assign_mode_ = assign;
		expand_mode_ = expand;
	}

	inline int bitmap::assign() {
		auto assign = -1;
		switch (assign_mode_) {
			case fifo: {
				assign = assign_fifo();
			}
			default: {
				assign = -1;
			}
		}

		if (assign > -1 && static_cast<size_t>(assign) < bitmap_.size()) {
			bitmap_[assign] = true;
			return assign;
		}

		return -1;
	}

	inline void bitmap::release(const size_t index)
	{
		if (index < bitmap_.size() && bitmap_[index]) {
			bitmap_[index] = false;
			switch (assign_mode_) {
				case fifo: {
					return release_fifo(index);
				}
			}
		}
	}

	inline int bitmap::assign_fifo() {
		auto assign = -1;
		if (!fifo_.empty()) {
			assign = fifo_.front();
			fifo_.pop();
		}
		return assign;
	}

	inline void bitmap::release_fifo(const size_t index) {
		fifo_.push(index);
	}
}
