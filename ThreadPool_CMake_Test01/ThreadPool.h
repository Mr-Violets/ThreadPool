#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <functional>

// ����һ���򵥵��̳߳���
class ThreadPool {
public:
    ThreadPool(size_t numThreads);  // ���캯��������ָ�������Ĺ����߳�
    ~ThreadPool();                  // �����������ȴ������߳����

    // ������������������
    void enqueueTask(const std::function<void()>& task);

private:
    // �����̴߳����������ȡ������ִ��
    void workerThread();

    std::vector<std::thread> workers;           // �����̵߳�����
    std::queue<std::function<void()>> taskQueue; // �������

    std::mutex mtx;                // ���ڱ���������еĻ�����
    std::condition_variable mCon;  // ����֪ͨ�����߳������������������
    bool stop;                     // �̳߳��Ƿ�ֹͣ����
};

#endif
