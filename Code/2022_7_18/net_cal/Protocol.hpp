#pragma once
#include <iostream>
#include <string>

using namespace std;

// 定制协议的过程 就是定制结构化数据的过程

// 请求格式
typedef struct request
{
    int _x;
    int _y;
    int _op;// +-/*%
}request_t;

// 相应格式
typedef struct response
{
    int code;// sever运算完毕的计算状态 code(0):success, code(-1):div 0 
    int result;// 计算结果
}response_t;

