#include<stnlib\stnstatementid.h>
#define SET_STATEMENT_INFO(stnid,yylineno) Plugin.ShmPtr->TestPlanProgram.Status.line = yylineno;\
Plugin.ShmPtr->TestPlanProgram.Status.Last_TestStatementID[0] = stnid;
#ifdef _MSC_VER
#pragma warning (disable:4716)
#pragma warning (disable:4166)
#pragma warning (disable:4430)
#include<uti/value_vc.h>
#include <winsock2.h>
#endif
#include <uti/stnplan.h>
#define __RN8213_1S_3380D_0002_B_Version_BODY_FILE
#include "reslib.h"
#include <awilib/awilib.h>
#ifndef _MSC_VER
#pragma warn -8057
#pragma warn -8066
#pragma warn -8067
#pragma warn -8065
#pragma warn -8004
#pragma warn -8008
#pragma warn -8070
#endif
bool power_down_fail_site_same_flag = false;
bool enter_power_down_fail_site = false;
const int parall_pin_cnt= DF_MAX_Pin_Count;
parall_struct parall_pin[parall_pin_cnt];
 long int GetAddr(char *address) {          
 char *tmplabelptr = NULL,tmppatfile[MAXPATH];          
 static _labelstruct *nouselabel = NULL;          
 static unsigned short nouselabelno = 0;          
 FILE *tmpout = NULL;          
 if(!nouselabelno) {          
  char tmpfile[MAXPATH];          
     sprintf(tmpfile,"unlabel.txt");          
   tmpout = fopen(tmpfile,"r");          
   while(!feof(tmpout)) {          
      if(!nouselabelno) {          
        nouselabel = (_labelstruct *)malloc( sizeof(_labelstruct)*(++nouselabelno) );          
      } else {          
        nouselabel = (_labelstruct *)realloc(nouselabel,sizeof(_labelstruct)*(++nouselabelno));          
      }          
      if(nouselabel) {          
        fscanf(tmpout,"%s",nouselabel[nouselabelno-1].labelname);          
        fscanf(tmpout,"%s",nouselabel[nouselabelno-1].modulename);          
        if(!strcmp(nouselabel[nouselabelno-1].modulename,"*"))  strcpy(nouselabel[nouselabelno-1].modulename,"");          
        fscanf(tmpout,"%ld",&nouselabel[nouselabelno-1].address);          
        fscanf(tmpout,"%d",&nouselabel[nouselabelno-1].pinmode);          
        fscanf(tmpout,"%u",&nouselabel[nouselabelno-1].testno);          
        fscanf(tmpout,"%d\n",&nouselabel[nouselabelno-1].chk_mode);          
        for(int ii=0;ii<DF_MAX_Pin_Count/64+1;ii++)          
        {          
           fscanf(tmpout,"%llx\n",&nouselabel[nouselabelno-1].pin2x[ii]);          
        }                  
        for(int ii=0;ii<DF_MAX_Pin_Count/64+1;ii++)          
        {          
           fscanf(tmpout,"%llx\n",&nouselabel[nouselabelno-1].validpin[ii]);          
        }          
      } else printf("nouselabel allocate error\n");          
   }          
 }          
 long int re = -1;          
 char tmp[65],module[32],label[32];          
 tmplabelptr = strstr(address,":");          
 if(tmplabelptr) {          
   memset(module,0,32);          
   memcpy(module,address,abs(tmplabelptr-address));          
   memset(label,0,32);          
   strcpy(label,tmplabelptr+1);          
   sprintf(tmp,"%s_C_%s",module,label);          
 } else {          
   sprintf(tmp,"%s",address);          
 }          
 re = GetLabelAddress(tmp);          
 if(re==-1) {          
   for(short ii=0;ii<nouselabelno;ii++) {          
     if(!strcmp(tmp,nouselabel[ii].labelname)){          
       re = nouselabel[ii].address;          
       break;          
     }          
   }          
 }          
 return re;          
}
 int GetPinMode(char *address) {          
 char *tmplabelptr = NULL,tmppatfile[MAXPATH];          
 static _labelstruct *nouselabel = NULL;          
 static unsigned short nouselabelno = 0;          
 FILE *tmpout = NULL;          
 if(!nouselabelno) {          
  char tmpfile[MAXPATH];          
     sprintf(tmpfile,"unlabel.txt");          
   tmpout = fopen(tmpfile,"r");          
   while(!feof(tmpout)) {          
      if(!nouselabelno) {          
        nouselabel = (_labelstruct *)malloc( sizeof(_labelstruct)*(++nouselabelno) );          
      } else {          
        nouselabel = (_labelstruct *)realloc(nouselabel,sizeof(_labelstruct)*(++nouselabelno));          
      }          
      if(nouselabel) {          
        fscanf(tmpout,"%s",nouselabel[nouselabelno-1].labelname);          
        fscanf(tmpout,"%s",nouselabel[nouselabelno-1].modulename);          
        if(!strcmp(nouselabel[nouselabelno-1].modulename,"*"))  strcpy(nouselabel[nouselabelno-1].modulename,"");          
        fscanf(tmpout,"%ld",&nouselabel[nouselabelno-1].address);          
        fscanf(tmpout,"%d",&nouselabel[nouselabelno-1].pinmode);          
        fscanf(tmpout,"%u",&nouselabel[nouselabelno-1].testno);          
        fscanf(tmpout,"%d\n",&nouselabel[nouselabelno-1].chk_mode);          
        for(int ii=0;ii<DF_MAX_Pin_Count/64+1;ii++)          
        {          
           fscanf(tmpout,"%llx\n",&nouselabel[nouselabelno-1].pin2x[ii]);          
        }          
        for(int ii=0;ii<DF_MAX_Pin_Count/64+1;ii++)          
        {          
           fscanf(tmpout,"%llx\n",&nouselabel[nouselabelno-1].validpin[ii]);          
        }          
      } else printf("nouselabel allocate error\n");          
   }          
 }          
 long int re = -1;          
 char tmp[65],module[32],label[32];          
 tmplabelptr = strstr(address,":");          
 if(tmplabelptr) {          
   memset(module,0,32);          
   memcpy(module,address,abs(tmplabelptr-address));          
   memset(label,0,32);          
   strcpy(label,tmplabelptr+1);          
   sprintf(tmp,"%s_C_%s",module,label);          
 } else {          
   sprintf(tmp,"%s",address);          
 }          
 unsigned __int64 p[DF_MAX_Pin_Count/64+1];          
 unsigned __int64 p2[DF_MAX_Pin_Count/64+1];          
 unsigned int     ts;          
   int     chk;          
 re = CLABEL::GetPinmodeByName(tmp,p,p2,ts,chk);          
 if(re==-1) {          
   for(short ii=0;ii<nouselabelno;ii++) {          
     if(!strcmp(tmp,nouselabel[ii].labelname)){          
       re = nouselabel[ii].pinmode;          
       break;          
     }          
   }          
 }          
 return re;          
}
 unsigned __int64* GetPin2x(char *address) {          
 char *tmplabelptr = NULL,tmppatfile[MAXPATH];          
 static _labelstruct *nouselabel = NULL;          
 static unsigned short nouselabelno = 0;          
 FILE *tmpout = NULL;          
 if(!nouselabelno) {          
  char tmpfile[MAXPATH];          
     sprintf(tmpfile,"unlabel.txt");          
   tmpout = fopen(tmpfile,"r");          
   while(!feof(tmpout)) {          
      if(!nouselabelno) {          
        nouselabel = (_labelstruct *)malloc( sizeof(_labelstruct)*(++nouselabelno) );          
      } else {          
        nouselabel = (_labelstruct *)realloc(nouselabel,sizeof(_labelstruct)*(++nouselabelno));          
      }          
      if(nouselabel) {          
        fscanf(tmpout,"%s",nouselabel[nouselabelno-1].labelname);          
        fscanf(tmpout,"%s",nouselabel[nouselabelno-1].modulename);          
        if(!strcmp(nouselabel[nouselabelno-1].modulename,"*"))  strcpy(nouselabel[nouselabelno-1].modulename,"");          
        fscanf(tmpout,"%ld",&nouselabel[nouselabelno-1].address);          
        fscanf(tmpout,"%d",&nouselabel[nouselabelno-1].pinmode);          
        fscanf(tmpout,"%u",&nouselabel[nouselabelno-1].testno);          
        fscanf(tmpout,"%d\n",&nouselabel[nouselabelno-1].chk_mode);          
        for(int ii=0;ii<DF_MAX_Pin_Count/64+1;ii++)          
        {          
           fscanf(tmpout,"%llx\n",&nouselabel[nouselabelno-1].pin2x[ii]);          
        }          
        for(int ii=0;ii<DF_MAX_Pin_Count/64+1;ii++)          
        {          
           fscanf(tmpout,"%llx\n",&nouselabel[nouselabelno-1].validpin[ii]);          
        }          
      } else printf("nouselabel allocate error\n");          
   }          
 }          
 static unsigned __int64 re[DF_MAX_Pin_Count/64+1];          
 memset(re,0,sizeof(unsigned __int64)*(DF_MAX_Pin_Count/64+1));          
 char tmp[65],module[32],label[32];          
 tmplabelptr = strstr(address,":");          
 if(tmplabelptr) {          
   memset(module,0,32);          
   memcpy(module,address,abs(tmplabelptr-address));          
   memset(label,0,32);          
   strcpy(label,tmplabelptr+1);          
   sprintf(tmp,"%s_C_%s",module,label);          
 } else {          
   sprintf(tmp,"%s",address);          
 }          
 unsigned __int64 p[DF_MAX_Pin_Count/64+1];          
 unsigned __int64 p2[DF_MAX_Pin_Count/64+1];          
 unsigned int  ts;          
   int  chk;          
 int a = CLABEL::GetPinmodeByName(tmp,p,p2,ts,chk);          
 if(a ==-1)          
 {          
   for(short ii=0;ii<nouselabelno;ii++) {          
     if(!strcmp(tmp,nouselabel[ii].labelname)){          
       memcpy(re,nouselabel[ii].pin2x,sizeof(unsigned __int64)*(DF_MAX_Pin_Count/64+1));          
       break;          
     }          
   }          
 }          
 else          
 {              
 memcpy(re,p,sizeof(unsigned __int64)*(DF_MAX_Pin_Count/64+1));          
 }          
 return re;          
}
 unsigned __int64* GetValidPin(char *address) {          
 char *tmplabelptr = NULL,tmppatfile[MAXPATH];          
 static _labelstruct *nouselabel = NULL;          
 static unsigned short nouselabelno = 0;          
 FILE *tmpout = NULL;          
 if(!nouselabelno) {          
  char tmpfile[MAXPATH];          
     sprintf(tmpfile,"unlabel.txt");          
   tmpout = fopen(tmpfile,"r");          
   while(!feof(tmpout)) {          
      if(!nouselabelno) {          
        nouselabel = (_labelstruct *)malloc( sizeof(_labelstruct)*(++nouselabelno) );          
      } else {          
        nouselabel = (_labelstruct *)realloc(nouselabel,sizeof(_labelstruct)*(++nouselabelno));          
      }          
      if(nouselabel) {          
        fscanf(tmpout,"%s",nouselabel[nouselabelno-1].labelname);          
        fscanf(tmpout,"%s",nouselabel[nouselabelno-1].modulename);          
        if(!strcmp(nouselabel[nouselabelno-1].modulename,"*"))  strcpy(nouselabel[nouselabelno-1].modulename,"");          
        fscanf(tmpout,"%ld",&nouselabel[nouselabelno-1].address);          
        fscanf(tmpout,"%d",&nouselabel[nouselabelno-1].pinmode);          
        fscanf(tmpout,"%u",&nouselabel[nouselabelno-1].testno);          
        fscanf(tmpout,"%d\n",&nouselabel[nouselabelno-1].chk_mode);          
        for(int ii=0;ii<DF_MAX_Pin_Count/64+1;ii++)          
        {          
           fscanf(tmpout,"%llx\n",&nouselabel[nouselabelno-1].pin2x[ii]);          
        }          
        for(int ii=0;ii<DF_MAX_Pin_Count/64+1;ii++)          
        {          
           fscanf(tmpout,"%llx\n",&nouselabel[nouselabelno-1].validpin[ii]);          
        }          
      } else printf("nouselabel allocate error\n");          
   }          
 }          
 static unsigned __int64 re[DF_MAX_Pin_Count/64+1];          
 memset(re,0,sizeof(unsigned __int64)*(DF_MAX_Pin_Count/64+1));          
 char tmp[65],module[32],label[32];          
 tmplabelptr = strstr(address,":");          
 if(tmplabelptr) {          
   memset(module,0,32);          
   memcpy(module,address,abs(tmplabelptr-address));          
   memset(label,0,32);          
   strcpy(label,tmplabelptr+1);          
   sprintf(tmp,"%s_C_%s",module,label);          
 } else {          
   sprintf(tmp,"%s",address);          
 }          
 unsigned __int64 p[DF_MAX_Pin_Count/64+1];          
 unsigned __int64 p2[DF_MAX_Pin_Count/64+1];          
 unsigned int ts;          
   int chk;          
 int a = CLABEL::GetPinmodeByName(tmp,p,p2,ts,chk);          
 if(a ==-1)          
 {          
   for(short ii=0;ii<nouselabelno;ii++) {          
     if(!strcmp(tmp,nouselabel[ii].labelname)){          
       memcpy(re,nouselabel[ii].validpin,sizeof(unsigned __int64)*(DF_MAX_Pin_Count/64+1));          
       break;          
     }          
   }          
 }          
 else          
 {              
 memcpy(re,p2,sizeof(unsigned __int64)*(DF_MAX_Pin_Count/64+1));          
 }          
 return re;          
}
 unsigned int GetLabelTestNo(char *address) {          
 char *tmplabelptr = NULL,tmppatfile[MAXPATH];          
 static _labelstruct *nouselabel = NULL;          
 static unsigned short nouselabelno = 0;          
 FILE *tmpout = NULL;          
 if(!nouselabelno) {          
  char tmpfile[MAXPATH];          
     sprintf(tmpfile,"unlabel.txt");          
   tmpout = fopen(tmpfile,"r");          
   while(!feof(tmpout)) {          
      if(!nouselabelno) {          
        nouselabel = (_labelstruct *)malloc( sizeof(_labelstruct)*(++nouselabelno) );          
      } else {          
        nouselabel = (_labelstruct *)realloc(nouselabel,sizeof(_labelstruct)*(++nouselabelno));          
      }          
      if(nouselabel) {          
        fscanf(tmpout,"%s",nouselabel[nouselabelno-1].labelname);          
        fscanf(tmpout,"%s",nouselabel[nouselabelno-1].modulename);          
        if(!strcmp(nouselabel[nouselabelno-1].modulename,"*"))  strcpy(nouselabel[nouselabelno-1].modulename,"");          
        fscanf(tmpout,"%ld",&nouselabel[nouselabelno-1].address);          
        fscanf(tmpout,"%d",&nouselabel[nouselabelno-1].pinmode);          
        fscanf(tmpout,"%u",&nouselabel[nouselabelno-1].testno);          
        fscanf(tmpout,"%d\n",&nouselabel[nouselabelno-1].chk_mode);          
        for(int ii=0;ii<DF_MAX_Pin_Count/64+1;ii++)          
        {          
           fscanf(tmpout,"%llx\n",&nouselabel[nouselabelno-1].pin2x[ii]);          
        }          
        for(int ii=0;ii<DF_MAX_Pin_Count/64+1;ii++)          
        {          
           fscanf(tmpout,"%llx\n",&nouselabel[nouselabelno-1].validpin[ii]);          
        }          
      } else printf("nouselabel allocate error\n");          
   }          
 }          
 static unsigned int retestno;          
 char tmp[65],module[32],label[32];          
 tmplabelptr = strstr(address,":");          
 if(tmplabelptr) {          
   memset(module,0,32);          
   memcpy(module,address,abs(tmplabelptr-address));          
   memset(label,0,32);          
   strcpy(label,tmplabelptr+1);          
   sprintf(tmp,"%s_C_%s",module,label);          
 } else {          
   sprintf(tmp,"%s",address);          
 }          
 unsigned __int64 p[DF_MAX_Pin_Count/64+1];          
 unsigned __int64 p2[DF_MAX_Pin_Count/64+1];          
 unsigned int     ts;          
   int     chk;          
 int a = CLABEL::GetPinmodeByName(tmp,p,p2,ts,chk);          
 if(a ==-1)          
 {          
   for(short ii=0;ii<nouselabelno;ii++) {          
     if(!strcmp(tmp,nouselabel[ii].labelname)){          
       retestno=nouselabel[ii].testno;          
       break;          
     }          
   }          
 }          
 else          
 {              
      retestno=ts;          
 }          
 return retestno;          
}
 int GetLabelChkMode(char *address) {          
 char *tmplabelptr = NULL,tmppatfile[MAXPATH];          
 static _labelstruct *nouselabel = NULL;          
 static unsigned short nouselabelno = 0;          
 FILE *tmpout = NULL;          
 if(!nouselabelno) {          
  char tmpfile[MAXPATH];          
     sprintf(tmpfile,"unlabel.txt");          
   tmpout = fopen(tmpfile,"r");          
   while(!feof(tmpout)) {          
      if(!nouselabelno) {          
        nouselabel = (_labelstruct *)malloc( sizeof(_labelstruct)*(++nouselabelno) );          
      } else {          
        nouselabel = (_labelstruct *)realloc(nouselabel,sizeof(_labelstruct)*(++nouselabelno));          
      }          
      if(nouselabel) {          
        fscanf(tmpout,"%s",nouselabel[nouselabelno-1].labelname);          
        fscanf(tmpout,"%s",nouselabel[nouselabelno-1].modulename);          
        if(!strcmp(nouselabel[nouselabelno-1].modulename,"*"))  strcpy(nouselabel[nouselabelno-1].modulename,"");          
        fscanf(tmpout,"%ld",&nouselabel[nouselabelno-1].address);          
        fscanf(tmpout,"%d",&nouselabel[nouselabelno-1].pinmode);          
        fscanf(tmpout,"%u",&nouselabel[nouselabelno-1].testno);          
        fscanf(tmpout,"%d\n",&nouselabel[nouselabelno-1].chk_mode);          
        for(int ii=0;ii<DF_MAX_Pin_Count/64+1;ii++)          
        {          
           fscanf(tmpout,"%llx\n",&nouselabel[nouselabelno-1].pin2x[ii]);          
        }          
        for(int ii=0;ii<DF_MAX_Pin_Count/64+1;ii++)          
        {          
           fscanf(tmpout,"%llx\n",&nouselabel[nouselabelno-1].validpin[ii]);          
        }          
      } else printf("nouselabel allocate error\n");          
   }          
 }          
 long int re = -1;          
 char tmp[65],module[32],label[32];          
 tmplabelptr = strstr(address,":");          
 if(tmplabelptr) {          
   memset(module,0,32);          
   memcpy(module,address,abs(tmplabelptr-address));          
   memset(label,0,32);          
   strcpy(label,tmplabelptr+1);          
   sprintf(tmp,"%s_C_%s",module,label);          
 } else {          
   sprintf(tmp,"%s",address);          
 }          
 unsigned __int64 p[DF_MAX_Pin_Count/64+1];          
 unsigned __int64 p2[DF_MAX_Pin_Count/64+1];          
 unsigned int     ts;          
   int     chk;          
 re = CLABEL::GetPinmodeByName(tmp,p,p2,ts,chk);          
 if(re==-1) {          
   for(short ii=0;ii<nouselabelno;ii++) {          
     if(!strcmp(tmp,nouselabel[ii].labelname)){          
       chk = nouselabel[ii].chk_mode;          
       break;          
     }          
   }          
 }          
 return chk;          
}
 int GetLabelAddrOffsetFactor(int pinmode, int offset, char *statement, int lineno) {		  
 char msg[256]; 		  
 if (pinmode==_IO2X_ && offset%2 != 0) { 		  
    sprintf(msg, "[Statement : %s] [Line : %d]\n The address must be even on IO_2X mode!", statement, lineno); 		  
    MessageBox(0,msg,"plncmp",MB_OK|MB_ICONERROR|MB_SETFOREGROUND); 		  
 } else if (pinmode==_DBL_IO2X_ && offset%4 != 0) { 		  
    sprintf(msg, "[Statement : %s] [Line : %d]\n The address must be multiple of 4 on DBL_2X mode!", statement, lineno); 		  
    MessageBox(0,msg,"plncmp",MB_OK|MB_ICONERROR|MB_SETFOREGROUND); 		  
 }           
 return pinmode==_IO2X_||pinmode==_DBL_IO2X_?2:1;          
}
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0,alltmpsiteno=0;
FW_SharedMemory* Fw_Shmem = NULL;
HANDLE FwShmHandle = NULL;
int MARK_NO[DF_MAX_Site_Count][100]={0};
short MARK_NO_CNT[DF_MAX_Site_Count]={0};
int tmppinmode[DF_MAX_Pin_Count]={ 0 };
 int ChkMarkNo(int val) {           
 if(alltmpsiteno==0)alltmpsiteno=GetSiteNo();           
 int *ate_pin,pin_num;           
 if(tmppinno!=0)           
 socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);           
 if(val==-1)           
 {           
   LUINT s = 0;           
   for(int ii=0;ii<alltmpsiteno;ii++){           
      if(Plugin.ShmPtr->TestPlanProgram.Status.ActiveSite[ii]==1)           
      s|=((LUINT)1<<ii);           
   }           
   return 0;           
 }           
   int actives=0;           
 int mask=0;           
 LUINT s = 0;           
 for(int ii=0;ii<alltmpsiteno;ii++){           
 if(Plugin.ShmPtr->TestPlanProgram.Status.ActiveSite[ii]==1)           
   actives++;           
 }           
 for(int ii=0;ii<alltmpsiteno;ii++){           
 bool found=0;           
 for(int jj=0;jj<MARK_NO_CNT[ii];jj++){           
 if((val == MARK_NO[ii][jj]) && Plugin.ShmPtr->TestPlanProgram.Status.ActiveSite[ii]){           
   mask++;           
   found=1;           
   break;           
 }           
 }           
 if(!found)           
 {           
   s|=((LUINT)1<<ii);           
 }           
 }           
 HW_EnabledSites(s);           
 tmppinno=0;           
 if(s!=0){           
 for(int _ii_=0; _ii_<alltmpsiteno; _ii_++){           
   if(!TrueOrFalse(s,_ii_)){           
     if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {           
     for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];           
     tmppinno += pin_num;           
     }           
     if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {           
     for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];           
     tmppinno += pin_num;           
     }           
   }           
 }           
 socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);           
 }           
 for(int ii=0;ii<alltmpsiteno;ii++){           
 if(!TrueOrFalse(s,ii))           
     Plugin.ShmPtr->TestPlanProgram.Status.ActiveSite[ii] = 0;           
 }           
 if(mask == actives)return 1;           
 else return 0;           
 }

 int SetMarkNo(int flag,char* str) {           
 if(alltmpsiteno==0)alltmpsiteno=GetSiteNo();           
 int Lindex=0;           
 int Llist[100];           
 char str2[1024];           
 sprintf(str2,"%s",str);           
 char* pt = strtok(str2,",");           
 while(pt!=NULL){           
 Llist[Lindex++] = atoi(pt);           
 pt = strtok(NULL,",");           
 }           
 if(flag==PASS){           
 for(int kk=0;kk<Lindex;kk++){           
     for(int ii=0;ii<alltmpsiteno;ii++){           
      if((PLAN_RESULT_MS[ii]==PASS) && Plugin.ShmPtr->TestPlanProgram.Status.ActiveSite[ii]){           
       int jj=0;           
           for(jj=0;jj<MARK_NO_CNT[ii];jj++){           
           if(MARK_NO[ii][jj]==Llist[kk])break;           
         }           
       if(jj==MARK_NO_CNT[ii])           
                  MARK_NO[ii][MARK_NO_CNT[ii]++]=Llist[kk];           
       }           
   }           
  }           
 }           
 else{           
 for(int kk=0;kk<Lindex;kk++){           
     for(int ii=0;ii<alltmpsiteno;ii++){           
      if((PLAN_RESULT_MS[ii]==FAIL)&& Plugin.ShmPtr->TestPlanProgram.Status.ActiveSite[ii]){           
       int jj=0;           
           for(jj=0;jj<MARK_NO_CNT[ii];jj++){           
           if(MARK_NO[ii][jj]==Llist[kk])break;           
         }           
       if(jj==MARK_NO_CNT[ii])           
                  MARK_NO[ii][MARK_NO_CNT[ii]++]=Llist[kk];           
       }           
   }           
  }           
 }           
 LUINT s = 0;           
 for(int ii=0;ii<alltmpsiteno;ii++)           
 {           
   PLAN_RESULT_MS[ii]=PASS;           
   s|=((LUINT)1<<ii);           
 }           
 HW_EnabledSites(s);           
 return 0;           
 }

 int ClearMarkNo( ) {           
 for(int ii=0;ii<DF_MAX_Site_Count;ii++)           
    MARK_NO_CNT[ii]=0;           
 }

 LUINT TranslateLUINT(int flag) {             
 static LUINT rtn;           
 rtn = (int)flag;           
 return rtn;             
 }           
 LUINT TranslateLUINT(unsigned int flag) {             
 static LUINT rtn;           
 rtn = (int)flag;           
 return rtn;             
 }           
 LUINT TranslateLUINT(LUINT flag) {             
 static LUINT rtn;           
 rtn = flag;           
 return rtn;             
 }














