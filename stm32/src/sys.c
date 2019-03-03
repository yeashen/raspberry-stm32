#include "sys.h" 
//ÉèÖÃÏòÁ¿±íÆ«ÒÆµØÖ·
//NVIC_VectTab:»ùÖ·
//Offset:Æ«ÒÆÁ¿			 
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset)	 
{ 	   	 
	SCB->VTOR = NVIC_VectTab|(Offset & (u32)0x1FFFFF80);//ÉèÖÃNVICµÄÏòÁ¿±íÆ«ÒÆ¼Ä´æÆ÷
	//ÓÃÓÚ±êÊ¶ÏòÁ¿±íÊÇÔÚCODEÇø»¹ÊÇÔÚRAMÇø
}
//ÉèÖÃNVIC·Ö×é
//NVIC_Group:NVIC·Ö×é 0~4 ×Ü¹²5×é 		   
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	 
{ 
	u32 temp,temp1;	  
	temp1=(~NVIC_Group)&0x07;//È¡ºóÈýÎ»
	temp1<<=8;
	temp=SCB->AIRCR;  //¶ÁÈ¡ÏÈÇ°µÄÉèÖÃ
	temp&=0X0000F8FF; //Çå¿ÕÏÈÇ°·Ö×é
	temp|=0X05FA0000; //Ð´ÈëÔ¿³×
	temp|=temp1;	   
	SCB->AIRCR=temp;  //ÉèÖÃ·Ö×é	    	  				   
}
//ÉèÖÃNVIC 
//NVIC_PreemptionPriority:ÇÀÕ¼ÓÅÏÈ¼¶
//NVIC_SubPriority       :ÏìÓ¦ÓÅÏÈ¼¶
//NVIC_Channel           :ÖÐ¶Ï±àºÅ
//NVIC_Group             :ÖÐ¶Ï·Ö×é 0~4
//×¢ÒâÓÅÏÈ¼¶²»ÄÜ³¬¹ýÉè¶¨µÄ×éµÄ·¶Î§!·ñÔò»áÓÐÒâÏë²»µ½µÄ´íÎó
//×é»®·Ö:
//×é0:0Î»ÇÀÕ¼ÓÅÏÈ¼¶,4Î»ÏìÓ¦ÓÅÏÈ¼¶
//×é1:1Î»ÇÀÕ¼ÓÅÏÈ¼¶,3Î»ÏìÓ¦ÓÅÏÈ¼¶
//×é2:2Î»ÇÀÕ¼ÓÅÏÈ¼¶,2Î»ÏìÓ¦ÓÅÏÈ¼¶
//×é3:3Î»ÇÀÕ¼ÓÅÏÈ¼¶,1Î»ÏìÓ¦ÓÅÏÈ¼¶
//×é4:4Î»ÇÀÕ¼ÓÅÏÈ¼¶,0Î»ÏìÓ¦ÓÅÏÈ¼¶
//NVIC_SubPriorityºÍNVIC_PreemptionPriorityµÄÔ­ÔòÊÇ,ÊýÖµÔ½Ð¡,Ô½ÓÅÏÈ	   
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
{ 
	u32 temp;	
	MY_NVIC_PriorityGroupConfig(NVIC_Group);//ÉèÖÃ·Ö×é
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;//È¡µÍËÄÎ»  
	NVIC->ISER[NVIC_Channel/32]|=(1<<NVIC_Channel%32);//Ê¹ÄÜÖÐ¶ÏÎ»(ÒªÇå³ýµÄ»°,Ïà·´²Ù×÷¾ÍOK) 
	NVIC->IP[NVIC_Channel]|=temp<<4;//ÉèÖÃÏìÓ¦ÓÅÏÈ¼¶ºÍÇÀ¶ÏÓÅÏÈ¼¶   	    	  				   
} 
//Íâ²¿ÖÐ¶ÏÅäÖÃº¯Êý
//Ö»Õë¶ÔGPIOA~G;²»°üÀ¨PVD,RTCºÍUSB»½ÐÑÕâÈý¸ö
//²ÎÊý:
//GPIOx:0~6,´ú±íGPIOA~G
//BITx:ÐèÒªÊ¹ÄÜµÄÎ»;
//TRIM:´¥·¢Ä£Ê½,1,ÏÂÉýÑØ;2,ÉÏ½µÑØ;3£¬ÈÎÒâµçÆ½´¥·¢
//¸Ãº¯ÊýÒ»´ÎÖ»ÄÜÅäÖÃ1¸öIO¿Ú,¶à¸öIO¿Ú,Ðè¶à´Îµ÷ÓÃ
//¸Ãº¯Êý»á×Ô¶¯¿ªÆô¶ÔÓ¦ÖÐ¶Ï,ÒÔ¼°ÆÁ±ÎÏß   	    
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM) 
{
	u8 EXTADDR;
	u8 EXTOFFSET;
	EXTADDR=BITx/4;//µÃµ½ÖÐ¶Ï¼Ä´æÆ÷×éµÄ±àºÅ
	EXTOFFSET=(BITx%4)*4; 
	RCC->APB2ENR|=0x01;//Ê¹ÄÜio¸´ÓÃÊ±ÖÓ			 
	AFIO->EXTICR[EXTADDR]&=~(0x000F<<EXTOFFSET);//Çå³ýÔ­À´ÉèÖÃ£¡£¡£¡
	AFIO->EXTICR[EXTADDR]|=GPIOx<<EXTOFFSET;//EXTI.BITxÓ³Éäµ½GPIOx.BITx 
	//×Ô¶¯ÉèÖÃ
	EXTI->IMR|=1<<BITx;//  ¿ªÆôline BITxÉÏµÄÖÐ¶Ï
	//EXTI->EMR|=1<<BITx;//²»ÆÁ±Îline BITxÉÏµÄÊÂ¼þ (Èç¹û²»ÆÁ±ÎÕâ¾ä,ÔÚÓ²¼þÉÏÊÇ¿ÉÒÔµÄ,µ«ÊÇÔÚÈí¼þ·ÂÕæµÄÊ±ºòÎÞ·¨½øÈëÖÐ¶Ï!)
 	if(TRIM&0x01)EXTI->FTSR|=1<<BITx;//line BITxÉÏÊÂ¼þÏÂ½µÑØ´¥·¢
	if(TRIM&0x02)EXTI->RTSR|=1<<BITx;//line BITxÉÏÊÂ¼þÉÏÉý½µÑØ´¥·¢
} 	  
//²»ÄÜÔÚÕâÀïÖ´ÐÐËùÓÐÍâÉè¸´Î»!·ñÔòÖÁÉÙÒýÆð´®¿Ú²»¹¤×÷.		    
//°ÑËùÓÐÊ±ÖÓ¼Ä´æÆ÷¸´Î»		  
void MYRCC_DeInit(void)
{	
 	RCC->APB1RSTR = 0x00000000;//¸´Î»½áÊø			 
	RCC->APB2RSTR = 0x00000000; 
	  
  	RCC->AHBENR = 0x00000014;  //Ë¯ÃßÄ£Ê½ÉÁ´æºÍSRAMÊ±ÖÓÊ¹ÄÜ.ÆäËû¹Ø±Õ.	  
  	RCC->APB2ENR = 0x00000000; //ÍâÉèÊ±ÖÓ¹Ø±Õ.			   
  	RCC->APB1ENR = 0x00000000;   
	RCC->CR |= 0x00000001;     //Ê¹ÄÜÄÚ²¿¸ßËÙÊ±ÖÓHSION	 															 
	RCC->CFGR &= 0xF8FF0000;   //¸´Î»SW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
	RCC->CR &= 0xFEF6FFFF;     //¸´Î»HSEON,CSSON,PLLON
	RCC->CR &= 0xFFFBFFFF;     //¸´Î»HSEBYP	   	  
	RCC->CFGR &= 0xFF80FFFF;   //¸´Î»PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
	RCC->CIR = 0x00000000;     //¹Ø±ÕËùÓÐÖÐ¶Ï		 
	//ÅäÖÃÏòÁ¿±í				  
#ifdef  VECT_TAB_RAM
	MY_NVIC_SetVectorTable(0x20000000, 0x0);
#else   
	MY_NVIC_SetVectorTable(0x08000000,0x0);
#endif
}

