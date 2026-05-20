#improper code
.alabel      
MOV a b
XTHL
OUT c hl
CALL hl de a b hl de a b c d e alabel
RST MOV a b
CPO
MOV a
