#include<thread>
#include<mutex>
#include<unistd.h>
class Test
{
private:
	std::mutex m1;
	std::mutex m2;
public:
	void fun1()
	{
		std::lock_guard<std::mutex> guard1(m1);
		//休眠,使死锁更容易发生
		sleep(1);
		std::lock_guard<std::mutex> guard2(m2);
	}
	void fun2()
	{
		std::lock_guard<std::mutex> guard1(m2);
		//休眠,使死锁更容易发生
		sleep(1);
		std::lock_guard<std::mutex> guard2(m1);
	}

};
void fun1(Test *p)
{
	p->fun1();
}
void fun2(Test *p)
{
	p->fun2();
}
int main()
{
	Test t;
	std::thread A(fun1, &t);
	std::thread B(fun2, &t);
	A.join();
	B.join();
	return 0;
}