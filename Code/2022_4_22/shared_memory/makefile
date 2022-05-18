.PHONY:all
all:client sever 

client:client.c
	gcc -o $@ $^
sever:sever.c
	gcc -o $@ $^

.PHONY:clean
clean:
	rm -rf client sever fifo
