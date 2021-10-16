#include <stdio.h>
#include <stdint.h>

#define OP_HALT 0x00
#define OP_LD   0x01
#define OP_ST   0x02
#define OP_ALU  0x03
#define OP_ADD  0x00
#define OP_SUB  0x01
#define OP_AND  0x02
#define OP_OR   0x03
#define OP_XOR  0x04
#define OP_ADDS 0x05
#define OP_SUBS 0x06
#define OP_ANDS 0x07
#define OP_SL   0x04
#define OP_SR   0x05
#define OP_ADDI 0x06
#define OP_SUBI 0x07
#define OP_CBZ  0x08
#define OP_CBNZ 0x09
#define OP_BZ   0x0A
#define OP_BN   0x0B
#define OP_BR   0x0C
#define OP_BL   0x0D
#define OP_RET  0x0E

#define W0  0x00
#define W1  0x01
#define W2  0x02
#define W3  0x03
#define W4  0x04
#define W5  0x05
#define W6  0x06
#define W7  0x07
#define W8  0x08
#define W9  0x09
#define W10 0x0A
#define W11 0x0B
#define W12 0x0C
#define W13 0x0D
#define W14 0x0E
#define W15 0x0F
#define W16 0x10
#define W17 0x11
#define W18 0x12
#define W19 0x13
#define W20 0x14
#define W21 0x15
#define W22 0x16
#define W23 0x17
#define W24 0x18
#define W25 0x19
#define W26 0x1A
#define W27 0x1B
#define W28 0x1C
#define W29 0x1D
#define W30 0x1E
#define W31 0x1F

signed char W[32];

#define LDW(Wa, address, displacement)      (OP_LD << 28)   |  (address << 10)  | (displacement << 5) | Wa
#define STW(Wa, address, displacement)      (OP_ST << 28)   |  (address << 10)  | (displacement << 5) | Wa
#define ADD(Wa, Wb, Wc)                     (OP_ALU << 28)  |  (OP_ADD << 25)   | (Wc << 20)          | (Wb << 5) | Wa
#define SUB(Wa, Wb, Wc)                     (OP_ALU << 28)  |  (OP_SUB << 25)   | (Wc << 20)          | (Wb << 5) | Wa
#define AND(Wa, Wb, Wc)                     (OP_ALU << 28)  |  (OP_AND << 25)   | (Wc << 20)          | (Wb << 5) | Wa
#define OR(Wa, Wb, Wc)                      (OP_ALU << 28)  |  (OP_OR << 25)    | (Wc << 20)          | (Wb << 5) | Wa
#define XOR(Wa, Wb, Wc)                     (OP_ALU << 28)  |  (OP_XOR << 25)   | (Wc << 20)          | (Wb << 5) | Wa
#define ADDS(Wa, Wb, Wc)                    (OP_ALU << 28)  |  (OP_ADDS << 25)  | (Wc << 20)          | (Wb << 5) | Wa
#define SUBS(Wa, Wb, Wc)                    (OP_ALU << 28)  |  (OP_SUBS << 25)  | (Wc << 20)          | (Wb << 5) | Wa
#define ANDS(Wa, Wb, Wc)                    (OP_ALU << 28)  |  (OP_ANDS << 25)  | (Wc << 20)          | (Wb << 5) | Wa
#define SL(Wa, Wb, ShiftAmt)                (OP_SL << 28)   |  (ShiftAmt << 10) | (Wb << 5)           | Wa
#define SR(Wa, Wb, ShiftAmt)                (OP_SR << 28)   |  (ShiftAmt << 10) | (Wb << 5)           | Wa
#define ADDI(Wa, Wb, Imm)                   (OP_ADDI << 28) |  (Imm << 10)      | (Wb << 5)           | Wa
#define SUBI(Wa, Wb, Imm)                   (OP_SUBI << 28) |  (Imm << 10)      | (Wb << 5)           | Wa
#define BR(address)                         (OP_BR << 28)   |  (address)
#define BL(address)                         (OP_BL << 28)   |  (address)
#define BZ(address)                         (OP_BZ << 28)   |  (address << 5)
#define BN(address)                         (OP_BN << 28)   |  (address << 5)
#define CBZ(Wa, address)                    (OP_CBZ << 28)  |  (address << 5)   | Wa
#define CBNZ(Wa, address)                   (OP_CBNZ << 28) |  (address << 5)   | Wa
#define RET()                               (OP_RET << 28)
#define HALT()                              (OP_HALT << 28)

char * Instr[] = {
  "Halt",
  "Load",
  "Store",
  "ALU",
  "SL",
  "SR",
  "ADDI",
  "SUBI",
  "CBZ",
  "CBNZ",
  "BZ",
  "BN",
  "BR",
  "BL",
  "RET"
};

char * ALUInstr[] = {
  "ADD",
  "SUB",
  "AND",
  "OR",
  "XOR",
  "ADDS",
  "SUBS",
  "ANDS"
};

/**The tasks are in the report.*/
uint32_t IM[256] = {
};

