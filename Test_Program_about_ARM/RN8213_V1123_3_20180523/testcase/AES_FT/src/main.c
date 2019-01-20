/****************************************Copyright (c)****************************************************
**                            REnergy Micro-Electronics Co.,LTD.
**
**                                 http://www.renergy-me.cn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified:       2011-03-16
** Last Version:        V1.0
** Descriptions:        User Programn Entry
**                      
**--------------------------------------------------------------------------------------------------------
** Created by:          
** Created date:        2011-03-16
** Version:             V1.0
** Descriptions:        first creation
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
**
*********************************************************************************************************/

#include "rn821x.h"
#include "common.h"
#include <stdio.h>
#include "sysctrl.h"

#define NIST_MODE_ECB 0
#define NIST_MODE_CBC 1
#define NIST_MODE_CTR 2
#define KEYLEN_128B   0
#define KEYLEN_192B   1
#define KEYLEN_256B   2
#define KLEN_IW       (4 + (1 << KLEN))
#define TWIST8(x1,x2,x3,x4,x5,x6,x7,x8) {x8,x7,x6,x5,x4,x3,x2,x1}
#define TWIST6(x1,x2,x3,x4,x5,x6)       {x6,x5,x4,x3,x2,x1}
#define TWIST4(x1,x2,x3,x4)             {x4,x3,x2,x1}

#define  RES_OFFSET     0x37f8
#define  CHIPID_OFFSET  0x37f0

#define  TEST_START       0xa5a5a5a5
#define  TEST_FAIL        0x12345ae5
#define  TEST_PASS        0x98765432
void qc_fail(void);

volatile uint32_t  remap;
volatile uint32_t  * sram_res;
volatile uint32_t  * sram_chipid;
/*
[Keylen = 128]
[IVlen = 1024]
[PTlen = 408]
[AADlen = 720]
[Taglen = 32]

Count = 0
Key = 2633d1781ce54f74ac609a5b5209a01f
IV = 7d0e90b7e9f36f760d2dcbd66f352df45f3917afdbe1d0a89cc44be0bd85cf8bf75edbdd33f1d16dad02824d81389210b0f146f3df63f9232d7035eb9e8297a09474985b3e038a5fa6840155d8848fc7c53061ba0f442b84408660a997176ca5bf3473103fd3c9a1de2580b9e539af872259ecae925a8ef50f5a176a069b1fb8
PT = ae695828625b264e0b13d3c9a539f2cf306a7501cdd35b817b699b2d7c25cf20d2dceec3fa883019db807272fddfdca8e7f672
AAD = 584c3cad3035d1427d6f5f1b261e97a5ea7d97c0b88cedf3b1aa5e21e5916805a63964eab4449d8806e7af60618465cf39f82769b7528bba9bb9c04992cd7b9e26efe9be38e1bfeeb41678c52d5ba3508fd7a2b1e8478505bfde
CT = fbc32a56885100a36c276ff368db9236906021a8cc7500f2b3e78a6ca01546827073ff1103145f139f4d116eb47b84e33c7160
Tag = 49589b3a
*/

