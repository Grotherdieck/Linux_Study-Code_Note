#pragma once
#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>

using namespace std;

// 定制协议的过程 就是定制结构化数据的过程

// 请求格式
typedef struct request
{
    int _x;
    int _y;
    char _op;// +-/*%
}request_t;

// 相应格式
typedef struct response
{
    int code;// sever运算完毕的计算状态 code(0):success, code(-1):div 0 
    int result;// 计算结果
}response_t;

// 序列化请求
std::string SerializeRequest(const request_t& req)
{
    Json::Value root;
    root["datax"] = req._x;
    root["datay"] = req._y;
    root["operator"] = req._op;
    Json::FastWriter writer;
    return writer.write(root);
}

// 回应序列化
std::string SerializeResponse(const response_t& res)
{
    Json::Value root;
    root["code"] = res.code;
    root["result"] = res.result;
    return Json::FastWriter().write(root);
}

// 反序列化请求
void DeserializationReq(const string& jsonstring, request_t& req)
{
    Json::Reader reader;
    Json::Value root;
    reader.parse(jsonstring, root);
    req._x = root["datax"].asInt();
    req._y = root["datay"].asInt();
    req._op = (char)root["operator"].asUInt();
}

// 反序列化回应
void DeserializationRes(const string& jsonstring, response_t& res)
{
    Json::Value root;
    Json::Reader reader;
    reader.parse(jsonstring, root);
    res.code = root["code"].asInt();
    res.result = root["result"].asInt();
}