int32_t DM[64] = {5, 3, 3, 9, 5, 10, 20};

uint32_t PC = 0;
unsigned short Z = 0;
unsigned short N = 0;
unsigned short sRun = 1;

int main();
uint32_t InstructionDecodeUnit(uint32_t uInstruction, uint32_t *ALUOpCode, uint32_t *Wa, uint32_t *Wb, uint32_t *Wc);
void incrPC();
void load(uint32_t Wa, uint32_t address, uint32_t displacement);
void store(uint32_t Wa, uint32_t address, uint32_t displacement);
void add(uint32_t Wa, uint32_t Wb, uint32_t Wc);
void sub(uint32_t Wa, uint32_t Wb, uint32_t Wc);
void and(uint32_t Wa, uint32_t Wb, uint32_t Wc);
void or(uint32_t Wa, uint32_t Wb, uint32_t Wc);
void xor(uint32_t Wa, uint32_t Wb, uint32_t Wc);
void adds(uint32_t Wa, uint32_t Wb, uint32_t Wc);
void subs(uint32_t Wa, uint32_t Wb, uint32_t Wc);
void ands(uint32_t Wa, uint32_t Wb, uint32_t Wc);
void sl(uint32_t Wa, uint32_t Wb, uint32_t shiftAmt);
void sr(uint32_t Wa, uint32_t Wb, uint32_t shiftAmt);
void addi(uint32_t Wa, uint32_t Wb, uint32_t imm);
void subi(uint32_t Wa, uint32_t Wb, uint32_t imm);
void br(uint32_t address);
void bl(uint32_t address);
void bz(uint32_t address);
void bn(uint32_t address);
void ret();
void cbz(uint32_t Wa, uint32_t address);
void cbnz(uint32_t Wa, uint32_t address);
void halt();

int main() {
    uint32_t Wa = 0, Wb = 0, Wc = 0, uOpCode = 0, ALUOpcode = 0;
    printf("The ultimate Pinky processor.\n");
    printf("----------------------------\n");
    while (sRun) {
        printf("\nCurrent Program Counter: %d\n", PC);
        uOpCode = InstructionDecodeUnit(IM[PC], & ALUOpcode, & Wa, & Wb, & Wc);
        printf("Instruction: %0X\n", IM[PC]);
        if(uOpCode == OP_ALU){
            printf("\nInstruction Being Executed: %s\n", ALUInstr[ALUOpcode]);
        } else {
            printf("\nInstruction Being Executed: %s\n", Instr[uOpCode]);
        }
        printf("Wa = %d ---- Wb = %d ---- Wc = %d\n", Wa, Wb, Wc);
        printf("N: %d ---- Z: %d\n", N, Z);
        switch (uOpCode) {
            case OP_LD:
                load(Wa, Wc, Wb);
                break;
            case OP_ST:
                store(Wa, Wc, Wb);
                break;
            case OP_ALU:
                switch (ALUOpcode) {
                    case OP_ADD:
                        add(Wa, Wb, Wc);
                        break;
                    case OP_SUB:
                        sub(Wa, Wb, Wc);
                        break;
                    case OP_AND:
                        and(Wa, Wb, Wc);
                        break;
                    case OP_XOR:
                        xor(Wa, Wb, Wc);
                        break;
                    case OP_OR:
                        or(Wa, Wb, Wc);
                        break;
                    case OP_ADDS:
                        adds(Wa, Wb, Wc);
                        break;
                    case OP_SUBS:
                        subs(Wa, Wb, Wc);
                        break;
                    case OP_ANDS:
                        ands(Wa, Wb, Wc);
                        break;
                }
                break;
            case OP_SL:
                sl(Wa, Wb, Wc);
                break;
            case OP_SR:
                sr(Wa, Wb, Wc);
                break;
            case OP_ADDI:
                addi(Wa, Wb, Wc);
                break;
            case OP_SUBI:
                subi(Wa, Wb, Wc);
                break;
            case OP_BR:
                br(Wc);
                break;
            case OP_BL:
                bl(Wc);
                break;
            case OP_BZ:
                bz(Wc);
                break;
            case OP_BN:
                bn(Wc);
                break;
            case OP_CBZ:
                cbz(Wa, Wc);
                break;
            case OP_CBNZ:
                cbnz(Wa, Wc);
                break;
            case OP_RET:
                ret();
                break;
            case OP_HALT:
                halt();
                break;
            default:
                halt();
                break;
        }
        printf("\nW[0]: %02d\tW[1]: %02d\tW[2]: %02d\tW[3]: %02d\nW[4]: %02d\tW[5]: %02d\tW[6]: %02d\tW[7]: %02d\n\n",
      W[0], W[1], W[2], W[3], W[4], W[5], W[6], W[7]);
    }
    return (0);
}

