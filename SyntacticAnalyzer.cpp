#include"MyHeader.h"
#include<stdio.h>
#include<string.h>

#define GRAMMARLEN 14//�����ʷ�����
#define FIRSTTABLELEN 12//�������ȱ���
#define VTLEN 10//����vp���ĳ���
#define STACLSIZE 100//��һ��ջ��С
#define NULL {1000,1000,"end"}


extern struct sBinaryRelation LexicalAnalyzer(char *pcSubName);//����lexicalanalyzer�еĺ���

struct sQuarternaryForm //��Ԫʽ����
{
	int iOperateScript;//���������±�
	struct sBinaryRelation sArgLeft;
	struct sBinaryRelation sArgRight;
	struct sBinaryRelation sArgResult;
	//next;//ָ����һ��
};

struct sVp//vt���Ľṹ��
{
	struct sBinaryRelation sHead;//��ʶ��
	struct sBinaryRelation asVt[VTLEN];//vp��
};

struct sStack//ջ�ṹ��
{
	char cType;
	struct sBinaryRelation sPlace;
};

static struct sBinaryRelation aasQuarternaryForm[GRAMMARLEN][10]={
	{{4,0,"start"},{4,0,"define"},{3,8,""},{4,0,"operate"},NULL},//���ս��start��ʼ����ȫ����ᵽstart����Ŀ�ʼ����start->define;operate;
	{{4,0,"type"},{5,0,"special"},{4,0,"name"},NULL},//define->type special name
	{{4,0,"type"},{0,0,""},NULL},//type->key
	{{4,0,"name"},{1,0,""},NULL},//name->var
	{{4,0,"operate"},{1,0,""},{3,1,""},{4,0,"operater"},{3,8,""},{4,0,"operate"},NULL},//operate->var=operater;operate
	{{4,0,"operate"},{1,0,""},{3,1,""},{4,0,"operater"},NULL},//operate->var=operater
	{{4,0,"operater"},{4,0,"left"},{3,2,""},{4,0,"right"},NULL},//operater->left+right
	{{4,0,"operater"},{4,0,"left"},NULL},//operater->left
	{{4,0,"operater"},{4,0,"left"},{3,3,""},{4,0,"right"},NULL},//operater->left-right
	{{4,0,"left"},{4,0,"left"},{3,4,""},{4,0,"other"},NULL},//left->left*other
	{{4,0,"left"},{4,0,"left"},{3,5,""},{4,0,"other"},NULL},//left->left/other
	{{4,0,"left"},{4,0,"other"},NULL},//left->other
	{{4,0,"other"},{4,0,"name"},NULL},//other->name
	{{4,0,"other"},{2,0,""},NULL}//other->const
};//�ʷ��������

static struct sBinaryRelation asFirstTableIndex[FIRSTTABLELEN]={
	NULL,{3,1,"="},{3,2,"+"},{3,3,"-"},{3,4,"*"},{3,5,"/"},{3,8,";"},{0,0,"key"},{1,0,"var"},{2,0,"const"},{5,0,"spe"},{3,9,"#"}
};//�������ȱ��������������±꼴Ϊ�����ȱ��е��±�

static char acFirstTable[FIRSTTABLELEN][FIRSTTABLELEN]={
	{'0','=','+','-','*','/',';','0','1','2','5','3'},//
	{'=',0},// =
	{'+',0},// +
	{'-',0},// -
	{'*',0},// *
	{'/',0},// /
	{';',0},// ;
	{'0',0},// key
	{'1',0},// var
	{'2',0},// const
	{'5',0},// spe
	{'3',0},// #
};//��7 8���var��const�����ȼ�

