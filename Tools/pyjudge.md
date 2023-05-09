# pyjudge

> 简单的算法竞赛评测脚本

## 使用方法与例子

```shell
> alias pyjudge="python pyjudge.py"
> pyjudge --help
> pyjudge [action] --help
```

可用的`action`：`judge,run,hack`。

### judge

评测`A.cpp`，测试目录指定为`test`，时限设定为`1.5`

```shell
> pyjudge judge A.cpp --dir test --tle 1.5
```

脚本将重定向测试代码的标准输入为`test`目录下的`.in`文件，之后将标准输出（忽略标准错误）与同目录下的`.out`文件进行对比。

如果未指定`dir`，那么默认为`data`，如果未指定`tle`，那么默认为`1s`

```shell
> pyjudge judge A.cpp
```

### run

运行`A.cpp`

```shell
> pyjudge run A.cpp --dir test --tle 1.5
```

运行`A.cpp`并保存输出到`.out`文件作为答案

```shell
> pyjudge run A.cpp --dir test --tle 1.5 --save
```

### hack

指定`gen.py`为数据生成器，`std.cpp`为标程，`tset.cpp`为测试代码，暴力搜索一组hack数据。

```shell
> pyjudge hack --std std.cpp --gen gen.py test.cpp --tle 2 --dir test
```

hack数据将被保存到当前目录下`test`文件夹的`hack_x.in`。

## 编译与运行参数

支持 `C/C++,python`。

- C++ 编译器 `'g++'`
- C++ 编译参数 `'-std=c++20','-O2','-DONLINE_JUDGE'`
- C 编译器 `'gcc'`
- C 编译参数 `'-std=c11','-O2','-DONLINE_JUDGE'`
- python 解释器 `'python'`