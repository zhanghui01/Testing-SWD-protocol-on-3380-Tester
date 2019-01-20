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
#define __RN8211B_V20C_38D_BODY_FILE
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

#line 1 "RN8211B_V20C_38D.PLN"




using namespace std;

CCATEGORY All_PASS_0("All_PASS",0);
CCATEGORY HW_OS_FAIL_0("HW_OS_FAIL",0);
CCATEGORY HW_UID_TEST_FAIL_0("HW_UID_TEST_FAIL",0);
CCATEGORY HW_ENABLE_Voltage_FAIL_0("HW_ENABLE_Voltage_FAIL",0);
CCATEGORY HW_SWD_RESET_FAIL_0("HW_SWD_RESET_FAIL",0);
CCATEGORY HW_POWER_SWICTH_FAIL_0("HW_POWER_SWICTH_FAIL",0);
CCATEGORY HW_Fail_last_item_test_0("HW_Fail_last_item_test",0);
CCATEGORY SYS_ERROR_HW_BIN_0("SYS_ERROR_HW_BIN",0);

#line 17 "RN8211B_V20C_38D.PLN"

CCATEGORY All_PASS_1("All_PASS",1);
CCATEGORY SW_OS_FAIL_1("SW_OS_FAIL",1);
CCATEGORY SW_RESET_SWD_FAIL_1("SW_RESET_SWD_FAIL",1);
CCATEGORY SW_UID_TEST_FAIL_1("SW_UID_TEST_FAIL",1);
CCATEGORY SW_ENABLE_Voltage_FAIL_1("SW_ENABLE_Voltage_FAIL",1);
CCATEGORY SW_SWD_RESET_FAIL_1("SW_SWD_RESET_FAIL",1);
CCATEGORY SW_POWER_SWICTH_FAIL_1("SW_POWER_SWICTH_FAIL",1);
CCATEGORY SW_Fail_last_item_test_1("SW_Fail_last_item_test",1);
CCATEGORY SYS_ERROR_SW_BIN_1("SYS_ERROR_SW_BIN",1);

#line 28 "RN8211B_V20C_38D.PLN"


 


#line 49 "RN8211B_V20C_38D.PLN"


__declspec(dllexport) int START_UP() 
{
TEST_NO(0);
if(FwShmHandle==NULL) COM_CreateAndMapSharedMemory_fw_shmem(&Fw_Shmem,&FwShmHandle);


#line 51 "RN8211B_V20C_38D.PLN"

  
#line 52 "RN8211B_V20C_38D.PLN"
SET_LEVELN("RN8211B_V20C_38D.PLN",52,rst_lvl,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),0.000000e+000,0.000000e+000,0.000000e+000,0.000000e+000,0.000000e+000,0.000000e+000,0.000000e+000,_KEEP_CURRENT_F,_KEEP_CURRENT_F);
  
#line 53 "RN8211B_V20C_38D.PLN"
SET_LEVELN("RN8211B_V20C_38D.PLN",53,UID_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,5.000000e+000,4.500000e+000,4.800000e+000,5.000000e-005,MAXDOUBLE,5.000000e+000,_KEEP_CURRENT_F,_KEEP_CURRENT_F);
  
#line 54 "RN8211B_V20C_38D.PLN"
SET_LEVELN("RN8211B_V20C_38D.PLN",54,LDO_POR_lvl,DVDD,(DVDD).GetTestPinno(),0.000000e+000,1.800000e+000,1.700000e+000,1.750000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);
  
#line 55 "RN8211B_V20C_38D.PLN"
SET_LEVELN("RN8211B_V20C_38D.PLN",55,PSW_lvl,AVDD,(AVDD).GetTestPinno(),0.000000e+000,3.300000e+000,3.100000e+000,3.200000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);
  
#line 56 "RN8211B_V20C_38D.PLN"
SET_LEVELN("RN8211B_V20C_38D.PLN",56,POWER_lvl,VBAT+VSWO,(VBAT+VSWO).GetTestPinno(),0.000000e+000,3.600000e+000,3.100000e+000,3.500000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);
  
  
#line 58 "RN8211B_V20C_38D.PLN"
SET_LEVELN("RN8211B_V20C_38D.PLN",58,CLK_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,3.300000e+000,1.000000e+000,2.300000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);
  
#line 59 "RN8211B_V20C_38D.PLN"
SET_LEVELN("RN8211B_V20C_38D.PLN",59,CLK_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,3.300000e+000,2.000000e+000,2.000000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);

  
#line 61 "RN8211B_V20C_38D.PLN"
SET_LEVELN("RN8211B_V20C_38D.PLN",61,LCD_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,3.300000e+000,1.000000e+000,2.300000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);

  
#line 63 "RN8211B_V20C_38D.PLN"
SET_LEVELN("RN8211B_V20C_38D.PLN",63,SAR_ADC_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,3.300000e+000,1.000000e+000,2.300000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);
  
#line 64 "RN8211B_V20C_38D.PLN"
SET_LEVELN("RN8211B_V20C_38D.PLN",64,SAR_ADC_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,6.000000e-001,1.000000e+000,2.300000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);

  
#line 66 "RN8211B_V20C_38D.PLN"
SET_LEVELN("RN8211B_V20C_38D.PLN",66,WNVR_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,3.300000e+000,1.000000e+000,2.300000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);

  
#line 68 "RN8211B_V20C_38D.PLN"
SET_LEVELN("RN8211B_V20C_38D.PLN",68,COMP_BOR_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,3.300000e+000,1.000000e+000,2.300000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);
  
#line 69 "RN8211B_V20C_38D.PLN"
SET_LEVELN("RN8211B_V20C_38D.PLN",69,SCAN_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,3.300000e+000,5.000000e-001,2.800000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);
  
#line 70 "RN8211B_V20C_38D.PLN"
SET_LEVELN("RN8211B_V20C_38D.PLN",70,SRAM_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,3.300000e+000,1.000000e+000,2.300000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);


  
#line 73 "RN8211B_V20C_38D.PLN"
SET_LEVELN("RN8211B_V20C_38D.PLN",73,GPIO_lvl,SWD_PINS,(SWD_PINS).GetTestPinno(),0.000000e+000,3.300000e+000,1.000000e+000,2.300000e+000,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE,MAXDOUBLE);
  
#line 74 "RN8211B_V20C_38D.PLN"
LOAD_PAT("RN8211B_V20C_38D.PLN",74,"./PAT/RN8213.ppo",-1);
}

__declspec(dllexport) int TEST_START() 
{

power_down_fail_site_same_flag = false;
enter_power_down_fail_site = false;

#line 77 "RN8211B_V20C_38D.PLN"

  global_var_Reset();
  #line 1 "RN8211B_V20C_38D_body.cpp"
if(enter_power_down_fail_site){          
  LUINT tmpmask = CPIN::GetMask(),tmpfailmask = 0x0;                    
  for(int _gg_=0;_gg_<DF_MAX_Site_Count;_gg_++) {                    
    if((PLAN_RESULT_MS[_gg_]==FAIL) && TrueOrFalse(tmpmask,_gg_))                    
      tmpfailmask |= (LUINT)0x1<<_gg_;                    
  }                    
  CPIN::SetMask(tmpfailmask);          
  #line 79 "RN8211B_V20C_38D.PLN"                    
  LEVELS("RN8211B_V20C_38D.PLN",79,rst_lvl,2.000000e-003,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),1);          
  #line 1 "RN8211B_V20C_38D_body.cpp"                    
  CPIN::SetMask(tmpmask);          
}           
  else{          
  #line 79 "RN8211B_V20C_38D.PLN"          
  LEVELS("RN8211B_V20C_38D.PLN",79,rst_lvl,2.000000e-003,NULL,0,0);          
  #line 1 "RN8211B_V20C_38D_body.cpp"          
  }          
  #line 79 "RN8211B_V20C_38D.PLN"
