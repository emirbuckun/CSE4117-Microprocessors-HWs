#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h> // Add this line

int numberofinst = 500;

char *convertregistertobinary(char Register[]){
	
	if(strstr(Register, "R0")){
		return "000";
	}
	else if(strstr(Register, "R1")){
		return "001";
	}
	else if(strstr(Register, "R2")){
		return "010";
	}
	else if(strstr(Register, "R3")){
		return "011";
	}
	else if(strstr(Register, "R4")){
		return "100";
	}
	else if(strstr(Register, "R5")){
		return "101";
	}
	else if(strstr(Register, "R6")){
		return "110";
	}
	else if(strstr(Register, "R7")){
		return "111";
	}
	
}

char* twoscomp16(int num){
    
    int mask = 0xFFFF;
    int twosComplement = num & mask;
    
    char* binary = (char*)malloc(17 * sizeof(char));
    if(binary == NULL){
        return NULL;
    }

    binary[16] = '\0'; 
    
    int i;
    for(i = 15; i >= 0; i--){
        binary[i] = (twosComplement & 1) ? '1' : '0';
        twosComplement >>= 1;
    }
    
    return binary;
}

char* twoscomp12(int num){
	
    int mask = 0xFFF;
    int twosComplement = num & mask;
    
    char* binary = (char*)malloc(13 * sizeof(char));
    if(binary == NULL){
        return NULL;
    }

    binary[12] = '\0';
    
    int i;
    for(i = 11; i >= 0; i--){
        binary[i] = (twosComplement & 1) ? '1' : '0';
        twosComplement >>= 1;
    }
    
    return binary;
}

