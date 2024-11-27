namespace FrogAssembler;
public class FrogAssembler
{
    // to increase registers to 8, two alu codes are used
    // AluOp1 for the instructions with 3 operands (ADD, SUB, AND, OR, XOR)
    // AluOp2 for the instructions with 2 or less operands (NOT, MOV, INC, DEC)
    enum ALUOp1 { ADD = 0, SUB = 1, AND = 2, OR = 3, XOR = 4, OTHERS = 7 }
    enum ALUOp2 { NOT = 0, MOV = 1, INC = 2, DEC = 3 }

    // Converts a hex string to an integer
    public static int Hex2Int(string hex)
    {
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

    public static void Main()
    {
        string line;
        string[] program = new string[1000];
        int counter = 0;

        try
        {
            using StreamReader reader = new("../Files/myprog.txt");
            while ((line = reader.ReadLine()!) != null)
            {
                // Split the line into tokens
                string[] tokens = line.Split(new[] { ' ', '\t', '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);
                if (tokens.Length == 0) continue; // Skip empty lines

                string token = tokens[0]; // First token is the operation name
                string aluop1, aluop2, op1, op2, op3; // ALU codes and operands
                int ir; // Instruction register

                if (token.Equals("ldi", StringComparison.OrdinalIgnoreCase)) // LDI INSTRUCTION
                { // LDI R1, 0x1234
                    if (tokens.Length < 3)
                    {
                        Console.WriteLine("LDI instruction requires 2 operands.");
                        continue;
                    }

                    op1 = tokens[1];
                    op2 = tokens[2];
                    program[counter] = (0x2000 + Hex2Int(op1)).ToString("X4");
                    counter++;

                    if (op2.StartsWith("0x"))
                        program[counter] = (Hex2Int(op2[2..]) & 0xffff).ToString("X4");
                    else if (op2[0] == '-' || char.IsDigit(op2[0]))
                        program[counter] = (int.Parse(op2) & 0xffff).ToString("X4");
                    else
                        Console.WriteLine("unrecognizable LDI offset");

                    counter++;
                }
                else if (token.Equals("add", StringComparison.OrdinalIgnoreCase)) // ADD INSTRUCTION
                { // ADD R1, R2, R3
                    if (tokens.Length < 4)
                    {
                        Console.WriteLine("ADD instruction requires 3 operands.");
                        continue;
                    }

                    op1 = tokens[1];
                    op2 = tokens[2];
                    op3 = tokens[3];
                    aluop1 = ((int)ALUOp1.ADD).ToString();
                    ir = ((aluop1[0] - '0') << 9) | ((op2[0] - '0') << 6) | ((op3[0] - '0') << 3) | (op1[0] - '0');
                    program[counter] = (0x1000 + (ir & 0x0fff)).ToString("X4");
                    counter++;
                }
                else if (token.Equals("sub", StringComparison.OrdinalIgnoreCase)) // SUB INSTRUCTION
                { // SUB R1, R2, R3
                    if (tokens.Length < 4)
                    {
                        Console.WriteLine("SUB instruction requires 3 operands.");
                        continue;
                    }

                    op1 = tokens[1];
                    op2 = tokens[2];
                    op3 = tokens[3];
                    aluop1 = ((int)ALUOp1.SUB).ToString();
                    ir = ((aluop1[0] - '0') << 9) | ((op2[0] - '0') << 6) | ((op3[0] - '0') << 3) | (op1[0] - '0');
                    program[counter] = (0x1000 + (ir & 0x0fff)).ToString("X4");
                    counter++;
                }
                else if (token.Equals("and", StringComparison.OrdinalIgnoreCase)) // AND INSTRUCTION
                { // AND R1, R2, R3
                    if (tokens.Length < 4)
                    {
                        Console.WriteLine("AND instruction requires 3 operands.");
                        continue;
                    }

                    op1 = tokens[1];
                    op2 = tokens[2];
                    op3 = tokens[3];
                    aluop1 = ((int)ALUOp1.AND).ToString();
                    ir = ((aluop1[0] - '0') << 9) | ((op2[0] - '0') << 6) | ((op3[0] - '0') << 3) | (op1[0] - '0');
                    program[counter] = (0x1000 + (ir & 0x0fff)).ToString("X4");
                    counter++;
                }
                else if (token.Equals("or", StringComparison.OrdinalIgnoreCase)) // OR INSTRUCTION
                { // OR R1, R2, R3
                    if (tokens.Length < 4)
                    {
                        Console.WriteLine("OR instruction requires 3 operands.");
                        continue;
                    }

                    op1 = tokens[1];
                    op2 = tokens[2];
                    op3 = tokens[3];
                    aluop1 = ((int)ALUOp1.OR).ToString();
                    ir = ((aluop1[0] - '0') << 9) | ((op2[0] - '0') << 6) | ((op3[0] - '0') << 3) | (op1[0] - '0');
                    program[counter] = (0x1000 + (ir & 0x0fff)).ToString("X4");
                    counter++;
                }
                else if (token.Equals("xor", StringComparison.OrdinalIgnoreCase)) // XOR INSTRUCTION
                { // XOR R1, R2, R3
                    if (tokens.Length < 4)
                    {
                        Console.WriteLine("XOR instruction requires 3 operands.");
                        continue;
                    }

                    op1 = tokens[1];
                    op2 = tokens[2];
                    op3 = tokens[3];
                    aluop1 = ((int)ALUOp1.XOR).ToString();
                    ir = ((aluop1[0] - '0') << 9) | ((op2[0] - '0') << 6) | ((op3[0] - '0') << 3) | (op1[0] - '0');
                    program[counter] = (0x1000 + (ir & 0x0fff)).ToString("X4");
                    counter++;
                }
                else if (token.Equals("not", StringComparison.OrdinalIgnoreCase)) // NOT INSTRUCTION
                { // NOT R1
                    if (tokens.Length < 2)
                    {
                        Console.WriteLine("NOT instruction requires 1 operands.");
                        continue;
                    }

                    op1 = tokens[1];
                    aluop1 = ((int)ALUOp1.OTHERS).ToString();
                    aluop2 = ((int)ALUOp2.NOT).ToString();
                    ir = ((aluop1[0] - '0') << 9) | ((op1[0] - '0') << 6) | ((aluop2[0] - '0') << 3) | (op1[0] - '0');
                    program[counter] = (0x1000 + (ir & 0x0fff)).ToString("X4");
                    counter++;
                }
                else if (token.Equals("mov", StringComparison.OrdinalIgnoreCase)) // MOV INSTRUCTION
                { // MOV R1, R2
                    if (tokens.Length < 3)
                    {
                        Console.WriteLine("MOV instruction requires 2 operands.");
                        continue;
                    }

                    op1 = tokens[1];
                    op2 = tokens[2];
                    aluop1 = ((int)ALUOp1.OTHERS).ToString();
                    aluop2 = ((int)ALUOp2.MOV).ToString();
                    ir = ((aluop1[0] - '0') << 9) | ((op2[0] - '0') << 6) | ((aluop2[0] - '0') << 3) | (op1[0] - '0');
                    program[counter] = (0x1000 + (ir & 0x0fff)).ToString("X4");
                    counter++;
                }
                else if (token.Equals("inc", StringComparison.OrdinalIgnoreCase)) // INC INSTRUCTION
                { // INC R1
                    if (tokens.Length < 2)
                    {
                        Console.WriteLine("INC instruction requires 1 operand.");
                        continue;
                    }

                    op1 = tokens[1];
                    aluop1 = ((int)ALUOp1.OTHERS).ToString();
                    aluop2 = ((int)ALUOp2.INC).ToString();
                    ir = ((aluop1[0] - '0') << 9) | ((op1[0] - '0') << 6) | ((aluop2[0] - '0') << 3) | (op1[0] - '0');
                    program[counter] = (0x1000 + (ir & 0x0fff)).ToString("X4");
                    counter++;
                }
                else if (token.Equals("dec", StringComparison.OrdinalIgnoreCase)) // DEC INSTRUCTION
                { // DEC R1
                    if (tokens.Length < 2)
                    {
                        Console.WriteLine("DEC instruction requires 1 operand.");
                        continue;
                    }

                    op1 = tokens[1];
                    aluop1 = ((int)ALUOp1.OTHERS).ToString();
                    aluop2 = ((int)ALUOp2.DEC).ToString();
                    ir = ((aluop1[0] - '0') << 9) | ((op1[0] - '0') << 6) | ((aluop2[0] - '0') << 3) | (op1[0] - '0');
                    program[counter] = (0x1000 + (ir & 0x0fff)).ToString("X4");
                    counter++;
                }
                else
                    Console.WriteLine("no valid opcode");
            }
        }
        catch (FileNotFoundException e)
        {
            Console.WriteLine($"Error: File not found. {e.Message}");
            return;
        }
        catch (Exception e)
        {
            Console.WriteLine($"An error occurred: {e.Message}");
            return;
        }

        using StreamWriter writer = new("../Files/RAM");
        writer.WriteLine("v2.0 raw");
        for (int i = 0; i < counter; i++)
            writer.WriteLine(program[i]);
    }
}