#line 14 "RN8213_1S_3380D_0002_B_Version.pln"




 
using namespace std;

CCATEGORY All_PASS_0("All_PASS",0);
CCATEGORY HW_OS_FAIL_0("HW_OS_FAIL",0);
CCATEGORY HW_UID_TEST_FAIL_0("HW_UID_TEST_FAIL",0);
CCATEGORY HW_ENABLE_Voltage_FAIL_0("HW_ENABLE_Voltage_FAIL",0);
CCATEGORY HW_SWD_RESET_FAIL_0("HW_SWD_RESET_FAIL",0);
CCATEGORY HW_POWER_SWICTH_FAIL_0("HW_POWER_SWICTH_FAIL",0);
CCATEGORY HW_SPLINE_FAIL_0("HW_SPLINE_FAIL",0);
CCATEGORY HW_Fail_last_item_test_0("HW_Fail_last_item_test",0);
CCATEGORY SYS_ERROR_HW_BIN_0("SYS_ERROR_HW_BIN",0);

#line 32 "RN8213_1S_3380D_0002_B_Version.pln"

CCATEGORY All_PASS_1("All_PASS",1);
CCATEGORY SW_OS_FAIL_1("SW_OS_FAIL",1);
CCATEGORY SW_RESET_SWD_FAIL_1("SW_RESET_SWD_FAIL",1);
CCATEGORY SW_UID_TEST_FAIL_1("SW_UID_TEST_FAIL",1);
CCATEGORY SW_ENABLE_Voltage_FAIL_1("SW_ENABLE_Voltage_FAIL",1);
CCATEGORY SW_SWD_RESET_FAIL_1("SW_SWD_RESET_FAIL",1);
CCATEGORY SW_POWER_SWICTH_FAIL_1("SW_POWER_SWICTH_FAIL",1);
CCATEGORY SW_SPLINE_FAIL_1("SW_SPLINE_FAIL",1);
CCATEGORY SW_LDO18_TRIM_FAIL_1("SW_LDO18_TRIM_FAIL",1);
CCATEGORY SW_RCL_TRIM_FAIL_1("SW_RCL_TRIM_FAIL",1);
CCATEGORY SW_VERSION_FAIL_1("SW_VERSION_FAIL",1);
CCATEGORY SW_Fail_last_item_test_1("SW_Fail_last_item_test",1);
CCATEGORY SYS_ERROR_SW_BIN_1("SYS_ERROR_SW_BIN",1);

#line 47 "RN8213_1S_3380D_0002_B_Version.pln"


 


#line 68 "RN8213_1S_3380D_0002_B_Version.pln"


__declspec(dllexport) int START_UP() 
{
TEST_NO(0);
if(FwShmHandle==NULL) COM_CreateAndMapSharedMemory_fw_shmem(&Fw_Shmem,&FwShmHandle);


#line 70 "RN8213_1S_3380D_0002_B_Version.pln"

  
#line 71 "RN8213_1S_3380D_0002_B_Version.pln"
SET_LEVELN("RN8213_1S_3380D_0002_B_Version.pln",71,rst_lvl,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),0.000000e+000,0.000000e+000,0.000000e+000,0.000000e+000,0.000000e+000,0.000000e+000,0.000000e+000,_KEEP_CURRENT_F,_KEEP_CURRENT_F);
  
#line 72 "RN8213_1S_3380D_0002_B_Version.pln"
SET_LEVELN("RN8213_1S_3380D_0002_B_Version.pln",72,UID_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,5.000000e+000,4.500000e+000,4.800000e+000,5.000000e-005,MAXDOUBLE,5.000000e+000,_KEEP_CURRENT_F,_KEEP_CURRENT_F);
  
#line 73 "RN8213_1S_3380D_0002_B_Version.pln"
SET_LEVELN("RN8213_1S_3380D_0002_B_Version.pln",73,LDO_POR_lvl,DVDD,(DVDD).GetTestPinno(),0.000000e+000,1.800000e+000,1.700000e+000,1.750000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);
  
#line 74 "RN8213_1S_3380D_0002_B_Version.pln"
SET_LEVELN("RN8213_1S_3380D_0002_B_Version.pln",74,PSW_lvl,AVDD,(AVDD).GetTestPinno(),0.000000e+000,3.300000e+000,3.100000e+000,3.200000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);
  
#line 75 "RN8213_1S_3380D_0002_B_Version.pln"
SET_LEVELN("RN8213_1S_3380D_0002_B_Version.pln",75,POWER_lvl,VBAT+VSWO,(VBAT+VSWO).GetTestPinno(),0.000000e+000,3.600000e+000,3.100000e+000,3.500000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);
  
  
#line 77 "RN8213_1S_3380D_0002_B_Version.pln"
SET_LEVELN("RN8213_1S_3380D_0002_B_Version.pln",77,CLK_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,3.300000e+000,1.000000e+000,2.300000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);
  
#line 78 "RN8213_1S_3380D_0002_B_Version.pln"
SET_LEVELN("RN8213_1S_3380D_0002_B_Version.pln",78,CLK_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,3.300000e+000,2.000000e+000,2.000000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);

  
#line 80 "RN8213_1S_3380D_0002_B_Version.pln"
SET_LEVELN("RN8213_1S_3380D_0002_B_Version.pln",80,LCD_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,3.300000e+000,1.000000e+000,2.300000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);

  
#line 82 "RN8213_1S_3380D_0002_B_Version.pln"
SET_LEVELN("RN8213_1S_3380D_0002_B_Version.pln",82,SAR_ADC_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,3.300000e+000,1.000000e+000,2.300000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);
  
#line 83 "RN8213_1S_3380D_0002_B_Version.pln"
SET_LEVELN("RN8213_1S_3380D_0002_B_Version.pln",83,SAR_ADC_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,6.000000e-001,1.000000e+000,2.300000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);

  
#line 85 "RN8213_1S_3380D_0002_B_Version.pln"
SET_LEVELN("RN8213_1S_3380D_0002_B_Version.pln",85,WNVR_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,3.300000e+000,1.000000e+000,2.300000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);

  
#line 87 "RN8213_1S_3380D_0002_B_Version.pln"
SET_LEVELN("RN8213_1S_3380D_0002_B_Version.pln",87,COMP_BOR_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,3.300000e+000,1.000000e+000,2.300000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);
  
#line 88 "RN8213_1S_3380D_0002_B_Version.pln"
SET_LEVELN("RN8213_1S_3380D_0002_B_Version.pln",88,SCAN_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,3.300000e+000,5.000000e-001,2.800000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);
  
#line 89 "RN8213_1S_3380D_0002_B_Version.pln"
SET_LEVELN("RN8213_1S_3380D_0002_B_Version.pln",89,SRAM_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,3.300000e+000,1.000000e+000,2.300000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);


  
#line 92 "RN8213_1S_3380D_0002_B_Version.pln"
SET_LEVELN("RN8213_1S_3380D_0002_B_Version.pln",92,GPIO_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,3.300000e+000,1.000000e+000,2.300000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);
  
#line 93 "RN8213_1S_3380D_0002_B_Version.pln"
LOAD_PAT("RN8213_1S_3380D_0002_B_Version.pln",93,"./PAT/RN8213.ppo",-1);
}

