#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <stdint.h>
#include <string.h>


#define PPCANAL 1789
#define PPCOR   1787

#define FORMAT 0  //
#define LOAD   1  // Constantes do TrendCurveFuncao
#define SAVE   2  //

#define FALSE 0
#define TRUE  1

#define TODOS 0xFF
#define COMMAND_SAVE_LOG    0X02
#define COMMAND_READ_ANALOG 0x1A

#define TUPLA_VP_SIZE       12

#define saltaIndice4(arg1) (arg1<3)?(arg1):((arg1)+1)


//------------------------------------------------------------------------------
struct{
        unsigned flag_save_time        :1;
        unsigned flag_wakeup           :1;
        unsigned flagSendDataFix       :1;
        unsigned flag_proculus_hs      :1;  
}statusgen1;

#define flag_save_time           statusgen1.flag_save_time
#define flag_wakeup              statusgen1.flag_wakeup
#define flagSendDataFix          statusgen1.flagSendDataFix
#define flag_proculus_hs         statusgen1.flag_proculus_hs
//------------------------------------------------------------------------------




typedef struct{
	          int16_t *entrada[15];
	          float   fator[15];
	          char    canal[15];
	          char    icone[15];
	          char  vpIcone[15];
			  int16_t   cor[15];	          
              } T_mapa;


enum cores {none=-1,
	       PRETO=1,
	       CINZA_ESC,
	       CINZA_CLARO,
	       MARROM,
	       MARROM_CLARO,
	       VERMELHO,
	       LARANJA,
	       ROSA,
	       AMARELO_ESC,
	       VERDE,
	       VERDE_CLARO,
	       AZUL,
	       ROXO
           };


const char iconePIC[13][13]={
	                    "PRETO       ",
	                    "CINZA_ESCURO",
	                    "CINZA_CLARO ",
                        "MARROM      ",
                        "MARROM_CLARO",
                        "VERMELHO    ",
                        "LARANJA     ",
                        "ROSA        ",
                        "AMARELO_ESC.",
                        "VERDE       ",
                        "VERDE_CLARO ",
                        "AZUL        ",
                        "ROXO        "
                        };


int16_t  PROCULUS_VP_Read_UInt16(int trendvp);
void PROCULUS_VP_Write_UInt16(int add, int data);
void TrendCurveFuncao(char funcao);
void EEPROM_Write_Byte(unsigned int addr, unsigned char dado);
unsigned char EEPROM_Read_Byte(unsigned int addr);
void EEPROM_Write_Buffer(unsigned int addr,char *dado,char size);
void EEPROM_Read_Buffer(unsigned int addr,char *dado,char size);
char buscaIndex(char *buffer,char valor);
char MenorCanalLivre();
int  Send_To_Slave(char destino, char comando, char size, char * buffer);
void TrendCurveFuncao(char funcao);
void PROCULUS_graphic_plot(unsigned char lcd_channel, unsigned int value);
void my_delay_ms_TMR1(unsigned int tempo);
void save_datalog(unsigned int add);
void PROCULUS_Clear_Line_Graphic(char canal);
void ShowSensorRealTimeHS(void);
void my_delay_ms(int value);
void show_on_display();
void showEEPROM();
void FillEEPROM_0XFF(void);
void TrataGrafico(char icone);
void TrendCurveLOAD();
void PROCULUS_Buzzer(int tempo);
void my_delay_ms_CLRWDT(int tempo);


const int TrendColor[13]={
                          0x0000, //Preto
                          0x39E7, //Cinza escuro
                          0x6B6D, //Cinza claro
                          0x7800, //Marron Escuro
                          0x9A23, //Marron Claro
                          0xF800, //Vermelho vivo
                          0xFBE0, //Laranja
                          0xFBF7, //Rosa
                          0xD540, //Amarelo Escuro
                          0x03E0, //Verde Escuro
                          0x07E0, //Verde Claro
                          0x07FF, //Azul Piscina
                          0xF81F  //Roxo
                          };




/*
unsigned char TrendColor[13]={
	                          PRETO,
	                          CINZA_ESC,
	                          CINZA_CLARO,
	                          MARROM,
	                          MARROM_CLARO,
	                          VERMELHO,
	                          LARANJA,
	                          ROSA,
	                          AMARELO_ESC,
	                          VERDE,
	                          VERDE_CLARO,
	                          AZUL,
	                          ROXO
	                          };
*/


