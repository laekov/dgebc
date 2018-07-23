#ifndef DUMMY_HH
#define DUMMY_HH
#include <cstdlib>
#include <unistd.h>

#include <task.hh>

namespace DGEBC {
	inline Task dummyCalc(Task t) {
		// sleep(1);
		t.score = rand() / 999999999.;
		return t;
	}
	inline std::string dummyMutate(std::string gene) {
		// sleep(1);
		for (int i = gene.length() - 1; i > 0; -- i) {
			if (rand() % 3) {
				gene[i] = (gene[i] - 97 + 1)% 26 + 97;
			} else if (rand() % 3) {
				gene[i] = (gene[i] - 97 + 25)% 26 + 97;
			}
		}
		return gene;
	}
};
#endif
