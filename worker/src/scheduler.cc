#include <unistd.h>

#include <scheduler.hh>

#include <task.hh>
#include <msgque.hh>

namespace DGEBC {
	void* schedulerMain(void* args) {
		MsgQue<Task> *task_q = static_cast<MsgQue<Task>*>(args);
		MsgQue<Task> *res_q = static_cast<MsgQue<Task>*>(args) + 1;
		Task t;
		t.gene = std::string("abcde");
		task_q->en(t);
		sleep(10);
	}
};
