/*Lu���Դʷ������� �պ��� 11.12.30
������ͨ�����룬��δ�Ż�
û�ε�����ȷ����һ�����ϴʷ�����ĵ���
Դ�ļ�������.lu�ļ�
֧������������壨��֧��int���ͣ��������Ӽ��˳�������֧��,֧�ֵ���ע��֧��()
����չ��֧��N���ؼ��֣�������ʶ�����֧��256���ַ���֧���������չ��֧�ֺ�������չ
int a=b+c;
int a=100;
//�պ���
*/

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include"MyHeader.h"

static char acScanBufL[SBUFSIZE]={0};//ɨ�軺����
static char acScanBufR[SBUFSIZE]={0};
static long lnOffset=0;//ƫ��������¼��ǰ�ļ�ָ����뿪ͷ�ľ���

char CheckChar(FILE * pfSubSourceFile,char ** ppcSubCurrent,char cSubTemp,char fcSubBlank)//����ַ����ܣ����кϲ��ո�ȥ���س�
{
	if(cSubTemp==EOF) 
	{
		return 0x20;
	}//�ж��Ƿ��ļ�ĩβ���������ؿո�

	if(cSubTemp!=10&&isspace(cSubTemp))//�ж��Ƿ�Ϊ�ո�,�ϲ�����ո�Ϊһ��
	{
		if(fcSubBlank=='N') 
		{
			**ppcSubCurrent=0x20;
			(*ppcSubCurrent)++;
			fcSubBlank='Y';//�޸Ŀո��־λ
		}
		cSubTemp=fgetc(pfSubSourceFile);//�ո�Ļ��ٶ���һ���ַ�
		cSubTemp=CheckChar(pfSubSourceFile,ppcSubCurrent,cSubTemp,fcSubBlank);//�ݹ���
	}

	if(cSubTemp=='/')//�ж�ע��
	{
		char cTemp;//��ʱ���������ڼ����һ���Ƿ�Ϊ'/'

		cTemp=fgetc(pfSubSourceFile);//�ٶ���һ���ַ�

		if(cTemp=='/') //��Ϊע��һֱ����֪�����з��������˻ظղŶ�����ַ�
		{
			while(fgetc(pfSubSourceFile)!='\n');
			//����ע�ӣ���ע�ӽ�β���ؿո�
			cSubTemp='\n';
		}
		else ungetc(cTemp,pfSubSourceFile);//�˻ظղŶ�����ַ�

	}

	return cSubTemp;//�����ַ�
}

int PreProcess(char *pcSubName)//Ԥ�����ӳ�����ɹ���ÿ����ScanBuffer��װ��̶��ֳ���Դ�������
{
	static char fcFlag='L';
	int i;

	//��Դ�����ж����޳��ո�ע�ӵȷŵ�buffer
	char * pcCurrent=0;//ֻ�ǵ�ǰҪ��ֵ���ֽ�
	char ** ppcCurrent=&pcCurrent;//ָ��ָ���ָ��
	char * pcStart;//ָ������Ŀ�ʼ������ƫ������
	char * pcTemp;//��ʱ��������ʼ����
	FILE * pfSourceFile;//ָ��Ҫ�򿪵�Դ�����ļ�

	//��ʼ��pcCurrentȷ�ϵ�ǰҪװ��Ļ�����
	if(fcFlag=='L') 
	{
		pcCurrent=acScanBufL;
		pcStart=acScanBufL;
	}
	else
	{
		pcCurrent=acScanBufR;
		pcStart=acScanBufR;
	}

	//��ʼ����ǰ������Ϊ���ַ�
	pcTemp=pcCurrent;
	for(i=0;i<SBUFSIZE;i++)
	{
		*pcTemp=0;
		pcTemp++;
	}

	//���ļ�
	pfSourceFile=fopen("test.txt","r");
	if(pfSourceFile==NULL) 
	{
		printf("The file %s was not opened\n",pcSubName);//�ж��ļ����Ƿ�ɹ�
		exit(0);//װ��ʧ���˳�
	}
	else//�򿪳ɹ�����
	{
		if(fseek(pfSourceFile,lnOffset,SEEK_SET))//�ƶ��ļ�ָ�뵽Ӧ�õ�λ��
		{
			perror("Fseek failed");
			exit(1);//�ƶ����ʧ���˳�
		}

		while((pcCurrent-pcStart)!=SBUFSIZE)//ѭ������ָ�������ַ�
		{
			char cTemp;//��ʱ����
			cTemp=fgetc(pfSourceFile);//����һ���ַ�
			cTemp=CheckChar(pfSourceFile,ppcCurrent,cTemp,'N');//��ȡһ���Ϸ����ַ�

			if(cTemp==0x20)
			{
				*pcCurrent=cTemp;
				pcCurrent++;
				*pcCurrent='#';//�������
				break;//�ж��Ƿ��ļ�ĩβ
			}

			*pcCurrent=cTemp;//���ղ�����Ĳ�Ϊ�ո�Ҳû���������뵽������
			pcCurrent++;
		
		}	

		//�޸�ƫ����Ϊ��ǰƫ������Ϊ�´ζ�����
		lnOffset=ftell(pfSourceFile);
		
		//�ر��ļ�
		fclose(pfSourceFile);

		//�޸�fcFlagΪ�´��ٴ�װ�룬���Ļ�����
		if(fcFlag=='L') fcFlag='R';
		else fcFlag='L';
	}

	return 3;
}

