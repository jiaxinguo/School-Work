/*
    Remove all unnecessary lines (including this one) 
    in this comment.
    REFER TO THE SUBMISSION INSTRUCTION FOR DETAILS

    Name 1: Full name of the first partner 
    Name 2: Full name of the second partner
    UTEID 1: UT EID of the first partner
    UTEID 2: UT EID of the second partner
*/

/***************************************************************/
/*                                                             */
/*   LC-3b Instruction Level Simulator                         */
/*                                                             */
/*   EE 460N                                                   */
/*   The University of Texas at Austin                         */
/*                                                             */
/***************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************/
/*                                                             */
/* Files: isaprogram   LC-3b machine language program file     */
/*                                                             */
/***************************************************************/

/***************************************************************/
/* These are the functions you'll have to write.               */
/***************************************************************/

void process_instruction();

/***************************************************************/
/* A couple of useful definitions.                             */
/***************************************************************/
#define FALSE 0
#define TRUE  1

/***************************************************************/
/* Use this to avoid overflowing 16 bits on the bus.           */
/***************************************************************/
#define Low16bits(x) ((x) & 0xFFFF)

/***************************************************************/
/* Main memory.                                                */
/***************************************************************/
/* MEMORY[A][0] stores the least significant byte of word at word address A
   MEMORY[A][1] stores the most significant byte of word at word address A 
*/

#define WORDS_IN_MEM    0x08000 
int MEMORY[WORDS_IN_MEM][2];

/***************************************************************/

/***************************************************************/

/***************************************************************/
/* LC-3b State info.                                           */
/***************************************************************/
#define LC_3b_REGS 8

int RUN_BIT;	/* run bit */


typedef struct System_Latches_Struct{

  int PC,		/* program counter */
    N,		/* n condition bit */
    Z,		/* z condition bit */
    P;		/* p condition bit */
  int REGS[LC_3b_REGS]; /* register file. */
} System_Latches;

/* Data Structure for Latch */

System_Latches CURRENT_LATCHES, NEXT_LATCHES;

/***************************************************************/
/* A cycle counter.                                            */
/***************************************************************/
int INSTRUCTION_COUNT;