#if 0
//THUMBÖ¸Áî²»Ö§³Ö»ã±àÄÚÁª
//²ÉÓÃÈçÏÂ·½·¨ÊµÏÖÖ´ÐÐ»ã±àÖ¸ÁîWFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//¹Ø±ÕËùÓÐÖÐ¶Ï
__asm void INTX_DISABLE(void)
{
	CPSID I;		  
}
//¿ªÆôËùÓÐÖÐ¶Ï
__asm void INTX_ENABLE(void)
{
	CPSIE I;		  
}
//ÉèÖÃÕ»¶¥µØÖ·
//addr:Õ»¶¥µØÖ·
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
#else
//THUMBÖ¸Áî²»Ö§³Ö»ã±àÄÚÁª
//²ÉÓÃÈçÏÂ·½·¨ÊµÏÖÖ´ÐÐ»ã±àÖ¸ÁîWFI  
void WFI_SET(void)
{
	__asm__("WFI");		  
}
//¹Ø±ÕËùÓÐÖÐ¶Ï
void INTX_DISABLE(void)
{
	__asm__("CPSID I");		  
}
//¿ªÆôËùÓÐÖÐ¶Ï
void INTX_ENABLE(void)
{
	__asm__("CPSIE I");		  
}
//ÉèÖÃÕ»¶¥µØÖ·
//addr:Õ»¶¥µØÖ·
void MSR_MSP(u32 addr) 
{
    __asm__("MSR MSP, r0\n\t"
    		"BX r14");
}
#endif

