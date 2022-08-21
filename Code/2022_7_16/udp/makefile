.PHONY:all
all:udp_sever udp_client

udp_client:udp_client.cc
	g++ -o $@ $^ -std=c++11
udp_sever:udp_sever.cc
	g++ -o $@ $^ -std=c++11
.PHONY:clean
clean:
	rm -rf udp_client udp_sever