    equ PORT 0
    org 0
.start
    LXI sp 32767;unnesecary
    LXI hl MSG
.loop
    MOV a m
    CPI 0
    JZ done
    OUT PORT
    INX hl
    JMP loop

.done JMP done

.MSG
    DB 72 DB 69 DB 76 DB 76
    DB 79 DB 32 DB 87 DB 79
    DB 82 DB 76 DB 68 DB 0
    end
;test