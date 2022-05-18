libmymath.a:add.o sub.o
	ar -rc $@ $^
%.o:%.c
	gcc -c $<
.PHONY:clean
clean:
	rm -rf *.o libmymath.a
.PHONY:output
output:
	mkdir output 
	cp *.h ./output/
	cp libmymath.a ./output/
.PHONY:install
install:
	cp *.h /usr/include/
	cp libmymath.a /usr/lib64/
