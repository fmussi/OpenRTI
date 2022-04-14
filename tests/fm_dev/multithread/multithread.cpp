// multithrea test code
#include <iostream>
#include <thread>


using namespace std;

//dummy function
void foo(int Z)
{
    for (int i = 0; i < Z; i++) {
        cout << "Thread using function"
                "pointer as callable\n";
    }
}

// callable object
class thread_obj {
public:
    void operator()(int x)
    {
        for (int i = 0; i < x; i++) {
        cout << "Thread using function"
                "object as callable\n";
        }
    }
};

int main()
{
    cout << "Thread 1 and 2 and 3 operating independently" << endl;

    // using function pointer
    thread th1(foo,3);
    // using function obj
    thread th2(thread_obj(),3);

    // define lambda
    auto f = [](int x){
        for (int i = 0; i < x; i++)
        {
             cout << "Thread using lambda"
             " expression as callable\n";
        }
    };

    thread th3(f,3);

    // wait for all thread to finisch
    th1.join();
    th2.join();
    th3.join();

    return 0;
}



