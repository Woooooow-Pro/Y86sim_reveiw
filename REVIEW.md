# Instruction Set (Y86-simulator) Review[^1]

## Y86 instructions process in defferent stages(SEO)

|Stage|OPq,rA,rB|rrmovq rA,rB|irmovq V,rB|rmmovq rA,D(rB)|mrmovq D(rB),rA|jXX Dest|call Dest|ret|push rA|pop rA|
|-|-|-|-|-|-|-|-|-|-|-|
|**Fetch**|icode:ifun = $\leftarrow$ $M_1$[PC]<br/>rA:rB $\leftarrow$ $M_1$[PC+1]<br/><br/> valP $\leftarrow$ PC + 2|icode:ifun = $\leftarrow$ $M_1$[PC]<br/>rA:rB $\leftarrow$ $M_1$[PC+1]<br/><br/>valP $\leftarrow$ PC + 2|icode:ifun = $\leftarrow$ $M_1$[PC]<br/>rA:rB $\leftarrow$ $M_1$[PC+1]<br/>valC $\leftarrow$ $M_8$[PC+2]<br/>PC $\leftarrow$ PC + 10|icode:ifun = $\leftarrow$ $M_1$[PC]<br/>rA:rB $\leftarrow$ $M_1$[PC+1]<br/>valC $\leftarrow$ $M_8$[PC+2]<br/>PC $\leftarrow$ PC + 10|icode:ifun = $\leftarrow$ $M_1$[PC]<br/>rA:rB $\leftarrow$ $M_1$[PC+1]<br/>valC $\leftarrow$ $M_8$[PC+2]<br/>PC $\leftarrow$ PC + 10|icode:ifun $\leftarrow$ $M_1$[PC]<br/><br/>valC $\leftarrow$ $M_8$[PC+1]<br/>valP $\leftarrow$ PC + 9|icode:ifun $\leftarrow$ $M_1$[PC]<br/><br/>valC $\leftarrow$ $M_8$[PC+1]<br/>valP $\leftarrow$ PC + 9|icode:ifun $\leftarrow$ $M_1$[PC]<br/><br/><br/>valP $\leftarrow$ PC + 9|icode:ifun <- $M_1$[PC]<br/>rA:rB <- $M_1$[PC+1]<br/><br/>valP <- PC+2|icode:ifun <- $M_1$[PC]<br/>rA:rB <- $M_1$[PC+1]<br/><br/>valP <- PC+2|
|**Decode**|valA $\leftarrow$ R[rA]<br/>valB $\leftarrow$ R[rB]|valA $\leftarrow$ R[rA]||valA $\leftarrow$ R[rA]<br/>valB $\leftarrow$ R[rB]|<br/>valB $\leftarrow$ R[rB]||<br/>valB $\leftarrow$ R[%rsp]|valA $\leftarrow$ R[%rsp]<br/>valB $\leftarrow$ R[%rsp]|valA <- R[rA]<br/>valB <- R[%rsp]|valA <- R[%rsp]<br/>valB <- R[%rsp]|
|**Excute**|valE $\leftarrow$ valB OP valA<br/>set CC|valE $\leftarrow$ 0 + valA|valE $\leftarrow$ 0 + valC|valE $\leftarrow$ valB + valC|valE $\leftarrow$ valB + valC|<br/>Cnd $\leftarrow$ Cond(CC, ifun)|valE $\leftarrow$ valB - 8|valE $\leftarrow$ valB + 8|valE <- valB - 8|valE <- valB + 8|
|**Memory**||||$M_8$[valE] $\leftarrow$ valA|valM $\leftarrow$ $M_8$[valE]||$M_8$[valE] $\leftarrow$ valP|valM $\leftarrow$ $M_8$[valA]|valM <- $M_8$[valA]|valM <- $M_8$[valA]|
|**Write back**|<br/>R[rB] $\leftarrow$ valE|<br/>R[rB] $\leftarrow$ valE|<br/>R[rB] $\leftarrow$ valE||R[rA] $\leftarrow$ valM||<br/>R[%rsp] $\leftarrow$ valE|<br/>R[%rsp] $\leftarrow$ valE|R[rA] <- valM<br/>R[%rsp] <- valE|R[rA] <- valM<br/>R[%rsp] <- valE|
|**PC update**|PC $\leftarrow$ valP|PC $\leftarrow$ valP|PC $\leftarrow$ valP|PC $\leftarrow$ valP|PC $\leftarrow$ valP|PC $\leftarrow$ Cnd? valC: valP|PC $\leftarrow$ valC|PC $\leftarrow$ valM|PC <- valP|PC <- valP|

## Y86 Decode

1. Instruction
   * ifun:  
     * IOPQ:
       * 00 ADD
       * 01 SUB
       * 02 AND
       * 03 XOR
     * IJXX:
       * 00 JUMP
       * 01 jle
       * 02 jl
       * 03 je
       * 04 jne
       * 05 jge
       * 06 jg
   * icode:

```cpp
// icode
enum Instruction : uint8_t {
    IHALT   = 0x0,
    INOP    = 0x1,
    IRRMOVQ = 0x2,
    IIRMOVQ = 0x3,
    IRMMOVQ = 0x4,
    IMRMOVQ = 0x5,
    IOPQ    = 0x6,
    IJXX    = 0x7,
    ICALL   = 0x8,
    IRET    = 0x9,
    IPUSHQ  = 0xA,
    IPOPQ   = 0xB
}
```

2. Register

```cpp
enum Register : int {
    RAX   = 0x0,
    RCX   = 0x1,
    RDX   = 0x2,
    RBX   = 0x3,
    RSP   = 0x4,
    RBP   = 0x5,
    RSI   = 0x6,
    RDI   = 0x7,
    R8    = 0x8,
    R9    = 0x9,
    R10   = 0xA,
    R11   = 0xB,
    R12   = 0xC,
    R13   = 0xD,
    R14   = 0xE,
    RNONE = 0xF
};
```

[^1]: Once upon a time, I wanted to write the Y86-Simulator by myself. Till i saw [Hakula's Simulator](https://github.com/hakula139/Y86-64-Simulator). His work is sooo pretty, like any other projects write by a Tech Company. In that case, I decide to study his code and rewrite it.