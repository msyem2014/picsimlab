/* ########################################################################

   PICsimLab - PIC laboratory simulator

   ########################################################################

   Copyright (c) : 2010-2017  Luis Claudio Gambôa Lopes

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   For e-mail suggestions :  lcgamboa@yahoo.com
   ######################################################################## */


#include "board_avr.h"

#include"picsimlab1.h"

board_avr::board_avr(void)
{
  avr=NULL;
}


void 
board_avr::MSetSerial(const char * port)
{
   //pic_set_serial(&pic,port,0,0,0);
}


int
board_avr::MInit(const char * processor, const char * fname, float freq)
{
  int ret;
  //elf_firmware_t f;
  avr = avr_make_mcu_by_name(processor);
  if (!avr) {
    fprintf(stderr, "Error creating the AVR core\n");
    return 0;
  }
  
  
  avr_init(avr);
  /*   
  ret=elf_read_firmware(fname, &f);
  if(ret > 0)
  {
    avr_load_firmware(avr, &f); 
  }
  */
  ret=read_ihx_avr(fname,1);
  
  avr->frequency = freq;
  
  //avr->reset_pc = bootloader
  
  avr_reset(avr);

          
  for(int p=0; p< MGetPinCount ();p++)
  {
      String pname=MGetPinName (p+1);
      
      if(pname[0] == 'P')
      {
          pins[p].port=pname[1];
          pins[p].pord=pname[2]-'0';        
  
          avr_irq_t* stateIrq = avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ(pins[p].port),  pins[p].pord );
          avr_irq_register_notify( stateIrq, out_hook, &pins[p] );

          avr_irq_t* directionIrq = avr_io_getirq( avr, AVR_IOCTL_IOPORT_GETIRQ(pins[p].port), IOPORT_IRQ_DIRECTION_ALL );
          avr_irq_register_notify( directionIrq, ddr_hook, &pins[p] );

          //FIXME
          
          const char* name = (const char *)pname.c_str ();
          Write_stat_irq[p] = avr_alloc_irq( &avr->irq_pool, 0, 1, &name );
  
          avr_irq_t* writeIrq = avr_io_getirq( avr, AVR_IOCTL_IOPORT_GETIRQ(pins[p].port), pins[p].pord );
          avr_connect_irq(Write_stat_irq[p], writeIrq );
          
          
          /*  
          avr_irq_t* adcIrq = avr_io_getirq( avr, AVR_IOCTL_ADC_GETIRQ, ADC_IRQ_OUT_TRIGGER );
          avr_irq_register_notify( adcIrq, adc_hook, &pins[i] );
  */
    
      }
      else
      {
          pins[p].port=0;
          pins[p].pord=-1;
      }
  }
  
  //avr_ioport_state_t  iostate;  
  //avr_ioctl( avr, AVR_IOCTL_IOPORT_GETSTATE('B'),&iostate);
  
  
          
  return ret;
}

void 
board_avr::MEnd(void)
{
  avr_terminate(avr); 	
  //delete avr;
  avr=NULL;
  
  //pic_end();
  //mplabxd_end();
};

void 
board_avr::MEraseFlash(void)
{
   //pic_erase_flash();
   memset(avr->flash,0xFF,avr->flashend);
};


void 
board_avr::MSetFreq(float freq)
{
  if(avr)
   avr->frequency = freq;
};

float
board_avr::MGetFreq(void)
{	
   return avr->frequency;
}        

float
board_avr::MGetInstClock(void)
{	
   return avr->frequency;
}        


void 
board_avr::MDumpMemory(const char * fname)
{
      write_ihx_avr(fname);
}

int
board_avr::DebugInit(void)
{
  /*
     avr->gdb_port = 1234;
   //avr->state = cpu_Stopped;
   return avr_gdb_init(avr);
   */
   return 0;
}

void
board_avr::DebugLoop(void)
{
  /*
   if(Window1.Get_picpwr())
   {
      //prog_loop(&pic);
      mplabxd_loop();
   }
   */ 
}

