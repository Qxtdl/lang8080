    equ SERIAL_DATA 0
    equ SERIAL_STATUS 5
    equ RXRDY 32
    equ TXRDY 1
.loop
    IN SERIAL_STATUS
    CPI RXRDY
    JZ rxrdy
    JMP loop
.rxrdy
    IN SERIAL_DATA
    MOV b a
.txrdy 
    IN SERIAL_STATUS
    CPI TXRDY
    JNZ txrdy
    MOV a b
    OUT SERIAL_DATA
    JMP loop
    end