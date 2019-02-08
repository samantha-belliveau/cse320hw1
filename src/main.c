#include <stdlib.h>

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

#include "hw1.h"
#include "debug.h"


int main(int argc, char **argv){

    if(!validargs(argc, argv))
        USAGE(*argv, EXIT_FAILURE);
    //printf("Options: 0x%X", global_options);
    if(global_options & 0x1) {
        USAGE(*argv, EXIT_SUCCESS);
    }

    unsigned int var = global_options;
    var = var << 30;
    var = var >> 30;

    if (var == 2){
        char line[5];

        var = global_options;
        var = var << 29;
        var = var >> 31;


        // disassembly mode
        // decode
        unsigned int addr = global_options >>12;
        addr = addr << 12;
        unsigned int binary = 0;
        unsigned int temp = 0;

        while (fgets(line, 5, stdin) != NULL){
            binary = 0;

            if (var == 1){
                temp = 0;
                temp = line[0]& 0x000000ff;
                temp = temp << 24;
                binary = binary | temp;

                temp = line[1]& 0x000000ff;
                temp = temp << 16;
                binary = binary | temp;

                temp = line[2]& 0x000000ff;
                temp = temp << 8;
                binary = binary | temp;

                temp = line[3]& 0x000000ff;
                binary = binary | temp;
            }
            else {

                temp = 0;
                temp = line[0]& 0x000000ff;
                //printf("what: %x\n", temp);
                binary = binary | temp;

                temp = line[1]& 0x000000ff;
                //printf("what: %x\n", temp);

                temp = temp << 8;
                binary = binary | temp;

                //printf("line2 : %x\n", line[2]);
                temp = line[2]& 0x000000ff;
                //printf("what: %x\n", temp);

                temp = temp << 16;
                binary = binary | temp;

                temp = line[3]& 0x000000ff;
                //printf("what: %x\n", temp);
                temp = temp << 24;
                binary = binary | temp;
            }
            // endiann stuff

            Instruction instr;
            instr.value = binary;
            Instruction *ip = &instr;
           // printf("Hex: %x\n", binary);

            if(!(decode(ip, addr))){
                return EXIT_FAILURE;
            }

            Instr_info info = *instr.info;
            char *format = info.format;

            int arg0 = instr.args[0];
            int arg1 = instr.args[1];
            int arg2 = instr.args[2];
            printf(format, arg0, arg1, arg2);
            printf("\n");
            addr += 4;
        }
        return EXIT_SUCCESS;

        // convert from string to binary #

    }
    else if (var == 0){
        char line[50];

        unsigned int addr = global_options >>12;
        addr = addr << 12;
        // assembly mode
        // encode


        while(fgets(line, sizeof(line), stdin) != NULL){
            int r1 = -1;
            int r2 = -1;
            int r3 = -1;
            int i = 0;
            Instruction instr;
            instr.value = 0;
            Instruction *ip = &instr;

            Instr_info info;
            char *format;
            int j = 0;
            int match;
            for (i = 0; i < 64; i++){
                //char test[40];

                info = instrTable[i];
                format = info.format;

                j = 0;
                match = 1;
                for (j = 0; line[j] != ' '; j++){
                    //printf("input: %c\n", line[j]);
                    //printf("format: %c\n", format[j]);
                    if (line[j+1] == '\0'){
                        break;
                    }
                    if (line[j] != format[j]){
                        if (line[j] != '\0'){
                            match = 0;
                        }
                        //match = 0;
                    }

                }
                if (!(format[j] != ' ' || format[j+1] != '\0')){
                    match = 0;
                }
                if (match == 0){
                    continue;
                }
                else{
                    break;
                }
            }
            if (match == 0){
                return EXIT_FAILURE;
            }

                instr.info = &info;

                int k = j+1;
                for (j = j+1; line[j] != '\0'; j++){
                    //printf("Current line and format: %c, %c\n", line[j], format[k]);
                    if (line[j] == format[k]){
                        k++;
                        continue;
                    }
                    else if (format[k] == '%'){
                        //printf("%c\n", format[k+1]);
                        if (format[k+1] == 'd'){
                            //if (line[j] >= '0' && line[j] <= '9'){
                                //what if register is more than 1 char? i.e., $12?
                                if (r1 < 0){
                                    int neg = 0;
                                    if (line[j] == '-'){
                                        neg = 1;
                                        j++;
                                    }
                                    int a = j;

                                    while (line[a] >= '0' && line[a] <= '9'){
                                        //printf("test: %c\n", line[a]);
                                        a++;
                                    }
                                    a = a-1;
                                    int c = 0;
                                    r1 = 0;
                                    int mul = 1;
                                    for (c = a; c >= j; c--){
                                        r1 += (line[c]-'0') * mul;
                                        mul = mul * 10;
                                    }

                                    //r1 = line[j] - '0';
                                    //sscanf(&line[j], &format[k], &r1);
                                    //printf("r1: %d\n", r1);
                                    if (r1 > 9){
                                        j++;
                                    }
                                    if (r1 > 99){
                                        j++;
                                    }
                                    if (neg){
                                        r1 = r1 * -1;
                                    }
                                }
                                else if (r2 < 0){
                                    int neg = 0;
                                    if (line[j] == '-'){
                                        neg = 1;
                                        j++;
                                    }
                                    int a = j;
                                    while (line[a] >= '0' && line[a] <= '9'){
                                        //printf("test: %c\n", line[a]);
                                        a++;
                                    }
                                    a = a-1;
                                    int c = 0;
                                    r2 = 0;
                                    int mul = 1;
                                    for (c = a; c >= j; c--){
                                        r2 += (line[c]-'0') * mul;
                                        mul = mul * 10;
                                    }

                                    //r2 = line[j] - '0';
                                    //sscanf(&line[j], &format[k], &r2);
                                    //printf("r2: %d\n", r2);
                                    if (r2 > 9){
                                        j++;
                                    }
                                    if (r2 > 99){
                                        j++;
                                    }
                                    if (neg){
                                        r2 = r2 * -1;
                                    }
                                }
                                else if (r3 < 0){
                                    int neg = 0;
                                    if (line[j] == '-'){
                                        neg = 1;
                                        j++;
                                    }
                                    int a = j;
                                    while (line[a] >= '0' && line[a] <= '9'){
                                        //printf("test: %c\n", line[a]);
                                        a++;
                                    }
                                    a = a-1;
                                    int c = 0;
                                    r3 = 0;
                                    int mul = 1;
                                    for (c = a; c >= j; c--){
                                        r3 += (line[c]-'0') * mul;
                                        mul = mul * 10;
                                    }
                                    //r3 = line[j] - '0';
                                    //sscanf(&line[j], &format[k], &r3);
                                    if (r3 > 9){
                                        j++;
                                    }
                                    if (r3 > 99){
                                        j++;
                                    }
                                    if (neg){
                                        r3 = r3 *-1;
                                    }
                                }
                        }
                        else if (format[k+1] == 'x'){
                            //printf("%c\n", line[j]);
                            if (r1 < 0){
                                sscanf(&line[j], &format[k], &r1);
                            }
                            else if (r2 < 0){
                                sscanf(&line[j], &format[k], &r2);

                            }
                            else if (r3 < 0){
                                sscanf(&line[j], &format[k], &r3);

                            }
                            break;
                        }
                        else{
                            match = 0;
                        }
                        k+= 2;
                    }
                }

            instr.args[0] = r1;
            instr.args[1] = r2;
            instr.args[2] = r3;

            instr.regs[0] = 0;
            instr.regs[1] = 0;
            instr.regs[2] = 0;

            if (info.srcs[0] == RS){
                instr.regs[0] = r1;
            }
            else if (info.srcs[0] == RT){
                instr.regs[1] = r1;
            }
            else if (info.srcs[0] == RD){
                instr.regs[2] = r1;
            }

            if (info.srcs[1] == RS){
                instr.regs[0] = r2;
            }
            else if (info.srcs[1] == RT){
                instr.regs[1] = r2;
            }
            else if (info.srcs[1] == RD){
                instr.regs[2] = r2;
            }

            if (info.srcs[2] == RS){
                instr.regs[0] = r3;
            }
            else if (info.srcs[2] == RT){
                instr.regs[1] = r3;
            }
            else if (info.srcs[2] == RD){
                instr.regs[2] = r3;
            }

            for (i = 0; i < 3; i++){
                if (info.srcs[i] == EXTRA){
                    instr.extra = instr.args[i];
                    break;
                }
            }
            //break;

            //}

            if(!(encode(ip, addr))){
                return EXIT_FAILURE;
            }
            addr += 4;

            unsigned int bin = instr.value;
            unsigned int b1, b2, b3, b4;
            //printf("Hex: %x\n: ", bin);
            b1 = bin & 0xff000000;
            //printf("b1: %x\n", b1);

            b1 = b1 >> 24;
            //printf("b1: %x\n", b1);

            b2 = bin & 0x00ff0000;
            b2 = b2 >> 16;
            //printf("b2: %x\n", b2);

            b3 = bin & 0x0000ff00;
            b3 = b3 >> 8;
            //printf("b3: %x\n", b3);

            b4 = bin & 0x000000ff;
            //printf("b4: %x\n", b4);

            var = global_options;
            var = var << 29;
            var = var >> 31;

            if (var == 1){
                putchar(b1);
                putchar(b2);
                putchar(b3);
                putchar(b4);
            }
            else{
                putchar(b4);
                putchar(b3);
                putchar(b2);
                putchar(b1);
            }

            //printf("Binary: %x\n", instr.value);
        }

    }
    /*printf("global_options : %X\n", global_options);
    *Instruction instrValue;
    *instrValue.value = 284163103;
    Instruction *ip;
    ip = &instrValue;
    unsigned int address = 4096;
    decode(ip, address);
    printf("arg1 in main : %d\n", instrValue.args[0]);

    printf("Opcode before encode: %d\n", info.opcode);
    //instrValue.info = &info;
    encode(ip, address);
    printf("Instruction as hex post encode: %x\n", instrValue.value);*/

    return EXIT_SUCCESS;
}

/*
 * Just a reminder: All non-main functions should
 * be in another file not named main.c
 */