__declspec(dllexport) int TEST_START() 
{

power_down_fail_site_same_flag = false;
enter_power_down_fail_site = false;

#line 96 "RN8213_1S_3380D_0002_B_Version.pln"

  global_var_Reset();
  trim_ldo18  = 0x10;
  trim_ANALDO = 0x0;
  trim_rcl = 0x9;
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"
if(enter_power_down_fail_site){          
  LUINT tmpmask = CPIN::GetMask(),tmpfailmask = 0x0;                    
  for(int _gg_=0;_gg_<DF_MAX_Site_Count;_gg_++) {                    
    if((PLAN_RESULT_MS[_gg_]==FAIL) && TrueOrFalse(tmpmask,_gg_))                    
      tmpfailmask |= (LUINT)0x1<<_gg_;                    
  }                    
  CPIN::SetMask(tmpfailmask);          
  #line 101 "RN8213_1S_3380D_0002_B_Version.pln"                    
  LEVELS("RN8213_1S_3380D_0002_B_Version.pln",101,rst_lvl,2.000000e-003,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),1);          
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"                    
  CPIN::SetMask(tmpmask);          
}           
  else{          
  #line 101 "RN8213_1S_3380D_0002_B_Version.pln"          
  LEVELS("RN8213_1S_3380D_0002_B_Version.pln",101,rst_lvl,2.000000e-003,NULL,0,0);          
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"          
  }          
  #line 101 "RN8213_1S_3380D_0002_B_Version.pln"
;
  
#line 102 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",102,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),(IO_ALLPINS).IsIOAllPins(),128,1.000000e-003);
}

__declspec(dllexport) int TEST_END() 
{


#line 105 "RN8213_1S_3380D_0002_B_Version.pln"

  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"
if(enter_power_down_fail_site){          
  LUINT tmpmask = CPIN::GetMask(),tmpfailmask = 0x0;                    
  for(int _gg_=0;_gg_<DF_MAX_Site_Count;_gg_++) {                    
    if((PLAN_RESULT_MS[_gg_]==FAIL) && TrueOrFalse(tmpmask,_gg_))                    
      tmpfailmask |= (LUINT)0x1<<_gg_;                    
  }                    
  CPIN::SetMask(tmpfailmask);          
  #line 106 "RN8213_1S_3380D_0002_B_Version.pln"                    
  LEVELS("RN8213_1S_3380D_0002_B_Version.pln",106,rst_lvl,2.000000e-003,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),1);          
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"                    
  CPIN::SetMask(tmpmask);          
}           
  else{          
  #line 106 "RN8213_1S_3380D_0002_B_Version.pln"          
  LEVELS("RN8213_1S_3380D_0002_B_Version.pln",106,rst_lvl,2.000000e-003,NULL,0,0);          
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"          
  }          
  #line 106 "RN8213_1S_3380D_0002_B_Version.pln"
;
  
#line 107 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_V_MLDPS("RN8213_1S_3380D_0002_B_Version.pln",107,VCC,(VCC).GetTestPinno(),0.000000e+000,0,5,6.000000e-002,0,1,3.000000e-003, MAXINT, MAXINT, MAXINT, MAXINT);
  
#line 108 "RN8213_1S_3380D_0002_B_Version.pln"
WAIT("RN8213_1S_3380D_0002_B_Version.pln",108,5.000000e-003);
  
#line 109 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_V_MLDPS("RN8213_1S_3380D_0002_B_Version.pln",109,VCC,(VCC).GetTestPinno(),0.000000e+000,0,5,6.000000e-002,0,0,2.000000e-003, MAXINT, MAXINT, MAXINT, MAXINT);
  
#line 110 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",110,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),(IO_ALLPINS).IsIOAllPins(),4,1.000000e-003); 

}


__declspec(dllexport) int RELEASE() 
{


#line 115 "RN8213_1S_3380D_0002_B_Version.pln"

  ;
}

#line 1 "RN8213_1S_3380D_0002_B_Version.pln"

/*BEFORE-AFTER FUNCTION ASSIGNMENT*/
__declspec(dllexport)  int TEST_START();
__declspec(dllexport)  int TEST_END();

/*FUNCTION DECLIRATION*/
__declspec(dllexport)  int OS_TEST();
__declspec(dllexport)  int Get_Chip_TEST();
__declspec(dllexport)  int Capture_Data();
__declspec(dllexport)  int Enable_Voltage();
__declspec(dllexport)  int POWER_SWICTH();
__declspec(dllexport)  int SPLINE_TEST();
__declspec(dllexport)  int last_test();

/*CITEM DECLIRATION*/
CITEM OS_TEST_0("OS_TEST",OS_TEST);
CITEM Get_Chip_TEST_1("Get_Chip_TEST",Get_Chip_TEST);
CITEM Capture_Data_2("Capture_Data",Capture_Data);
CITEM Enable_Voltage_3("Enable_Voltage",Enable_Voltage);
CITEM POWER_SWICTH_4("POWER_SWICTH",POWER_SWICTH);
CITEM SPLINE_TEST_5("SPLINE_TEST",SPLINE_TEST);
CITEM last_test_6("last_test",last_test);

#line 130 "RN8213_1S_3380D_0002_B_Version.pln"

#line 130 "RN8213_1S_3380D_0002_B_Version.pln"





__declspec(dllexport) int OS_TEST() 
{


#line 135 "RN8213_1S_3380D_0002_B_Version.pln"

  
#line 136 "RN8213_1S_3380D_0002_B_Version.pln"
strcpy(DATALOG_MSG,"==   OS_TEST  ==");

 

  
#line 140 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_V_MLDPS("RN8213_1S_3380D_0002_B_Version.pln",140,VCC,(VCC).GetTestPinno(),0.000000e+000,0,5,1.000000e-001,0,1,2.000000e-003, MAXINT, MAXINT, MAXINT, MAXINT);
  
#line 141 "RN8213_1S_3380D_0002_B_Version.pln"
PIN_MODE("RN8213_1S_3380D_0002_B_Version.pln",141,SWD_PINS,(SWD_PINS).GetTestPinno(),(SWD_PINS).IsIOAllPins(),0,0,0,3,0);
  
#line 142 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",142,SWD_PINS,(SWD_PINS).GetTestPinno(),(SWD_PINS).IsIOAllPins(),127,2.000000e-003);

  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"
if(enter_power_down_fail_site){          
  LUINT tmpmask = CPIN::GetMask(),tmpfailmask = 0x0;                    
  for(int _gg_=0;_gg_<DF_MAX_Site_Count;_gg_++) {                    
    if((PLAN_RESULT_MS[_gg_]==FAIL) && TrueOrFalse(tmpmask,_gg_))                    
      tmpfailmask |= (LUINT)0x1<<_gg_;                    
  }                    
  CPIN::SetMask(tmpfailmask);          
  #line 144 "RN8213_1S_3380D_0002_B_Version.pln"                    
  LEVELS("RN8213_1S_3380D_0002_B_Version.pln",144,rst_lvl,2.000000e-003,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),1);          
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"                    
  CPIN::SetMask(tmpmask);          
}           
  else{          
  #line 144 "RN8213_1S_3380D_0002_B_Version.pln"          
  LEVELS("RN8213_1S_3380D_0002_B_Version.pln",144,rst_lvl,2.000000e-003,NULL,0,0);          
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"          
  }          
  #line 144 "RN8213_1S_3380D_0002_B_Version.pln"
;
  {
short setno = 0;
for(int kk=0;kk<parall_pin_cnt;kk++){
parall_pin[kk].pinno=0;}

	setno = parall_cnt(SWD_PINS+AVDD+DVDD,(SWD_PINS+AVDD+DVDD).GetTestPinno(),(SWD_PINS+AVDD+DVDD).GetFunc(),parall_pin);
	for(short _ii_ = 0; _ii_ < setno; _ii_++){
		parall_struct PIN_DEFS = parall_pin[_ii_];
		setincpin(PIN_DEFS.pin,PIN_DEFS.pinno);
int *PARAL_INC_ATECH=PIN_DEFS, PARAL_INC_ATENO=(PIN_DEFS).GetTestPinno();
double* PARAL_INC_MEAS=getmeasvalue();
#line 145 "RN8213_1S_3380D_0002_B_Version.pln"

 
    
#line 147 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",147,PIN_DEFS,(PIN_DEFS).GetTestPinno(),(PIN_DEFS).IsIOAllPins(),4,2.000000e-003);
    
#line 148 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_ON("RN8213_1S_3380D_0002_B_Version.pln",148,PIN_DEFS,(PIN_DEFS).GetTestPinno(),1,2.000000e-003);
    
    
#line 150 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_I_PMU("RN8213_1S_3380D_0002_B_Version.pln",150,CH2PMU(PIN_DEFS.pinno,PIN_DEFS.pin),-3.000000e-004,3,0,2.000000e+000,1,2.000000e-003);
    {strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
strcpy(USE_TYPE ,"")  ;
USE_UNIT = 0  ;
TEST_NO(33000001);
#line 151 "RN8213_1S_3380D_0002_B_Version.pln"
JUDGE_V_PMU("RN8213_1S_3380D_0002_B_Version.pln",151,CH2PMU(PIN_DEFS.pinno,PIN_DEFS.pin),4,-9.000000e-001,-2.000000e-001,2.000000e-003,8,_3500_AVE_MODE,5.000000e-005,_KEEP_CURRENT_F,_KEEP_CURRENT_F);}
    
#line 152 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_V_PMU("RN8213_1S_3380D_0002_B_Version.pln",152,CH2PMU(PIN_DEFS.pinno,PIN_DEFS.pin),0.000000e+000,0,3,1.000000e-003,1,2.000000e-003);
    
#line 153 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_V_PMU("RN8213_1S_3380D_0002_B_Version.pln",153,CH2PMU(PIN_DEFS.pinno,PIN_DEFS.pin),0.000000e+000,0,3,1.000000e-003,0,2.000000e-003);
    
#line 154 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",154,PIN_DEFS,(PIN_DEFS).GetTestPinno(),(PIN_DEFS).IsIOAllPins(),1,2.000000e-003);
    
#line 155 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_ON("RN8213_1S_3380D_0002_B_Version.pln",155,PIN_DEFS,(PIN_DEFS).GetTestPinno(),4,2.000000e-003);
  	}
	relase_parall(parall_pin,20);
}
#line 156 "RN8213_1S_3380D_0002_B_Version.pln"

#line 156 "RN8213_1S_3380D_0002_B_Version.pln"

  
  
#line 158 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",158,SWD_PINS,(SWD_PINS).GetTestPinno(),(SWD_PINS).IsIOAllPins(),127,2.000000e-003);
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"
if(enter_power_down_fail_site){          
  LUINT tmpmask = CPIN::GetMask(),tmpfailmask = 0x0;                    
  for(int _gg_=0;_gg_<DF_MAX_Site_Count;_gg_++) {                    
    if((PLAN_RESULT_MS[_gg_]==FAIL) && TrueOrFalse(tmpmask,_gg_))                    
      tmpfailmask |= (LUINT)0x1<<_gg_;                    
  }                    
  CPIN::SetMask(tmpfailmask);          
  #line 159 "RN8213_1S_3380D_0002_B_Version.pln"                    
  LEVELS("RN8213_1S_3380D_0002_B_Version.pln",159,rst_lvl,2.000000e-003,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),1);          
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"                    
  CPIN::SetMask(tmpmask);          
}           
  else{          
  #line 159 "RN8213_1S_3380D_0002_B_Version.pln"          
  LEVELS("RN8213_1S_3380D_0002_B_Version.pln",159,rst_lvl,2.000000e-003,NULL,0,0);          
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"          
  }          
  #line 159 "RN8213_1S_3380D_0002_B_Version.pln"
;
}
 

 

__declspec(dllexport) int Get_Chip_TEST() 
{


#line 165 "RN8213_1S_3380D_0002_B_Version.pln"
   
  POWER_ON(POWER_VOL);
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"
if(enter_power_down_fail_site){          
  LUINT tmpmask = CPIN::GetMask(),tmpfailmask = 0x0;                    
  for(int _gg_=0;_gg_<DF_MAX_Site_Count;_gg_++) {                    
    if((PLAN_RESULT_MS[_gg_]==FAIL) && TrueOrFalse(tmpmask,_gg_))                    
      tmpfailmask |= (LUINT)0x1<<_gg_;                    
  }                    
  CPIN::SetMask(tmpfailmask);          
  #line 167 "RN8213_1S_3380D_0002_B_Version.pln"                    
  LEVELS("RN8213_1S_3380D_0002_B_Version.pln",167,UID_lvl,3.000000e-003,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),1);          
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"                    
  CPIN::SetMask(tmpmask);          
}           
  else{          
  #line 167 "RN8213_1S_3380D_0002_B_Version.pln"          
  LEVELS("RN8213_1S_3380D_0002_B_Version.pln",167,UID_lvl,3.000000e-003,NULL,0,0);          
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"          
  }          
  #line 167 "RN8213_1S_3380D_0002_B_Version.pln"
; 
  swd_rate(1.000000e-006);
  
  
#line 170 "RN8213_1S_3380D_0002_B_Version.pln"
PIN_MODE("RN8213_1S_3380D_0002_B_Version.pln",170,SWDIO,(SWDIO).GetTestPinno(),(SWDIO).IsIOAllPins(),0,0,1,0,0);
  
#line 171 "RN8213_1S_3380D_0002_B_Version.pln"
PIN_MODE("RN8213_1S_3380D_0002_B_Version.pln",171,SWDCLK,(SWDCLK).GetTestPinno(),(SWDCLK).IsIOAllPins(),2,0,1,0,0);
  
#line 172 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",172,SWD_PINS,(SWD_PINS).GetTestPinno(),(SWD_PINS).IsIOAllPins(),127,1.000000e-003);
  
#line 173 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_ON("RN8213_1S_3380D_0002_B_Version.pln",173,SWD_PINS,(SWD_PINS).GetTestPinno(),4,2.000000e-003);
  
  
  {TEST_NO(33000002);
#line 176 "RN8213_1S_3380D_0002_B_Version.pln"
JUDGE_PAT("RN8213_1S_3380D_0002_B_Version.pln",176,GET_CHIP_ID,__GET_CHIP_ID,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,(__GET_CHIP_ID).Getpinmode(),(__GET_CHIP_ID).Getpin2x(),(__GET_CHIP_ID).GetValidpin(),(__GET_CHIP_ID).Gettestno(),(__GET_CHIP_ID).Getchkmode());}
  
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,184);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                       
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 178 "RN8213_1S_3380D_0002_B_Version.pln"

    if(PLAN_RESULT_MS[SOCKET] == FAIL )
    {
        cout<<"Test Get chip id Error"<<endl;  
    }
   
  		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 184 "RN8213_1S_3380D_0002_B_Version.pln"

#line 184 "RN8213_1S_3380D_0002_B_Version.pln"

  POWER_OFF();
}


__declspec(dllexport) int Capture_Data() 
{


#line 189 "RN8213_1S_3380D_0002_B_Version.pln"

    u32 ID[DUT_COUNT] = {0}; 
    u32 CP_Value[DUT_COUNT] = {0};
    POWER_ON(POWER_VOL);
    #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"
if(enter_power_down_fail_site){          
  LUINT tmpmask = CPIN::GetMask(),tmpfailmask = 0x0;                    
  for(int _gg_=0;_gg_<DF_MAX_Site_Count;_gg_++) {                    
    if((PLAN_RESULT_MS[_gg_]==FAIL) && TrueOrFalse(tmpmask,_gg_))                    
      tmpfailmask |= (LUINT)0x1<<_gg_;                    
  }                    
  CPIN::SetMask(tmpfailmask);          
  #line 193 "RN8213_1S_3380D_0002_B_Version.pln"                    
  LEVELS("RN8213_1S_3380D_0002_B_Version.pln",193,UID_lvl,3.000000e-003,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),1);          
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"                    
  CPIN::SetMask(tmpmask);          
}           
  else{          
  #line 193 "RN8213_1S_3380D_0002_B_Version.pln"          
  LEVELS("RN8213_1S_3380D_0002_B_Version.pln",193,UID_lvl,3.000000e-003,NULL,0,0);          
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"          
  }          
  #line 193 "RN8213_1S_3380D_0002_B_Version.pln"
; 
    swd_rate(1.000000e-006);
    
