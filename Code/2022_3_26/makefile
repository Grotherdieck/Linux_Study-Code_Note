.PHONY:all
all:myload myexe 
myload:myload.c
	gcc -o $@ $^ -std=c99
myexe:myexe.c
	gcc -o $@ $^ -std=c99
.PHONY:clean
clean:
	rm -rf myload myexe
