#include "hw1.h"

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

/*
 * You may modify this file and/or move the functions contained here
 * to other source files (except for main.c) as you wish.
 */

/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the
 * program, returning 1 if validation succeeds and 0 if validation fails.
 * Upon successful return, the selected program options will be set in the
 * global variable "global_options", where they will be accessible
 * elsewhere in the program.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return 1 if validation succeeds and 0 if validation fails.
 * Refer to the homework document for the effects of this function on
 * global variables.
 * @modifies global variable "global_options" to contain a bitmap representing
 * the selected options.
 */
int validargs(int argc, char **argv)
{
    if(argc == 1){
        return 0;
    }

    int i = 1;

    if ((argv[i])[0] == '-' && (argv[i])[1] == 'h'){
            global_options = 1;
            return 1;
    }
    else{
        global_options = 0;
    }
    char dash = 'a';
    char letter = 'a';
    //printf("%d\n", argc);
    while(i < argc){
    //    printf("%s\n", argv[i]);
     //   printf("%d\n", i);
        dash = (argv[i])[0];
        letter = (argv[i][1]);
        if (dash != '-'){
      //      printf("DASH PROBLEM");
            return 0;
        }
        else if (argv[i][2] != 0){
     //       printf("NOT A VALID FLAG or FLAG OPTION");
            return 0;
        }
        else{
            if (i == 1){
                if (letter == 'a'){
                    global_options = 0;
        //            printf("LETTER IS A\n");
                    i = i +1;
                }
                else if (letter == 'd'){
                    global_options = 2;
                //    printf("LETTER IS D\n");
                    i = i + 1;
                }
                else{
                //    printf("FIRST NOT A OR D");
                    global_options = 0;
                    return 0;
                }
            }
            else{
                if (letter == 'e'){
                    char endianness = (argv[i+1])[0];
                //    printf("%c\n", endianness);
                    if (argv[i+1][1] != 0){
                        global_options = 0;
                        return 0;
                    }
                    else if (endianness == 'b'){
                        global_options += 4;
                        i += 2;
                  //      printf("BIG\n");

                    }
                    else if (endianness == 'l'){
                        i += 2;
                //        printf("LITTLE\n");
                    }
                    else{
                        global_options = 0;
                        return 0;
                    }
                }
                else if (letter == 'b'){
                    int j = 0;
                    for(j = 0; j < 8; j++){
                        char c = (argv[i+1])[j];
                //        printf("hex char : %d\n", c);
                        if (c == 0){
                            break;
                        }
                        else if (!((c >= 48 && c <= 57) || (c >= 65 && c <= 70) || (c >= 97 && c <= 102))){
                //            printf("bad char");
                            global_options = 0;
                            return 0;
                        }
                    }
                    if (argv[i+1][j] != 0){
                        global_options = 0;
                        return 0;
                    }
                    int k = 0;
                    for(k = j-3; k < j; k++){
                        char c = (argv[i+1][k]);
                        if (c != '0'){
                            global_options = 0;
                            return 0;
                        }
                    }
                    int power = j-1;
                    int multiplier = 1;
                    for (int l = 0; l < power; l++){
                        multiplier = multiplier * 16;
                    }
                    for (k = 0; k < j-2; k++){
                        char c = (argv[i+1])[k];
                        int num = 0;
                        if (c >= 48 && c <= 57){
                            num = c - 48;
                            global_options = num*multiplier + global_options;
                        }
                        else{
                            if (c == 65 || c == 97){
                                global_options = 10*multiplier + global_options;
                            }
                            else if (c == 66 || c == 98){
                                global_options = 11*multiplier + global_options;
                            }
                            else if (c == 67 || c == 99){
                                global_options = 12*multiplier + global_options;
                            }
                            else if (c == 68 || c == 100){
                                global_options = 13*multiplier + global_options;
                            }
                            else if (c == 69 || c == 101){
                                global_options = 14*multiplier + global_options;
                            }
                            else if (c == 70 || c == 102){
                                global_options = 15*multiplier + global_options;
                            }
                        }
                        multiplier = multiplier/16;
                    }
                    i += 2;
                }
                else{
                    global_options = 0;
                    return 0;
                }
            }

        }


    }
    return 1;
}

/**
 * @brief Computes the binary code for a MIPS machine instruction.
 * @details This function takes a pointer to an Instruction structure
 * that contains information defining a MIPS machine instruction and
 * computes the binary code for that instruction.  The code is returne
 * in the "value" field of the Instruction structure.
 *
 * @param ip The Instruction structure containing information about the
 * instruction, except for the "value" field.
 * @param addr Address at which the instruction is to appear in memory.
 * The address is used to compute the PC-relative offsets used in branch
 * instructions.
 * @return 1 if the instruction was successfully encoded, 0 otherwise.
 * @modifies the "value" field of the Instruction structure to contain the
 * binary code for the instruction.
 */