/*
Count = 0
Key= 0x2633d178,0x1ce54f74,0xac609a5b,0x5209a01f
IV = 0x7d0e90b7,0xe9f36f76,0x0d2dcbd6,0x6f352df4,0x5f3917af,0xdbe1d0a8,0x9cc44be0,0xbd85cf8b,0xf75edbdd,0x33f1d16d,
     0xad02824d,0x81389210,0xb0f146f3,0xdf63f923,0x2d7035eb,0x9e8297a0,0x9474985b,0x3e038a5f,0xa6840155,0xd8848fc7,
     0xc53061ba,0x0f442b84,0x408660a9,0x97176ca5,0xbf347310,0x3fd3c9a1,0xde2580b9,0xe539af87,0x2259ecae,0x925a8ef5,
     0x0f5a176a,0x069b1fb8,0x00000000,0x00000000,0x00000000,0x00000400
PT'= 0xae695828,0x625b264e,0x0b13d3c9,0xa539f2cf,0x306a7501,0xcdd35b81,0x7b699b2d,0x7c25cf20,0xd2dceec3,0xfa883019,0xdb807272,0xfddfdca8,0xe7f672     
PT = 0xae6958,0x28625b26,0x4e0b13d3,0xc9a539f2,0xcf306a75,0x01cdd35b,0x817b699b,0x2d7c25cf,0x20d2dcee,0xc3fa8830,0x19db8072,0x72fddfdc,0xa8e7f672
AAD= 0x584c3cad,0x3035d142,0x7d6f5f1b,0x261e97a5,0xea7d97c0,0xb88cedf3,0xb1aa5e21,0xe5916805,0xa63964ea,0xb4449d88,
     0x06e7af60,0x618465cf,0x39f82769,0xb7528bba,0x9bb9c049,0x92cd7b9e,0x26efe9be,0x38e1bfee,0xb41678c5,0x2d5ba350,
     0x8fd7a2b1,0xe8478505,0xbfde0000,0x00000000
CT'= 0xfbc32a56,0x885100a3,0x6c276ff3,0x68db9236,0x906021a8,0xcc7500f2,0xb3e78a6c,0xa0154682,0x7073ff11,0x03145f13,0x9f4d116e,0xb47b84e3,0x3c7160     
CT = 0x0,0x0,0x0,0x00fbc32a,0x56885100,0xa36c276f,0xf368db92,0x36906021,0xa8cc7500,0xf2b3e78a,0x6ca01546,0x827073ff,0x1103145f,0x139f4d11,0x6eb47b84,0xe33c7160
Tag = 0x49589b3a
*/

#define AES_MODE      NIST_MODE_CTR
#define KLEN          KEYLEN_128B
#define INITIALV      TWIST4(0xf0f1f2f3,0xf4f5f6f7,0xf8f9fafb,0xfcfdfeff)
#define AES_KEY       TWIST4(0x2633d178,0x1ce54f74,0xac609a5b,0x5209a01f)
#define PLAINTEXT     TWIST4(0x6bc1bee2,0x2e409f96,0xe93d7e11,0x7393172a)
#define CIPHERTEXT    TWIST4(0x874d6191,0xb620e326,0x1bef6864,0x990db6ce)
#define ZERO128       TWIST4(0x00000000,0x00000000,0x00000000,0x00000000)
#define HASHIV        TWIST4(0x3c819d9a,0x9bed0876,0x15030b65,0x00000001)
#define HASHTAG       TWIST4(0x49589b3a,0x0,0x0,0x0)



typedef struct{
 volatile unsigned int ctrl;
 volatile unsigned int sta;
 volatile unsigned int rev[2];
 volatile unsigned int key[8];
 volatile unsigned int pt[4];
 volatile unsigned int iv[4];
 volatile unsigned int ct[4];
} s_aes_codec;//,* ps_aes_codec;

typedef struct{
 volatile unsigned int ctrl;
 volatile unsigned int sta;
 volatile unsigned int rev[2];
 volatile unsigned int ht[4];
 volatile unsigned int at[4];
 volatile unsigned int ct[4];
 volatile unsigned int rt[4];
} s_ghash;//,* ps_ghash;

#define AES_ENCRYPT_BASE_ADDR  0x5000D000
#define AES_DECRYPT_BASE_ADDR  0x5000E000
#define GHASH_BASE_ADDR  0x5000F000
#define AES_ENCRYPT            ((s_aes_codec *)(AES_ENCRYPT_BASE_ADDR))
#define AES_DECRYPT            ((s_aes_codec *)(AES_DECRYPT_BASE_ADDR))
#define GHASH                  ((s_ghash *)(GHASH_BASE_ADDR))

void enc_init(unsigned int * key,unsigned int * pt,unsigned int * ct, unsigned int * iv);
void enc_start(unsigned int cm,unsigned int kl,unsigned int mode,unsigned int be,unsigned int ie,unsigned int pde,unsigned int cde);
void enc_stop(void);
void enc_wait_for_idle(void);
void enc_wait_for_bc_done(void);
uint32_t enc_verify(unsigned int * ct);
void enc_set_pt(unsigned int * pt);
void enc_set_iv(unsigned int * iv);

