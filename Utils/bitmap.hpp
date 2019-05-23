#pragma once
#include <vector>

namespace utils {
	class  bitmap {
	public:
		enum expand_mode {
			no_expand = 0,
			increase_one,
			increase_double,
		};
		explicit bitmap(const int initial_id_size = 30);
		int assign(bool FIFO, expand_mode expand);
	private:
		std::vector<bool> bitmap_;
		size_t FIFO_cursor;
	};

	inline bitmap::bitmap(const int initial_id_size) {
		bitmap_.resize(initial_id_size, false);
		FIFO_cursor = 0;
	}
}
