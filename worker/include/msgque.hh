#ifndef MSGQUE_HH
#define MSGQUE_HH

#include <queue>
#include <semaphore.h>
#include <pthread.h>
#include <assert.h>

namespace DGEBC {
	/* MsgQue class:
	 * a simple thread-safe message queue
	 */
	template<class DataType>
	class MsgQue {
		private:
			std::priority_queue<DataType> q;
			sem_t data_ready_sem;
			pthread_mutex_t editing_mtx;
			int tot_cnt;
		public:
			MsgQue(): tot_cnt(0) {
				sem_init(&data_ready_sem, 0, 0u);
				pthread_mutex_init(&editing_mtx, 0);
			}
			~MsgQue() {
				sem_destroy(&data_ready_sem);
				pthread_mutex_destroy(&editing_mtx);
			}
			inline int tot() {
				return tot_cnt;
			}
			inline int sz() {
				return q.size();
			}
			inline DataType de() {
				DataType result;
				sem_wait(&data_ready_sem);
				pthread_mutex_lock(&editing_mtx);
				result = q.top();
				q.pop();
				pthread_mutex_unlock(&editing_mtx);
				return result;
			}
			inline void en(const DataType& d) {
				pthread_mutex_lock(&editing_mtx);
				++ tot_cnt;
				q.push(d);
				pthread_mutex_unlock(&editing_mtx);
				sem_post(&data_ready_sem);
			}
	};
};

#endif
