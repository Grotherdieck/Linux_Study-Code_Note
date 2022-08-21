.PHONY:all
all:tcp_client tcp_sever tcp_sever_multhread tcp_sever_ThreadPool
tcp_client:tcp_client.cc 
	g++ -o $@ $^ -std=c++11 -static
tcp_sever:tcp_sever.cc 
	g++ -o $@ $^ -std=c++11
tcp_sever_multhread:tcp_sever_multhread.cc
	g++ -o $@ $^ -std=c++11 -lpthread
tcp_sever_ThreadPool:tcp_sever_ThreadPool.cc
	g++ -o $@ $^ -std=c++11 -lpthread
.PHONY:clean
clean:
	rm -rf tcp_client tcp_sever tcp_sever_multhread tcp_sever_ThreadPool
