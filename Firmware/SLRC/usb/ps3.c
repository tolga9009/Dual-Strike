void resetPS3ReportBuffer() {
	data.ps3report.buttons[0] =
	data.ps3report.buttons[1] = 0;
	data.ps3report.hatswitch  = 0b00001111;
	data.ps3report.joystick_axes = 0b01010101;
	data.ps3report.analogue_buttons[0] =
	data.ps3report.analogue_buttons[1] =
	data.ps3report.analogue_buttons[2] =
	data.ps3report.analogue_buttons[3] =
	data.ps3report.analogue_buttons[4] =
	data.ps3report.analogue_buttons[5] =
	data.ps3report.analogue_buttons[6] =
	data.ps3report.analogue_buttons[7] =
	data.ps3report.analogue_buttons[8] =
	data.ps3report.analogue_buttons[9] =
	data.ps3report.analogue_buttons[10] =
	data.ps3report.analogue_buttons[11] = 0;
}

#define PS3_SQUARE		{ data.ps3report.buttons[0] |= (1<<0); data.ps3report.analogue_buttons[7] = 0xFF; }
#define PS3_CROSS		{ data.ps3report.buttons[0] |= (1<<1); data.ps3report.analogue_buttons[6] = 0xFF; }
#define PS3_CIRCLE		{ data.ps3report.buttons[0] |= (1<<2); data.ps3report.analogue_buttons[5] = 0xFF; }
#define PS3_TRIANGLE	{ data.ps3report.buttons[0] |= (1<<3); data.ps3report.analogue_buttons[4] = 0xFF; }
#define PS3_L1			{ data.ps3report.buttons[0] |= (1<<4); data.ps3report.analogue_buttons[8] = 0xFF; }
#define PS3_R1			{ data.ps3report.buttons[0] |= (1<<5); data.ps3report.analogue_buttons[9] = 0xFF; }
#define PS3_L2			{ data.ps3report.buttons[0] |= (1<<6); data.ps3report.analogue_buttons[10] = 0xFF; }
#define PS3_R2			{ data.ps3report.buttons[0] |= (1<<7); data.ps3report.analogue_buttons[11] = 0xFF; }

#define PS3_SELECT		{ data.ps3report.buttons[1] |= (1<<0); }
#define PS3_START		{ data.ps3report.buttons[1] |= (1<<1); }
#define PS3_L3			{ data.ps3report.buttons[1] |= (1<<2); }
#define PS3_R3			{ data.ps3report.buttons[1] |= (1<<3); }
#define PS3_PS			{ data.ps3report.buttons[1] |= (1<<4); }

#define PS3_DPAD_UP			{ data.ps3report.hatswitch = 0; data.ps3report.analogue_buttons[2] = 0xFF; }
#define PS3_DPAD_UP_RIGHT	{ data.ps3report.hatswitch = 1; data.ps3report.analogue_buttons[2] = 0xFF; data.ps3report.analogue_buttons[0] = 0xFF; }
#define PS3_DPAD_RIGHT		{ data.ps3report.hatswitch = 2; 								   data.ps3report.analogue_buttons[0] = 0xFF; }
#define PS3_DPAD_DOWN_RIGHT	{ data.ps3report.hatswitch = 3; data.ps3report.analogue_buttons[3] = 0xFF; data.ps3report.analogue_buttons[0] = 0xFF; }
#define PS3_DPAD_DOWN		{ data.ps3report.hatswitch = 4; data.ps3report.analogue_buttons[3] = 0xFF; }
#define PS3_DPAD_DOWN_LEFT	{ data.ps3report.hatswitch = 5; data.ps3report.analogue_buttons[3] = 0xFF; data.ps3report.analogue_buttons[1] = 0xFF; }
#define PS3_DPAD_LEFT		{ data.ps3report.hatswitch = 6; 								   data.ps3report.analogue_buttons[1] = 0xFF; }
#define PS3_DPAD_UP_LEFT	{ data.ps3report.hatswitch = 7; data.ps3report.analogue_buttons[2] = 0xFF; data.ps3report.analogue_buttons[1] = 0xFF; }

