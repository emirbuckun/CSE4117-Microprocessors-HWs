namespace BirdAssembler;
public class BirdAssembler {
    // Opcode base values
    private const int OPCODE_BASE_LDI = 0x1000;
    private const int OPCODE_BASE_LD = 0x2000;
    private const int OPCODE_BASE_ST = 0x3000;
    private const int OPCODE_BASE_JZ = 0x4000;
    private const int OPCODE_BASE_JMP = 0x5000;
    private const int OPCODE_BASE_ALU = 0x7000;
    private const int OPCODE_BASE_PUSH = 0x8000;
    private const int OPCODE_BASE_POP = 0x9000;
    private const int OPCODE_BASE_CALL = 0xA000;
    private const int OPCODE_BASE_RET = 0xB000;

    // File constants
    private const string VERSION_HEADER = "v2.0 raw";
    private const string FILE_NAME = "bird-test-fibo-sequence";
    private const string INPUT_FILE = $"../Files/Bird/{FILE_NAME}-input.txt";
    private const string OUTPUT_FILE = $"../Files/Bird/{FILE_NAME}-output";

    // AluOp1 for the instructions with 3 operands (ADD, SUB, AND, OR, XOR)
    // AluOp2 for the instructions with 2 or less operands (NOT, MOV, INC, DEC)
    enum ALUOp1 { ADD = 0, SUB = 1, AND = 2, OR = 3, XOR = 4, OTHERS = 7 }
    enum ALUOp2 { NOT = 0, MOV = 1, INC = 2, DEC = 3 }

    // Converts a hex string to an integer
    public static int Hex2Int(string hex) {
        int result = 0;
        foreach (char ch in hex)
            if ('0' <= ch && ch <= '9')
                result = result * 16 + (ch - '0');
            else if ('a' <= ch && ch <= 'f')
                result = result * 16 + ch - 'a' + 10;
            else if ('A' <= ch && ch <= 'F')
                result = result * 16 + ch - 'A' + 10;
        return result;
    }

