# CSE4117-Microprocessors-HWs

## HW1

- Grade: 80 points

### Components

- Register Bank ✅
  - 16 bit 8 registers
- ALU ✅
  - ADD, SUB, AND, OR, XOR, NOT, MOVE, INC, DEC
- Memory ✅
  - 10-bit address, 16-bit data
- PC ✅
  - 10 bit register

### Details

- Realize a register bank with 8 16-bit registers in [Logisim](http://www.cburch.com/logisim/)
- Reg7 will be inc & dec.
- Reg6 will be inc. only
- Use INC7 DEC7 INC6 signals
- SR flip flops will be used
- Use ready muxes

### Result

![image](https://github.com/user-attachments/assets/016a2a6d-a39c-4e47-8d81-3e311e34b60d)

## HW2

- Grade: 100 points

### Components

- Same components as HW1
- Instruction Register ✅
  - 12-bit register
- Control Unit ✅
  - Send control signals: PCINC, REGLD, MUX, IRLD, FETCH
- Assembler ✅
  - Convert assembly to hex

### Details

- Create an assembler for converting assembly to hex
- Upload the assembler result file to ROM
- Only ALU and load operations will be performed
- Create instruction register using 12-bit register
- Prepare control unit according to the instructions

### Instruction Set Architecture Design

- TODO: Prepare a design drawing 

### Result

![image](https://github.com/user-attachments/assets/4c361e1a-42a0-4ddd-9764-a534ab6ee0c7)