char RequestVt(struct sVp asVt[],int iIndex,int iVtIndex,char fcVt)//����Ҫ���vt����
{
	asVt[iIndex].sHead=aasQuarternaryForm[iIndex][0];//��ֵ����˭��vtֵ

	if(fcVt=='F')//��firstvt
	{
		if (aasQuarternaryForm[iIndex][1].iId==4)//Q...
		{
			if(aasQuarternaryForm[iIndex][2]!=NULL)//Qa
			{
				asVt[iIndex].asVt[iVtIndex]=aasQuarternaryForm[iIndex][2];//��a��
				RequestVt(asVt,iIndex,iVtIndex+1,'F');//�ݹ������F��q��
			}
			else//Q
			{
				RequestVt(asVt,iIndex,iVtIndex,'F');//�ݹ������F��q��
			}
		}
		else//a....
		{
			asVt[iIndex].asVt[iVtIndex]=aasQuarternaryForm[iIndex][1];//��ֵ
			asVt[iIndex].asVt[iVtIndex+1]=NULL;//����һ�ֵΪ��
		}
		return 'F';
	}
	else//��lastvt
	{
		int i=0;
		while(aasQuarternaryForm[iIndex][i+1]!=NUll) i++;//������i=....Q���±�

		if(aasQuarternaryForm[iIndex][i].iId==4)//...Q
		{
			if(i!=1)//...aQ
			{
				asVt[iIndex].asVt[iVtIndex]=aasQuarternaryForm[iIndex][i-1];//��ֵa
				RequestVt(asVt,iIndex,iVtIndex+1,'L');//��lastvt
			}
			else//...Q
			{
				RequestVt(asVt,iIndex,iVtIndex,'L');//��lastvt
			}
		}
		else//.......a
		{
			asVt[iIndex].asVt[iVtIndex]=aasQuarternaryForm[iIndex][i];//��ֵa
			asVt[iIndex].asVt[iVtIndex+1]=NULL;//��ֵNULL
		}
		return 'L';
	}
	return 'E';//����
}

int RequestFirstTableScript(static struct sBinaryRelation RequestScript)//�����ս����firsttable�е��±�
{
	int i;
	for(i=1;i<FIRSTTABLELEN;i++)//ѭ������firsttableindex���е�ÿһ��
	{
		if(asFirstTableIndex[i].iId==RequestScript.iId&&asFirstTableIndex[i].iSubScript==RequestScript.iSubScript) return i;
	}
	return 0;//û�ҵ�ʧ��
}

int MyRequestQuarternaryFormScript(static struct sBinaryRelation RequestScript)//���ط��ս�����ķ����е�ƫ����
{
	int i;
	for(i=0;i<GRAMMARLEN;i++)//ѭ�������ķ���
	{
		if(asFirstTableIndex[0][i]==RequestScript) return i;
	}
	return -1;//����
}

int MyRequestVtLength(struct sVp RequestVtItem)//����vt����һ��ĳ���
{
	int iLength=0;

	while(RequestVtItem.asVt[iLength]!=NULL) iLength++;

	return iLength;//����vt�ĳ���
}

int CheckGrammar(char CheckChar,int iRowScript,int iLineScript)//������ȱ��Ƿ񸴹�ֵ�Ӷ��ж��ʷ��Ƿ���ȷ����δ��ֵ����1����ֵ�˳�
{
	if(CheckChar==0) return 1;
	printf("���ȱ���󣬷���������ķ� %s %s ���й�ϵ�����������޹�ϵ\n",asFirstTableIndex[iRowScript].acTempValName,asFirstTableIndex[iLineScript].acTempValName,acFirstTable[iRowScript][iLineScript]);
	exit(0);//�˳�
	return 0;
}

