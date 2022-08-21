.PHONY:all
all:CalSever CalClient

CalClient:CalClient.cc 
	g++ -o $@ $^ -std=c++11 -lpthread
CalSever:CalSever.cc 
	g++ -o $@ $^ -std=c++11 -lpthread
.PHONY:clean
clean:
	rm -rf CalSever CalClient