#define PS3_LS_LEFT		{ data.ps3report.joystick_axes &= 0b11111100; }
#define PS3_LS_RIGHT	{ data.ps3report.joystick_axes |= 0b00000010; data.ps3report.joystick_axes &= 0b11111110; }

#define PS3_LS_UP		{ data.ps3report.joystick_axes &= 0b11110011; }
#define PS3_LS_DOWN		{ data.ps3report.joystick_axes |= 0b00001000; data.ps3report.joystick_axes &= 0b11111011; }

#define PS3_RS_LEFT		{ data.ps3report.joystick_axes &= 0b11001111; }
#define PS3_RS_RIGHT	{ data.ps3report.joystick_axes |= 0b00100000; data.ps3report.joystick_axes &= 0b11101111; }

#define PS3_RS_UP		{ data.ps3report.joystick_axes &= 0b00111111; }
#define PS3_RS_DOWN		{ data.ps3report.joystick_axes |= 0b10000000; data.ps3report.joystick_axes &= 0b10111111; }

void readInputPS3() {
	resetPS3ReportBuffer();
	updateStickState();	

	// Digital Pad Directions
	if(STICK_STATE_SIGNAL(stickState, STICK_STATE_UP)) {
		if(STICK_STATE_SIGNAL(stickState, STICK_STATE_LEFT)) {
			PS3_DPAD_UP_LEFT
		}
		else if(STICK_STATE_SIGNAL(stickState, STICK_STATE_RIGHT)) {
			PS3_DPAD_UP_RIGHT
		}
		else {
			PS3_DPAD_UP
		}
	}
	else if(STICK_STATE_SIGNAL(stickState, STICK_STATE_DOWN)) {
		if(STICK_STATE_SIGNAL(stickState, STICK_STATE_LEFT)) {
			PS3_DPAD_DOWN_LEFT
		}
		else if(STICK_STATE_SIGNAL(stickState, STICK_STATE_RIGHT)) {
			PS3_DPAD_DOWN_RIGHT
		}
		else {
			PS3_DPAD_DOWN
		}
	}
	else if(STICK_STATE_SIGNAL(stickState, STICK_STATE_LEFT)) {
		PS3_DPAD_LEFT
	}
	else if(STICK_STATE_SIGNAL(stickState, STICK_STATE_RIGHT)) {
		PS3_DPAD_RIGHT
	}

	// Buttons
	if(!Stick_LP)
		PS3_SQUARE

	if(!Stick_MP)
		PS3_TRIANGLE

	if(!Stick_HP) {
		PS3_R1
	}

	if(!Stick_LK) {
		PS3_CROSS
	}

	if(!Stick_MK) {
		PS3_CIRCLE
	}

	if(!Stick_HK) {
		PS3_R2
	}

#ifdef EXTRA_BUTTONS					
	if(!Stick_4P)
		PS3_L1

	if(!Stick_4K)
		PS3_L2
#endif

	if(CFG_X3_READ) {
		if(!Stick_S3)
			PS3_L3

		if(!Stick_S4)
			PS3_R3
	}

	if(!Stick_Start)
		PS3_START

	if(!Stick_Select)
		PS3_SELECT

	if(!Stick_Home)
		PS3_PS
}

/* ------------------------------------------------------------------------- */

void ps3_init_controller() {
	usbMode = USB_MODE_PS3;
	resetPS3ReportBuffer();
	setupUSB();
}

void ps3_controller() {
    while(1) { /* main event loop */
		usbPoll();
		updateJoystickMode();
        readInputPS3();
		usbSetInterrupt3(data.array, 16);

		while(!usbInterruptIsReady3())
			usbPoll();
    }
}
