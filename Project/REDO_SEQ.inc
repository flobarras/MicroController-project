/*
 * REDO_SEQ.inc
 * The user has to redo the sequence. If he succeeds, the length of the sequence is increased. If he fails, the game ends
 */ 

REDO_SEQ:

	; "Reload" random sequence to check 
	CALL RANDOM_NUM

	waiting:
		SBI PORTD,7															
		SBI PORTD,6
		SBI PORTD,5
		SBI PORTD,4
		LDI R18,0xFE
		; If R24 > 30, ends game (15s of inactivity)
		LDI R26,0x1E
		CPSE R24,R26
		RJMP not_ending					
		RJMP error		

	not_ending:																	; Keyboard: 4 steps method
		row1_low:													
			CBI PORTD,7															; STEP 1 : Row 1 set to LOW
			SBI PORTD,6
			SBI PORTD,5
			SBI PORTD,4
			KEYBOARD_CHECK_COL BT7Pressed,OtherBut,OtherBut,OtherBut,row2_low
		row2_low:																	
			SBI PORTD,7															; STEP 2 : Row 2 set to LOW
			CBI PORTD,6
			SBI PORTD,5
			SBI PORTD,4
			KEYBOARD_CHECK_COL BT4Pressed,OtherBut,OtherBut,OtherBut,row3_low
		row3_low:
			SBI PORTD,7															; STEP 3 : Row 3 set to LOW
			SBI PORTD,6
			CBI PORTD,5
			SBI PORTD,4
			KEYBOARD_CHECK_COL BT1Pressed,OtherBut,OtherBut,OtherBut,row4_low
		row4_low:
			SBI PORTD,7															; STEP 4 : Row 4 set to LOW
			SBI PORTD,6
			SBI PORTD,5
			CBI PORTD,4
			KEYBOARD_CHECK_COL BTAPressed,OtherBut,OtherBut,OtherBut,waiting

		OtherBut:
			RJMP waiting
		
		BT7Pressed:
			LDI R18,0xFF
			CALL FIRST_PART
			LDI R26,0x01
			CPSE R20,R26
			RJMP error
			RJMP correct


		BT4Pressed:
			LDI R18,0xFF
			CALL SECOND_PART
			LDI R26,0x02
			CPSE R20,R26
			RJMP error
			RJMP correct	
			

		BT1Pressed:
			LDI R18,0xFF
			CALL THIRD_PART
			LDI R26,0x03
			CPSE R20,R26
			RJMP error
			RJMP correct

		BTAPressed:
			LDI R18,0xFF
			CALL FOURTH_PART
			LDI R26,0x04
			CPSE R20,R26
			RJMP error
			RJMP correct
			
		correct:
			LDI R23,0xDB			; A5 (880 Hz) (FF - 16e6 / (256 * 880 * 2)) because complete period is 'buzzer on -> 
			CALL Buzzer_loop		; next time buzzer is on ----> corresponds to 2 overflows of timer 0
			LDI R26,0x0F 
			LDI R27,0x08
			LDI R28,0x30
			CALL Delay
			LDI R25,0x00
			STS TIMSK0,R25			; Disable interrpt for timer 0
			CALL TURN_OFF
			LDI R26,0x20
			LDI R27,0x40
			LDI R28,0x50
			CALL Delay
			RJMP continue_game

		error:
			LDI R23,0x10			; 130.81 Hz (C3)
			CALL Buzzer_loop
			LDI R26,0x1F 
			LDI R27,0x08
			LDI R28,0x30
			CALL Delay
			RJMP end
		

	continue_game:
		LDI R26,0x01
		CPSE R26,R30
		RJMP continue_redo
		RJMP stop_redo
	
	continue_redo:
		DEC R30
		RJMP REDO_SEQ

	stop_redo:
		INC R21							; Increase score
		INC R31							; Increase sequence length
		MOV R29,R22						; Seed for generation of random numbers
		MOV R30,R31	

		LDI R26,0x80
		LDI R27,0xFF
		LDI R28,0xFF
		CALL Delay

		RJMP main 