#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>

using namespace std;

// 请求格式 测试序列化这个东西
typedef struct request
{
    int _x;
    int _y;
    char _op;// +-/*%
}request_t;

int main()
{
    request_t req = {10, 20, '+'};
    Json::Value root;
    // Json::Value是一个KV式的能够承载对象的东西
    // 先把待序列化的数据承载到root中
    root["datax"] = req._x;
    root["datay"] = req._y;
    root["operator"] = req._op;
    // Writer是一个虚基类 有两个子类 FastWriter和StyledWriter
    //Json::StyledWriter writer;
    Json::FastWriter writer;
    std::string jsonstring = writer.write(root);
    std::cout << jsonstring << std::endl;

    // 反序列化 把一个字符串转化为字符串
    // R"()"表示里头是一个原生字符串,省去了转义
    std::string JsonString = R"({"datax":10,"datay":20,"operator":43})";
    Json::Value Root;
    Json::Reader reader;
    reader.parse(jsonstring, Root);// 让这个序列化数据充斥Root
    // 然后用KV类似map一样去用“名字”拿它即可
    request_t ans;
    // 当做整数去拿
    ans._x = Root["datax"].asInt();
    ans._y = Root["datay"].asInt();
    //ans._op = Root["operator"].asInt();
    // 如果是char
    ans._op = (char)Root["operator"].asUInt();
    cout << "ans._x = " << ans._x << ", ans._y = " << ans._y << ", ans._op = " << ans._op << endl;
    return 0;
}