#line 195 "RN8213_1S_3380D_0002_B_Version.pln"
PIN_MODE("RN8213_1S_3380D_0002_B_Version.pln",195,SWDIO,(SWDIO).GetTestPinno(),(SWDIO).IsIOAllPins(),0,0,1,0,0);
    
#line 196 "RN8213_1S_3380D_0002_B_Version.pln"
PIN_MODE("RN8213_1S_3380D_0002_B_Version.pln",196,SWDCLK,(SWDCLK).GetTestPinno(),(SWDCLK).IsIOAllPins(),2,0,1,0,0);
    
#line 197 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",197,SWD_PINS,(SWD_PINS).GetTestPinno(),(SWD_PINS).IsIOAllPins(),127,1.000000e-003);
    
#line 198 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_ON("RN8213_1S_3380D_0002_B_Version.pln",198,SWD_PINS,(SWD_PINS).GetTestPinno(),4,2.000000e-003);
    RESET_SWD();
    
    
    
    write_word(0xB1,0x00000000);
    write_word(0x87,0x00000000);
    write_word(0xBD,0x03000040);
    write_word(0xA3,0x80000042);
    write_word(0x8B,0x4003402C);
    write_word(0x9F,0x00000000);
    capture_word(0x9F,ID);
    
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,213);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                       
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 211 "RN8213_1S_3380D_0002_B_Version.pln"
   
      TDO_PRINTF("CHIP_ID=0x%04X\n",ID[SOCKET]);
    		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 213 "RN8213_1S_3380D_0002_B_Version.pln"

#line 213 "RN8213_1S_3380D_0002_B_Version.pln"

    write_word(0xB1,0x00000000);
    write_word(0x87,0x00000000);
    write_word(0xBD,0x03000042);
    write_word(0xA3,0x80000041);
    write_word(0x8B,0x08C0008C);
    write_word(0x9F,0x00000000);
    capture_word(0x9F,CP_Value);
    
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,229);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                       
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 222 "RN8213_1S_3380D_0002_B_Version.pln"
   
      TDO_PRINTF("CP_UID=0x%04X\n",(CP_Value[SOCKET]&0x0000FFFF));
      if((CP_Value[SOCKET]&0x0000FFFF) != CP_UID)
      {
        PLAN_RESULT_MS[SOCKET] =FAIL;
        TDO_PRINTF("CP_UID=0x%04X,Need UID=0x%04X\n",(CP_Value[SOCKET]&0x0000FFFF),CP_UID);
      }  
    		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 229 "RN8213_1S_3380D_0002_B_Version.pln"

#line 229 "RN8213_1S_3380D_0002_B_Version.pln"

    
    POWER_OFF();
} 


__declspec(dllexport) int Enable_Voltage() 
{


#line 235 "RN8213_1S_3380D_0002_B_Version.pln"

    POWER_ON(POWER_VOL);
    #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"
if(enter_power_down_fail_site){          
  LUINT tmpmask = CPIN::GetMask(),tmpfailmask = 0x0;                    
  for(int _gg_=0;_gg_<DF_MAX_Site_Count;_gg_++) {                    
    if((PLAN_RESULT_MS[_gg_]==FAIL) && TrueOrFalse(tmpmask,_gg_))                    
      tmpfailmask |= (LUINT)0x1<<_gg_;                    
  }                    
  CPIN::SetMask(tmpfailmask);          
  #line 237 "RN8213_1S_3380D_0002_B_Version.pln"                    
  LEVELS("RN8213_1S_3380D_0002_B_Version.pln",237,UID_lvl,3.000000e-003,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),1);          
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"                    
  CPIN::SetMask(tmpmask);          
}           
  else{          
  #line 237 "RN8213_1S_3380D_0002_B_Version.pln"          
  LEVELS("RN8213_1S_3380D_0002_B_Version.pln",237,UID_lvl,3.000000e-003,NULL,0,0);          
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"          
  }          
  #line 237 "RN8213_1S_3380D_0002_B_Version.pln"
;
    {
short setno = 0;
for(int kk=0;kk<parall_pin_cnt;kk++){
parall_pin[kk].pinno=0;}

	setno = parall_cnt(LDO_PINS-DVDD,(LDO_PINS-DVDD).GetTestPinno(),(LDO_PINS-DVDD).GetFunc(),parall_pin);
	for(short _ii_ = 0; _ii_ < setno; _ii_++){
		parall_struct PIN_DEFS = parall_pin[_ii_];
		setincpin(PIN_DEFS.pin,PIN_DEFS.pinno);
int *PARAL_INC_ATECH=PIN_DEFS, PARAL_INC_ATENO=(PIN_DEFS).GetTestPinno();
double* PARAL_INC_MEAS=getmeasvalue();
#line 238 "RN8213_1S_3380D_0002_B_Version.pln"

  
      
#line 240 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",240,PIN_DEFS,(PIN_DEFS).GetTestPinno(),(PIN_DEFS).IsIOAllPins(),4,2.000000e-003);
      
#line 241 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_ON("RN8213_1S_3380D_0002_B_Version.pln",241,PIN_DEFS,(PIN_DEFS).GetTestPinno(),1,2.000000e-003);
      
#line 242 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_I_PMU("RN8213_1S_3380D_0002_B_Version.pln",242,CH2PMU(PIN_DEFS.pinno,PIN_DEFS.pin),0.000000e+000,3,0,5.000000e+000,1,2.000000e-003);
      {strcpy(USE_TYPE ,"")  ;
USE_UNIT = 0  ;
strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
TEST_NO(33000003);
#line 243 "RN8213_1S_3380D_0002_B_Version.pln"
JUDGE_V_PMU("RN8213_1S_3380D_0002_B_Version.pln",243,CH2PMU(PIN_DEFS.pinno,PIN_DEFS.pin),4,MAXDOUBLE,5.000000e-001,2.000000e-003,8,_3500_AVE_MODE,5.000000e-005,_KEEP_CURRENT_F,_KEEP_CURRENT_F);}
      
#line 244 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_V_PMU("RN8213_1S_3380D_0002_B_Version.pln",244,CH2PMU(PIN_DEFS.pinno,PIN_DEFS.pin),0.000000e+000,0,3,1.000000e-003,1,2.000000e-003);
      
#line 245 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_V_PMU("RN8213_1S_3380D_0002_B_Version.pln",245,CH2PMU(PIN_DEFS.pinno,PIN_DEFS.pin),0.000000e+000,0,3,1.000000e-003,0,2.000000e-003);
      
#line 246 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",246,PIN_DEFS,(PIN_DEFS).GetTestPinno(),(PIN_DEFS).IsIOAllPins(),1,2.000000e-003);
      
#line 247 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_ON("RN8213_1S_3380D_0002_B_Version.pln",247,PIN_DEFS,(PIN_DEFS).GetTestPinno(),4,2.000000e-003);
    	}
	relase_parall(parall_pin,20);
}
#line 248 "RN8213_1S_3380D_0002_B_Version.pln"

#line 248 "RN8213_1S_3380D_0002_B_Version.pln"

    swd_rate(1.000000e-006);
    
#line 250 "RN8213_1S_3380D_0002_B_Version.pln"
PIN_MODE("RN8213_1S_3380D_0002_B_Version.pln",250,SWDIO,(SWDIO).GetTestPinno(),(SWDIO).IsIOAllPins(),0,0,1,0,0);
    
#line 251 "RN8213_1S_3380D_0002_B_Version.pln"
PIN_MODE("RN8213_1S_3380D_0002_B_Version.pln",251,SWDCLK,(SWDCLK).GetTestPinno(),(SWDCLK).IsIOAllPins(),2,0,1,0,0);
    
#line 252 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",252,SWD_PINS,(SWD_PINS).GetTestPinno(),(SWD_PINS).IsIOAllPins(),127,1.000000e-003);
    
#line 253 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_ON("RN8213_1S_3380D_0002_B_Version.pln",253,SWD_PINS,(SWD_PINS).GetTestPinno(),4,2.000000e-003);
    RESET_SWD();
    write_word(0xB1,0x00000000);
    write_word(0x87,0x00000000);
    
    write_word(0xBD,0x03000040); 
    write_word(0xA3,0x80000042);
    write_word(0x8B,0x40034030);
    write_word(0xBB,0x00000082);
    write_word(0x8B,0x40034008);
    write_word(0xBB,0x0000009E);
    {
int PIN_IDX;for(unsigned short _ii_ = 0 ; _ii_ < (AVDD).GetMemberno();_ii_++){
PIN_IDX=_ii_;short setno = 0;for(int kk=0;kk<parall_pin_cnt;kk++){
parall_pin[kk].pinno=0;}
setno = parall_cnt((AVDD)[_ii_],((AVDD)[_ii_]).GetTestPinno(),((AVDD)[_ii_]).GetFunc(),parall_pin);
for(unsigned short _jj_ = 0; _jj_ < setno; _jj_++){
		parall_struct PIN_DEF = parall_pin[_jj_];
		setincpin(PIN_DEF.pin,PIN_DEF.pinno);
int *PIN_INC_ATECH=PIN_DEF, PIN_INC_ATENO=(PIN_DEF).GetTestPinno();
double*PIN_INC_MEAS=getmeasvalue();
double*PIN_INC_DPS_MEAS=getdpsmeasvalue();
double*PIN_INC_PREF_MEAS=getprefmeasvalue();
double*PIN_INC_UVI_MEAS=getuvimeasvalue();
#line 264 "RN8213_1S_3380D_0002_B_Version.pln"

   
      
#line 266 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",266,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),127,2.000000e-003);
      
#line 267 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_ON("RN8213_1S_3380D_0002_B_Version.pln",267,PIN_DEF,(PIN_DEF).GetTestPinno(),1,2.000000e-003); 
      
#line 268 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_I_PMU("RN8213_1S_3380D_0002_B_Version.pln",268,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,0,0,5.000000e+000,1,2.000000e-003);
      {strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
TEST_NO(33000004);
#line 269 "RN8213_1S_3380D_0002_B_Version.pln"
JUDGE_V_PMU("RN8213_1S_3380D_0002_B_Version.pln",269,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),4,3.100000e+000,3.500000e+000,2.000000e-003,16,_3500_AVE_MODE,5.000000e-005,_KEEP_CURRENT_F,_KEEP_CURRENT_F);}
      
#line 270 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_V_PMU("RN8213_1S_3380D_0002_B_Version.pln",270,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,0,3,1.000000e-003,1,2.000000e-003);
      
#line 271 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_V_PMU("RN8213_1S_3380D_0002_B_Version.pln",271,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,0,3,1.000000e-003,0,2.000000e-003);
      
#line 272 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",272,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),1,2.000000e-003);
    	}
relase_parall(parall_pin,20);}
}
#line 273 "RN8213_1S_3380D_0002_B_Version.pln"

#line 273 "RN8213_1S_3380D_0002_B_Version.pln"

    {
int PIN_IDX;for(unsigned short _ii_ = 0 ; _ii_ < (VREF).GetMemberno();_ii_++){
PIN_IDX=_ii_;short setno = 0;for(int kk=0;kk<parall_pin_cnt;kk++){
parall_pin[kk].pinno=0;}
setno = parall_cnt((VREF)[_ii_],((VREF)[_ii_]).GetTestPinno(),((VREF)[_ii_]).GetFunc(),parall_pin);
for(unsigned short _jj_ = 0; _jj_ < setno; _jj_++){
		parall_struct PIN_DEF = parall_pin[_jj_];
		setincpin(PIN_DEF.pin,PIN_DEF.pinno);
int *PIN_INC_ATECH=PIN_DEF, PIN_INC_ATENO=(PIN_DEF).GetTestPinno();
double*PIN_INC_MEAS=getmeasvalue();
double*PIN_INC_DPS_MEAS=getdpsmeasvalue();
double*PIN_INC_PREF_MEAS=getprefmeasvalue();
double*PIN_INC_UVI_MEAS=getuvimeasvalue();
#line 274 "RN8213_1S_3380D_0002_B_Version.pln"

   
      
#line 276 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",276,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),127,2.000000e-003);
      
#line 277 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_ON("RN8213_1S_3380D_0002_B_Version.pln",277,PIN_DEF,(PIN_DEF).GetTestPinno(),1,2.000000e-003); 
      
#line 278 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_I_PMU("RN8213_1S_3380D_0002_B_Version.pln",278,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,0,0,3.000000e+000,1,2.000000e-003);
      {strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
TEST_NO(33000005);
#line 279 "RN8213_1S_3380D_0002_B_Version.pln"
JUDGE_V_PMU("RN8213_1S_3380D_0002_B_Version.pln",279,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),4,1.220000e+000,1.290000e+000,2.000000e-003,16,_3500_AVE_MODE,5.000000e-005,_KEEP_CURRENT_F,_KEEP_CURRENT_F);}
      
#line 280 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_V_PMU("RN8213_1S_3380D_0002_B_Version.pln",280,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,0,3,1.000000e-003,1,2.000000e-003);
      
#line 281 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_V_PMU("RN8213_1S_3380D_0002_B_Version.pln",281,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,0,3,1.000000e-003,0,2.000000e-003);
      
#line 282 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",282,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),1,2.000000e-003);
    	}
relase_parall(parall_pin,20);}
}
#line 283 "RN8213_1S_3380D_0002_B_Version.pln"

#line 283 "RN8213_1S_3380D_0002_B_Version.pln"

    



























    POWER_OFF();
}


__declspec(dllexport) int POWER_SWICTH() 
{


#line 316 "RN8213_1S_3380D_0002_B_Version.pln"

  POWER_ON(POWER_VOL);
  
#line 318 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",318,VBAT,(VBAT).GetTestPinno(),(VBAT).IsIOAllPins(),127,1.000000e-003);
  
#line 319 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_ON("RN8213_1S_3380D_0002_B_Version.pln",319,VBAT,(VBAT).GetTestPinno(),128,1.000000e-003);
  
#line 320 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_V_PPMU("RN8213_1S_3380D_0002_B_Version.pln",320,VBAT,(VBAT).GetTestPinno(),3.600000e+000,2147483647,7,MAXDOUBLE,-1,2.000000e-003); 
  {
int PIN_IDX;for(unsigned short _ii_ = 0 ; _ii_ < (VSWO).GetMemberno();_ii_++){
PIN_IDX=_ii_;short setno = 0;for(int kk=0;kk<parall_pin_cnt;kk++){
parall_pin[kk].pinno=0;}
setno = parall_cnt((VSWO)[_ii_],((VSWO)[_ii_]).GetTestPinno(),((VSWO)[_ii_]).GetFunc(),parall_pin);
for(unsigned short _jj_ = 0; _jj_ < setno; _jj_++){
		parall_struct PIN_DEF = parall_pin[_jj_];
		setincpin(PIN_DEF.pin,PIN_DEF.pinno);
int *PIN_INC_ATECH=PIN_DEF, PIN_INC_ATENO=(PIN_DEF).GetTestPinno();
double*PIN_INC_MEAS=getmeasvalue();
double*PIN_INC_DPS_MEAS=getdpsmeasvalue();
double*PIN_INC_PREF_MEAS=getprefmeasvalue();
double*PIN_INC_UVI_MEAS=getuvimeasvalue();
#line 321 "RN8213_1S_3380D_0002_B_Version.pln"

   
    
#line 323 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",323,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),127,2.000000e-003);
    
#line 324 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_ON("RN8213_1S_3380D_0002_B_Version.pln",324,PIN_DEF,(PIN_DEF).GetTestPinno(),1,2.000000e-003); 
    
#line 325 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_I_PMU("RN8213_1S_3380D_0002_B_Version.pln",325,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,3,1,6.000000e+000,1,2.000000e-003);
    {strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
TEST_NO(33000006);
#line 326 "RN8213_1S_3380D_0002_B_Version.pln"
JUDGE_V_PMU("RN8213_1S_3380D_0002_B_Version.pln",326,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),4,4.500000e+000,5.200000e+000,2.000000e-003,16,_3500_AVE_MODE,5.000000e-005,_KEEP_CURRENT_F,_KEEP_CURRENT_F);}
    
#line 327 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_V_PMU("RN8213_1S_3380D_0002_B_Version.pln",327,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,1,3,1.000000e-003,1,2.000000e-003);
    
#line 328 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_V_PMU("RN8213_1S_3380D_0002_B_Version.pln",328,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,1,3,1.000000e-003,0,2.000000e-003);
    
#line 329 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",329,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),1,2.000000e-003);
  	}
relase_parall(parall_pin,20);}
}
#line 330 "RN8213_1S_3380D_0002_B_Version.pln"