int 
board_avr::CpuInitialized(void)
{
  return (avr != NULL);
}

int 
board_avr::MGetPinCount(void)
{
  if(avr == NULL) return 0;	
  if( String(avr->mmcu).compare (wxT("atmega328")) == 0)return 28;
  return 0;
}

String 
board_avr::MGetPinName(int pin)
{
  if(pin <0 && pin >= MGetPinCount())return "error";
  
  switch(pin)
  {
    //case 1:
    //  return "PC6 (RST)";break;
    case 2:
      return "PD0/0";break;
    case 3:
      return "PD1/1";break;
    case 4:
      return "PD2/2";break;
    case 5:
      return "PD3/~3";break;
    case 6:
      return "PD4/4";break;
    //case 7:
    //  return "VCC";break;
    //case 8:
    //  return "GND";break;    
    //case 9:
    //  return "PB6 (OSC1)";break;
    //case 10:
    //  return "PB7 (OSC2)";break;
    case 11:
      return "PD5/~5";break;
    case 12:
      return "PD6/~6";break;
    case 13:
      return "PD7/7";break;
    case 14:
      return "PB0/8";break;
    case 15:
      return "PB1/~9";break;      
    case 16:
      return "PB2/~10";break;
    case 17:
      return "PB3/~11";break;
    case 18:
      return "PB4/12";break;
    case 19:
      return "PB5/13";break;
    //case 20:
    //  return "VCC";break;
    //case 21:
    //  return "AREF";break;
    //case 22:
    //  return "GND";break;
    case 23:
      return "PC0/A0";break;
    case 24:
      return "PC1/A1";break;
    case 25:
      return "PC2/A2";break;
    case 26:
      return "PC3/A3";break;
    case 27:
      return "PC4/A4";break;
    case 28:
      return "PC5/A5";break;      
  }
  
  return "error";
}

void 
board_avr::MSetPin(int pin, unsigned char value)
{
  if(pin <0 && pin >= MGetPinCount())return;
  
  avr_raise_irq(Write_stat_irq[pin-1], value);
}
      
void 
board_avr::MSetAPin(int pin, float value)
{
  if(pin <0 && pin >= MGetPinCount())return;  
  pins[pin-1].avalue=value;
}
      
unsigned char 
board_avr::MGetPin(int pin)
{
  if(pin <0 && pin >= MGetPinCount())return -1;
  
  return pins[pin-1].value;
}
      
const picpin * 
board_avr::MGetPinsValues(void)
{
     return pins;
}
      
      
      
//hexfile support


#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int
board_avr::parse_hex(char *line,int bytes)
{
  char snum[200];
  int num;
    
  strncpy(snum,line,bytes);
  snum[bytes]='\0';
  sscanf(snum,"%X",&num);

  return num; 
};


unsigned char
board_avr::checksum(char* str)
{
  unsigned int i;
  unsigned char acum=0;

  for(i=0; i < ((strlen(str)-1)/2);i++)
  {
    acum+=parse_hex(str+1+(i*2),2);
  }
  
  return acum;
}



