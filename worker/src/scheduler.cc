#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>

#include <civetweb.h>

#include <scheduler.hh>

#include <task.hh>
#include <msgque.hh>
#include <../../engine/engine.h>

namespace DGEBC {
	inline bool operator <(const Task& a, const Task& b) {
		return a.score > b.score;
	}

	char *server_addr, *server_port;

	using std::pair;
	using std::vector;
	using std::string;
	using std::stringstream;

	vector<pair<string, string> > fellows;

	void setServer(char* addr, char* port) {
		server_addr = addr;
		server_port = port;
	}

	void registerWorker() {
		struct mg_connection *conn;
		char buf[4096];
		conn = mg_download(server_addr, atoi(server_port), 0, buf, sizeof(buf),
				"GET /register_worker?port=%s HTTP/1.0\r\nHost: %s\r\n\r\n",
				server_port, server_addr);
		mg_close_connection(conn);
	}

	void updateClientList() {
		struct mg_connection *conn;
		char buf[4096];
		int szread;
		conn = mg_download(server_addr, atoi(server_port), 0, buf, sizeof(buf),
				"GET /get_workers HTTP/1.0\r\nHost: %s\r\n\r\n", server_addr);
		stringstream response;
		while ((szread = mg_read(conn, buf, sizeof(buf) - 1)) > 0) {
			buf[szread] = 0;
			response << buf;
		}
		fellows.clear();
		string addr, port;
		while (response >> addr >> port) {
			fellows.push_back(pair<string, string>(addr, port));
			std::cerr << addr << ":" << port << "\n";
		}
	}

	void spreadGene(Task t) {
		char buf[4096];
		for (auto i: fellows) {
			struct mg_connection *conn;
			conn = mg_download(server_addr, atoi(server_port), 0, buf, 
					sizeof(buf),
					"GET /spread_gene?gene=%s&score=%.9f"
				    " HTTP/1.0\r\nHost: %s\r\n\r\n", 
					t.gene.c_str(), t.score, server_addr);
			mg_close_connection(conn);
		}
	}

	void* schedulerMain(void* args) {
		MsgQue<Task> *task_q = static_cast<MsgQue<Task>*>(args);
		MsgQue<Task> *res_q = static_cast<MsgQue<Task>*>(args) + 1;

		DGEBC::Engine engine;

		Task init_t;
		init_t.gene = engine.initial();
		init_t.score = -1;
		task_q->en(init_t);

		// TODO(laekov): make it variable under server's control
		static const int group_size = 103;
		static const int mutate_const = 10;
		static const int combine_const = 10;
		static const int max_iter = 100;

		double current_max(.1);
		vector<Task> survivor;
		registerWorker();
		for (int cnt = 0; ; ++ cnt) {
			if (cnt % 1031 == 0) {
				updateClientList();
			} else if (cnt % 97 == 0) {
				for (auto it: survivor) {
					spreadGene(it);
				}
			} else {
				Task c(res_q->de());
				/* mutate */
				int num_mutate(c.score / current_max * mutate_const + 1);
				if (num_mutate > max_iter) {
					num_mutate = max_iter;
				}
				for (int j = 0; j < num_mutate; ++ j) {
					Task d;
					d.gene = engine.mutate(c.gene);
					d.score = -1;
					d.parents.push_back(c.gene);
					task_q->en(d);
				}
				
				/* combine with top gene in the group */
				if (survivor.size() > 0) {
					int num_combine(c.score / current_max * combine_const + 1);
					if (num_combine > max_iter) {
						num_combine = max_iter;
					}
					for (int j = 0; j < num_combine; ++ j) {
						Task e;
						int k(rand() % survivor.size());
						e.gene = engine.combine(
								survivor[k].gene,
								c.gene);
						e.score = -1;
						e.parents.push_back(survivor[k].gene);
						e.parents.push_back(c.gene);
						task_q->en(e);
					}
				}

				/* compete against the surviving group */
				if (survivor.size() < group_size) {
					survivor.push_back(c);
				} else {
					int k(rand() % survivor.size());
					int sel_score(survivor[k].score * 10000 + 1);
					int c_score(c.score * 10000 + 1);
					if (rand() % (sel_score + c_score) < c_score) {
						survivor[k] = c;
					}
				}

				if (c.score > current_max) {
					current_max = c.score;
					std::cerr << "new best\n";
					std::cerr << "gene " << c.gene 
						<< " score " << c.score << "\n";
				}
			}
		}
	}
};