unsigned int  memoDisplay[2000];
unsigned int  trendvp;
unsigned char icone;
unsigned int  VP[0xFFFF];
int16_t memoria[0xFFFF];
char EEPROM[0x3FF];
char totalboard;
char texto[30];
char texto1[30];

//----------------------------
int16_t Vacuometro, Voltimetro, Condensador, Sonda;
T_mapa mapa;
int16_t leitura[0x0F]   ;
unsigned char flag_array_slave_WDT[15];


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                           M  A  I  N      P  R  O  G  R  A  M
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
int main()
  {   
  unsigned char cnt, i;
  char click, canal, tecla;
  int index;
  int16_t valor;
  float total,fator;  
  //char show;
  
  Voltimetro =111;  
  Vacuometro =222;
  Condensador=333;
  Sonda=444;  
  totalboard=7;  
  
  srand(time(NULL));  
  FillEEPROM_0XFF();
  TrendCurveFuncao(FORMAT);
  TrendCurveFuncao(LOAD);
  ShowSensorRealTimeHS();  
  
    for(i=0;i<13;i++) mapa.entrada[i]=NULL; 
   
    do{ 


  
    	printf("=============================================================================================================================================================\n");
		printf(" FIGURA  |        CORES         |       VP       | vpIcone |      COR      |    CANAL    | Value  |  icone  |  canal |   cor   |  valor  | fator |   total  |\n");
		for(cnt=0;cnt<13;cnt++)	
				 {
				 	
				 	
			     if(mapa.icone[cnt]!=-1)
			        {
			        valor=*mapa.entrada[cnt];
			        fator=mapa.fator[cnt];
					total=valor*fator;	
					}			        
				 else
				    {
				    valor=0;
					fator=0;
					total=0;	
					}
				    				 	
				 	
				 	
				 printf("%5d\t | %s\t 0x%4X\t| 0X%3X\t = %2d\t |  %4d   |   %d=%4X   |  %d=%3X   |  %4d  |  %3d    |  %3d   |  0x%4X | %6d  |  %4.2f |  %7.2f |\n",cnt+1
				                                                        					,iconePIC[cnt]
																        					,TrendColor[cnt]
																        					,(cnt+0x0310)
																	    					,PROCULUS_VP_Read_UInt16(cnt+0x0310)
																	    					,mapa.vpIcone[cnt]
																	    					,(cnt*10)+PPCOR																																					    
																	    					,PROCULUS_VP_Read_UInt16((cnt*10)+PPCOR)&(0xFFFF)
																	    					,(cnt*10)+PPCANAL
																	    					,PROCULUS_VP_Read_UInt16(cnt*10+PPCANAL)
																	    					,leitura[cnt]
																	    					,mapa.icone[cnt]
																	    					,mapa.canal[cnt]
																	    					,mapa.cor[cnt]&0xFFFF
																	    					,valor
																	    					,fator
																	    					,total
																	    					);
				 }
    	printf("=============================================================================================================================================================\n");				 
    	printf("\nMemoria EEPROM\n");
    	showEEPROM();
    	



    	
    	
    	
    	
    	
    	
        printf("\n[99]Exit    [33]Load   [44]Save   [55]Format\n");		
		
		 
		//click=rand() % 13;
		scanf("%d",&click);
		printf("Click=%d\n",click);
        PROCULUS_VP_Write_UInt16(0x0310+(click-1),PROCULUS_VP_Read_UInt16(0x0310+(click-1))+15); //Muda a figura para um ponto negro ou solicita zerar
        
        if(click==33){TrendCurveLOAD();system("cls");continue;}
        if(click==44){TrendCurveFuncao(SAVE);system("cls");continue;}
        if(click==55){TrendCurveFuncao(FORMAT);system("cls");continue;}
		                    
        TrataGrafico(0);                                                                   
                                  

		if(click==99) break;						  	 
        save_datalog(0);
        //show_on_display();
        //getch();
        system("cls");
    }while(1);
    
    return 0;
	
}


