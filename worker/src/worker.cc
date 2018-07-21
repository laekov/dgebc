#include <cstdlib>

#include <worker.hh>

#include <task.hh>
#include <msgque.hh>

namespace DGEBC {
	Task dummyCalc(Task t) {
		t.score = rand();
		return t;
	}
	
	void* workerMain(void* args) {
		MsgQue<Task> *task_q = static_cast<MsgQue<Task>*>(args);
		MsgQue<Task> *res_q = static_cast<MsgQue<Task>*>(args) + 1;
		while (1) {
			Task t(task_q->de());
			// TODO(laekov): use real calculation
			Task result(dummyCalc(t));
			res_q->en(result);
		}
	}
};