int IniFirstTable()//��ʼ��firsttable���ȱ�,ÿ�ι�Լǰ�����ʼ��
{
	struct sVp asFirstVt[GRAMMARLEN];//firstvt�ļ��ϣ�ÿ�����һ����ʼ�����±���ķ������
	struct sVp asLastVt[GRAMMARLEN];//lastvt�ļ���
	int i;

	for(i=1;i<FIRSTTABLELEN;i++)//��ʼ��firsttable���е�#
	{
		acFirstTable[i][FIRSTTABLELEN-1]='>';
		acFirstTable[FIRSTTABLELEN-1][i]='<';
	}

	acFirstTable[FIRSTTABLELEN-1][FIRSTTABLELEN-1]='=';//#��#�Ĺ�ϵΪ����

	for(i=0;i<GRAMMARLEN;i++)//ѭ����firstvt��lastvt
	{
		RequestVt(asFirstVt,i,0,'F');//��firstvt
		RequestVt(asLastVt,i,0,'L');//��lastvt
	}

	for(i=0;i<GRAMMARLEN;i++)//ѭ�������ȱ�
	{
		int iGrammarItemLen=0;//��¼ÿ������
		int j;
		while(aasQuarternaryForm[i][iGrammarItemLen]!=NULL) iGrammarItemLen++;//��ĵ�ǰ����ĳ���

		for(j=0;j<iGrammarItemLen;j++)//ѭ����
		{
			if(aasQuarternaryForm[i][j].iId!=4&&aasQuarternaryForm[i][j+1].iId!=4)//j��j+1��Ϊ�ս��j==j+1
			{
				int ij;//j���±�
				int ij1;//j+1���±�

				ij=RequestFirstTableScript(aasQuarternaryForm[i][j]);
				ij1=RequestFirstTableScript(aasQuarternaryForm[i][j+1]);

				if(CheckGrammar(acFirstTable[ij][ij1],ij,ij1)) acFirstTable[ij][ij1]='=';//���޹�ϵ���
			}
			
			if((j<iGrammarItemLen-1)&&(aasQuarternaryForm[i][j].iId!=4&&aasQuarternaryForm[i][j+2].iId!=4))//xj��xj+2��Ϊ�ս��
			{
				if(aasQuarternaryForm[i][j+1]==4)//xj+1ΪΪ���ս��
				{
					int ij;//j���±�
					int ij2;//j+1���±�

					ij=RequestFirstTableScript(aasQuarternaryForm[i][j]);
					ij2=RequestFirstTableScript(aasQuarternaryForm[i][j+2]);

					if(CheckGrammar(acFirstTable[ij][ij2],ij,ij2)) acFirstTable[ij][ij2]='=';//���޹�ϵ���
				}
			}

			if(aasQuarternaryForm[i][j].iId!=4&&aasQuarternaryForm[i][j+1].iId==4)//xjΪ�ս��xj+1Ϊ���ս��
			{
				int iVtIdenx;//���ս�����±�
				int iVtLength;//���ս����firstvt�ĳ���
				int i;

				iIdenx=MyRequestQuarternaryFormScript(aasQuarternaryForm[i][j+1]);//���ط��ս�������ʷ����е�ƫ����Ҳ������vt�е�ƫ����
				iVtLength=MyRequestVtLength(asFirstVt[iIdenx]);

				for(i=0;i<iVtLength;i++)//firstvt(xj+1)��ÿ���������ȼ�����xj
				{
					int ij;//j���±�
					int ij1;//j+1���±�
					ij=RequestFirstTableScript(aasQuarternaryForm[i][j]);//����xj�ս�������ȱ��е��±�
					ij1=RequestFirstTableScript(asFirstVt[iIdenx].asVt[i]);//����xj+1���ս����first�е�ÿ�������ȱ��е��±�
					if(CheckGrammar(acFirstTable[ij][ij1],ij,ij1))//�ж��Ƿ����ȹ�ϵ�Ѿ�����
					{
						acFirstTable[ij][ij1]='<';//���ȹ�ϵС��
					}
				}
			}

			if(aasQuarternaryForm[i][j].iId==4&&aasQuarternaryForm[i][j+1].iId!=4)//xjΪ���ս����xj+1Ϊ�ս��
			{
				int iVtIdenx;
				int iVtLength;
				int i;
				iIdenx=MyRequestQuarternaryFormScript(aasQuarternaryForm[i][j]);//���ط��ս�������ʷ����е�ƫ����Ҳ������vt�е�ƫ����
				iVtLength=MyRequestVtLength(asLastVt[iIdenx]);

				for(i=0;i<iVtLength;i++)//firstvt(xj+1)��ÿ���������ȼ�����xj
				{
					int ij;//j���±�
					int ij1;//j+1���±�
					ij=RequestFirstTableScript(asLastVt[iIdenx].asVt[i]);//����xj�ս�������ȱ��е��±�
					ij1=RequestFirstTableScript(aasQuarternaryForm[i][j+1]);//����xj+1���ս����first�е�ÿ�������ȱ��е��±�
					if(CheckGrammar(acFirstTable[ij][ij1],ij,ij1))//�ж��Ƿ����ȹ�ϵ�Ѿ�����
					{
						acFirstTable[ij][ij1]='>';//���ȹ�ϵС��
					}
				}
			}
		}
	}
	return 1;//��ʼ���ɹ�
}

