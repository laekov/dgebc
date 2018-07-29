#ifndef SCHEDULER_HH
#define SCHEDULRE_HH
namespace DGEBC {
	void* schedulerMain(void*);
	void* daemonMain(void*);
	void setServer(char*, char*);
	void setWorkerPort(char*);
};
#endif
