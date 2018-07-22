#include <unistd.h>

#include <scheduler.hh>

#include <task.hh>
#include <msgque.hh>

#include <dummy.hh>

namespace DGEBC {
	void* schedulerMain(void* args) {
		MsgQue<Task> *task_q = static_cast<MsgQue<Task>*>(args);
		MsgQue<Task> *res_q = static_cast<MsgQue<Task>*>(args) + 1;
		Task tmp;
		while (1) {
			Task c(res_q->de());
			Task d;
			d.gene = dummyMutate(c.gene);
			d.score = -1;
			d.parents = new std::string[2];
			d.parents[0] = std::string(c.gene);
			task_q->en(d);
		}
	}
};
