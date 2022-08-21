.PHONY:all
all:CalSever CalClient

CalClient:CalClient.cc 
	g++ -o $@ $^ -std=c++11 -lpthread -ljsoncpp
CalSever:CalSever.cc 
	g++ -o $@ $^ -std=c++11 -lpthread -ljsoncpp
.PHONY:clean
clean:
	rm -rf CalSever CalClient