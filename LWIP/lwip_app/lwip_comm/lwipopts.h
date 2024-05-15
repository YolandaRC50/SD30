/**
	LWIP�����ļ�,ͨ�����ļ�����ѡ��LWIP�Ĺ���,
	���ļ�����opt.h�ļ�,opt.h��LWIP"����"ʱԭװ���ļ�,��Adam���˱�д
	��lwipopt.h��������ST��,��������lwip�г�����Ҫ�Ķ��ĵط�
	�����ST�����е�����ѡ����ӽ�����,�������ļ���߶������˵�
	ѡ���Ժ���Ϊ׼,���ǹ��е�ѡ�������Ǹ��Ե�����Ϊ׼
*/

#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#ifndef TCPIP_THREAD_PRIO
#define TCPIP_THREAD_PRIO        (configMAX_PRIORITIES - 1)                //�ں��������ȼ�
#endif
//*****************************************************************************
//
// ---------- SNMP options ----------
//
////*****************************************************************************
//#define LWIP_SNMP                       0      ����//SNMPЭ������
//#if LWIP_SNMP
//#define SNMP_CONCURRENT_REQUESTS     1            //SNMP��ǰ������������
//#define SNMP_TRAP_DESTINATIONS       1            //SNMP trapĿ�ĵ�ַ��Ŀ
//#define SNMP_PRIVATE_MIB             1      ������ //SNMP ˽�нڵ���������
//#define SNMP_SAFE_REQUESTS           1            //���ظ�SNMP��ȫ����
//#endif


#define SYS_LIGHTWEIGHT_PROT    1
//NO_SYS==0:ʹ�ò���ϵͳ
#define NO_SYS                  0  //��ʹ��UCOS����ϵͳ



//#define MEM_LIBC_MALLOC                 0                //����LWIP�Դ��������䶯̬�ڴ�

//ʹ��4�ֽڶ���ģʽ
#define MEM_ALIGNMENT           4  

//MEM_SIZE:heap�ڴ�Ĵ�С,�����Ӧ�����д������ݷ��͵Ļ����ֵ������ô�һ�� 
#define MEM_SIZE                8*1024 //�ڴ�Ѵ�С

//#define MEMP_SANITY_CHECK               0                //mem_free���ú��������Ƿ���ѭ�� by zc
//#define MEMP_OVERFLOW_CHECK             0                //lwip���ڴ������� 
//#define MEM_USE_POOLS                   0                //�ڴ�Ѳ�ͨ���ڴ�ط���
//#define MEM_USE_POOLS_TRY_BIGGER_POOL   0             �� //�����ڴ�ʧ�ܲ�ѡ����ڴ��
//#define MEMP_USE_CUSTOM_POOLS           0                //ͬ��

//MEMP_NUM_PBUF:memp�ṹ��pbuf����,���Ӧ�ô�ROM���߾�̬�洢�����ʹ�������ʱ,���ֵӦ�����ô�һ��
#define MEMP_NUM_PBUF           60
//#define MEMP_NUM_RAW_PCB        4    ���� //RAW���ӵ�PCB��Ŀ(��ҪLWIP RAW��Ч)

//MEMP_NUM_UDP_PCB:UDPЭ����ƿ�(PCB)����.ÿ�����UDP"����"��Ҫһ��PCB.
#define MEMP_NUM_UDP_PCB        6       //�ܹ�ͬʱ���ӵ�UDP��PCB��Ŀ

//MEMP_NUM_TCP_PCB:ͬʱ���������TCP����
#define MEMP_NUM_TCP_PCB        10
//#define MEMP_NUM_TCP_PCB                    (TCP_WND + TCP_SND_BUF)/TCP_MSS  //�ܹ�ͬʱ���ӵ�TCP��PCB��Ŀ 12

//MEMP_NUM_TCP_PCB_LISTEN:�ܹ�������TCP��������
//#define MEMP_NUM_TCP_PCB_LISTEN 			 6
#define MEMP_NUM_TCP_PCB_LISTEN              6       //(TCP_WND + TCP_SND_BUF)/TCP_MSS  //�ܹ�ͬʱ������TCP��PCB��Ŀ

