#pragma once 

#define SBUFSIZE 256//����ɨ�軺�����Ĵ�С
#define ARROPELEN 9//�������������ĳ���
#define ARRKEYLEN 2//����ؼ�������ĳ���


struct sBinaryRelation//����ֵ��Ϊ�ؼ��ֻ����
{
	int iId;//������ʲô
	int iSubScript;//��Ų������͹ؼ��ֵ�ƫ���������ǳ�������Ϊ����������ֵ
	char acTempValName[SBUFSIZE];//��ű�����ֵ
};

char * apcKeyWords[]={"","int"};
char apcOperator[]={' ','=','+','-','*','/','(',')',';','#'};
char * apcWordList[]={"key","var","const","operate","nonTerminalSymbol","special"};