;
  
#line 80 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",80,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),(IO_ALLPINS).IsIOAllPins(),128,1.000000e-003);
}

__declspec(dllexport) int TEST_END() 
{


#line 83 "RN8211B_V20C_38D.PLN"

  #line 1 "RN8211B_V20C_38D_body.cpp"
if(enter_power_down_fail_site){          
  LUINT tmpmask = CPIN::GetMask(),tmpfailmask = 0x0;                    
  for(int _gg_=0;_gg_<DF_MAX_Site_Count;_gg_++) {                    
    if((PLAN_RESULT_MS[_gg_]==FAIL) && TrueOrFalse(tmpmask,_gg_))                    
      tmpfailmask |= (LUINT)0x1<<_gg_;                    
  }                    
  CPIN::SetMask(tmpfailmask);          
  #line 84 "RN8211B_V20C_38D.PLN"                    
  LEVELS("RN8211B_V20C_38D.PLN",84,rst_lvl,2.000000e-003,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),1);          
  #line 1 "RN8211B_V20C_38D_body.cpp"                    
  CPIN::SetMask(tmpmask);          
}           
  else{          
  #line 84 "RN8211B_V20C_38D.PLN"          
  LEVELS("RN8211B_V20C_38D.PLN",84,rst_lvl,2.000000e-003,NULL,0,0);          
  #line 1 "RN8211B_V20C_38D_body.cpp"          
  }          
  #line 84 "RN8211B_V20C_38D.PLN"
;
  
#line 85 "RN8211B_V20C_38D.PLN"
FORCE_V_MLDPS("RN8211B_V20C_38D.PLN",85,VCC,(VCC).GetTestPinno(),0.000000e+000,0,5,6.000000e-002,0,1,3.000000e-003, MAXINT, MAXINT, MAXINT, MAXINT);
  
#line 86 "RN8211B_V20C_38D.PLN"
WAIT("RN8211B_V20C_38D.PLN",86,5.000000e-003);
  
#line 87 "RN8211B_V20C_38D.PLN"
FORCE_V_MLDPS("RN8211B_V20C_38D.PLN",87,VCC,(VCC).GetTestPinno(),0.000000e+000,0,5,6.000000e-002,0,0,2.000000e-003, MAXINT, MAXINT, MAXINT, MAXINT);
  
#line 88 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",88,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),(IO_ALLPINS).IsIOAllPins(),4,1.000000e-003); 

}


__declspec(dllexport) int RELEASE() 
{


#line 93 "RN8211B_V20C_38D.PLN"

  ;
}

#line 1 "RN8211B_V20C_38D.PLN"

/*BEFORE-AFTER FUNCTION ASSIGNMENT*/
__declspec(dllexport)  int TEST_START();
__declspec(dllexport)  int TEST_END();

/*FUNCTION DECLIRATION*/
__declspec(dllexport)  int OS_TEST();
__declspec(dllexport)  int Get_Chip_TEST();
__declspec(dllexport)  int Capture_Data();
__declspec(dllexport)  int Enable_Voltage();
__declspec(dllexport)  int POWER_SWICTH();
__declspec(dllexport)  int last_test();

/*CITEM DECLIRATION*/
CITEM OS_TEST_0("OS_TEST",OS_TEST);
CITEM Get_Chip_TEST_1("Get_Chip_TEST",Get_Chip_TEST);
CITEM Capture_Data_2("Capture_Data",Capture_Data);
CITEM Enable_Voltage_3("Enable_Voltage",Enable_Voltage);
CITEM POWER_SWICTH_4("POWER_SWICTH",POWER_SWICTH);
CITEM last_test_5("last_test",last_test);

#line 107 "RN8211B_V20C_38D.PLN"

#line 107 "RN8211B_V20C_38D.PLN"





__declspec(dllexport) int OS_TEST() 
{


#line 112 "RN8211B_V20C_38D.PLN"

  
#line 113 "RN8211B_V20C_38D.PLN"
strcpy(DATALOG_MSG,"==   OS_TEST  ==");

 

  
#line 117 "RN8211B_V20C_38D.PLN"
FORCE_V_MLDPS("RN8211B_V20C_38D.PLN",117,VCC,(VCC).GetTestPinno(),0.000000e+000,0,5,1.000000e-001,0,1,2.000000e-003, MAXINT, MAXINT, MAXINT, MAXINT);
  
#line 118 "RN8211B_V20C_38D.PLN"
PIN_MODE("RN8211B_V20C_38D.PLN",118,SWD_PINS,(SWD_PINS).GetTestPinno(),(SWD_PINS).IsIOAllPins(),0,0,0,3,0);
  
#line 119 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",119,SWD_PINS,(SWD_PINS).GetTestPinno(),(SWD_PINS).IsIOAllPins(),127,2.000000e-003);

  #line 1 "RN8211B_V20C_38D_body.cpp"
if(enter_power_down_fail_site){          
  LUINT tmpmask = CPIN::GetMask(),tmpfailmask = 0x0;                    
  for(int _gg_=0;_gg_<DF_MAX_Site_Count;_gg_++) {                    
    if((PLAN_RESULT_MS[_gg_]==FAIL) && TrueOrFalse(tmpmask,_gg_))                    
      tmpfailmask |= (LUINT)0x1<<_gg_;                    
  }                    
  CPIN::SetMask(tmpfailmask);          
  #line 121 "RN8211B_V20C_38D.PLN"                    
  LEVELS("RN8211B_V20C_38D.PLN",121,rst_lvl,2.000000e-003,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),1);          
  #line 1 "RN8211B_V20C_38D_body.cpp"                    
  CPIN::SetMask(tmpmask);          
}           
  else{          
  #line 121 "RN8211B_V20C_38D.PLN"          
  LEVELS("RN8211B_V20C_38D.PLN",121,rst_lvl,2.000000e-003,NULL,0,0);          
  #line 1 "RN8211B_V20C_38D_body.cpp"          
  }          
  #line 121 "RN8211B_V20C_38D.PLN"
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
#line 122 "RN8211B_V20C_38D.PLN"

 
    
#line 124 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",124,PIN_DEFS,(PIN_DEFS).GetTestPinno(),(PIN_DEFS).IsIOAllPins(),4,2.000000e-003);
    
