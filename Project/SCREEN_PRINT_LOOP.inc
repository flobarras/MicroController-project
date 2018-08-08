/*
 * SCREEN_PRINT_LOOP.inc
 * Used to print different things on the screen, based on the bytes stored in program memory
 * These bytes are loaded using LPM instruction, which loads one byte pointed to by the Z-register 
 * into the chosen destination register
 */ 
 

SCREEN_PRINT_LOOP:
	LDI R17,0b01000000			; Position of the row ON
	PrintScreenLoop:			; Loop on the rows
		RCALL ScreenLoop
		LSR R17 
		BRNE PrintScreenLoop
	RET



ScreenLoop:						; Turn on one row
	PUSH R26
	PUSH R27
	PUSH R16	
	LDI R26,0x0A				
	Loop_on_columns:			; Loop on the 10 bytes pointed by Z
		LPM R27,Z+				; Loads one byte pointed to by the Z-register into the destination register Rd. Z is then incremented
		LDI R16,0x08
		CALL on_or_off 			; Loop on the byte stored in R27  ("Turn on" (bit = 1) or "turn off" (bit = 0) the columns) 
		DEC R26
		BRNE Loop_on_columns
	LDI R16,0x08
	MOV R27,R17
	CALL on_or_off				; "Turn on" one row, "turn off" the others
	SBI PORTB,4 
	RCALL pause 
	CBI PORTB,4 
	POP R16
	POP R27
	POP R26
	RET



pause1:
	LDI R26,0xFF
	loop_pause1:
		DEC R26
		LDI R27,0xFF
		loop_pause2:
			DEC R27
			LDI R25,0xFF
			loop_pause3:
				DEC R25
				BRNE loop_pause3
			BRNE loop_pause2	
		BRNE loop_pause1
	RET 

pause:
	RCALL pause1
	RCALL pause1
	RET


on_or_off:
	CBI PORTB,3
	SBRC R27,7 				; Skip next instruction if MSB of R27 is cleared  (turn on if bit = 1)
	SBI PORTB,3
	SBI PORTB,5
	CBI PORTB,5
	LSL R27				
	DEC R16
	BRNE on_or_off
	RET