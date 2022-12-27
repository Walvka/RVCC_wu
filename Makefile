
CC = clang

CFLAGS = -std=c11 -g -fno-common

# C源代码文件，表示所有的.c结尾的文件
SRCS=$(wildcard *.c)
# C文件编译生成的未链接的可重定位文件，将所有.c文件替换为同名的.o结尾的文件名
OBJS=$(SRCS:.c=.o)

# rvcc标签，表示如何构建最终的二进制文件，依赖于所有的.o文件
# $@表示目标文件，此处为rvcc，$^表示依赖文件，此处为$(OBJS)
rvcc: $(OBJS)
# 将多个*.o文件编译为rvcc
	$(CC) $(CFLAGS) -o $@ $^

# 所有的可重定位文件依赖于rvcc.h的头文件
$(OBJS): rvcc.h

test: rvcc
	chmod 777 test.sh
	chmod 777 test-driver.sh
	./test.sh 
	./test-driver.sh

clean:
	rm -rf rvcc *.o *.s tmp* a.out

.PHONY: test clean