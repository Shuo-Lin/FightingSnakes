#ifndef MYTIMER
#define MYTIMER

#include<time.h>
//ѭ����ǰ�涨��const double start=clock();Ȼ������Ϊ��һ������
bool timer(const double & start,const double& length)
{
	if (((double)clock() - start) / CLK_TCK >= length)
		return false;
	else
		return true;
}



class ctimer
{
	const double start;
	const double length;

public:
	ctimer(const double& len) :start(clock()), length(len){}//����ʱʱ��
	bool blast()
	{
		if ((clock() - start)/CLK_TCK >= length)
			return false;
		else
			return true;
	}
};

#endif