int CheckOperate(char cCheck)//����Ƿ������������������������е�ƫ����
{
	int i;
	for(i=0;i<ARROPELEN;i++)
	{
		if(cCheck==apcOperator[i]) return i;//����������������е�ƫ����		
	}
	return 0;//���������
}

int CheckNewLine(char cCheck,int *piSubLine)//����ַ��Ƿ�Ϊ�س�
{
	if(cCheck=='\n') 
	{
		(*piSubLine)++;
		return 1;
	}
	return 0;
}

int CheckKeyWords(char acCheckWords[])//��鵥���Ƿ�Ϊ�ؼ������Ƿ����ڹؼ����б��е��±꣬���Ƿ���0
{
	int i;
	for(i=0;i<ARRKEYLEN;i++)
	{
		if(!strcmp(acCheckWords, apcKeyWords[i])) return i;
	}
	return 0;
}

int Error(int iSubLine)//����������buffer�ʿ�
{
	//��ӡ������Ϣ
	printf("��%d��",iSubLine);

	lnOffset=0;//��ʼ���ļ�ƫ����
	return 0;
}

int CheckEndBuffer(char **ppcCheckPoint,char *pcSubName)//����Ƿ�Խ��,���Զ��л���������bufferLԽ�緵��1��bufferRԽ�緵��2��û��Խ�緵��0
{
	static char fcBuffer ='L';//��ʼ����һ��ɨ��bufferl
	int iTempOffset;//��ʱƫ��������
	fcBuffer=='L'?(iTempOffset=*ppcCheckPoint-acScanBufL):(iTempOffset=*ppcCheckPoint-acScanBufR);//����ƫ����
	if(iTempOffset>=SBUFSIZE-1)//Խ��
	{
		if(fcBuffer=='L')
		{
			PreProcess(pcSubName);//װ��Buffer
			*ppcCheckPoint=acScanBufR;//�޸ĵ�ǰ��ָʾ������һ���������Ŀ�ʼ
			fcBuffer='R';//�޸�buffer��־����ǰָ����bufferR��
			return 1;//bufferLԽ�緵��1
		}
		else
		{
			PreProcess(pcSubName);//װ��Buffer
			*ppcCheckPoint=acScanBufL;//�޸ĵ�ǰ��ָʾ������һ���������Ŀ�ʼ
			fcBuffer='L';//�޸�buffer��־����ǰָ����bufferL��
			return 2;//bufferRԽ�緵��2
		}
	}
	return 0;//û��Խ�緵��0
}

struct sBinaryRelation LexicalAnalyzer(char *pcSubName)//�ʷ�������
{
	static char fcFirst='Y';
	//��ʼ��ɨ��ָʾ��Ϊ��һ��Ԫ��
	static char *fpcStart;
	char **fppcStart=&fpcStart;//ȡfpcstart�ĵ�ַ
	static char *fpcSerching;
	char **fppcfpcSerching=&fpcSerching;//ȡfpcSerching�ĵ�ַ
	static int iLine=1;//��¼��ǰ������
	int *piLine=&iLine;//ָ��������ָ��
	struct sBinaryRelation sTempResult;//��ŷ��ؽ��

	if(fcFirst=='Y')//��һ�ε���װ��bufferl
	{
		PreProcess(pcSubName);
		fcFirst='N';
		fpcStart=acScanBufL;//��start��serch��ָ���һ����ַ
	}

	while(*fpcStart==0x20) 
	{
		if(!CheckEndBuffer(fppcStart,pcSubName)) fpcStart++;//����Ƿ�Խ��,��ûԽ�罫startָ��ָ��һ����Ϊ�ո���ַ�
	}
	fpcSerching=fpcStart;//serchingָ��start
	//fpcSerching++;//serchingָ��start����һ��λ��

