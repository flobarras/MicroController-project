/*
 * OTHER_FUNCTIONS.inc
 *  - col_off
 *  - col_on
 *  - row_on
 *	- row_off
 *	- Delay
 *  - Buzzer_loop
 *  - FIRST_PART
 *  - SECOND_PART
 *  - THIRD_PART
 *  - FOURTH_PART
 *  - TURN_OFF
 *  - Delay1
 */ 


 
col_off:
	Columns_loop_off_1:				
		CBI PORTB,3
		SBI PORTB,5
		CBI PORTB,5
		DEC R26
		BRNE Columns_loop_off_1
	RET

col_on:
	Columns_loop_on_1:				
		SBI PORTB,3
		SBI PORTB,5
		CBI PORTB,5
		DEC R26
		BRNE Columns_loop_on_1
	RET
	
row_on:
	Rows_loop_on_1:		
		SBI PORTB,3
		SBI PORTB,5
		CBI PORTB,5
		DEC R26
		BRNE Rows_loop_on_1
	RET

row_off:	
	Rows_loop_off_1:		
		CBI PORTB,3
		SBI PORTB,5
		CBI PORTB,5
		DEC R26
		BRNE Rows_loop_off_1
	RET	



Delay:
	loop1:
		loop2:
			loop3:
				DEC R28
				BRNE loop3
			DEC R27
			BRNE loop2
		DEC R26
		BRNE loop1
	RET


Buzzer_loop:
	LDI R16,0x01
	STS TIMSK0,R16			; Enable interrupt for timer 0
	LDI R16,0x04			; Prescaler 256
	OUT TCCR0B,R16 
	OUT TCNT0,R23
	RET
	

		
FIRST_PART:
	LDI R26,0x3C
	RCALL col_off
	LDI R26,0x14
	RCALL col_on
	LDI R26,0x08
	RCALL row_on
	SBI PORTB,4
	CALL pause
	CBI PORTB,4
	RET

SECOND_PART:
	LDI R26,0x28
	RCALL col_off
	LDI R26,0x14
	RCALL col_on
	LDI R26,0x14
	RCALL col_off
	LDI R26,0x08
	RCALL row_on
	SBI PORTB,4
	CALL pause
	CBI PORTB,4
	RET

THIRD_PART:
	LDI R26,0x14
	RCALL col_off
	LDI R26,0x14
	RCALL col_on
	LDI R26,0x28
	RCALL col_off
	LDI R26,0x08
	RCALL row_on
	SBI PORTB,4
	CALL pause
	CBI PORTB,4
	RET

FOURTH_PART:
	LDI R26,0x14
	RCALL col_on
	LDI R26,0x3C
	RCALL col_off
	LDI R26,0x08
	RCALL row_on
	SBI PORTB,4
	CALL pause
	CBI PORTB,4
	RET

TURN_OFF:
	LDI R26,0x50
	RCALL col_off
	LDI R26,0x08
	RCALL row_off
	SBI PORTB,4
	CALL pause
	CBI PORTB,4
	RET
	


Delay1:
	CALL Buzzer_loop
	LDI R26,0x01 
	LDI R27,0xDF
	loop11:
		loop21:
			LDI R16,0x00
			CPSE R24,R16
			RJMP curr
			RJMP besttt
			curr:
			MOV R21,R29
			MACRO_PRINT_SCORE score_zero,score_one,score_two,score_three,score_four,score_five,score_six,score_seven,score_eight,score_nine
			MOV R21,R22
			RJMP cont_score
			besttt:
			MOV R21,R29
			MACRO_PRINT_SCORE best_zero,best_one,best_two,best_three,best_four,best_five,best_six,best_seven,best_eight,best_nine
			MOV R21,R20
			cont_score:
			MACRO_PRINT_SCORE zero,one,two,three,four,five,six,seven,eight,nine
			SBIC PINB,2
			RJMP NoJoy1
			RJMP Joy
			NoJoy1:
			DEC R27
			LDI R16,0x00
			CPSE R27,R16
			RJMP loop21
		DEC R26
		CPSE R26,R16
		RJMP loop11
	RET