    public static void Main() {
        string line;
        string[] program = new string[1000];
        int counter = 0;
        var labels = new Dictionary<string, int>();

        try {
            using StreamReader reader = new(INPUT_FILE);
            // First pass: collect labels
            while ((line = reader.ReadLine()!) != null) {
                // Split the line into tokens
                string[] tokens = line.Split(new[] { ' ', '\t', '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);
                if (tokens.Length == 0) continue; // Skip empty lines

                string token = tokens[0]; // First token is the operation name

                if (token.EndsWith(":")) {
                    // Label found
                    string label = token.TrimEnd(':');
                    labels[label] = counter;
                }
                else {
                    // Count instructions
                    counter++;
                    if (token.ToLower() == "ldi") counter++; // LDI has an extra operand
                }
            }

            counter = 0;
            reader.BaseStream.Seek(0, SeekOrigin.Begin); // Reset reader to the beginning

            // Second pass: generate instructions
            while ((line = reader.ReadLine()!) != null) {
                // Split the line into tokens
                string[] tokens = line.Split(new[] { ' ', '\t', '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);
                if (tokens.Length == 0) continue; // Skip empty lines

                string token = tokens[0]; // First token is the operation name

                if (token.EndsWith(":")) // Skip labels
                    continue;

                string op1, op2, op3; // Operands

                // Dictionary to map instruction names to their corresponding actions
                var instructionHandlers = new Dictionary<string, Action<string[]>>
                {
                    { "ldi", tokens => HandleLDI(tokens) },
                    { "ld", tokens => HandleLD(tokens) },
                    { "st", tokens => HandleST(tokens) },
                    { "jz", tokens => HandleJZ(tokens) },
                    { "jmp", tokens => HandleJMP(tokens) },
                    { "add", tokens => HandleALUInstruction(tokens, ALUOp1.ADD) },
                    { "sub", tokens => HandleALUInstruction(tokens, ALUOp1.SUB) },
                    { "and", tokens => HandleALUInstruction(tokens, ALUOp1.AND) },
                    { "or", tokens => HandleALUInstruction(tokens, ALUOp1.OR) },
                    { "xor", tokens => HandleALUInstruction(tokens, ALUOp1.XOR) },
                    { "not", tokens => HandleALUInstruction(tokens, ALUOp1.OTHERS, ALUOp2.NOT) },
                    { "mov", tokens => HandleALUInstruction(tokens, ALUOp1.OTHERS, ALUOp2.MOV) },
                    { "inc", tokens => HandleALUInstruction(tokens, ALUOp1.OTHERS, ALUOp2.INC) },
                    { "dec", tokens => HandleALUInstruction(tokens, ALUOp1.OTHERS, ALUOp2.DEC) },
                    { "push", tokens => HandlePUSH(tokens) },
                    { "pop", tokens => HandlePOP(tokens) },
                    { "call", tokens => HandleCALL(tokens) },
                    { "ret", tokens => HandleRET(tokens) }
                };

                void HandleLDI(string[] tokens) { // LDI r1 0x10
                    if (tokens.Length < 3) { Console.WriteLine("LDI requires 2 operands."); return; }
                    op1 = tokens[1]; op2 = tokens[2];
                    int instructionReg = (Hex2Int(op1)) & 0x0FFF;
                    program[counter++] = (OPCODE_BASE_LDI + instructionReg).ToString("X4");
                    program[counter++] = (op2.StartsWith("0x") ? Hex2Int(op2[2..]) : int.Parse(op2)).ToString("X4");
                }

                void HandleLD(string[] tokens) { // LD r1 r2
                    if (tokens.Length < 3) { Console.WriteLine("LD requires 2 operands."); return; }
                    op1 = tokens[1]; op2 = tokens[2];
                    int instructionReg = ((Hex2Int(op2) << 3) + Hex2Int(op1)) & 0x0FFF;
                    program[counter++] = (OPCODE_BASE_LD + instructionReg).ToString("X4");
                }

                void HandleST(string[] tokens) { // ST r1 r2
                    if (tokens.Length < 3) { Console.WriteLine("ST requires 2 operands."); return; }
                    op1 = tokens[1]; op2 = tokens[2];
                    int instructionReg = ((Hex2Int(op2) << 6) + (Hex2Int(op1) << 3)) & 0x0FFF;
                    program[counter++] = (OPCODE_BASE_ST + instructionReg).ToString("X4");
                }

                void HandleJZ(string[] tokens) { // JZ label or JZ 0x10
                    if (tokens.Length < 2) { Console.WriteLine("JZ requires 1 operand."); return; }
                    op1 = tokens[1];
                    int instructionReg;
                    if (labels.TryGetValue(op1, out int value))
                        instructionReg = (value - counter - 1) & 0x0FFF;
                    else
                        instructionReg = (op1.StartsWith("0x") ? Hex2Int(op1[2..]) : int.Parse(op1)) & 0x0FFF;
                    program[counter++] = (OPCODE_BASE_JZ + instructionReg).ToString("X4");
                }

                void HandleJMP(string[] tokens) { // JMP label or JMP 0x10
                    if (tokens.Length < 2) { Console.WriteLine("JMP requires 1 operand."); return; }
                    op1 = tokens[1];
                    int instructionReg;
                    if (labels.TryGetValue(op1, out int value))
                        instructionReg = (value - counter - 1) & 0x0FFF;
                    else
                        instructionReg = (op1.StartsWith("0x") ? Hex2Int(op1[2..]) : int.Parse(op1)) & 0x0FFF;
                    program[counter++] = (OPCODE_BASE_JMP + instructionReg).ToString("X4");
                }

                void HandlePUSH(string[] tokens) { // PUSH r1
                    if (tokens.Length < 2) { Console.WriteLine("PUSH requires 1 operand."); return; }
                    op1 = tokens[1];
                    int instructionReg = (Hex2Int(op1) << 6) & 0x0FFF;
                    program[counter++] = (OPCODE_BASE_PUSH + instructionReg).ToString("X4");
                }

                void HandlePOP(string[] tokens) { // POP r1
                    if (tokens.Length < 2) { Console.WriteLine("POP requires 1 operand."); return; }
                    op1 = tokens[1];
                    int instructionReg = Hex2Int(op1) & 0x0FFF;
                    program[counter++] = (OPCODE_BASE_POP + instructionReg).ToString("X4");
                }

                void HandleCALL(string[] tokens) { // CALL label or CALL 0x10
                    if (tokens.Length < 2) { Console.WriteLine("CALL requires 1 operand."); return; }
                    op1 = tokens[1];
                    int instructionReg;
                    if (labels.TryGetValue(op1, out int value))
                        instructionReg = (value - counter - 1) & 0x0FFF;
                    else
                        instructionReg = (op1.StartsWith("0x") ? Hex2Int(op1[2..]) : int.Parse(op1)) & 0x0FFF;
                    program[counter++] = (OPCODE_BASE_CALL + instructionReg).ToString("X4");
                }

                void HandleRET(string[] tokens) { // RET
                    program[counter++] = OPCODE_BASE_RET.ToString("X4");
                }

                void HandleALUInstruction(string[] tokens, ALUOp1 aluOp1, ALUOp2? aluOp2 = null) { // ADD r1 r2 r3, NOT r1 r2, INC r1
                    if (aluOp2 == null && tokens.Length < 4) { Console.WriteLine($"{tokens[0].ToUpper()} requires 3 operands."); return; }
                    if (aluOp2 != null && (aluOp2 == ALUOp2.NOT || aluOp2 == ALUOp2.MOV) && tokens.Length < 3) { Console.WriteLine($"{tokens[0].ToUpper()} requires 2 operand."); return; }
                    if (aluOp2 != null && (aluOp2 == ALUOp2.INC || aluOp2 == ALUOp2.DEC) && tokens.Length < 2) { Console.WriteLine($"{tokens[0].ToUpper()} requires 1 operand."); return; }

                    op1 = tokens[1];
                    op2 = tokens.Length > 2 ? tokens[2] : "0";
                    op3 = tokens.Length > 3 ? tokens[3] : "0";

                    if (aluOp1 != ALUOp1.OTHERS && aluOp2 == null) // ADD, SUB, AND, OR, XOR => ADD r1 r2 r3
                        program[counter++] = (OPCODE_BASE_ALU + ((int)aluOp1 << 9) + (Hex2Int(op2) << 6) + (Hex2Int(op3) << 3) + Hex2Int(op1)).ToString("X4");
                    else if (aluOp1 == ALUOp1.OTHERS && aluOp2 == ALUOp2.NOT || aluOp2 == ALUOp2.MOV) // NOT, MOV => NOT r1 r2
                        program[counter++] = (OPCODE_BASE_ALU + ((int)aluOp1 << 9) + ((int)(aluOp2 ?? 0) << 6) + (Hex2Int(op2) << 3) + Hex2Int(op1)).ToString("X4");
                    else if (aluOp1 == ALUOp1.OTHERS && aluOp2 == ALUOp2.INC || aluOp2 == ALUOp2.DEC) // INC, DEC => INC r1
                        program[counter++] = (OPCODE_BASE_ALU + ((int)aluOp1 << 9) + ((int)(aluOp2 ?? 0) << 6) + (Hex2Int(op1) << 3) + Hex2Int(op1)).ToString("X4");
                }

                if (instructionHandlers.TryGetValue(token.ToLower(), out var handler))
                    handler(tokens);
                else
                    Console.WriteLine("Error: No valid opcode.");
            }
        }
        catch (FileNotFoundException e) {
            Console.WriteLine($"Error: File not found. {e.Message}");
            return;
        }
        catch (Exception e) {
            Console.WriteLine($"An error occurred: {e.Message}");
            return;
        }

        using StreamWriter writer = new(OUTPUT_FILE);
        writer.WriteLine(VERSION_HEADER);
        for (int i = 0; i < counter; i++)
            writer.WriteLine(program[i]);
    }
}
