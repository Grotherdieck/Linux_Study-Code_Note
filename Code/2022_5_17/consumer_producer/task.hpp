#pragma once
#include <iostream>
#include <pthread.h>
using namespace std;
    


namespace Ns_task
{
    class Task
    {
    private:
        int x_;
        int y_;
        char op_;// 操作 + - * / 
    public:
        Task() {}
        Task(int x, int y, char op) : x_(x), y_(y), op_(op) {}
        ~Task() {}
        int Run()
        {
            cout << "当前任务正在被" << pthread_self() << "处理," << x_ << ' ' << op_ << ' ' << y_ << "=";
            int res = 0;
            if (op_ == '+') res =  x_ + y_;
            else if (op_ == '-') res =  x_ - y_;
            else if (op_ == '*') res =  x_ * y_;
            else if (op_ == '/') res =  x_ / y_;
            else if (op_ == '%') res=  x_ % y_;
            else 
            {
                cout << "op error" << endl;
                return -1;
            }
            cout << res << endl;
            return res;
        }
        int operator()()
        {
            return Run();
        }
        string show()
        {
            string message = to_string(x_);
            message.push_back(' ');
            message += op_;
            message += to_string(y_);
            message += '=';
            message += '?';
            return message;
        }
    };
}