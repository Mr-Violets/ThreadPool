#include "ThreadPool.h"

// 构造函数：创建指定数量的工作线程
ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&ThreadPool::workerThread, this);
    }
}

// 析构函数：通知所有线程停止工作，并等待它们完成
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

// 向任务队列中添加任务
void ThreadPool::enqueueTask(const std::function<void()>& task) {
    {
        std::unique_lock<std::mutex> lck(mtx);
        taskQueue.push(task);
    }
    mCon.notify_one();
}

// 工作线程：从任务队列中取出任务并执行
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
        task(); // 执行任务
    }
}
