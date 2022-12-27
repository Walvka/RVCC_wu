#!/bin/bash

# 创建一个临时文件夹，XXXXXX会被替换为随机字符串, -d为创建文件路径
tmp=`mktemp -d /tmp/rvcc-test-XXXXXX`
# 清理工作
# 在接收到 中断（ctrl+c），终止，挂起（ssh掉线，用户退出），退出 信号时
# 执行rm命令，删除掉新建的临时文件夹
trap 'rm -rf $tmp' INT TERM HUP EXIT
# 在临时文件夹内，新建一个空文件，名为empty.c
echo > $tmp/empty.c

# 判断返回值是否为0来判断程序是否成功执行
check() {
  if [ $? -eq 0 ]; then #  $? -->显示最后命令的退出状态，0表示没有错误，其他值表示错误
    echo "testing $1 ... passed"
  else
    echo "testing $1 ... failed"
    exit 1
  fi
}

# -o
# 清理掉$tmp中的out文件
rm -f $tmp/out
# 编译生成out文件
./rvcc -o $tmp/out $tmp/empty.c
# 条件判断，是否存在out文件, [ -f $filename]，检测文件是否是普通文件(既不是目录又不是设备文件)，是的话就返回true
[ -f $tmp/out ]
# 将-o传入check函数
check -o

# --help
# 将--help的结果传入到grep进行 行过滤
# -q不输出，是否匹配到存在rvcc字符串的行结果
# 2>&1 指的是shell的重定向，此句将stderr->stdout,再通过管道传递到grep中
# [1=>stdout, 2=>stderr, 0=>stdin]
./rvcc --help 2>&1 | grep -q rvcc
# 将--help传入check函数
check --help


#当前目录中的in.c是为了断点调试时有输入，in.c是个空文件，无实际意义。

echo OK