#line 330 "RN8213_1S_3380D_0002_B_Version.pln"

  
#line 331 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_V_MLDPS("RN8213_1S_3380D_0002_B_Version.pln",331,VCC,(VCC).GetTestPinno(),2.300000e+000,0,5,2.000000e-001,0,1,3.000000e-003, MAXINT, MAXINT, MAXINT, MAXINT);   
  
#line 332 "RN8213_1S_3380D_0002_B_Version.pln"
WAIT("RN8213_1S_3380D_0002_B_Version.pln",332,5.000000e-003);
  {
int PIN_IDX;for(unsigned short _ii_ = 0 ; _ii_ < (VSWO).GetMemberno();_ii_++){
PIN_IDX=_ii_;short setno = 0;for(int kk=0;kk<parall_pin_cnt;kk++){
parall_pin[kk].pinno=0;}
setno = parall_cnt((VSWO)[_ii_],((VSWO)[_ii_]).GetTestPinno(),((VSWO)[_ii_]).GetFunc(),parall_pin);
for(unsigned short _jj_ = 0; _jj_ < setno; _jj_++){
		parall_struct PIN_DEF = parall_pin[_jj_];
		setincpin(PIN_DEF.pin,PIN_DEF.pinno);
int *PIN_INC_ATECH=PIN_DEF, PIN_INC_ATENO=(PIN_DEF).GetTestPinno();
double*PIN_INC_MEAS=getmeasvalue();
double*PIN_INC_DPS_MEAS=getdpsmeasvalue();
double*PIN_INC_PREF_MEAS=getprefmeasvalue();
double*PIN_INC_UVI_MEAS=getuvimeasvalue();
#line 333 "RN8213_1S_3380D_0002_B_Version.pln"

   
    
#line 335 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",335,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),127,2.000000e-003);
    
#line 336 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_ON("RN8213_1S_3380D_0002_B_Version.pln",336,PIN_DEF,(PIN_DEF).GetTestPinno(),1,2.000000e-003); 
    
#line 337 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_I_PMU("RN8213_1S_3380D_0002_B_Version.pln",337,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,3,1,5.000000e+000,1,2.000000e-003);
    {strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
TEST_NO(33000007);
#line 338 "RN8213_1S_3380D_0002_B_Version.pln"
JUDGE_V_PMU("RN8213_1S_3380D_0002_B_Version.pln",338,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),4,3.400000e+000,3.800000e+000,2.000000e-003,16,_3500_AVE_MODE,5.000000e-005,_KEEP_CURRENT_F,_KEEP_CURRENT_F);}
    
#line 339 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_V_PMU("RN8213_1S_3380D_0002_B_Version.pln",339,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,1,3,1.000000e-003,1,2.000000e-003);
    
#line 340 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_V_PMU("RN8213_1S_3380D_0002_B_Version.pln",340,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,1,3,1.000000e-003,0,2.000000e-003);
    
#line 341 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",341,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),1,2.000000e-003);
  	}
relase_parall(parall_pin,20);}
}
#line 342 "RN8213_1S_3380D_0002_B_Version.pln"

#line 342 "RN8213_1S_3380D_0002_B_Version.pln"

  
#line 343 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_V_MLDPS("RN8213_1S_3380D_0002_B_Version.pln",343,VCC,(VCC).GetTestPinno(),3.100000e+000,0,5,2.000000e-001,0,1,3.000000e-003, MAXINT, MAXINT, MAXINT, MAXINT);   
  
#line 344 "RN8213_1S_3380D_0002_B_Version.pln"
WAIT("RN8213_1S_3380D_0002_B_Version.pln",344,5.000000e-003);
  {
int PIN_IDX;for(unsigned short _ii_ = 0 ; _ii_ < (VSWO).GetMemberno();_ii_++){
PIN_IDX=_ii_;short setno = 0;for(int kk=0;kk<parall_pin_cnt;kk++){
parall_pin[kk].pinno=0;}
setno = parall_cnt((VSWO)[_ii_],((VSWO)[_ii_]).GetTestPinno(),((VSWO)[_ii_]).GetFunc(),parall_pin);
for(unsigned short _jj_ = 0; _jj_ < setno; _jj_++){
		parall_struct PIN_DEF = parall_pin[_jj_];
		setincpin(PIN_DEF.pin,PIN_DEF.pinno);
int *PIN_INC_ATECH=PIN_DEF, PIN_INC_ATENO=(PIN_DEF).GetTestPinno();
double*PIN_INC_MEAS=getmeasvalue();
double*PIN_INC_DPS_MEAS=getdpsmeasvalue();
double*PIN_INC_PREF_MEAS=getprefmeasvalue();
double*PIN_INC_UVI_MEAS=getuvimeasvalue();
#line 345 "RN8213_1S_3380D_0002_B_Version.pln"

   
    
#line 347 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",347,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),127,2.000000e-003);
    
#line 348 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_ON("RN8213_1S_3380D_0002_B_Version.pln",348,PIN_DEF,(PIN_DEF).GetTestPinno(),1,2.000000e-003); 
    
#line 349 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_I_PMU("RN8213_1S_3380D_0002_B_Version.pln",349,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,3,1,5.000000e+000,1,2.000000e-003);
    {strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
strcpy(USE_TYPE ,"")  ;
USE_UNIT = 0  ;
TEST_NO(33000008);
#line 350 "RN8213_1S_3380D_0002_B_Version.pln"
JUDGE_V_PMU("RN8213_1S_3380D_0002_B_Version.pln",350,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),4,2.900000e+000,MAXDOUBLE,2.000000e-003,16,_3500_AVE_MODE,5.000000e-005,_KEEP_CURRENT_F,_KEEP_CURRENT_F);}
    
#line 351 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_V_PMU("RN8213_1S_3380D_0002_B_Version.pln",351,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,1,3,1.000000e-003,1,2.000000e-003);
    
#line 352 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_V_PMU("RN8213_1S_3380D_0002_B_Version.pln",352,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,1,3,1.000000e-003,0,2.000000e-003);
    
#line 353 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",353,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),1,2.000000e-003);
  	}
relase_parall(parall_pin,20);}
}
#line 354 "RN8213_1S_3380D_0002_B_Version.pln"

#line 354 "RN8213_1S_3380D_0002_B_Version.pln"

  double meas_value[DUT_COUNT]={0};
  
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,365);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                       
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 357 "RN8213_1S_3380D_0002_B_Version.pln"

     meas_value[SOCKET]= RESULT_PIN("RN8213_1S_3380D_0002_B_Version.pln",358,((LUINT)1<<SOCKET),VSWO,(VSWO).GetTestPinno(),0);
     {strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
TEST_NO(33000009);
#line 359 "RN8213_1S_3380D_0002_B_Version.pln"
JUDGE_VARIABLE("RN8213_1S_3380D_0002_B_Version.pln",359,((LUINT)1<<SOCKET),meas_value[SOCKET],2.900000e+000,3.100000e+000,"VSWO");}
     if(PLAN_RESULT_MS[SOCKET]==FAIL)
     {
        vih_trim_flag[SOCKET] = 1;
     }
     TDO_PRINTF("vih_trim_flag[%d]=%d\n",SOCKET,vih_trim_flag[SOCKET]); 
  		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 365 "RN8213_1S_3380D_0002_B_Version.pln"

#line 365 "RN8213_1S_3380D_0002_B_Version.pln"

  
  POWER_OFF();
  
#line 368 "RN8213_1S_3380D_0002_B_Version.pln"
FORCE_V_PPMU("RN8213_1S_3380D_0002_B_Version.pln",368,VBAT,(VBAT).GetTestPinno(),0.000000e+000,2147483647,7,MAXDOUBLE,-1,2.000000e-003);
  
#line 369 "RN8213_1S_3380D_0002_B_Version.pln"
RELAY_OFF("RN8213_1S_3380D_0002_B_Version.pln",369,VBAT,(VBAT).GetTestPinno(),(VBAT).IsIOAllPins(),128,1.000000e-003);
}



__declspec(dllexport) int SPLINE_TEST() 
{


#line 374 "RN8213_1S_3380D_0002_B_Version.pln"

  
#line 375 "RN8213_1S_3380D_0002_B_Version.pln"
strcpy(DATALOG_MSG,"This is spline test\n");
  sp0LinePro();
}


__declspec(dllexport) int last_test() 
{


#line 380 "RN8213_1S_3380D_0002_B_Version.pln"

  cout<<"Test End"<<endl;
}                                         
	__declspec(dllexport) int LOADPIN()
{
IO_ALLPINS.GetFunc();
}



long
#line 4 "IC_Ctrl.pln"
 int   Active_Site = 0x0;

void global_var_Reset()
{
  int i;
  for(i = 0;i < DUT_COUNT;i++)
  {
    LDO18_trim_flag[i] = 0;
    vih_trim_flag[i]   = 0;
  }
}

void POWER_ON(double  volt)
{
  
#line 18 "IC_Ctrl.pln"
FORCE_V_MLDPS("IC_Ctrl.pln",18,VCC,(VCC).GetTestPinno(),0.000000e+000,0,5,3.000000e-001,0,1,2.000000e-003, MAXINT, MAXINT, MAXINT, MAXINT);
  
#line 19 "IC_Ctrl.pln"
WAIT("IC_Ctrl.pln",19,5.000000e-003);
  
#line 20 "IC_Ctrl.pln"
FORCE_V_MLDPS("IC_Ctrl.pln",20,VCC,(VCC).GetTestPinno(),volt,0,5,3.000000e-001,0,1,2.000000e-003, MAXINT, MAXINT, MAXINT, MAXINT);
  
#line 21 "IC_Ctrl.pln"
WAIT("IC_Ctrl.pln",21,5.000000e-003);
}

void POWER_OFF()
{
  
#line 26 "IC_Ctrl.pln"
FORCE_V_MLDPS("IC_Ctrl.pln",26,VCC,(VCC).GetTestPinno(),0.000000e+000,0,5,3.000000e-001,0,1,3.000000e-003, MAXINT, MAXINT, MAXINT, MAXINT);
  
#line 27 "IC_Ctrl.pln"
WAIT("IC_Ctrl.pln",27,2.000000e-003);
  
#line 28 "IC_Ctrl.pln"
FORCE_V_MLDPS("IC_Ctrl.pln",28,VCC,(VCC).GetTestPinno(),0.000000e+000,0,5,3.000000e-001,0,0,3.000000e-003, MAXINT, MAXINT, MAXINT, MAXINT);
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"
if(enter_power_down_fail_site){          
  LUINT tmpmask = CPIN::GetMask(),tmpfailmask = 0x0;                    
  for(int _gg_=0;_gg_<DF_MAX_Site_Count;_gg_++) {                    
    if((PLAN_RESULT_MS[_gg_]==FAIL) && TrueOrFalse(tmpmask,_gg_))                    
      tmpfailmask |= (LUINT)0x1<<_gg_;                    
  }                    
  CPIN::SetMask(tmpfailmask);          
  #line 29 "IC_Ctrl.pln"                    
  LEVELS("IC_Ctrl.pln",29,rst_lvl,2.000000e-003,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),1);          
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"                    
  CPIN::SetMask(tmpmask);          
}           
  else{          
  #line 29 "IC_Ctrl.pln"          
  LEVELS("IC_Ctrl.pln",29,rst_lvl,2.000000e-003,NULL,0,0);          
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"          
  }          
  #line 29 "IC_Ctrl.pln"
; 
  
#line 30 "IC_Ctrl.pln"
RELAY_OFF("IC_Ctrl.pln",30,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),(IO_ALLPINS).IsIOAllPins(),127,2.000000e-003);
}

void RESET_SWD()
{
  int Data[37] = {0};
  int result[5] = {0};
  u32 IDCODE = 0;
  
#line 38 "IC_Ctrl.pln"
SET_CLOG_MODE("IC_Ctrl.pln",38,SWDIO,(SWDIO).GetTestPinno(),3,1,3,1024,1,1,2147483647); 
  {TEST_NO(33000010);
#line 39 "IC_Ctrl.pln"
JUDGE_PAT("IC_Ctrl.pln",39,Reset_SWD,__Reset_SWD,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,(__Reset_SWD).Getpinmode(),(__Reset_SWD).Getpin2x(),(__Reset_SWD).GetValidpin(),(__Reset_SWD).Gettestno(),(__Reset_SWD).Getchkmode());} 
  
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,74);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                       
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 41 "IC_Ctrl.pln"

    for(int j = 0;j < 16;j++)
    {
      if(j<4) 
        result[j] = 0;
      Data[j]=0;
    }
    IDCODE = 0;

    
#line 50 "IC_Ctrl.pln"
READ_CAPTURE_MEM_BLOCK("IC_Ctrl.pln",50,(SWDIO).GetSitePins(SOCKET),(SWDIO).GetMemberno(),0,36,Data);
    int index = 0; 
    for(int i=0; i < 33; i++)
    {       
        if(i%8==0)
        { 
            index++; 
        }            
        result[index-1] += Data[i] << (i - (index*8 -8));          
    } 
    for(int i=3;i>=0;i--)
    { 
        IDCODE += result[i] << 8*i;
    }
    TDO_PRINTF("The IDCODE=0x%08X\n",IDCODE);
   
    if(IDCODE != DP_IDCODE)
    {
      TDO_PRINTF("\nThe Parity is bit %d\n",result[4]);
      TDO_PRINTF("The Capture IDCODE=0x%08X,Need DP_IDCODE=0x%08X\n",IDCODE,DP_IDCODE);
      PLAN_RESULT_MS[SOCKET] = FAIL;
      TDO_PRINTF("SWD_RESET_FAIL,Please Check it\n");
    }

  		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 74 "IC_Ctrl.pln"

#line 74 "IC_Ctrl.pln"

  
#line 75 "IC_Ctrl.pln"
SET_CLOG_MODE("IC_Ctrl.pln",75,SWDIO,(SWDIO).GetTestPinno(),3,0,3,1024,1,1,2147483647);
  
}

void swd_rate(double  rate)
{
  
   
#line 82 "IC_Ctrl.pln"
RATEN("IC_Ctrl.pln",82,1,rate);  
   
#line 83 "IC_Ctrl.pln"
MARKERN("IC_Ctrl.pln",83,1,SWDIO,(SWDIO).GetTestPinno(),(SWDIO).IsIOAllPins(),rate*5.000000e-001,rate*6.000000e-001,-1,0.000000e+000,-1,rate*5.000000e-001,-1);
   
#line 84 "IC_Ctrl.pln"
MARKERN("IC_Ctrl.pln",84,1,SWDCLK,(SWDCLK).GetTestPinno(),(SWDCLK).IsIOAllPins(),rate*2.500000e-001,rate*7.500000e-001,-1,0.000000e+000,-1,rate*5.000000e-001,-1);


}

u32 write_word(u8 Head_data,u32 reg_data)
{
  char temp_head[8],temp_data[33];
  int Parity = 0;
  if(!((Head_data >> 2) & 0x01))
  {
    for(int i=0;i<8;i++)
    {     
      if((Head_data >> i) & 0x01)
      {
        temp_head[i] = '1';
      }
      else
      {
        temp_head[i] = '0';          
      }       
    }
    for(int i=0;i<32;i++)
    {     
      if((reg_data >> i) & 0x01)
      {
        temp_data[i] = '1';
        Parity += 1;
      }
      else
      {
        temp_data[i] = '0';          
      }        
    }
    if(Parity%2)
    {
      temp_data[32] = '1';
      
    }
    else
    {
      temp_data[32] = '0';
      
    }
    
    
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,140);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                                     
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 130 "IC_Ctrl.pln"

      for(int i=0;i<33;i++)
      {
        
        
#line 134 "IC_Ctrl.pln"
zz_20150729_WRITE_PAT_MEM("IC_Ctrl.pln",134, (SWDIO).GetSitePins(SOCKET), (SWDIO).GetMemberno(),wstart_data+(i)/(wstart_data.Getpinmode()==_IO2X_||wstart_data.Getpinmode()==_DBL_IO2X_?2:1),wstart_data.Getpinmode()==_IO2X_||wstart_data.Getpinmode()==_DBL_IO2X_?i%2:0,0,temp_data+i);
        if(i<8)
        {
          
#line 137 "IC_Ctrl.pln"
zz_20150729_WRITE_PAT_MEM("IC_Ctrl.pln",137, (SWDIO).GetSitePins(SOCKET), (SWDIO).GetMemberno(),wstart_head+(i)/(wstart_head.Getpinmode()==_IO2X_||wstart_head.Getpinmode()==_DBL_IO2X_?2:1),wstart_head.Getpinmode()==_IO2X_||wstart_head.Getpinmode()==_DBL_IO2X_?i%2:0,0,temp_head+i);
        }
      }
    		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 140 "IC_Ctrl.pln"

#line 140 "IC_Ctrl.pln"

    {TEST_NO(33000011);
#line 141 "IC_Ctrl.pln"
JUDGE_PAT("IC_Ctrl.pln",141,WIRTE_WORD,__WIRTE_WORD,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,(__WIRTE_WORD).Getpinmode(),(__WIRTE_WORD).Getpin2x(),(__WIRTE_WORD).GetValidpin(),(__WIRTE_WORD).Gettestno(),(__WIRTE_WORD).Getchkmode());}
    return 0;
  }
  else
  {
      
      Compare_word(Head_data,reg_data);
      return 0;
  
  }
}

