# 样例框架说明

本框架为2024年秋季《形式语言与自动机》课程的样例框架。助教们将使用该框架对同学们的程序进行测试，因此请确保你的工程可以使用该框架编译。

## 软件依赖

本框架依赖于cmake进行编译，需安装3.13.0以上版本的cmake方可使用。

## 使用方法

将文件打包为如下结构：

```plain
学号_姓名_FLA24proj.zip
    |- CMakeLists.txt
    |- README.md
    |- fla-project
        |- main.cpp
        |- ...
    |- pda
        |- case.pda
    |- tm
        |- case1.tm
        |- case2.tm
```

而非：

```plain
学号_姓名_FLA24proj.zip
    |- 学号_姓名_FLA24proj
        |- CMakeLists.txt
        |- README.md
        |- fla-project
            |- main.cpp
            |- xxx
        |- pda
            |- case.pda
        |- tm
            |- case1.tm
            |- case2.tm
```

本框架的使用方法为：

1. 将所有`.h` 以及`.cpp`文件置于`/fla-project`文件夹下，或者将该框架的`CMakeLists.txt`复制至你的工程根目录
2. 在工程根目录处，使用指令`cmake -B build`
3. 在工程根目录处，使用指令`cd ./build; make`

测试框架将使用上述方法对工程进行编译。执行完毕后，在工程根目录下应当出现`/bin`文件夹，其中含有可执行文件`fla`，测试框架将对该可执行文件进行测试。

**重点**：提交时，你的工程根目录下应当有一个`CMakeLists.txt`文件用于指导cmake进行编译，且通过这种方式能够在`/bin`文件夹下生成可执行文件`fla`。

## 编译测试

将工程根目录直接打包为一个`.zip`文件
Windows用户在命令行中输入：`curl.exe -X POST -F "file=@学号_姓名_FLA24proj.zip" http://114.212.84.155:5000/upload`
MacOS/Linux用户在命令行中输入：`curl -X POST -F "file=@学号_姓名_FLA24proj.zip" http://114.212.84.155:5000/upload`
以获取编译结果。**该页面仅用于测试工程编译可行性，不是正式提交入口！正式提交方式以讲义为准！**
