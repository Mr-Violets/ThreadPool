#include "ThreadPool.h"

// ���캯��������ָ�������Ĺ����߳�
ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&ThreadPool::workerThread, this);
    }
}

// ����������֪ͨ�����߳�ֹͣ���������ȴ��������
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lck(mtx);
        stop = true;
    }
    mCon.notify_all();
    for (std::thread& worker : workers) {
        worker.join();
    }
}

// ������������������
void ThreadPool::enqueueTask(const std::function<void()>& task) {
    {
        std::unique_lock<std::mutex> lck(mtx);
        taskQueue.push(task);
    }
    mCon.notify_one();
}

// �����̣߳������������ȡ������ִ��
void ThreadPool::workerThread() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lck(mtx);
            mCon.wait(lck, [this] { return !taskQueue.empty() || stop; });
            if (stop && taskQueue.empty()) return;
            task = taskQueue.front();
            taskQueue.pop();
        }
        task(); // ִ������
    }
}