char CheckTerminalPriority(struct sBinaryRelation sLeft,struct sBinaryRelation sRight)//���ز��������޹�ϵ< > = ��Ϊ��������E
{
	int iLeftOfFirstScript;
	int iRightOfFirstScript;
	iLeftOfFirstScript=RequestFirstTableScript(sLeft);
	iRightOfFirstScript=RequestFirstTableScript(sRight);
	if(acFirstTable[iLeftOfFirstScript][iRightOfFirstScript]=='<') return '<';
	if(acFirstTable[iLeftOfFirstScript][iRightOfFirstScript]=='>') return '>';
	if(acFirstTable[iLeftOfFirstScript][iRightOfFirstScript]=='=') return '=';
	return 'E';//���ش����־
}

int FindStackTopTerminal(struct sStack asSubStack[],int iSubStackTop,int iSubStackBottom)//����ջ�����ս��������ֵΪջ���ս�����±�
{
	int i;
	for(i=iSubStackTop;i>=iSubStackBottom;i--)//��ջ����ʼ��ջ����
	{
		if(asSubStack[i].cType=='T') return i;
	}
	printf("�﷨������δ֪������Ѱջ���ս������");
	exit(0);
	return -1;
}

int FindTermHead(struct sStack asSubStack[],int iSubTermTail,int iSubTempHead)//���Լ�еĹ�Լͷ��ջ�±�
{
	int iTempTermHead;//��ʱ�������termhead
	char cTempCheck;//��ʱ����ս�������ȹ�ϵ

	iTempTermHead=FindStackTopTerminal(asSubStack,iSubTempHead,0);
	cTempCheck=CheckTerminalPriority(asSubStack[iSubTermTail],asSubStack[iTempTermHead]);//��ĵ�ǰ�ս����ջ���ս�������ȹ�ϵ
	iSubTempHead=iTempTermHead-1;

	if(cTempCheck=='<')//����ϵΪС��
	{
		iTempTermHead=FindTermHead(asSubStack,iSubTermTail,iSubTempHead);//�ݹ�����һ���ս��
	}
	return iTempTermHead;
}

int MyFindGrammarTableHead(struct sStack asSubStack[],int iSubTermHead,int iSubTermTail)//���ҿɹ�Լ��������ֵС��grammar����ҳɹ���������ʧ�ܣ��ɹ�iΪ�ķ����ķ����е��±�
{
	int i;
	for(i=0;i<GRAMMARLEN;i++)//ѭ��ɨ�������ķ���
	{
		if(aasQuarternaryForm[i][1]==asSubStack[iSubTermHead])//�ҵ��ɹ�Լͷ��
		{
			int j;
			int k=2;
			char cFlag='Y';
			for(j=iSubTermHead+1;j<=iSubTermTail;j++,k++)//ѭ�����Һ���Ƿ����
			{
				if(aasQuarternaryForm[i][k]==NULL)//���ķ����������������
				{
					cFlag='N';
					break;	
				}
				if(asSubStack[j].cType=='N'&&aasQuarternaryForm[i][k].iId==4) continue;//����ǰ����Ϊ���ս���ҵ�ǰ�ķ������ķ�����ҲΪ���ս���Ź�Լ
				if(asSubStack[j].sPlace==aasQuarternaryForm[i][k]) continue //��Ϊ�ս���������
				
				//ִ�е����뱾���ķ������
				cFlag='N';
				break;	
			}
			if(cFlag=='Y') return i;//�ҵ���iΪ�±�
		}
	}
	return i;//ʧ�ܷ���grammar
}