void capture_word(u8 Head_data,u32* get_data)
{
  char temp_head[8];
  int Data[37] = {0};
  int result[DUT_COUNT][5] = {{0}};
  
  if(!((Head_data >> 2) & 0x01))
     return ;
  for(int i=0;i<8;i++)
  {
    temp_head[i]= ((Head_data>>i) & 0x01) ? '1':'0';
  }
  {
SET_STATEMENT_INFO(_ID_SOCKET_INC,169);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                                     
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 165 "IC_Ctrl.pln"

    for(int i=0;i<8;i++){
      
#line 167 "IC_Ctrl.pln"
zz_20150729_WRITE_PAT_MEM("IC_Ctrl.pln",167, (SWDIO).GetSitePins(SOCKET), (SWDIO).GetMemberno(),Capture_addr+(i)/(Capture_addr.Getpinmode()==_IO2X_||Capture_addr.Getpinmode()==_DBL_IO2X_?2:1),Capture_addr.Getpinmode()==_IO2X_||Capture_addr.Getpinmode()==_DBL_IO2X_?i%2:0,0,temp_head+i);
    }
  		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 169 "IC_Ctrl.pln"

#line 169 "IC_Ctrl.pln"


  
#line 171 "IC_Ctrl.pln"
SET_CLOG_MODE("IC_Ctrl.pln",171,SWDIO,(SWDIO).GetTestPinno(),3,1,3,1024,1,1,2147483647);

  {TEST_NO(33000012);
#line 173 "IC_Ctrl.pln"
JUDGE_PAT("IC_Ctrl.pln",173,CAPTURE_WORD,__CAPTURE_WORD,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,(__CAPTURE_WORD).Getpinmode(),(__CAPTURE_WORD).Getpin2x(),(__CAPTURE_WORD).GetValidpin(),(__CAPTURE_WORD).Gettestno(),(__CAPTURE_WORD).Getchkmode());}
  
  
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,198);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                       
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 176 "IC_Ctrl.pln"
  
    for(int j = 0;j < 33;j++)
    {
      if(j<5) 
        result[SOCKET][j] = 0;
      Data[j]=0;
    }
    
#line 183 "IC_Ctrl.pln"
READ_CAPTURE_MEM_BLOCK("IC_Ctrl.pln",183,(SWDIO).GetSitePins(SOCKET),(SWDIO).GetMemberno(),0,36,Data);
    int index = 0; 
    get_data[SOCKET] = 0;
    for(int i=0; i < 33; i++)
    {       
        if(i%8==0)
        { 
            index++; 
        }            
        result[SOCKET][index-1] += Data[i] << (i - (index*8 -8));          
    } 
    for(int i=3;i>=0;i--)
    {
       get_data[SOCKET] += result[SOCKET][i] << 8*i;
    }
  		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 198 "IC_Ctrl.pln"

#line 198 "IC_Ctrl.pln"
 
  
#line 199 "IC_Ctrl.pln"
SET_CLOG_MODE("IC_Ctrl.pln",199,SWDIO,(SWDIO).GetTestPinno(),3,0,3,1024,1,0,2147483647);
}


void Compare_word(u8 Head_data,u32 reg_data)
{
  char temp_head[8],temp_data[33];
  int Parity = 0;
  if((Head_data >> 2) & 0x01)
  {
    for(int i=0;i<8;i++)
    {     
      if((Head_data >> i) & 0x01)
      {
        temp_head[i] = '1';
      }
      else
      {
        temp_head[i] = '0';          
      }       
    }
    for(int i=0;i<32;i++)
    {     
      if((reg_data >> i) & 0x01)
      {
        temp_data[i] = 'H';
        Parity += 1;
      }
      else
      {
        temp_data[i] = 'L';          
      }        
    }
    if(Parity%2)
    {
      temp_data[32] = 'H';
      
    }
    else
    {
      temp_data[32] = 'L';
      
    }
    
    
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,254);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                                     
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 244 "IC_Ctrl.pln"

      for(int i=0;i<33;i++)
      {
        
        
#line 248 "IC_Ctrl.pln"
zz_20150729_WRITE_PAT_MEM("IC_Ctrl.pln",248, (SWDIO).GetSitePins(SOCKET), (SWDIO).GetMemberno(),wread_data+(i)/(wread_data.Getpinmode()==_IO2X_||wread_data.Getpinmode()==_DBL_IO2X_?2:1),wread_data.Getpinmode()==_IO2X_||wread_data.Getpinmode()==_DBL_IO2X_?i%2:0,0,temp_data+i);
        if(i<8)
        {
          
#line 251 "IC_Ctrl.pln"
zz_20150729_WRITE_PAT_MEM("IC_Ctrl.pln",251, (SWDIO).GetSitePins(SOCKET), (SWDIO).GetMemberno(),wread_addr+(i)/(wread_addr.Getpinmode()==_IO2X_||wread_addr.Getpinmode()==_DBL_IO2X_?2:1),wread_addr.Getpinmode()==_IO2X_||wread_addr.Getpinmode()==_DBL_IO2X_?i%2:0,0,temp_head+i);
        }
      }
    		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 254 "IC_Ctrl.pln"

#line 254 "IC_Ctrl.pln"

    {TEST_NO(33000013);
#line 255 "IC_Ctrl.pln"
JUDGE_PAT("IC_Ctrl.pln",255,READ_WORD,__READ_WORD,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,(__READ_WORD).Getpinmode(),(__READ_WORD).Getpin2x(),(__READ_WORD).GetValidpin(),(__READ_WORD).Gettestno(),(__READ_WORD).Getchkmode());}
  }
  else
  {
      TDO_PRINTF("This is NO Read command(0x%02X)\n",Head_data);     
  }
}

void Judge_A_Or_D_Version(u32* data)
{
  POWER_ON(POWER_VOL);
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"
if(enter_power_down_fail_site){          
  LUINT tmpmask = CPIN::GetMask(),tmpfailmask = 0x0;                    
  for(int _gg_=0;_gg_<DF_MAX_Site_Count;_gg_++) {                    
    if((PLAN_RESULT_MS[_gg_]==FAIL) && TrueOrFalse(tmpmask,_gg_))                    
      tmpfailmask |= (LUINT)0x1<<_gg_;                    
  }                    
  CPIN::SetMask(tmpfailmask);          
  #line 266 "IC_Ctrl.pln"                    
  LEVELS("IC_Ctrl.pln",266,UID_lvl,3.000000e-003,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),1);          
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"                    
  CPIN::SetMask(tmpmask);          
}           
  else{          
  #line 266 "IC_Ctrl.pln"          
  LEVELS("IC_Ctrl.pln",266,UID_lvl,3.000000e-003,NULL,0,0);          
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"          
  }          
  #line 266 "IC_Ctrl.pln"
;
  swd_rate(1.000000e-006);
  
#line 268 "IC_Ctrl.pln"
PIN_MODE("IC_Ctrl.pln",268,SWDIO,(SWDIO).GetTestPinno(),(SWDIO).IsIOAllPins(),0,0,1,0,0);
  
#line 269 "IC_Ctrl.pln"
PIN_MODE("IC_Ctrl.pln",269,SWDCLK,(SWDCLK).GetTestPinno(),(SWDCLK).IsIOAllPins(),2,0,1,0,0);
  
#line 270 "IC_Ctrl.pln"
RELAY_OFF("IC_Ctrl.pln",270,SWD_PINS,(SWD_PINS).GetTestPinno(),(SWD_PINS).IsIOAllPins(),127,1.000000e-003);
  
#line 271 "IC_Ctrl.pln"
RELAY_ON("IC_Ctrl.pln",271,SWD_PINS,(SWD_PINS).GetTestPinno(),4,2.000000e-003);
  RESET_SWD();
  write_word(0xB1,0x00000000);
  write_word(0x87,0x00000000);
  write_word(0xBD,0x03000040);      
  write_word(0xA3,0x80000042);
  write_word(0x8B,0x40034030);      
  write_word(0xBB,0x00000082);      
  write_word(0x8B,0x4003401C);
  write_word(0xBB,0x00002004);
  write_word(0x8B,0x40038004);
  write_word(0x9F,0x00000000);
  capture_word(0x9F,data);          
  POWER_OFF();
  return ;
}





 
u32
#line 4 "spline.pln"
 LDO18_TRIM()    
{
  double vldo18[DUT_COUNT] = {0.0};
  RESET_SWD();
  {
int PIN_IDX;for(unsigned short _ii_ = 0 ; _ii_ < (DVDD).GetMemberno();_ii_++){
PIN_IDX=_ii_;short setno = 0;for(int kk=0;kk<parall_pin_cnt;kk++){
parall_pin[kk].pinno=0;}
setno = parall_cnt((DVDD)[_ii_],((DVDD)[_ii_]).GetTestPinno(),((DVDD)[_ii_]).GetFunc(),parall_pin);
for(unsigned short _jj_ = 0; _jj_ < setno; _jj_++){
		parall_struct PIN_DEF = parall_pin[_jj_];
		setincpin(PIN_DEF.pin,PIN_DEF.pinno);
int *PIN_INC_ATECH=PIN_DEF, PIN_INC_ATENO=(PIN_DEF).GetTestPinno();
double*PIN_INC_MEAS=getmeasvalue();
double*PIN_INC_DPS_MEAS=getdpsmeasvalue();
double*PIN_INC_PREF_MEAS=getprefmeasvalue();
double*PIN_INC_UVI_MEAS=getuvimeasvalue();
#line 8 "spline.pln"

   
    
#line 10 "spline.pln"
RELAY_OFF("spline.pln",10,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),127,2.000000e-003);
    
#line 11 "spline.pln"
RELAY_ON("spline.pln",11,PIN_DEF,(PIN_DEF).GetTestPinno(),1,2.000000e-003); 
    
#line 12 "spline.pln"
FORCE_I_PMU("spline.pln",12,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,0,0,3.000000e+000,1,2.000000e-003);
    {strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
TEST_NO(33000014);
#line 13 "spline.pln"
JUDGE_V_PMU("spline.pln",13,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),4,1.650000e+000,1.980000e+000,2.000000e-003,16,_3500_AVE_MODE,5.000000e-005,_KEEP_CURRENT_F,_KEEP_CURRENT_F);}
    
#line 14 "spline.pln"
FORCE_V_PMU("spline.pln",14,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,0,3,1.000000e-003,1,2.000000e-003);
    
#line 15 "spline.pln"
FORCE_V_PMU("spline.pln",15,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,0,3,1.000000e-003,0,2.000000e-003);
    
#line 16 "spline.pln"
RELAY_OFF("spline.pln",16,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),1,2.000000e-003);
  	}
relase_parall(parall_pin,20);}
}
#line 17 "spline.pln"

#line 17 "spline.pln"

  
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,69);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                       
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 19 "spline.pln"

    if(PLAN_RESULT_MS[SOCKET] == PASS)
    {
      int ok=1,cnt = 0;
      while(ok)
      {
          cnt += 1;    
          vldo18[SOCKET] = RESULT_PIN("spline.pln",26,((LUINT)1<<SOCKET),DVDD,(DVDD).GetTestPinno(),0);
          trim_ldo18 = (signed)trim_ldo18 + (signed)((1.8 - vldo18[SOCKET]) / 0.005);  
          TDO_PRINTF("Trim befrom vldo18=%0.3f,trim_ldo18=0x%08X\n",vldo18[SOCKET],trim_ldo18);
          if(trim_ldo18 & 0x80000000)
          {
            trim_ldo18 = 0;
          }
          else if(trim_ldo18 > 0x1f)
          {
            trim_ldo18 = 0x1f;
          }
          ldo18_trim_write(trim_ldo18);
          {
int PIN_IDX;for(unsigned short _ii_ = 0 ; _ii_ < (DVDD).GetMemberno();_ii_++){
PIN_IDX=_ii_;short setno = 0;for(int kk=0;kk<parall_pin_cnt;kk++){
parall_pin[kk].pinno=0;}
setno = parall_cnt((DVDD)[_ii_],((DVDD)[_ii_]).GetTestPinno(),((DVDD)[_ii_]).GetFunc(),parall_pin);
for(unsigned short _jj_ = 0; _jj_ < setno; _jj_++){
		parall_struct PIN_DEF = parall_pin[_jj_];
		setincpin(PIN_DEF.pin,PIN_DEF.pinno);
int *PIN_INC_ATECH=PIN_DEF, PIN_INC_ATENO=(PIN_DEF).GetTestPinno();
double*PIN_INC_MEAS=getmeasvalue();
double*PIN_INC_DPS_MEAS=getdpsmeasvalue();
double*PIN_INC_PREF_MEAS=getprefmeasvalue();
double*PIN_INC_UVI_MEAS=getuvimeasvalue();
#line 38 "spline.pln"

   
            
#line 40 "spline.pln"
RELAY_OFF("spline.pln",40,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),127,2.000000e-003);
            
#line 41 "spline.pln"
RELAY_ON("spline.pln",41,PIN_DEF,(PIN_DEF).GetTestPinno(),1,2.000000e-003); 
            
#line 42 "spline.pln"
FORCE_I_PMU("spline.pln",42,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,0,0,3.000000e+000,1,2.000000e-003);
            {strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
TEST_NO(33000015);
#line 43 "spline.pln"
JUDGE_V_PMU("spline.pln",43,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),4,1.650000e+000,1.970000e+000,2.000000e-003,16,_3500_AVE_MODE,5.000000e-005,_KEEP_CURRENT_F,_KEEP_CURRENT_F);}
            
#line 44 "spline.pln"
FORCE_V_PMU("spline.pln",44,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,0,3,1.000000e-003,1,2.000000e-003);
            
#line 45 "spline.pln"
FORCE_V_PMU("spline.pln",45,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,0,3,1.000000e-003,0,2.000000e-003);
            
#line 46 "spline.pln"
RELAY_OFF("spline.pln",46,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),1,2.000000e-003);
          	}
relase_parall(parall_pin,20);}
}
#line 47 "spline.pln"
      
          vldo18[SOCKET] = RESULT_PIN("spline.pln",48,((LUINT)1<<SOCKET),DVDD,(DVDD).GetTestPinno(),0);
          TDO_PRINTF("After Trim vldo18=%0.3f,trim_ldo18=0x%08X\n",vldo18[SOCKET],trim_ldo18);
          if(((vldo18[SOCKET] > 1.8) && ((vldo18[SOCKET] - 1.8) < 0.01)) ||
             ((vldo18[SOCKET] < 1.8) && ((1.8 - vldo18[SOCKET]) < 0.01)) ||
             (vldo18[SOCKET] == 1.8))
          {
            ok = 0;
            TDO_PRINTF("LDO18_TRIM Successful,COUNT=%d\n",cnt);
          }          
          if(cnt > 17)
          {
            ok = 0;    
            PLAN_RESULT_MS[SOCKET] = FAIL;
            TDO_PRINTF("SITE %d LDO18_TRIM FAIL,COUNT=%d\n",SOCKET,cnt); 
          }
      }
    }
    else
    {
      {strcpy(USE_TYPE ,"S")  ;
USE_UNIT = -3  ;
strcpy(USE_TYPE ,"")  ;
USE_UNIT = 0  ;
TEST_NO(33000016);
#line 67 "spline.pln"
JUDGE_VARIABLE("spline.pln",67,((LUINT)1<<SOCKET),vldo18[SOCKET],1.650000e+000,1.970000e+000,"LDO_TRIM_FAIL");}                 
    }
  		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 69 "spline.pln"

