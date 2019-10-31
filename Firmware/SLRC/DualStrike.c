#include "DualStrike.h"

#include <avr/eeprom.h> /* EEPROM functions */
#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include <util/delay.h>     /* for _delay_ms() */

// README
/* 
Configuration Mode
==================
In the configuration mode the behaviour of the Dual Strike can be changed. 
While plugging in the USB Cable just press Select. Leave it by pressing Start. 
*/

#define WORKING_MODE_PT		0
#define WORKING_MODE_PS3	1
#define WORKING_MODE_PC		2
#define WORKING_MODE_AUTO	3

#if (ATMEGA_NO == 168 || ATMEGA_NO == 328)
#define WORKING_MODE_MAME	4
#define WORKING_MODE_XBOX	5
#endif

#define enableUsbLines()	S1_PORT |=  (1<<S1_PIN);
#define disableUsbLines()	S1_PORT &= ~(1<<S1_PIN);


// declares config and config_EEPROM
CFG_DECLARATION

uchar configsDiffer(uint8_t newConfig0[CONFIG_BYTE_WIDTH + 2]) {
	for(int i = 0; i < (CONFIG_BYTE_WIDTH + 2); i++)
		if(config[i] != newConfig0[i])
			return 1;

	return 0;
}

void writeConfig(uint8_t newConfig[CONFIG_BYTE_WIDTH + 2]) {
	if(configsDiffer(newConfig)) {
		// if newConfig was changed update configuration
		eeprom_write_block(newConfig, config_EEPROM, CONFIG_BYTE_WIDTH + 2);

		for(int i = 0; i < (CONFIG_BYTE_WIDTH + 2); i++)
			config[i] = newConfig[i];
	}
}

void readConfig(uint8_t newConfig[CONFIG_BYTE_WIDTH + 2]) {
	 // read config from EEPROM
	eeprom_read_block(config, config_EEPROM, CONFIG_BYTE_WIDTH + 2);

	if(config[0] != CONFIG_DEVICE || config[1] != CONFIG_VERSION) {
		/* if EEPROM does not conform to current version and device set to default config */
        CONFIG_SET_DEFAULTS(newConfig)
		writeConfig(newConfig);
	}
	else {
		for(int i = 0; i < (CONFIG_BYTE_WIDTH + 2); i++)
			newConfig[i] = config[i];
	}
}

void configInit() {
	uint8_t newConfig[CONFIG_BYTE_WIDTH + 2];

	readConfig(newConfig);

	if(!Stick_Select) {
		/* enter configuration modification mode */

		enableUsbLines();
		programmer_setup();

		while(Stick_Start)
			programmer_poll();

		disableUsbLines();
		readConfig(newConfig);
	}
}

int setModePS3() {
	enableUsbLines();

	return WORKING_MODE_PS3;
}

int setModePC() {
	S3_PORT |= (1<<S3_PIN); // pin S3 is high
	S4_PORT |= (1<<S4_PIN); // pin S4 is high
	enableUsbLines();

	return WORKING_MODE_PC;
}

#if (ATMEGA_NO == 168 || ATMEGA_NO == 328)
int setModeMAME() {	
	enableUsbLines();

	return WORKING_MODE_MAME;
}

int setModeXBox() {	
	enableUsbLines();

	return WORKING_MODE_XBOX;
}
#endif

int setModePT() {
	if(CFG_JOYSTICK_SWITCH_EMU || CFG_INVERTED_TRIGGERS) {
        S3_PORT &= ~(1<<S3_PIN); // pin S3 is low	
        S4_PORT &= ~(1<<S4_PIN); // pin S4 is low
	}

	disconnectUSB();
	disableUsbLines();

	S2_PORT |= (1<<S2_PIN); // enable pass-through usb lines

	return WORKING_MODE_PT;
}

int setModeAutodetect() {
	return WORKING_MODE_AUTO;
}


int setModeDefault() {
	if(CFG_DEF_WORK_MODE_PS3)
		return setModePS3();
	else if(CFG_DEF_WORK_MODE_PC)
		return setModePC();
#if (ATMEGA_NO == 168 || ATMEGA_NO == 328)
	else if(CFG_DEF_WORK_MODE_MAME)
		return setModeMAME();
	else if(CFG_DEF_WORK_MODE_XBOX)
		return setModeXBox();
#endif
	else
		return setModePT();
}


