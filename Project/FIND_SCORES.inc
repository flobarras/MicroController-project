/*
 * FIND_SCORES.inc
 * Find out the ten and unit of current and best scores, in order to print them
 */ 

; R21 : current score
; R30 : best score

; R29 : ten of the current score
; R22 : unit of the current score
; R19 : ten of the best score
; R20 : unit of the best score


FIND_SCORES:

	LDI R26,0x64
	CP R21,R26					
	BRGE except1			;  Branch if current score >= 100

	; CURRENT SCORE
	LDI R29,0x00
	LDI R18,0x0A
	LDI R27,0x0A
	decaade:
		CP R21,R18
		BRLO numbers
		ADD R18,R27
		INC R29
		BRNE decaade
	numbers:
		SUB R18,R27
		SUB R21,R18
		LDI R22,0x00
		loopnb:
			CPSE R21,R22
			RJMP notthis
			RJMP best1
		notthis:
			INC R22
			BRNE loopnb

	; BEST SCORE
	best1:
		CP R30,R26					
		BRGE except2			;  Branch if best score >= 100
		LDI R19,0x00
		LDI R18,0x0A
		decaade2:
			CP R30,R18
			BRLO numbers2
			ADD R18,R27
			INC R19
			BRNE decaade2
		numbers2:
			SUB R18,R27
			SUB R30,R18
			LDI R20,0x00
			loopnb2:
				CPSE R30,R20
				RJMP notthis2
				RET
			notthis2:
				INC R20
				BRNE loopnb2


	except1:
		LDI R29,0x09
		LDI R22,0x09
		RJMP best1

	except2:
		LDI R19,0x09
		LDI R20,0x09
		RET

