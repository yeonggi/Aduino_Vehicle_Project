#include "Arduino.h"
#include "HomeAutoLight.h"
#include <avr/sleep.h>



//////////global variable
#define THRES_OF_MISS  10
#define THRES_OF_DATA	5
#define NOT_DEFINED_YET 0xff

int fixed_distance_data;
unsigned char miss_count;
unsigned char hit_count;
unsigned char mean_data_cnt;
int Sensing_range = 120;


/***************************Sleep Mode Class**************************************/
SleepMode::SleepMode()
{
	_led_pin = 0;
	_sleep_status = LOW;
}

void SleepMode::sleepNow()
{

	set_sleep_mode(SLEEP_MODE_PWR_SAVE);   // sleep mode is set here
	//SLEEP_MODE_PWR_DOWN -> external interrupt and WathchDog Timer
	sleep_enable();
	//Serial.println("Go Sleep Mode !!!!!");
	attachInterrupt(0, wakeUpNow, CHANGE); // use interrupt 0 (pin 2) and run function
	sleep_mode(); // here the device is actually put to sleep!!
	sleep_disable();    // first thing after waking from sleep:
	detachInterrupt(0);      // disables interrupt 0 on pin 2 so the

}
/***************************LED Function**************************************/
LedOut::LedOut()
{
	pinMode(led_out_pin1,OUTPUT);
	pinMode(led_out_pin2,OUTPUT);
	pinMode(led_out_pin3,OUTPUT);
	pinMode(led_out_pin4,OUTPUT);
}

void LedOut::flash(int delay_ms, int flash_time)
{
	int i;
	for(i=0; i<flash_time; i++)
	{
		digitalWrite(led_out_pin1,HIGH);
		digitalWrite(led_out_pin2,HIGH);
		digitalWrite(led_out_pin3,HIGH);
		digitalWrite(led_out_pin4,HIGH);
		//digitalWrite(led_out_pin5,HIGH);///
		delay(delay_ms);

		digitalWrite(led_out_pin1,LOW);
		digitalWrite(led_out_pin2,LOW);
		digitalWrite(led_out_pin3,LOW);
		digitalWrite(led_out_pin4,LOW);
		//digitalWrite(led_out_pin5,LOW);
		delay(delay_ms);
	}

}

void LedOut::Event_flash()
{
	int i,j;
	int delay_ms = 25;
	int flash_count = 2;
	int pin_bright = led_out_pin1;
	for (j=0; j<4; j++)
	{

		for(i=0; i<flash_count; i++)
		{
			digitalWrite(pin_bright,HIGH);
			delay(delay_ms);
			digitalWrite(pin_bright,LOW);
			delay(delay_ms);
		}
		pin_bright++;
	}


}

/***************************CCD Sensor Class**************************************/
CCDSensor::CCDSensor(int sensing_pin, int interrupt_pin)
{
	Serial.begin(9600);
	pinMode(sensing_pin, INPUT);
	pinMode(interrupt_pin, INPUT);
	_sensing_pin = sensing_pin;
	_interrupt_pin = interrupt_pin;

	Bright_State =0;
	Dark_State = 0;
	_light_val = 0;
	_digit_val = 0;
	_before_sleep_state = 0;

}

int CCDSensor::get_anal_light_val()
{
	_light_val = analogRead(_sensing_pin) / 2;
	if(_light_val>400)
	{
		//Serial.println("Strong Light Detected  --");
	}
	return _light_val;
	//analogWrite(ledPin, analogRead(lightPin)/2);  //send the value to the ledPin. Depending on value of resistor
}

int CCDSensor::get_digit_light_val()
{
	_digit_val = digitalRead(_interrupt_pin);
	return _digit_val;
	//analogWrite(ledPin, analogRead(lightPin)/2);  //send the value to the ledPin. Depending on value of resistor
}

