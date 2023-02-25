.equ DRDY = 2
.equ DOUT = 1
.equ DCLK = 0

delay:
	push r24
	push r25
    ; ldi r18, 0x01
    ldi r24, 0x4f
    ldi r25, 0xff

inner_loop:
    ; subi r18, 0x01 ; Substract 1
    subi r24, 0x01 ; Substract c (1) if previous sub overflowed
    sbci r25, 0x00 ; Substract c (1) if previous sub overflowed
    brne inner_loop ; branch not equal (in this case if != 0)
	pop r25
	pop r24
    ret

transmit:
	push r18
	push r22

	ldi r18, 8

transmit_loop:
	rol r22
	brcs transmit_high
	cbi 0x18, DOUT
	rjmp transmit_clk
transmit_high:
	sbi 0x18, DOUT

transmit_clk:
	sbi 0x18, DCLK
	cbi 0x18, DCLK

	dec r18
	brne transmit_loop

	sbi 0x18, DRDY
	rcall delay
	cbi 0x18, DRDY

	pop r22
	pop r18

	ret

receive:
	rjmp receive

init:
	ldi r16, 0b00000111
	out 0x17, r16
	ldi r16, 0b00000000
	out 0x18, r16

	; ldi r22, 0b10101010
	; rcall transmit

	ret