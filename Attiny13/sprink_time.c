#include <avr/io.h>
#include <stdint.h>

//for_pcb_version LED>>>PORTB3 & PB>>PINB4. otherwise its inverted.

uint8_t dig_read(uint8_t pin){

	uint8_t LOG_ = PINB & (1<<pin);

	if(LOG_==(1<<pin))return 1;
	else return 0;
}

//1sec=0.05*20

uint8_t hours_,minutes_,seconds_,pb_stat;
//uint16_t dump;



void _TIMER_NORMAL(volatile uint8_t COUNT_){

        TCCR0A=0;
        TCCR0B=0b101;

        for(volatile uint8_t i=0;i<COUNT_;i++){
                while(!(TIFR0&2));
                TIFR0=2;

        }
        TCCR0B=0;
	TIFR0=0;
}


uint16_t ANL_READ(uint8_t channel){

	//ADMUX=0;	
	ADMUX=channel;

	ADCSRA=(0b111)|(1<<7)|(1<<6);


	while(!(ADCSRA&(1<<4)));

	ADCSRA|=(1<<4);

	uint16_t LOG_= ((uint16_t)ADCL)|(((uint16_t)ADCH)<<8);

	return LOG_;
}
//DEFAULT_WAIT_TIME_TOTAL=13.5s[count_=1]
//normally_low

void _pump_on(volatile uint8_t count_){
	for(volatile uint8_t i=0;i<count_;i++){

		PORTB=1;
		_TIMER_NORMAL(25);
		PORTB=1;
		_TIMER_NORMAL(25);
		PORTB=0;
		_TIMER_NORMAL(200);
		PORTB=0;
		_TIMER_NORMAL(200);
	}

}

int main(){

 //init_time
 hours_=14;
 minutes_=59;
 seconds_=57;

 DDRB=0b1001;

 PORTB=0b1000;

 _TIMER_NORMAL(50);
 PORTB=0b0000;
 _TIMER_NORMAL(50);
 PORTB=0b1000;
 _TIMER_NORMAL(50);
 PORTB=0b0000;
 _TIMER_NORMAL(50);
 PORTB=0b1000;
 _TIMER_NORMAL(50);
 PORTB=0b0000;
 _TIMER_NORMAL(50);
 PORTB=0b1000;
 _TIMER_NORMAL(50);
 PORTB=0b0000;
 
 while(1){

	 while(pb_stat==1){

	 	if(dig_read(4)==1){
	 	 pb_stat=0;
		 _TIMER_NORMAL(200);
		 PORTB=0b0000;
		 _TIMER_NORMAL(50);
		 PORTB=0b1000;
		 _TIMER_NORMAL(50);
		 PORTB=0b0000;
		 _TIMER_NORMAL(50);
		 PORTB=0b1000;
		 _TIMER_NORMAL(50);
		 PORTB=0b0000;
		 _TIMER_NORMAL(50);
		 PORTB=0b1000;
		 _TIMER_NORMAL(50);
		 PORTB=0b0000;
		 //err_=0;
		 hours_=14;
		 minutes_=59;
		 seconds_=57;
	 	 break;
	 	}

		PORTB=0b0000;
		_TIMER_NORMAL(50);
		PORTB=0b1000;
		_TIMER_NORMAL(50);
		
		//if(err_==0)ANL_READ(1);

	 	if(ANL_READ(1)>=360)_pump_on(3);

	 	//err_=1;



	 }

	 while(pb_stat==0){

	 	//start_time
	 	while(hours_<24){


	 	 while(minutes_<60){
 	 	

	 	  while(seconds_<60){

	 	  	_TIMER_NORMAL(60);
	 	  	seconds_++;

	 	  	if(dig_read(4)==1){
	 	  	 pb_stat=1;
			 _TIMER_NORMAL(50);
			 PORTB=0b0000;
			 _TIMER_NORMAL(50);
			 PORTB=0b1000;
			 _TIMER_NORMAL(50);
			 PORTB=0b0000;
			 _TIMER_NORMAL(50);
			 PORTB=0b1000;
			 _TIMER_NORMAL(50);
			 PORTB=0b0000;
			 _TIMER_NORMAL(50);
			 PORTB=0b1000;
			 _TIMER_NORMAL(50);
			 PORTB=0b0000;
	 	  	 break;
	 	  	}

	 	  	//end----
	 	  }

			if(pb_stat==1)break;

	 	  	minutes_++;
	 	  	seconds_=0;
	 	  	//end----
	 	 }

	 	 	if(pb_stat==1)break;

	 	 	hours_++;

	 	 	if((hours_==3)||(hours_==6)||(hours_==9)||(hours_==12)||(hours_==15)||(hours_==18)||(hours_==21)){
	 	 		_pump_on(3);
	 	 		//minutes_=1;
				seconds_=41;
	 	 	}else minutes_=0;
	 	 	//end----

	 	}
	 	
	 	if(pb_stat==1)break;
	 	
	 	hours_=0;

	 	//activate_pump
	 	_pump_on(3);
	 	seconds_=41;
		//minutes_=1;

	 }
 }

 return 0;

}
