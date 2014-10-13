#include<thread>
#include<mutex>
#include<iostream>
class Test
{
private:
	std::mutex m1;
public:
	void fun1()
	{
		std::lock_guard<std::mutex> guard1(m1);
		fun2();
		
	}
	void fun2()
	{
		std::lock_guard<std::mutex> guard1(m1);
	}

};
void fun(Test *p)
{
	p->fun1();
	std::cout << "fun1" << std::endl;
}

int main()
{
	Test t;
	std::thread A(fun, &t);
	A.join();
	return 0;
}