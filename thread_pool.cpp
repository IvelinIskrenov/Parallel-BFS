#include "thread_pool.h"


bool ThreadPool::is_busy()
{
    bool pool_busy;
    {
        unique_lock<mutex> lock(queue_mutex);
        pool_busy = !jobs.empty();
    }
    return pool_busy;
}


void ThreadPool::wait_work()
{
    while (is_busy())
    {}
}


ThreadPool::ThreadPool(int number_of_workers)
{
    this->terminating = false;
    this->number_of_workers = number_of_workers;
}

void ThreadPool::start()
{
    for (int64_t i = 0; i < number_of_workers; i++)
    {
        workers.push_back(thread(&ThreadPool::thread_loop, this, i));
    }
};

void ThreadPool::stop()
{
    unique_lock<mutex> lock(queue_mutex);
    this->terminating = true;
    lock.unlock();
    queue_cv.notify_all();
    for (auto& worker: workers)
	{
		if (worker.joinable())
		{
			worker.join();
		}
	}
    workers.clear();
}


void ThreadPool::thread_loop(int thread_number)
{
    while(true)
    {
        unique_lock<mutex> lock(queue_mutex);
        queue_cv.wait(lock, [this] {return this->terminating || !this->jobs.empty();});

        if (terminating == true)
        {
            return;
        }
        BFSTask bfs = jobs.front();
        jobs.pop();
        queue_cv.notify_one();
        lock.unlock();
        if (terminating == true)
        {
            return;
        }
        bfs.fill(thread_number + 1);
    }
}

bool ThreadPool::push_job(const BFSTask& job)
{
	{
		unique_lock<mutex> lock(queue_mutex);
		jobs.push(job);
	}
    queue_cv.notify_one();
    return true;
}