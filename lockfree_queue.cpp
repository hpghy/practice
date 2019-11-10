#include <iostream>
#include <memory>
#include <thread>
#include <string>
using namespace std;

class Message
{
public:
    char msg_char;
};

/**
 * a lockfree stack using a lockfree shared_ptr<>
 **/
template <typename T>
class lockfree_stack
{
public:
    struct node 
    {
        node(const T& data) : data(make_shared<T>(data))
        {}
        shared_ptr<T> data;
        shared_ptr<node> next;
    };
public:
    void push(const T& data)        // 接口不是push(shared_ptr) 避免外界引用里面的节点
    {
        shared_ptr<node> new_node = make_shared<node>(data);
        new_node->next = atomic_load(&head);   
        while (!atomic_compare_exchange_weak(&head, &new_node->next, new_node));
    }
    shared_ptr<T> pop()
    {
        shared_ptr<node> old_head = atomic_load(&head);
        while (old_head && !atomic_compare_exchange_weak(&head, &old_head, old_head->next));
        return old_head ? old_head->data : nullptr;
    }
private:
    shared_ptr<node> head;          // c++20可以使用atomic<shared_ptr<T>>
};

int main()
{
    shared_ptr<lockfree_stack<Message>::node> test_node;
    shared_ptr<lockfree_stack<int>::node> test_node2;
    cout << "shared_ptr<lockfree_stack<Message>::node> is lockfree ? " << atomic_is_lock_free(&test_node) << "\n";
    cout << "shared_ptr<lockfree_stack<int>::node> is lockfree ? " << atomic_is_lock_free(&test_node2) << "\n";
    return 0;
}
