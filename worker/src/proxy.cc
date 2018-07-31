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
		extern double current_max;
		extern int throughput;
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
			res_q->en(t, 0);
			sprintf(buf, "Done\n");
		} else if (!strcmp(ri->uri, "/status")) {
			sprintf(buf, "g,%d", throughput);
		} else if (!strcmp(ri->uri, "/get_status")) {
			sprintf(buf, "total tasks:\t%d\n"
					"finished tasks:\t%d\n"
					"pending tasks:\t%d\n"
					"throughput:\t%d\n"
					"current max:\t%.10lf\n",
					task_q->tot(),
					res_q->tot(),
					task_q->sz(),
					throughput,
					current_max);
		} else {
			std::cerr << "Unknown uri " << ri->uri << "\n";
		}
		mg_printf(conn,
				"HTTP/1.1 200 OK\r\n"
				"Content-Type: text/plain\r\n"
				"Content-Length: %d\r\n"     
				"\r\n"
				"%s",
				strlen(buf), buf);
	}
	void* proxyMain(void* args) {
		task_q = static_cast<MsgQue<Task>*>(args);
		res_q = static_cast<MsgQue<Task>*>(args) + 1;
	}
};
