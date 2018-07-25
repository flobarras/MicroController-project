/*
 * RANDOM_NUM.inc
 * Generate a random number between 1 and 4. Linear congruential algorithm
 */ 


 
RANDOM_NUM:
	; 4 lines that are lighted up in random order

	; To do this, we first generate a one-byte pseudo-random number (so that period is high enough to have a 'good' pseudo-random number
	; Algorithm implemented: Linear-Congruential Generator: x = (a * x_prev + b) modulo m   , m = 128

	LDI R26,0x05				; a = 5 (for generation of random numbers)

	MUL R29,R26
	MOVW R16,R0				; R16: low bits / R17: high bits
	LDI R26,0x03			; b = 3
	ADD R16,R26


	MOV R29,R16			; update

	; R29 contains a pseudo-random byte. To generate a pseudo-random number between 1 and 4, we keep only 2 bits of R29 and we add 1

	MOV R20,R16
	CBR R20,$E0		
	CBR R20,$07	
	LSR R20
	LSR R20
	LSR R20	
	INC R20

	RET
