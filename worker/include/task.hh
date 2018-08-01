#ifndef TASK_HH
#define TASK_HH
#include <string>
#include <vector>
#include <iostream>

namespace DGEBC {
	struct Task {
		std::string gene;
		std::vector<std::string> parents;
		int outcomer;
		double score;
		Task(): gene(), score(-1.), outcomer(0) {}
		inline void show() {
			std::cout << "Task " << gene << " " << score << std::endl;
		}
	};

	inline bool operator <(const Task& a, const Task& b) {
		return a.score < b.score;
	}

};
#endif