#line 125 "RN8211B_V20C_38D.PLN"
RELAY_ON("RN8211B_V20C_38D.PLN",125,PIN_DEFS,(PIN_DEFS).GetTestPinno(),1,2.000000e-003);
    
    
#line 127 "RN8211B_V20C_38D.PLN"
FORCE_I_PMU("RN8211B_V20C_38D.PLN",127,CH2PMU(PIN_DEFS.pinno,PIN_DEFS.pin),-3.000000e-004,3,0,2.000000e+000,1,2.000000e-003);
    {strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
strcpy(USE_TYPE ,"")  ;
USE_UNIT = 0  ;
TEST_NO(33000001);
#line 128 "RN8211B_V20C_38D.PLN"
JUDGE_V_PMU("RN8211B_V20C_38D.PLN",128,CH2PMU(PIN_DEFS.pinno,PIN_DEFS.pin),4,-9.000000e-001,-2.000000e-001,2.000000e-003,8,_3500_AVE_MODE,5.000000e-005,_KEEP_CURRENT_F,_KEEP_CURRENT_F);}
    
#line 129 "RN8211B_V20C_38D.PLN"
FORCE_V_PMU("RN8211B_V20C_38D.PLN",129,CH2PMU(PIN_DEFS.pinno,PIN_DEFS.pin),0.000000e+000,0,3,1.000000e-003,1,2.000000e-003);
    
#line 130 "RN8211B_V20C_38D.PLN"
FORCE_V_PMU("RN8211B_V20C_38D.PLN",130,CH2PMU(PIN_DEFS.pinno,PIN_DEFS.pin),0.000000e+000,0,3,1.000000e-003,0,2.000000e-003);
    
#line 131 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",131,PIN_DEFS,(PIN_DEFS).GetTestPinno(),(PIN_DEFS).IsIOAllPins(),1,2.000000e-003);
    
#line 132 "RN8211B_V20C_38D.PLN"
RELAY_ON("RN8211B_V20C_38D.PLN",132,PIN_DEFS,(PIN_DEFS).GetTestPinno(),4,2.000000e-003);
  	}
	relase_parall(parall_pin,20);
}
#line 133 "RN8211B_V20C_38D.PLN"

#line 133 "RN8211B_V20C_38D.PLN"

  
  
#line 135 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",135,SWD_PINS,(SWD_PINS).GetTestPinno(),(SWD_PINS).IsIOAllPins(),127,2.000000e-003);
  #line 1 "RN8211B_V20C_38D_body.cpp"
if(enter_power_down_fail_site){          
  LUINT tmpmask = CPIN::GetMask(),tmpfailmask = 0x0;                    
  for(int _gg_=0;_gg_<DF_MAX_Site_Count;_gg_++) {                    
    if((PLAN_RESULT_MS[_gg_]==FAIL) && TrueOrFalse(tmpmask,_gg_))                    
      tmpfailmask |= (LUINT)0x1<<_gg_;                    
  }                    
  CPIN::SetMask(tmpfailmask);          
  #line 136 "RN8211B_V20C_38D.PLN"                    
  LEVELS("RN8211B_V20C_38D.PLN",136,rst_lvl,2.000000e-003,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),1);          
  #line 1 "RN8211B_V20C_38D_body.cpp"                    
  CPIN::SetMask(tmpmask);          
}           
  else{          
  #line 136 "RN8211B_V20C_38D.PLN"          
  LEVELS("RN8211B_V20C_38D.PLN",136,rst_lvl,2.000000e-003,NULL,0,0);          
  #line 1 "RN8211B_V20C_38D_body.cpp"          
  }          
  #line 136 "RN8211B_V20C_38D.PLN"
;
}
 

 

__declspec(dllexport) int Get_Chip_TEST() 
{


#line 142 "RN8211B_V20C_38D.PLN"
   
  POWER_ON(POWER_VOL);
  #line 1 "RN8211B_V20C_38D_body.cpp"
if(enter_power_down_fail_site){          
  LUINT tmpmask = CPIN::GetMask(),tmpfailmask = 0x0;                    
  for(int _gg_=0;_gg_<DF_MAX_Site_Count;_gg_++) {                    
    if((PLAN_RESULT_MS[_gg_]==FAIL) && TrueOrFalse(tmpmask,_gg_))                    
      tmpfailmask |= (LUINT)0x1<<_gg_;                    
  }                    
  CPIN::SetMask(tmpfailmask);          
  #line 144 "RN8211B_V20C_38D.PLN"                    
  LEVELS("RN8211B_V20C_38D.PLN",144,UID_lvl,3.000000e-003,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),1);          
  #line 1 "RN8211B_V20C_38D_body.cpp"                    
  CPIN::SetMask(tmpmask);          
}           
  else{          
  #line 144 "RN8211B_V20C_38D.PLN"          
  LEVELS("RN8211B_V20C_38D.PLN",144,UID_lvl,3.000000e-003,NULL,0,0);          
  #line 1 "RN8211B_V20C_38D_body.cpp"          
  }          
  #line 144 "RN8211B_V20C_38D.PLN"
; 
  swd_rate(1.000000e-006);
  
  
#line 147 "RN8211B_V20C_38D.PLN"
PIN_MODE("RN8211B_V20C_38D.PLN",147,SWDIO,(SWDIO).GetTestPinno(),(SWDIO).IsIOAllPins(),0,0,1,0,0);
  
#line 148 "RN8211B_V20C_38D.PLN"
PIN_MODE("RN8211B_V20C_38D.PLN",148,SWDCLK,(SWDCLK).GetTestPinno(),(SWDCLK).IsIOAllPins(),2,0,1,0,0);
  
#line 149 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",149,SWD_PINS,(SWD_PINS).GetTestPinno(),(SWD_PINS).IsIOAllPins(),127,1.000000e-003);
  
#line 150 "RN8211B_V20C_38D.PLN"
RELAY_ON("RN8211B_V20C_38D.PLN",150,SWD_PINS,(SWD_PINS).GetTestPinno(),4,2.000000e-003);
  
  {TEST_NO(33000002);
#line 152 "RN8211B_V20C_38D.PLN"
JUDGE_PAT("RN8211B_V20C_38D.PLN",152,GET_CHIP_ID,__GET_CHIP_ID,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,(__GET_CHIP_ID).Getpinmode(),(__GET_CHIP_ID).Getpin2x(),(__GET_CHIP_ID).GetValidpin(),(__GET_CHIP_ID).Gettestno(),(__GET_CHIP_ID).Getchkmode());}
  
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,160);
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
#line 154 "RN8211B_V20C_38D.PLN"

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
#line 160 "RN8211B_V20C_38D.PLN"

#line 160 "RN8211B_V20C_38D.PLN"

  POWER_OFF();
}