void CCDSensor::print_Value()
{
	Serial.println("====================start====================");
	Serial.print("CCD Sensing Value (30Kohm) = ");
	Serial.println(get_anal_light_val());

	Serial.print("Bright Digital value = ");
	Serial.println(get_digit_light_val());

	Serial.print("Bright state = ");
	Serial.print(Bright_State);
	Serial.print(", Dark State == " );
	Serial.print(Dark_State);
	Serial.println();

	Serial.print("before state : ");
	Serial.println(_before_sleep_state);
	Serial.println("======================end======================");

}

int CCDSensor::check_state_go_sleep()
{
	if(get_anal_light_val() > 100 && get_anal_light_val() < 500)
	{
		Bright_State++;
		if(Bright_State > 5)
		{
			Bright_State = 0;
			Dark_State = 0;
			return BRIGHT_STATE;
		}
	}

	else if(get_anal_light_val() >= 0 && get_anal_light_val() <= 100)
	{
		Dark_State++;
		if(Dark_State > 5)
		{
			Bright_State =0;
			Dark_State = 0;
			return DARK_STATE;
		}
	}
	delay(5);

	return false;
}

void CCDSensor::AutoLight_Power_Saving_Main()
{
	int state = 0;
	//int sleep_state =0;

	state = check_state_go_sleep();

	if(_before_sleep_state == 0 && state > 0)
	{
		if(state == DARK_STATE)
			_before_sleep_state = BRIGHT_STATE;
		else
			_before_sleep_state = DARK_STATE;
	}

	if(state ==  BRIGHT_STATE && _before_sleep_state == DARK_STATE)
	{
		Serial.println("State Bright");
		_before_sleep_state = BRIGHT_STATE;
		LedObj.flash(500,5);
	}

	if(state ==  DARK_STATE && _before_sleep_state == BRIGHT_STATE)
	{
		Serial.println("State Darked");
		_before_sleep_state = DARK_STATE;
		//Serial.println("Get Darked Turn on LED");
		LedObj.flash(6000,2);
	}

/*
	if(state > 0)
	{
		while(1){
			sleep_state = check_state_go_sleep();
			if(sleep_state>0)
				break;
		}
		if(sleep_state == state)
			sleepObj.sleepNow();
		else
			_before_sleep_state = 0;

	}*/

	delay(5);
}


/***************************RC Function**************************************/
RCfunction::RCfunction()
{
	Serial.begin(9600);
#if(Transe_mode == 1)
	RC_function.enableTransmit(rc_sender);
	RC_function.setRepeatTransmit(8);
#else
	RC_function.enableReceive(1);  // Receiver on inerrupt 1 => that is pin #3
#endif

}


void RCfunction::Send_Signal(char send_Char)
{
	RC_function.send(send_Char, 8);
	digitalWrite(led_out_pin1,HIGH);
	delay(500);
	digitalWrite(led_out_pin1,LOW);
}

int RCfunction::Recive_Siganl()
{
	if (RC_function.available()) {

	    int value = RC_function.getReceivedValue();

	    if (value == 0) {
	      Serial.print("Unknown encoding");
	    }
	    else {
	      Serial.print("Received ");
	      Serial.print( RC_function.getReceivedValue(),HEX );
	      Serial.print(" / ");
	      Serial.print( RC_function.getReceivedBitlength() );
	      Serial.print("bit ");
	      Serial.print("Protocol: ");
	      Serial.println( RC_function.getReceivedProtocol() );
	    }

	    RC_function.resetAvailable();
	    return value;
	}
	return 0;
}


/***************************distance sensor Function**************************************/
Distance_Sensor::Distance_Sensor(int t_pin, int e_pin)
{
	pinMode(t_pin, OUTPUT);
	pinMode(e_pin, INPUT);
	memset(Queue,0,sizeof(Queue));


}
void Distance_Sensor::HC_SR04_generate_signal()
{
	digitalWrite(triger_pin, LOW);  // Added this line
	delayMicroseconds(2); // Added this line
	digitalWrite(triger_pin, HIGH);
	delayMicroseconds(10); // Added this line
	digitalWrite(triger_pin, LOW);
}

