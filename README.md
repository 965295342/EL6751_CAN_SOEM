1.EL6751是一个Beckhoff的can板卡，连接在EK1100主站下，可以实现在Linux实时系统下的can message的收发。

2，SOEM下的/test/linux/simple_test下的main.c文件是我此次上传的功能的入口，主要功能是通过读取csv文件的配置，完成can收发和将收发的数据通过udp发送出去（自己有别的用处）。

3.如何产生csv文件呢，可以有twincat产生，如有需要可以评论或者私信我，我会发上来，或者自己使用Setting下的文件夹内的matlab的模块产生，当然也有别的方法产生，需要可以私信我。
