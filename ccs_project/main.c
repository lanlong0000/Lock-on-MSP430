#include <msp430.h> 
#include <key.c>
#include <intrins.h>
#include <string.h>
void Key_Proc(void);

unsigned long ms_Tick = 0;
unsigned int SlowDown_Key;
unsigned char Key_Value;
unsigned char Key_Old, Key_Down;
typedef unsigned char uchar;
typedef unsigned int uint;
uchar j;
uint number=100;
uint n=0;
uint fre=0;
uint set=0;
uint open=0;
uint BEEP;
uint LED;
uint Show;
uint lock;
uint password_test[8]={0,0,0,0,0,0,0,0};
uint password_admin[8]={1,1,1,1,1,1,1,1};
int i;
uchar table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};




int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	 WDT_init();
	    lock=1;
	    BEEP=1;
	    LED=0;
	    while(1)
	    {
	        if(fre>8){
	        fre=1;}
	        Key_Proc();

	        Show=0xff;
	        Show=table[number];
	        delay(5);

	        Show=0xff;
	        Show=table[fre];
	        delay(5);

	        Show=0xff;
	        Show=0x8e;
	        delay(5);

	        Show=0xff;
	        Show=0xc8;
	        delay(5);

	        i++;
	        if(i>8)
	            i=0;
	        if(fre==i&&set==1){
	            password_test[i]=number;
	            password_admin[i]=number;}

	        if(open==1&&(memcmp(password_test,password_admin,sizeof password_admin)==0))
	        {
	            lock=0;
	            open=0;
	            fre=0;
	            for(n=0;i<8;i++)
	                password_admin[i]=99;
	        }
	        if(n==3)
	        {
	            if(memcmp(password_test,password_admin,sizeof password_admin)!=0)
	            {
	            open=0;
	            fre=0;
	            BEEP=0;
	            delay(100);
	            for(n=0;i<8;i++)
	                password_admin[i]=99;
	            }
	        }



	        if(n!=3){
	            BEEP=1;}
	    }
	return 0;
}

void WDT_init()
{
    WDTCTL=WDT_ADLY_16;
    IE1|=WDTIE;
}

#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
    ms_Tick++;
    if(++SlowDown_Key == 3) SlowDown_Key = 0;
}

void    Key_Proc(void)
{
    if(SlowDown_Key) return;
    SlowDown_Key = 1;
    Key_Value = Key_Read();
    Key_Down = Key_Value & (Key_Old ^ Key_Value);
    Key_Old = Key_Value;

    if((Key_Down-1>=0) && (Key_Down-1<=9)){
        number=Key_Down-1;
        fre++;}
    if(Key_Down>=10)
        fre=0;
    if(Key_Down==11)
        set=1;
    if(Key_Down==12)
        set=0;
    if(Key_Down==13){
        open=1;
        n++;
        }
    if(Key_Down==14){
        open=0;
        lock = 1;
        n=0;
    }

}
