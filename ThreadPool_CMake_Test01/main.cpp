#include "ThreadPool.h"
#include <chrono>

// 示例任务：打印任务编号并休眠一段时间
void exampleTask(int taskNumber) {
    std::cout << "Task " << taskNumber << " is starting.\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Task " << taskNumber << " is completed.\n";
}

int main() {
    ThreadPool pool(4); // 创建包含4个工作线程的线程池

    // 向线程池中添加10个任务
    for (int i = 0; i < 10; ++i) {
        pool.enqueueTask([i] { exampleTask(i); });
    }

    // 等待所有任务完成
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "All tasks are completed.\n";

    return 0;
}