__declspec(dllexport) int Capture_Data() 
{


#line 165 "RN8211B_V20C_38D.PLN"

    u32 ID[DUT_COUNT] = {0}; 
    u32 CP_Value[DUT_COUNT] = {0};
    POWER_ON(POWER_VOL);
    #line 1 "RN8211B_V20C_38D_body.cpp"
if(enter_power_down_fail_site){          
  LUINT tmpmask = CPIN::GetMask(),tmpfailmask = 0x0;                    
  for(int _gg_=0;_gg_<DF_MAX_Site_Count;_gg_++) {                    
    if((PLAN_RESULT_MS[_gg_]==FAIL) && TrueOrFalse(tmpmask,_gg_))                    
      tmpfailmask |= (LUINT)0x1<<_gg_;                    
  }                    
  CPIN::SetMask(tmpfailmask);          
  #line 169 "RN8211B_V20C_38D.PLN"                    
  LEVELS("RN8211B_V20C_38D.PLN",169,UID_lvl,3.000000e-003,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),1);          
  #line 1 "RN8211B_V20C_38D_body.cpp"                    
  CPIN::SetMask(tmpmask);          
}           
  else{          
  #line 169 "RN8211B_V20C_38D.PLN"          
  LEVELS("RN8211B_V20C_38D.PLN",169,UID_lvl,3.000000e-003,NULL,0,0);          
  #line 1 "RN8211B_V20C_38D_body.cpp"          
  }          
  #line 169 "RN8211B_V20C_38D.PLN"
; 
    swd_rate(1.000000e-006);
    
#line 171 "RN8211B_V20C_38D.PLN"
PIN_MODE("RN8211B_V20C_38D.PLN",171,SWDIO,(SWDIO).GetTestPinno(),(SWDIO).IsIOAllPins(),0,0,1,0,0);
    
#line 172 "RN8211B_V20C_38D.PLN"
PIN_MODE("RN8211B_V20C_38D.PLN",172,SWDCLK,(SWDCLK).GetTestPinno(),(SWDCLK).IsIOAllPins(),2,0,1,0,0);
    
#line 173 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",173,SWD_PINS,(SWD_PINS).GetTestPinno(),(SWD_PINS).IsIOAllPins(),127,1.000000e-003);
    
#line 174 "RN8211B_V20C_38D.PLN"
RELAY_ON("RN8211B_V20C_38D.PLN",174,SWD_PINS,(SWD_PINS).GetTestPinno(),4,2.000000e-003);
    RESET_SWD();
    
    
    
    write_word(0xB1,0x00000000);
    write_word(0x87,0x00000000);
    write_word(0xBD,0x03000040);
    write_word(0xA3,0x80000042);
    write_word(0x8B,0x4003402C);
    write_word(0x9F,0x00000000);
    capture_word(0x9F,ID);
    
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,189);
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
#line 187 "RN8211B_V20C_38D.PLN"
   
      TDO_PRINTF("CHIP_ID=0x%04X\n",ID[SOCKET]);
    		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 189 "RN8211B_V20C_38D.PLN"

#line 189 "RN8211B_V20C_38D.PLN"

    write_word(0xB1,0x00000000);
    write_word(0x87,0x00000000);
    write_word(0xBD,0x03000042);
    write_word(0xA3,0x80000041);
    write_word(0x8B,0x08C0008C);
    write_word(0x9F,0x00000000);
    capture_word(0x9F,CP_Value);
    
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,205);
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
#line 198 "RN8211B_V20C_38D.PLN"
   
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
#line 205 "RN8211B_V20C_38D.PLN"

#line 205 "RN8211B_V20C_38D.PLN"

    
    POWER_OFF();
}


__declspec(dllexport) int Enable_Voltage() 
{


#line 211 "RN8211B_V20C_38D.PLN"

    POWER_ON(POWER_VOL);
    #line 1 "RN8211B_V20C_38D_body.cpp"
if(enter_power_down_fail_site){          
  LUINT tmpmask = CPIN::GetMask(),tmpfailmask = 0x0;                    
  for(int _gg_=0;_gg_<DF_MAX_Site_Count;_gg_++) {                    
    if((PLAN_RESULT_MS[_gg_]==FAIL) && TrueOrFalse(tmpmask,_gg_))                    
      tmpfailmask |= (LUINT)0x1<<_gg_;                    
  }                    
  CPIN::SetMask(tmpfailmask);          
  #line 213 "RN8211B_V20C_38D.PLN"                    
  LEVELS("RN8211B_V20C_38D.PLN",213,UID_lvl,3.000000e-003,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),1);          
  #line 1 "RN8211B_V20C_38D_body.cpp"                    
  CPIN::SetMask(tmpmask);          
}           
  else{          
  #line 213 "RN8211B_V20C_38D.PLN"          
  LEVELS("RN8211B_V20C_38D.PLN",213,UID_lvl,3.000000e-003,NULL,0,0);          
  #line 1 "RN8211B_V20C_38D_body.cpp"          
  }          
  #line 213 "RN8211B_V20C_38D.PLN"
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
#line 214 "RN8211B_V20C_38D.PLN"

 
      
#line 216 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",216,PIN_DEFS,(PIN_DEFS).GetTestPinno(),(PIN_DEFS).IsIOAllPins(),4,2.000000e-003);
      
#line 217 "RN8211B_V20C_38D.PLN"
RELAY_ON("RN8211B_V20C_38D.PLN",217,PIN_DEFS,(PIN_DEFS).GetTestPinno(),1,2.000000e-003);
      
#line 218 "RN8211B_V20C_38D.PLN"
FORCE_I_PMU("RN8211B_V20C_38D.PLN",218,CH2PMU(PIN_DEFS.pinno,PIN_DEFS.pin),0.000000e+000,3,0,5.000000e+000,1,2.000000e-003);
      {strcpy(USE_TYPE ,"")  ;
USE_UNIT = 0  ;
strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
TEST_NO(33000003);
#line 219 "RN8211B_V20C_38D.PLN"
JUDGE_V_PMU("RN8211B_V20C_38D.PLN",219,CH2PMU(PIN_DEFS.pinno,PIN_DEFS.pin),4,MAXDOUBLE,5.000000e-001,2.000000e-003,8,_3500_AVE_MODE,5.000000e-005,_KEEP_CURRENT_F,_KEEP_CURRENT_F);}
      
#line 220 "RN8211B_V20C_38D.PLN"
FORCE_V_PMU("RN8211B_V20C_38D.PLN",220,CH2PMU(PIN_DEFS.pinno,PIN_DEFS.pin),0.000000e+000,0,3,1.000000e-003,1,2.000000e-003);
      
#line 221 "RN8211B_V20C_38D.PLN"
FORCE_V_PMU("RN8211B_V20C_38D.PLN",221,CH2PMU(PIN_DEFS.pinno,PIN_DEFS.pin),0.000000e+000,0,3,1.000000e-003,0,2.000000e-003);
      
#line 222 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",222,PIN_DEFS,(PIN_DEFS).GetTestPinno(),(PIN_DEFS).IsIOAllPins(),1,2.000000e-003);
      
#line 223 "RN8211B_V20C_38D.PLN"
RELAY_ON("RN8211B_V20C_38D.PLN",223,PIN_DEFS,(PIN_DEFS).GetTestPinno(),4,2.000000e-003);
    	}
	relase_parall(parall_pin,20);
}
#line 224 "RN8211B_V20C_38D.PLN"

#line 224 "RN8211B_V20C_38D.PLN"

    swd_rate(1.000000e-006);
    
#line 226 "RN8211B_V20C_38D.PLN"
PIN_MODE("RN8211B_V20C_38D.PLN",226,SWDIO,(SWDIO).GetTestPinno(),(SWDIO).IsIOAllPins(),0,0,1,0,0);
    
