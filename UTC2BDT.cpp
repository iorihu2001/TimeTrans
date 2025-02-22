#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string.h>
using namespace std;


int CalLeapSec( double UTC_JD);//函数功能：已知UTC简略儒略日计算跳秒
int CalLeapSecINV(double TAI_JD);//函数功能：已知TAI简略儒略日计算跳秒



///////////////////////////////////////////////////////////////////////////////////////////
//函数名称：UTC2BDT
//函数功能：UTC转BDT    (XIELING)
//输入:
//       (1)UTC_JD （UTC的儒略日形式 ） 
//       (2)SetLeapSec （设置的跳秒）
//输出:
//       (1)BDT_JD （BDT时的儒略日形式）(BDT_JD>=2453736.5 )
//返回值 :
//       (1)0 超界
//       (2)1`運行正確
//      （3）-9999 跳秒文件不在
//其他说明：调用此函数时跳秒（m_LeapSec）必须存在，2016年以前,且flag等于1：读程序自带的跳秒文件得到跳秒;其他情况：m_LeapSec为设置值（缺省值为36）
//被本函数调用的函数清单：int CalLeapSec(UTC_JD)
//调用本函数的函数清单：无
///////////////////////////////////////////////////////////////////////////////////////////
int UTC2BDT(double UTC_JD,double *BDT_JD,int flag,int SetLeapSec=36)
{
	int m_LeapSec=SetLeapSec;
	if(UTC_JD<2457388.5&&flag==1)
	{
		m_LeapSec= CalLeapSec(  UTC_JD-2400000.5);
		if(m_LeapSec==-9999)
		{
			return -9999;
		}
	}
	*BDT_JD=UTC_JD+m_LeapSec/86400.0-19/86400.0-14/86400.0;

	//北斗时的起始儒略日是2453736.5
	if(*BDT_JD<(2453736.5-10E-8))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
//函数名称：BDT2UTC
//函数功能：BDT 转UTC  xieling
//输入:
//       (1)BDT_JD (BDT时的儒略日形式)(BDT_JD>=2453736.5  )
//       (2)SetLeapSec （设置的跳秒）
//输出:
//       (1)UTC_JD （UTC的儒略日形式)
//返回值 :
//       (1)0 超界
//       (2)1`運行正確
//      （3）-9999 跳秒文件不在
//其他说明：调用此函数时跳秒（m_LeapSec）必须存在，2016年以前,且flag等于1：读程序自带的跳秒文件得到跳秒;其他情况：m_LeapSec为设置值（缺省值为36）

//被本函数调用的函数清单：int CalLeapSecINV(TAI_JD)
//调用本函数的函数清单：无
///////////////////////////////////////////////////////////////////////////////////////////
int BDT2UTC(double BDT_JD,double *UTC_JD,int flag,int SetLeapSec=36)
{
	int m_LeapSec=SetLeapSec;
	//北斗时的起始儒略日是2453736.5
	if(BDT_JD<(2453736.5-10E-8))
	{
		return 0;
	}
	else
	{  
		if(BDT_JD<2457388.5&&flag==1)
		{
			m_LeapSec=CalLeapSecINV(BDT_JD+19/86400.0+14/86400.0-2400000.5);
			if(m_LeapSec==-9999)
			{
				return -9999;
			}
		}
		*UTC_JD=BDT_JD-m_LeapSec/86400.0+19/86400.0+14/86400.0;
	}
	return 1;
}




///////////////////////////////////////////////////////////////////////////////////////////
//函数功能：计算当前UTC时间对应的跳秒
//输入:
//       (1)UTC_JD  (UTC的简略儒略日)
//输出:   
//       无
//返回值:
//(1)得到的跳秒值 (秒）
//（2）-9999 文件不在
//其他说明：无
//被本函数调用的函数清单：无
///////////////////////////////////////////////////////////////////////////////////////////
int CalLeapSec( double UTC_JD)
{
	char filename[100]="LeapSec.txt";
	int leapsec=0,beforeleapsec=0;
	int time=0,beforetime=0;
	FILE *fp=fopen(filename,"r");
	if(fp==NULL)
	{
		return -9999;
	}
	while(!feof(fp))
	{
		fscanf(fp,"%d %d",&time,&leapsec);
		if(UTC_JD<(time-10E-8))
		{
			leapsec=beforeleapsec;
			break;
		}
		beforeleapsec=leapsec;
		beforetime=time;
	}
	fclose(fp);
	return leapsec;


}


///////////////////////////////////////////////////////////////////////////////////////////
//函数功能：计算当前TAI_JD时间对应的跳秒
//输入:
//       (1)TAI_JD  (TAI的简略儒略日)
//输出:   
//      无
//返回值:
// (1)得到的跳秒值 (秒）
// （2）-9999 文件不在
//其他说明：无
//被本函数调用的函数清单：无
///////////////////////////////////////////////////////////////////////////////////////////
int  CalLeapSecINV(double TAI_JD)
{
	char filename[100]="LeapSec.txt";
	int leapsec=0,beforeleapsec=0;
	int time=0,beforetime=0;
	FILE *fp0=fopen(filename,"r");
	if(fp0==NULL)
	{
		return -9999;
	}
	while(!feof(fp0))
	{
		fscanf(fp0,"%d %d",&time,&leapsec);
		if(TAI_JD<(time+leapsec/86400.0-10E-8))
		{
			leapsec=beforeleapsec;
			break;
		}
		beforeleapsec=leapsec;
		beforetime=time;
	}
	fclose(fp0);
	return leapsec;

}