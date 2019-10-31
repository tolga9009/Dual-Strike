void initMAMEReports() {
	data.mame_reports.keyboard.reportID = 1;
	data.mame_reports.consumer.reportID = 2;
}

void resetMAMEReports() {
	data.mame_reports.keyboard.previousData[0] = data.mame_reports.keyboard.data[0];
	data.mame_reports.keyboard.previousData[1] = data.mame_reports.keyboard.data[1];
	data.mame_reports.keyboard.previousData[2] = data.mame_reports.keyboard.data[2];
	data.mame_reports.keyboard.previousData[3] = data.mame_reports.keyboard.data[3];
	
	data.mame_reports.keyboard.data[0] =
	data.mame_reports.keyboard.data[1] =
	data.mame_reports.keyboard.data[2] =
	data.mame_reports.keyboard.data[3] = 0;

	data.mame_reports.consumer.data = 0;
}

#define MAME_IS_NOT_IDLE (\
	(data.mame_reports.keyboard.previousData[0] != data.mame_reports.keyboard.data[0])\
||	(data.mame_reports.keyboard.previousData[1] != data.mame_reports.keyboard.data[1])\
||	(data.mame_reports.keyboard.previousData[2] != data.mame_reports.keyboard.data[2])\
||	(data.mame_reports.keyboard.previousData[3] != data.mame_reports.keyboard.data[3])\
)


#define MAME_REPORT_UP 				{ data.mame_reports.keyboard.data[0] |= (1<<0); }
#define MAME_REPORT_RIGHT			{ data.mame_reports.keyboard.data[0] |= (1<<1); }
#define MAME_REPORT_DOWN			{ data.mame_reports.keyboard.data[0] |= (1<<2); }
#define MAME_REPORT_LEFT			{ data.mame_reports.keyboard.data[0] |= (1<<3); }
#define MAME_REPORT_LCTRL_ARRAY 	0
#define MAME_REPORT_LCTRL_BIT		4
#define MAME_REPORT_LALT_ARRAY		0
#define MAME_REPORT_LALT_BIT		5
#define MAME_REPORT_LSHIFT_BIT		6
#define MAME_REPORT_LSHIFT_ARRAY	0
#define MAME_REPORT_ENTER			{ data.mame_reports.keyboard.data[0] |= (1<<7); }
#define MAME_REPORT_ESCAPE			{ data.mame_reports.keyboard.data[1] |= (1<<0); }
#define MAME_REPORT_SPACE_ARRAY		1
#define MAME_REPORT_SPACE_BIT		1
#define MAME_REPORT_TAB				{ data.mame_reports.keyboard.data[1] |= (1<<2); }
#define MAME_REPORT_F2				{ data.mame_reports.keyboard.data[1] |= (1<<3); }
#define MAME_REPORT_F3				{ data.mame_reports.keyboard.data[1] |= (1<<4); }
#define MAME_REPORT_A_ARRAY			1
#define MAME_REPORT_A_BIT			5
#define MAME_REPORT_D				{ data.mame_reports.keyboard.data[1] |= (1<<6); }
#define MAME_REPORT_F				{ data.mame_reports.keyboard.data[1] |= (1<<7); }
#define MAME_REPORT_G				{ data.mame_reports.keyboard.data[2] |= (1<<0); }
#define MAME_REPORT_I_ARRAY			2
#define MAME_REPORT_I_BIT			1
#define MAME_REPORT_K_ARRAY			2
#define MAME_REPORT_K_BIT			2
#define MAME_REPORT_P				{ data.mame_reports.keyboard.data[2] |= (1<<3); }
#define MAME_REPORT_Q_ARRAY			2
#define MAME_REPORT_Q_BIT			4
#define MAME_REPORT_R				{ data.mame_reports.keyboard.data[2] |= (1<<5); }
#define MAME_REPORT_S_ARRAY			2
#define MAME_REPORT_S_BIT			6
#define MAME_REPORT_W_ARRAY			2
#define MAME_REPORT_W_BIT			7
#define MAME_REPORT_X_ARRAY			3
#define MAME_REPORT_X_BIT			0
#define MAME_REPORT_Y_ARRAY			3
#define MAME_REPORT_Y_BIT			1
#define MAME_REPORT_1				{ data.mame_reports.keyboard.data[3] |= (1<<2); }
#define MAME_REPORT_2				{ data.mame_reports.keyboard.data[3] |= (1<<3); }
#define MAME_REPORT_5				{ data.mame_reports.keyboard.data[3] |= (1<<4); }
#define MAME_REPORT_6				{ data.mame_reports.keyboard.data[3] |= (1<<5); }
#define MAME_REPORT_9				{ data.mame_reports.keyboard.data[3] |= (1<<6); }
#define MAME_REPORT_ERROR			{ data.mame_reports.keyboard.data[3] |= (1<<7); }

