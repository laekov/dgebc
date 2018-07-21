#include <bits/stdc++.h>
#include <pthread.h>
#include <civetweb.h>

#include <task.hh>
#include <msgque.hh>

#include <worker.hh>

int main(int argc, char* args[]) {
	char *port = "8080";
	int num_workers = 1;
	for (int i = 1; i < argc; ++ i) {
		if (!strcmp(args[i], "-p")) {
			port = args[++ i];
		} else if (!strcmp(args[i], "-w")) {
			num_workers = atoi(args[++ i]);
		}
	}
    const char *options[] = {"listening_ports", port, NULL};
	struct mg_context *ctx;
	struct mg_callbacks callbacks;
	memset(&callbacks, 0, sizeof(callbacks));
	ctx = mg_start(&callbacks, 0, options);
	pthread_t* worker_handles = new pthread_t[num_workers];
	// queues: {todo_queue, result_queue}
	DGEBC::MsgQue<DGEBC::Task> queues[2];
	for (int i = 0; i < num_workers; ++ i) {
		pthread_create(worker_handles + i, 0, DGEBC::workerMain,
				       static_cast<void*>(queues));
	}
	for (int i = 0; i < num_workers; ++ i) {
		pthread_join(worker_handles[i], 0);
	}
	mg_stop(ctx);
	return 0;
}
