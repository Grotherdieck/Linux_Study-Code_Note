mycode:mycode.o
	gcc -o $@ $^
mycode.o:mycode.s
	gcc -o $@ $^ -c
mycode.s:mycode.i
	gcc -o $@ $^ -S
mycode.i:mycode.c
	gcc -o $@ $^ -E
.PHONY:clean
clean:
	rm -rf mycode
