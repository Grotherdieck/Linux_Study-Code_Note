#pragma once
#include <iostream>
#include <string>

using namespace std;

// ����Э��Ĺ��� ���Ƕ��ƽṹ�����ݵĹ���

// �����ʽ
typedef struct request
{
    int _x;
    int _y;
    int _op;// +-/*%
}request_t;

// ��Ӧ��ʽ
typedef struct response
{
    int code;// sever������ϵļ���״̬ code(0):success, code(-1):div 0 
    int result;// ������
}response_t;