long Distance_Sensor::HC_SR04_decode_signal()
{
	long duration,distance;
	duration = pulseIn(echo_pin, HIGH);
	distance = (duration/2) / 29.1;
	return distance;
}

/***************************Application Function**************************************/

void HALS::Autolight_Body()
{
	int state = 0;
	int signal=0;
	int loop_count = 0;

	state = Light_object.check_state_go_sleep();
	signal = remote_control.Recive_Siganl();

	if(signal == 'a')
	{
		Serial.println("Signal Received ");
		while(1){
			loop_count++;
			if(Light_object.check_state_go_sleep() == DARK_STATE){
				state = DARK_STATE;
				Light_object._before_sleep_state = BRIGHT_STATE;
				break;
			}
			if(loop_count > 10)
			{
				state = EVENT_STATE;
				Serial.println("Not detected dark state so get out from loop !!");
				break;
			}
		}
	}

	if(Light_object._before_sleep_state == 0 && state > 0)
	{
		if(state == DARK_STATE)
			Light_object._before_sleep_state = BRIGHT_STATE;
		else
			Light_object._before_sleep_state = DARK_STATE;
	}

	else if(state ==  BRIGHT_STATE && Light_object._before_sleep_state == DARK_STATE)
	{
		Serial.println("State Bright");
		Light_object._before_sleep_state = BRIGHT_STATE;
		Light_object.LedObj.flash(50,10);
	}

	else if(state ==  DARK_STATE && Light_object._before_sleep_state == BRIGHT_STATE)
	{
		Serial.println("State Dark");
		Light_object._before_sleep_state = DARK_STATE;
		Light_object.LedObj.flash(12000,1);///
	}
	else if(state == EVENT_STATE)
	{
		Light_object.LedObj.Event_flash();
		Serial.Trace(T_Test,"In event state There was bright state Just before \n\n");//
	}


	delay(5);

}


void HALS::Autolight_Sender()
{
	int i;
	int distance=0;
	memset(HC_SR04.Queue,0,sizeof(int)*max_sensor_q_size);

	for(i=0; i<max_sensor_q_size; i++)
	{
		HC_SR04.HC_SR04_generate_signal();
		HC_SR04.Queue[i] = HC_SR04.HC_SR04_decode_signal();
		delayMicroseconds(5);
		//Serial.printf("--%d ",HC_SR04.Queue[i]);
	}

	distance = get_reliable_data(HC_SR04.Queue,max_sensor_q_size);
	distance = leaning_enviroment(distance);

	//Serial.println("-----------------------------");
	if(distance > 200 && distance < 0)
	{
		//Serial.println("out of range !!!");
	}
	else if(distance < Sensing_range)
	{
		//Serial.println("distance lower than 120");
		remote_control.Send_Signal('a');
		Serial.printf("distance = %d \n",distance);
		fixed_distance_data = 0;
		mean_data_cnt = 0;
	}
	else
	{
		//Serial.print("distance = ");
		Serial.println(distance);//
	}
	//Serial.println("-----------------------------");
}





/***************************C Function**************************************/
void wakeUpNow()       // LED DisPlay
{

	// execute code here after wake-up before returning to the loop() function
	// timers and code using timers (serial.print and more...) will not work here.
	// we don't really need to execute any special functions here, since we
	// just want the thing to wake up
	digitalWrite(12, HIGH);
	delay(1000);
	digitalWrite(12, LOW);
}