void dec_init(unsigned int * key,unsigned int * pt,unsigned int * ct, unsigned int * iv);
void dec_start(unsigned int cm,unsigned int kl,unsigned int mode,unsigned int be,unsigned int ie,unsigned int pde,unsigned int cde);
void dec_stop(void);
void dec_wait_for_idle(void);
void dec_wait_for_bc_done(void);
void dec_wait_for_kb_done(void);
void dec_verify(unsigned int * pt);
void dec_set_ct(unsigned int * ct);
void dec_set_iv(unsigned int * iv);

void ghash_stop(void);
void ghash_wait_for_idle(void);

//hash begin
void ghash_init(unsigned int * ht,unsigned int * at,unsigned int * ct, unsigned int * rt) {
  unsigned int i = 0;

  for (i = 0; i < 4; i ++) {
    GHASH->ht[i] = ht[i];
    GHASH->at[i] = at[i];
    GHASH->ct[i] = ct[i];
  }
}

void ghash_start(unsigned int be) {
  ghash_stop();
  GHASH->ctrl = 1 | ((be << 1) & 0x2);
}

void ghash_stop(void) {
  GHASH->ctrl = 0;
  ghash_wait_for_idle();
}

void ghash_wait_for_bc_done(void) {
  while(((GHASH->sta) & 1) == 0)
    ;
}

void ghash_wait_for_idle(void) {
  while(((GHASH->sta) & 2) != 0)
    ;
}

uint32_t ghash_verify(unsigned int * rt) {
  unsigned int i = 0;
	uint32_t res;
  //printf("Verifying last ghash result ...\n");
  ghash_wait_for_bc_done();
  for (i = 0; i < 4; i ++) {
    if ((GHASH->rt[i]) != rt[i]) {
      //printf("\terror found on No.%d data (exp:0x%08x,fnd:0x%08x)\n",i,rt[i],GHASH->rt[i]);
			res = 1;
    } 
  }
  return res;	
}
 
void ghash_getrt(unsigned int * rt) {
  unsigned int i = 0;
  ghash_wait_for_bc_done();
  for (i = 0; i < 4; i ++) {
    rt[i] = GHASH->rt[i]; 
  }
}

void ghash_set_ct(unsigned int * ct) {
  unsigned int i = 0;

  for (i = 0; i < 4; i ++) {
    GHASH->ct[i] = ct[i];
  }
} 

void ghash_set_at(unsigned int * at) {
  unsigned int i = 0;

  for (i = 0; i < 4; i ++) {
    GHASH->at[i] = at[i];
  }
}

//hash end

void enc_init(unsigned int * key,unsigned int * pt,unsigned int * ct, unsigned int * iv) {
  unsigned int i = 0;

  for (i = 0; i < KLEN_IW; i ++) { 
    AES_ENCRYPT->key[i] = key[i];
  } 
  for (i = 0; i < 4; i ++) {
    AES_ENCRYPT->pt[i] = pt[i];
    AES_ENCRYPT->iv[i] = iv[i];
  }
}

void enc_start(unsigned int cm,unsigned int kl,unsigned int mode,unsigned int be,unsigned int ie,unsigned int pde,unsigned int cde) {
  enc_stop();
  AES_ENCRYPT->ctrl = 1 | ((cm << 1) & 0x2) | ((kl << 2) & 0xC) | ((mode << 4) & 0x30) | ((be << 6) & 0x40) | ((ie << 7) & 0x80) | ((pde << 8) & 0x100)| ((cde << 9) & 0x200);
}

void enc_stop(void) {
  AES_ENCRYPT->ctrl = 0;
  enc_wait_for_idle();
}

void enc_wait_for_bc_done(void) {
  while(((AES_ENCRYPT->sta) & 1) == 0)
    ;
}

void enc_wait_for_idle(void) {
  while(((AES_ENCRYPT->sta) & 2) != 0)
    ; 
}

uint32_t enc_verify(unsigned int * ct) {
  unsigned int i = 0;
	uint32_t  res = 0;
  //printf("Verifying last block encryption result ...\n");
  enc_wait_for_bc_done();
  for (i = 0; i < 4; i ++) {
    if ((AES_ENCRYPT->ct[i]) != ct[i]) {
      //printf("\terror found on No.%d data (exp:0x%08x,fnd:0x%08x)\n",i,ct[i],AES_ENCRYPT->ct[i]);
			res = 1;
    }
  }
	return res;
}

