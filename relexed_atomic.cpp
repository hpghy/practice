#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
using namespace std;

/**
 * 测试memory order影响的不是atomic而是多线程下其他关联变量的可见性
 **/ 

atomic<int> result;

int main()
{
    vector<thread> thread_vec; 
    for (int i = 0; i < 100; ++i)
    {
        thread_vec.emplace_back([]()
            {
                for (int j = 0; j < 100000; ++j)
                {
                    result.fetch_add(1, memory_order_relaxed);
                }
            }) ;
    }
    cout << "wait children thread over\n";
    for (int i = 0; i < 100; ++i)
    {
        thread_vec[i].join();
    }
    cout << "result: " << result << "\n";
    return 0;
}
