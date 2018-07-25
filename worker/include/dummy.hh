#ifndef DUMMY_HH
#define DUMMY_HH
#include <cstdlib>
#include <unistd.h>

#include <task.hh>

namespace DGEBC {
	inline Task dummyCalc(Task t) {
		// sleep(1);
		int s(0);
		int l(t.gene.length());
		for (int i = 0; i < l; ++ i) {
			if (t.gene[i] == 'a') {
				++ s;
			}
		}
		t.score = s / (double)t.gene.length();
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

	inline std::string dummyCombine(std::string a, std::string b) {
		int la(a.length()), lb(b.length());
		std::string s("");
		for (int i = 0, j = 0; i < la || j < lb;) {
			int c(rand() % 4);
			if (c == 0 && i < la) {
				s += a[i ++];
			} else if (c == 1 && j < lb) {
				s += b[j ++];
			} else if (c == 2) {
				++ i;
			} else if (c == 3) {
				++ j;
			}
		}
		if (s.length() == 0) {
			s += a[0];
		}
		return s;
	}
};
#endif