int
board_avr::read_ihx_avr(const char * fname, int leeprom)
{
  FILE* fin;
  int  lc=0;
  unsigned int bc;
  char line[256];
  unsigned int nbytes,addr,type;  
  unsigned int addrx;
  //unsigned short addrh=0;
  //unsigned short addrl=0;
  char *mptr;

  fin=fopen(fname,"r");

  if(fin)
  {
    do
    {
      fgets(line,256,fin);
      lc++;
  
      /*for dos file*/ 
      if(line[strlen(line)-2]=='\r')
      {
        line[strlen(line)-2]='\n';
        line[strlen(line)-1]=0;
      } 

      if(checksum(line) == 0)
      {
        nbytes=parse_hex(line+1,2);
        addr=parse_hex(line+3,4);
        type=parse_hex(line+7,2);

	switch(type)
	{
	  case 0:
	    //addrl=addr/2;
/*
            if(addrh == 0x0030 )
	    {
              //config
              mptr=(char*)pic->config;
              for(bc=0;bc < nbytes;bc++)
	      {
	        addrx=addr+bc;
                if((addrx/2) < pic->CONFIGSIZE)
	          mptr[addrx]=parse_hex(line+9+(bc*2),2);
              }
	    }
            else
	    {
              if(addrh == 0x00F0 )
              { 
                //EEPROM
                if(leeprom == 1) 
                for(bc=0;bc < nbytes;bc++)
	        {
                  addrx= addr+bc;
                  if(addrx < avr->flashend)
	            avr->flash[addrx]= parse_hex(line+9+(bc*2),2);
                }
              }
              else
              {
                if(addrh == 0x0020 )
                {
                  //IDS
                  mptr=(char*)pic->id;
                  for(bc=0;bc < nbytes;bc++)
	          {
	            addrx=addr+bc;
                    if((addrx/2) < pic->IDSIZE)
	              mptr[addrx]=parse_hex(line+9+(bc*2),2);
                  }
                } 
                else
 */ 
                //{
                  //prog mem
                  mptr=(char*)avr->flash;
                  for(bc=0;bc < nbytes;bc++)
	          {
	            addrx=addr+bc;
                    if(addrx <= avr->flashend)
	             mptr[addrx]=parse_hex(line+9+(bc*2),2);
                  }
                //}
              //}
	    //}
	  break;
	  case 1:
            fclose(fin);  
	    return 0;//no error
	  break;
	  case 4:
	    //addrh=((parse_hex(line+9,2)<<8)|parse_hex(line+11,2));
	  break;
	}
      }
      else
      {
        printf("ERRO: Picsim->File bad checksum line %i!(%s)\n",lc,fname);
        fclose(fin);  
        return HEX_CHKSUM;
      }
     }
     while(!feof(fin));
     fclose(fin); 
  }
  else
  {
    printf("ERRO: Picsim->File not found!(%s)\n",fname);
    return HEX_NFOUND;
  }
  return 0;//no error
};


