
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include"BaseStruct.h"
using namespace std;

int CAL2UTC(UTCTime *T,int flag );//函数功能：UTC的格里历与儒略日进行转换
int CalLeapSec( double UTC_JD);//函数功能：已知UTC简略儒略日计算跳秒
int CalLeapSecINV(double TAI_JD);//函数功能：已知TAI简略儒略日计算跳秒


///////////////////////////////////////////////////////////////////////////////////////////
//函数名称：UTC2TAI
//函数功能：UTC的JD转TAI
//输入:
//       (1)UTC_JD (UTC的儒略日形式)(大于2378861.5)
//       (2)SetLeapSec （设置的跳秒）
//输出:
//       (1)TAI (TAI的儒略日)(大于2378861.5)
//返回值 :
//       (1)0 超界
//       (2)1`運行正確
//      （3）-9999 跳秒文件不在
//其他说明：跳秒（m_LeapSec）必须存在，1972 年以前：计算得到;1972~2016年之间且flag等于1时：读程序自带的跳秒文件得到跳秒;其他情况：m_LeapSec为设置值(缺省值是36）
//被本函数调用的函数清单：
//                    (1)CAL2UTC
//                    (2)int CalLeapSec(UTC_JD)
//调用本函数的函数清单：无
///////////////////////////////////////////////////////////////////////////////////////////
int UTC2TAI(double UTC_JD,double *TAI,int flag,int SetLeapSec=36)
{
	int m_LeapSec=SetLeapSec;
	//算法起算时间是1801年1月1号
	if(UTC_JD<2378861.5)
	{
		return 0;
	}
	UTCTime timeof1999(1999,8,22,0,0,0.0,0.1);
	//刚刚加的
	UTCTime timeof1972(1972,1,1,0,0,0.0,0.1);
	CAL2UTC(&timeof1972,1);
	//1972年以前直接用下面的公式
	if(UTC_JD<timeof1972.JD)
	{
		UTCTime timeof1965(1965,12,31,0,0,0.0,0.1);
		CAL2UTC(&timeof1965,1);
		*TAI=UTC_JD+(4.21317+(UTC_JD-timeof1965.JD)*0.002592)/86400.0;
	}
	else
	{
		//flag等于1且时间在1972~2016年之间就会调用CalLeapSecINV读程序自带的跳秒文件
		if(UTC_JD<2457388.5&&flag==1)
		{
			m_LeapSec= CalLeapSec(  UTC_JD-2400000.5);
			if(m_LeapSec==-9999)
			{
				return -9999;
			}

		}
		//1972年以后直接是整秒差
		*TAI = UTC_JD+( m_LeapSec/ 86400.0 );

	}
	if(*TAI <2378861.5)
	{
		return 0;
	}
	else
	{
		return 1;
	}

}