#line 227 "RN8211B_V20C_38D.PLN"
PIN_MODE("RN8211B_V20C_38D.PLN",227,SWDCLK,(SWDCLK).GetTestPinno(),(SWDCLK).IsIOAllPins(),2,0,1,0,0);
    
#line 228 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",228,SWD_PINS,(SWD_PINS).GetTestPinno(),(SWD_PINS).IsIOAllPins(),127,1.000000e-003);
    
#line 229 "RN8211B_V20C_38D.PLN"
RELAY_ON("RN8211B_V20C_38D.PLN",229,SWD_PINS,(SWD_PINS).GetTestPinno(),4,2.000000e-003);
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
#line 240 "RN8211B_V20C_38D.PLN"

   
      
#line 242 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",242,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),127,2.000000e-003);
      
#line 243 "RN8211B_V20C_38D.PLN"
RELAY_ON("RN8211B_V20C_38D.PLN",243,PIN_DEF,(PIN_DEF).GetTestPinno(),1,2.000000e-003); 
      
#line 244 "RN8211B_V20C_38D.PLN"
FORCE_I_PMU("RN8211B_V20C_38D.PLN",244,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,0,0,5.000000e+000,1,2.000000e-003);
      {strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
TEST_NO(33000004);
#line 245 "RN8211B_V20C_38D.PLN"
JUDGE_V_PMU("RN8211B_V20C_38D.PLN",245,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),4,3.100000e+000,3.500000e+000,2.000000e-003,16,_3500_AVE_MODE,5.000000e-005,_KEEP_CURRENT_F,_KEEP_CURRENT_F);}
      
#line 246 "RN8211B_V20C_38D.PLN"
FORCE_V_PMU("RN8211B_V20C_38D.PLN",246,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,0,3,1.000000e-003,1,2.000000e-003);
      
#line 247 "RN8211B_V20C_38D.PLN"
FORCE_V_PMU("RN8211B_V20C_38D.PLN",247,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,0,3,1.000000e-003,0,2.000000e-003);
      
#line 248 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",248,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),1,2.000000e-003);
    	}
relase_parall(parall_pin,20);}
}
#line 249 "RN8211B_V20C_38D.PLN"

#line 249 "RN8211B_V20C_38D.PLN"

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
#line 250 "RN8211B_V20C_38D.PLN"

   
      
#line 252 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",252,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),127,2.000000e-003);
      
#line 253 "RN8211B_V20C_38D.PLN"
RELAY_ON("RN8211B_V20C_38D.PLN",253,PIN_DEF,(PIN_DEF).GetTestPinno(),1,2.000000e-003); 
      
#line 254 "RN8211B_V20C_38D.PLN"
FORCE_I_PMU("RN8211B_V20C_38D.PLN",254,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,0,0,3.000000e+000,1,2.000000e-003);
      {strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
TEST_NO(33000005);
#line 255 "RN8211B_V20C_38D.PLN"
JUDGE_V_PMU("RN8211B_V20C_38D.PLN",255,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),4,1.220000e+000,1.290000e+000,2.000000e-003,16,_3500_AVE_MODE,5.000000e-005,_KEEP_CURRENT_F,_KEEP_CURRENT_F);}
      
#line 256 "RN8211B_V20C_38D.PLN"
FORCE_V_PMU("RN8211B_V20C_38D.PLN",256,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,0,3,1.000000e-003,1,2.000000e-003);
      
#line 257 "RN8211B_V20C_38D.PLN"
FORCE_V_PMU("RN8211B_V20C_38D.PLN",257,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,0,3,1.000000e-003,0,2.000000e-003);
      
#line 258 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",258,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),1,2.000000e-003);
    	}
relase_parall(parall_pin,20);}
}
#line 259 "RN8211B_V20C_38D.PLN"

#line 259 "RN8211B_V20C_38D.PLN"

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
#line 260 "RN8211B_V20C_38D.PLN"

   
      
#line 262 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",262,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),127,2.000000e-003);
      
#line 263 "RN8211B_V20C_38D.PLN"
RELAY_ON("RN8211B_V20C_38D.PLN",263,PIN_DEF,(PIN_DEF).GetTestPinno(),1,2.000000e-003); 
      
#line 264 "RN8211B_V20C_38D.PLN"
FORCE_I_PMU("RN8211B_V20C_38D.PLN",264,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,0,0,3.000000e+000,1,2.000000e-003);
      {strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
TEST_NO(33000006);
#line 265 "RN8211B_V20C_38D.PLN"
JUDGE_V_PMU("RN8211B_V20C_38D.PLN",265,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),4,1.650000e+000,1.980000e+000,2.000000e-003,16,_3500_AVE_MODE,5.000000e-005,_KEEP_CURRENT_F,_KEEP_CURRENT_F);}
      
#line 266 "RN8211B_V20C_38D.PLN"
FORCE_V_PMU("RN8211B_V20C_38D.PLN",266,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,0,3,1.000000e-003,1,2.000000e-003);
      
#line 267 "RN8211B_V20C_38D.PLN"
FORCE_V_PMU("RN8211B_V20C_38D.PLN",267,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,0,3,1.000000e-003,0,2.000000e-003);
      
#line 268 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",268,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),1,2.000000e-003);
    	}
relase_parall(parall_pin,20);}
}
#line 269 "RN8211B_V20C_38D.PLN"

#line 269 "RN8211B_V20C_38D.PLN"

    




















    POWER_OFF();
    u32 Version[DUT_COUNT] = {0};
    Judge_A_Or_D_Version(Version);
    
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,309);
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
#line 295 "RN8211B_V20C_38D.PLN"
      
      if(Version[SOCKET] == A_Version)
      {
        LDO18_trim_flag[SOCKET] = 0;
      }
      else if(Version[SOCKET] == D_Version)
      {
        LDO18_trim_flag[SOCKET] = 1;            
      }
      else
      {
        PLAN_RESULT_MS[SOCKET] = FAIL;
        TDO_PRINTF("SOCKET%d Chip Version (0x%08X)is unNormal,Need 0x%08X or 0x%08X;Please Check it!!!\n",SOCKET,Version[SOCKET],A_Version,D_Version);
      }
    		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 309 "RN8211B_V20C_38D.PLN"

#line 309 "RN8211B_V20C_38D.PLN"


}


__declspec(dllexport) int POWER_SWICTH() 
{


#line 314 "RN8211B_V20C_38D.PLN"

  POWER_ON(POWER_VOL);
  
#line 316 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",316,VBAT,(VBAT).GetTestPinno(),(VBAT).IsIOAllPins(),127,1.000000e-003);
  
#line 317 "RN8211B_V20C_38D.PLN"
RELAY_ON("RN8211B_V20C_38D.PLN",317,VBAT,(VBAT).GetTestPinno(),128,1.000000e-003);
  
#line 318 "RN8211B_V20C_38D.PLN"
FORCE_V_PPMU("RN8211B_V20C_38D.PLN",318,VBAT,(VBAT).GetTestPinno(),3.600000e+000,2147483647,7,MAXDOUBLE,-1,2.000000e-003); 
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
#line 319 "RN8211B_V20C_38D.PLN"

   
    
#line 321 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",321,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),127,2.000000e-003);
    
#line 322 "RN8211B_V20C_38D.PLN"
RELAY_ON("RN8211B_V20C_38D.PLN",322,PIN_DEF,(PIN_DEF).GetTestPinno(),1,2.000000e-003); 
    
#line 323 "RN8211B_V20C_38D.PLN"
FORCE_I_PMU("RN8211B_V20C_38D.PLN",323,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,3,1,6.000000e+000,1,2.000000e-003);
    {strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
TEST_NO(33000007);
#line 324 "RN8211B_V20C_38D.PLN"
JUDGE_V_PMU("RN8211B_V20C_38D.PLN",324,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),4,4.500000e+000,5.200000e+000,2.000000e-003,16,_3500_AVE_MODE,5.000000e-005,_KEEP_CURRENT_F,_KEEP_CURRENT_F);}
    
