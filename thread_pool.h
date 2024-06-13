#pragma once
#include<thread>
#include<mutex>
#include<condition_variable>
#include<queue>
#include<atomic>
#include "bfs_task.h"
class ThreadPool
{
private:
    unsigned int number_of_workers;
    vector<thread> workers;
    queue<BFSTask> jobs;
    mutex queue_mutex;
    condition_variable queue_cv;
    bool terminating;

    void thread_loop(const int index);

public:
    ThreadPool(int number_of_workers);
    void start();
    void stop();
    bool push_job(const BFSTask& job);
    bool is_busy();
    void wait_work();
};