/*
EEPROM_Write_Buffer(19,&mapa.vpIcone[0],15);
EEPROM_Write_Buffer(0xEA,&mapa.icone[0],15);
EEPROM_Write_Buffer(36,&mapa.canal[0],15);  
*/
void showEEPROM(){
	 char i;
	 printf("VpIcone\t    Mp.icone\t    Mp.canal\n");
	 for(i=0;i<15;i++){
	 	printf("%d=%d\t     0x%X=%d\t      %d=%d\n"
		                                    ,19+i,  (char)EEPROM_Read_Byte(19+i)
		                                    ,0xEA+i,(char)EEPROM_Read_Byte(0xEA+i)
		                                    ,36+i,  (char)EEPROM_Read_Byte(36+i)
		                                    );
	 }
}


  
  
void show_on_display()
     {
     char i;
     for(i=0;i<13;i++)
        {
        printf("%d\t %d\t %d\t 0x%X\t 0x%X\t \n"
                                        ,i
                                        ,PROCULUS_VP_Read_UInt16(1000+i) 
                                        ,PROCULUS_VP_Read_UInt16(0x0310+i)
                                        ,PROCULUS_VP_Read_UInt16((i*10)+1787)&0xFFFF
                                        ,PROCULUS_VP_Read_UInt16((i*10)+1789)&0xFFFF
		                                );	
		}


     }  
  
  
  
  
  
  
  
//Salva e atualiza grafico
void save_datalog(unsigned int add){
     char index;
     char bb[3];
     char boardadd;
     
     //===================================================================================
     Send_To_Slave(TODOS, COMMAND_SAVE_LOG , 0, bb); 
     //===================================================================================
     for(index=0;index<(totalboard*2);index++)
	     {
         if(mapa.entrada[index]!=NULL) 
            {	
            PROCULUS_graphic_plot(mapa.canal[index]+1,*mapa.entrada[index]*mapa.fator[index]);
            }
         }      
}

  
  
  
  
  

void ShowSensorRealTimeHS(void)
     {     
     char bb[3];
     char SlaveBoard;
     char canal;
     char tupla;
     int  vp, vpicone;
     
     //-------------------------LEITURA DAS PLACAS------------------------------
     for(tupla=0;tupla<(totalboard*2);tupla++)
        { 
        SlaveBoard  = (tupla / 2)+1; 
        canal = tupla % 2;
        bb[0]=canal; 
        leitura[tupla]=Send_To_Slave(SlaveBoard, COMMAND_READ_ANALOG, 1, bb);
        flag_array_slave_WDT[SlaveBoard]=TRUE;
        }
     
     
     //------------------------GRAVA NO DISPLAY---------------------------------
     my_delay_ms(50);
     flag_proculus_hs=TRUE;
     for(tupla=0;tupla<(totalboard*2);tupla++)
        { 
        switch(tupla)
              {
              case 0:
                     PROCULUS_VP_Write_UInt16(151,leitura[tupla]); //Vacuometro 
                     Vacuometro=leitura[tupla];
                     break;               
              case 1:
                     PROCULUS_VP_Write_UInt16(153,leitura[tupla]); //Voltimetro 
                     Voltimetro=leitura[tupla];
                     break;  
              case 2:
                     PROCULUS_VP_Write_UInt16(150,leitura[tupla]); //Condensador  
                     Condensador=leitura[tupla];
                     break; 
              case 3:
                     //sem sensor nesta tupla
                     break;
              default:
                  canal=tupla-4;
                  vp    = 230+(canal*TUPLA_VP_SIZE);
                  vpicone     = 400+canal;
                  PROCULUS_VP_Write_UInt16(vp+1,leitura[canal+4]);     

                  if(leitura[canal+4]<-400)PROCULUS_VP_Write_UInt16(vpicone,1); //sem sensor
                  else if(leitura[canal+4]==-1)PROCULUS_VP_Write_UInt16(vpicone,0); //sem placa                 
                  else PROCULUS_VP_Write_UInt16(vpicone,3); //Temperatura normal                  
                  
                  break;
              }  
        }
      flag_proculus_hs=FALSE;        
     
     }
 











void my_delay_ms_TMR1(unsigned int tempo)
{
	
} 

void PROCULUS_graphic_plot(unsigned char lcd_channel, unsigned int value)
{
   /*	
   if((char)lcd_channel!=-1) 	
      printf("Escrevendo %d no canal %2d.\n",value,(char) lcd_channel);	
   else
      printf("Canal Desabilitado (%d).\n",(char) lcd_channel);
   */   
}