//MEMP_NUM_TCP_SEG:���ͬʱ�ڶ����е�TCP������
#define MEMP_NUM_TCP_SEG        15
//#define MEMP_NUM_TCP_SEG                    40  ������//80 �ܹ�ͬʱ�ڶ������TCP��PCB��Ŀ

//#define MEMP_NUM_REASSDATA                   8       //���ͬʱ�ȴ���װ��IP����Ŀ��������IP��������IP��Ƭ
//#define MEMP_NUM_ARP_QUEUE                  30       //���ȴ��ظ�ARP�������Ŀ(ARP_QUEUEING��Ч)
//#define MEMP_NUM_IGMP_GROUP                  8       //�ಥ��ͬʱ�������ݵ�����Ա��Ŀ(LWIP_IGMP��Ч)

//MEMP_NUM_SYS_TIMEOUT:�ܹ�ͬʱ�����timeout����
//#define MEMP_NUM_SYS_TIMEOUT    8
#define MEMP_NUM_SYS_TIMEOUT                20       //�ܹ�ͬʱ����ĳ�ʱ������Ŀ(NO_SYS==0��Ϸ)

#define MEMP_NUM_NETBUF                     10       //netbufs�ṹ����Ŀ
#define MEMP_NUM_NETCONN                    16       //netconns�ṹ����Ŀ
#define MEMP_NUM_TCPIP_MSG_API              40       //tcpip_msg�ṹ�������Ŀ������callback��API��ͨѶ by zc
#define MEMP_NUM_TCPIP_MSG_INPKT            40       //tcpip_msg�������ݰ��ṹ�������Ŀ by zc

/* ---------- Pbufѡ��---------- */
//PBUF_POOL_SIZE:pbuf�ڴ�ظ���. 
//#define PBUF_POOL_SIZE          20
#define PBUF_POOL_SIZE                      20       //�ڴ������(С�ڴ��С��ѡ��ɴ���С�ڴ�ռ��)

//PBUF_POOL_BUFSIZE:ÿ��pbuf�ڴ�ش�С. 
#define PBUF_POOL_BUFSIZE       512



//*****************************************************************************
//
// ---------- ARP options ----------
//
//*****************************************************************************
#define LWIP_ARP                            1        //ARPЭ������ 
//#define ARP_TABLE_SIZE                      10  ������//ARPά���ı����С
//#define ARP_QUEUEING                        1   ������//Ӳ����ַ����ʱ�����������ݰ��������
//#define ETHARP_TRUST_IP_MAC                 1   ������//����IP���ݰ�����ֱ������ARP table�ĸ��£�
//                                                     //Ϊ0��Ǳ�����IP-MAC��ϵ������ARP����,���Ա���IP��ƭ�������������ʱ
//#define ETHARP_SUPPORT_VLAN                 0   ������//�������������Ϊ1������������ͨѶ��Ч
// 
 

//*****************************************************************************
//
// ---------- IP options ----------
//
//*****************************************************************************
#define IP_FORWARD                          0    //������ƥ�����ݰ�ת��,��ӿ�ʱΪ1
#define IP_OPTIONS_ALLOWED                  1    //��IPѡ�����ݰ����� Ϊ0��������IP���ݰ�
#define IP_REASSEMBLY                       1    //�������IP����Ƭ��(Ϊ0���������ܹ����մ���MTU�İ�)
#define IP_FRAG                             1    //������IP����Ƭ��
#define IP_REASS_MAXAGE                     3    //������յ����ֶ���
#define IP_REASS_MAX_PBUFS                  10   //���������ڵ�IP��Ƭ��ռ�õ��ڴ�ظ���
#define IP_FRAG_USES_STATIC_BUF             1    //IP��Ƭʹ�þ�̬������
#define IP_FRAG_MAX_MTU                     1500 //IP��Ƭ��󻺳�����
#define IP_DEFAULT_TTL                      255  //IP���ݰ�������豸��Ŀ
#define IP_SOF_BROADCAST                    0    //IP���͹㲥������
#define IP_SOF_BROADCAST_RECV               0    //IP���չ㲥������

/* ---------- TCPѡ��---------- */
#define LWIP_TCP                1  //Ϊ1��ʹ��TCP
#define TCP_TTL                  (IP_DEFAULT_TTL)                   //TCP���ݰ��������255//����ʱ��

/*��TCP�����ݶγ�������ʱ�Ŀ���λ,���豸���ڴ��С��ʱ�����ӦΪ0*/
#undef TCP_QUEUE_OOSEQ
#define TCP_QUEUE_OOSEQ         0				//���������Ҫ����Ϊ1
//���TCP�ֶ�
#define TCP_MSS                 (1500 - 40)	  //TCP_MSS = (MTU - IP��ͷ��С - TCP��ͷ��С
#define TCP_CALCULATE_EFF_SEND_MSS          1                                  //tcpԤ�Ʒ��͵ķֶγ��ȣ�Ϊ1����ݴ��ڴ�С����

//TCP���ͻ�������С(bytes).
#define TCP_SND_BUF             (4*TCP_MSS)		//(8*TCP_MSS)     //TCP���ͻ�����  zc 7.1

//TCP_SND_QUEUELEN: TCP���ͻ�������С(pbuf).���ֵ��СΪ(2 * TCP_SND_BUF/TCP_MSS) */
#define TCP_SND_QUEUELEN        (2* TCP_SND_BUF/TCP_MSS)	//(4*(TCP_SND_BUF/TCP_MSS))          //TCP���Ͷ��г���
#define TCP_SNDLOWAT            (TCP_SND_BUF/4)                    //TCP�ɷ������ݳ���
#define TCP_LISTEN_BACKLOG       1                                  //TCP����������
#define TCP_DEFAULT_LISTEN_BACKLOG          0xff                               //TCP���ӺͰ����ӵ�����
#define LWIP_TCP_TIMESTAMPS                 0                                  //TCPʱ���ѡ��
#define TCP_WND_UPDATE_THRESHOLD            (TCP_WND / 4)                      //TCP���ڸ�����ֵ

//TCP���ʹ���
#define TCP_WND                  4*TCP_MSS                          //tcp���ڴ�С
#define TCP_MAXRTX               12                                 //��������ش�TCP���ݰ���Ŀ
#define TCP_SYNMAXRTX            6                                  //��������ش�SYN���Ӱ���Ŀ

//TCP��ʱ������ѡ��
//#define  TCP_MSL                             10000UL                            //TCP���Ӵ���ʱ�� ��λms
//#define  TCP_FIN_WAIT_TIMEOUT                20000UL                            //FIN�ȵ�ACKӦ��ʱ�� ��λms
//#define  TCP_TMR_INTERVAL                    20                                 //TCP��ʱ��������� 20ms

/* ---------- ICMPѡ��---------- */

#define LWIP_ICMP                           1    //����ping������/����
#define ICMP_TTL                            (IP_DEFAULT_TTL) //ping��������豸��Ŀ
#define LWIP_BROADCAST_PING                 0    //���ظ��㲥ping��
#define LWIP_MULTICAST_PING                 0    //���ظ��ಥping��

//*****************************************************************************
//
// ---------- RAWѡ�� ----------
//
//*****************************************************************************
//#define LWIP_RAW                            0              //�޲���ϵͳ���ڻص���������
//#define RAW_TTL                           (IP_DEFAULT_TTL) //Ӧ�ò����ݴ������(����IP���TTL)