void enc_getct(unsigned int * ct) {
  unsigned int i = 0;
  enc_wait_for_bc_done();
  for (i = 0; i < 4; i ++) {
    ct[i] = AES_ENCRYPT->ct[i]; 
  }
} 


void enc_set_pt(unsigned int * pt) {
  unsigned int i = 0;

  for (i = 0; i < 4; i ++) {
    AES_ENCRYPT->pt[i] = pt[i];
  }
}

void enc_set_iv(unsigned int * iv) {
  unsigned int i = 0;

  for (i = 0; i < 4; i ++) {
    AES_ENCRYPT->iv[i] = iv[i];
  }
}



/*********************************************************************************************************
** Function name:       main
** Descriptions:        User Programn Entry Example
** input parameters:    none
** output parameters:   none
** Returned value:      none/never return
*********************************************************************************************************/
//--------------------------------------------------------------------------
//regctl writing
//Register spictl (offset: 0x00)
//{ 8'b{clkdiv}, 2'b{width}, scsn_dis, cpha, 
//   cpol,lmsb,cole,erre,tre,mas_or_slv,spi_en }
//--------------------------------------------------------------------------
int main (void)
{
  unsigned int key[KLEN_IW] = AES_KEY; 
  unsigned int pt[4]        = PLAINTEXT;
  unsigned int ct[4]        ;
  unsigned int mode         = AES_MODE;
  unsigned int kl           = KLEN;
  unsigned int cm           = 1;
  unsigned int be           = 0;
  unsigned int ie           = 0;
  unsigned int pde          = 0;
  unsigned int cde          = 0;
  
  unsigned int hash_key[4];
  unsigned int at[4]       ;
  unsigned int rt[4]; 
  unsigned int tag[4]       = HASHTAG;
  unsigned int i,j;
  
  unsigned int pt_init[16] = {0xae695828,0x625b264e,0x0b13d3c9,0xa539f2cf,0x306a7501,0xcdd35b81,0x7b699b2d,0x7c25cf20,0xd2dceec3,0xfa883019,0xdb807272,0xfddfdca8,0xe7f67200,0x0,0x0,0x0};
  unsigned int ct_init[16] = {0xfbc32a56,0x885100a3,0x6c276ff3,0x68db9236,0x906021a8,0xcc7500f2,0xb3e78a6c,0xa0154682,0x7073ff11,0x03145f13,0x9f4d116e,0xb47b84e3,0x3c716000,0x0,0x0,0x0};
  unsigned int iv[36] = {0x7d0e90b7,0xe9f36f76,0x0d2dcbd6,0x6f352df4,0x5f3917af,0xdbe1d0a8,0x9cc44be0,0xbd85cf8b,0xf75edbdd,0x33f1d16d,
     0xad02824d,0x81389210,0xb0f146f3,0xdf63f923,0x2d7035eb,0x9e8297a0,0x9474985b,0x3e038a5f,0xa6840155,0xd8848fc7,
     0xc53061ba,0x0f442b84,0x408660a9,0x97176ca5,0xbf347310,0x3fd3c9a1,0xde2580b9,0xe539af87,0x2259ecae,0x925a8ef5,
     0x0f5a176a,0x069b1fb8,0x00000000,0x00000000,0x00000000,0x00000400};
  
  unsigned int aad[24] = {0x584c3cad,0x3035d142,0x7d6f5f1b,0x261e97a5,0xea7d97c0,0xb88cedf3,0xb1aa5e21,0xe5916805,0xa63964ea,0xb4449d88,
                          0x06e7af60,0x618465cf,0x39f82769,0xb7528bba,0x9bb9c049,0x92cd7b9e,0x26efe9be,0x38e1bfee,0xb41678c5,0x2d5ba350,
                          0x8fd7a2b1,0xe8478505,0xbfde0000,0x00000000};
  unsigned int length_array[4] = {0x0,0x2d0,0x0,0x198};
  
  unsigned int j0[4];
	
  //systemInit(0);
  //uart0Init(115200); 
  
  * ((unsigned int *)0x40034030) = 0x82;
  * ((unsigned int *)0x4003401C) = 0xa589ffff;
  * ((unsigned int *)0x40034020) = 0xa580ffff;

  remap = (SYSCTL->MAP_CTRL & 0x3);
	sram_res = (uint32_t *)(((remap == 2) ? 0x0 : 0x10000000) + RES_OFFSET);
	*sram_res = TEST_START;
	sram_chipid = (uint32_t *)(((remap == 2) ? 0x0 : 0x10000000) + CHIPID_OFFSET);
	SYSCTL->SYS_PS = 0x82;
	SYSCTL->OSC_CTRL2 = 0x13;
	SysModeChg(3, 0);
  //printf("GCM Basic Test Starts\n");

//Get Hash Key  
  mode = NIST_MODE_ECB;
  kl   = KEYLEN_128B;
  cm   = 1;
  
  for(i=0;i<4;i++)
  {
    pt[i] = 0x0;
  }
  enc_init(key,pt,ct,iv);
  enc_start(cm,kl,mode,be,ie,pde,cde);
  enc_getct(hash_key);
  
//Get J0
/*
  length = IV_LEN;
  if(length == 96)
  {
    for(i=3;i>0;i--)
    {
      j0[i] = iv[3-i];
    }
    j0[0] = 0x00000001;
  }
  else
  {
  }  
*/
  for(i=0;i<4;i++)
  {
    at[i] = 0x0;
  } 

  for(j=0;j<9;j++)
  { 
    for(i=0;i<4;i++)
    {
      ct[i] = iv[j*4+(3-i)];
    }
    
    ghash_stop();
    ghash_init(hash_key,at,ct,rt);
    ghash_start(1);
    ghash_getrt(at);
  }
  
  for(i=0;i<4;i++)
  {
    j0[i] = at[i];
  }

//Get CT  
  mode = NIST_MODE_CTR;
  kl   = KEYLEN_128B;
  cm   = 1;
  
  
  //for(j=0;j<4;j++)
  for(j=0;j<3;j++)   //TEMP
  {
    j0[0] = j0[0] + 1;
    
    for(i=0;i<4;i++)
    {
      pt[i] = pt_init[j*4+(3-i)];
      ct[i] = ct_init[j*4+(3-i)];
    }
    
    enc_stop();
    enc_init(key,pt,ct,j0);
    enc_start(cm,kl,mode,be,ie,pde,cde);
    if(enc_verify(ct))
		{
		  qc_fail();	
		}
  }
  
  j0[0] = j0[0] - 4;
  
//Get ....  

  for(i=0;i<3;i++)
  {
    at[i] = 0x0;
  }  
  
  for(j=0;j<11;j++)
  { 
    if(j<6)
    {
      for(i=0;i<4;i++)
      {
        ct[i] = aad[j*4+(3-i)];
      }
    }
    
    if((j>=6) && (j<10))
    {
      for(i=0;i<4;i++)
      {
        ct[i] = ct_init[(j-6)*4+(3-i)];
      }      
    }
   
    if(j>=10)
    {
      for(i=0;i<4;i++)
      {
        ct[i] = length_array[(j-10)*4+(3-i)];
      }
    }    
     
    ghash_stop();
    ghash_init(hash_key,at,ct,rt);
    ghash_start(1);
    ghash_getrt(at);
  }  
  
  mode = NIST_MODE_CTR;
  kl   = KEYLEN_128B;
  cm   = 1;
    
  enc_stop();
  enc_init(key,at,ct,j0);
  enc_start(cm,kl,mode,be,ie,pde,cde);
// 	if(enc_verify(tag))  //TEMP
// 	{
// 		exitFail(1);	
// 	}	
                                      
	*sram_res = TEST_PASS;
  while(1)
  {
    WDT->EN = 0xbb;
  }	
}
/*****************main end*********************/

void qc_fail(void)
{
  *sram_res = TEST_FAIL;
  while(1)
  {
    WDT->EN = 0xbb;
  }
}

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
