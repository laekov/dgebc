#include <ctime>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <unistd.h>

#include <civetweb.h>

#include <scheduler.hh>

#include <task.hh>
#include <msgque.hh>
#include <../../engine/engine.h>

namespace DGEBC {
	char *server_addr, *server_port;
	char *worker_port;

	using std::pair;
	using std::vector;
	using std::set;
	using std::string;
	using std::stringstream;

	vector<pair<string, string> > fellows;

	inline double randf() {
		static const int rng = 12345677;
		return (rand() % rng) / (double)rng;
	}

	void setServer(char* addr, char* port) {
		server_addr = addr;
		server_port = port;
	}

	void setWorkerPort(char* port) {
		worker_port = port;
	}

	void* registerWorker(void*) {
		struct mg_connection *conn;
		char buf[4096];
		while (1) {
			conn = mg_download(server_addr, atoi(server_port), 0, buf, 
					sizeof(buf),
					"GET /register_worker?port=%s HTTP/1.0\r\n"
					"Host: %s\r\n\r\n",
					worker_port, server_addr);
			mg_close_connection(conn);
			sleep(1);
			break;
		}
	}

	inline std::ostream& dgebc_log() {
		time_t t(time(0));
		char* tstr(ctime(&t));
		tstr[strlen(tstr) - 1] = 0;
		return std::cerr << tstr << "\t";
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
		}
		dgebc_log() << "Working with " << fellows.size() << " fellows\n";
	}

	MsgQue<Task> spread_que;

	void dumpGene(Task t) {
		struct mg_connection *conn;
		char buf[4096];
		conn = mg_download(server_addr, atoi(server_port), 0, buf, sizeof(buf),
			"GET /dump?gene=%s&score=%.9lf&port=%s HTTP/1.0\r\n"
			"Host: %s\r\n\r\n", 
			t.gene.c_str(), t.score, worker_port, server_addr);
		mg_close_connection(conn);
	}

	void spreadGene(Task t) {
		char buf[4096];
		struct mg_connection *conn;
		for (auto i: fellows) {
			conn = mg_connect_client(i.first.c_str(), atoi(i.second.c_str()),
					                 0, buf, sizeof(buf));
			sprintf(buf, "gene=%s&score=%.9lf", t.gene.c_str(), t.score);
			mg_printf(conn, "POST /spread_gene HTTP/1.1\r\n"
					"Host: %s\r\n"
					"Content-Type: application/x-www-form-urlencoded\r\n"
					"Content-Length: %d\r\n"
					"\r\n"
					"%s", i.first.c_str(), strlen(buf), buf);
			mg_close_connection(conn);
		}
		if (rand() & 1) {
			dumpGene(t);
		}
	}

	void* getListAndSpread(void*) {
		while (1) {
			updateClientList();
			for (int i = 0; i < 10; ++ i) {
				Task t;
				if ((t = spread_que.de()).score > -1) {
					spreadGene(t);
				} else {
					break;
				}
			}
			sleep(1);
		}
	}

	int throughput;

	void* daemonMain(void* args) {
		MsgQue<Task> *task_q = static_cast<MsgQue<Task>*>(args);
		MsgQue<Task> *res_q = static_cast<MsgQue<Task>*>(args) + 1;
		int c_sz_report(0);
		int last_sz(res_q->tot());
		pthread_t update_handle;
		pthread_create(&update_handle, 0, getListAndSpread, 0);
		pthread_t register_handle;
		pthread_create(&register_handle, 0, registerWorker, 0);
		while (1) {
			if (!c_sz_report) {
				int new_sz(res_q->tot());
				dgebc_log() << "queue size: " << task_q->sz()
					<< "\ttotal tasks: " << task_q->tot() 
					<< "\tTPS: " << (throughput = new_sz - last_sz) << "\n";
				last_sz = new_sz;
			}
			sleep(1);
		}
	}

	double current_max(.1);

	void* schedulerMain(void* args) {
		MsgQue<Task> *task_q = static_cast<MsgQue<Task>*>(args);
		MsgQue<Task> *res_q = static_cast<MsgQue<Task>*>(args) + 1;

		DGEBC::Engine engine;

		Task init_t;
		init_t.gene = engine.initial();
		init_t.score = -1;
		task_q->en(init_t);

		// TODO(laekov): make it variable under server's control
		static const int group_size = 1003;
		static const int mutate_const = 10;
		static const int combine_const = 10;
		static const int max_iter = 100;
		static const double spread_ratio = 0.7;

		vector<Task> survivor;
		for (int cnt = 0; ; ++ cnt) {
			if (cnt % 97 == 96) {
				for (auto it: survivor) {
					if (it.score > current_max * spread_ratio) {
						spread_que.en(it);
					}
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
					d.score = c.score * randf() * 2;
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
						e.score = (c.score + e.score)* randf();
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
					if (survivor[k].score < c.score) {
						sel_score = 0;
					}
					if (rand() % (sel_score + c_score) < c_score) {
						survivor[k] = c;
					}
				}

				if (c.score > current_max) {
					current_max = c.score;
					dgebc_log() << "Found new best" 
						<< " score " << c.score << "\n";
					dumpGene(c);
				}
			}
		}
	}
};

