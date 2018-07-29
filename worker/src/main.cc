#include <bits/stdc++.h>
#include <pthread.h>
#include <civetweb.h>

#include <task.hh>
#include <msgque.hh>

#include <proxy.hh>
#include <worker.hh>
#include <scheduler.hh>

#include <../../engine/engine.h>

int main(int argc, char* args[]) {
	{
		using namespace std;
		srand(time(0));
		// Test Engine
		cout << "========== Begin Engine Test ==========" << endl;
		DGEBC::Engine e;
		string gene = e.initial();
		cout << "initial gene: " << gene << endl;
		double score = e.score(gene);
		cout << "initial score: " << score << endl;
		string new_gene = e.mutate(e.mutate(e.mutate(gene)));
		cout << "mutated gene: " << gene << endl;
		double new_score = e.score(new_gene);
		cout << "mutated score: " << new_score << endl;
		string combined_gene = e.combine(gene, new_gene);
		cout << "combined gene: " << gene << endl;
		double combined_score = e.score(combined_gene);
		cout << "combined score: " << combined_score << endl;
		cout << "==========  End Engine Test  ==========" << endl;
	}
	
	char *server_addr = "localhost";
	char *server_port = "8080";
	char *port = "8080";
	int num_workers = 1;
	for (int i = 1; i < argc; ++ i) {
		if (!strcmp(args[i], "-p")) {
			port = args[++ i];
		} else if (!strcmp(args[i], "-w")) {
			num_workers = atoi(args[++ i]);
		} else if (!strcmp(args[i], "-h")) {
			server_addr = args[++ i];
		} else if (!strcmp(args[i], "-q")) {
			server_port = args[++ i];
		}
	}
    const char *options[] = {"listening_ports", port, NULL};
	struct mg_context *ctx;
	struct mg_callbacks callbacks;
	memset(&callbacks, 0, sizeof(callbacks));
    callbacks.begin_request = DGEBC::proxyHandlerWrapper;
	ctx = mg_start(&callbacks, 0, options);
	pthread_t* worker_handles = new pthread_t[num_workers];

	// queues: {todo_queue, result_queue}
	DGEBC::MsgQue<DGEBC::Task> queues[2];
	for (int i = 0; i < num_workers; ++ i) {
		pthread_create(worker_handles + i, 0, DGEBC::workerMain,
				       static_cast<void*>(queues));
	}

	// Optional: execute another pthread to run scheduler
	pthread_t proxy_handle;
	pthread_create(&proxy_handle, 0, DGEBC::proxyMain,
			       static_cast<void*>(queues));

	DGEBC::setServer(server_addr, server_port);
	DGEBC::setWorkerPort(port);

	pthread_t daemon_handle;
	pthread_create(&daemon_handle, 0, DGEBC::daemonMain,
			       static_cast<void*>(queues));

	DGEBC::schedulerMain(static_cast<void*>(queues));

	mg_stop(ctx);

	return 0;
}

