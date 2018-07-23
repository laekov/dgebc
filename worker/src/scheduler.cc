#include <unistd.h>

#include <civetweb.h>

#include <scheduler.hh>

#include <task.hh>
#include <msgque.hh>

#include <dummy.hh>

namespace DGEBC {
	char *server_addr, *server_port;

	std::vector<std::string> fellows;

	void setServer(char* addr, char* port) {
		server_addr = addr;
		server_port = port;
	}

	void registerWorker() {
		struct mg_context *conn;
		char buf[4096];
		conn = mg_download(server_addr, atoi(server_port), 0, buf, sizeof(buf),
				"GET /register_worker?port=%s HTTP/1.0\r\nHost: %s\r\n\r\n",
				server_port, server_addr);
		mg_close_connection(conn);
	}

	void updateClientList() {
		struct mg_context *conn;
		char buf[4096];
		int szread;
		conn = mg_download(server_addr, atoi(server_port), 0, buf, sizeof(buf),
				"GET /get_workers HTTP/1.0\r\nHost: %s\r\n\r\n", server_addr);
		std::string response;
		while ((szread = mg_read(conn, buf, sizeof(buf) - 1)) > 0) {
			buf[szread] = 0;
			response += std::string(buf);
		}
	}

	void* schedulerMain(void* args) {
		MsgQue<Task> *task_q = static_cast<MsgQue<Task>*>(args);
		MsgQue<Task> *res_q = static_cast<MsgQue<Task>*>(args) + 1;
		Task tmp, survivor;
		tmp.score = -1;
		registerWorker();
		for (int cnt = 0; ; ++ cnt) {
			if (cnt % 1031 == 0) {
				updateClientList();
			} else if (cnt % 97 == 0) {
			} else {
				Task c(res_q->de());
				Task d;
				d.gene = dummyMutate(c.gene);
				d.score = -1;
				d.parents.push_back(c.gene);
				task_q->en(d);
				if (tmp.score != -1) {
					Task e;
					e.gene = dummyCombine(tmp.gene, c.gene);
					e.score = -1;
					d.parents.push_back(tmp.gene);
					d.parents.push_back(c.gene);
					task_q->en(d);
				}
				tmp = c;
			}
		}
	}
};
