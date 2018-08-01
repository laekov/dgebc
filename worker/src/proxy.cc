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
		char buf[4096];
		if (!strcmp(ri->uri, "/spread_gene")) {
			if (!ri->query_string) {
				std::cerr << "Invalid spread request\n";
			} else {
				int data_len(strlen(ri->query_string));
				Task t;
				mg_get_var(ri->query_string, data_len, "gene",
						   buf, sizeof(buf));
				t.gene = std::string(buf);
				mg_get_var(ri->query_string, data_len, "score",
						   buf, sizeof(buf));
				t.score = atof(buf);
				if (t.gene.length() != 320) {
					sprintf(buf, "Bad request\n");
				} else {
					res_q->en(t, 0);
					sprintf(buf, "Done\n");
				}
			}
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