/* ---------- DHCPѡ��---------- */
//��ʹ��DHCPʱ��λӦ��Ϊ1,LwIP 0.5.1�汾��û��DHCP����.
#define LWIP_DHCP               							0	 //��̬����Э������(Ϊ1ʱ)
//#define LWIP_AUTOIP                         0    //��̬����IP��ַ����(Ϊ1ʱ)
//#define LWIP_DHCP_AUTOIP_COOP               0    //����������������ͬʱ������1���ӿ�(Ϊ1ʱ)
//#define LWIP_DHCP_AUTOIP_COOP_TRIES         9    //DHCP��������ʧ�ܴ�����ʧ����ʹ��AUTOUP

//*****************************************************************************
//
// ---------- IGMPѡ�� ----------
//
//*****************************************************************************
#define LWIP_IGMP                           0            //LWIP�����Э��

//*****************************************************************************
//
// ---------- DNSѡ�� -----------
//
//*****************************************************************************
#define LWIP_DNS                            0     //����������ģ��(����UDPЭ��)
#define DNS_TABLE_SIZE                      4     //����������ά������������Ŀ
#define DNS_MAX_NAME_LENGTH                 256   //����������������ַ��󳤶�
#define DNS_MAX_SERVERS                     2     //������������������Ŀ
#define DNS_DOES_NAME_CHECK                 1     //��ѯ����������ʱ����ַ��
#define DNS_USES_STATIC_BUF                 1     //����������ʹ�þ�̬��ַ
#define DNS_MSG_SIZE                        512   //�����������������ͨѶ���ݳ���
#define DNS_LOCAL_HOSTLIST                  0     //�ڱ���ά����������������-��ַ��(Ϊ1ʱ)
#define DNS_LOCAL_HOSTLIST_IS_DYNAMIC       0     //����-��ַ��ʵʱ����(Ϊ1ʱ)


/* ---------- UDPѡ�� ---------- */ 
#define LWIP_UDP                               1            //����UDPЭ��(snmpЭ����ڴ�)
#define LWIP_UDPLITE                           1            //UDPЭ�����þ����
#define UDP_TTL                                (IP_DEFAULT_TTL) //UDP���ݰ��������
#define LWIP_NETBUF_RECVINFO                   0            //���յ������ݰ����׸�������������Ŀ�ĵ�ַ�Ͷ˿�


/* ---------- Statistics options ---------- */
#define LWIP_STATS 0				//LWIPͳ��ѡ��
#define LWIP_PROVIDE_ERRNO 1

//*****************************************************************************
//         
// ---------- Internal Memory Pool Sizes ----------
//
//*****************************************************************************
 #define  PBUF_LINK_HLEN                 14           //Ϊ������̫��ͷ����İ�����(����MAC��ַ+Զ��MAC��ַ+Э������) 6+6+2                          #define  PBUF_POOL_BUFSIZE             256           //�����ڴ�س��ȣ�Ҫ���ǵ�4�ֽڶ������Ѵ�С
 #define  ETH_PAD_SIZE                  0             //��̫����䳤�ȣ�stm32����Ϊ0 ���ݷ��Ͱ��ж�
 
//��ϵ����.. Ĭ�ϼ���

/*
   --------------------------------------
   ---------- ֡У���ѡ��----------
   --------------------------------------
*/

//STM32F4x7����ͨ��Ӳ��ʶ��ͼ���IP,UDP��ICMP��֡У���

#define CHECKSUM_BY_HARDWARE //����CHECKSUM_BY_HARDWARE,ʹ��Ӳ��֡У��


#ifdef CHECKSUM_BY_HARDWARE
  //CHECKSUM_GEN_IP==0: Ӳ������IP���ݰ���֡У���
  #define CHECKSUM_GEN_IP                 0
  //CHECKSUM_GEN_UDP==0: Ӳ������UDP���ݰ���֡У���
  #define CHECKSUM_GEN_UDP                0
  //CHECKSUM_GEN_TCP==0: Ӳ������TCP���ݰ���֡У���
  #define CHECKSUM_GEN_TCP                0 
  //CHECKSUM_CHECK_IP==0: Ӳ����������IP���ݰ�֡У���
  #define CHECKSUM_CHECK_IP               0
  //CHECKSUM_CHECK_UDP==0: Ӳ����������UDP���ݰ�֡У���
  #define CHECKSUM_CHECK_UDP              0
  //CHECKSUM_CHECK_TCP==0: Ӳ����������TCP���ݰ�֡У���
  #define CHECKSUM_CHECK_TCP              0
