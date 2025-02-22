#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include"BaseStruct.h"
using namespace std;
int CAL2UTC(UTCTime *T,int flag );


///////////////////////////////////////////////////////////////////////////////////////////
//�������ƣ�GPST_WnSec2JD
//�������ܣ�GPSʱ���ܺ�������ת��Ϊ�����գ�xeiling)
//����:
//       (1)WN (�ܣ���WN>=0)
//       (2)Sec (�����룩(0<=Sec<604800)
//���:
//       (1)JD (JD>=2444244.5)
//����ֵ :
//       (1)0 ����
//       (2)1`�\�����_
//����˵������
//�����������õĺ����嵥��CAL2UTC
//���ñ������ĺ����嵥����
///////////////////////////////////////////////////////////////////////////////////////////
int GPST_WnSec2JD(int WN,double Sec,double *JD)
{
	if(WN<0||Sec>=604800||Sec<0)
	{
		return 0;
	}
	else
	{
		//GPSʱ����ʼʱ����1980��1��6��
		UTCTime timeof1980(1980,1,6,0,0,0.0,0.1);
		CAL2UTC(&timeof1980,1);

		UTCTime CurTime;
		CurTime.JD=timeof1980.JD+WN*7+Sec/86400.0;
		//GPSʱ����ʼ������Ϊ2444244.5
		if(CurTime.JD<(2444244.5-10e-8))
		{
			return 0;
		}
		else
		{
			*JD=CurTime.JD;
		}
	}
	return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////
//�������ƣ�GPST_JD2WnSec����GPST_ymdhms2ws��ȣ����Ƚϵͣ�
//�������ܣ�GPSʱ��������ת��Ϊ�ܺ������루xeiling)
//����:
//       (1)JD�������գ�(JD��Ӧ�������մ���(1980,1,6,0,0,0.0,0.1)��Ӧ��������) ��JD>=2444244.5��
//���:
//       (1)WN (�ܣ���WN>=0)
//       (2)Sec(�����룩(0<=Sec<604800)
//����ֵ :
//       (1)0 ����
//       (2)1`�\�����_
//����˵������
//�����������õĺ����嵥��CAL2UTC
//���ñ������ĺ����嵥����
///////////////////////////////////////////////////////////////////////////////////////////
int GPST_JD2WnSec(double JD,int *WN,double *Sec)// GPSʱ��������תΪ�ܺ������루xeiling)
{
	//GPSʱ����ʼʱ����1980��1��6��
	UTCTime timeof1980(1980,1,6,0,0,0.0,0.1);
	CAL2UTC(&timeof1980,1);
	double delt_JD=JD-timeof1980.JD;
	if (delt_JD<(-10e-9))
	{
		return 0;
	}
	else
	{
		*WN= int(delt_JD/7);
		*Sec= (delt_JD/7-*WN)*7*86400.0;
		if(*Sec>(604800.0-10e-8))
		{
			*Sec=0;
			*WN+=1;
		}
	}
	return 1;

}

///////////////////////////////////////////////////////////////////////////////////////////
//�������ƣ�CAL2UTC
//�������ܣ�UTC�ĸ������������ս���ת��
//����:
//       (1)T    (UTCʱ�� �� flag=1ʱ����UTC����������ʽ;flag=2ʱ����UTC����������ʽ��(T ��Ӧ��ʱ�����1801��1��1�ţ�
//       (2)flag �����λ ������flag��1ʱ���ɹ�������JD;��flag��2ʱ����JD���㹫����
//���:��
//����ֵ:
//       (1) 0 �����߽�
//       (2) 1 ������ȷ
//����˵����ֱ�Ӹ����Ʋ�����ֵ
//�����������õĺ����嵥����
//���ñ������ĺ����嵥��(1)UTC2doy1 (2)doy2UTC (3)BDT_JD2WnSec  (4)GPST_JD2WnSec (5)BDT_WnSec2JD (6)GPST_WnSec2JD  (7)GST_WnSec2JD (8)GST_JD2WnSec (9)GLONASST_N4NtHrMinSec2JD (10)GLONASST_JD2N4NtHrMinSec  (11)GLONASST_N4NtHrMinSec2JD (12)TAI2UTC (13)UTC2TAI (14)UTC2TDB (15)TDB2UTC
///////////////////////////////////////////////////////////////////////////////////////////
int CAL2UTC(UTCTime *T,int flag )
{	
	long JD;
	long LX,NX;
	long double XTIME,UTCP;


	if(flag==1)						//�ɹ�������JD,������С���ֿ����и߾��ȴ���
	{

		if(T->Mon>12||T->Mon<1||T->Hour<0||T->Hour>23||T->Min>59||T->Min<0||T->Sec<0||T->Sec>=60||T->Day>31||T->Day<1||(T->Year<1801))
		{
			return 0;
		}

		if(T->Mon==2)
		{
			//�����ж��ն����Ƿ����29
			if((T->Year%4==0&&T->Year%100!=0)||(T->Year%400==0))
			{
				if (T->Day>29||T->Day<1)
					return 0;
			}
			//���������ж����Ƿ����28
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

		//���м�������
		T->JD=T->IntJD+T->DouJD;

	}
	
			else
				if(flag==2)				//��JD���㹫��,������С���ֿ����и߾��ȴ���
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
								//���ﻹ���жϣ����鷳��Ҫ��������·�
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