#line 325 "RN8211B_V20C_38D.PLN"
FORCE_V_PMU("RN8211B_V20C_38D.PLN",325,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,1,3,1.000000e-003,1,2.000000e-003);
    
#line 326 "RN8211B_V20C_38D.PLN"
FORCE_V_PMU("RN8211B_V20C_38D.PLN",326,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,1,3,1.000000e-003,0,2.000000e-003);
    
#line 327 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",327,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),1,2.000000e-003);
  	}
relase_parall(parall_pin,20);}
}
#line 328 "RN8211B_V20C_38D.PLN"

#line 328 "RN8211B_V20C_38D.PLN"

  
#line 329 "RN8211B_V20C_38D.PLN"
FORCE_V_MLDPS("RN8211B_V20C_38D.PLN",329,VCC,(VCC).GetTestPinno(),2.300000e+000,0,5,2.000000e-001,0,1,3.000000e-003, MAXINT, MAXINT, MAXINT, MAXINT);   
  
#line 330 "RN8211B_V20C_38D.PLN"
WAIT("RN8211B_V20C_38D.PLN",330,5.000000e-003);
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
#line 331 "RN8211B_V20C_38D.PLN"

   
    
#line 333 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",333,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),127,2.000000e-003);
    
#line 334 "RN8211B_V20C_38D.PLN"
RELAY_ON("RN8211B_V20C_38D.PLN",334,PIN_DEF,(PIN_DEF).GetTestPinno(),1,2.000000e-003); 
    
#line 335 "RN8211B_V20C_38D.PLN"
FORCE_I_PMU("RN8211B_V20C_38D.PLN",335,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,3,1,5.000000e+000,1,2.000000e-003);
    {strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
TEST_NO(33000008);
#line 336 "RN8211B_V20C_38D.PLN"
JUDGE_V_PMU("RN8211B_V20C_38D.PLN",336,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),4,3.400000e+000,3.800000e+000,2.000000e-003,16,_3500_AVE_MODE,5.000000e-005,_KEEP_CURRENT_F,_KEEP_CURRENT_F);}
    
#line 337 "RN8211B_V20C_38D.PLN"
FORCE_V_PMU("RN8211B_V20C_38D.PLN",337,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,1,3,1.000000e-003,1,2.000000e-003);
    
#line 338 "RN8211B_V20C_38D.PLN"
FORCE_V_PMU("RN8211B_V20C_38D.PLN",338,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,1,3,1.000000e-003,0,2.000000e-003);
    
#line 339 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",339,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),1,2.000000e-003);
  	}
relase_parall(parall_pin,20);}
}
#line 340 "RN8211B_V20C_38D.PLN"

#line 340 "RN8211B_V20C_38D.PLN"

  
#line 341 "RN8211B_V20C_38D.PLN"
FORCE_V_MLDPS("RN8211B_V20C_38D.PLN",341,VCC,(VCC).GetTestPinno(),3.100000e+000,0,5,2.000000e-001,0,1,3.000000e-003, MAXINT, MAXINT, MAXINT, MAXINT);   
  
#line 342 "RN8211B_V20C_38D.PLN"
WAIT("RN8211B_V20C_38D.PLN",342,5.000000e-003);
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
#line 343 "RN8211B_V20C_38D.PLN"

   
    
#line 345 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",345,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),127,2.000000e-003);
    
#line 346 "RN8211B_V20C_38D.PLN"
RELAY_ON("RN8211B_V20C_38D.PLN",346,PIN_DEF,(PIN_DEF).GetTestPinno(),1,2.000000e-003); 
    
#line 347 "RN8211B_V20C_38D.PLN"
FORCE_I_PMU("RN8211B_V20C_38D.PLN",347,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,3,1,5.000000e+000,1,2.000000e-003);
    {strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
strcpy(USE_TYPE ,"")  ;
USE_UNIT = 0  ;
TEST_NO(33000009);
#line 348 "RN8211B_V20C_38D.PLN"
JUDGE_V_PMU("RN8211B_V20C_38D.PLN",348,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),4,2.900000e+000,MAXDOUBLE,2.000000e-003,16,_3500_AVE_MODE,5.000000e-005,_KEEP_CURRENT_F,_KEEP_CURRENT_F);}
    
#line 349 "RN8211B_V20C_38D.PLN"
FORCE_V_PMU("RN8211B_V20C_38D.PLN",349,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,1,3,1.000000e-003,1,2.000000e-003);
    
#line 350 "RN8211B_V20C_38D.PLN"
FORCE_V_PMU("RN8211B_V20C_38D.PLN",350,CH2PMU(PIN_DEF.pinno,PIN_DEF.pin),0.000000e+000,1,3,1.000000e-003,0,2.000000e-003);
    
#line 351 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",351,PIN_DEF,(PIN_DEF).GetTestPinno(),(PIN_DEF).IsIOAllPins(),1,2.000000e-003);
  	}
relase_parall(parall_pin,20);}
}
#line 352 "RN8211B_V20C_38D.PLN"

#line 352 "RN8211B_V20C_38D.PLN"

  double meas_value[DUT_COUNT]={0};
  
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,363);
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
#line 355 "RN8211B_V20C_38D.PLN"

     meas_value[SOCKET]= RESULT_PIN("RN8211B_V20C_38D.PLN",356,((LUINT)1<<SOCKET),VSWO,(VSWO).GetTestPinno(),0);
     {strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
strcpy(USE_TYPE ,"V")  ;
USE_UNIT = 0  ;
TEST_NO(33000010);
#line 357 "RN8211B_V20C_38D.PLN"
JUDGE_VARIABLE("RN8211B_V20C_38D.PLN",357,((LUINT)1<<SOCKET),meas_value[SOCKET],2.900000e+000,3.100000e+000,"VSWO");}
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
#line 363 "RN8211B_V20C_38D.PLN"

#line 363 "RN8211B_V20C_38D.PLN"

  
  POWER_OFF();
  
#line 366 "RN8211B_V20C_38D.PLN"
FORCE_V_PPMU("RN8211B_V20C_38D.PLN",366,VBAT,(VBAT).GetTestPinno(),0.000000e+000,2147483647,7,MAXDOUBLE,-1,2.000000e-003);
  
#line 367 "RN8211B_V20C_38D.PLN"
RELAY_OFF("RN8211B_V20C_38D.PLN",367,VBAT,(VBAT).GetTestPinno(),(VBAT).IsIOAllPins(),128,1.000000e-003);
}


