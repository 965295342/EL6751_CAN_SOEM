
#include "sendUdp.h"
// float float_data_toBe_display[256];	  // float data sent to Linux-simulini to display
// uint8_t uint8_data_toBe_display[128]; // uint8 data sent to Linux-simulini to display
// typedef struct udp_struct
// {
// 	int udp_socket_fd;
// 	struct sockaddr_in local_addr;
// 	struct sockaddr_in dest_addr;
// } udp_struct;
uint8_t init_udp(int port, udp_struct *udp)
{
	//  1.创建udp通信socket
	udp->udp_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp->udp_socket_fd < 0)
	{
		perror("creat socket fail\n");
		return 0;
	}
	// 2.设置UDP的地址并绑定
	udp->local_addr.sin_family = AF_INET; //使用IPv4协议
	// local_addr.sin_port = htons(port);		 //网络通信都使用大端格式
	udp->local_addr.sin_addr.s_addr = INADDR_ANY; //让系统检测本地网卡，自动绑定本地IP

	//设置目的IP地址
	udp->dest_addr.sin_family = AF_INET; //使用IPv4协议

	int dest_port = port;								 //目的端口号
	char dest_ip[32] = "127.0.0.1";						 //目的IP
	udp->dest_addr.sin_port = htons(dest_port);			 //设置接收方端口号
	udp->dest_addr.sin_addr.s_addr = inet_addr(dest_ip); //设置接收方IP
	return 1;
}

// time_t getMillisecondTime() //获得系统毫秒时间
// {
// 	time_t nowTime;
// 	struct timeb tTimeB;
// 	ftime(&tTimeB);
// 	nowTime = tTimeB.time * 1000;
// 	nowTime += tTimeB.millitm;
// 	return nowTime;
// }

int sendUdpMsg(float msg[], int length, udp_struct *udp)
{
	return sendto(udp->udp_socket_fd, msg, length, 0, (struct sockaddr *)&udp->dest_addr, sizeof(udp->dest_addr));
}