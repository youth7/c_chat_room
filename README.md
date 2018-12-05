# chat room write with C
一个运行于Win7的用于教学演示的C语言聊天室，编译环境是CodeBlocks 16.01+mingw。它的工作模型是阻塞IO+多线程（为了方便学生理解）。

# 编译环境和方法
## 编译器
CodeBlocks 16.01 + mingw gcc

## 编译步骤
* 编译前需要设置链接器，打开CodeBlocks，点击*setttings->compiler->linker settings*。然后在*Link Libraries*中点击*Add去*添加两个库文件，分别是libws2_32.a和libwsock32.a。（这些文件位于CodeBlocks的安装目录下，例如在我的机器上是D:\Program Files (x86)\CodeBlocks\MinGW\lib）
* 将项目中所有文件放在同一目录下，然后在CodeBlocks中打开所有.c文件和common.h文件
* 修改common.h文件中的变量*IP_OF_SERVER*，将其改为你的服务器的IP
* 点击*build*按钮分别编译所有的.c文件

# 使用方法
在服务端运行win32_server.exe，用户端运行win32_client.exe（用户用来输入数据）和win32_client_for_display.exe（用户用来显示数据）。
