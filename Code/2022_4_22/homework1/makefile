.PHONY:all
all:sever client
sever:sever.c
	gcc -o $@ $^
client:client.c
	gcc -o $@ $^
.PHONY:clean
	rm -rf sever client myfifo
