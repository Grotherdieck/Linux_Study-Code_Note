#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>

using namespace std;

// �����ʽ �������л��������
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
    // Json::Value��һ��KVʽ���ܹ����ض���Ķ���
    // �ȰѴ����л������ݳ��ص�root��
    root["datax"] = req._x;
    root["datay"] = req._y;
    root["operator"] = req._op;
    // Writer��һ������� ���������� FastWriter��StyledWriter
    //Json::StyledWriter writer;
    Json::FastWriter writer;
    std::string jsonstring = writer.write(root);
    std::cout << jsonstring << std::endl;

    // �����л� ��һ���ַ���ת��Ϊ�ַ���
    // R"()"��ʾ��ͷ��һ��ԭ���ַ���,ʡȥ��ת��
    std::string JsonString = R"({"datax":10,"datay":20,"operator":43})";
    Json::Value Root;
    Json::Reader reader;
    reader.parse(jsonstring, Root);// ��������л����ݳ��Root
    // Ȼ����KV����mapһ��ȥ�á����֡���������
    request_t ans;
    // ��������ȥ��
    ans._x = Root["datax"].asInt();
    ans._y = Root["datay"].asInt();
    //ans._op = Root["operator"].asInt();
    // �����char
    ans._op = (char)Root["operator"].asUInt();
    cout << "ans._x = " << ans._x << ", ans._y = " << ans._y << ", ans._op = " << ans._op << endl;
    return 0;
}