	while(*fpcSerching!=0x00)
	{
		if(CheckNewLine(*fpcStart,piLine))//���س�����¼������һ
		{
			//�������Ƿ�\nҲ������ʶ��
			fpcStart++;
			fpcSerching=fpcStart;//serchingָ��start
		}
		else if(isalpha(*fpcStart))//��һ���ַ�����ĸ��Ϊ�ؼ���
		{
			char cTempResult[SBUFSIZE]={0};
			char *pcCurrent=cTempResult;

			//����Ƿ�Ϊ������ؼ���
			while(isalnum(*fpcSerching))//��һ�����ַ�������
			{
				//if(isspace(*fpcSerching)) break;//��Ϊ�ո����

				*(pcCurrent++)=*fpcSerching;//���ַ����浽��ʱ���飬����ָ�����

				if(!CheckEndBuffer(fppcfpcSerching,pcSubName)) fpcSerching++; //����Ƿ�Խ��,��ûԽ�罫serching++
			}//ѭ�����ʱfpceSerchingָ��ǰ��ʶ������һ��λ��	
			//��start��serching�غ�
			fpcStart=fpcSerching;

			//���ص�ǰ�ı�ʶ���ŵ��ṹ����
			int KeyWordScript;//�ؼ��ֵ��±�
			if((KeyWordScript=CheckKeyWords(cTempResult))!=0)//����Ƿ�Ϊ�ؼ��ֻ�����ͨ����
			{
				sTempResult.iId=0;//��־����Ϊ0���ؼ��ֵ��±�
				sTempResult.iSubScript=KeyWordScript;
				sTempResult.acTempValName[0]='\0';//��ֵ
			}
			else//��ͨ����
			{
				sTempResult.iId=1;//����Ϊ����
				int i=-1;
				while(cTempResult[++i]!='\0')//��������ֵ����������������
				{
					sTempResult.acTempValName[i]=cTempResult[i];
				}
				sTempResult.acTempValName[i]=cTempResult[i];//��ֵ��'\0'
				sTempResult.iSubScript=0;//��ֵ
			}
			//���ؽṹ��
			
			return sTempResult;
		}
		else if(isdigit(*fpcStart))//��������ֳ���
		{
			int iTemp=0;
			while(*fpcSerching!=0x20)//�����ַ���Ҫ������
			{
				if((!isdigit(*fpcSerching))&&(isalpha(*fpcSerching)))//����������Ļ���߲��ǲ�����
				{
					printf("�ʷ�������");
					Error(iLine);//�������ֱ���
					printf(" %d %c�Ƿ��ַ�\n",iTemp,*fpcSerching);
				}
				iTemp=iTemp*10+int(*fpcSerching-0x30);//�������ַ�ת��Ϊ����
				if(!CheckEndBuffer(fppcfpcSerching,pcSubName)) fpcSerching++; //����Ƿ�Խ��,��ûԽ�罫serching++
				//�ж��Ƿ��ǣ�����һ���ַ��ǷֺŻ��߲������ͽ���
				if(*fpcSerching==';') break;
				if(CheckOperate(*fpcSerching)) break;
			}
			//ѭ�����ʱfpceSerchingָ��ǰ��ʶ������һ��λ��
			fpcStart=fpcSerching;//��start��serching�غ�

			//���ص�ǰ�ı�ʶ���ŵ��ṹ����
			sTempResult.iId=2;
			sTempResult.iSubScript=iTemp;
			sTempResult.acTempValName[0]='\0';//��ֵ
			return sTempResult;

		}
		else if(CheckOperate(*fpcStart)) //�ж������
		{
			int iTempOperateOffset;//��¼��ʶ���±�
			iTempOperateOffset=CheckOperate(*fpcStart);
			//����Ҫ��Ϊserching��������start����һ��fpceSerching++;
			if(!CheckEndBuffer(fppcStart,pcSubName)) fpcStart=++fpcSerching;//���serching�Ƿ�Խ��,��ûԽ�罫start��serching�غ�
			
			//���ص�ǰ�ı�ʶ���ŵ��ṹ����
			sTempResult.iId=3;
			sTempResult.iSubScript=iTempOperateOffset;
			sTempResult.acTempValName[0]='\0';//��ֵ
			return sTempResult;

		}
		else
		{
			printf("�ʷ�������");
			Error(iLine);//�������ֱ���
			printf(" %c�Ƿ��ַ�\n",*fpcSerching);
			exit(1);//�˳�����
		}
				 
	}
	if(*fpcSerching!=0) printf("�ʷ�������δ֪����\n");//����Ϊ����������ʾδ֪����
	exit(1);//ʧ�ܷ��ؿ�
}

int DebugLexicalAnalyzer(char *pcSubName)
{
	struct sBinaryRelation sDebugResult;//��ŷ��ؽ��
	int i;
	for(i=0;i<500;i++)
	{
		sDebugResult=LexicalAnalyzer(pcSubName);
		printf("%d  %d %d %s\n",i,sDebugResult.iId,sDebugResult.iSubScript,sDebugResult.acTempValName);
	}
	return 0;
}