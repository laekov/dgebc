#include <cstdio>

#include <worker.hh>

#include <task.hh>
#include <msgque.hh>

#include <dummy.hh>

namespace DGEBC {
	void* workerMain(void* args) {
		MsgQue<Task> *task_q = static_cast<MsgQue<Task>*>(args);
		MsgQue<Task> *res_q = static_cast<MsgQue<Task>*>(args) + 1;
		while (1) {
			Task t(task_q->de());
			printf("Task got %lf %s\n", t.score, t.gene.c_str());
			// TODO(laekov): use real calculation
			Task result(dummyCalc(t));
			res_q->en(result);
		}
	}
};
