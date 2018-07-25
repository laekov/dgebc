#ifndef TASK_HH
#define TASK_HH
#include <string>
#include <vector>
#include <iostream>

namespace DGEBC {
	struct Task {
		std::string gene;
		std::vector<std::string> parents;
		double score;
		Task(): gene(), score(-1.) {}
		inline void show() {
			std::cout << "Task " << gene << " " << score << std::endl;
		}
	};
};
#endif