#line 69 "spline.pln"
    
}

u32 ldo18_trim_write(u32 trim)
{
  u32 temp = 0;
  write_word(0xB1,0x00000000);
  write_word(0xA3,0x80000042);
  write_word(0x8B,0x40034030);
  write_word(0xBB,0x00000075);
  write_word(0x8B,0x40034054);
  write_word(0x9F,0x00000000);
  capture_word(0x9F,&temp);
  temp = 0xa5580000 + ((((temp & (~(0x1f << 7))) | ((trim & 0x1f) << 7))) & 0xffef);
  trim_ANALDO = (temp & 0xffff);
  write_word(0x8B,0x40034054);
  write_word(0xBB,trim_ANALDO);
  TDO_PRINTF("PWRSW_BIN trim_ANALDO= 0x%08X\n",trim_ANALDO);
  return 1;
}

u32  rcl_trim()
{
  u32 temp;
  u32 hosc_cnt = 327670*3;  
  
  RESET_SWD();
  write_word(0xB1,0x00000000);
  write_word(0xA3,0x80000042);
  write_word(0x8B,0x40034030);      
  write_word(0xBB,0x00000082);      
  
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,106);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                       
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 101 "spline.pln"

    if(PLAN_RESULT_MS[SOCKET] == FAIL)
    {
      TDO_PRINTF("rcl_trim Function Write SOC PS fail\n");
    }
  		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 106 "spline.pln"

#line 106 "spline.pln"

  write_word(0x8B,0x40034020);
  write_word(0x9F,0x00000000);
  u32 ID = 0;
  capture_word(0x9F,&ID);
  TDO_PRINTF("Read SOC MOD = 0x%08X\n",ID);
  write_word(0x8B,0x40034020);
  write_word(0xBB,0x00000020);      
  
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,120);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                       
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 115 "spline.pln"

    if(PLAN_RESULT_MS[SOCKET] == FAIL)
    {
      TDO_PRINTF("rcl_trim Function Write MOD1 fail\n");
    }
  		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 120 "spline.pln"

#line 120 "spline.pln"

  write_word(0x8B,0x5000000C);
  write_word(0x9F,0x00000000);
  write_word(0x9F,0x00000020);
  
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,130);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                       
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 125 "spline.pln"

    if(PLAN_RESULT_MS[SOCKET] == FAIL)
    {
      TDO_PRINTF("rcl_trim Function Rd PCA1 fail\n");
    }
  		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 130 "spline.pln"

#line 130 "spline.pln"

  write_word(0x8B,0x5000000C);
  write_word(0xBB,0x0000F020);    
  
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,139);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                       
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 134 "spline.pln"

    if(PLAN_RESULT_MS[SOCKET] == FAIL)
    {
      TDO_PRINTF("rcl_trim Function Write PCA1 fail\n");
    }
  		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 139 "spline.pln"

#line 139 "spline.pln"

  write_word(0x8B,0x40034010);
  write_word(0x9F,0x00000000);
  write_word(0x9F,0x00000008);
  
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,149);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                       
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 144 "spline.pln"

    if(PLAN_RESULT_MS[SOCKET] == FAIL)
    {
      TDO_PRINTF("rcl_trim Function Rd PCA1 fail\n");
    }
  		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 149 "spline.pln"

#line 149 "spline.pln"

  write_word(0x8B,0x40034010);
  write_word(0xBB,0x00000000);
  
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,158);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                       
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 153 "spline.pln"

    if(PLAN_RESULT_MS[SOCKET] == FAIL)
    {
      TDO_PRINTF("rcl_trim Function Write PCA1 fail\n");
    }
  		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 158 "spline.pln"

#line 158 "spline.pln"

  write_word(0x8B,0x4003407C);
  write_word(0xBB,0x03000000);
  
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,167);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                       
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 162 "spline.pln"

    if(PLAN_RESULT_MS[SOCKET] == FAIL)
    {
      TDO_PRINTF("rcl_trim Function Write SOC TRIM_START fail\n");
    }
  		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 167 "spline.pln"

#line 167 "spline.pln"

  write_word(0x8B,0x40034078);
  write_word(0xBB,0x050EFFE2);    
  
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,176);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                       
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 171 "spline.pln"

    if(PLAN_RESULT_MS[SOCKET] == FAIL)
    {
      TDO_PRINTF("rcl_trim Function Write SOC TRIM_START fail\n");
    }
  		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 176 "spline.pln"

#line 176 "spline.pln"

  write_word(0x8B,0x4003407C);
  write_word(0xBB,0x07000000);
  
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,185);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                       
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 180 "spline.pln"

    if(PLAN_RESULT_MS[SOCKET] == FAIL)
    {
      TDO_PRINTF("rcl_trim Function Write SOC TRIM_START fail(0x4003407C,0x07000000)\n");
    }
  		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 185 "spline.pln"

