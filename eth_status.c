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
     ifr_flags ���U���X�ЩM�����G
         IFF_UP              �˸m���b�B�@
         IFF_BROADCAST       ���Ī��s����}
         IFF_DEBUG           Debug �лx
         IFF_LOOPBACK        �o�O Loopback �˸m
         IFF_POINTOPOINT     �o�O�I���I�������˸m����
         IFF_RUNNING         �귽�w���t
         IFF_NOARP           �Larp��ĳ�A�S���]�m�ĤG�h�ت��a�}
         IFF_PROMISC         ����������(promiscuous)�Ҧ�
         IFF_NOTRAILERS      �קK�ϥ� trailer
         IFF_ALLMULTI        �����Ҧ��s�ռs��(multicast)�ʥ]���
         IFF_MASTER          �D�t�����Ÿs(bundle)
         IFF_SLAVE           �q�t�����Ÿs(bundle)
         IFF_MULTICAST       ����s�ռs��(multicast)
         IFF_PORTSEL         �i�H�q�L ifmap ��� media ����
         IFF_AUTOMEDIA       �۰ʿ�� media
         IFF_DYNAMIC         �˸m���������ɥ��a�}
 */
 if (ifr.ifr_flags & IFF_MASTER) {
     printf("eth0 is master!");
 }
 printf("eth0 is %x\n!",ifr.ifr_flags);
 close(sockfd);
}