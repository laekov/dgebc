#ifndef PROXY_HH
#define PROXY_HH

namespace DGEBC {
	int proxyHandler(struct mg_connection*);
	static int proxyHandlerWrapper(struct mg_connection* conn) {
		return proxyHandler(conn);
	}
	void* proxyMain(void*);
};
#endif