#line 185 "spline.pln"

  int ok = 1,cnt = 0;
  while(ok)
  {
    cnt += 1;
    write_word(0x8B,0x4003407C);
    write_word(0x9F,0x00000000);
    capture_word(0x9F,&temp);
    TDO_PRINTF("capture_word rcl_trim = 0x%08X\n",  temp);
    if(temp & (1 << 24))
    {
      trim_rcl = ((float)(temp & 0xfffff) * 29491200.0 / (float)((float)(hosc_cnt+1))) ;  
      {
SET_STATEMENT_INFO(_ID_SOCKET_INC,199);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                       
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 197 "spline.pln"

        {strcpy(USE_TYPE ,"")  ;
USE_UNIT = 0  ;
strcpy(USE_TYPE ,"")  ;
USE_UNIT = 0  ;
TEST_NO(33000017);
#line 198 "spline.pln"
JUDGE_VARIABLE("spline.pln",198,((LUINT)1<<SOCKET),trim_rcl,24800,37200,"trim_rcl_Fre");}
      		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 199 "spline.pln"

#line 199 "spline.pln"

      TDO_PRINTF("rcl_trim = %d,count=%d\n",  trim_rcl,cnt);  
      trim_rcl = ( trim_rcl * 8) / 32768;
      TDO_PRINTF("rcl_trim = %d\n",  trim_rcl);
      ok = 0;
    }
    if(cnt > 100)
    {
      ok = 0;
      {
SET_STATEMENT_INFO(_ID_SOCKET_INC,210);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                       
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 208 "spline.pln"

        PLAN_RESULT_MS[SOCKET] = FAIL;
      		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 210 "spline.pln"

#line 210 "spline.pln"

    }
  }
  {
SET_STATEMENT_INFO(_ID_SOCKET_INC,215);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                       
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 213 "spline.pln"

   {strcpy(USE_TYPE ,"")  ;
USE_UNIT = 0  ;
strcpy(USE_TYPE ,"")  ;
USE_UNIT = 0  ;
TEST_NO(33000018);
#line 214 "spline.pln"
JUDGE_VARIABLE("spline.pln",214,((LUINT)1<<SOCKET),trim_rcl,6,8,"trim_rcl_Value");}
  		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 215 "spline.pln"

#line 215 "spline.pln"
 
  return 1;
}

u32 UID_adr_read(u32 address)
{
  u32 data = 0;    
  write_word(0x8B,address);       
  write_word(0x9F,0x00000000);
  capture_word(0x9F,&data);        
  return data;
}

u32 sumGen32(u32* p_data,u32 length)
{
  u32 sum;
  u32 i; 
  sum = 0x0;
  for(i=0;i<length;i++)
  {
    sum = sum + *(p_data+i);   
  }  
  return sum;
}

u32 sp0LinePro(void)
{
    u32 i;
    u32 checksum;
    u32 temp;
    u32 iso7816_cks[DUT_COUNT] = {0};
    u32 chip_id;
    Judge_A_Or_D_Version(iso7816_cks);
    POWER_ON(POWER_VOL);
    #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"
if(enter_power_down_fail_site){          
  LUINT tmpmask = CPIN::GetMask(),tmpfailmask = 0x0;                    
  for(int _gg_=0;_gg_<DF_MAX_Site_Count;_gg_++) {                    
    if((PLAN_RESULT_MS[_gg_]==FAIL) && TrueOrFalse(tmpmask,_gg_))                    
      tmpfailmask |= (LUINT)0x1<<_gg_;                    
  }                    
  CPIN::SetMask(tmpfailmask);          
  #line 249 "spline.pln"                    
  LEVELS("spline.pln",249,UID_lvl,3.000000e-003,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),1);          
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"                    
  CPIN::SetMask(tmpmask);          
}           
  else{          
  #line 249 "spline.pln"          
  LEVELS("spline.pln",249,UID_lvl,3.000000e-003,NULL,0,0);          
  #line 1 "RN8213_1S_3380D_0002_B_Version_body.cpp"          
  }          
  #line 249 "spline.pln"
;
    swd_rate(1.000000e-006);
    
#line 251 "spline.pln"
PIN_MODE("spline.pln",251,SWDIO,(SWDIO).GetTestPinno(),(SWDIO).IsIOAllPins(),0,0,1,0,0);
    
#line 252 "spline.pln"
PIN_MODE("spline.pln",252,SWDCLK,(SWDCLK).GetTestPinno(),(SWDCLK).IsIOAllPins(),2,0,1,0,0);
    
#line 253 "spline.pln"
RELAY_OFF("spline.pln",253,SWD_PINS,(SWD_PINS).GetTestPinno(),(SWD_PINS).IsIOAllPins(),127,1.000000e-003);
    
#line 254 "spline.pln"
RELAY_ON("spline.pln",254,SWD_PINS,(SWD_PINS).GetTestPinno(),4,2.000000e-003); 
    
    RESET_SWD();
    write_word(0xB1,0x00000000);
    write_word(0xA3,0x80000042);
    write_word(0x8B,0x40034030);
    write_word(0xBB,0x00000082);
    write_word(0x8B,0x4003401C);
    write_word(0xBB,0x00000004);
    
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,711);
LUINT tmpMask = CPIN::GetMask();
int *ate_pin,pin_num;
int tmpfreezepin[DF_MAX_Pin_Count],tmppinno=0;
int tmppinmode[DF_MAX_Pin_Count];
for(int _ii_=0; _ii_<DF_MAX_Site_Count; _ii_++){
	if(TrueOrFalse(tmpMask,_ii_)){
		if ((pin_num = GetPinSiteAllIOChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(_ii_, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) tmpfreezepin[tmppinno+_jj_] = ate_pin[_jj_];
		tmppinno += pin_num;
		}
	}
}
socket_freeze(tmpfreezepin,tmppinno,_ON_,tmppinmode);                       
for(short SOCKET = 0; SOCKET<DF_MAX_Site_Count; SOCKET++) {
	if(TrueOrFalse(tmpMask,SOCKET)){
		int curfreezepin[DF_MAX_Pin_Count],curpinno;
		CPIN::SetMask((LUINT)0x1<<SOCKET);
		curpinno=0;
		if ((pin_num = GetPinSiteAllIOChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		if ((pin_num = GetPinSiteAllLCDChannel(SOCKET, ate_pin)) > 0) {
		for (int _jj_=0;_jj_<pin_num;_jj_++) curfreezepin[curpinno+_jj_] = ate_pin[_jj_];
		curpinno += pin_num;
		}
		socket_freeze(curfreezepin,curpinno,_OFF_,tmppinmode);
#line 264 "spline.pln"

      chip_id = 0x0;
      TDO_PRINTF("SITE %d Chip Version = 0x%08X,A is 0x%08X ,D is 0x%08X;!!!\n",SOCKET,iso7816_cks[SOCKET],A_Version,D_Version);
      if(iso7816_cks[SOCKET] == D_Version)
      {
        chip_id = 0xD0000000;   
      }
      else if(iso7816_cks[SOCKET] == A_Version)
      {
        chip_id = 0xA0000000;   
      }
      else
      {
        PLAN_RESULT_MS[SOCKET] = FAIL;
        TDO_PRINTF("SITE %d Chip Version (0x%08X)is unNormal,Need 0x%08X or 0x%08X;Please Check it!!!\n",SOCKET,iso7816_cks[SOCKET],A_Version,D_Version);
      }
      if((chip_id & 0xf0000000)==0xA0000000)
      {
          u8 a_Version_spl0[PAGE_SIZE] = {
              F1 & 0xff,        (F1 >> 8) & 0xff,        (F1 >> 16) & 0xff, (F1 >> 24) & 0xff, 
              F2 & 0xff,        (F2 >> 8) & 0xff,        (F2 >> 16) & 0xff, (F2 >> 24) & 0xff, 
              F3 & 0xff,        (F3 >> 8) & 0xff,        (F3 >> 16) & 0xff, (F3 >> 24) & 0xff, 
              F4 & 0xff,        (F4 >> 8) & 0xff,        (F4 >> 16) & 0xff, (F4 >> 24) & 0xff, 
              ANAADC & 0xff,    (ANAADC >> 8) & 0xff,    ANACLK1 & 0xff,    (ANACLK1 >> 8) & 0xff, 
              ANACLK2 & 0xff,   (ANACLK2 >> 8) & 0xff,   ANALDO & 0xff,     (ANALDO >> 8) & 0xff, 
              ANABGR & 0xff,    (ANABGR >> 8) & 0xff,    ANAPSW & 0xff,     (ANAPSW >> 8) & 0xff, 
              0xff,             0xff,                    0xff,              0xff, 
              TPSCTRL & 0xff,   (TPSCTRL >> 8) & 0xff,   ADCTEST & 0xff,    (ADCTEST >> 8) & 0xff, 
              CACHECTRL & 0xff, (CACHECTRL >> 8) & 0xff, 0xff,              0xff, 
              0xff,             0xff,                    0xff,              0xff, 
              0xff,             0xff,                    0xff,              0xff, 
              0xff,             0xff,                    0xff,              0xff, 
              0xff,             0xff,                    0xff,              0xff, 
              0xff,             0xff,                    0xff,              0xff, 
              0xff,             0xff,                    0xff,              0xff
             };
        
          
          a_Version_spl0[28] = ((0x3<<4)|(FT_times & 0xf));
          a_Version_spl0[29] = ((0x12) & 0xff);                 
          

          temp = UID_adr_read(WAFER_CHIPID);

          a_Version_spl0[38] = temp & 0xff;
          a_Version_spl0[39] = (temp >> 8) & 0xff;
          
          temp = UID_adr_read( WAFER_SERIAL_1_ADR );
          a_Version_spl0[40] =  temp & 0xff;
          a_Version_spl0[41] = (temp >> 8) & 0xff;
          
          temp = UID_adr_read( WAFER_SERIAL_2_ADR );
          a_Version_spl0[42] = temp & 0xff;
          a_Version_spl0[43] = (temp >> 8) & 0xff;
          
          temp = UID_adr_read( WAFER_NUM_ADR );  
          a_Version_spl0[44] = temp & 0xff;
          a_Version_spl0[45] = (temp >> 8) & 0xff;
          
          temp = UID_adr_read( WAFER_X_ADR );    
          a_Version_spl0[46] = temp & 0xff;
          a_Version_spl0[47] = (temp >> 8) & 0xff;
          
          temp = UID_adr_read( WAFER_Y_ADR );  
          a_Version_spl0[48] = temp & 0xff;
          a_Version_spl0[49] = (temp >> 8) & 0xff;
          
          temp = UID_adr_read( WAFER_RESULT_ADR );  
          a_Version_spl0[50] = temp & 0xff;
          a_Version_spl0[51] = (temp >> 8) & 0xff;

        checksum = 0 - sumGen32((u32 *)a_Version_spl0,15);
        TDO_PRINTF("SITE %d checksum =0x%08X\n",checksum);
        
        a_Version_spl0[PAGE_SIZE - 1] = (checksum >> 24) & 0xff;
        a_Version_spl0[PAGE_SIZE - 2] = (checksum >> 16) & 0xff;
        a_Version_spl0[PAGE_SIZE - 3] = (checksum >> 8) & 0xff;
        a_Version_spl0[PAGE_SIZE - 4] = checksum & 0xff;
            
        
        for(i=0;i<(PAGE_SIZE/4);i++)
        {
          write_word(0x8B,EEPROM_SP0_BASE+i*4);
          write_word(0x9F,0x00000000);
          capture_word(0x9F,&temp);
          u32 spl0_temp = (a_Version_spl0[4*(i+1)-1] << 24) + 
          (a_Version_spl0[4*(i+1)-2] << 16) + (a_Version_spl0[4*(i+1)-3] << 8) +
          (a_Version_spl0[4*(i+1)-4]);
          if(temp != spl0_temp)
          {
            break;
          }
        }

        if(i == (PAGE_SIZE/4))
        {
          TDO_PRINTF("SPLINEPRO MESSAGE is same,Need't write again\n");
        } 
        else
        {
        
          
          write_word(0xB1,0x00000000);
          write_word(0xA3,0x80000042);
          write_word(0x8B,0x08800014);
          write_word(0xBB,0xFEED0000);

          
          u32 ID_temp = 0;
          int cnt = 0;
          do
          { 
            cnt += 1;
            write_word(0x8B,0x08800008);
            write_word(0x9F,0x00000000);
            capture_word(0x9F,&ID_temp);          
          }while((ID_temp != 0x0000003E) & cnt < 10);

          
          write_word(0x8B,0x08800000);
          write_word(0xBB,0xFEED0004);
          
          write_word(0xB1,0x00000000);
          write_word(0xA3,0x80000040);     
          for( i=0 ;i<PAGE_SIZE ;i++ )
          {
            write_word(0x8B,(EEPROM_BASE_REMAP3+i)); 
            write_word(0xBB,*(a_Version_spl0+i));
          }       

          
          write_word(0xB1,0x00000000);
          write_word(0x87,0x00000000);
          write_word(0xA3,0x80000042);
          write_word(0x8B,0x08800004);
          write_word(0xBB,0x00000300); 

          
          ID_temp = 0;
          cnt = 0;
          do
          { 
            cnt += 1;
            write_word(0x8B,0x08800008);
            write_word(0x9F,0x00000000);
            capture_word(0x9F,&ID_temp);          
          }while((ID_temp != 0x0000002E) & cnt < 10);

          
          write_word(0x8B,0x08800000);
          write_word(0xBB,0xFEED000B);

          
          ID_temp = 0;
          cnt = 0;
          do
          { 
            cnt += 1;
            write_word(0x8B,0x08800008);
            write_word(0x9F,0x00000000);
            capture_word(0x9F,&ID_temp);          
          }while((ID_temp != 0x0000002E) & cnt < 10);

          
          write_word(0x8B,0x08800000);
          write_word(0xBB,0xFEED000F);

          
          write_word(0x8B,0x08800014);
          write_word(0xBB,0xFEEDFFFF);

          
          ID_temp = 0;
          cnt = 0;
          do
          { 
            cnt += 1;
            write_word(0x8B,0x08800008);
            write_word(0x9F,0x00000000);
            capture_word(0x9F,&ID_temp);          
          }while((ID_temp != 0x0000002E) & cnt < 10);

          
          for(i=0;i<(PAGE_SIZE/4);i++)
          {
           
            write_word(0x8B,EEPROM_SP0_BASE+i*4);
            write_word(0x9F,0x00000000);
            capture_word(0x9F,&temp);
            u32 spl0_temp = (a_Version_spl0[4*(i+1)-1] << 24) + 
            (a_Version_spl0[4*(i+1)-2] << 16) + (a_Version_spl0[4*(i+1)-3] << 8) +
            (a_Version_spl0[4*(i+1)-4]);
            if(PRINT)
              TDO_PRINTF("TEMP=0x%08X,a_Version_spl0[%d] = 0x%08X\n",temp,i,a_Version_spl0[i]);
            if(temp != spl0_temp)
            {
              PLAN_RESULT_MS[SOCKET] = FAIL;
              TDO_PRINTF("TEMP=0x%08X,but a_Version_spl0[%d] = 0x%08X\n",temp,i,spl0_temp);
            }
           }
         }
       }
       else if((chip_id & 0xf0000000)==0xD0000000)
       {
          u8 d_Version_spl0[PAGE_SIZE] = {
          F1 & 0xff,        (F1 >> 8) & 0xff,        (F1 >> 16) & 0xff, (F1 >> 24) & 0xff, 
          F2 & 0xff,        (F2 >> 8) & 0xff,        (F2 >> 16) & 0xff, (F2 >> 24) & 0xff, 
          F3 & 0xff,        (F3 >> 8) & 0xff,        (F3 >> 16) & 0xff, (F3 >> 24) & 0xff, 
          F4 & 0xff,        (F4 >> 8) & 0xff,        (F4 >> 16) & 0xff, (F4 >> 24) & 0xff, 
          ANAADC & 0xff,    (ANAADC >> 8) & 0xff,    ANACLK1 & 0xff,    (ANACLK1 >> 8) & 0xff, 
          ANACLK2_B & 0xff,   (ANACLK2_B >> 8) & 0xff,   ANALDO_B & 0xff,     (ANALDO_B >> 8) & 0xff, 
          ANABGR_B & 0xff,    (ANABGR_B >> 8) & 0xff,    ANAPSW_B & 0xff,     (ANAPSW_B >> 8) & 0xff, 
          0xff,             0xff,                    0xff,              0xff, 
          TPSCTRL & 0xff,   (TPSCTRL >> 8) & 0xff,   ADCTEST & 0xff,    (ADCTEST >> 8) & 0xff, 
          CACHECTRL & 0xff, (CACHECTRL >> 8) & 0xff, SOFT_CHIPID_B & 0xff,            (SOFT_CHIPID_B >> 8) &  0xff, 
          0xff,             0xff,                    0xff,              0xff, 
          0xff,             0xff,                    0xff,              0xff, 
          0xff,             0xff,                    0xff,              0xff, 
          0xff,             0xff,                    0xff,              0xff, 
          0xff,             0xff,                    0xff,              0xff, 
          0xff,             0xff,                    0xff,              0xff
        };
          
#line 486 "spline.pln"
SET_FAIL_BIN("spline.pln",486, HW_SPLINE_FAIL_0.GetVal(), SW_LDO18_TRIM_FAIL_1.GetVal());
          LDO18_TRIM();
          if( ((trim_ANALDO >>4) & 0x1) != 0 )
          {
            TDO_PRINTF("powswh_bin = %d,trim_ANALDO=%x! \n",2,trim_ANALDO);
            PLAN_RESULT_MS[SOCKET] = FAIL;
          }
          else
          {
              d_Version_spl0[22] = (( ANALDO_B & 0xf06f)  | (trim_ANALDO & 0x0f90) )& 0xff;
              d_Version_spl0[23] = (((ANALDO_B & 0xf06f)  | (trim_ANALDO & 0x0f90) ) >> 8) & 0xff; 
              
#line 497 "spline.pln"
SET_FAIL_BIN("spline.pln",497, HW_SPLINE_FAIL_0.GetVal(), SW_RCL_TRIM_FAIL_1.GetVal());
              rcl_trim();

              d_Version_spl0[20] = (( ANACLK2_B & 0x07ff) | ((trim_rcl & 0x1f) << 11)) & 0xff;
              d_Version_spl0[21] = (((ANACLK2_B & 0x07ff) | ((trim_rcl & 0x1f) << 11)) >> 8) & 0xff;

            
              write_word(0xB1,0x00000000);
              write_word(0xA3,0x80000042);
              write_word(0x8B,0x40034030);
              write_word(0xBB,0x00000082);
              write_word(0x8B,0x4003401C);
              write_word(0xBB,0x00000004);
            
              d_Version_spl0[28] = ((0x3<<4)|(FT_times & 0xf));
              d_Version_spl0[29] = ((0x12) & 0xff);                 
            
              temp = UID_adr_read( WAFER_CHIPID );
              temp = temp >> 16;
              if(PRINT)
                TDO_PRINTF("WAFER_CHIPID =0x%08X\n",temp);
              if(temp == 0xffff)
              {
                d_Version_spl0[38] = SOFT_CHIPID_B & 0xff;
                d_Version_spl0[39] = (SOFT_CHIPID_B >> 8) & 0xff;
              } 
              else if((temp == 0x000B)||(temp == 0x00B1))
              {
                d_Version_spl0[38] = temp & 0xff;
                d_Version_spl0[39] = (temp >> 8) & 0xff;
              }     
              else
              {
                
#line 530 "spline.pln"
SET_FAIL_BIN("spline.pln",530, HW_SPLINE_FAIL_0.GetVal(), SW_VERSION_FAIL_1.GetVal());
                PLAN_RESULT_MS[SOCKET] = FAIL;
              }
              temp = UID_adr_read( WAFER_SERIAL_1_ADR );
              if(PRINT)
                TDO_PRINTF("WAFER_SERIAL_1_ADR =0x%08X\n",temp);

              d_Version_spl0[40] =  temp & 0xff;
              d_Version_spl0[41] = (temp >> 8) & 0xff;
              
              temp = UID_adr_read( WAFER_SERIAL_2_ADR );
              if(PRINT)
                TDO_PRINTF("WAFER_SERIAL_2_ADR =0x%08X\n",temp);
              d_Version_spl0[42] = temp & 0xff;
              d_Version_spl0[43] = (temp >> 8) & 0xff;
              
              temp = UID_adr_read( WAFER_NUM_ADR );
              if(PRINT)
                TDO_PRINTF("WAFER_NUM_ADR =0x%08X\n",temp);
              d_Version_spl0[44] = temp & 0xff;
              d_Version_spl0[45] = (temp >> 8) & 0xff;
              
              temp = UID_adr_read( WAFER_X_ADR ); 
              if(PRINT)
                TDO_PRINTF("WAFER_X_ADR =0x%08X\n",temp);
              d_Version_spl0[46] = temp & 0xff;
              d_Version_spl0[47] = (temp >> 8) & 0xff;
              
              temp=UID_adr_read( WAFER_Y_ADR );  
              if(PRINT)
                TDO_PRINTF("WAFER_Y_ADR =0x%08X\n",temp);
              d_Version_spl0[48] = temp & 0xff;
              d_Version_spl0[49] = (temp >> 8) & 0xff;
              
              temp=UID_adr_read( WAFER_RESULT_ADR ); 
              if(PRINT)
                TDO_PRINTF("WAFER_RESULT_ADR =0x%08X\n",temp);
              d_Version_spl0[50] = temp & 0xff;
              d_Version_spl0[51] = (temp >> 8) & 0xff;

              checksum = 0 - sumGen32((u32 *)d_Version_spl0,15);

              d_Version_spl0[PAGE_SIZE - 1] = (checksum >> 24) & 0xff;
              d_Version_spl0[PAGE_SIZE - 2] = (checksum >> 16) & 0xff;
              d_Version_spl0[PAGE_SIZE - 3] = (checksum >> 8) & 0xff;
              d_Version_spl0[PAGE_SIZE - 4] = checksum & 0xff;

            
            for(i=0;i<(PAGE_SIZE/4);i++)
            {
              write_word(0x8B,EEPROM_SP0_BASE+i*4);
              write_word(0x9F,0x00000000);
              capture_word(0x9F,&temp);
              u32 spl0_temp = (d_Version_spl0[4*(i+1)-1] << 24) + 
              (d_Version_spl0[4*(i+1)-2] << 16) + (d_Version_spl0[4*(i+1)-3] << 8) +
              (d_Version_spl0[4*(i+1)-4]);
              if(PRINT)
                TDO_PRINTF("TEMP=0x%08X,d_Version_spl0[%d] = 0x%08X,spl0_temp=0x%08X\n",temp,i,d_Version_spl0[i],spl0_temp);
              if(temp != spl0_temp)
              {
                i = 500;
              }
            }
            if(i == (PAGE_SIZE/4))
            {
              TDO_PRINTF("SPLINEPRO MESSAGE is same,Need't write again\n");
            } 
            else
            {
              
              write_word(0xB1,0x00000000);
              write_word(0xA3,0x80000042);
              write_word(0x8B,EEPROM_BASE_REMAP3+EEPROM_PROT_OFFSET);
              write_word(0xBB,0xFEED0000); 

              
              u32 ID_temp = 0;
              int cnt = 0;
              do
              { 
                cnt += 1;
                write_word(0x8B,EEPROM_BASE_REMAP3+EEPROM_STA_OFFSET);
                write_word(0x9F,0x00000000);
                capture_word(0x9F,&ID_temp);          
              }while((ID_temp != 0x0000003E) & cnt < 10);
              if(PRINT)
                TDO_PRINTF("ID_temp=0x%08X,cnt = %d\n",ID_temp,cnt);

              
              write_word(0x8B,EEPROM_BASE_REMAP3+EEPROM_CMD_OFFSET);
              write_word(0xBB,0xFEED0004);
              
              write_word(0xB1,0x00000000);
              write_word(0xA3,0x80000040);     
              for( i=0 ;i<PAGE_SIZE ;i++ )
              {
                
                write_word(0x8B,(EEPROM_BASE_REMAP3+i)); 
                write_word(0xBB,*(d_Version_spl0+i));
                if(PRINT)
                  TDO_PRINTF("EEPROM_BASE_REMAP3 address is:0x%08X,d_Version_spl0[%d] = 0x%08X\n",EEPROM_BASE_REMAP3+i,i,d_Version_spl0[i]);
              }       

              
              write_word(0xB1,0x00000000);
              write_word(0x87,0x00000000);
              write_word(0xA3,0x80000042);
              write_word(0x8B,EEPROM_BASE_REMAP3+EEPROM_PGA_OFFSET);
              write_word(0xBB,0x00000300); 

              
              ID_temp = 0;
              cnt = 0;
              do
              { 
                cnt += 1;
                write_word(0x8B,EEPROM_BASE_REMAP3+EEPROM_STA_OFFSET);
                write_word(0x9F,0x00000000);
                capture_word(0x9F,&ID_temp);
              }while((ID_temp != 0x0000002E) & cnt < 10);
              if(PRINT)
                TDO_PRINTF("ID_temp=0x%08X,cnt = %d\n",ID_temp,cnt);
              
              write_word(0x8B,EEPROM_BASE_REMAP3+EEPROM_CMD_OFFSET);
              write_word(0xBB,0xFEED000B);

              
              ID_temp = 0;
              cnt = 0;
              do
              { 
                cnt += 1;
                write_word(0x8B,EEPROM_BASE_REMAP3+EEPROM_STA_OFFSET);
                write_word(0x9F,0x00000000);
                capture_word(0x9F,&ID_temp);          
              }while((ID_temp != 0x0000002E) & cnt < 10);
              if(PRINT)
                TDO_PRINTF("ID_temp=0x%08X,cnt = %d\n",ID_temp,cnt);
              
              write_word(0x8B,EEPROM_BASE_REMAP3+EEPROM_CMD_OFFSET);
              write_word(0xBB,0xFEED000F);

              
              write_word(0x8B,EEPROM_BASE_REMAP3+EEPROM_PROT_OFFSET);
              write_word(0xBB,0xFEEDFFFF);

              

              
              ID_temp = 0;
              cnt = 0;
              do
              { 
                cnt += 1;
                write_word(0x8B,0x08800008);
                write_word(0x9F,0x00000000);
                capture_word(0x9F,&ID_temp);          
              }while((ID_temp != 0x0000002E) & cnt < 10);
              if(PRINT)
                TDO_PRINTF("ID_temp=0x%08X,cnt = %d\n",ID_temp,cnt);                        
              
              for(i=0;i<(PAGE_SIZE/4);i++)
              {
                
                write_word(0x8B,EEPROM_SP0_BASE+i*4);
                write_word(0x9F,0x00000000);
                capture_word(0x9F,&temp);
                u32 spl0_temp = (d_Version_spl0[4*(i+1)-1] << 24) + 
                (d_Version_spl0[4*(i+1)-2] << 16) + (d_Version_spl0[4*(i+1)-3] << 8) +
                (d_Version_spl0[4*(i+1)-4]);
                if(PRINT)
                TDO_PRINTF("TEMP=0x%08X,d_Version_spl0[%d] = 0x%08X,spl0_temp=0x%08X\n",temp,i,d_Version_spl0[i],spl0_temp);
                if(temp != spl0_temp)
                {
                  PLAN_RESULT_MS[SOCKET] = FAIL;
                  TDO_PRINTF("TEMP=0x%08X,but d_Version_spl0[%d] = 0x%08X\n",temp,i,d_Version_spl0[i]);
                }
              }
            }
        }
      }
    		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 711 "spline.pln"

#line 711 "spline.pln"

  POWER_OFF();  
  return 1;
}