#define MAME_REPORT_VOL_UP	 		{ data.mame_reports.consumer.data |= (1<<0); }
#define MAME_REPORT_VOL_DOWN 		{ data.mame_reports.consumer.data |= (1<<1); }


#define MAME_SET_REPORT_BIT_FUNCTION(functionName, array, bit) void functionName() { data.mame_reports.keyboard.data[array] |= (1<<bit); }
// meaning: LK, MK, HK, 4K, LP, MP, HP, 4P
static int buttonMapping1[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
static int buttonMapping2[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
static int buttonMapping3[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
//static int buttonMapping4[8] = {-1, -1, -1, -1, -1, -1, -1, -1};

static int* buttonMapping = buttonMapping1;

#define MAME_LK 0
#define MAME_MK 1
#define MAME_HK 2
#define MAME_4K 3
#define MAME_LP 4
#define MAME_MP 5
#define MAME_HP 6
#define MAME_4P 7

#define MAME_BUTTON_CFG(buttonLayout, stickButton, mameButton) CFG_MAME_BL##buttonLayout##_##stickButton##_##mameButton

#define MAME_LOAD_BUTTON_MAPPING(buttonLayout, stickButton) \
	if(CFG_MAME_BL##buttonLayout##_##stickButton##_NOTHING)\
		buttonMapping##buttonLayout[MAME_##stickButton] = -1;\
	else if(MAME_BUTTON_CFG(buttonLayout, stickButton, BUTTON1))\
		buttonMapping##buttonLayout[MAME_##stickButton] = 0;\
	else if(MAME_BUTTON_CFG(buttonLayout, stickButton, BUTTON2))\
		buttonMapping##buttonLayout[MAME_##stickButton] = 1;\
	else if(MAME_BUTTON_CFG(buttonLayout, stickButton, BUTTON3))\
		buttonMapping##buttonLayout[MAME_##stickButton] = 2;\
	else if(MAME_BUTTON_CFG(buttonLayout, stickButton, BUTTON4))\
		buttonMapping##buttonLayout[MAME_##stickButton] = 3;\
	else if(MAME_BUTTON_CFG(buttonLayout, stickButton, BUTTON5))\
		buttonMapping##buttonLayout[MAME_##stickButton] = 4;\
	else if(MAME_BUTTON_CFG(buttonLayout, stickButton, BUTTON6))\
		buttonMapping##buttonLayout[MAME_##stickButton] = 5;\

#define MAME_LOAD_BUTTON_MAPPINGS(buttonLayout)\
	MAME_LOAD_BUTTON_MAPPING(buttonLayout, LK)\
	MAME_LOAD_BUTTON_MAPPING(buttonLayout, MK)\
	MAME_LOAD_BUTTON_MAPPING(buttonLayout, HK)\
	MAME_LOAD_BUTTON_MAPPING(buttonLayout, 4K)\
	MAME_LOAD_BUTTON_MAPPING(buttonLayout, LP)\
	MAME_LOAD_BUTTON_MAPPING(buttonLayout, MP)\
	MAME_LOAD_BUTTON_MAPPING(buttonLayout, HP)\
	MAME_LOAD_BUTTON_MAPPING(buttonLayout, 4P)\

void initMAMEButtonMappings() {
	MAME_LOAD_BUTTON_MAPPINGS(1)
	MAME_LOAD_BUTTON_MAPPINGS(2)
	MAME_LOAD_BUTTON_MAPPINGS(3)
	//MAME_LOAD_BUTTON_MAPPINGS(4)
}

MAME_SET_REPORT_BIT_FUNCTION(mamePlayer1Button1, MAME_REPORT_LCTRL_ARRAY, MAME_REPORT_LCTRL_BIT)
MAME_SET_REPORT_BIT_FUNCTION(mamePlayer1Button2, MAME_REPORT_LALT_ARRAY, MAME_REPORT_LALT_BIT)
MAME_SET_REPORT_BIT_FUNCTION(mamePlayer1Button3, MAME_REPORT_SPACE_ARRAY, MAME_REPORT_SPACE_BIT)
MAME_SET_REPORT_BIT_FUNCTION(mamePlayer1Button4, MAME_REPORT_LSHIFT_ARRAY, MAME_REPORT_LSHIFT_BIT)
MAME_SET_REPORT_BIT_FUNCTION(mamePlayer1Button5, MAME_REPORT_X_ARRAY, MAME_REPORT_X_BIT)
MAME_SET_REPORT_BIT_FUNCTION(mamePlayer1Button6, MAME_REPORT_Y_ARRAY, MAME_REPORT_Y_BIT)

void (*player1Buttons[6])() = {mamePlayer1Button1, mamePlayer1Button2, mamePlayer1Button3, mamePlayer1Button4, mamePlayer1Button5, mamePlayer1Button6 };

MAME_SET_REPORT_BIT_FUNCTION(mamePlayer2Button1, MAME_REPORT_A_ARRAY, MAME_REPORT_A_BIT)
MAME_SET_REPORT_BIT_FUNCTION(mamePlayer2Button2, MAME_REPORT_S_ARRAY, MAME_REPORT_S_BIT)
MAME_SET_REPORT_BIT_FUNCTION(mamePlayer2Button3, MAME_REPORT_Q_ARRAY, MAME_REPORT_Q_BIT)
MAME_SET_REPORT_BIT_FUNCTION(mamePlayer2Button4, MAME_REPORT_W_ARRAY, MAME_REPORT_W_BIT)
MAME_SET_REPORT_BIT_FUNCTION(mamePlayer2Button5, MAME_REPORT_K_ARRAY, MAME_REPORT_K_BIT)
MAME_SET_REPORT_BIT_FUNCTION(mamePlayer2Button6, MAME_REPORT_I_ARRAY, MAME_REPORT_I_BIT)

void (*player2Buttons[6])() = {mamePlayer2Button1, mamePlayer2Button2, mamePlayer2Button3, mamePlayer2Button4, mamePlayer2Button5, mamePlayer2Button6 };

#define MAME_SET_BUTTON(playerNo, buttonID) { if(buttonMapping[buttonID] >= 0) (*player##playerNo##Buttons[buttonMapping[buttonID]])(); }

// one HID idle rate unit is 4ms
#define IDLE_RATE_UNIT_COUNT_CYCLES ((uchar)(F_CPU / 250 / 1024))
#define IDLE_RATE_OVERLOW_COUNT		(255 / IDLE_RATE_UNIT_COUNT_CYCLES)
#define IDLE_RATE_OVERLOW_CYCLES	(255 % IDLE_RATE_UNIT_COUNT_CYCLES)

static uchar idleRateCount = 0;

#include <avr/io.h>
#include <avr/interrupt.h>

void initIdleTimer() {
	TCCR0B = (1<<CS00) | (1<<CS02); // clock prescaling: 1024 cycles
}

void resetIdleTimer() {
	idleRateCount = 0;
	TCNT0 = 0;
}

uchar hasIdlePeriodPassed() {
	if(mameIdleRate == 0)
		return 0;

	if(idleRateCount == 255)
		return 1;

	while(TIFR0 & (1<<TOV0)) { // overflow
		if(idleRateCount < mameIdleRate) {
			uchar temp;

			temp = TCNT0;
			TCNT0 += IDLE_RATE_OVERLOW_CYCLES;

			if(TCNT0 > temp)
				TIFR0 |= (1<<TOV0); // reset overflow flag

			temp = idleRateCount;
			idleRateCount += IDLE_RATE_OVERLOW_COUNT;
			
			if(idleRateCount < temp) { //overflow
				idleRateCount = 255;
				break;
			}
		}
		else {
			TIFR0 |= (1<<TOV0); // reset overflow flag
			break;
		}
	}

	while((idleRateCount < 255) && (TCNT0 >= IDLE_RATE_UNIT_COUNT_CYCLES)) { // idle rate count cylce finished
		idleRateCount++;
		TCNT0 -= IDLE_RATE_UNIT_COUNT_CYCLES;
	}

	return (idleRateCount >= mameIdleRate);
}

void setMAMEReportPlayer1() {
	if (!Stick_Up)
		MAME_REPORT_UP
	
	if (!Stick_Down)
		MAME_REPORT_DOWN

	if (!Stick_Left)
		MAME_REPORT_LEFT
	
	if (!Stick_Right)
		MAME_REPORT_RIGHT

	if(!Stick_LK)
		MAME_SET_BUTTON(1, MAME_LK)

	if(!Stick_MK)
		MAME_SET_BUTTON(1, MAME_MK)

	if(!Stick_HK)
		MAME_SET_BUTTON(1, MAME_HK)

	if(!Stick_LP)
		MAME_SET_BUTTON(1, MAME_LP)

	if(!Stick_MP)
		MAME_SET_BUTTON(1, MAME_MP)

	if(!Stick_HP)
		MAME_SET_BUTTON(1, MAME_HP)

#ifdef EXTRA_BUTTONS
	if(!Stick_4K)
		MAME_SET_BUTTON(1, MAME_4K)

	if(!Stick_4P)
		MAME_SET_BUTTON(1, MAME_4P)
#endif

	if(!Stick_Start) 
		MAME_REPORT_1

	if(!Stick_Home)
		MAME_REPORT_P
}

void setMAMEReportPlayer2() {
	if (!Stick_Up)
		MAME_REPORT_R
	
	if (!Stick_Down)
		MAME_REPORT_F

	if (!Stick_Left)
		MAME_REPORT_D
	
	if (!Stick_Right)
		MAME_REPORT_G

	if(!Stick_LK)
		MAME_SET_BUTTON(2, MAME_LK)

	if(!Stick_MK)
		MAME_SET_BUTTON(2, MAME_MK)

	if(!Stick_HK)
		MAME_SET_BUTTON(2, MAME_HK)

	if(!Stick_LP)
		MAME_SET_BUTTON(2, MAME_LP)

	if(!Stick_MP)
		MAME_SET_BUTTON(2, MAME_MP)

	if(!Stick_HP)
		MAME_SET_BUTTON(2, MAME_HP)

#ifdef EXTRA_BUTTONS
	if(!Stick_4K)
		MAME_SET_BUTTON(2, MAME_4K)

	if(!Stick_4P)
		MAME_SET_BUTTON(2, MAME_4P)
#endif

	if(!Stick_Start) 
		MAME_REPORT_2

	if(!Stick_Home)
		MAME_REPORT_P
}

void setMAMEReportsControl() {
	if (!Stick_Up)
		MAME_REPORT_UP
	
	if (!Stick_Down)
		MAME_REPORT_DOWN

	if (!Stick_Left)
		MAME_REPORT_LEFT
	
	if (!Stick_Right)
		MAME_REPORT_RIGHT

	if(!Stick_LP)
		MAME_REPORT_ENTER

	if(!Stick_MP)
		MAME_REPORT_ESCAPE

	if(!Stick_LK)
		MAME_REPORT_F2

	if(!Stick_MK)
		MAME_REPORT_F3

	if(!Stick_Start)
		MAME_REPORT_TAB

#ifdef EXTRA_BUTTONS		
	if(!Stick_4P)
		MAME_REPORT_VOL_UP

	if(!Stick_4K)
		MAME_REPORT_VOL_DOWN
#endif
}

/*
#define MAME_SET_BUTTON_MAPPING(buttonLayout, stickButton)\
	if(!Stick_Select) {\
		CFG_SET_MAME_BL##buttonLayout##_##stickButton##_NOTHING(newConfig)\
	}\
	else if(!Stick_Up) {\
		if(!Stick_Right) {\
			CFG_SET_MAME_BL##buttonLayout##_##stickButton##_BUTTON2(newConfig)\
		}\
		else if(!Stick_Left) {\
		}\
		else {\
			CFG_SET_MAME_BL##buttonLayout##_##stickButton##_BUTTON1(newConfig)\
		}\
	}\
	else if(!Stick_Down) {\
		if(!Stick_Right) {\
			CFG_SET_MAME_BL##buttonLayout##_##stickButton##_BUTTON4(newConfig)\
		}\
		else if(!Stick_Left) {\
			CFG_SET_MAME_BL##buttonLayout##_##stickButton##_BUTTON6(newConfig)\
		}\
		else {\
			CFG_SET_MAME_BL##buttonLayout##_##stickButton##_BUTTON5(newConfig)\
		}\
	}\
	else if(!Stick_Right) {\
		CFG_SET_MAME_BL##buttonLayout##_##stickButton##_BUTTON3(newConfig)\
	}\
	else if(!Stick_Left) {\
	}\

#define MAME_READ_BUTTON_MAPPING(stickButton)\
	switch(configurationButtonMappingNo) {\
	case 1:\
		MAME_SET_BUTTON_MAPPING(1, stickButton)\
		break;\
	case 2:\
		MAME_SET_BUTTON_MAPPING(2, stickButton)\
		break;\
	case 3:\
		MAME_SET_BUTTON_MAPPING(3, stickButton)\
		break;\
	case 4:\
		MAME_SET_BUTTON_MAPPING(4, stickButton)\
		break;\
	}\

void configureMAMEButtonMappings() {
	uchar configurationButtonMappingNo;
	uint8_t newConfig[CONFIG_BYTE_WIDTH + 2];

	configurationButtonMappingNo = currentButtonMappingNo;
	readConfig(newConfig);

	while(!(!Stick_Start && !Stick_HP)) {
		if(!Stick_Start && !Stick_Up) {
			configurationButtonMappingNo = 1;
		}
		else if(!Stick_Start && !Stick_Right) {
			configurationButtonMappingNo = 2;
		}
		else if(!Stick_Start && !Stick_Down) {
			configurationButtonMappingNo = 3;
		}
		else if(!Stick_Start && !Stick_Left) {
			configurationButtonMappingNo = 4;
		}

		if(!Stick_LK) {
			MAME_READ_BUTTON_MAPPING(LK)
		}

		if(!Stick_MK) {
			MAME_READ_BUTTON_MAPPING(MK)
		}

		if(!Stick_HK) {
			MAME_READ_BUTTON_MAPPING(HK)
		}

		if(!Stick_4K) {
			MAME_READ_BUTTON_MAPPING(4K)
		}

		if(!Stick_LP) {
			MAME_READ_BUTTON_MAPPING(LP)
		}

		if(!Stick_MP) {
			MAME_READ_BUTTON_MAPPING(MP)
		}

		if(!Stick_HP) {
			MAME_READ_BUTTON_MAPPING(HP)
		}

		if(!Stick_4P) {
			MAME_READ_BUTTON_MAPPING(4P)
		}
	}

	writeConfig(newConfig);
	initMAMEButtonMappings();
}
*/

void sendMAMEReports() {	
	resetMAMEReports();
	/*
	joystick mode setting corresponds to the MAME mode, either set by
	SPDT switch or on the fly switching:
		-left analogue switch: player 1
		-digital pad: control mode
		-right analogue switch: player 2
	*/
	updateJoystickMode();

	if(CFG_DIGITAL_PAD) {
		setMAMEReportsControl();
	}
	else {
		if(CFG_LEFT_STICK)
			setMAMEReportPlayer1();
		else if(CFG_RIGHT_STICK)
			setMAMEReportPlayer2();
	}

	if(hasIdlePeriodPassed() || MAME_IS_NOT_IDLE) {
		sendDataUSB((void*)&data.mame_reports.keyboard, 5);
		resetIdleTimer();
	}

	if(data.mame_reports.consumer.data)
		sendDataUSB((void*)&data.mame_reports.consumer, 2);
}

void mame_controller() {
	usbMode = USB_MODE_MAME;

	if(CFG_MAME_DEFAULT_PLAYER_1) {
		CFG_SET_LEFT_STICK(config);
	}
	else {
		CFG_SET_RIGHT_STICK(config);
	}
	
	setupUSB();
	usbPoll();
	initMAMEReports();
	initMAMEButtonMappings();
	initIdleTimer();

    while(1) { /* main event loop */
        sendMAMEReports();
    }
}