int encode(Instruction *ip, unsigned int addr){
    Instruction instr = *ip;

    Instr_info *infoPtr = instr.info; // pointer to info
    Instr_info info = *infoPtr;

    unsigned int binaryCode = 0;

    Opcode op = info.opcode;
    //printf("\nENCODE\nOpcode : %d\n", op);
    //printf("type : %d\n", info.type);
    //printf("binary num: %d\n", instr.value);

    int i = 0;
    for (i = 0; i < 64; i++){
        if (specialTable[i] == op){
            break;
        }
    }
    //printf("special op: %d\n", i);
    if (i < 64){
        binaryCode = binaryCode|i;
    }
    else{
        if (op == OP_BLTZ){
            binaryCode = 1 << 28;
        }
        else if(op == OP_BGEZ){
            binaryCode = 268500992;
        }
        else if (op == OP_BLTZAL){
            binaryCode = 269484032;

        }
        else if (op == OP_BGEZAL){
            binaryCode = 269549568;
        }
        else{
            for (i = 0; i < 64; i++){
                if (opcodeTable[i] == op){
                    break;
                }
            }
            if (i < 64){
                i = i << 26;
                binaryCode = binaryCode|i;
            }
            else{
                return 0;
            }
        }
    }
    //printf("binary with opcode: %x\n", binaryCode);
    unsigned int arg = 0;
    int immed = 0;
    for (i = 0; i < 3; i++){
        Source src = info.srcs[i];

        if (src == NSRC){
            continue;
        }
        else if (src == RS){
            arg = instr.args[i];
            arg = arg << 21;
            binaryCode = binaryCode|arg;
        }
        else if (src == RT){
            arg = instr.args[i];
            arg = arg << 16;
            binaryCode = binaryCode|arg;
        }
        else if (src == RD){
            arg = instr.args[i];
            arg = arg << 11;
            binaryCode = binaryCode|arg;
        }
        else if (src == EXTRA){
            if (op == OP_BREAK){
                arg = instr.extra;
                arg = arg << 6;
                binaryCode = binaryCode|arg;
            }
            else if (info.type == RTYP){
                arg = instr.extra;
                arg = arg << 6;
                binaryCode = binaryCode|arg;
            }
            else if (info.type == ITYP){
                int extra = instr.extra;
                //printf("extra: %d\n", extra);
                if (op == OP_BEQ || op == OP_BGEZ || op == OP_BGEZAL || op == OP_BGTZ || op == OP_BLEZ || op == OP_BLTZ || op == OP_BLTZAL || op == OP_BNE){
                    extra = extra - (addr+4);
                    //printf("extra: %d\n", extra);
                    extra = extra >> 2;
                    //printf("extra: %d\n", extra);
                    arg = extra;
                    //printf("arg: %d\n", arg);
                    arg = arg << 16;
                    arg = arg >> 16;
                    binaryCode = binaryCode|arg;
                }
                else if (extra < 0){
                    //extra = 0 - extra;
                    //arg = extra + 32768;
                    arg = extra & 0x0000ffff;
                    binaryCode = binaryCode|arg;
                }
                else{
                    arg = extra;
                    binaryCode = binaryCode|arg;
                }
                //printf("itype immed: %d\n", arg);

            }
            else if (info.type == JTYP){
                arg = instr.extra;
                unsigned int pc = addr >> 28;
                pc = pc << 28;
                arg = arg - pc;
                arg = arg >> 2;
                binaryCode = binaryCode|arg;
            }
        }
    }
    (*ip).value = binaryCode;
   // printf("binary with RS, RT, RD : %x\n", binaryCode);
    return 1;
}

/**
 * @brief Decodes the binary code for a MIPS machine instruction.
 * @details This function takes a pointer to an Instruction structure
 * whose "value" field has been initialized to the binary code for
 * MIPS machine instruction and it decodes the instruction to obtain
 * details about the type of instruction and its arguments.
 * The decoded information is returned by setting the other fields
 * of the Instruction structure.
 *
 * @param ip The Instruction structure containing the binary code for
 * a MIPS instruction in its "value" field.
 * @param addr Address at which the instruction appears in memory.
 * The address is used to compute absolute branch addresses from the
 * the PC-relative offsets that occur in the instruction.
 * @return 1 if the instruction was successfully decoded, 0 otherwise.
 * @modifies the fields other than the "value" field to contain the
 * decoded information about the instruction.
 */
