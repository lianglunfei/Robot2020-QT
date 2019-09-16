*** Error in `/home/robot/src/build-RobotControlSystem-Desktop_Qt_5_12_0_GCC_64bit-Debug/RobotControlSystem': corrupted size vs. prev_size: 0x00007f6338005510 ***
======= Backtrace: =========
/lib/x86_64-linux-gnu/libc.so.6(+0x777e5)[0x7f63681517e5]
/lib/x86_64-linux-gnu/libc.so.6(+0x80d36)[0x7f636815ad36]
/lib/x86_64-linux-gnu/libc.so.6(cfree+0x4c)[0x7f636815e53c]

some solutions:
1. addr2line.exe -f -C -e RobotControlSystem.exe 0040187C 004017AB 004016BC 00403B45
2. objdump -S RobotControlSystem.exe > RobotControlSystem.asm，根据dmp文件查看具体的出错地址，再在asm文件中寻找相对应的函数名字
3. 使用windbg加载pdb、exe、dmp文件分析具体位置。
4. 生成map文件。
    1. 修改pro如下： 
        #加入调试信息
        QMAKE_CFLAGS_RELEASE += -g
        QMAKE_CXXFLAGS_RELEASE += -g
        #禁止优化
        QMAKE_CFLAGS_RELEASE -= -O2
        QMAKE_CXXFLAGS_RELEASE -= -O2
        #release在最后link时默认有"-s”参数，表示"Omit all symbol information from the output file"，因此要去掉该参数
        QMAKE_LFLAGS_RELEASE = -mthreads -Wl,-Map,RobotControlSystem.map