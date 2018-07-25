/*
 * REDO_SEQ.inc
 * The user has to redo the sequence. If he succeeds, the length of the sequence is increased. If he fails, the game ends
 */ 

REDO_SEQ:

	; "Reload" random sequence to check 
	CALL RANDOM_NUM

	waiting:
		LDI R18,0xFE
		; If R24 > 30, ends game (15s of inactivity)
		LDI R26,0x1E
		CPSE R24,R26
		RJMP not_ending					
		RJMP error			;  Branch if R24 >= 30

		not_ending:
		SBIS PIND,0                     ; Skip next instruction if the least significant bit of pin D is set (not pressed).
		RJMP C4Pressed
		SBIS PIND,1
		RJMP C3Pressed
		SBIS PIND,2
		RJMP C2Pressed
		SBIS PIND,3
		RJMP C1Pressed
		RJMP waiting

	C4Pressed:
		RJMP waiting

	C3Pressed:
		RJMP waiting

	C2Pressed:
		RJMP waiting

	C1Pressed:
		
		LDI R18,0xFF
		
		; See p100 Atmega328p datasheet and https://www.avrfreaks.net/forum/explain-it
		; << left bit shift   ,   | bitwise OR
		; To build a bit mask that has bit number n set: (0x01 << n)    (1 is 'left shifted' by n)
		; To build a bit mask that has bits number n and x set: (0x01 << n)|(0x01 << x)
		LDI R16,(1<<7)|(1<<6)|(1<<5)|(1<<4)			; 7 = 1st row, 6 = 2nd row, 5 = 3rd row, 4 = 4th row
		LDI R17,(1<<3)|(1<<2)|(1<<1)|(1<<0)			; 3 = 1st col, 2 = 2nd col, 1 = 3rd col, 0 = 4th col
		OUT PORTD,R16
		OUT DDRD,R17
		NOP
		NOP
		NOP
		NOP										; to be able to read back the value recently assigned to some of the pins


		SBIS PIND,7                     
		RJMP BT7Pressed
		SBIS PIND,6						; Skip next instruction if not pressed
		RJMP BT4Pressed
		SBIS PIND,5
		RJMP BT1Pressed
		SBIS PIND,4
		RJMP BTAPressed
		RJMP reset

		BT7Pressed:
			CALL FIRST_PART
			LDI R26,0x01
			CPSE R20,R26
			RJMP error
			RJMP correct


		BT4Pressed:
			CALL SECOND_PART
			LDI R26,0x02
			CPSE R20,R26
			RJMP error
			RJMP correct	
			

		BT1Pressed:
			CALL THIRD_PART
			LDI R26,0x03
			CPSE R20,R26
			RJMP error
			RJMP correct

		BTAPressed:
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
			RJMP reset


		error:
			LDI R23,0x10			; 130.81 Hz (C3)
			CALL Buzzer_loop
			LDI R26,0x1F 
			LDI R27,0x08
			LDI R28,0x30
			CALL Delay
			RJMP end
		

	reset:
		/*
		; Rows (7,6,5,4) are outputs (bits set)
		; Columns (3,2,1,0) are inputs (bits cleared)
		LDI R26,0xF0
		OUT DDRD,R26

		; Columns : enable pull-up (bits set)
		; Rows are set to LOW (bits cleared)
		LDI R26,0x0F
		OUT PORTD,R26
		*/

		LDI R16,(1<<3)|(1<<2)|(1<<1)|(1<<0)			; 3 = 1st col, 2 = 2nd col, 1 = 3rd col, 0 = 4th col  ; equivalent to LDI R16,0x0F
		LDI R17,(1<<7)|(1<<6)|(1<<5)|(1<<4)			; 7 = 1st row, 6 = 2nd row, 5 = 3rd row, 4 = 4th row
		OUT PORTD,R16
		OUT DDRD,R17
	
	
	

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
		MOV R29,R22					; Seed for generation of random numbers
		MOV R30,R31	

		LDI R26,0x80
		LDI R27,0xFF
		LDI R28,0xFF
		CALL Delay

		RJMP main