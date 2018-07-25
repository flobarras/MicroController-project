/*
 * INITIALIZATION.inc
 * Initialization of the I/O
 */ 

INITIALIZATION:

; Joystick
CBI DDRB,2								; Pin PB2 is an input
SBI PORTB,2								; Enable the pull-up resistor

; Buzzer
SBI DDRB,1								; Pin PB1 is an output (buzzer)

; Screen 
SBI DDRB,3
SBI DDRB,4
SBI DDRB,5						
SBI PORTB,3								; If PORTxn is written to '1' when the pin is 
SBI PORTB,4								; configured as an output pin, the port pin is driven high.
SBI PORTB,5


; Keyboard
; Rows (7 - first row,6,5,4) are outputs (bits set)
; Columns (3 - first column,2,1,0) are inputs (bits cleared)
LDI R26,0xF0
OUT DDRD,R26
; Columns : enable pull-up (bits set)
; Rows are set to LOW (bits cleared)
LDI R26,0x0F
OUT PORTD,R26


; Start Timer			
LDI R17,0x04
STS TCCR1B,R17							; Prescaler 256
SEI										; enable global interrupt & timer1 interrupt
LDI R17,0x01
STS TIMSK1,R17
LDI R17,0x00
STS PRTIM1,R17
LDI R16,0x85							; TCNTinit  (0xFFFF - 16e6 / (256 * 2))  --> 2Hz
LDI R17,0xED
STS TCNT1H,R16
STS TCNT1L,R17


RET