/***************************************************************/
/*                                                             */
/* Procedure : help                                            */
/*                                                             */
/* Purpose   : Print out a list of commands                    */
/*                                                             */
/***************************************************************/
void help() {                                                    
  printf("----------------LC-3b ISIM Help-----------------------\n");
  printf("go               -  run program to completion         \n");
  printf("run n            -  execute program for n instructions\n");
  printf("mdump low high   -  dump memory from low to high      \n");
  printf("rdump            -  dump the register & bus values    \n");
  printf("?                -  display this help menu            \n");
  printf("quit             -  exit the program                  \n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : cycle                                           */
/*                                                             */
/* Purpose   : Execute a cycle                                 */
/*                                                             */
/***************************************************************/
void cycle() {                                                

  process_instruction();
  CURRENT_LATCHES = NEXT_LATCHES;
  INSTRUCTION_COUNT++;
}

/***************************************************************/
/*                                                             */
/* Procedure : run n                                           */
/*                                                             */
/* Purpose   : Simulate the LC-3b for n cycles                 */
/*                                                             */
/***************************************************************/
void run(int num_cycles) {                                      
  int i;

  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating for %d cycles...\n\n", num_cycles);
  for (i = 0; i < num_cycles; i++) {
    if (CURRENT_LATCHES.PC == 0x0000) {
	    RUN_BIT = FALSE;
	    printf("Simulator halted\n\n");
	    break;
    }
    cycle();
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : go                                              */
/*                                                             */
/* Purpose   : Simulate the LC-3b until HALTed                 */
/*                                                             */
/***************************************************************/
void go() {                                                     
  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating...\n\n");
  while (CURRENT_LATCHES.PC != 0x0000)
    cycle();
  RUN_BIT = FALSE;
  printf("Simulator halted\n\n");
}

/***************************************************************/ 
/*                                                             */
/* Procedure : mdump                                           */
/*                                                             */
/* Purpose   : Dump a word-aligned region of memory to the     */
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void mdump(FILE * dumpsim_file, int start, int stop) {          
  int address; /* this is a byte address */

  printf("\nMemory content [0x%0.4x..0x%0.4x] :\n", start, stop);
  printf("-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    printf("  0x%0.4x (%d) : 0x%0.2x%0.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  printf("\n");

  /* dump the memory contents into the dumpsim file */
  fprintf(dumpsim_file, "\nMemory content [0x%0.4x..0x%0.4x] :\n", start, stop);
  fprintf(dumpsim_file, "-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    fprintf(dumpsim_file, " 0x%0.4x (%d) : 0x%0.2x%0.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  fprintf(dumpsim_file, "\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : rdump                                           */
/*                                                             */
/* Purpose   : Dump current register and bus values to the     */   
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void rdump(FILE * dumpsim_file) {                               
  int k; 

  printf("\nCurrent register/bus values :\n");
  printf("-------------------------------------\n");
  printf("Instruction Count : %d\n", INSTRUCTION_COUNT);
  printf("PC                : 0x%0.4x\n", CURRENT_LATCHES.PC);
  printf("CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  printf("Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    printf("%d: 0x%0.4x\n", k, CURRENT_LATCHES.REGS[k]);
  printf("\n");

  /* dump the state information into the dumpsim file */
  fprintf(dumpsim_file, "\nCurrent register/bus values :\n");
  fprintf(dumpsim_file, "-------------------------------------\n");
  fprintf(dumpsim_file, "Instruction Count : %d\n", INSTRUCTION_COUNT);
  fprintf(dumpsim_file, "PC                : 0x%0.4x\n", CURRENT_LATCHES.PC);
  fprintf(dumpsim_file, "CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  fprintf(dumpsim_file, "Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    fprintf(dumpsim_file, "%d: 0x%0.4x\n", k, CURRENT_LATCHES.REGS[k]);
  fprintf(dumpsim_file, "\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : get_command                                     */
/*                                                             */
/* Purpose   : Read a command from standard input.             */  
/*                                                             */
/***************************************************************/
void get_command(FILE * dumpsim_file) {                         
  char buffer[20];
  int start, stop, cycles;

  printf("LC-3b-SIM> ");

  scanf("%s", buffer);
  printf("\n");

  switch(buffer[0]) {
  case 'G':
  case 'g':
    go();
    break;

  case 'M':
  case 'm':
    scanf("%i %i", &start, &stop);
    mdump(dumpsim_file, start, stop);
    break;

  case '?':
    help();
    break;
  case 'Q':
  case 'q':
    printf("Bye.\n");
    exit(0);

  case 'R':
  case 'r':
    if (buffer[1] == 'd' || buffer[1] == 'D')
	    rdump(dumpsim_file);
    else {
	    scanf("%d", &cycles);
	    run(cycles);
    }
    break;

  default:
    printf("Invalid Command\n");
    break;
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : init_memory                                     */
/*                                                             */
/* Purpose   : Zero out the memory array                       */
/*                                                             */
/***************************************************************/
void init_memory() {                                           
  int i;

  for (i=0; i < WORDS_IN_MEM; i++) {
    MEMORY[i][0] = 0;
    MEMORY[i][1] = 0;
  }
}

/**************************************************************/
/*                                                            */
/* Procedure : load_program                                   */
/*                                                            */
/* Purpose   : Load program and service routines into mem.    */
/*                                                            */
/**************************************************************/
void load_program(char *program_filename) {                   
  FILE * prog;
  int ii, word, program_base;

  /* Open program file. */
  prog = fopen(program_filename, "r");
  if (prog == NULL) {
    printf("Error: Can't open program file %s\n", program_filename);
    exit(-1);
  }

  /* Read in the program. */
  if (fscanf(prog, "%x\n", &word) != EOF)
    program_base = word >> 1;
  else {
    printf("Error: Program file is empty\n");
    exit(-1);
  }

  ii = 0;
  while (fscanf(prog, "%x\n", &word) != EOF) {
    /* Make sure it fits. */
    if (program_base + ii >= WORDS_IN_MEM) {
	    printf("Error: Program file %s is too long to fit in memory. %x\n",
             program_filename, ii);
	    exit(-1);
    }

    /* Write the word to memory array. */
    MEMORY[program_base + ii][0] = word & 0x00FF;
    MEMORY[program_base + ii][1] = (word >> 8) & 0x00FF;
    ii++;
  }

  if (CURRENT_LATCHES.PC == 0) CURRENT_LATCHES.PC = (program_base << 1);

  printf("Read %d words from program into memory.\n\n", ii);
}

/************************************************************/
/*                                                          */
/* Procedure : initialize                                   */
/*                                                          */
/* Purpose   : Load machine language program                */ 
/*             and set up initial state of the machine.     */
/*                                                          */
/************************************************************/
void initialize(char *program_filename, int num_prog_files) { 
  int i;

  init_memory();
  for ( i = 0; i < num_prog_files; i++ ) {
    load_program(program_filename);
    while(*program_filename++ != '\0');
  }
  CURRENT_LATCHES.Z = 1;  
  NEXT_LATCHES = CURRENT_LATCHES;
    
  RUN_BIT = TRUE;
}

/***************************************************************/
/*                                                             */
/* Procedure : main                                            */
/*                                                             */
/***************************************************************/
int main(int argc, char *argv[]) {                              
  FILE * dumpsim_file;

  /* Error Checking */
  if (argc < 2) {
    printf("Error: usage: %s <program_file_1> <program_file_2> ...\n",
           argv[0]);
    exit(1);
  }

  printf("LC-3b Simulator\n\n");

  initialize(argv[1], argc - 1);

  if ( (dumpsim_file = fopen( "dumpsim", "w" )) == NULL ) {
    printf("Error: Can't open dumpsim file\n");
    exit(-1);
  }

  while (1)
    get_command(dumpsim_file);
    
}

/***************************************************************/
/* Do not modify the above code.
   You are allowed to use the following global variables in your
   code. These are defined above.

   MEMORY

   CURRENT_LATCHES
   NEXT_LATCHES

   You may define your own local/global variables and functions.
   You may use the functions to get at the control bits defined
   above.

   Begin your code here 	  			       */

/***************************************************************/



void process_instruction(){
  /*  function: process_instruction
   *  
   *    Process one instruction at a time  
   *       -Fetch one instruction
   *       -Decode 
   *       -Execute
   *       -Update NEXT_LATCHES
   */   
	NEXT_LATCHES = CURRENT_LATCHES;	//FIX SIGN EXTEND
	int DR_MASK = 0x0E00;
	int SR1_MASK = 0x01C0;
	int IMM5_MASK = 0x001F;
	int PCOFFSET11_MASK = 0x07FF;
	int BOFFSET6SEXT = 0xFFC0;
	int PCOFFSET11SEXT = 0xF800;
	int IMM5SEXT = 0xFFE0;
	int BYTESEXT = 0xFF00;
	int PCOFFSET9SEXT = 0xFE00;
	int instruction = low16bits((MEMORY[CURRENT_LATCHES.PC][0] << 8) | MEMORY[CURRENT_LATCHES.PC][1]);
	switch (instruction >> 12)
	{
	case 1:	//ADD
		if (((instruction >> 5) % 2) == 1)
		{
			int DR = (instruction & DR_MASK) >> 9;
			int SR1 = (instruction & SR1_MASK) >> 6;
			int imm5 = instruction & IMM5_MASK;
			if (imm5 >> 4)
			{
				imm5 = imm5 | IMM5SEXT;
			}
			NEXT_LATCHES.REGS[DR] = low16bits(CURRENT_LATCHES.REGS[SR1] + imm5);
		}
		else
		{
			int DR = (instruction & DR_MASK) >> 9;
			int SR1 = (instruction & SR1_MASK) >> 6;
			int SR2 = instruction & 0x7;
			NEXT_LATCHES.REGS[DR] = low16bits(CURRENT_LATCHES.REGS[SR1] + CURRENT_LATCHES.REGS[SR2]);
		}
		if (NEXT_LATCHES.REGS[DR] >> 15)
		{ 
			NEXT_LATCHES.P = FALSE;
			NEXT_LATCHES.N = TRUE;
			NEXT_LATCHES.Z = FALSE;
		}
		else if if ((NEXT_LATCHES.REGS[DR] >> 15 == 0) && (NEXT_LATCHES.REGS[DR] != 0))
		{
			NEXT_LATCHES.P = TRUE;
			NEXT_LATCHES.N = FALSE;
			NEXT_LATCHES.Z = FALSE;
		}
		else
		{
			NEXT_LATCHES.P = FALSE;
			NEXT_LATCHES.N = FALSE;
			NEXT_LATCHES.Z = TRUE;
		}
		break;
	case 2: //LDB
		int DR = (instruction & DR_MASK) >> 9;
		int baseR = (instruction & SR1_MASK) >> 6;
		int boffset6 = instruction & 0x3F;
		if (boffset6 >> 5)
		{
			boffset6 = boffset6 | BOFFSET6SEXT;
		}
		int addressToLoad = low16bits(CURRENT_LATCHES.REGS[baseR] + boffset6);
		if (addressToStore % 2 == 1)
		{
			addressToStore = addressToStore >> 1;
			NEXT_LATCHES.REGS[DR] = low16bits(MEMORY[addressToLoad][1]);
			if (NEXT_LATCHES.REGS[DR] >> 7)
			{
				NEXT_LATCHES.REGS[DR] = NEXT_LATCHES.REGS[DR] | BYTESEXT;
			}
		}
		else
		{
			addressToStore = addressToStore >> 1;
			NEXT_LATCHES.REGS[DR] = low16bits(MEMORY[addressToLoad][0]);
			if (NEXT_LATCHES.REGS[DR] >> 7)
			{
				NEXT_LATCHES.REGS[DR] = NEXT_LATCHES.REGS[DR] | BYTESEXT;
			}
		}
		if (NEXT_LATCHES.REGS[DR] >> 15)
		{
			NEXT_LATCHES.P = FALSE;
			NEXT_LATCHES.N = TRUE;
			NEXT_LATCHES.Z = FALSE;
		}
		else if ((NEXT_LATCHES.REGS[DR] >> 15 == 0) && (NEXT_LATCHES.REGS[DR] != 0))
		{
			NEXT_LATCHES.P = TRUE;
			NEXT_LATCHES.N = FALSE;
			NEXT_LATCHES.Z = FALSE;
		}
		else
		{
			NEXT_LATCHES.P = FALSE;
			NEXT_LATCHES.N = FALSE;
			NEXT_LATCHES.Z = TRUE;
		}
		break;
	case 3:	//STB
		int SR = (instruction & DR_MASK) >> 9;
		int toStore = CURRENT_LATCHES.REGS[SR] & 0xFF;
		int baseR = (instruction & SR1_MASK) >> 6;
		int boffset6 = instruction & 0x3F;
		if (boffset6 >> 5)
		{
			boffset6 = boffset6 | BOFFSET6SEXT;
		}
		int addressToStore = low16bits(boffset6 + CURRENT_LATCHES.REGS[baseR]);
		if (addressToStore % 2 == 1)
		{
			addressToStore = addressToStore >> 1;
			MEMORY[addressToStore][1] = toStore;
		}
		else
		{
			addressToStore = addressToStore >> 1;
			MEMORY[addressToStore][0] = toStore;
		}
		break;
	case 4:	//JSR/JSRR
		NEXT_LATCHES.REGS[7] = CURRENT_LATCHES.PC;
		if (((instruction >> 11) % 2) == 1) //add error check
		{
			int PCoffset11 = low16bits(instruction & PCOFFSET11_MASK);
			if (PCoffset11 >> 10)
			{
				PCoffset11 = PCoffset11 | PCOFFSET11SEXT;
			}
			PCoffset11 = PCoffset11 << 1;
			NEXT_LATCHES.PC = low16bits(CURRENT_LATCHES.PC + PCoffset11);
		}
		else
		{
			int baseR = (instruction & SR1_MASK) >> 6;
			NEXT_LATCHES.PC = low16bits(CURRENT_LATCHES.REGS(baseR);
		}
		break;
	case 5: //AND
		if (((instruction >> 5) % 2) == 1)
		{
			int DR = (instruction & DR_MASK) >> 9;
			int SR1 = (instruction & SR1_MASK) >> 6;
			int imm5 = instruction & IMM5_MASK;
			if (imm5 >> 4)
			{
				imm5 = imm5 | IMM5SEXT;
			}
			NEXT_LATCHES.REGS[DR] = low16bits(CURRENT_LATCHES.REGS[SR1] & imm5);
		}
		else
		{
			int DR = (instruction & DR_MASK) >> 9;
			int SR1 = (instruction & SR1_MASK) >> 6;
			int SR2 = instruction & 0x7;
			NEXT_LATCHES.REGS[DR] = low16bits(CURRENT_LATCHES.REGS[SR1] & CURRENT_LATCHES.REGS[SR2]);
		}
		if (NEXT_LATCHES.REGS[DR] >> 15)
		{
			NEXT_LATCHES.P = FALSE;
			NEXT_LATCHES.N = TRUE;
			NEXT_LATCHES.Z = FALSE;
		}
		else if ((NEXT_LATCHES.REGS[DR] >> 15 == 0) && (NEXT_LATCHES.REGS[DR] != 0))
		{
			NEXT_LATCHES.P = TRUE;
			NEXT_LATCHES.N = FALSE;
			NEXT_LATCHES.Z = FALSE;
		}
		else
		{
			NEXT_LATCHES.P = FALSE;
			NEXT_LATCHES.N = FALSE;
			NEXT_LATCHES.Z = TRUE;
		}
		break;
	case 6:	//LDW
		int DR = (instruction & DR_MASK) >> 9;
		int baseR = (instruction & SR1_MASK) >> 6;
		int offset6 = instruction & 0x3F;
		if (offset6 >> 5)
		{
			offset6 = offset6 | BOFFSET6SEXT;
		}
		int addressToLoad = low16bits(CURRENT_LATCHES.REGS[baseR] + (offset6 << 1));
		NEXT_LATCHES.REGS[DR] = low16bits(MEMORY[addressToLoad >> 1][1]);
		NEXT_LATCHES.REGS[DR] = NEXT_LATCHES.REGS[DR] << 8;
		NEXT_LATCHES.REGS[DR] = NEXT_LATCHES.REGS[DR] | (low16bits(MEMORY[addressToLoad >> 1][0]));
		if (NEXT_LATCHES.REGS[DR] >> 15)
		{
			NEXT_LATCHES.P = FALSE;
			NEXT_LATCHES.N = TRUE;
			NEXT_LATCHES.Z = FALSE;
		}
		else if ((NEXT_LATCHES.REGS[DR] >> 15 == 0) && (NEXT_LATCHES.REGS[DR] != 0))
		{
			NEXT_LATCHES.P = TRUE;
			NEXT_LATCHES.N = FALSE;
			NEXT_LATCHES.Z = FALSE;
		}
		else
		{
			NEXT_LATCHES.P = FALSE;
			NEXT_LATCHES.N = FALSE;
			NEXT_LATCHES.Z = TRUE;
		}
		break;
	case 7:	//STW
		int SR = (instruction & DR_MASK) >> 9;
		int toStore = CURRENT_LATCHES.REGS[SR] & 0xFF;
		int baseR = (instruction & SR1_MASK) >> 6;
		int	offset6 = instruction & 0x3F;
		if (offset6 >> 5)
		{
			offset6 = offset6 | BOFFSET6SEXT;
		}cacac
		int addressToStore = low16bits((offset6 << 1) + CURRENT_LATCHES.REGS[baseR]);
		if (addressToStore % 2 == 1)
		{
			MEMORY[addressToStore][1] = toStore & 0x00FF;
		}
		else
		{
			MEMORY[addressToStore][0] = toStore & 0x00FF;
		}
		break;
	case 8:	//RTI
		break;
	case 9:	//XOR/NOT
		if (((instruction >> 5) % 2) == 1)
		{
			int DR = (instruction & DR_MASK) >> 9;
			int SR1 = (instruction & SR1_MASK) >> 6;
			NEXT_LATCHES.REGS[DR] = low16bits(CURRENT_LATCHES.REGS[SR1] ^ (instruction & IMM5_MASK));
		}
		else
		{
			int DR = (instruction & DR_MASK) >> 9;
			int SR1 = (instruction & SR1_MASK) >> 6;
			int SR2 = instruction & 0x7;
			NEXT_LATCHES.REGS[DR] = low16bits(CURRENT_LATCHES.REGS[SR1] ^ CURRENT_LATCHES.REGS[SR2]);
		}
		if (NEXT_LATCHES.REGS[DR] >> 15)
		{
			NEXT_LATCHES.P = FALSE;
			NEXT_LATCHES.N = TRUE;
			NEXT_LATCHES.Z = FALSE;
		}
		else if ((NEXT_LATCHES.REGS[DR] >> 15 == 0) && (NEXT_LATCHES.REGS[DR] != 0))
		{
			NEXT_LATCHES.P = TRUE;
			NEXT_LATCHES.N = FALSE;
			NEXT_LATCHES.Z = FALSE;
		}
		else
		{
			NEXT_LATCHES.P = FALSE;
			NEXT_LATCHES.N = FALSE;
			NEXT_LATCHES.Z = TRUE;
		}
		break;
	case 10:	//reserved
		break;
	case 11:	//reserved
		break;
	case 12:	//JMP/RET
		int baseR = (instruction & SR1_MASK) >> 6;
		NEXT_LATCHES.PC = CURRENT_LATCHES.REGS[baseR];
		break;
	case 13:	//SHF
		int DR = (instruction & DR_MASK) >> 9;
		int SR = (instruction & SR1_MASK) >> 6;
		int ammount4 = instruction & 0x000F;
		int i;
		if (((instruction >> 4) % 2) == 0)
		{
			NEXT_LATCHES.REGS[DR] = CURRENT_LATCHES.REGS[SR] << ammount4;
		}
		else
		{
			if (((instruction >> 5) % 2) == 0)
			{
				NEXT_LATCHES.REGS[DR] = CURRENT_LATCHES.REGS[SR] >> ammount4;
			}
			else
			{
				if (((instruction >> 15) % 2) == 1)
				{
					for (i = 0; i < ammount4; i++)
					{
						NEXT_LATCHES.REGS[DR] = CURRENT_LATCHES.REGS[SR] >> 1;
						NEXT_LATCHES.REGS[DR] = NEXT_LATCHES.REGS[DR] | 0x8000;
					}
				}
				else
				{
					NEXT_LATCHES.REGS[DR] = CURRENT_LATCHES.REGS[SR] >> ammount4;
				}
			}
		}
		if (NEXT_LATCHES.REGS[DR] >> 15)
		{
			NEXT_LATCHES.P = FALSE;
			NEXT_LATCHES.N = TRUE;
			NEXT_LATCHES.Z = FALSE;
		}
		else if ((NEXT_LATCHES.REGS[DR] >> 15 == 0) && (NEXT_LATCHES.REGS[DR] != 0))
		{
			NEXT_LATCHES.P = TRUE;
			NEXT_LATCHES.N = FALSE;
			NEXT_LATCHES.Z = FALSE;
		}
		else
		{
			NEXT_LATCHES.P = FALSE;
			NEXT_LATCHES.N = FALSE;
			NEXT_LATCHES.Z = TRUE;
		}
		break;
	case 14:	//LEA
		int DR = (instruction & DR_MASK) >> 9;
		int PCoffset9 = instruction & 0x01FF;
		if (PCoffset9 >> 8)
		{
			PCoffset9 = PCoffset9 | PCOFFSET9SEXT;
		}
		NEXT_LATCHES.REGS[DR] = low16bits(CURRENT_LATCHES.PC + (PCoffset9 << 1));
		break;
	case 15:	//TRAP
		int trapvect8 = instruction & 0x00FF;
		NEXT_LATCHES.REGS[7] = CURRENT_LATCHES.PC;
		NEXT_LATCHES.PC = low16bits(MEMORY[trapvect << 1][1]);
		NEXT_LATCHES.PC = NEXT_LATCHES.PC << 8;
		NEXT_LATCHES.PC = NEXT_LATCHES.PC | (low16bits(MEMORY[trapvect << 1][0]));
		break;
	}
}
