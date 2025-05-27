INSTR_OPCODE_OFFSET = 0
INSTR_RS1_OFFSET = INSTR_OPCODE_OFFSET + 8
INSTR_RS2_OFFSET = INSTR_RS1_OFFSET + 5
INSTR_RD_OFFSET = INSTR_RS2_OFFSET + 5
INSTR_IMM_OFFSET = INSTR_RD_OFFSET + 6

INSTR_OPCODE_MASK = 0xFF << INSTR_OPCODE_OFFSET
INSTR_RS1_MASK = 0x1F << INSTR_RS1_OFFSET
INSTR_RS2_MASK = 0x1F << INSTR_RS2_OFFSET
INSTR_RD_MASK = 0x1F << INSTR_RD_OFFSET
INSTR_IMM_MASK = 0xFF << INSTR_IMM_OFFSET

INSTR_LD = 0b00000010
INSTR_SD = 0b00000011
INSTR_ADD = 0b00001000
INSTR_HLT = 0b00000000



def encode_imm(imm: int) -> int:
    if not (-256 <= imm <= 255):
        raise ValueError("Imm out of range (-256..255)")
    imm_encoded = (imm + 0x80) & 0xFF
    return imm_encoded

def build_instr(opcode: int, rs1=0, rs2=0, rd=0, imm=0) -> int:
    val = 0
    val |= (opcode & 0xFF) << INSTR_OPCODE_OFFSET
    val |= (rs1 & 0x1F) << INSTR_RS1_OFFSET
    val |= (rs2 & 0x1F) << INSTR_RS2_OFFSET
    val |= (rd & 0x1F) << INSTR_RD_OFFSET
    val |= (imm & 0x1FF) << INSTR_IMM_OFFSET
    return val

def VALUE(val: int) -> bytes:
    return val

def LD(imm: int, rd: int, rs1=0) -> bytes:
    imm_enc = encode_imm(imm)
    return build_instr(INSTR_LD, rs1=rs1, rd=rd, imm=imm_enc)

def SD(imm: int, rs2: int, rs1=0) -> bytes:
    imm_enc = encode_imm(imm)
    return build_instr(INSTR_SD, rs1=rs1, rs2=rs2, imm=imm_enc)

def ADD(rs1: int, rs2: int, rd: int) -> bytes:
    return build_instr(INSTR_ADD, rs1=rs1, rs2=rs2, rd=rd)

def HLT() -> bytes:
    return build_instr(INSTR_HLT)



program = []
program.append(LD(imm=16, rd=10)) # 0x00
program.append(LD(imm=16, rd=11)) # 0x04
program.append(ADD(rs1=10, rs2=11, rd=12)) # 0x08
program.append(SD(imm=12, rs2=12)) # 0x0C
program.append(HLT()) # 0x10
program.append(VALUE(0x01234567)) # 0x14
program.append(VALUE(0x11111111)) # 0x18
program.append(VALUE(0xC0FFEE00)) # 0x1C

f = open('code.bin', 'wb')

for i, instr in enumerate(program):
    print(f"{i}: 0x{str(hex(instr + 0x100000000))[3:]}")
    f.write(instr.to_bytes(4, byteorder='little'))