#else
  /* CHECKSUM_GEN_IP==1: Generate checksums in software for outgoing IP packets.*/
  #define CHECKSUM_GEN_IP                 1
  /* CHECKSUM_GEN_UDP==1: Generate checksums in software for outgoing UDP packets.*/
  #define CHECKSUM_GEN_UDP                1
  /* CHECKSUM_GEN_TCP==1: Generate checksums in software for outgoing TCP packets.*/
  #define CHECKSUM_GEN_TCP                1
  /* CHECKSUM_CHECK_IP==1: Check checksums in software for incoming IP packets.*/
  #define CHECKSUM_CHECK_IP               1
  /* CHECKSUM_CHECK_UDP==1: Check checksums in software for incoming UDP packets.*/
  #define CHECKSUM_CHECK_UDP              1
  /* CHECKSUM_CHECK_TCP==1: Check checksums in software for incoming TCP packets.*/
  #define CHECKSUM_CHECK_TCP              1
#endif

/*
   ----------------------------------------------
   ---------- SequentialAPIѡ��----------
   ----------------------------------------------
*/

//LWIP_NETCONN==1:ʹ��NETCON����(Ҫ��ʹ��api_lib.c)
#define LWIP_TCPIP_CORE_LOCKING           0           //?
#define LWIP_NETCONN                      1           //Ӧ�ò�ʹ��NETCONN��غ���
#define LWIP_SOCKET                     1	//LWIP_SOCKET==1:ʹ��Sicket API(Ҫ��ʹ��sockets.c)
#define LWIP_COMPAT_MUTEX               1
#define LWIP_SO_RCVTIMEO                1 	//ͨ������LWIP_SO_RCVTIMEOʹ��netconn�ṹ����recv_timeout,ʹ��recv_timeout���Ա��������߳�


/*
   ------------------------------------
   ---------- Socket APIѡ��----------
   ------------------------------------
*/
//LWIP_SOCKET==1:ʹ��Sicket API(Ҫ��ʹ��sockets.c)
//#define LWIP_SOCKET                       0               //�ر�SOCKETͨѶ
//#define LWIP_COMPAT_SOCKETS             1
//#define LWIP_POSIX_SOCKETS_IO_NAMES     1
//#define LWIP_SO_RCVTIMEO                  0               //conn->acceptmbox/recvmbox�����г�ʱ����,��ʱ���Զ��Ͽ�����
//#define LWIP_SO_RCVBUF                  0
//#define SO_REUSE                        0
//#define LWIP_COMPAT_MUTEX               1


//*****************************************************************************
//
// ---------- TCP KEEPLIVE ----------
//
//*****************************************************************************
#define LWIP_TCP_KEEPALIVE                  1                //tcp���ʱ��
#define TCP_KEEPIDLE_DEFAULT                60000            //����ʱ�� 60s
#define TCP_KEEPINTVL_DEFAULT               10000            //����̽�� 10s
#define TCP_KEEPCNT_DEFAULT                 9U  

//�й�ϵͳ��ѡ��
#define TCPIP_THREAD_STACKSIZE          1000				//*4	//�ں������ջ��С
#define DEFAULT_UDP_RECVMBOX_SIZE       2000
#define DEFAULT_THREAD_STACKSIZE        512


//*****************************************************************************
//
// ---------- DEBUG_OPTIONS  ----------
//
//*****************************************************************************
#define DEBUG_OPTIONS                                   0                    //���Դ���

/*
   ----------------------------------------
   ---------- Lwip����ѡ��----------
   ----------------------------------------
*/
//#define LWIP_DEBUG                     1 //����DEBUGѡ��

#define ICMP_DEBUG                      LWIP_DBG_OFF //����/�ر�ICMPdebug

#endif /* __LWIPOPTS_H__ */