// README
/*
By pressing a button or joystick direction while the Dual Strike gets activated, it can be set into the different modes:

    * If the Select button is pressed, then configuration mode is entered.
    * If the Start button is pressed, then firmware update mode is entered.
    * The working modes can be activated as follows:
          o If only one working mode is enabled, then this working mode will be activated in any case.
          o If two working modes are enabled and one of the buttons LK, MK, LP, MP, HP and 4P is pressed, then the non-default mode is activated.
          o If more than two working modes are enabled,:
                + the button LK is pressed and the PS3 mode is enabled, then the PS3 mode is activated.
                + the button MK is pressed and the MAME mode is enabled, then the MAME mode is activated.
                + the button HK is pressed and the PC mode is enabled, then the PC mode is activated.
                + the button LP is pressed and the XBox mode is enabled, then the XBox mode is activated.
                + the button MP is pressed and the pass-through mode is enabled, then the pass-through mode is activated.
*/
int hardwareInit() {
	DDRC	= 0b00000000;	// PINC inputs
	PORTC	= 0b00111111;	// PORTC with pull-ups except S4

	DDRB	= 0b00000000;	// PINB inputs
	PORTB	= 0b00111111;	// PORTB with pull-ups except clock
	

	DDRD	= 0b00001001;  // PIND inputs, except S1 and S2
	PORTD	= 0b11100000;  // PORTD with pull-ups except S1, S2, D+, D- and S3

	configInit();

	if(!Stick_Up) // [precedence]
	{
		CFG_SET_DIGITAL_PAD(config)
	}
	else if(!Stick_Left) {
		CFG_SET_LEFT_STICK(config)
	}
	else if(!Stick_Right) {
		CFG_SET_RIGHT_STICK(config)
	}

	if(CFG_JOYSTICK_SWITCH_READ || CFG_X3_READ) {	
		S3_PORT |= (1<<S3_PIN); // pin S3 is high
		S4_PORT |= (1<<S4_PIN); // pin S4 is high
	}

	int enabledWorkingModes = 0;
	
	if(CFG_WORK_MODE_PS3_ENABLED)
		enabledWorkingModes++;

	if(CFG_WORK_MODE_PC_ENABLED)
		enabledWorkingModes++;

#if (ATMEGA_NO == 168 || ATMEGA_NO == 328)
	if(CFG_WORK_MODE_MAME_ENABLED)
		enabledWorkingModes++;
	
	if(CFG_WORK_MODE_XBOX_ENABLED)
		enabledWorkingModes++;
#endif

	if(CFG_WORK_MODE_PT_ENABLED)
		enabledWorkingModes++;
	
	if(CFG_DEF_WORK_MODE_AUTODETECT || enabledWorkingModes > 2) {
		if(!Stick_LK && CFG_WORK_MODE_PS3_ENABLED)
			return setModePS3();

#if (ATMEGA_NO == 168 || ATMEGA_NO == 328)
		if(!Stick_MK && CFG_WORK_MODE_MAME_ENABLED)
			return setModeMAME();

		if(!Stick_LP && CFG_WORK_MODE_XBOX_ENABLED)
			return setModeXBox();
#endif

		if(!Stick_MP && CFG_WORK_MODE_PT_ENABLED)
			return setModePT();

		if(!Stick_HK && CFG_WORK_MODE_PC_ENABLED)
			return setModePC();		
	}
	else if(enabledWorkingModes == 2) {
		if(!Stick_LK
		|| !Stick_MK
		|| !Stick_LP
		|| !Stick_MP
		|| !Stick_HP
		|| !Stick_4P) {
			if(!CFG_DEF_WORK_MODE_PS3 && CFG_WORK_MODE_PS3_ENABLED)
				return setModePS3();

			if(!CFG_DEF_WORK_MODE_PC && CFG_WORK_MODE_PC_ENABLED)
				return setModePC();

#if (ATMEGA_NO == 168 || ATMEGA_NO == 328)
			if(!CFG_DEF_WORK_MODE_MAME && CFG_WORK_MODE_MAME_ENABLED)
				return setModeMAME();
	
			if(!CFG_DEF_WORK_MODE_XBOX && CFG_WORK_MODE_XBOX_ENABLED)
				return setModeXBox();
#endif	
		}
	}

	if(CFG_DEF_WORK_MODE_AUTODETECT)
		return setModeAutodetect();
	else
		return setModeDefault();
}

/* ------------------------------------------------------------------------- */
/* Here the meta-button functionality is defined. */

uchar metaPressed = 0;
uchar metaWasUsed = 0;
uint metaSendCount = 0;
uint metaSendRepeats = 0;

void updateMetaState() {
	uchar lastMetaPressed = metaPressed;
	
	if(CFG_META_BUTTON_START)
		metaPressed = !Stick_Start;
	else
		metaPressed = !Stick_Select;

	uchar metaReleased = lastMetaPressed && !metaPressed;

	if(metaWasUsed) {
		metaSendCount = 0;

		if(metaReleased)
			metaWasUsed = 0;
	}
	else if(metaReleased)
		metaSendCount = metaSendRepeats;
    else if(metaSendCount > 0)
		 metaSendCount--;
}

/* ------------------------------------------------------------------------- */

