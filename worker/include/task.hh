#ifndef TASK_HH
#define TASK_HH
#include <string>

namespace DGEBC {
	struct Task {
		std::string gene, *parents;
		double score;
		Task(): gene(), parents(0), score(-1.) {}
	};
};
#endif