__declspec(dllexport) int last_test() 
{


#line 371 "RN8211B_V20C_38D.PLN"

  cout<<"Test End"<<endl;
}                                         
	__declspec(dllexport) int LOADPIN()
{
IO_ALLPINS.GetFunc();
}


long
#line 3 "IC_Ctrl.pln"
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
  
#line 17 "IC_Ctrl.pln"
FORCE_V_MLDPS("IC_Ctrl.pln",17,VCC,(VCC).GetTestPinno(),0.000000e+000,0,5,3.000000e-001,0,1,2.000000e-003, MAXINT, MAXINT, MAXINT, MAXINT);
  
#line 18 "IC_Ctrl.pln"
WAIT("IC_Ctrl.pln",18,5.000000e-003);
  
#line 19 "IC_Ctrl.pln"
FORCE_V_MLDPS("IC_Ctrl.pln",19,VCC,(VCC).GetTestPinno(),volt,0,5,3.000000e-001,0,1,2.000000e-003, MAXINT, MAXINT, MAXINT, MAXINT);
  
#line 20 "IC_Ctrl.pln"
WAIT("IC_Ctrl.pln",20,5.000000e-003);
}

void POWER_OFF()
{
  
#line 25 "IC_Ctrl.pln"
FORCE_V_MLDPS("IC_Ctrl.pln",25,VCC,(VCC).GetTestPinno(),0.000000e+000,0,5,3.000000e-001,0,1,3.000000e-003, MAXINT, MAXINT, MAXINT, MAXINT);
  
#line 26 "IC_Ctrl.pln"
WAIT("IC_Ctrl.pln",26,2.000000e-003);
  
#line 27 "IC_Ctrl.pln"
FORCE_V_MLDPS("IC_Ctrl.pln",27,VCC,(VCC).GetTestPinno(),0.000000e+000,0,5,3.000000e-001,0,0,3.000000e-003, MAXINT, MAXINT, MAXINT, MAXINT);
  #line 1 "RN8211B_V20C_38D_body.cpp"
if(enter_power_down_fail_site){          
  LUINT tmpmask = CPIN::GetMask(),tmpfailmask = 0x0;                    
  for(int _gg_=0;_gg_<DF_MAX_Site_Count;_gg_++) {                    
    if((PLAN_RESULT_MS[_gg_]==FAIL) && TrueOrFalse(tmpmask,_gg_))                    
      tmpfailmask |= (LUINT)0x1<<_gg_;                    
  }                    
  CPIN::SetMask(tmpfailmask);          
  #line 28 "IC_Ctrl.pln"                    
  LEVELS("IC_Ctrl.pln",28,rst_lvl,2.000000e-003,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),1);          
  #line 1 "RN8211B_V20C_38D_body.cpp"                    
  CPIN::SetMask(tmpmask);          
}           
  else{          
  #line 28 "IC_Ctrl.pln"          
  LEVELS("IC_Ctrl.pln",28,rst_lvl,2.000000e-003,NULL,0,0);          
  #line 1 "RN8211B_V20C_38D_body.cpp"          
  }          
  #line 28 "IC_Ctrl.pln"
; 
  
#line 29 "IC_Ctrl.pln"
RELAY_OFF("IC_Ctrl.pln",29,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),(IO_ALLPINS).IsIOAllPins(),127,2.000000e-003);
}

void RESET_SWD()
{
  int Data[37] = {0};
  int result[5] = {0};
  u32 IDCODE = 0;
  
#line 37 "IC_Ctrl.pln"
SET_CLOG_MODE("IC_Ctrl.pln",37,SWDIO,(SWDIO).GetTestPinno(),3,1,3,1024,1,1,2147483647); 
  {TEST_NO(33000011);
#line 38 "IC_Ctrl.pln"
JUDGE_PAT("IC_Ctrl.pln",38,Reset_SWD,__Reset_SWD,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,(__Reset_SWD).Getpinmode(),(__Reset_SWD).Getpin2x(),(__Reset_SWD).GetValidpin(),(__Reset_SWD).Gettestno(),(__Reset_SWD).Getchkmode());} 
  
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,72);
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
#line 40 "IC_Ctrl.pln"

    for(int j = 0;j < 16;j++)
    {
      if(j<4) 
        result[j] = 0;
      Data[j]=0;
    }
    IDCODE = 0;

    
#line 49 "IC_Ctrl.pln"
READ_CAPTURE_MEM_BLOCK("IC_Ctrl.pln",49,(SWDIO).GetSitePins(SOCKET),(SWDIO).GetMemberno(),0,36,Data);
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
#line 72 "IC_Ctrl.pln"

#line 72 "IC_Ctrl.pln"

  
#line 73 "IC_Ctrl.pln"
SET_CLOG_MODE("IC_Ctrl.pln",73,SWDIO,(SWDIO).GetTestPinno(),3,0,3,1024,1,1,2147483647);
  
}

void swd_rate(double  rate)
{
  
   
#line 80 "IC_Ctrl.pln"
RATEN("IC_Ctrl.pln",80,1,rate);  
   
#line 81 "IC_Ctrl.pln"
MARKERN("IC_Ctrl.pln",81,1,SWDIO,(SWDIO).GetTestPinno(),(SWDIO).IsIOAllPins(),rate*5.000000e-001,rate*6.000000e-001,-1,0.000000e+000,-1,rate*5.000000e-001,-1);
   
#line 82 "IC_Ctrl.pln"
MARKERN("IC_Ctrl.pln",82,1,SWDCLK,(SWDCLK).GetTestPinno(),(SWDCLK).IsIOAllPins(),rate*2.500000e-001,rate*7.500000e-001,-1,0.000000e+000,-1,rate*5.000000e-001,-1);


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
    TDO_PRINTF("Parity bit=%c\n",temp_data[32]);  
    
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,138);
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
#line 128 "IC_Ctrl.pln"

      for(int i=0;i<33;i++)
      {
        
        
#line 132 "IC_Ctrl.pln"
zz_20150729_WRITE_PAT_MEM("IC_Ctrl.pln",132, (SWDIO).GetSitePins(SOCKET), (SWDIO).GetMemberno(),wstart_data+(i)/(wstart_data.Getpinmode()==_IO2X_||wstart_data.Getpinmode()==_DBL_IO2X_?2:1),wstart_data.Getpinmode()==_IO2X_||wstart_data.Getpinmode()==_DBL_IO2X_?i%2:0,0,temp_data+i);
        if(i<8)
        {
          
#line 135 "IC_Ctrl.pln"
zz_20150729_WRITE_PAT_MEM("IC_Ctrl.pln",135, (SWDIO).GetSitePins(SOCKET), (SWDIO).GetMemberno(),wstart_head+(i)/(wstart_head.Getpinmode()==_IO2X_||wstart_head.Getpinmode()==_DBL_IO2X_?2:1),wstart_head.Getpinmode()==_IO2X_||wstart_head.Getpinmode()==_DBL_IO2X_?i%2:0,0,temp_head+i);
        }
      }
    		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 138 "IC_Ctrl.pln"