void updateJoystickMode() {
    if(CFG_JOYSTICK_SWITCH_READ) {
        if(!Stick_S3 && Stick_S4) { 	 // S3 low and S4 high
			CFG_SET_LEFT_STICK(config)
        }
        else if(Stick_S3 && !Stick_S4) { // S3 high and S4 low
			CFG_SET_RIGHT_STICK(config)
        }
        else if(Stick_S3 && Stick_S4) {  // S3 high and S4 high
			CFG_SET_DIGITAL_PAD(config)
        }
    }
	else {
		if(CFG_ON_THE_FLY_JOYSTICK_MODE_SWITCHING) {
			if(metaPressed) {
				if(!Stick_Up) {
					CFG_SET_DIGITAL_PAD(config)
					metaWasUsed = 1;
				}
				else if(!Stick_Left) {
					CFG_SET_LEFT_STICK(config)
					metaWasUsed = 1;
				}
				else if(!Stick_Right) {
					CFG_SET_RIGHT_STICK(config)
					metaWasUsed = 1;
				}
			}
		}
	}
}

/* ------------------------------------------------------------------------- */

/* buffer for data */
/*
NOTE:
As V-USB conforms to the USB low speed standard, the maximum packet size is 8 bytes for one call of usbSetInterrupt()!
But it is possible to send longer reports by using
---
while(!usbInterruptIsReady()) usbPoll();
---
to wait until the data is sent, then you can send the next part like this:
---
usbSetInterrupt((uchar *)&data + 8, 1*sizeof(uchar));
---
*/
uchar* data[132];

uchar autodetectCount = 0;
uchar autodetectLimit = 0;
uchar detected = 0;

void initAutodetectTimer() {
#if (ATMEGA_NO == 168 || ATMEGA_NO == 328)
	TCCR0B = (1<<CS00) | (1<<CS02); // clock prescaling: 1024 cycles
#else
	TCCR0 = (1<<CS00) | (1<<CS02); // clock prescaling: 1024 cycles
#endif
}

void resetAutodetectTimer() {
	autodetectCount = 0;
#if (ATMEGA_NO == 168 || ATMEGA_NO == 328)
	TCNT0 = 0;
	TIFR0 |= (1<<TOV0); // reset overflow flag
#else
	TCNT0 = 0;
	TIFR |= (1<<TOV0); // reset overflow flag
#endif
}

void resetAutodetect() {
	disableUsbLines();
	detected = 0;
}

uchar autodetectTimePassed() {
	if(
#if (ATMEGA_NO == 168 || ATMEGA_NO == 328)
		TIFR0 & (1<<TOV0)
#else
		TIFR & (1<<TOV0)
#endif
) { // overflow
		if(autodetectCount < 255)
			autodetectCount++;

#if (ATMEGA_NO == 168 || ATMEGA_NO == 328)
		TIFR0 |= (1<<TOV0); // reset overflow flag
#else
		TIFR |= (1<<TOV0); // reset overflow flag
#endif

	}

	return (autodetectCount >= autodetectLimit);
}

void autodetect() {
	initAutodetectTimer();

	if(CFG_WORK_MODE_PS3_ENABLED) {
		resetAutodetect();
		setModePS3();
		ps3_init_controller();

		while(detected == 0)
			usbPoll();
		
		autodetectLimit = 15;
		resetAutodetectTimer();

		while(!autodetectTimePassed())
			usbPoll();

		if(detected == 2)
			ps3_controller();
	}


#if (ATMEGA_NO == 168 || ATMEGA_NO == 328)
	if(CFG_WORK_MODE_XBOX_ENABLED) {
		resetAutodetect();
		setModeXBox();
		xbox_init_controller();

		while(detected == 0)
			usbPoll();

		autodetectLimit = 5;
		resetAutodetectTimer();

		while(!autodetectTimePassed())
			usbPoll();

		if(detected == 2)
			xbox_controller();
	}
#endif
	
	if(CFG_WORK_MODE_PC_ENABLED) {
		resetAutodetect();
		setModePC();
		pc_init_controller();

		while(detected == 0) {
			usbPoll();

			if(usbIsConfigured())
				detected = 1;
		}

		autodetectLimit = 80;
		resetAutodetectTimer();

		while(!autodetectTimePassed())
			pc_test_controller();

		if(detected == 2)
			pc_controller();
	}

	if(CFG_WORK_MODE_PT_ENABLED) {
		resetAutodetect();
		_delay_ms(300);
		setModePT();
		pass_through();
	}
}

int main(void) {
	switch(hardwareInit()) {
	case WORKING_MODE_AUTO:
	  autodetect();
	  break;

	case WORKING_MODE_PT:
	  pass_through();
	  break;

	case WORKING_MODE_PS3:
	  ps3_init_controller();
	  ps3_controller();
	  break;

	case WORKING_MODE_PC:
	  pc_init_controller();
	  pc_controller();
	  break;

#if (ATMEGA_NO == 168 || ATMEGA_NO == 328)
	case WORKING_MODE_MAME:
	  mame_controller();
	  break;

	case WORKING_MODE_XBOX:
	  xbox_init_controller();
	  xbox_controller();
	  break;
#endif
	}

    return 0;
}