void TrendCurveFuncao(char funcao){
     char figura;
     char canal;
     int  cor;
     char i, index;
     switch(funcao)
           {
           case FORMAT:
                       for(i=0;i<13;i++)
                          { 
                          mapa.cor[i]=0xFFFF;
						  mapa.canal[i]=0x0A;                          
						  mapa.icone[i]=-1;                          
                          mapa.vpIcone[i]=-1;
                          mapa.entrada[i]=NULL;
                          mapa.fator[i]=0;
                          
                          EEPROM_Write_Byte(19+i,mapa.vpIcone[i]);
                          EEPROM_Write_Byte(0xEA+i,mapa.icone[i]);        
                          EEPROM_Write_Byte(36+i,mapa.canal[i]);
                          
                          PROCULUS_VP_Write_UInt16(0x0310+i,-1);           //Figura
                          PROCULUS_VP_Write_UInt16((i*10+PPCANAL),0x0A00); //Canal
                          PROCULUS_VP_Write_UInt16((i*10+PPCOR),0xFFFF);   //Cor 
                          }               
                       break;
           case   LOAD:
                      
                       EEPROM_Read_Buffer(19,mapa.vpIcone,15);
                       EEPROM_Read_Buffer(0xEA,mapa.icone,15);
                       EEPROM_Read_Buffer(36,mapa.canal,15);
                       


                       //-----------------------------------ICONE------------------------------------
                       for(i=0;i<13;i++)
                          {                          
                          figura=mapa.vpIcone[i];
                          if(figura!=255) 
                             {							   
                             PROCULUS_VP_Write_UInt16(0x0310+i, figura); //Figura -quadro colorido                             
                             }
                          else
                            {							
                            PROCULUS_VP_Write_UInt16(0x0310+i,-1);                            
                            }
                          }
                       
					   //------------------------------------COR-------------------------------------
                       for(i=0;i<13;i++)   
                          {
                          if(mapa.canal[i]<8)	
                             {                            
							 cor=TrendColor[mapa.canal[i]];								 
							 }
						  else
						     {
						     cor=0xFFFF;
						     } 						     
						  PROCULUS_VP_Write_UInt16((i*10+PPCOR),cor);
						  mapa.cor[i]=cor; 		
						  }
                          
                       //--------------------------------------CANAL------------------------------------------
                       for(i=0;i<13;i++)
                          { 
                          canal=mapa.canal[i];
						  if((canal>=0)&&(canal<=7))
						     {
                             PROCULUS_VP_Write_UInt16((i*10+PPCANAL),(canal<<8)|(0x0001)); //Canal Associado                             						     	
							 }
						  else	 
							 {							 
                             PROCULUS_VP_Write_UInt16((i*10+PPCANAL),0x0A00); //Canal                             
						     }
					      }
					      
                       //----------------------------------------VALOR-----------------------------------------------
                       for(index=0;index<8;index++)
                           {                           
                           if(mapa.icone[index]!=-1)
                              {                               
                              mapa.entrada[mapa.icone[index]]=&leitura[mapa.icone[index]];                          
							  mapa.fator[mapa.icone[index]]=1.0;                                 //Fator Padrão
                              if(mapa.icone[index]==0)mapa.fator[mapa.icone[index]]=0.4546;      //Fator para Tensão
                              if(mapa.icone[index]==1)mapa.fator[mapa.icone[index]]=0.05;        //Fator para Vacuo                            
                              }
                           }
						   				      
					      
                       break;
           case   SAVE:
                       for(i=0;i<14;i++)   
                           {
                           mapa.vpIcone[i]=PROCULUS_VP_Read_UInt16(0x0310+i);
						   mapa.canal[i]=PROCULUS_VP_Read_UInt16(i*10+PPCANAL)>>8;	
                           }
                       EEPROM_Write_Buffer(19,&mapa.vpIcone[0],15);
                       EEPROM_Write_Buffer(0xEA,&mapa.icone[0],15);
                       EEPROM_Write_Buffer(36,&mapa.canal[0],15);   
                       break;

                                                            
           } 
}






