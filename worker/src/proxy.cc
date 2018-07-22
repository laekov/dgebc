#include <cstdlib>
#include <cstring>

#include <civetweb.h>

#include <proxy.hh>

#include <msgque.hh>
#include <task.hh>

namespace DGEBC {

	MsgQue<Task> *task_q;
	MsgQue<Task> *res_q;
	int proxyHandler(struct mg_connection* conn) {
		const struct mg_request_info *ri = mg_get_request_info(conn);
		char post_data[4096];
		char buf[4096];
		int post_data_len;
		if (!strcmp(ri->uri, "/spread_gene")) {
			Task t;
			post_data_len = mg_read(conn, post_data, sizeof(post_data));
			mg_get_var(post_data, post_data_len, "gene", buf, sizeof(buf));
			t.gene = std::string(buf);
			mg_get_var(post_data, post_data_len, "score", buf, sizeof(buf));
			t.score = atof(buf);
			task_q->en(t);
		}
	}
	void* proxyMain(void* args) {
	 	task_q = static_cast<MsgQue<Task>*>(args);
		res_q = static_cast<MsgQue<Task>*>(args) + 1;
	}
};