    equ PORT 0
.start 
    MVI a 65
.loop
    INR a
    PUSH psw
    POP psw
    OUT PORT
    CPI 0
    JNZ loop
.done
    JMP done
    DB 69
    DB 69
    DB 69
    DB 69
    end
