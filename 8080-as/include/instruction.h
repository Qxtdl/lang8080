#pragma once

#include <stdint.h>

typedef struct {
    const char *name;
    enum RegType {
        RT_RP, // register pair
        RT_STACK_RP,
        RT_REGISTER
    } type;
    int id;
} reg_t;

#define reg(name, regtype, id) \
    { \
        #name, \
        regtype, \
        id \
    },

static const reg_t regs[] = {
    reg(b, RT_REGISTER, 0)
    reg(c, RT_REGISTER, 1)
    reg(d, RT_REGISTER, 2)
    reg(e, RT_REGISTER, 3)
    reg(h, RT_REGISTER, 4)
    reg(l, RT_REGISTER, 5)
    reg(m, RT_REGISTER, 6)
    reg(a, RT_REGISTER, 7)
    reg(bc, RT_RP, 0)
    reg(de, RT_RP, 1)
    reg(hl, RT_RP, 2)
    reg(sp, RT_RP, 3)
    reg(psw, RT_STACK_RP, 3)
};

typedef struct {
    const char *name;
    enum Opcode {
        O_NOP =   0b00000000,
        O_LXI =   0b00000001,
        O_STAX =  0b00000010,
        O_INX =   0b00000011,
        O_INR =   0b00000100,
        O_DCR =   0b00000101,
        O_MVI =   0b00000110,
        O_DAD =   0b00001001,
        O_LDAX =  0b00001010,
        O_DCX =   0b00001011,
        O_RLC =   0b00000111,
        O_RRC =   0b00001111,
        O_RAL =   0b00010111,
        O_RAR =   0b00011111,
        O_SHLD =  0b00100010,
        O_DAA =   0b00100111,
        O_LHLD =  0b00101010,
        O_CMA =   0b00101111,
        O_STA =   0b00110010,
        O_STC =   0b00110111,
        O_LDA =   0b00111010,
        O_CMC =   0b00111111,
        O_MOV =   0b01000000,
        O_HLT =   0b01110110,

        O_ADD =   0b10000000,
        O_ADC =   0b10001000,
        O_SUB =   0b10010000,
        O_SBB =   0b10011000,
        O_ANA =   0b10100000,
        O_XRA =   0b10101000,
        O_ORA =   0b10110000,
        O_CMP =   0b10111000,

        O_RNZ =   0b11000000,
        O_RZ =    0b11001000,
        O_RNC =   0b11010000,
        O_RC =    0b11011000,
        O_RPO =   0b11100000,
        O_RPE =   0b11101000,
        O_RP =    0b11110000,
        O_RM =    0b11111000,
        O_POP =   0b11000001,
        O_JC =    0b11011010,
        O_JNC =   0b11010010,
        O_JZ =    0b11001010,
        O_JNZ =   0b11000010,
        O_JM =    0b11111010,
        O_JP =    0b11110010,
        O_JPE =   0b11101010,
        O_JPO =   0b11100010,

        O_JMP =   0b11000011,

        O_CC =    0b11011100,
        O_CNC =   0b11010100,
        O_CZ =    0b11001100,
        O_CNZ =   0b11000100,
        O_CM =    0b11111100,
        O_CP =    0b11110100,
        O_CPE =   0b11101100,
        O_CPO =   0b11100100,

        O_PUSH =  0b11000101,

        O_ADI =   0b11000110,
        O_ACI =   0b11001110,
        O_SUI =   0b11010110,
        O_SBI =   0b11011110,
        O_ANI =   0b11100110,
        O_XRI =   0b11101110,
        O_ORI =   0b11110110,
        O_CPI =   0b11111110,

        O_RST =   0b11000111,
        O_RET =   0b11001001,
        O_CALL =  0b11001101,
        O_OUT =   0b11010011,
        O_IN =    0b11011011,
        O_XTHL =  0b11100011,
        O_PCHL =  0b11101001,
        O_XCHG =  0b11101011,
        O_DI =    0b11110011,
        O_SPHL =  0b11111001,
        O_EI =    0b11111011,

        O_DB = 0xdeadc0de, // define byte
        O_INVALID = 0xdeadbeef
    } op;
    enum OperandType {
        OT_NONE,
        OT_RP,
        OT_STACK_RP,
        OT_DEST_REG,
        OT_DEST_REG_SRC_REG,
        OT_SRC_REG,
        OT_INLINE_IMM
    } op_type;
    enum ExtraBytes {
        EB_NONE,
        EB_BYTE,
        EB_WORD,
        EB_BRANCH = EB_WORD
    } eb;
    int operands_size;
    reg_t operands[2];
    uint8_t byte_imm;
    uint16_t word_imm;
} instruction_t;

#define inst(name, operand_type, extrabytes) \
    { \
        #name, \
        O_##name, \
        operand_type, \
        extrabytes \
    },

