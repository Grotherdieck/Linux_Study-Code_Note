.PHONY:all
all:tcp_client tcp_sever
tcp_client:tcp_client.cc 
	g++ -o $@ $^ -std=c++11 -static
tcp_sever:tcp_sever.cc 
	g++ -o $@ $^ -std=c++11
.PHONY:clean
clean:
	rm -rf tcp_client tcp_sever 
