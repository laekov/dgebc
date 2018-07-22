#ifndef TASK_HH
#define TASK_HH
#include <string>
#include <iostream>

namespace DGEBC {
	struct Task {
		std::string gene, *parents;
		double score;
		Task(): gene(), parents(0), score(-1.) {}
		inline void show() {
			std::cout << "Task " << gene << " " << score << std::endl;
		}
	};
};
#endif
