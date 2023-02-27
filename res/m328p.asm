.equ UBRR0H = 0xc5
.equ UBRR0L = 0xc4

.equ UCSR0A = 0xc0
.equ UCSR0B = 0xc1
.equ UCSR0C = 0xc2

.equ UDR0   = 0xc6

.equ BAUD   = 9600
.equ F_CPU  = 16000000
.equ UBRR_V = (((F_CPU) + 8 * (BAUD)) / (16 * (BAUD)) - 1)

USART0_init:
    push r16
    push r30
    push r31
    
    ldi r16, low(UBRR_V)

    ldi r30, low(UBRR0L) ; set ZL
    ldi r31, high(UBRR0L) ; set ZH
    st Z, r16 ; set low part of baud rate reg

    ldi r16, high(UBRR_V)

    ldi r30, low(UBRR0H) ; set ZL
    ldi r31, high(UBRR0H) ; set ZH
    st Z, r16 ; set high part of baud rate reg


    ldi r16, 0b00011000

    ldi r30, low(UCSR0B) ; set ZL
    ldi r31, high(UCSR0B) ; set ZH
    st Z, r16 ; enable transmitter and receiver

    ldi r16, 0b00001110

    ldi r30, low(UCSR0C) ; set ZL
    ldi r31, high(UCSR0C) ; set ZH
    st Z, r16 ; set frame format: 8data, 2stop bit

    pop r31
    pop r30
    pop r16
    ret

; char to transmit is in r22
USART0_transmit:
    push r16
    push r30
    push r31
    
USART0_transmit_loop:
    ldi r30, low(UCSR0A) ; set ZL
    ldi r31, high(UCSR0A) ; set ZH
    ld r16, Z

    sbrs r16, 5 ; wait for empty transmit buffer
    rjmp USART0_transmit_loop

    ldi r30, low(UDR0) ; set ZL
    ldi r31, high(UDR0) ; set ZH
    st Z, r22 ; put data into buffer, sends the data

    pop r31
    pop r30
    pop r16

    ret

; char is in r22
USART0_receive:
    push r16
    push r30
    push r31

USART0_receive_loop:
    ldi r30, low(UCSR0A) ; set ZL
    ldi r31, high(UCSR0A) ; set ZH
    ld r16, Z

    sbrs r16, 7 ; wait for data to be received
    rjmp USART0_receive_loop

    ldi r30, low(UDR0) ; set ZL
    ldi r31, high(UDR0) ; set ZH
    ld r22, Z ; get and return received data from buffer

    pop r31
    pop r30
    pop r16

    ret