void TrataGrafico(char icone){
     int canal;
	 char i; 
     
     if(icone>12) return;

     if(icone==0)
           {
           for(trendvp=0x0310;trendvp<0x031E;trendvp++)
               { 
               if((PROCULUS_VP_Read_UInt16(trendvp)==14)||
                 ((PROCULUS_VP_Read_UInt16(trendvp)>=15)&&
                  (PROCULUS_VP_Read_UInt16(trendvp)<=30)))      
                   break;                
               }
               icone=trendvp-0x0310;
            }
         else
            { 
            trendvp=icone+0x0310;
            //printf("Trendvp=%d\n",trendvp);
            PROCULUS_VP_Write_UInt16(trendvp,14);
            }        
        //----------------------------------------------------------------------

        //----------------------------------------------------------------------     
             if(PROCULUS_VP_Read_UInt16(trendvp)==14)
                  {                                          
                  canal=MenorCanalLivre();
                  if(canal<8)
                     { 											                                                                 
                     PROCULUS_VP_Write_UInt16(0x310+icone,icone+1); //Colore o quadrado com uma cor fixa                                           
                     PROCULUS_VP_Write_UInt16((canal*10+PPCANAL),(canal<<8)|(0x0001)); //Seta um canal para um dos 13 icones
                     PROCULUS_VP_Write_UInt16((canal*10+PPCOR),TrendColor[icone]);     //Seta uma cor de linha do grafico											                                             


                     mapa.canal[canal]=canal;   //Seleciona um Canal
                     mapa.icone[canal]=icone+1;              //Registra qual icone está sendo tratado
                     mapa.vpIcone[icone]=icone+1;
                     mapa.cor[canal]=TrendColor[icone];
                     mapa.fator[canal]=1.0;
                     mapa.entrada[canal]=&leitura[saltaIndice4(icone)];    //Aponta para uma leitura

                     mapa.fator[canal]=1.0;                  //Fator Padrão

                     if(icone==0)mapa.fator[canal]=0.4546;      //Fator para Tensão
                     if(icone==1)mapa.fator[canal]=0.05;      //Fator para Vacuo

                     //PROCULUS_Clear_Line_Graphic(icone);
                     //PROCULUS_VP_Write_UInt16(1000+canal,*mapa.entrada[canal]*mapa.fator[canal]);      
                     //TrendCurveFuncao(SAVE); //Salva os icones acionados
                     } 
                  else
                     {	
                     PROCULUS_VP_Write_UInt16((canal*10+PPCANAL),(canal<<8)|(0x0A00)); //Canal
                     PROCULUS_VP_Write_UInt16((canal*10+PPCOR),0xFFFF);//Cor                                               
                     PROCULUS_VP_Write_UInt16(trendvp,-1);  
                     }
                  }
             else 
              if((PROCULUS_VP_Read_UInt16(trendvp)>=15)&&(PROCULUS_VP_Read_UInt16(trendvp)<=30))
                  {	
                  char canal_aleatorio, canal_sequencial;

                  canal_sequencial=buscaIndex(mapa.icone,icone+1);
                  canal_aleatorio=mapa.icone[icone]-1; 	                           //canal para lista aleatoria                                         

                  PROCULUS_VP_Write_UInt16(trendvp,-1);                            //Apaga o quadrado colorido do display 
                  PROCULUS_VP_Write_UInt16((canal_sequencial*10+PPCANAL),0x0A00);  //Libera o canal utilizado
                  PROCULUS_VP_Write_UInt16((canal_sequencial*10+PPCOR),0xFFFF);               //Torna a cor padrao do canal em branco 

                  mapa.entrada[canal_sequencial]=NULL;                             //Torna a entrada do canal NULL  
                  mapa.canal[canal_sequencial]=0X0A;                               //Seleciona um Canal
                  mapa.icone[canal_sequencial]=-1;                                 //Registra qual icone está sendo tratado
                  mapa.vpIcone[icone]=-1;                                          //Desliga ícone aleatorio
                  mapa.cor[canal_sequencial]=0xFFFF;                               //Desliga a Cor           
                  mapa.fator[canal_sequencial]=0.0;                                //Fator padrão para Temperatura

                  //PROCULUS_Clear_Line_Graphic(canal+1);               //Apaga a Linha desenhada  										                      
                  //TrendCurveFuncao(SAVE);								//Salva as alterações		                                  
                  }     
}
    
    
void TrendCurveLOAD()
{
char icone;
//EEPROM_Read_Buffer(19,mapa.vpIcone,15);

//EEPROM_Read_Buffer(36,mapa.canal,15); 

EEPROM_Read_Buffer(0xEA,mapa.icone,15);
TrataGrafico(mapa.icone[icone-2]);
   
}  
       
    
    
    










