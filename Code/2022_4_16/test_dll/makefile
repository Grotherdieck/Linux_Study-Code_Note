# -shared 形成一个动态链接的共享库
libmymath.so:add.o sub.o
	gcc -shared -o $@ $^
# -fPIC 程序加载时与位置无关 采用相对位置寻址 这样不会占用其他运行的程序的地址空间
%.o:%.c 
	gcc -fPIC -c $<
.PHONY:clean
clean:
	rm -rf libmymath.so
# 发布
dll:
	mkdir dll
	cp *.h dll
	cp *.so dll