int decode(Instruction *ip, unsigned int addr) {
    Instruction instr = *ip;
    unsigned int binary = instr.value;
    unsigned int binaryOp = binary >> 26;
    //printf("binary number : %d\n", binary);
    //printf("opcode value: %d\n", binaryOp);
    Opcode op = opcodeTable[binaryOp];
    //printf("%d\n", op);
    enum opcode special = SPECIAL;
    enum opcode bcond = BCOND;
    if (op == ILLEGL){
        return 0;
    }
    if (op == special){
       // printf("special\n");
        binaryOp = binary <<  26;
        binaryOp  = binaryOp >> 26;
       // printf("bits 5:0 : %d\n", binaryOp);
        op = specialTable[binaryOp];
    }
    else if (op == bcond){
        binaryOp = binary << 12;
        binaryOp = binaryOp >> 27;
       // printf("bits 20:16 : %d\n", binaryOp);
        if (binaryOp == 0){
            op = OP_BLTZ;
        }
        else if (binaryOp == 1){
            op = OP_BGEZ;
        }
        else if (binaryOp == 16){
            op = OP_BLTZAL;
        }
        else if (binaryOp == 17){
            op = OP_BGEZAL;
        }
        else {
            return 0;
        }
    }
  //  printf("printing final op: %d\n", op);
    enum source rs = RS;
    enum source rt = RT;
    enum source rd = RD;


    Instr_info info = instrTable[op];
    (*ip).info = &instrTable[op];
  //  printf("Instr_info opcode : %d\n", (*((*ip).info)).opcode);
    Type type = (*((*ip).info)).type;
    unsigned int reg = 0;
    int i = 0;
    for (int i = 0; i < 3; i++){
        if (i == 0){
            reg = binary << 6;
            reg = reg >> 27;
        }
        else if (i == 1){
            reg = binary << 11;
            reg = reg >> 27;
        }
        else if (i == 2){
            reg = binary << 16;
            reg = reg >> 27;
        }
        (*ip).regs[i] =  reg;
    }
    unsigned int arg = 0;
    i = 0;
    for (i = 0; i < 3; i++){
        Source src = info.srcs[i];
     //   printf("source %d: %d\n", i, src);

        if (src == RS){
           // printf("RS\n");
            arg = binary << 6;
            arg = arg >> 27;
        }
        else if (src == RT){
          //  printf("RT\n");
            arg = binary << 11;
            arg = arg >> 27;

        }
        else if (src == RD){
           // printf("RD\n");
            arg = binary << 16;
            arg = arg >> 27;
        }
        else if (src == EXTRA){
          //  printf("EXTRA\n");
          //  printf("type : %d\n", type);
            if (op == OP_BREAK){
                arg = binary << 6;
                arg = arg >> 12;
            }
            else if (type == RTYP){
                arg = binary << 21;
                arg = arg >> 27;
            }
            else if (type == ITYP){
                arg = binary << 16;
                arg = arg >> 16;
                if (op == OP_BEQ || op == OP_BGEZ || op == OP_BGEZAL || op == OP_BGTZ || op == OP_BLEZ || op == OP_BLTZ || op == OP_BLTZAL || op == OP_BNE){
                    int extraArg = arg;
                    extraArg = extraArg << 16;
                    extraArg = extraArg >> 16;
                    extraArg = extraArg << 2;
                    unsigned int memAddress = extraArg + addr + 4;
                    (*ip).args[i] = memAddress;
                    (*ip).extra = memAddress;
                  //  printf("extra arg: %d\n", memAddress);
                    continue;
                }
               // if (arg >= 32768){
                    int extraArg = arg;
                    extraArg = extraArg << 16;
                    extraArg = extraArg >> 16;
                    //arg = arg - 32768;
                    //extraArg = 0 - arg;
                    (*ip).args[i] = extraArg;
                    (*ip).extra = extraArg;
                   // printf("extra arg: %d\n", extraArg);
                    continue;
                //}

            }
            else if (type == JTYP){
                arg = binary << 6;
                arg = arg >> 6;
                arg = arg << 2;
             //   printf("j bits: %d\n", arg);
                unsigned int memAddress = addr >> 28;
                memAddress = memAddress << 28;
            //    printf("memAddress : %u\n", memAddress);
                arg = memAddress + arg;

            }
           // printf("extra arg pos: %u\n", arg);
            (*ip).extra = arg;

        }
        else if (src == NSRC){
            arg = 0;
        }
       // printf("arg%d: %d\n", i, arg);
        (*ip).args[i] = arg;
    }

    return 1;
}