char buscaIndex(char *buffer,char valor)
{
	char i, retorno=-1;
	for(i=0;i<13;i++)
	   {
	   if(valor==buffer[i]) 
	      {
	      retorno=i;	
	      break;	
		  }
	   }
	return retorno;   
}


char MenorCanalLivre()
     {
     unsigned char canal, canalLivre, canalBusca, busca, Exist;	
     canal=0x0A;	
     for(canalLivre=0;canalLivre<8;canalLivre++)
	    {	    	
	    //printf("Buscando existencia de canal %d\n",canalLivre);
	    Exist=FALSE;
	    for(canalBusca=0;canalBusca<13;canalBusca++)		
	       {	       
	       busca=mapa.canal[canalBusca];
	       //printf("Varrendo figura %d=%d    \n",canalLivre,busca);
		   if(canalLivre==busca)
			 {
			 //printf("Existe o canal %d\n",canalLivre);	
			 Exist=TRUE;
			 break;	
			 }  
		   }
		if(Exist==FALSE)
		  {
		  //printf("NAO Existe o canal %d\n",canalLivre);	
		  canal=canalLivre;	
		  break;
		  }		   
		}
	 //printf("Retornando canal %d\n",canal);	
	 return canal;	
	 }



int  Send_To_Slave(char destino, char comando, char size, char * buffer)
{
	switch(destino)
	      {
	      case 0://printf("Erro no código. Endereco zero é da placa mae!");
	      	     break;
	      case 1:if(buffer[0]==0) 
	                return 50; //VACUO
	             else
				    return 100;  //VOLTIMETRO  
		         break;
	      case 2:if(buffer[0]==0)
	                return 150;   //CONDENSADOR
	             else
				    return -1;   
		         break;
	      case 3:if(buffer[0]==0) 
	                return 200;
	             else 
				    return 250;   
		         break;
	      case 4:if(buffer[0]==0) 
	                return 300;
	             else
				    return 350;   
		         break;
	      case 5:if(buffer[0]==0)
	                return 400;
	             else
				    return 450;   
		         break;
	      case 6:if(buffer[0]==0)
	                return 500;
	             else
				    return 550;   
		         break;
	      case 7:if(buffer[0]==0)
	                return 600;
	             else
				    return 650;   
		         break;	
	  		 				 				 				 				 			 				 				 				 	
		  }
    return 0;
}
 

void FillEEPROM_0XFF(void){
	 int add;
	 for(add=0;add<=0x3FF;add++) EEPROM_Write_Byte(add,0xFF);
}


/*------------------------------------------------------------------------------
                                    BUFFER                                     |
------------------------------------------------------------------------------*/
void EEPROM_Write_Buffer(unsigned int addr,char *dado,char size)
{
	char i;
    for(i=0;i<size;i++)
    {
     EEPROM_Write_Byte(addr,*dado);
     addr++;
     dado++;
    }
}

//------------------------------------------------------------------------------
void EEPROM_Read_Buffer(unsigned int addr,char *dado,char size)
{
	char i;
    for(i=0;i<size;i++)  
        {
        (*dado)=EEPROM_Read_Byte(addr);        
        addr++;
        dado++;
        };
}



void EEPROM_Write_Byte(unsigned int addr, unsigned char dado)
{
  EEPROM[addr]=dado;    
}


//------------------------------------------------------------------------------
unsigned char EEPROM_Read_Byte(unsigned int addr)
{    
   return EEPROM[addr];
}



void PROCULUS_Clear_Line_Graphic(char canal){
	 //printf("Limpou o canal %d do Grafico.\n",canal);
}
   

int16_t PROCULUS_VP_Read_UInt16(int trendvp)
{
	return memoria[trendvp];
}  
  
void PROCULUS_VP_Write_UInt16(int add, int data)
{
	memoria[add]=data;
}

void my_delay_ms(int value){
	
}

void PROCULUS_Buzzer(int tempo){
}

void my_delay_ms_CLRWDT(int tempo){
}

