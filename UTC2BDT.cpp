#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string.h>
using namespace std;


int CalLeapSec( double UTC_JD);//�������ܣ���֪UTC���������ռ�������
int CalLeapSecINV(double TAI_JD);//�������ܣ���֪TAI���������ռ�������



///////////////////////////////////////////////////////////////////////////////////////////
//�������ƣ�UTC2BDT
//�������ܣ�UTCתBDT    (XIELING)
//����:
//       (1)UTC_JD ��UTC����������ʽ �� 
//       (2)SetLeapSec �����õ����룩
//���:
//       (1)BDT_JD ��BDTʱ����������ʽ��(BDT_JD>=2453736.5 )
//����ֵ :
//       (1)0 ����
//       (2)1`�\�����_
//      ��3��-9999 �����ļ�����
//����˵�������ô˺���ʱ���루m_LeapSec��������ڣ�2016����ǰ,��flag����1���������Դ��������ļ��õ�����;���������m_LeapSecΪ����ֵ��ȱʡֵΪ36��
//�����������õĺ����嵥��int CalLeapSec(UTC_JD)
//���ñ������ĺ����嵥����
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

	//����ʱ����ʼ��������2453736.5
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
//�������ƣ�BDT2UTC
//�������ܣ�BDT תUTC  xieling
//����:
//       (1)BDT_JD (BDTʱ����������ʽ)(BDT_JD>=2453736.5  )
//       (2)SetLeapSec �����õ����룩
//���:
//       (1)UTC_JD ��UTC����������ʽ)
//����ֵ :
//       (1)0 ����
//       (2)1`�\�����_
//      ��3��-9999 �����ļ�����
//����˵�������ô˺���ʱ���루m_LeapSec��������ڣ�2016����ǰ,��flag����1���������Դ��������ļ��õ�����;���������m_LeapSecΪ����ֵ��ȱʡֵΪ36��

//�����������õĺ����嵥��int CalLeapSecINV(TAI_JD)
//���ñ������ĺ����嵥����
///////////////////////////////////////////////////////////////////////////////////////////
int BDT2UTC(double BDT_JD,double *UTC_JD,int flag,int SetLeapSec=36)
{
	int m_LeapSec=SetLeapSec;
	//����ʱ����ʼ��������2453736.5
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
	return leapsec;

}