int main(){
	
	char dreg[3];
	char s1reg[3];
	char s2reg[3];
	//char sreg[4];
	//char reg1[4];
	//char reg2[4];
	char insname[10];
	char opcode[4];
	char alucode[3];//new
	char alucodeextra[3];//new
	char binary[16];
	char binary32[32];
	//int IMM;
	//int ADDR;
	int DATA;
	int OFFSET;
	
	int currentstatementbunmber = 0;
	
	FILE *fp;
    fp = fopen("input.txt", "rt");
    if (fp == NULL) { 
        printf("File does not exist."); 
        exit(1); 
	}
	
	char *outputname= "output_test.txt";
	FILE *file1= fopen(outputname,"w");
	fprintf(file1,"v2.0 raw\n");
	
	/////////////////////////////////////
	int count = 0;
	int loadiline[numberofinst];
	int labelline[numberofinst];
	char word[100];
	FILE *fp2;
    fp2 = fopen("input.txt", "rt");
    if(fp2 == NULL){ 
        printf("File does not exist."); 
        exit(1); 
	}
	while(fscanf(fp2, "%s", word) == 1){
        if(strcmp(word,"LDI") == 0){
            loadiline[count] = 1;
        }
        else{
        	loadiline[count] = 0;
		}
		
		int len = strlen(word);
        if (len > 0 && word[len - 1] == ':'){
            labelline[count] = 1;
        } 
		else{
            labelline[count] = 0;
        }

        int c;
        while((c = fgetc(fp2)) != '\n' && c != EOF){
        }

        count++;
    }
    int abc;
    for(abc = 0;abc < numberofinst;abc++){
    	if(loadiline[abc] == 1){
    		printf("%d ",abc);
		}
	}
	printf("\n");
	for(abc = 0;abc < numberofinst;abc++){
    	if(labelline[abc] == 1){
    		printf("%d ",abc);
		}
	}
	printf("\n");
    fclose(fp2);
	/////////////////////////////////////
	
	while(10){
		printf("***\n");
		fscanf(fp,"%s",&insname);
		
		if ( feof(fp) )
            break;
		
		
		
		if(strstr(insname, "ADD") || strstr(insname, "SUB") || strstr(insname, "AND") || strstr(insname, "OR") || strstr(insname, "XOR") || strstr(insname, "SHL")){
			
			strcpy(opcode,"0111");
			
		    if(strstr(insname, "ADD")){
				strcpy(alucode,"000");
			}
			else if(strstr(insname, "SUB")){
				strcpy(alucode,"001");
			}
			else if(strstr(insname, "AND")){
				strcpy(alucode,"010");
			}
			else if(strstr(insname, "XOR")){
				strcpy(alucode,"100");
			}
			else if(strstr(insname, "OR")){
				strcpy(alucode,"011");
			}
			else if(strstr(insname, "SHL")){
				strcpy(alucode,"101");
			}
			
			fscanf(fp,"%s",&dreg);
			fscanf(fp,"%s",&s1reg);
			fscanf(fp,"%s",&s2reg);
			
			char bdreg[3];
			strcpy(bdreg,convertregistertobinary(dreg));
			char bs1reg[3];	
			strcpy(bs1reg,convertregistertobinary(s1reg));
			char bs2reg[3];	
			strcpy(bs2reg,convertregistertobinary(s2reg));
			
			//printf("%s %s %s %s -> ",insname,bdreg,bs1reg,bs1reg);
			strcpy(binary,strcat(opcode,strcat(alucode,strcat(bs1reg,strcat(bs2reg,bdreg)))));
			//printf("%s -> \n",binary);
			

			int newbinary[16];
			int hg;
			for(hg = 0;hg < sizeof(binary);hg++){
				newbinary[hg] = binary[hg] - 48;
			}
			
			char hex[4];
			
			
			int k,m,c = 0,l = 0,sum = 0;
            for(k = 4;k > 0;k--){
  	            for(m = (k * 4)- 1 ;m >= (k*4) - 4;m--){
  		            sum += pow(2,l)* newbinary[m];
  		            l++;
	            }
	        if(sum == 0){
	  	        hex[c] = '0';
	        }
	        else if(sum == 1){
	  	        hex[c] = '1';
	        }
	        else if(sum == 2){
	  	        hex[c] = '2';
	        }
	        else if(sum == 3){
	  	        hex[c] = '3';
	        }
	        else if(sum == 4){
	  	        hex[c] = '4';
	        }
	        else if(sum == 5){
	  	        hex[c] = '5';
	        }
	        else if(sum == 6){
	  	        hex[c] = '6';
	        }
	        else if(sum == 7){
	  	        hex[c] = '7';
	        }
	        else if(sum == 8){
	  	        hex[c] = '8';
	        }
	        else if(sum == 9){
	  	        hex[c] = '9';
	        }
	        else if(sum == 10){
	  	        hex[c] = 'A';
	        }
	        else if(sum == 11){
	  	        hex[c] = 'B';
	        }
	        else if(sum == 12){
	  	        hex[c] = 'C';
	        }
	        else if(sum == 13){
	  	        hex[c] = 'D';
	        }
	        else if(sum == 14){
	  	        hex[c] = 'E';
	        }
	        else if(sum == 15){
	  	        hex[c] = 'F';
	        }
	        sum = 0;
	        l = 0;
	        c++;
            }
			
			fprintf(file1,"%c%c%c%c\n",hex[3],hex[2],hex[1],hex[0]);
			
			
			currentstatementbunmber++;
			continue;
			
		}
		
		else if(strstr(insname, "INC") || strstr(insname, "DEC")){
			
			strcpy(opcode,"0111");
			
			if(strstr(insname, "INC")){
				strcpy(alucode,"111");
				strcpy(alucodeextra,"010");
			}
			else if(strstr(insname, "DEC")){
				strcpy(alucode,"111");
				strcpy(alucodeextra,"011");
			}
			
			fscanf(fp,"%s",&dreg);

			char bdreg[3];
			strcpy(bdreg,convertregistertobinary(dreg));
			//printf("%s %s -> ",insname,bdreg);
			strcpy(binary,strcat(opcode,strcat(alucode,strcat(alucodeextra,strcat(bdreg,bdreg)))));
			//printf("%s -> \n",binary);//
			
			

			int newbinary[16];
			int hg;
			for(hg = 0;hg < sizeof(binary);hg++){
				newbinary[hg] = binary[hg] - 48;
			}
			
			char hex[4];
			
			
			int k,m,c = 0,l = 0,sum = 0;
            for(k = 4;k > 0;k--){
  	            for(m = (k * 4)- 1 ;m >= (k*4) - 4;m--){
  		            sum += pow(2,l)* newbinary[m];
  		            l++;
	            }
	        if(sum == 0){
	  	        hex[c] = '0';
	        }
	        else if(sum == 1){
	  	        hex[c] = '1';
	        }
	        else if(sum == 2){
	  	        hex[c] = '2';
	        }
	        else if(sum == 3){
	  	        hex[c] = '3';
	        }
	        else if(sum == 4){
	  	        hex[c] = '4';
	        }
	        else if(sum == 5){
	  	        hex[c] = '5';
	        }
	        else if(sum == 6){
	  	        hex[c] = '6';
	        }
	        else if(sum == 7){
	  	        hex[c] = '7';
	        }
	        else if(sum == 8){
	  	        hex[c] = '8';
	        }
	        else if(sum == 9){
	  	        hex[c] = '9';
	        }
	        else if(sum == 10){
	  	        hex[c] = 'A';
	        }
	        else if(sum == 11){
	  	        hex[c] = 'B';
	        }
	        else if(sum == 12){
	  	        hex[c] = 'C';
	        }
	        else if(sum == 13){
	  	        hex[c] = 'D';
	        }
	        else if(sum == 14){
	  	        hex[c] = 'E';
	        }
	        else if(sum == 15){
	  	        hex[c] = 'F';
	        }
	        sum = 0;
	        l = 0;
	        c++;
            }
			
			fprintf(file1,"%c%c%c%c\n",hex[3],hex[2],hex[1],hex[0]);
			
			currentstatementbunmber++;
			continue;
			
		}
		
		else if(strstr(insname, "MOV") || strstr(insname, "NOT")){
			
			strcpy(opcode,"0111");
			
			if(strstr(insname, "MOV")){
				strcpy(alucode,"111");
				strcpy(alucodeextra,"001");
			}
			if(strstr(insname, "NOT")){
				strcpy(alucode,"111");
				strcpy(alucodeextra,"000");
			}
			fscanf(fp,"%s",&dreg);
			fscanf(fp,"%s",&s1reg);
			
			char bdreg[3];
			strcpy(bdreg,convertregistertobinary(dreg));
			char bs1reg[3];
			strcpy(bs1reg,convertregistertobinary(s1reg));
			
			//printf("%s %s %s -> ",insname,bdreg,bs1reg);
			strcpy(binary,strcat(opcode,strcat(alucode,strcat(alucodeextra,strcat(bs1reg,bdreg)))));
			//printf("%s -> \n",binary);
			
			

			int newbinary[16];
			int hg;
			for(hg = 0;hg < sizeof(binary);hg++){
				newbinary[hg] = binary[hg] - 48;
			}
			
			char hex[4];
			
			
			int k,m,c = 0,l = 0,sum = 0;
            for(k = 4;k > 0;k--){
  	            for(m = (k * 4)- 1 ;m >= (k*4) - 4;m--){
  		            sum += pow(2,l)* newbinary[m];
  		            l++;
	            }
	        if(sum == 0){
	  	        hex[c] = '0';
	        }
	        else if(sum == 1){
	  	        hex[c] = '1';
	        }
	        else if(sum == 2){
	  	        hex[c] = '2';
	        }
	        else if(sum == 3){
	  	        hex[c] = '3';
	        }
	        else if(sum == 4){
	  	        hex[c] = '4';
	        }
	        else if(sum == 5){
	  	        hex[c] = '5';
	        }
	        else if(sum == 6){
	  	        hex[c] = '6';
	        }
	        else if(sum == 7){
	  	        hex[c] = '7';
	        }
	        else if(sum == 8){
	  	        hex[c] = '8';
	        }
	        else if(sum == 9){
	  	        hex[c] = '9';
	        }
	        else if(sum == 10){
	  	        hex[c] = 'A';
	        }
	        else if(sum == 11){
	  	        hex[c] = 'B';
	        }
	        else if(sum == 12){
	  	        hex[c] = 'C';
	        }
	        else if(sum == 13){
	  	        hex[c] = 'D';
	        }
	        else if(sum == 14){
	  	        hex[c] = 'E';
	        }
	        else if(sum == 15){
	  	        hex[c] = 'F';
	        }
	        sum = 0;
	        l = 0;
	        c++;
            }
			
			fprintf(file1,"%c%c%c%c\n",hex[3],hex[2],hex[1],hex[0]);
			
			currentstatementbunmber++;
			continue;
			
		}
		
		else if(strstr(insname, "RET") || strstr(insname, "IRET") || strstr(insname, "STI") || strstr(insname, "CLI")){
			
			if(strstr(insname, "IRET")){
				strcpy(opcode,"1110");
			}
			else if(strstr(insname, "RET")){
				strcpy(opcode,"1011");
			}
			else if(strstr(insname, "STI")){
				strcpy(opcode,"1100");
			}
			else if(strstr(insname, "CLI")){
				strcpy(opcode,"1101");
			}
			
			char nul[] = "000000000000";
			
			//printf("%s %s -> ",insname,nul);
			strcpy(binary,strcat(opcode,nul));
			//printf("%s -> \n",binary);
			
			

			int newbinary[16];
			int hg;
			for(hg = 0;hg < sizeof(binary);hg++){
				newbinary[hg] = binary[hg] - 48;
			}
			
			char hex[4];
			
			
			int k,m,c = 0,l = 0,sum = 0;
            for(k = 4;k > 0;k--){
  	            for(m = (k * 4)- 1 ;m >= (k*4) - 4;m--){
  		            sum += pow(2,l)* newbinary[m];
  		            l++;
	            }
	        if(sum == 0){
	  	        hex[c] = '0';
	        }
	        else if(sum == 1){
	  	        hex[c] = '1';
	        }
	        else if(sum == 2){
	  	        hex[c] = '2';
	        }
	        else if(sum == 3){
	  	        hex[c] = '3';
	        }
	        else if(sum == 4){
	  	        hex[c] = '4';
	        }
	        else if(sum == 5){
	  	        hex[c] = '5';
	        }
	        else if(sum == 6){
	  	        hex[c] = '6';
	        }
	        else if(sum == 7){
	  	        hex[c] = '7';
	        }
	        else if(sum == 8){
	  	        hex[c] = '8';
	        }
	        else if(sum == 9){
	  	        hex[c] = '9';
	        }
	        else if(sum == 10){
	  	        hex[c] = 'A';
	        }
	        else if(sum == 11){
	  	        hex[c] = 'B';
	        }
	        else if(sum == 12){
	  	        hex[c] = 'C';
	        }
	        else if(sum == 13){
	  	        hex[c] = 'D';
	        }
	        else if(sum == 14){
	  	        hex[c] = 'E';
	        }
	        else if(sum == 15){
	  	        hex[c] = 'F';
	        }
	        sum = 0;
	        l = 0;
	        c++;
            }
			
			fprintf(file1,"%c%c%c%c\n",hex[3],hex[2],hex[1],hex[0]);
			
			currentstatementbunmber++;
			continue;
			
		}
		
		else if(strstr(insname, "LDI")){
			
			if(strstr(insname, "LDI")){
				strcpy(opcode,"0001");
			}
			
			char nul[] = "000000000";
			char temp[20];
			
			fscanf(fp,"%s",&dreg);
			fscanf(fp,"%s",&temp);
			
			if(isdigit(temp[0])){
				DATA = atoi(temp);
			}
			else{
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			    char firstWord1[20]; 
			    char modifiedFirstWord1[20];
			    int linenumber = 0;
			
			    FILE *fp5;
                fp5 = fopen("input.txt", "rt");
            
                while(fscanf(fp5, "%s", firstWord1) == 1){
            	
            	    int len1 = strlen(firstWord1);
                    if (len1 > 0 && firstWord1[len1 - 1] == ':'){
                        strncpy(modifiedFirstWord1, firstWord1, len1 - 1);
                        modifiedFirstWord1[len1 - 1] = '\0'; 
                    } 
				    else{
                        strcpy(modifiedFirstWord1, firstWord1);
                    }
            	
                    if (strcmp(modifiedFirstWord1, temp) == 0){
                        printf("LDI string %d satirda bulundu.\n",linenumber);
                        DATA = linenumber;
                        break;
                    }
                
                    int ch1;
                    while ((ch1 = fgetc(fp5)) != '\n' && ch1 != EOF);
                    linenumber++;
                }
                
            	int x;
            	for(x = 0;x < linenumber;x++){
            		if(loadiline[x] == 1){
            			DATA++;
					}
					if(labelline[x] == 1){
            			DATA--;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				printf("LDI adress : %d\n",DATA);
			}
			
			char bdreg[3];
			strcpy(bdreg,convertregistertobinary(dreg));
			
			//printf("%s %s -> ",insname,bdreg);
			strcpy(binary,strcat(opcode,strcat(nul,bdreg)));
			//printf("%s -> \n",binary);
			
			char binary32bit[32];
			
			char bDATA[16];
	        strcpy(binary32bit,strcat(binary,twoscomp16(DATA)));
	        
	        //printf("\n\n*** %s ***\n\n",binary32bit);
			
			int newbinary[32];
			int hg;
			for(hg = 0;hg < sizeof(binary32bit);hg++){
				newbinary[hg] = binary32bit[hg] - 48;
			}
			
			char hex[8];
			
			
			int k,m,c = 0,l = 0,sum = 0;
            for(k = 8;k > 0;k--){
  	            for(m = (k * 4)- 1 ;m >= (k*4) - 4;m--){
  		            sum += pow(2,l)* newbinary[m];
  		            l++;
	            }
	        if(sum == 0){
	  	        hex[c] = '0';
	        }
	        else if(sum == 1){
	  	        hex[c] = '1';
	        }
	        else if(sum == 2){
	  	        hex[c] = '2';
	        }
	        else if(sum == 3){
	  	        hex[c] = '3';
	        }
	        else if(sum == 4){
	  	        hex[c] = '4';
	        }
	        else if(sum == 5){
	  	        hex[c] = '5';
	        }
	        else if(sum == 6){
	  	        hex[c] = '6';
	        }
	        else if(sum == 7){
	  	        hex[c] = '7';
	        }
	        else if(sum == 8){
	  	        hex[c] = '8';
	        }
	        else if(sum == 9){
	  	        hex[c] = '9';
	        }
	        else if(sum == 10){
	  	        hex[c] = 'A';
	        }
	        else if(sum == 11){
	  	        hex[c] = 'B';
	        }
	        else if(sum == 12){
	  	        hex[c] = 'C';
	        }
	        else if(sum == 13){
	  	        hex[c] = 'D';
	        }
	        else if(sum == 14){
	  	        hex[c] = 'E';
	        }
	        else if(sum == 15){
	  	        hex[c] = 'F';
	        }
	        sum = 0;
	        l = 0;
	        c++;
            }
			
			fprintf(file1,"%c%c%c%c\n",hex[7],hex[6],hex[5],hex[4]);
			fprintf(file1,"%c%c%c%c\n",hex[3],hex[2],hex[1],hex[0]);	        
			
			currentstatementbunmber++;
			continue;
			
			
		}
		
		else if(strstr(insname, "LD")){
			
			strcpy(opcode,"0010");
			
			char nul[] = "000000";
			
			fscanf(fp,"%s",&dreg);
			fscanf(fp,"%s",&s1reg);
			
			char bdreg[3];
			strcpy(bdreg,convertregistertobinary(dreg));
			char bs1reg[3];
			strcpy(bs1reg,convertregistertobinary(s1reg));
			
			//printf("%s %s %s -> ",insname,bdreg,bs1reg);
			strcpy(binary,strcat(opcode,strcat(nul,strcat(bs1reg,bdreg))));
			//printf("%s -> \n",binary);
			
			

			int newbinary[16];
			int hg;
			for(hg = 0;hg < sizeof(binary);hg++){
				newbinary[hg] = binary[hg] - 48;
			}
			
			char hex[4];
			
			
			int k,m,c = 0,l = 0,sum = 0;
            for(k = 4;k > 0;k--){
  	            for(m = (k * 4)- 1 ;m >= (k*4) - 4;m--){
  		            sum += pow(2,l)* newbinary[m];
  		            l++;
	            }
	        if(sum == 0){
	  	        hex[c] = '0';
	        }
	        else if(sum == 1){
	  	        hex[c] = '1';
	        }
	        else if(sum == 2){
	  	        hex[c] = '2';
	        }
	        else if(sum == 3){
	  	        hex[c] = '3';
	        }
	        else if(sum == 4){
	  	        hex[c] = '4';
	        }
	        else if(sum == 5){
	  	        hex[c] = '5';
	        }
	        else if(sum == 6){
	  	        hex[c] = '6';
	        }
	        else if(sum == 7){
	  	        hex[c] = '7';
	        }
	        else if(sum == 8){
	  	        hex[c] = '8';
	        }
	        else if(sum == 9){
	  	        hex[c] = '9';
	        }
	        else if(sum == 10){
	  	        hex[c] = 'A';
	        }
	        else if(sum == 11){
	  	        hex[c] = 'B';
	        }
	        else if(sum == 12){
	  	        hex[c] = 'C';
	        }
	        else if(sum == 13){
	  	        hex[c] = 'D';
	        }
	        else if(sum == 14){
	  	        hex[c] = 'E';
	        }
	        else if(sum == 15){
	  	        hex[c] = 'F';
	        }
	        sum = 0;
	        l = 0;
	        c++;
            }
			
			fprintf(file1,"%c%c%c%c\n",hex[3],hex[2],hex[1],hex[0]);
			
			currentstatementbunmber++;
			continue;
			
		}
		
		else if(strstr(insname, "ST")){
			
			strcpy(opcode,"0011");
			
			char nul[] = "000";
			
			fscanf(fp,"%s",&dreg);
			fscanf(fp,"%s",&s1reg);
			
			char bdreg[3];
			strcpy(bdreg,convertregistertobinary(dreg));
			char bs1reg[3];
			strcpy(bs1reg,convertregistertobinary(s1reg));
			
			//printf("%s %s %s -> ",insname,bdreg,bs1reg);
			strcpy(binary,strcat(opcode,strcat(nul,strcat(bs1reg,strcat(bdreg,nul)))));
			//printf("%s -> \n",binary);
			
			

			int newbinary[16];
			int hg;
			for(hg = 0;hg < sizeof(binary);hg++){
				newbinary[hg] = binary[hg] - 48;
			}
			
			char hex[4];
			
			
			int k,m,c = 0,l = 0,sum = 0;
            for(k = 4;k > 0;k--){
  	            for(m = (k * 4)- 1 ;m >= (k*4) - 4;m--){
  		            sum += pow(2,l)* newbinary[m];
  		            l++;
	            }
	        if(sum == 0){
	  	        hex[c] = '0';
	        }
	        else if(sum == 1){
	  	        hex[c] = '1';
	        }
	        else if(sum == 2){
	  	        hex[c] = '2';
	        }
	        else if(sum == 3){
	  	        hex[c] = '3';
	        }
	        else if(sum == 4){
	  	        hex[c] = '4';
	        }
	        else if(sum == 5){
	  	        hex[c] = '5';
	        }
	        else if(sum == 6){
	  	        hex[c] = '6';
	        }
	        else if(sum == 7){
	  	        hex[c] = '7';
	        }
	        else if(sum == 8){
	  	        hex[c] = '8';
	        }
	        else if(sum == 9){
	  	        hex[c] = '9';
	        }
	        else if(sum == 10){
	  	        hex[c] = 'A';
	        }
	        else if(sum == 11){
	  	        hex[c] = 'B';
	        }
	        else if(sum == 12){
	  	        hex[c] = 'C';
	        }
	        else if(sum == 13){
	  	        hex[c] = 'D';
	        }
	        else if(sum == 14){
	  	        hex[c] = 'E';
	        }
	        else if(sum == 15){
	  	        hex[c] = 'F';
	        }
	        sum = 0;
	        l = 0;
	        c++;
            }
			
			fprintf(file1,"%c%c%c%c\n",hex[3],hex[2],hex[1],hex[0]);
			
			currentstatementbunmber++;
			continue;
			
		}
		
		else if(strstr(insname, "JMP") || strstr(insname, "JZ") || strstr(insname, "CALL")){
			
			if(strstr(insname, "JMP")){
				strcpy(opcode,"0101");
			}
			else if(strstr(insname, "JZ")){
				strcpy(opcode,"0100");
			}
			else if(strstr(insname, "CALL")){
				strcpy(opcode,"1010");
			}
			
			char label[20];//////////////////////////////////////////////////////////////
			char firstWord[20];
			char modifiedFirstWord[20];
			int line = 0;
			fscanf(fp,"%s",&label);
			
			FILE *fp1;
            fp1 = fopen("input.txt", "rt");
            
            while (fscanf(fp1, "%s", firstWord) == 1){
            	
            	int len = strlen(firstWord);
                if (len > 0 && firstWord[len - 1] == ':'){
                    strncpy(modifiedFirstWord, firstWord, len - 1);
                    modifiedFirstWord[len - 1] = '\0'; 
                } 
				else{
                    strcpy(modifiedFirstWord, firstWord);
                }
            	
                if (strcmp(modifiedFirstWord, label) == 0){
                    printf("'%s' kelimesi %d. satirin basinda bulundu. *%d*\n", label, line,currentstatementbunmber);
                    //OFFSET = (currentstatementbunmber > line) ? (line - currentstatementbunmber - 1) : (currentstatementbunmber - line - 1);
                    //OFFSET = (line - currentstatementbunmber - 1);//currentstatementbunmber2
                    if(line > currentstatementbunmber){
                    	OFFSET = (line - currentstatementbunmber - 1);
					}
					else{
						OFFSET = (line - currentstatementbunmber);
					}
                    
                    break;
                }
                
                int ch;
                while ((ch = fgetc(fp1)) != '\n' && ch != EOF);
                line++;
            }
            //printf("Real Line : %d\n OFFSET : %d\n",realline,OFFSET);
            if(line > currentstatementbunmber){
            	int x;
            	for(x = currentstatementbunmber + 1;x < line;x++){
            		if(loadiline[x] == 1){
            			OFFSET++;
            			printf("x : %d ",x);
					}
					if(labelline[x] == 1){
            			OFFSET--;
            			printf("x : %d ",x);
					}
				}
			}
			else{
				int y;
            	for(y = line + 1;y < currentstatementbunmber;y++){
            		if(loadiline[y] == 1){
            			OFFSET--;
            			printf("y : %d ",y);
					}
					if(labelline[y] == 1){
            			OFFSET++;
            			printf("y : %d ",y);
					}
				}
			}
			
			char bOFFSET[12];
	        strcpy(bOFFSET,twoscomp12(OFFSET));
			

			printf("%s %d -> \n*********************\n",insname,OFFSET);
			strcpy(binary,strcat(opcode,bOFFSET));
			//printf("%s -> \n",binary);
			
			
			int newbinary[16];
			int hg;
			for(hg = 0;hg < sizeof(binary);hg++){
				newbinary[hg] = binary[hg] - 48;
			}
			
			char hex[4];
			
			
			int k,m,c = 0,l = 0,sum = 0;
            for(k = 4;k > 0;k--){
  	            for(m = (k * 4)- 1 ;m >= (k*4) - 4;m--){
  		            sum += pow(2,l)* newbinary[m];
  		            l++;
	            }
	        if(sum == 0){
	  	        hex[c] = '0';
	        }
	        else if(sum == 1){
	  	        hex[c] = '1';
	        }
	        else if(sum == 2){
	  	        hex[c] = '2';
	        }
	        else if(sum == 3){
	  	        hex[c] = '3';
	        }
	        else if(sum == 4){
	  	        hex[c] = '4';
	        }
	        else if(sum == 5){
	  	        hex[c] = '5';
	        }
	        else if(sum == 6){
	  	        hex[c] = '6';
	        }
	        else if(sum == 7){
	  	        hex[c] = '7';
	        }
	        else if(sum == 8){
	  	        hex[c] = '8';
	        }
	        else if(sum == 9){
	  	        hex[c] = '9';
	        }
	        else if(sum == 10){
	  	        hex[c] = 'A';
	        }
	        else if(sum == 11){
	  	        hex[c] = 'B';
	        }
	        else if(sum == 12){
	  	        hex[c] = 'C';
	        }
	        else if(sum == 13){
	  	        hex[c] = 'D';
	        }
	        else if(sum == 14){
	  	        hex[c] = 'E';
	        }
	        else if(sum == 15){
	  	        hex[c] = 'F';
	        }
	        sum = 0;
	        l = 0;
	        c++;
            }
			
			fprintf(file1,"%c%c%c%c\n",hex[3],hex[2],hex[1],hex[0]);
			
			currentstatementbunmber++;
			continue;
			
			
		}
		
		else if(strstr(insname, "PUSH")){
			
			strcpy(opcode,"1000");
			
			char nul[] = "000";
			char nul1[] = "000000";
			
			fscanf(fp,"%s",&s1reg);
			
			char bs1reg[3];
			strcpy(bs1reg,convertregistertobinary(s1reg));
			
			//printf("%s %s -> ",insname,bs1reg);
			strcpy(binary,strcat(opcode,strcat(nul,strcat(bs1reg,nul1))));
			//printf("%s -> \n",binary);
			
			

			int newbinary[16];
			int hg;
			for(hg = 0;hg < sizeof(binary);hg++){
				newbinary[hg] = binary[hg] - 48;
			}
			
			char hex[4];
			
			
			int k,m,c = 0,l = 0,sum = 0;
            for(k = 4;k > 0;k--){
  	            for(m = (k * 4)- 1 ;m >= (k*4) - 4;m--){
  		            sum += pow(2,l)* newbinary[m];
  		            l++;
	            }
	        if(sum == 0){
	  	        hex[c] = '0';
	        }
	        else if(sum == 1){
	  	        hex[c] = '1';
	        }
	        else if(sum == 2){
	  	        hex[c] = '2';
	        }
	        else if(sum == 3){
	  	        hex[c] = '3';
	        }
	        else if(sum == 4){
	  	        hex[c] = '4';
	        }
	        else if(sum == 5){
	  	        hex[c] = '5';
	        }
	        else if(sum == 6){
	  	        hex[c] = '6';
	        }
	        else if(sum == 7){
	  	        hex[c] = '7';
	        }
	        else if(sum == 8){
	  	        hex[c] = '8';
	        }
	        else if(sum == 9){
	  	        hex[c] = '9';
	        }
	        else if(sum == 10){
	  	        hex[c] = 'A';
	        }
	        else if(sum == 11){
	  	        hex[c] = 'B';
	        }
	        else if(sum == 12){
	  	        hex[c] = 'C';
	        }
	        else if(sum == 13){
	  	        hex[c] = 'D';
	        }
	        else if(sum == 14){
	  	        hex[c] = 'E';
	        }
	        else if(sum == 15){
	  	        hex[c] = 'F';
	        }
	        sum = 0;
	        l = 0;
	        c++;
            }
			
			fprintf(file1,"%c%c%c%c\n",hex[3],hex[2],hex[1],hex[0]);
			
			currentstatementbunmber++;
			continue;
			
		}
		
		else if(strstr(insname, "POP")){
			
			strcpy(opcode,"1001");
			
			char nul[] = "000000000";
			
			fscanf(fp,"%s",&dreg);
			
			char bdreg[3];
			strcpy(bdreg,convertregistertobinary(dreg));
			
			//printf("%s %s -> ",insname,bdreg);
			strcpy(binary,strcat(opcode,strcat(nul,bdreg)));
			//printf("%s -> \n",binary);
			
			

			int newbinary[16];
			int hg;
			for(hg = 0;hg < sizeof(binary);hg++){
				newbinary[hg] = binary[hg] - 48;
			}
			
			char hex[4];
			
			
			int k,m,c = 0,l = 0,sum = 0;
            for(k = 4;k > 0;k--){
  	            for(m = (k * 4)- 1 ;m >= (k*4) - 4;m--){
  		            sum += pow(2,l)* newbinary[m];
  		            l++;
	            }
	        if(sum == 0){
	  	        hex[c] = '0';
	        }
	        else if(sum == 1){
	  	        hex[c] = '1';
	        }
	        else if(sum == 2){
	  	        hex[c] = '2';
	        }
	        else if(sum == 3){
	  	        hex[c] = '3';
	        }
	        else if(sum == 4){
	  	        hex[c] = '4';
	        }
	        else if(sum == 5){
	  	        hex[c] = '5';
	        }
	        else if(sum == 6){
	  	        hex[c] = '6';
	        }
	        else if(sum == 7){
	  	        hex[c] = '7';
	        }
	        else if(sum == 8){
	  	        hex[c] = '8';
	        }
	        else if(sum == 9){
	  	        hex[c] = '9';
	        }
	        else if(sum == 10){
	  	        hex[c] = 'A';
	        }
	        else if(sum == 11){
	  	        hex[c] = 'B';
	        }
	        else if(sum == 12){
	  	        hex[c] = 'C';
	        }
	        else if(sum == 13){
	  	        hex[c] = 'D';
	        }
	        else if(sum == 14){
	  	        hex[c] = 'E';
	        }
	        else if(sum == 15){
	  	        hex[c] = 'F';
	        }
	        sum = 0;
	        l = 0;
	        c++;
            }
			
			fprintf(file1,"%c%c%c%c\n",hex[3],hex[2],hex[1],hex[0]);
			
			currentstatementbunmber++;
			continue;
			
		}
		
		else{
			currentstatementbunmber++;
			continue;
		}
		
	}
	
	
	fclose(fp);
	//fclose(outputname);
	
	return 0;
}
