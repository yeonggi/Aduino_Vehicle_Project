#ifndef HomeAutoLight_h
#define HomeAutoLight_h

#include "Arduino.h"
#include "RCSwitch.h"
#include "TraceID.h"
/*
거리가 짧음

들어 왔다는 신호 전송(AScii)

현재 빛 상태를 확인

밝은 상태면 켜지 않음 / 어두운 상태면 켬

다시 Sleep 모드로 들어감
*/
//Min Max

#define max_sensor_q_size 		3
// about operation
#define power_saving_mode

/** Pin Definition **/
#define 	Sensing_pin  			0
//#define 	check_sleep_pin 		12
#define 	Interrupt_pin			2
#define 	Interrupt_pin1			3


#define		led_out_pin1			4
#define		led_out_pin2			5
#define		led_out_pin3			6
#define		led_out_pin4			7

// Sender Pin
#define		triger_pin				12
#define 	echo_pin				13
#define 	rc_sender				10
#define 	Transe_mode 			1 // Transe 1 Recieve 2

/**State Definition**/
#define 	DARK_STATE 		1
#define 	BRIGHT_STATE 	2
#define 	EVENT_STATE		3


extern "C" {
	void wakeUpNow();
	int Get_Time_Data(int* time_data);
	int get_reliable_data(int* data, int data_count);


}

class SleepMode
{
	public:
		SleepMode();
		void sleepNow();
		int _sleep_status;
	private:
		int _led_pin; //for check sleep mode

};

class LedOut
{
	public:
		LedOut();
		void flash(int delay_ms, int flash_time);
		void Event_flash();


};

class CCDSensor
{
	//embedded object


	public:
		CCDSensor(int sensing_pin, int interrupt_pin);
		int get_anal_light_val();
		int get_digit_light_val();
		void print_Value();
		int check_state_go_sleep();
		void AutoLight_Power_Saving_Main();

		void AutoLight_Nomal_Main();//coding need

		SleepMode sleepObj;
		LedOut	  LedObj;

		int _light_val;
		int _digit_val;
		int _before_sleep_state;

		int Bright_State;
		int Dark_State;

	private:
		int _interrupt_pin;
		int _sensing_pin;
};

class RCfunction
{
	RCSwitch RC_function = RCSwitch();

	public:
		RCfunction();
		//send function
		void Send_Signal(char send_Char);
		void Get_data_from_serial();
		void Get_data_from_distance_sensor();

		//recive function
		int Recive_Siganl();



};
//HC_SR04
class Distance_Sensor
{
	public :
		Distance_Sensor(int t_pin, int e_pin);
		void HC_SR04_generate_signal();
		long HC_SR04_decode_signal();
		int Queue[max_sensor_q_size];

};
//home auto light system
class HALS
{
	public :
		//HALS();
		void Autolight_Body();
		void Autolight_Sender();


		RCfunction remote_control = RCfunction();
		CCDSensor Light_object = CCDSensor(Sensing_pin,Interrupt_pin);
		Distance_Sensor HC_SR04 = Distance_Sensor(triger_pin,echo_pin);
};



#endif