//½øÈë´ý»úÄ£Ê½	  
void Sys_Standby(void)
{
	SCB->SCR|=1<<2;//Ê¹ÄÜSLEEPDEEPÎ» (SYS->CTRL)	   
  	RCC->APB1ENR|=1<<28;     //Ê¹ÄÜµçÔ´Ê±ÖÓ	    
 	PWR->CSR|=1<<8;          //ÉèÖÃWKUPÓÃÓÚ»½ÐÑ
	PWR->CR|=1<<2;           //Çå³ýWake-up ±êÖ¾
	PWR->CR|=1<<1;           //PDDSÖÃÎ»		  
	WFI_SET();				 //Ö´ÐÐWFIÖ¸Áî	
}	

//ÏµÍ³Èí¸´Î»   
void Sys_Soft_Reset(void)
{   
	SCB->AIRCR =0X05FA0000|(u32)0x04;	  
} 		 
//JTAGÄ£Ê½ÉèÖÃ,ÓÃÓÚÉèÖÃJTAGµÄÄ£Ê½
//mode:jtag,swdÄ£Ê½ÉèÖÃ;00,È«Ê¹ÄÜ;01,Ê¹ÄÜSWD;10,È«¹Ø±Õ;	   
//#define JTAG_SWD_DISABLE   0X02
//#define SWD_ENABLE         0X01
//#define JTAG_SWD_ENABLE    0X00		  
void JTAG_Set(u8 mode)
{
	u32 temp;
	temp=mode;
	temp<<=25;
	RCC->APB2ENR|=1<<0;     //¿ªÆô¸¨ÖúÊ±ÖÓ	   
	AFIO->MAPR&=0XF8FFFFFF; //Çå³ýMAPRµÄ[26:24]
	AFIO->MAPR|=temp;       //ÉèÖÃjtagÄ£Ê½
} 
//ÏµÍ³Ê±ÖÓ³õÊ¼»¯º¯Êý
//pll:Ñ¡ÔñµÄ±¶ÆµÊý£¬´Ó2¿ªÊ¼£¬×î´óÖµÎª16		 
void Stm32_Clock_Init(u8 PLL)
{
	unsigned char temp=0;   
	MYRCC_DeInit();		  //¸´Î»²¢ÅäÖÃÏòÁ¿±í
 	RCC->CR|=0x00010000;  //Íâ²¿¸ßËÙÊ±ÖÓÊ¹ÄÜHSEON
	while(!(RCC->CR>>17));//µÈ´ýÍâ²¿Ê±ÖÓ¾ÍÐ÷
	RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
	PLL-=2;//µÖÏû2¸öµ¥Î»
	RCC->CFGR|=PLL<<18;   //ÉèÖÃPLLÖµ 2~16
	RCC->CFGR|=1<<16;	  //PLLSRC ON 
	FLASH->ACR|=0x32;	  //FLASH 2¸öÑÓÊ±ÖÜÆÚ

	RCC->CR|=0x01000000;  //PLLON
	while(!(RCC->CR>>25));//µÈ´ýPLLËø¶¨
	RCC->CFGR|=0x00000002;//PLL×÷ÎªÏµÍ³Ê±ÖÓ	 
	while(temp!=0x02)     //µÈ´ýPLL×÷ÎªÏµÍ³Ê±ÖÓÉèÖÃ³É¹¦
	{   
		temp=RCC->CFGR>>2;
		temp&=0x03;
	}    
}		    




