int Get_Time_Data(int *time_data)
{
	int input;
	int i=0;
	int data_input=0;
	int data_out[5];
	int ch=0;
	enum date_string {YY, MM, DD, TT, mm};
	int date_buffer[11];

	char max_value[] = {99, 12, 32, 24, 60};
	Serial.begin(9600);  //Begin serial communcation

 	restart :

	input = 0;
  	memset(date_buffer,0,sizeof(date_buffer));
  	memset(data_out,0,sizeof(data_out));
  	Serial.println(" put date YY-MM-DD-TT-mm (No line ending mdoe): ");

  	while(1)
  	{
  		if(Serial.available())
  		{
  			// ascii 48 -> 0
  			date_buffer[input] = Serial.read() - (int)48;
  			ch = Serial.read();
  			Serial.print(date_buffer[input]);
  			if(input%2 == 0 && input > 0)
  				Serial.print("-");

  			input++;
  			//\r == 13 \n == 10
  			//only behind code work
  			if(date_buffer[input-1] == -35 || date_buffer[input-1] == -38 || input == 10)
  			{
  				Serial.println();
  				for(i=0; i<10; i=i+2)
  				{
  					data_input = (date_buffer[i]*10) + date_buffer[i+1];
  					if(data_input > 0 && data_input <= max_value[(int)(i/2)])
  					{
  						Serial.print("---");
  						Serial.print(data_input);
  						data_out[i/2] = data_input;
  					}
  					else
  					{
  						Serial.println("error retype date please");
  						goto restart;
  					}
  				}
  				if(i == 10)
  				{
  					Serial.println();
  					Serial.println("Time set done - go to program");
  					break;
  				}

  			}
  		}
  	}
  	memcpy(time_data,data_out,sizeof(data_out));
  	return 1;

}

int get_reliable_data(int* data, int data_count)
{
	int mean;
	int mean_2;
	int i = 0;
	int ok_flag=0;
	int variable_data_count = 0;
	int Stnadard_deviation=0;
	int *compare_data;

    compare_data = (int*) malloc(data_count*sizeof(int)+1);

    memset(compare_data,0,data_count*sizeof(int));

	restart :

	mean = 0;
	mean_2 = 0;
	variable_data_count = 0;
	ok_flag = 0;

	for (i = 0; i < data_count; i++)
	{
		if (compare_data[i] == 0)
		{
			mean = mean + data[i];
			mean_2 = mean_2 + pow(data[i],2);
			variable_data_count++;
		}
	}

    if(variable_data_count > 0)
    {
         mean_2 = mean_2/variable_data_count ;
         mean = mean/variable_data_count;

		if ((mean_2 - pow(mean, 2)) > 0)
			Stnadard_deviation = sqrt(mean_2 - pow(mean, 2));
		else
			Stnadard_deviation = 0;

	     //Serial.printf("Stnadard_deviation = %d \n",Stnadard_deviation);

    }
    else
    {
    	free(compare_data);
        return -2;
    }

    //Serial.printf(" mean = %d \n", mean);


    if(Stnadard_deviation > 50)
    {
    	for (i = 0; i < data_count; i++)
    	{
    		if(compare_data[i] == 0)
    		{
    			if (abs(mean - data[i]) > Stnadard_deviation)
    			{
    				compare_data[i] = data[i];
    				ok_flag++;
    			}
    		}
    	}
    }


	if(ok_flag == data_count)
	{
		free(compare_data);
        return -1;
	}
	else if(ok_flag == 0)
		free(compare_data);
	else
		goto restart;

	return mean;

}

int leaning_enviroment(int mean_data)
{

    // start
	if(fixed_distance_data == 0 || fixed_distance_data == NOT_DEFINED_YET )
	{
		if(mean_data_cnt >= 2)
		{
			fixed_distance_data = mean_data;
		}
		else
		{
		    fixed_distance_data = NOT_DEFINED_YET;
		    Sensing_range = 0;
		}

	    mean_data_cnt++;

	}
	else if((fixed_distance_data - THRES_OF_DATA) < mean_data && \
       (fixed_distance_data + THRES_OF_DATA) >= mean_data )
	{
		hit_count++;
		if(miss_count > 0)
            miss_count--;
	}
	else
	{
		miss_count++;
		if(hit_count > 0)
            hit_count--;
	}

	if(miss_count > THRES_OF_MISS)
	{
		fixed_distance_data = mean_data;
		miss_count = 0;
		hit_count = 0;
	}
	if(hit_count > 20)
	{
		Sensing_range = mean_data + 10;
		// 여기에 가까이 계속 있을 때 처리 모듈 설계 해 넣어야해
	}

	return fixed_distance_data;
}