static const instruction_t instructions[] = {
    inst(NOP, OT_NONE, EB_NONE)
    inst(LXI, OT_RP, EB_WORD)
    inst(STAX, OT_RP, EB_NONE)
    inst(INX, OT_RP, EB_NONE)
    inst(INR, OT_DEST_REG, EB_NONE)
    inst(DCR, OT_DEST_REG, EB_NONE)
    inst(MVI, OT_DEST_REG, EB_BYTE)
    inst(DAD, OT_RP, EB_NONE)
    inst(LDAX, OT_RP, EB_NONE)
    inst(DCX, OT_RP, EB_NONE)
    inst(RLC, OT_NONE, EB_NONE)
    inst(RRC, OT_NONE, EB_NONE)
    inst(RAL, OT_NONE, EB_NONE)
    inst(RAR, OT_NONE, EB_NONE)
    inst(SHLD, OT_NONE, EB_WORD)
    inst(DAA, OT_NONE, EB_NONE)
    inst(LHLD, OT_NONE, EB_WORD)
    inst(CMA, OT_NONE, EB_NONE)
    inst(STA, OT_NONE, EB_WORD)
    inst(STC, OT_NONE, EB_NONE)
    inst(LDA, OT_NONE, EB_WORD)
    inst(CMC, OT_NONE, EB_NONE)
    inst(MOV, OT_DEST_REG_SRC_REG, EB_NONE)
    inst(HLT, OT_NONE, EB_NONE)
    inst(ADD, OT_SRC_REG, EB_NONE)
    inst(ADC, OT_SRC_REG, EB_NONE)
    inst(SUB, OT_SRC_REG, EB_NONE)
    inst(SBB, OT_SRC_REG, EB_NONE)
    inst(ANA, OT_SRC_REG, EB_NONE)
    inst(XRA, OT_SRC_REG, EB_NONE)
    inst(ORA, OT_SRC_REG, EB_NONE)
    inst(CMP, OT_SRC_REG, EB_NONE)
    inst(RNZ, OT_NONE, EB_NONE)
    inst(RZ , OT_NONE, EB_NONE)
    inst(RNC, OT_NONE, EB_NONE)
    inst(RC , OT_NONE, EB_NONE)
    inst(RPO, OT_NONE, EB_NONE)
    inst(RPE, OT_NONE, EB_NONE)
    inst(RP , OT_NONE, EB_NONE)
    inst(RM , OT_NONE, EB_NONE)
    inst(POP, OT_RP, EB_NONE)
    inst(JC , OT_NONE, EB_BRANCH)
    inst(JNC, OT_NONE, EB_BRANCH)
    inst(JZ , OT_NONE, EB_BRANCH)
    inst(JNZ, OT_NONE, EB_BRANCH)
    inst(JM , OT_NONE, EB_BRANCH)
    inst(JP , OT_NONE, EB_BRANCH)
    inst(JPE, OT_NONE, EB_BRANCH)
    inst(JPO, OT_NONE, EB_BRANCH)
    inst(JMP, OT_NONE, EB_BRANCH) 
    inst(CC, OT_NONE, EB_BRANCH)
    inst(CNC, OT_NONE, EB_BRANCH)
    inst(CZ, OT_NONE, EB_BRANCH)
    inst(CNZ, OT_NONE, EB_BRANCH)
    inst(CM, OT_NONE, EB_BRANCH)
    inst(CP, OT_NONE, EB_BRANCH)
    inst(CPE, OT_NONE, EB_BRANCH)
    inst(CPO, OT_NONE, EB_BRANCH)
    inst(PUSH, OT_RP, EB_NONE)
    inst(ADI, OT_SRC_REG, EB_BYTE)
    inst(ACI, OT_SRC_REG, EB_BYTE)
    inst(SUI, OT_SRC_REG, EB_BYTE)
    inst(SBI, OT_SRC_REG, EB_BYTE)
    inst(ANI, OT_SRC_REG, EB_BYTE)
    inst(XRI, OT_SRC_REG, EB_BYTE)
    inst(ORI, OT_SRC_REG, EB_BYTE)
    inst(CPI, OT_SRC_REG, EB_BYTE)
    inst(RST, OT_INLINE_IMM, EB_NONE)
    inst(RET, OT_NONE, EB_NONE)
    inst(CALL, OT_NONE, EB_BRANCH)
    inst(OUT, OT_NONE, EB_BYTE)
    inst(IN, OT_NONE, EB_BYTE)
    inst(XTHL, OT_NONE, EB_NONE)
    inst(PCHL, OT_NONE, EB_NONE)
    inst(XCHG, OT_NONE, EB_NONE)
    inst(DI, OT_NONE, EB_NONE)
    inst(SPHL, OT_NONE, EB_NONE)
    inst(EI, OT_NONE, EB_NONE)
    inst(DB, OT_NONE, EB_BYTE)
};