#line 138 "IC_Ctrl.pln"

    {TEST_NO(33000012);
#line 139 "IC_Ctrl.pln"
JUDGE_PAT("IC_Ctrl.pln",139,WIRTE_WORD,__WIRTE_WORD,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,(__WIRTE_WORD).Getpinmode(),(__WIRTE_WORD).Getpin2x(),(__WIRTE_WORD).GetValidpin(),(__WIRTE_WORD).Gettestno(),(__WIRTE_WORD).Getchkmode());}
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
#line 163 "IC_Ctrl.pln"

    for(int i=0;i<8;i++){
      
#line 165 "IC_Ctrl.pln"
zz_20150729_WRITE_PAT_MEM("IC_Ctrl.pln",165, (SWDIO).GetSitePins(SOCKET), (SWDIO).GetMemberno(),Capture_addr+(i)/(Capture_addr.Getpinmode()==_IO2X_||Capture_addr.Getpinmode()==_DBL_IO2X_?2:1),Capture_addr.Getpinmode()==_IO2X_||Capture_addr.Getpinmode()==_DBL_IO2X_?i%2:0,0,temp_head+i);
    }
  		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 167 "IC_Ctrl.pln"

#line 167 "IC_Ctrl.pln"


  
#line 169 "IC_Ctrl.pln"
SET_CLOG_MODE("IC_Ctrl.pln",169,SWDIO,(SWDIO).GetTestPinno(),3,1,3,1024,1,1,2147483647);

  {TEST_NO(33000013);
#line 171 "IC_Ctrl.pln"
JUDGE_PAT("IC_Ctrl.pln",171,CAPTURE_WORD,__CAPTURE_WORD,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,(__CAPTURE_WORD).Getpinmode(),(__CAPTURE_WORD).Getpin2x(),(__CAPTURE_WORD).GetValidpin(),(__CAPTURE_WORD).Gettestno(),(__CAPTURE_WORD).Getchkmode());}
  
  
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,196);
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
#line 174 "IC_Ctrl.pln"
  
    for(int j = 0;j < 33;j++)
    {
      if(j<5) 
        result[SOCKET][j] = 0;
      Data[j]=0;
    }
    
#line 181 "IC_Ctrl.pln"
READ_CAPTURE_MEM_BLOCK("IC_Ctrl.pln",181,(SWDIO).GetSitePins(SOCKET),(SWDIO).GetMemberno(),0,36,Data);
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
#line 196 "IC_Ctrl.pln"

#line 196 "IC_Ctrl.pln"
 
  
#line 197 "IC_Ctrl.pln"
SET_CLOG_MODE("IC_Ctrl.pln",197,SWDIO,(SWDIO).GetTestPinno(),3,0,3,1024,1,0,2147483647);
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
    TDO_PRINTF("Parity bit=%c\n",temp_data[32]);  
    
{
SET_STATEMENT_INFO(_ID_SOCKET_INC,252);
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
#line 242 "IC_Ctrl.pln"

      for(int i=0;i<33;i++)
      {
        
        
#line 246 "IC_Ctrl.pln"
zz_20150729_WRITE_PAT_MEM("IC_Ctrl.pln",246, (SWDIO).GetSitePins(SOCKET), (SWDIO).GetMemberno(),wread_data+(i)/(wread_data.Getpinmode()==_IO2X_||wread_data.Getpinmode()==_DBL_IO2X_?2:1),wread_data.Getpinmode()==_IO2X_||wread_data.Getpinmode()==_DBL_IO2X_?i%2:0,0,temp_data+i);
        if(i<8)
        {
          
#line 249 "IC_Ctrl.pln"
zz_20150729_WRITE_PAT_MEM("IC_Ctrl.pln",249, (SWDIO).GetSitePins(SOCKET), (SWDIO).GetMemberno(),wread_addr+(i)/(wread_addr.Getpinmode()==_IO2X_||wread_addr.Getpinmode()==_DBL_IO2X_?2:1),wread_addr.Getpinmode()==_IO2X_||wread_addr.Getpinmode()==_DBL_IO2X_?i%2:0,0,temp_head+i);
        }
      }
    		socket_freeze(curfreezepin,curpinno,_ON_,NULL);
	}
}
socket_freeze(tmpfreezepin,tmppinno,_OFF_,tmppinmode);
CPIN::SetMask(tmpMask);
}
#line 252 "IC_Ctrl.pln"

#line 252 "IC_Ctrl.pln"

    {TEST_NO(33000014);
#line 253 "IC_Ctrl.pln"
JUDGE_PAT("IC_Ctrl.pln",253,READ_WORD,__READ_WORD,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,MAXINT,(__READ_WORD).Getpinmode(),(__READ_WORD).Getpin2x(),(__READ_WORD).GetValidpin(),(__READ_WORD).Gettestno(),(__READ_WORD).Getchkmode());}
  }
  else
  {
      TDO_PRINTF("This is NO Read command(0x%02X)\n",Head_data);     
  }
}

void Judge_A_Or_D_Version(u32* data)
{
  POWER_ON(POWER_VOL);
  #line 1 "RN8211B_V20C_38D_body.cpp"
if(enter_power_down_fail_site){          
  LUINT tmpmask = CPIN::GetMask(),tmpfailmask = 0x0;                    
  for(int _gg_=0;_gg_<DF_MAX_Site_Count;_gg_++) {                    
    if((PLAN_RESULT_MS[_gg_]==FAIL) && TrueOrFalse(tmpmask,_gg_))                    
      tmpfailmask |= (LUINT)0x1<<_gg_;                    
  }                    
  CPIN::SetMask(tmpfailmask);          
  #line 264 "IC_Ctrl.pln"                    
  LEVELS("IC_Ctrl.pln",264,UID_lvl,3.000000e-003,IO_ALLPINS,(IO_ALLPINS).GetTestPinno(),1);          
  #line 1 "RN8211B_V20C_38D_body.cpp"                    
  CPIN::SetMask(tmpmask);          
}           
  else{          
  #line 264 "IC_Ctrl.pln"          
  LEVELS("IC_Ctrl.pln",264,UID_lvl,3.000000e-003,NULL,0,0);          
  #line 1 "RN8211B_V20C_38D_body.cpp"          
  }          
  #line 264 "IC_Ctrl.pln"
;
  swd_rate(1.000000e-006);
  
#line 266 "IC_Ctrl.pln"
PIN_MODE("IC_Ctrl.pln",266,SWDIO,(SWDIO).GetTestPinno(),(SWDIO).IsIOAllPins(),0,0,1,0,0);
  
#line 267 "IC_Ctrl.pln"
PIN_MODE("IC_Ctrl.pln",267,SWDCLK,(SWDCLK).GetTestPinno(),(SWDCLK).IsIOAllPins(),2,0,1,0,0);
  
#line 268 "IC_Ctrl.pln"
RELAY_OFF("IC_Ctrl.pln",268,SWD_PINS,(SWD_PINS).GetTestPinno(),(SWD_PINS).IsIOAllPins(),127,1.000000e-003);
  
#line 269 "IC_Ctrl.pln"
RELAY_ON("IC_Ctrl.pln",269,SWD_PINS,(SWD_PINS).GetTestPinno(),4,2.000000e-003);
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
