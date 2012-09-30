#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>


void main(void)
{
 struct ifreq ifr;
 int sockfd;

 sockfd = socket(AF_INET, SOCK_DGRAM, 0);
 bzero(&ifr, sizeof(ifr));
 strcpy(ifr.ifr_name, "eth0");

 ioctl(sockfd, SIOCGIFFLAGS, &ifr);
 /*
     ifr_flags 的各項旗標和說明：
         IFF_UP              裝置正在運作
         IFF_BROADCAST       有效的廣播位址
         IFF_DEBUG           Debug 標誌
         IFF_LOOPBACK        這是 Loopback 裝置
         IFF_POINTOPOINT     這是點到點的網路裝置介面
         IFF_RUNNING         資源已分配
         IFF_NOARP           無arp協議，沒有設置第二層目的地址
         IFF_PROMISC         介面為雜湊(promiscuous)模式
         IFF_NOTRAILERS      避免使用 trailer
         IFF_ALLMULTI        接收所有群組廣播(multicast)封包資料
         IFF_MASTER          主負載平衡群(bundle)
         IFF_SLAVE           從負載平衡群(bundle)
         IFF_MULTICAST       支持群組廣播(multicast)
         IFF_PORTSEL         可以通過 ifmap 選擇 media 類型
         IFF_AUTOMEDIA       自動選擇 media
         IFF_DYNAMIC         裝置介面關閉時丟棄地址
 */
 if (ifr.ifr_flags & IFF_MASTER) {
     printf("eth0 is master!");
 }
 printf("eth0 is %x\n!",ifr.ifr_flags);
 close(sockfd);
}