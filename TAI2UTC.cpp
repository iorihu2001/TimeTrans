
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include"BaseStruct.h"
using namespace std;

int CAL2UTC(UTCTime *T,int flag );//�������ܣ�UTC�ĸ������������ս���ת��
int CalLeapSec( double UTC_JD);//�������ܣ���֪UTC���������ռ�������
int CalLeapSecINV(double TAI_JD);//�������ܣ���֪TAI���������ռ�������


///////////////////////////////////////////////////////////////////////////////////////////
//�������ƣ�UTC2TAI
//�������ܣ�UTC��JDתTAI
//����:
//       (1)UTC_JD (UTC����������ʽ)(����2378861.5)
//       (2)SetLeapSec �����õ����룩
//���:
//       (1)TAI (TAI��������)(����2378861.5)
//����ֵ :
//       (1)0 ����
//       (2)1`�\�����_
//      ��3��-9999 �����ļ�����
//����˵�������루m_LeapSec��������ڣ�1972 ����ǰ������õ�;1972~2016��֮����flag����1ʱ���������Դ��������ļ��õ�����;���������m_LeapSecΪ����ֵ(ȱʡֵ��36��
//�����������õĺ����嵥��
//                    (1)CAL2UTC
//                    (2)int CalLeapSec(UTC_JD)
//���ñ������ĺ����嵥����
///////////////////////////////////////////////////////////////////////////////////////////
int UTC2TAI(double UTC_JD,double *TAI,int flag,int SetLeapSec=36)
{
	int m_LeapSec=SetLeapSec;
	//�㷨����ʱ����1801��1��1��
	if(UTC_JD<2378861.5)
	{
		return 0;
	}
	UTCTime timeof1999(1999,8,22,0,0,0.0,0.1);
	//�ոռӵ�
	UTCTime timeof1972(1972,1,1,0,0,0.0,0.1);
	CAL2UTC(&timeof1972,1);
	//1972����ǰֱ��������Ĺ�ʽ
	if(UTC_JD<timeof1972.JD)
	{
		UTCTime timeof1965(1965,12,31,0,0,0.0,0.1);
		CAL2UTC(&timeof1965,1);
		*TAI=UTC_JD+(4.21317+(UTC_JD-timeof1965.JD)*0.002592)/86400.0;
	}
	else
	{
		//flag����1��ʱ����1972~2016��֮��ͻ����CalLeapSecINV�������Դ��������ļ�
		if(UTC_JD<2457388.5&&flag==1)
		{
			m_LeapSec= CalLeapSec(  UTC_JD-2400000.5);
			if(m_LeapSec==-9999)
			{
				return -9999;
			}

		}
		//1972���Ժ�ֱ���������
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
//�������ƣ�TAI2UTC
//�������ܣ�TAIתUTC_JD
//����:
//       (1)TAI (TAI�������� )(����2378861.5)
//       (2)SetLeapSec �����õ����룩
//���:
//       (1)UTC_JD(UTC����������ʽ )(����2378861.5)
//����ֵ :
//       (1)0 ����
//       (2)1`�\�����_
//      ��3��-9999 �����ļ�����
//����˵�������루m_LeapSec��������ڣ�1972 ����ǰ������õ�;1972~2016��֮����flag����1ʱ���������Դ��������ļ��õ�����;���������m_LeapSecΪ����ֵ(ȱʡֵ��36��
//�����������õĺ����嵥��
//                    (1)CAL2UTC
//                    (2)int CalLeapSecINV(TAI_JD)
//���ñ������ĺ����嵥����
///////////////////////////////////////////////////////////////////////////////////////////
int TAI2UTC(double TAI,double *UTC_JD,int flag,int SetLeapSec=36)
{
	//�㷨����ʱ����1801��1��1��
	if(TAI<2378861.5)
	{
		return 0;
	}
	int m_LeapSec=SetLeapSec;
	//�ոռӵ�
	UTCTime timeof1972(1972,1,1,0,0,0.0,0.1);
	CAL2UTC(&timeof1972,1);
	//1972����ǰֱ��������Ĺ�ʽ
	if(TAI<timeof1972.JD)
	{
		UTCTime timeof1965(1965,12,31,0,0,0.0,0.1);
		CAL2UTC(&timeof1965,1);
		*UTC_JD=(TAI+(timeof1965.JD*0.002592-4.21317)/86400.0)/(1+0.002592/86400.0);
	}
	else 
	{
		//flag����1��ʱ����1972~2016��֮��ͻ����CalLeapSecINV�������Դ��������ļ�
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


///////////////////////////////////////////////////////////////////////////////////////////
//�������ܣ����㵱ǰUTCʱ���Ӧ������
//����:
//       (1)UTC_JD  (UTC�ļ���������)
//���:   
//       ��
//����ֵ:
//(1)�õ�������ֵ (�룩
//��2��-9999 �ļ�����
//����˵������
//�����������õĺ����嵥����
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
//�������ܣ����㵱ǰTAI_JDʱ���Ӧ������
//����:
//       (1)TAI_JD  (TAI�ļ���������)
//���:   
//      ��
//����ֵ:
// (1)�õ�������ֵ (�룩
// ��2��-9999 �ļ�����
//����˵������
//�����������õĺ����嵥����
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