uint32_t InstructionDecodeUnit(uint32_t uInstruction, uint32_t *ALUOpCode, uint32_t *Wa, uint32_t *Wb, uint32_t *Wc) {

    uint32_t uOpCode = (uInstruction >> 28) & (0xF);
    switch (uOpCode) {
        case OP_LD:
        case OP_ST:
            * Wa = (uInstruction & 0x1F);
            * Wc = (uInstruction >> 10) & 0x3FFFF;
            * Wb = (uInstruction >> 5) & 0x1F;
            break;
        case OP_ALU:
            * ALUOpCode = (uInstruction >> 25) & 0x7;
            * Wa = (uInstruction) & 0x1F;
            * Wb = (uInstruction >> 5) & 0x1F;
            * Wc = (uInstruction >> 20) & 0x1F;
            break;
        case OP_ADDI:
        case OP_SUBI:
            * Wa = (uInstruction) & 0x1F;
            * Wb = (uInstruction >> 5) & 0x1F;
            * Wc = (uInstruction >> 10) & 0x3FFF;
            break;
        case OP_SL:
        case OP_SR:
            * Wa = (uInstruction) & 0x1F;
            * Wb = (uInstruction >> 5) & 0x1F;
            * Wc = (uInstruction >> 10) & 0x3FF;
            break;
        case OP_BR:
        case OP_BL:
            * Wc = (uInstruction) & 0xFFFFFFF;
            break;
        case OP_CBZ:
        case OP_CBNZ:
        case OP_BZ:
        case OP_BN:
            * Wc = (uInstruction >> 5) & 0x7FFFFF;
            * Wa = (uInstruction) & 0x1F;
            break;
        case OP_HALT:
            break;
        default:
            break;
  }
  return (uOpCode);
}

void incrPC() {
    PC = PC + 1;
}

void load(uint32_t Wa, uint32_t address, uint32_t displacement) {
    W[Wa] = DM[address + displacement];
    incrPC();
}

void store(uint32_t Wa, uint32_t address, uint32_t displacement) {
    printf("DM[%d] = %d\n", address + displacement, W[Wa]);
    DM[address + displacement] = W[Wa];
    incrPC();
}

void add(uint32_t Wa, uint32_t Wb, uint32_t Wc) {
    W[Wa] = W[Wb] + W[Wc];
    incrPC();
}

void sub(uint32_t Wa, uint32_t Wb, uint32_t Wc) {
    W[Wa] = W[Wb] - W[Wc];
    incrPC();
}

void and(uint32_t Wa, uint32_t Wb, uint32_t Wc) {
    W[Wa] = W[Wb] & W[Wc];
    incrPC();
}

void or(uint32_t Wa, uint32_t Wb, uint32_t Wc) {
    W[Wa] = W[Wb] | W[Wc];
    incrPC();
}

void xor(uint32_t Wa, uint32_t Wb, uint32_t Wc) {
    W[Wa] = W[Wb] ^ W[Wc];
    incrPC();
}

void adds(uint32_t Wa, uint32_t Wb, uint32_t Wc) {
    W[Wa] = W[Wb] + W[Wc];
    Z = (W[Wa] == 0) ? 1 : 0;
    N = (W[Wa] < 0) ? 1 : 0;
    incrPC();
}

void subs(uint32_t Wa, uint32_t Wb, uint32_t Wc) {
    W[Wa] = W[Wb] - W[Wc];
    Z = (W[Wa] == 0) ? 1 : 0;
    N = (W[Wa] < 0) ? 1 : 0;
    incrPC();
}

void ands(uint32_t Wa, uint32_t Wb, uint32_t Wc) {
    W[Wa] = W[Wb] & W[Wc];
    Z = (W[Wa] == 0) ? 1 : 0;
    N = (W[Wa] < 0) ? 1 : 0;
    incrPC();
}

void sl(uint32_t Wa, uint32_t Wb, uint32_t shiftAmt) {
    W[Wa] = W[Wb] << shiftAmt;
    incrPC();
}

void sr(uint32_t Wa, uint32_t Wb, uint32_t shiftAmt) {
    W[Wa] = W[Wb] >> shiftAmt;
    incrPC();
}

void addi(uint32_t Wa, uint32_t Wb, uint32_t imm) {
    W[Wa] = W[Wb] + imm;
    incrPC();
}

void subi(uint32_t Wa, uint32_t Wb, uint32_t imm) {
    W[Wa] = W[Wb] - imm;
    incrPC();
}

void br(uint32_t address) {
    PC = address;
}

void bl(uint32_t address) {
    W[W30] = PC + 1;
    PC = address;
}

void bz(uint32_t address) {
    if (Z == 1) {
        PC = address;
    } else {
        incrPC();
    }
}

void bn(uint32_t address) {
    if (N == 1) {
        PC = address;
    } else {
        incrPC();
    }
}

void ret() {
    PC = W[W30];
}

void cbz(uint32_t Wa, uint32_t address) {
    if (W[Wa] == 0) {
        PC = address;
    } else {
        incrPC();
    }
}

void cbnz(uint32_t Wa, uint32_t address) {
    if (W[Wa] != 0) {
        PC = address;
    } else {
        incrPC();
    }
}

void halt() {
    printf("Halt executed, program normally terminated. ");
    sRun = 0;
}
