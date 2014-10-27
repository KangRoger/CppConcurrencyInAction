#include<iostream>
#include<mutex>
#include<condition_variable>
#include<queue>
#include<thread>
#include<unistd.h>
std::mutex mut;
std::queue<int> q;
std::condition_variable cond;

void data_preparation_thread()
{
	while (true)
	{
		//休眠一下
		sleep(1);//1s
		int data = rand() % 20;
		std::lock_guard<std::mutex> lk(mut);
		q.push(data);
		std::cout << "Preparation Data:" << data << std::endl;
		cond.notify_one();//唤醒一个，notify_any()唤醒所有
	}
}

void data_processing_thread()
{
	while (true)
	{
		sleep(2);//2s
		std::unique_lock<std::mutex> lk(mut);
		/*
		wait先检查lambda表达式，如果满足条件则返回。否则就等待，此时unlock mut然后线程阻塞。
		当线程被唤醒，会重新lock mut，并检查lambda表达式。如果lambda不满足，那么unlock mut继续等待。
		所以上面要有unique_lock
		*/
		cond.wait(lk, []{return !q.empty(); });//lambda function，等待的条件[]{return !q.empty(); }
		int data = q.front();
		q.pop();
		lk.unlock();
		std::cout << "Processing Data:" << data << std::endl;
	}
}
int main()
{
	std::thread t1(data_preparation_thread);
	std::thread t2(data_processing_thread);
	t1.join();
	t2.join();
	return 0;
}