///////////////////////////////////////////////////////////////////////////////////////////
//函数名称：TAI2UTC
//函数功能：TAI转UTC_JD
//输入:
//       (1)TAI (TAI的儒略日 )(大于2378861.5)
//       (2)SetLeapSec （设置的跳秒）
//输出:
//       (1)UTC_JD(UTC的儒略日形式 )(大于2378861.5)
//返回值 :
//       (1)0 超界
//       (2)1`運行正確
//      （3）-9999 跳秒文件不在
//其他说明：跳秒（m_LeapSec）必须存在，1972 年以前：计算得到;1972~2016年之间且flag等于1时：读程序自带的跳秒文件得到跳秒;其他情况：m_LeapSec为设置值(缺省值是36）
//被本函数调用的函数清单：
//                    (1)CAL2UTC
//                    (2)int CalLeapSecINV(TAI_JD)
//调用本函数的函数清单：无
///////////////////////////////////////////////////////////////////////////////////////////
int TAI2UTC(double TAI,double *UTC_JD,int flag,int SetLeapSec=36)
{
	//算法起算时间是1801年1月1号
	if(TAI<2378861.5)
	{
		return 0;
	}
	int m_LeapSec=SetLeapSec;
	//刚刚加的
	UTCTime timeof1972(1972,1,1,0,0,0.0,0.1);
	CAL2UTC(&timeof1972,1);
	//1972年以前直接用下面的公式
	if(TAI<timeof1972.JD)
	{
		UTCTime timeof1965(1965,12,31,0,0,0.0,0.1);
		CAL2UTC(&timeof1965,1);
		*UTC_JD=(TAI+(timeof1965.JD*0.002592-4.21317)/86400.0)/(1+0.002592/86400.0);
	}
	else 
	{
		//flag等于1且时间在1972~2016年之间就会调用CalLeapSecINV读程序自带的跳秒文件
		if(TAI<=2457388.5&&flag==1)
		{
			m_LeapSec=CalLeapSecINV(  TAI-2400000.5);
			if(m_LeapSec==-9999)
			{
				return -9999;
			}

		}
		*UTC_JD = TAI-( m_LeapSec/ 86400.0 );

	}
	if(*UTC_JD<2378861.5)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////
//函数名称：CAL2UTC
//函数功能：UTC的格里历与儒略日进行转换
//输入:
//       (1)T    (UTC时） （ flag=1时，是UTC的年月日形式;flag=2时，是UTC的儒略日形式）(T 对应的时间大于1801年1月1号）
//       (2)flag （标记位 ）（当flag＝1时，由公历计算JD;当flag＝2时，由JD计算公历）
//输出:无
//返回值:
//       (1) 0 超出边界
//       (2) 1 运行正确
//其他说明：直接给极移参数赋值
//被本函数调用的函数清单：无
//调用本函数的函数清单：(1)UTC2doy1 (2)doy2UTC (3)BDT_JD2WnSec  (4)GPST_JD2WnSec (5)BDT_WnSec2JD (6)GPST_WnSec2JD  (7)GST_WnSec2JD (8)GST_JD2WnSec (9)GLONASST_N4NtHrMinSec2JD (10)GLONASST_JD2N4NtHrMinSec  (11)GLONASST_N4NtHrMinSec2JD (12)TAI2UTC (13)UTC2TAI (14)UTC2TDB (15)TDB2UTC
///////////////////////////////////////////////////////////////////////////////////////////
int CAL2UTC(UTCTime *T,int flag )
{	
	long JD;
	long LX,NX;
	long double XTIME,UTCP;


	if(flag==1)						//由公历计算JD,整数和小数分开进行高精度处理
	{

		if(T->Mon>12||T->Mon<1||T->Hour<0||T->Hour>23||T->Min>59||T->Min<0||T->Sec<0||T->Sec>=60||T->Day>31||T->Day<1||(T->Year<1801))
		{
			return 0;
		}

		if(T->Mon==2)
		{
			//闰年判断日二月是否大于29
			if((T->Year%4==0&&T->Year%100!=0)||(T->Year%400==0))
			{
				if (T->Day>29||T->Day<1)
					return 0;
			}
			//不是闰年判断日是否大于28
			else 
			{
				if(T->Day>28||T->Day<1)
					return 0;
			}

		}
		else
		{
			if(T->Mon==4||T->Mon==6||T->Mon==9||T->Mon==11)
			{
				if (T->Day>30)
					return 0;

			}
		}

		T->IntJD = T->Day - 32075 + int(1461 * (T->Year + 4800 + int((T->Mon-14)/ 12) )/ 4) +
			int(367 * (T->Mon - 2 - int((T->Mon-14)/ 12)*12 ) /12) - 
			int(3* (T->Year + 4900 + int((T->Mon-14)/ 12))/ 100/ 4);
		T->DouJD =- 0.5 + T->Hour/ 24.0 + T->Min/ 1440.0 + T->Sec/ 86400.0;
		if(T->DouJD<0)
		{
			T->DouJD+=1.0;
			T->IntJD-=1.0;
		}

		//进行计算误差处理
		T->JD=T->IntJD+T->DouJD;

	}
	
			else
				if(flag==2)				//由JD计算公历,整数和小数分开进行高精度处理
				{
					if(T->JD<2378861.5)
					{
						return 0;
					}
					T->IntJD=(int)(T->JD);
					T->DouJD=T->JD-T->IntJD;
					if(T->DouJD<0)
					{
						T->DouJD+=1.0;
						T->IntJD-=1.0;
					}
					//COMPUTE THE HOUR, MINUTE, AND SECOND.                                                                                         		
					UTCP = T->DouJD+0.5;		 
					XTIME = fmod (UTCP*24,24) ;                                                                
					T->Hour = (long)XTIME;                                                                                                           
					XTIME = (XTIME-T->Hour) * 60.0 ;                                                                                           
					T->Min = (long)XTIME;
					T->Sec = (XTIME- T->Min ) * 60.0 ;                                                                                           
					JD=(long)(T->IntJD+0.00001);
					if (T->Hour <12) JD = JD + 1 ;                                                                                           
					LX=JD + 68569 ;                                                                                                              
					NX = 4 * LX / 146097 ;                                                                                                       
					LX = LX - (146097*NX+3)/4  ;                                                                                                 
					T->Year = 4000 * (LX+1) / 1461001;                                                                                         
					LX = LX - 1461*T->Year/4 + 31  ;                                                                                           
					T->Mon = 80 * LX / 2447    ;                                                                                              
					T->Day = LX - 2447*T->Mon/80    ;                                                                                      
					LX = T->Mon / 11 ;                                                                                                        
					T->Mon = T->Mon + 2 - 12*LX ;                                                                                          
					T->Year = 100*(NX-49) + T->Year + LX ;
					if(T->Sec>59.9999)
					{
						T->Sec=0;
						T->Min+=1;
						if(T->Min>59)
						{
							T->Min=0;
							T->Hour+=1;
							if(T->Hour>23)
							{
								T->Hour=0;
								T->Day+=1;
								//这里还需判断，好麻烦，要考虑年和月份
								if(T->Mon==2)
								{
									if((T->Year%4==0&&T->Year%100!=0)||(T->Year%400==0))
									{
										if(T->Day>29)
											T->Day=0;
										T->Mon+=1;
									}
									else
									{
										if(T->Day>28)
											T->Day=0;
										T->Mon+=1;
									}

								}
								if(T->Mon==1||T->Mon==3||T->Mon==5||T->Mon==7||T->Mon==8||T->Mon==10||T->Mon==12)
								{
									if(T->Day>31)
										T->Day=0;
									T->Mon+=1;
								}
								if(T->Mon==4||T->Mon==6||T->Mon==9||T->Mon==11)
								{
									if(T->Day>30)
										T->Day=0;
									T->Mon+=1;
								}
								if(T->Mon>12)
								{
									T->Mon=0;
									T->Year+=1;
								}

							}

						}
					}
				}

				else 
				{

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
	fp=NULL;
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
	fp0=NULL;
	return leapsec;

}

