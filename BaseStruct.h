//UTC形式的时间结构
struct UTCTime			    
{
	int Year;
	int Mon;
	int Day;
	int Hour;
	int Min;
	double Sec;

	double JD;
	double IntJD;	  //JD的小数部分
	double DouJD;    //JD的整数部分   
	
	UTCTime()
	{
		//默认值为北斗0时，公历2006年1月1日0时0分0秒  2013.1.1 0:0:0 JD:2456293.5
		Year=2006;
		Mon=1;
		Day=1;
		Hour= Min= 0; 
		Sec= 0.0;
		JD=2453736.5;
		IntJD=2453736.0;
		DouJD=0.5;

	};
	UTCTime(long Iy,long Im,long Id,long Ih,long Imin,double Is,double iJD)
	{
		Year=Iy;
		Mon=Im; 
		Day=Id; 
		Hour=Ih; 
		Min=Imin;
		Sec=Is; 
		JD=iJD;

		IntJD=(long)JD;
		DouJD=JD-IntJD;
	};
};