#ifndef MSGQUE_HH
#define MSGQUE_HH

#include <queue>
#include <semaphore.h>
#include <pthread.h>

namespace DGEBC {
	/* MsgQue class:
	 * a simple thread-safe message queue
	 */
	template<class DataType>
	class MsgQue {
		private:
			std::queue<DataType> q;
			sem_t data_ready_sem;
			pthread_mutex_t editing_mtx;
		public:
			MsgQue() {
				sem_init(&data_ready_sem, 0, 0u);
				pthread_mutex_init(&editing_mtx, 0);
			}
			~MsgQue() {
				sem_destroy(&data_ready_sem);
				pthread_mutex_destroy(&editing_mtx);
			}
			inline DataType de() {
				DataType result;
				sem_wait(&data_ready_sem);
				result = q.front();
				pthread_mutex_lock(&editing_mtx);
				q.pop();
				pthread_mutex_unlock(&editing_mtx);
			}
			inline void en(const DataType& d) {
				pthread_mutex_lock(&editing_mtx);
				q.push(d);
				pthread_mutex_unlock(&editing_mtx);
			}
	};
};

#endif