int SyntacticAnalyzer(char *pcSubName)//�﷨������
{
	struct sStack asStack[STACLSIZE]={NULL};
	struct sStack sTempItem=NULL;//��ʱ��ſɹ�Լ���ַ�

	int iStackTop=0;//ջ��
	int iStackBottom=0;//ջ��
	int iStackTopTerminal;//ջ�����ս��
	//int iStackNearTopTerminal=iStackBottom;//����ջ���ķ��ս��
	//int iTopTerminalInFirstScript;//ջ�����ս����firsttable�е��±�
	//int iOutTerminalInFirstScript;//ջ���ս����firsttable�е��±�

	//��ʼ������ջջ���и�#
	asStack[0].cType='T';//�ս��
	asStack[0].sPlace.iId=3;//Ϊ�����
	asStack[0].sPlace.iSubScript=9;//#
	//iTopTerminalInFirstScript=FIRSTTABLELEN-1;//��ʼ��topΪ#
	
	if(IniFirstTable())//�����ʼ�����ȹ�ϵ��ɹ�
	{
		while(1)//��Լ
		{
			if(sTempItem==NULL)//�ж�sTempItem�Ƿ�Ϊ��
			{
				struct sBinaryRelation sTemp;
				sTemp=LexicalAnalyzer(char *pcSubName);//�Ӵʷ���������һ���ַ�

				if(sTemp.iId==4)//���Ƿ��ս��
				{
					sTempItem.cType='N';
				}
				else//���ս��
				{
					sTempItem.cType='T';
				}
				sTempItem.sPlace=sTemp;
			}
			//�ж��Ƿ��վ���ǹ�Լ����ι�Լ��ջ
			//RequestFirstTableScript(static struct sBinaryRelation RequestScript)
			char cTemp;
			iStackTopTerminal=FindStackTopTerminal(asStack,iStackTop,iStackBottom);
			cTemp=CheckTerminalPriority(asStack[iStackTopTerminal].sPlace,sTempItem.sPlace);
			//�ж�cTempΪ����С�ڻ��ǵ��ڻ��
			if(cTemp=='<'||cTemp=='=')//��ջ���ս�����ȼ�С�ڻ����ջ���ս����վ
			{
				iStackTop++;//ջ��ָ��++
				asStack[iStackTop]=sTempItem;//��ջ
				if(asStack[iStackTop].sPlace.iId==3&&asStack[iStackTop].sPlace.iSubScript==9) exit();//�������
				//��ʼ��stempΪ�´���׼��
				sTempItem=NULL;
			}
			else if(cTemp=='>')//��Լ
			{
				//��Լ����
				int iTermTail;//��Ź�Լ��β
				int iTermHead;//��Ź�Լ��ͷ
				//��ʼ��Լ
			
				iTermTail=FindStackTopTerminal(asStack,iStackTop,iStackBottom);//�ҵ�ջ�����׸����ս��
				iTermHead=FindTermHead(asSubStack,iTermTail,iTermTail-1)+1;//�ҵ����޹�ϵС�ڵ���ջ���ս�����±�
				iTermTail=iStackTop;//���¸�ֵ��ԼβΪջ��
				//��Լ
				if(iTermHead==iTermTail)//����Լvar����ֻ��һ�����ŵ�
				{
					//�����ķ����ܷ��Լ
					int i;
					char cFlag='N';
					for(i=0;i<GRAMMARLEN;i++)//ѭ��ɨ�������ķ���
					{
						if(aasQuarternaryForm[i][1]==asStack[iTermHead].sPlace)//�ҵ��ɹ�Լͷ��
						{
							cFlag='Y';//����i=�ķ�����+1��ʱiΪ��־λ��ѭ�����i=grammarʱ���Թ�Լ
							break;		
						}
					}
					if(cFlag=='Y')//���Թ�Լ
					{
						asStack[iTermHead].cType='N';//��ԼΪ���ս��
					}
				}
				else//����һ���Լa=b+c��������
				{
					//ѭ�������ķ����ҵ���Լ�ص�
					int iTemp;
					iTemp=MyFindGrammarTableHead(asStack,iTermHead,iTermTail);//�����ķ�����ҿɹ�Լ�ķ�
					if(iTemp<GRAMMARLEN)//��Լ�ɹ�
					{
						//��Լ�ɹ�
						//���ú���֪��������Ԫʽ

						//������ջ
						asStack[iTermHead].cType='T';
						asStack[iTermHead].sPlace=aasQuarternaryForm[iTemp][0];

						iStackTop=iTermHead;//������ջ
					}
					else
					{
						//��Լʧ�ܴ�ӡ������Ϣ
						printf("��Լ������δ���ķ����в鵽��Լ��");
					}
				}
			}
			else//���ȹ�ϵΪ�մ���
			{
				//����
				printf("��Լ���������޹�ϵ����");
			}
		}
	}

	return 0;
}