# Instruction Set (Y86-simulator) Review

## Y86 instructions process in defferent stages(SEO)

|Stage|OPq,rA,rB|rrmovq rA,rB|irmovq V,rB|rmmovq rA,D(rB)|mrmovq D(rB),rA|jXX Dest|call Dest|ret|push rA|pop rA|
|-|-|-|-|-|-|-|-|-|-|-|
|**Fetch**|icode:ifun = $\leftarrow$ $M_1$[PC]<br/>rA:rB $\leftarrow$ $M_1$[PC+1]<br/><br/> valP $\leftarrow$ PC + 2|icode:ifun = $\leftarrow$ $M_1$[PC]<br/>rA:rB $\leftarrow$ $M_1$[PC+1]<br/><br/>valP $\leftarrow$ PC + 2|icode:ifun = $\leftarrow$ $M_1$[PC]<br/>rA:rB $\leftarrow$ $M_1$[PC+1]<br/>valC $\leftarrow$ $M_8$[PC+2]<br/>PC $\leftarrow$ PC + 10|icode:ifun = $\leftarrow$ $M_1$[PC]<br/>rA:rB $\leftarrow$ $M_1$[PC+1]<br/>valC $\leftarrow$ $M_8$[PC+2]<br/>PC $\leftarrow$ PC + 10|icode:ifun = $\leftarrow$ $M_1$[PC]<br/>rA:rB $\leftarrow$ $M_1$[PC+1]<br/>valC $\leftarrow$ $M_8$[PC+2]<br/>PC $\leftarrow$ PC + 10|icode:ifun $\leftarrow$ $M_1$[PC]<br/><br/>valC $\leftarrow$ $M_8$[PC+1]<br/>valP $\leftarrow$ PC + 9|icode:ifun $\leftarrow$ $M_1$[PC]<br/><br/>valC $\leftarrow$ $M_8$[PC+1]<br/>valP $\leftarrow$ PC + 9|icode:ifun $\leftarrow$ $M_1$[PC]<br/><br/><br/>valP $\leftarrow$ PC + 9|icode:ifun <- $M_1$[PC]<br/>rA:rB <- $M_1$[PC+1]<br/><br/>valP <- PC+2|icode:ifun <- $M_1$[PC]<br/>rA:rB <- $M_1$[PC+1]<br/><br/>valP <- PC+2|
|**Decode**|valA $\leftarrow$ R[rA]<br/>valB $\leftarrow$ R[rB]|valA $\leftarrow$ R[rA]||valA $\leftarrow$ R[rA]<br/>valB $\leftarrow$ R[rB]|<br/>valB $\leftarrow$ R[rB]||<br/>valB $\leftarrow$ R[%rsp]|valA $\leftarrow$ R[%rsp]<br/>valB $\leftarrow$ R[%rsp]|valA <- R[rA]<br/>valB <- R[%rsp]|valA <- R[%rsp]<br/>valB <- R[%rsp]|
|**Excute**|valE $\leftarrow$ valB OP valA<br/>set CC|valE $\leftarrow$ 0 + valA|valE $\leftarrow$ 0 + valC|valE $\leftarrow$ valB + valC|valE $\leftarrow$ valB + valC|<br/>Cnd $\leftarrow$ Cond(CC, ifun)|valE $\leftarrow$ valB - 8|valE $\leftarrow$ valB + 8|valE <- valB - 8|valE <- valB + 8|
|**Memory**||||$M_8$[valE] $\leftarrow$ valA|valM $\leftarrow$ $M_8$[valE]||$M_8$[valE] $\leftarrow$ valP|valM $\leftarrow$ $M_8$[valA]|valM <- $M_8$[valA]|valM <- $M_8$[valA]|
|**Write back**|<br/>R[rB] $\leftarrow$ valE|<br/>R[rB] $\leftarrow$ valE|<br/>R[rB] $\leftarrow$ valE||R[rA] $\leftarrow$ valM||<br/>R[%rsp] $\leftarrow$ valE|<br/>R[%rsp] $\leftarrow$ valE|R[rA] <- valM<br/>R[%rsp] <- valE|R[rA] <- valM<br/>R[%rsp] <- valE|
|**PC update**|PC $\leftarrow$ valP|PC $\leftarrow$ valP|PC $\leftarrow$ valP|PC $\leftarrow$ valP|PC $\leftarrow$ valP|PC $\leftarrow$ Cnd? valC: valP|PC $\leftarrow$ valC|PC $\leftarrow$ valM|PC <- valP|PC <- valP|