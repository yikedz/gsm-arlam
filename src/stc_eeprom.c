#include "STC11.h"
#include <intrins.H>

//����Flash �����ȴ�ʱ�估����IAP/ISP/EEPROM �����ĳ���
//#define ENABLE_ISP 0x80 //ϵͳ����ʱ��<30MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x81 //ϵͳ����ʱ��<24MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
#define ENABLE_ISP 0x82 //ϵͳ����ʱ��<20MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x83 //ϵͳ����ʱ��<12MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x84 //ϵͳ����ʱ��<6MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x85 //ϵͳ����ʱ��<3MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x86 //ϵͳ����ʱ��<2MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x87 //ϵͳ����ʱ��<1MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ

unsigned char Byte_Read(unsigned int addr);              //��һ�ֽڣ�����ǰ���IAP ����
void Byte_Program(unsigned int addr, unsigned char ch);  //�ֽڱ�̣�����ǰ���IAP ����
void Sector_Erase(unsigned int addr);            //��������
void IAP_Disable();                       //�ر�IAP ����

//��һ�ֽڣ�����ǰ���IAP ���ܣ����:DPTR = �ֽڵ�ַ������:A = �����ֽ�
unsigned char Byte_Read(unsigned int addr)
{
	unsigned char dat = 0;
	
    IAP_CONTR = ENABLE_ISP;         //��IAP ����, ����Flash �����ȴ�ʱ��
	
    IAP_CMD = 0x01;                 //IAP/ISP/EEPROM �ֽڶ�����

    IAP_ADDRL = addr;    //����Ŀ�굥Ԫ��ַ�ĸ�8 λ��ַ
	
    IAP_ADDRH = addr>>8;    //����Ŀ�굥Ԫ��ַ�ĵ�8 λ��ַ

    IAP_TRIG = 0x5A;   //���� 5Ah,����A5h ��ISP/IAP �����Ĵ���,ÿ�ζ������
	
    IAP_TRIG = 0xA5;   //����A5h ��ISP/IAP ����������������
	
    _nop_();
    _nop_();

	dat = IAP_DATA;
	
    IAP_Disable();  //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
                    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
    return (dat);
}

//�ֽڱ�̣�����ǰ���IAP ���ܣ����:DPTR = �ֽڵ�ַ, A= �����ֽڵ�����
void Byte_Program(unsigned int addr, unsigned char dat)
{
    IAP_CONTR = ENABLE_ISP;         //�� IAP ����, ����Flash �����ȴ�ʱ��
	
    IAP_CMD = 0x02;                 //IAP/ISP/EEPROM �ֽڱ������
	
    IAP_ADDRL = addr;    //����Ŀ�굥Ԫ��ַ�ĸ�8 λ��ַ
	
    IAP_ADDRH = addr>>8;    //����Ŀ�굥Ԫ��ַ�ĵ�8 λ��ַ
	
    IAP_DATA = dat;                  //Ҫ��̵��������ͽ�IAP_DATA �Ĵ���
	
    IAP_TRIG = 0x5A;   //���� 5Ah,����A5h ��ISP/IAP �����Ĵ���,ÿ�ζ������
	
    IAP_TRIG = 0xA5;   //����A5h ��ISP/IAP ����������������
	
    _nop_();

    _nop_();

	
    IAP_Disable();  //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
                    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
}

//��������, ���:addr = ������ַ
void Sector_Erase(unsigned int addr)
{
    IAP_CONTR = ENABLE_ISP;         //��IAP ����, ����Flash �����ȴ�ʱ��
	
    IAP_CMD = 0x03;                 //IAP/ISP/EEPROM ������������

    IAP_ADDRL = addr;    //����Ŀ�굥Ԫ��ַ�ĸ�8 λ��ַ
	
    IAP_ADDRH = addr>>8;    //����Ŀ�굥Ԫ��ַ�ĵ�8 λ��ַ

    IAP_TRIG = 0x5A;   //���� 5Ah,����A5h ��ISP/IAP �����Ĵ���,ÿ�ζ������
	
    IAP_TRIG = 0xA5;   //����A5h ��ISP/IAP ����������������
	
    _nop_();

    _nop_();

	
    IAP_Disable();  //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
                    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
}

void IAP_Disable()
{
    //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
    IAP_CONTR = 0;      //�ر�IAP ����
	
    IAP_CMD   = 0;      //������Ĵ���,ʹ����Ĵ���������,�˾�ɲ���
	
    IAP_TRIG  = 0;      //��������Ĵ���,ʹ������Ĵ����޴���,�˾�ɲ���
	
    IAP_ADDRH = 0X80;
	
    IAP_ADDRL = 0;
}