int
board_avr::write_ihx_avr(const char * fname)
{

  FILE * fout;
  unsigned char sum;
  unsigned char nb;
  unsigned int iaddr=0;
  unsigned int i;
  char values[100]; 
  char tmp[100]; 

  fout=fopen(fname,"w");

  if(fout)
  {
  //program memory  //TODO P18 only address < 64K bytes  
    nb=0;
    sum=0;
    //fprintf(fout,":020000040000FA\n");
    for(i=0;i <= avr->flashend;i++)
    {
    
      if(nb==0)
      {
        iaddr=i;
        sprintf(values,"%02X",avr->flash[i]);
      }
      else
      {
        sprintf(tmp,"%s%02X",values,avr->flash[i]);
        strcpy(values,tmp);
      }

      nb++;
      sum+=avr->flash[i];
     
      if(nb==16)
      {
        sum+=nb;
        sum+=(iaddr&0x00FF);
        sum+=((iaddr&0xFF00)>>8);
      //printf("sum=%02X %02X %02X\n",sum,~sum,(~sum)+1);
        sum=(~sum)+1; 
        fprintf(fout,":%02X%04X00%s%02X\n",nb,iaddr,values,sum);
        nb=0;
        sum=0;
      }
    }
    if(nb)
    {
        sum+=nb;
        sum+=(iaddr&0x00FF);
        sum+=((iaddr&0xFF00)>>8);
        sum=(~sum)+1; 
        fprintf(fout,":%02X%04X00%s%02X\n",nb,iaddr,values,sum);
    }
/*
//ids
    nb=0;
    sum=0;
    fprintf(fout,":020000040020DA\n");
    for(i=0;i<pic->IDSIZE;i++)
    {
    
      if(nb==0)
      {
        iaddr=(i*2);
        sprintf(values,"%02X%02X",pic->id[i]&0x00FF ,(pic->id[i]&0xFF00)>>8);
      }
      else
      {
        sprintf(tmp,"%s%02X%02X",values,pic->id[i]&0x00FF,(pic->id[i]&0xFF00)>>8);
        strcpy(values,tmp);
      }

      nb+=2;
      sum+=pic->id[i]&0x00FF;
      sum+=(pic->id[i]&0xFF00)>>8;
      if(nb==16)
      {
        sum+=nb;
        sum+=(iaddr&0x00FF);
        sum+=((iaddr&0xFF00)>>8);
      //printf("sum=%02X %02X %02X\n",sum,~sum,(~sum)+1);
        sum=(~sum)+1; 
        fprintf(fout,":%02X%04X00%s%02X\n",nb,iaddr,values,sum);
        nb=0;
        sum=0;
      }
    }
    if(nb)
    {
        sum+=nb;
        sum+=(iaddr&0x00FF);
        sum+=((iaddr&0xFF00)>>8);
        sum=(~sum)+1; 
        fprintf(fout,":%02X%04X00%s%02X\n",nb,iaddr,values,sum);
    }
*/
/*    
//config
    nb=0;
    sum=0;
    fprintf(fout,":020000040030CA\n");
    for(i=0;i<pic->CONFIGSIZE;i++)
    {
    
      if(nb==0)
      {
        iaddr=(i*2);
        sprintf(values,"%02X%02X",pic->config[i]&0x00FF ,(pic->config[i]&0xFF00)>>8);
      }
      else
      {
        sprintf(tmp,"%s%02X%02X",values,pic->config[i]&0x00FF,(pic->config[i]&0xFF00)>>8);
        strcpy(values,tmp);
      }

      nb+=2;
      sum+=pic->config[i]&0x00FF;
      sum+=(pic->config[i]&0xFF00)>>8;
      if(nb==16)
      {
        sum+=nb;
        sum+=(iaddr&0x00FF);
        sum+=((iaddr&0xFF00)>>8);
      //printf("sum=%02X %02X %02X\n",sum,~sum,(~sum)+1);
        sum=(~sum)+1; 
        fprintf(fout,":%02X%04X00%s%02X\n",nb,iaddr,values,sum);
        nb=0;
        sum=0;
      }
    }
    if(nb)
    {
        sum+=nb;
        sum+=(iaddr&0x00FF);
        sum+=((iaddr&0xFF00)>>8);
        sum=(~sum)+1; 
        fprintf(fout,":%02X%04X00%s%02X\n",nb,iaddr,values,sum);
    }
 */
  /*  
//eeprom
    nb=0;
    sum=0;
    fprintf(fout,":0200000400F00A\n");
    for(i=0;i<pic->EEPROMSIZE;i++)
    {
    
      if(nb==0)
      {
        iaddr=i;
        sprintf(values,"%02X",pic->eeprom[i]);
      }
      else
      {
        sprintf(tmp,"%s%02X",values,pic->eeprom[i]);
        strcpy(values,tmp);
      }

      nb++;
      sum+=pic->eeprom[i];
      if(nb==16)
      {
        sum+=nb;
        sum+=(iaddr&0x00FF);
        sum+=((iaddr&0xFF00)>>8);
      //printf("sum=%02X %02X %02X\n",sum,~sum,(~sum)+1);
        sum=(~sum)+1; 
        fprintf(fout,":%02X%04X00%s%02X\n",nb,iaddr,values,sum);
        nb=0;
        sum=0;
      }
    }
    if(nb)
    {
        sum+=nb;
        sum+=(iaddr&0x00FF);
        sum+=((iaddr&0xFF00)>>8);
        sum=(~sum)+1; 
        fprintf(fout,":%02X%04X00%s%02X\n",nb,iaddr,values,sum);
    }
*/
    
//end
    fprintf(fout,":00000001FF\n");
    fclose(fout);   
    return 0;//no error
  }
  else
  {
    printf("ERRO: Picsim->File not found!(%s)\n",fname);
    return HEX_NWRITE;
  }
  return 0;//no error
}


 