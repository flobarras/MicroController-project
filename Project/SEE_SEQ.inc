/*
 * SEE_SEQ.inc
 * One random part of the screen is lit up. This action is repeated to form a sequence of a certain length
 */ 

SEE_SEQ:

	show_seq:
		CALL RANDOM_NUM		; Give a random number stored in R20 to know which part of the board must be lit up
		LDI R26,0x01
		CPSE R20,R26			
		RJMP next1
		RJMP First_quart

		next1:
			LDI R26,0x02
			CPSE R20,R26
			RJMP next2
			RJMP Second_quart
		next2:
			LDI R26,0x03
			CPSE R20,R26
			RJMP Fourth_quart
			RJMP Third_quart
		
		First_quart:	
			CALL FIRST_PART
			RJMP cont

		Second_quart:
			CALL SECOND_PART
			RJMP cont

		Third_quart:
			CALL THIRD_PART
			RJMP cont

		Fourth_quart:
			CALL FOURTH_PART
			RJMP cont

		cont:
			; Delay depending on speed (controlled via R19, incremented every 0.5s)
			LDI R26,0x5F 
			SUB R26,R19  
			LDI R27,0x03
			LDI R28,0x10
			CALL Delay				

			; 'Turn off' screen between each 'member' of the sequence
			CALL TURN_OFF
			LDI R26,0x20
			LDI R27,0xFF
			LDI R28,0xFF
			CALL Delay

			; check if sequence is over or not
			LDI R26,0x01
			CPSE R26,R30			
			RJMP continue_seq
			RJMP stop_seq
	
	continue_seq:
		DEC R30
		RJMP show_seq

	stop_seq:
		MOV R29,R22			; Seed for generation of random numbers
		MOV R30,R31	
		LDI R24,0x00
		RET