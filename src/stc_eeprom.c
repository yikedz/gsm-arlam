#include "STC11.h"
#include <intrins.H>

//定义Flash 操作等待时间及允许IAP/ISP/EEPROM 操作的常数
//#define ENABLE_ISP 0x80 //系统工作时钟<30MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x81 //系统工作时钟<24MHz 时，对IAP_CONTR 寄存器设置此值
#define ENABLE_ISP 0x82 //系统工作时钟<20MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x83 //系统工作时钟<12MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x84 //系统工作时钟<6MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x85 //系统工作时钟<3MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x86 //系统工作时钟<2MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x87 //系统工作时钟<1MHz 时，对IAP_CONTR 寄存器设置此值

unsigned char Byte_Read(unsigned int addr);              //读一字节，调用前需打开IAP 功能
void Byte_Program(unsigned int addr, unsigned char ch);  //字节编程，调用前需打开IAP 功能
void Sector_Erase(unsigned int addr);            //擦除扇区
void IAP_Disable();                       //关闭IAP 功能

//读一字节，调用前需打开IAP 功能，入口:DPTR = 字节地址，返回:A = 读出字节
unsigned char Byte_Read(unsigned int addr)
{
	unsigned char dat = 0;
	
    IAP_CONTR = ENABLE_ISP;         //打开IAP 功能, 设置Flash 操作等待时间
	
    IAP_CMD = 0x01;                 //IAP/ISP/EEPROM 字节读命令

    IAP_ADDRL = addr;    //设置目标单元地址的高8 位地址
	
    IAP_ADDRH = addr>>8;    //设置目标单元地址的低8 位地址

    IAP_TRIG = 0x5A;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
	
    IAP_TRIG = 0xA5;   //送完A5h 后，ISP/IAP 命令立即被触发起动
	
    _nop_();
    _nop_();

	dat = IAP_DATA;
	
    IAP_Disable();  //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
    return (dat);
}

//字节编程，调用前需打开IAP 功能，入口:DPTR = 字节地址, A= 须编程字节的数据
void Byte_Program(unsigned int addr, unsigned char dat)
{
    IAP_CONTR = ENABLE_ISP;         //打开 IAP 功能, 设置Flash 操作等待时间
	
    IAP_CMD = 0x02;                 //IAP/ISP/EEPROM 字节编程命令
	
    IAP_ADDRL = addr;    //设置目标单元地址的高8 位地址
	
    IAP_ADDRH = addr>>8;    //设置目标单元地址的低8 位地址
	
    IAP_DATA = dat;                  //要编程的数据先送进IAP_DATA 寄存器
	
    IAP_TRIG = 0x5A;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
	
    IAP_TRIG = 0xA5;   //送完A5h 后，ISP/IAP 命令立即被触发起动
	
    _nop_();

    _nop_();

	
    IAP_Disable();  //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
}

//擦除扇区, 入口:addr = 扇区地址
void Sector_Erase(unsigned int addr)
{
    IAP_CONTR = ENABLE_ISP;         //打开IAP 功能, 设置Flash 操作等待时间
	
    IAP_CMD = 0x03;                 //IAP/ISP/EEPROM 扇区擦除命令

    IAP_ADDRL = addr;    //设置目标单元地址的高8 位地址
	
    IAP_ADDRH = addr>>8;    //设置目标单元地址的低8 位地址

    IAP_TRIG = 0x5A;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
	
    IAP_TRIG = 0xA5;   //送完A5h 后，ISP/IAP 命令立即被触发起动
	
    _nop_();

    _nop_();

	
    IAP_Disable();  //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
}

void IAP_Disable()
{
    //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
    IAP_CONTR = 0;      //关闭IAP 功能
	
    IAP_CMD   = 0;      //清命令寄存器,使命令寄存器无命令,此句可不用
	
    IAP_TRIG  = 0;      //清命令触发寄存器,使命令触发寄存器无触发,此句可不用
	
    IAP_ADDRH = 0X80;
	
    IAP_ADDRL = 0;
}
