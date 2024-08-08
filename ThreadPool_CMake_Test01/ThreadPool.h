#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <functional>

// 定义一个简单的线程池类
class ThreadPool {
public:
    ThreadPool(size_t numThreads);  // 构造函数，创建指定数量的工作线程
    ~ThreadPool();                  // 析构函数，等待所有线程完成

    // 向任务队列中添加任务
    void enqueueTask(const std::function<void()>& task);

private:
    // 工作线程从任务队列中取出任务并执行
    void workerThread();

    std::vector<std::thread> workers;           // 工作线程的容器
    std::queue<std::function<void()>> taskQueue; // 任务队列

    std::mutex mtx;                // 用于保护任务队列的互斥锁
    std::condition_variable mCon;  // 用于通知工作线程有新任务的条件变量
    bool stop;                     // 线程池是否停止工作
};

#endif
