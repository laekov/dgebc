#include <cstdio>

#include <worker.hh>

#include <task.hh>
#include <msgque.hh>
#include <../../engine/engine.h>

namespace DGEBC {
	void* workerMain(void* args) {
		MsgQue<Task> *task_q = static_cast<MsgQue<Task>*>(args);
		MsgQue<Task> *res_q = static_cast<MsgQue<Task>*>(args) + 1;
		
		DGEBC::Engine engine;
		while (1) {
			Task t(task_q->de());
			t.score = engine.score(t.gene);
			res_q->en(t);
		}
	}
};
