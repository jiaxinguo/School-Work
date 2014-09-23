/*
Name 1: Jiaxin Guo
UTEID 1: jg46989
*/

#include <stdio.h>
#include <stdlib.h>
#define MAXLENGTHOFLABEL 21
#define MAX_LINE_LENGTH 255
#define MAXOUTPUTSTRINGLENGTH 7
#define PCOFFSET9WOSIGN 8
#define PCOFFSET11WOSIGN 10
#define BOFFSET6WOSIGN 5
#define OFFSET6WOSIGN 5
#define INT_MAX 2147483647
enum
{
	DONE, OK, EMPTY_LINE, BRNZP, BRNZ, BRZP, BRNP, BRN, BRZ, BRP, BR, NOTBRANCH
};

void printString(char* labelFinder, int i);
typedef struct node
{
	char label[MAXLENGTHOFLABEL];
	int address;
	struct Node* next;
}Node;
Node* createLinkedList(char* label, int address);
Node* addLinkedList(Node* head, char* label, int address);
Node* removeLinkedList(Node* head, char* label);
int sizeOfLinkedList(Node* head);
int isLinkedListEmpty(Node* head);
void printLinkedList(Node* head);
Node* isInLinkedList(Node* head, char* label);
void freeLinkedList(Node* head);


void totalOffsetToHexOffset6(int totalOffset, char* offset1, char* offset2);
void boffset6ToHex(int boffset6, char* offset1, char* offset2);
void totalOffsetToHexPC9(int totalOffset, int* offset1, int* offset2);
void totalOffsetToHexPC11(int totalOffset, int* offset1, int* offset2, int* offset3);
char intToCharHex(int toConvert);
int charToIntRegister(char toConvert);
int isPseudoOp(char** pOpcode);
int isAllAlphanumberic(char** pLabel);
int isSpecialCases(char** pLabel);	
int startWithNumber(char **labelFinder);
/*Node* createSymbolTable(FILE* input);*/
char* reallocLabelFinder(char* labelFinder, int i);
int readAndParse(FILE * pInfile, char * pLine, char ** pLabel, char
	** pOpcode, char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4
	);
int toNum(char * pStr);
int isOpcode(char* lPtr);

int main(int argc, char *argv[])
{
	if (argc == 3)
	{
		int createdLinkedList = 0;
		int numOfLabels = 0;
		int currentAddress = -1;
		FILE *input, *output;
		char* pLine = malloc(MAX_LINE_LENGTH*sizeof(char));
		char** pLabel = malloc(MAX_LINE_LENGTH*sizeof(char));
		char** pOpcode = malloc(MAX_LINE_LENGTH*sizeof(char));;
		char** pArg1 = malloc(MAX_LINE_LENGTH*sizeof(char));;
		char** pArg2 = malloc(MAX_LINE_LENGTH*sizeof(char));;
		char** pArg3 = malloc(MAX_LINE_LENGTH*sizeof(char));;
		char** pArg4 = malloc(MAX_LINE_LENGTH*sizeof(char));;
		input = fopen(argv[1], "r");
		output = fopen(argv[2], "w");
		if (input == NULL)
		{
			printf("Failed to open file \"%s\"\n", argv[1]);
			exit(4);
		}
		if (output == NULL)
		{
			printf("Failed to open file \"%s\"\n", argv[2]);
			exit(4);
		}

		int temp = readAndParse(input, pLine, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);
		Node* head = NULL;
		while (temp != DONE)
		{
			if (temp == EMPTY_LINE)
			{
				temp = readAndParse(input, pLine, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);
				continue;
			}
			else
			{
				if (isPseudoOp(pOpcode) == 1) /*0 is not, 1 is .End, 2 is .ORIG, and 3 is .FILL*/
				{
					break;
				}
				else if (isPseudoOp(pOpcode) == 2)
				{
					currentAddress = toNum(*pArg1);
					if (currentAddress % 2 == 1)
					{
						exit(3);
					}
					temp = readAndParse(input, pLine, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);
					continue;
				}
				else if (**pLabel != NULL)
				{
					if (startWithNumber(pLabel) == 1 || *pLabel[0] == 'x')
					{
						exit(4);
					}
					if (isAllAlphanumeric(pLabel) == 0 || isSpecialCases(pLabel) == 0)
					{
						exit(4); /*fix this, puts shouldn't crash*/
					}
					else
					{
						if (createdLinkedList == 0)
						{
							head = createLinkedList(*pLabel, currentAddress);
							createdLinkedList = 1;
						}
						else
						{
							if (isLinkedListFull(head) == 255)
							{
								exit(4);
							}
							if (addLinkedList(head, (*pLabel), currentAddress) == NULL)
							{
								exit(4);
							}
						}
					}
				}
				currentAddress = currentAddress + 2;
				temp = readAndParse(input, pLine, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);
			}
		}

		/*second run through file*/
		int dotOrigCount = 0;
		rewind(input);
		temp = readAndParse(input, pLine, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);
		while (temp != DONE)
		{
			if (temp == EMPTY_LINE)
			{
				temp = readAndParse(input, pLine, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);
				continue;
			}
			else
			{
				if (isPseudoOp(pOpcode) == 1) /* -1 is not a pseudo, 1 is .End, 2 is .ORIG and 3 is .FILL*/
				{
					exit(0);
				}
				else if (isPseudoOp(pOpcode) == 2)
				{
					dotOrigCount = 1;
					currentAddress = toNum(*pArg1);
					if (currentAddress < 0 || currentAddress > 0xFFFF)
					{
						exit(3);
					}
					fprintf(output, "%s", "0x");
					fprintf(output, "%x\n", currentAddress);
					temp = readAndParse(input, pLine, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);
					continue;
				}

				if (dotOrigCount == 0)
				{
					temp = readAndParse(input, pLine, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);
					continue;
				}
				
				if (isPseudoOp(pOpcode) == 3)
				{
					int constant = toNum(*pArg1);
					if (constant > 0xFFFF)
					{
						exit(3);
					}
					constant &= 0x0000FFFF;
					fprintf(output, "%s", "0x");
					fprintf(output, "%04X\n", constant);
				}

				if (strlen(*pLabel) != 0 && strlen(*pOpcode) == 0)
				{
					exit(2);
				}
				if (strcmp((*pOpcode), "add") == 0 || strcmp((*pOpcode), "and") == 0)
				{
					int iArg1;
					int iArg2;
					int iArg3;
					char outString[MAXOUTPUTSTRINGLENGTH] = "0x";
					if (strlen(*pArg4) != 0)
					{
						exit(4);
					}
					if (strcmp((*pOpcode), "add") == 0)
					{
						outString[2] = '1';
					}
					else if (strcmp((*pOpcode), "and") == 0)
					{
						outString[2] = '5';
					}
					char* cArg3 = *pArg3;
					iArg1 = charToIntRegister((*pArg1)[1]);
					iArg2 = charToIntRegister((*pArg2)[1]);
					iArg3 = charToIntRegister(cArg3[1]);
					if ((*pArg1)[0] != 'r' || (*pArg2)[0] != 'r') /*check for 2 registers*/
					{
						exit(4);
					}

					if (strlen(*pArg1) != 2 || strlen(*pArg2) != 2)
					{
						exit(4);
					}

					if (cArg3[0] == 'r')	/*check if register */
					{
						if (strlen(*pArg3) != 2)
						{
							exit(4);
						}

						if (iArg3 == -1 || iArg1 == -1 || iArg2 == -1)
						{
							exit(4);
						}
						else
						{
							if (iArg2 >= 4)
							{
								iArg1 = (iArg1 * 2) + 1;
								outString[3] = intToCharHex(iArg1);
								outString[4] = intToCharHex(((iArg2 * 2) % 8) * 2);
							}
							else
							{
								outString[3] = intToCharHex(iArg1 * 2);
								outString[4] = intToCharHex(iArg2 * 2 * 2);
							}
							outString[5] = intToCharHex(iArg3);
							fprintf(output, "%s\n", outString);
						}

					}
					else
					{
						if (iArg1 == -1 || iArg2 == -1)
						{
							exit(4);
						}
						else
						{
							int immd = toNum((*pArg3));
							if (immd > 15 || immd < -16)
							{
								exit(3);
							}
							if (iArg2 >= 4)
							{
								iArg1 = (iArg1 * 2) + 1;
								outString[3] = intToCharHex(iArg1);
								if (immd < 0)
								{
									outString[4] = intToCharHex((((iArg2 * 2) % 8) * 2) + 3);
									outString[5] = intToCharHex(immd + 16);
								}
								else
								{
									outString[4] = intToCharHex((((iArg2 * 2) % 8) * 2) + 2);
									outString[5] = intToCharHex(immd);
								}
								fprintf(output, "%s\n", outString);
							}
							else
							{
								iArg1 = (iArg1 * 2);	
								outString[3] = intToCharHex(iArg1);
								if (immd < 0)
								{
									outString[4] = intToCharHex((iArg2 * 2 * 2) + 3);
									outString[5] = intToCharHex(immd + 16);
								}
								else
								{
									outString[4] = intToCharHex((iArg2 * 2 * 2) + 2);
									outString[5] = intToCharHex(immd);
								}
								fprintf(output, "%s\n", outString);
							}
						}
					}
				}
				else if (isBranch((*pOpcode)) != NOTBRANCH)
				{
					char outString[MAXOUTPUTSTRINGLENGTH] = "0x0";
					if (strlen(*pArg2) != 0)
					{
						exit(4);
					}
					int whichBranch = isBranch((*pOpcode));
					int condCodeValue = 0;
					if (whichBranch == BR || whichBranch == BRNZP || whichBranch == BRN || whichBranch == BRNZ|| whichBranch == BRNP)
					{
						condCodeValue = condCodeValue + 4;
					}
					if (whichBranch == BR || whichBranch == BRNZP || whichBranch == BRZ || whichBranch == BRNZ || whichBranch == BRZP)
					{
						condCodeValue = condCodeValue + 2;
					}
					if (whichBranch == BR || whichBranch == BRNZP || whichBranch == BRP || whichBranch == BRZP || whichBranch == BRNP)
					{
						condCodeValue = condCodeValue + 1;
					}

					if (strlen(*pArg1) == 0)
					{
						exit(4);
					}

					if (startWithNumber(pArg1) == 1 || *pArg1[0] == 'x')
					{
						exit(4);
					}
					if (isAllAlphanumeric(pArg1) == 0 || isSpecialCases(pArg1) == 0)
					{
						exit(4); /*fix this, puts shouldn't crash*/
					}

					if (head == NULL)
					{
						exit(1);
					}

					if (isInLinkedList(head, (*pArg1)) != NULL)
					{
						int totalOffset = 0;
						int offset1[1]; /*most significant 4 bits*/
						int offset2[1];
						*offset1 = 0;
						*offset2 = 0;
						Node* node = isInLinkedList(head, (*pArg1));
						int targetAddress = (*node).address;
						totalOffset = targetAddress - (currentAddress + 2);  /*considering only labels, no constants for branches*/
						if (totalOffset < -256 || totalOffset > 255)
						{
							exit(3);
						}
						if (totalOffset < 0)
						{
							condCodeValue = (condCodeValue * 2) + 1;
							totalOffsetToHexPC9(totalOffset, offset1, offset2);
						}
						else
						{
							condCodeValue = (condCodeValue * 2);
							totalOffsetToHexPC9(totalOffset, offset1, offset2);
						}
						outString[3] = intToCharHex(condCodeValue);
						outString[4] = intToCharHex((*offset1));
						outString[5] = intToCharHex((*offset2));
						fprintf(output, "%s\n", outString);
					}
					else
					{
						exit(1);
					}
				}
				else if (strcmp((*pOpcode), "jmp") == 0 || strcmp((*pOpcode), "ret") == 0)
				{
					int iArg1;
					int iArg2;
					int iArg3;
					char outString[MAXOUTPUTSTRINGLENGTH] = "0xC";
					if (strcmp((*pOpcode), "ret") == 0)
					{
						outString[3] = '1';
						outString[4] = 'C';
						outString[5] = '0';
						fprintf(output, "%s\n", outString);
					}
					else
					{
						if (strlen(*pArg2) != 0)
						{
							exit(4);
						}
						int iArg1 = charToIntRegister((*pArg1)[1]);

						if (strlen(*pArg1) != 2)
						{
							exit(4);
						}

						if (iArg1 == -1)
						{
							exit(3);
						}
						else
						{
							if (iArg1 >= 4)
							{
								outString[3] = '1';
								outString[4] = intToCharHex((iArg1 - 4) * 2 * 2);
								outString[5] = '0';
							}
							else
							{
								outString[3] = '0';
								outString[4] = intToCharHex(iArg1 * 2 * 2);
								outString[5] = '0';
							}
							fprintf(output, "%s\n", outString);
						}
					}
				}
				else if (strcmp((*pOpcode), "jsr") == 0)
				{
					if (strlen(*pArg2) != 0)
					{
						exit(4);
					}

					if (strlen(*pArg1) == 0)
					{
						exit(4);
					}

					if (startWithNumber(pArg1) == 1 || *pArg1[0] == 'x')
					{
						exit(4);
					}
					if (isAllAlphanumeric(pArg1) == 0 || isSpecialCases(pArg1) == 0)
					{
						exit(4);
					}

					if (head == NULL)
					{
						exit(1);
					}

					if (isInLinkedList(head, (*pArg1)) != NULL)
					{
						char outString[MAXOUTPUTSTRINGLENGTH] = "0x4";
						int totalOffset = 0;
						int offset1[1];
						int offset2[1];
						int offset3[1];
						*offset1 = 0;
						*offset2 = 0;
						*offset3 = 0;

						Node* node = isInLinkedList(head, (*pArg1));
						int targetAddress = (*node).address;
						totalOffset = targetAddress - (currentAddress + 2);
						if (totalOffset < -1024 || totalOffset > 1023)
						{
							exit(3);
						}
						totalOffsetToHexPC11(totalOffset, offset1, offset2, offset3);
						outString[3] = intToCharHex((*offset1));
						outString[4] = intToCharHex((*offset2));
						outString[5] = intToCharHex((*offset3));
						fprintf(output, "%s\n", outString);
					}
					else
					{
						exit(1);
					}

				}
				else if (strcmp((*pOpcode), "jsrr") == 0)
				{
					char outString[MAXOUTPUTSTRINGLENGTH] = "0x4";
					if (strlen(*pArg2) != 0)
					{
						exit(4);
					}
					int iArg1 = charToIntRegister((*pArg1)[1]);

					if (strlen(*pArg1) != 2)
					{
						exit(4);
					}

					if (iArg1 == -1)
					{
						exit(3);
					}
					if (iArg1 >= 4)
					{
						outString[3] = '1';
						outString[4] = intToCharHex((iArg1 - 4) * 2 * 2);
						outString[5] = '0';
					}
					else
					{
						outString[3] = '0';
						outString[4] = intToCharHex(iArg1 * 2 * 2);
						outString[5] = '0';
					}
					fprintf(output, "%s\n", outString);
				}
				else if (strcmp((*pOpcode), "ldb") == 0 || strcmp((*pOpcode), "stb") == 0)
				{
					char outString[MAXOUTPUTSTRINGLENGTH] = "0x2";
					if (strlen(*pArg4) != 0)
					{
						exit(4);
					}
					if (strcmp((*pOpcode), "stb") == 0)
					{
						outString[2] = '3';
					}
					int iArg1 = charToIntRegister((*pArg1)[1]);
					int iArg2 = charToIntRegister((*pArg2)[1]);
					int boffset6 = toNum((*pArg3));

					if (strlen(*pArg1) != 2 || strlen(*pArg2) != 2)
					{
						exit(4);
					}

					if (iArg1 == -1)
					{
						exit(3);
					}
					if (iArg2 == -1)
					{
						exit(3);
					}
					if (boffset6 < -32 || boffset6 > 31)
					{
						exit(3);
					}
					if (iArg2 >= 4)
					{
						outString[3] = intToCharHex((iArg1 * 2) + 1);
					}
					else
					{
						outString[3] = intToCharHex((iArg1 * 2));
					}
					char offset1[1];
					char offset2[1];
					*offset1 = iArg2;
					*offset2 = 0;
					boffset6ToHex(boffset6, offset1, offset2);
					outString[4] = intToCharHex((*offset1));
					outString[5] = intToCharHex((*offset2));
					fprintf(output, "%s\n", outString);
				}
				else if(strcmp((*pOpcode), "ldw") == 0 || strcmp((*pOpcode), "stw") == 0)
				{
					char outString[MAXOUTPUTSTRINGLENGTH] = "0x6";
					if (strlen(*pArg4) != 0)
					{
						exit(4);
					}
					if (strcmp((*pOpcode), "stw") == 0)
					{
						outString[2] = '7';
					}
					int iArg1 = charToIntRegister((*pArg1)[1]);
					int iArg2 = charToIntRegister((*pArg2)[1]);
					int offset6 = toNum((*pArg3));
					if (strlen(*pArg1) != 2 || strlen(*pArg2) != 2)
					{
						exit(4);
					}
					if (iArg1 == -1)
					{
						exit(3);
					}
					if (iArg2 == -1)
					{
						exit(3);
					}
					if (offset6 < -32 || offset6 > 31)
					{
						exit(3);
					}
					if (iArg2 >= 4)
					{
						outString[3] = intToCharHex((iArg1 * 2) + 1);
					}
					else
					{
						outString[3] = intToCharHex((iArg1 * 2));
					}
					char offset1[1];
					char offset2[1];
					*offset1 = iArg2;
					*offset2 = 0;
					totalOffsetToHexOffset6(offset6, offset1, offset2);
					outString[4] = intToCharHex((*offset1));
					outString[5] = intToCharHex((*offset2));
					fprintf(output, "%s\n", outString);
				}
				else if (strcmp((*pOpcode), "lea") == 0)
				{
					if (strlen(*pArg3) != 0)
					{
						exit(4);
					}
					if (strlen(*pArg2) == 0)
					{
						exit(4);
					}

					if (startWithNumber(pArg2) == 1 || *pArg2[0] == 'x')
					{
						exit(4);
					}
					if (isAllAlphanumeric(pArg2) == 0 || isSpecialCases(pArg2) == 0)
					{
						exit(4); /*fix this, puts shouldn't crash*/
					}

					if (head == NULL)
					{
						exit(1);
					}
					if (isInLinkedList(head, (*pArg2)) != NULL)
					{
						char outString[MAXOUTPUTSTRINGLENGTH] = "0xE";
						int totalOffset = 0;
						int offset1[1];
						int offset2[1];
						int offset3[1];
						*offset1 = 0;
						*offset2 = 0;
						*offset3 = 0;
						int iArg1 = charToIntRegister((*pArg1)[1]);
						if (strlen(*pArg1) != 2)
						{
							exit(4);
						}
						if (iArg1 == -1)
						{
							exit(3);
						}
						iArg1 *= 2;

						Node* node = isInLinkedList(head, (*pArg2));
						int targetAddress = (*node).address;
						totalOffset = targetAddress - (currentAddress + 2);
						if (totalOffset < -1024 || totalOffset > 1023)
						{
							exit(3);
						}
						if (totalOffset < 0)
						{
							iArg1 += 1;
						}
						totalOffsetToHexPC9(totalOffset, offset2, offset3);
						outString[3] = intToCharHex(iArg1);
						outString[4] = intToCharHex((*offset2));
						outString[5] = intToCharHex((*offset3));
						fprintf(output, "%s\n", outString);
					}
					else
					{
						exit(1);
					}
				}
				else if (strcmp((*pOpcode), "xor") == 0 || strcmp((*pOpcode), "not") == 0)
				{
					int iArg1;
					int iArg2;
					int iArg3;
					char outString[MAXOUTPUTSTRINGLENGTH] = "0x9";
					if (strcmp((*pOpcode), "not") == 0 && strlen(*pArg3) != 0)
					{
						exit(4);
					}
					if (strcmp((*pOpcode), "xor") == 0 && strlen(*pArg4) != 0)
					{
						exit(4);
					}
					char* cArg3 = *pArg3;
					iArg1 = charToIntRegister((*pArg1)[1]);
					iArg2 = charToIntRegister((*pArg2)[1]);
					iArg3 = charToIntRegister(cArg3[1]);
					if ((*pArg1)[0] != 'r' || (*pArg2)[0] != 'r')
					{
						exit(4);
					}

					if (strlen(*pArg1) != 2 || strlen(*pArg2) != 2)
					{
						exit(4);
					}
					if (cArg3[0] == 'r')
					{
						if (strlen(*pArg3) != 2)
						{
							exit(4);
						}
						if (iArg3 == -1 || iArg1 == -1 || iArg2 == -1)
						{
							exit(4);
						}
						else
						{
							if (iArg2 >= 4)
							{
								iArg1 = (iArg1 * 2) + 1;
								outString[3] = intToCharHex(iArg1);
								outString[4] = intToCharHex(((iArg2 * 2) % 8) * 2);
							}
							else
							{
								outString[3] = intToCharHex(iArg1 * 2);
								outString[4] = intToCharHex(iArg2 * 2 * 2);
							}
							outString[5] = intToCharHex(iArg3);
							fprintf(output, "%s\n", outString);
						}

					}
					else
					{
						if (iArg1 == -1 || iArg2 == -1)
						{
							exit(4);
						}
						else
						{

							int immd;
							if (strcmp((*pOpcode), "not") == 0)
							{
								immd = -1;
							}
							else
							{
								immd = toNum((*pArg3));
							}
							if (immd > 15 || immd < -16)
							{
								exit(3);
							}
							if (iArg2 >= 4)
							{
								iArg1 = (iArg1 * 2) + 1;
								outString[3] = intToCharHex(iArg1);
								if (immd < 0)
								{
									outString[4] = intToCharHex((((iArg2 * 2) % 8) * 2) + 3);
									outString[5] = intToCharHex(immd + 16);
								}
								else
								{
									outString[4] = intToCharHex((((iArg2 * 2) % 8) * 2) + 2);
									outString[5] = intToCharHex(immd);
								}
								fprintf(output, "%s\n", outString);
							}
							else
							{
								iArg1 = (iArg1 * 2);
								outString[3] = intToCharHex(iArg1);
								if (immd < 0)
								{
									outString[4] = intToCharHex((iArg2 * 2 * 2) + 3);
									outString[5] = intToCharHex(immd + 16);
								}
								else
								{
									outString[4] = intToCharHex((iArg2 * 2 * 2) + 2);
									outString[5] = intToCharHex(immd);
								}
								fprintf(output, "%s\n", outString);
							}
						}
					}
				}
				else if (strcmp((*pOpcode), "rti") == 0)
				{
					char outString[MAXOUTPUTSTRINGLENGTH] = "0x8000";
					fprintf(output, "%s\n", outString);
				}
				else if (strcmp((*pOpcode), "lshf") == 0 || strcmp((*pOpcode), "rshfl") == 0 || strcmp((*pOpcode), "rshfa") == 0)
				{
					char outString[MAXOUTPUTSTRINGLENGTH] = "0xD";
					if (strlen(*pArg4) != 0)
					{
						exit(4);
					}
					int iArg1;
					char iArg2;
					char amount4 = 0;
					iArg1 = charToIntRegister((*pArg1)[1]);
					iArg2 = (char)charToIntRegister((*pArg2)[1]);
					amount4 = toNum((*pArg3));
					if (amount4 < 0 || amount4 > 15)
					{
						exit(3);
					}
					if (strlen(*pArg1) != 2 || strlen(*pArg2) != 2)
					{
						exit(4);
					}
					if (iArg1 == -1)
					{
						exit(3);
					}
					if (iArg2 == -1)
					{
						exit(3);
					}
					if (iArg2 >= 4)
					{
						outString[3] = intToCharHex((iArg1 * 2) + 1);
					}
					else
					{
						outString[3] = intToCharHex((iArg1 * 2));
					}
					iArg2 = iArg2 & 0xFFFB;
					iArg2 *= 4;
					if (strcmp((*pOpcode), "rshfl") == 0)
					{
						iArg2 += 1;
					}
					else if (strcmp((*pOpcode), "rshfa") == 0)
					{
						iArg2 += 3;
					}
					outString[4] = intToCharHex(iArg2);
					outString[5] = intToCharHex(amount4);
					fprintf(output, "%s\n", outString);
				}
				else if (strcmp((*pOpcode), "trap") == 0)
				{
					char outString[MAXOUTPUTSTRINGLENGTH - 2] = "0xF0";
					int vector = toNum(*pArg1);
					char x = (*pArg1)[0];
					if (x != 'x')
					{
						exit(4);
					}
					if (vector < 0 || vector > 0xFF)
					{
						exit(3);
					}
					fprintf(output, "%s", outString);
					fprintf(output, "%02X\n", vector);
				}
				else if (strcmp((*pOpcode), "nop") == 0)
				{
					char outString[MAXOUTPUTSTRINGLENGTH] = "0x0000";
					fprintf(output, "%s\n", outString);
				}
				else if (strcmp((*pOpcode), "halt") == 0)
				{
					char outString[MAXOUTPUTSTRINGLENGTH] = "0xF025";
					fprintf(output, "%s\n", outString);
				}
				currentAddress = currentAddress + 2;
				temp = readAndParse(input, pLine, pLabel, pOpcode, pArg1, pArg2, pArg3, pArg4);
			}
		}
		fclose(input);
		fclose(output);
		/*freeLinkedList(symbolTable);*/
		exit(0);
	}
	else
	{
		printf("Invalid Number of Command Line Arguments\n");
		exit(4);
	}
}

void totalOffsetToHexOffset6(int totalOffset, char* offset1, char* offset2)
{
	int temp[OFFSET6WOSIGN] = {-1, -1, -1, -1, -1 };
	int i, j;
	*offset1 = *offset1 & 0xFB;
	*offset1 = *offset1 * 4;
	if (totalOffset == 0)
	{
		*offset2 = 0;
		return;
	}

	if (totalOffset < 0)
	{
		int remainder = totalOffset * -1;
		*offset1 += 2;
		for (i = OFFSET6WOSIGN - 1; i >= 0; i--)
		{
			temp[i] = remainder % 2;
			remainder = remainder / 2;
		}
		for (i = 0; i < OFFSET6WOSIGN; i++)
		{
			if (temp[i] == 0)
			{
				temp[i] = 1;
			}
			else
			{
				temp[i] = 0;
			}
		}
		for (i = OFFSET6WOSIGN - 1; temp[i] == 1 && i > 0; i--);
		if (i >= 0)
		{
			temp[i] = 1;
		}
		if (i != OFFSET6WOSIGN - 1)
		{
			for (j = i + 1; j < OFFSET6WOSIGN - 1; j++)
			{
				temp[j] = 0;
			}
		}
		int previousBit = temp[0];
		for (i = 1; i < OFFSET6WOSIGN; i++)
		{
			int tempBit = temp[i];
			temp[i] = previousBit;
			previousBit = tempBit;
		}
		temp[0] = 1;
		i = 1;
		if (temp[0] == 1)
		{
			*offset1 += 1;
		}
		while (i < OFFSET6WOSIGN)
		{
			if (i == 1 && temp[i] == 1)
			{
				*offset2 = *offset2 + 8;
			}
			if (i == 2 && temp[i] == 1)
			{
				*offset2 = *offset2 + 4;
			}
			if (i == 3 && temp[i] == 1)
			{
				*offset2 = *offset2 + 2;
			}
			if (i == 4 && temp[i] == 1)
			{
				*offset2 = *offset2 + 1;
			}
			i++;
		}
	}
	else
	{
		int remainder = totalOffset;
		for (i = OFFSET6WOSIGN - 1; i >= 0; i--)
		{
			temp[i] = remainder % 2;
			remainder = remainder / 2;
		}
		int previousBit = temp[0];
		for (i = 1; i < OFFSET6WOSIGN; i++)
		{
			int tempBit = temp[i];
			temp[i] = previousBit;
			previousBit = tempBit;
		}
		temp[0] = 0;
		i = 1;
		while (i < 5)
		{
			if (i == 1 && temp[i] == 1)
			{
				*offset2 = *offset2 + 8;
			}
			if (i == 2 && temp[i] == 1)
			{
				*offset2 = *offset2 + 4;
			}
			if (i == 3 && temp[i] == 1)
			{
				*offset2 = *offset2 + 2;
			}
			if (i == 4 && temp[i] == 1)
			{
				*offset2 = *offset2 + 1;
			}
			i++;
		}
	}

}
void boffset6ToHex(int boffset6, char* offset1, char* offset2)
{
	int temp[BOFFSET6WOSIGN] = { -1, -1, -1, -1, -1};
	*offset1 = *offset1 & 0xFB;
	*offset1 = *offset1 * 4;

	int i,j;
	if (boffset6 < 0)
	{
		*offset1 = (*offset1) + 2;
		int remainder = boffset6 * -1;
		for (i = BOFFSET6WOSIGN - 1; i >= 0; i--)
		{
			temp[i] = remainder % 2;
			remainder = remainder / 2;
		}
		for (i = 0; i < BOFFSET6WOSIGN; i++)
		{
			if (temp[i] == 0)
			{
				temp[i] = 1;
			}
			else
			{
				temp[i] = 0;
			}
		}
		for (i = BOFFSET6WOSIGN - 1; temp[i] == 1 && i > 0; i--);
		if (i >= 0)
		{
			temp[i] = 1;
		}
		if (i != BOFFSET6WOSIGN - 1)
		{
			for (j = i + 1; j < BOFFSET6WOSIGN - 1; j++)
			{
				temp[j] = 0;
			}
		}
	}
	else
	{
		int remainder = boffset6;
		for (i = BOFFSET6WOSIGN - 1; i >= 0; i--)
		{
			temp[i] = remainder % 2;
			remainder = remainder / 2;
		}
	}
	if (temp[0] == 1)
	{
		*offset1 = *offset1 + 1;
	}
	for (i = 1; i < BOFFSET6WOSIGN; i++)
	{
		if (temp[i] == 1 && i == 1)
		{
			*offset2 = *offset2 + 8;
		}
		if (temp[i] == 1 && i == 2)
		{
			*offset2 = *offset2 + 4;
		}
		if (temp[i] == 1 && i == 3)
		{
			*offset2 = *offset2 + 2;
		}
		if (temp[i] == 1 && i == 4)
		{
			*offset2 = *offset2 + 1;
		}
	}
}

void totalOffsetToHexPC11(int totalOffset, int* offset1, int* offset2, int* offset3)
{
	int temp[PCOFFSET11WOSIGN] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	int i,j;
	if (totalOffset == 0)
	{
		*offset1 = 0;
		*offset2 = 0;
		*offset3 = 0;
		return;
	}
	if (totalOffset == -1024)
	{
		*offset1 = 12;
		*offset2 = 0;
		*offset3 = 0;
		return;
	}
	if (totalOffset < 0)
	{
		int remainder = totalOffset * -1;
		for (i = PCOFFSET11WOSIGN - 1; i >= 0; i--)
		{
			temp[i] = remainder % 2;
			remainder = remainder / 2;
		}
		for (i = 0; i < PCOFFSET11WOSIGN; i++)
		{
			if (temp[i] == 0)
			{
				temp[i] = 1;
			}
			else
			{
				temp[i] = 0;
			}
		}
		for (i = PCOFFSET11WOSIGN - 1; temp[i] == 1 && i > 0; i--);
		if (i >= 0)
		{
			temp[i] = 1;
		}
		if (i != PCOFFSET11WOSIGN - 1)
		{
			for (j = i + 1; j < PCOFFSET11WOSIGN - 1; j++)
			{
				temp[j] = 0;
			}
		}
		int previousBit = temp[0];
		for (i = 1; i < PCOFFSET11WOSIGN; i++)
		{
			int tempBit = temp[i];
			temp[i] = previousBit;
			previousBit = tempBit;
		}
		temp[0] = 1;
		int j = 6;
		if (temp[0] == 1)
		{
			*offset1 = *offset1 + 2;
		}
		if (temp[1] == 1)
		{
			*offset1 = *offset1 + 1;
		}
		i = 2;
		while (i < 6)
		{
			if (i == 2 && temp[i] == 1)
			{
				*offset2 = *offset2 + 8;
			}
			if (j == 6 && temp[j] == 1)
			{
				*offset3 = *offset3 + 8;
			}
			if (i == 3 && temp[i] == 1)
			{
				*offset2 = *offset2 + 4;
			}
			if (j == 7 && temp[j] == 1)
			{
				*offset3 = *offset3 + 4;
			}
			if (i == 4 && temp[i] == 1)
			{
				*offset2 = *offset2 + 2;
			}
			if (j == 8 && temp[j] == 1)
			{
				*offset3 = *offset3 + 2;
			}
			if (i == 5 && temp[i] == 1)
			{
				*offset2 = *offset2 + 1;
			}
			if (j == 9 && temp[j] == 1)
			{
				*offset3 = *offset3 + 1;
			}
			i++;
			j++;
		}
		*offset1 = *offset1 + 12;
	}
	else
	{
		int remainder = totalOffset;
		for (i = PCOFFSET11WOSIGN - 1; i >= 0; i--)
		{
			temp[i] = remainder % 2;
			remainder = remainder / 2;
		}
		int previousBit = temp[0];
		for (i = 1; i < PCOFFSET11WOSIGN; i++)
		{
			int tempBit = temp[i];
			temp[i] = previousBit;
			previousBit = tempBit;
		}
		temp[0] = 0;
		int j = 6;
		if (temp[0] == 1)
		{
			*offset1 = *offset1 + 2;
		}
		if (temp[1] == 1)
		{
			*offset1 = *offset1 + 1;
		}
		i = 2;
		while (i < 6)
		{
			if (i == 2 && temp[i] == 1)
			{
				*offset2 = *offset2 + 8;
			}
			if (j == 6 && temp[j] == 1)
			{
				*offset3 = *offset3 + 8;
			}
			if (i == 3 && temp[i] == 1)
			{
				*offset2 = *offset2 + 4;
			}
			if (j == 7 && temp[j] == 1)
			{
				*offset3 = *offset3 + 4;
			}
			if (i == 4 && temp[i] == 1)
			{
				*offset2 = *offset2 + 2;
			}
			if (j == 8 && temp[j] == 1)
			{
				*offset3 = *offset3 + 2;
			}
			if (i == 5 && temp[i] == 1)
			{
				*offset2 = *offset2 + 1;
			}
			if (j == 9 && temp[j] == 1)
			{
				*offset3 = *offset3 + 1;
			}
			i++;
			j++;
		}
		*offset1 = *offset1 + 8;
	}

}

void totalOffsetToHexPC9(int totalOffset, int* offset1, int* offset2)
{
	int temp[PCOFFSET9WOSIGN] = { -1, -1, -1, -1, -1, -1, -1, -1 };
	int i,j;
	if (totalOffset == 0)
	{
		*offset1 = 0;
		*offset2 = 0;
		return;
	}
	if (totalOffset == -256)
	{
		*offset1 = 0;
		*offset2 = 0;
		return;
	}
	if (totalOffset < 0)
	{
		int remainder = totalOffset * -1;
		for (i = PCOFFSET9WOSIGN - 1; i >= 0; i--)
		{
			temp[i] = remainder % 2;
			remainder = remainder / 2;
		}
		for (i = 0; i < PCOFFSET9WOSIGN; i++)
		{
			if (temp[i] == 0)
			{
				temp[i] = 1;
			}
			else
			{
				temp[i] = 0;
			}
		}
		for (i = PCOFFSET9WOSIGN - 1; temp[i] == 1 && i > 0; i--);
		if (i >= 0)
		{
			temp[i] = 1;
		}
		if (i != PCOFFSET9WOSIGN - 1)
		{
			for (j = i + 1; j < PCOFFSET9WOSIGN - 1; j++)
			{
				temp[j] = 0;
			}
		}
		int previousBit = temp[0];
		for (i = 1; i < PCOFFSET9WOSIGN; i++)
		{
			int tempBit = temp[i];
			temp[i] = previousBit;
			previousBit = tempBit;
		}
		temp[0] = 1;
		int j = 4;
		i = 0;
		while (i < 4)
		{
			if (i == 0 && temp[i] == 1)
			{
				*offset1 = *offset1 + 8;
			}
			if (j == 4 && temp[j] == 1)
			{
				*offset2 = *offset2 + 8;
			}
			if (i == 1 && temp[i] == 1)
			{
				*offset1 = *offset1 + 4;
			}
			if (j == 5 && temp[j] == 1)
			{
				*offset2 = *offset2 + 4;
			}
			if (i == 2 && temp[i] == 1)
			{
				*offset1 = *offset1 + 2;
			}
			if (j == 6 && temp[j] == 1)
			{
				*offset2 = *offset2 + 2;
			}
			if (i == 3 && temp[i] == 1)
			{
				*offset1 = *offset1 + 1;
			}
			if (j == 7 && temp[j] == 1)
			{
				*offset2 = *offset2 + 1;
			}
			i++;
			j++;
		}
	}
	else
	{
		int remainder = totalOffset;
		for (i = PCOFFSET9WOSIGN - 1; i >= 0; i--)
		{
			temp[i] = remainder % 2;
			remainder = remainder / 2;
		}
		int previousBit = temp[0];
		for (i = 1; i < PCOFFSET9WOSIGN; i++)
		{
			int tempBit = temp[i];
			temp[i] = previousBit;
			previousBit = tempBit;
		}
		temp[0] = 0;
		int j = 4;
		i = 0;
		while (i < 4)
		{
			if (i == 0 && temp[i] == 1)
			{
				*offset1 = *offset1 + 8;
			}
			if (j == 4 && temp[j] == 1)
			{
				*offset2 = *offset2 + 8;
			}
			if (i == 1 && temp[i] == 1)
			{
				*offset1 = *offset1 + 4;
			}
			if (j == 5 && temp[j] == 1)
			{
				*offset2 = *offset2 + 4;
			}
			if (i == 2 && temp[i] == 1)
			{
				*offset1 = *offset1 + 2;
			}
			if (j == 6 && temp[j] == 1)
			{
				*offset2 = *offset2 + 2;
			}
			if (i == 3 && temp[i] == 1)
			{
				*offset1 = *offset1 + 1;
			}
			if (j == 7 && temp[j] == 1)
			{
				*offset2 = *offset2 + 1;
			}
			i++;
			j++;
		}
	}

}

int isBranch(char* opCode)
{
	if (strcmp(opCode, "brnzp") == 0)
	{
		return BRNZP;
	}
	if (strcmp(opCode, "brnz") == 0)
	{
		return BRNZ;
	}
	if (strcmp(opCode, "brzp") == 0)
	{
		return BRZP;
	}
	if (strcmp(opCode, "brnp") == 0)
	{
		return BRNP;
	}
	if (strcmp(opCode, "brn") == 0)
	{
		return BRN;
	}
	if (strcmp(opCode, "brz") == 0)
	{
		return BRZ;
	}
	if (strcmp(opCode, "brp") == 0)
	{
		return BRP;
	}
	if (strcmp(opCode, "br") == 0)
	{
		return BR;
	}
	return NOTBRANCH;
}

int isPseudoOp(char** pOpcode)
{
	if (strcmp((*pOpcode), ".end") == 0)
	{
		return 1;
	}
	else if (strcmp((*pOpcode), ".orig") == 0)
	{
		return 2;
	}
	else if (strcmp((*pOpcode), ".fill") == 0)
	{
		return 3;
	}
	else
	{
		return -1;
	}
}

int isAllAlphanumeric(char** pLabel)
{
	int i;
	for (i = 0; i < strlen((*pLabel)); i++)
	{
		if ((*pLabel)[i] < 'a' || (*pLabel)[i] > 'z')
		{
			if ((*pLabel)[i] < '0' || (*pLabel)[i] > '9')
			{
				return 0;
			}
		}
	}

	return 1;
}

int isSpecialCases(char** pLabel)
{
	if (strcmp((*pLabel), "in") == 0)
	{
		return 0;
	}
	else if (strcmp((*pLabel), "out") == 0)
	{
		return 0;
	}
	else if (strcmp((*pLabel), "gets") == 0)
	{
		return 0;
	}
	else if (strcmp((*pLabel), "puts") == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

char intToCharHex(int toConvert)
{
	switch (toConvert)
	{
	case 0:
		return '0';
	case 1:
		return '1';
	case 2:
		return '2';
	case 3:
		return '3';
	case 4:
		return '4';
	case 5:
		return '5';
	case 6:
		return '6';
	case 7:
		return '7';
	case 8:
		return '8';
	case 9:
		return '9';
	case 10:
		return 'A';
	case 11:
		return 'B';
	case 12:
		return 'C';
	case 13:
		return 'D';
	case 14:
		return 'E';
	case 15:
		return 'F';
	}
	return NULL;

}

int charToIntRegister(char toConvert)
{
	switch (toConvert)
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	}
	return -1;
}

int startWithNumber(char **labelFinder)
{
	switch ((*labelFinder)[0])
	{
	case '0':
		return 1;
	case '1':
		return 1;
	case '2':
		return 1;
	case '3':
		return 1;
	case '4':
		return 1;
	case '5':
		return 1;
	case '6':
		return 1;
	case '7':
		return 1;
	case '8':
		return 1;
	case '9':
		return 1;
	}
	return 0;
}

void printString(char* labelFinder, int i)
{
	int j;
	for (j = 0; j <= i; j++)
	{
		printf("%c", labelFinder[j]);
	}
}

char* reallocLabelFinder(char* labelFinder, int i)
{
	char* temp;
	temp = (char*)realloc(labelFinder, i*sizeof(char));
	return temp;
}

Node* createLinkedList(char* label, int address)
{
	Node* head = (Node*)malloc(sizeof(Node));
	strcpy(((*head).label), label);
	(*head).address = address;
	(*head).next = NULL;
	return head;
}

Node* addLinkedList(Node* head, char* label, int address)
{
	if (isInLinkedList(head, label) == NULL)
	{
		Node* new = (Node*)malloc(sizeof(Node));
		Node* temp = head;
		while ((*temp).next != NULL)
		{
			temp = (*temp).next;
		}
		(*temp).next = new;
		strcpy((*new).label, label);
		(*new).address = address;
		(*new).next = NULL;
		return head;
	}
	else
	{
		return NULL;
	}
}

Node* removeLinkedList(Node* head, char* label)
{
	Node* prevTemp = head;
	Node* temp = (*head).next;
	while ((*temp).next != NULL)
	{
		if (strcmp((*head).label, label) == 0)
		{
			free(head);
			return temp;
		}
		else if (strcmp((*temp).label, label) == 0)
		{
			(*prevTemp).next = (*temp).next;
			free(temp);
			return head;
		}
		prevTemp = (*head).next;
		temp = (*temp).next;
	}
}

int sizeOfLinkedList(Node* head)
{
	Node* temp = head;
	int i = 1;
	while ((*temp).next != NULL) { i++; }
	return i;
}

Node* isInLinkedList(Node* head, char* label)
{
	Node* temp = (*head).next;
	if (strcmp((*head).label, label) == 0)
	{
		return head;
	}
	while (temp != NULL)
	{
		if (strcmp((*temp).label, label) == 0)
		{
			return temp;
		}
		temp = (*temp).next;
	}
	return NULL;
}

int isLinkedListFull(Node* head)
{
	Node* temp = (*head).next;
	int i = 1;
	while (temp != NULL)
	{
		i++;
		temp = (*temp).next;
	}
	return i-1;
}

void printLinkedList(Node* head)
{
	while ((*head).next != NULL)
	{
		printf("%s\n", (*head).label);
		head = (*head).next;
	}
	printf("%s\n", (*head).label);
}

void freeLinkedList(Node* head)
{
	Node* temp = head;
	Node* nextTemp;
	if ((*head).next != NULL)
	{
		nextTemp = (*head).next;
	}
	while ((*temp).next != NULL)
	{
		nextTemp = ((*temp).next);
		free(temp);
		temp = nextTemp;
	}
	free(temp);
}

int readAndParse(FILE * pInfile, char * pLine, char ** pLabel, char
** pOpcode, char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4
)
{
	char * lRet, *lPtr;
	int i;
	if (!fgets(pLine, MAX_LINE_LENGTH, pInfile))
		return(DONE);
	for (i = 0; i < strlen(pLine); i++)
		pLine[i] = tolower(pLine[i]);

	/* convert entire line to lowercase */
	*pLabel = *pOpcode = *pArg1 = *pArg2 = *pArg3 = *pArg4 = pLine + strlen(pLine);

	/* ignore the comments */
	lPtr = pLine;

	while (*lPtr != ';' && *lPtr != '\0' &&
		*lPtr != '\n')
		lPtr++;

	*lPtr = '\0';
	if (!(lPtr = strtok(pLine, "\t\n ,")))
		return(EMPTY_LINE);

	if (isOpcode(lPtr) == -1 && lPtr[0] != '.') /* found a label */
	{
		*pLabel = lPtr;
		if (!(lPtr = strtok(NULL, "\t\n ,"))) return(OK);
	}

	if (isOpcode(lPtr) == -1 && isPseudoOp(&lPtr) == -1)
	{
		exit(2);
	}
	*pOpcode = lPtr;

	if (!(lPtr = strtok(NULL, "\t\n ,"))) return(OK);

	*pArg1 = lPtr;

	if (!(lPtr = strtok(NULL, "\t\n ,"))) return(OK);

	*pArg2 = lPtr;
	if (!(lPtr = strtok(NULL, "\t\n ,"))) return(OK);

	*pArg3 = lPtr;

	if (!(lPtr = strtok(NULL, "\t\n ,"))) return(OK);

	*pArg4 = lPtr;

	return(OK);
}

int toNum(char * pStr)
{
	char * t_ptr;
	char * orig_pStr;
	int t_length, k;
	int lNum, lNeg = 0;
	long int lNumLong;

	orig_pStr = pStr;
	if (*pStr == '#')				/* decimal */
	{
		pStr++;
		if (*pStr == '-')				/* dec is negative */
		{
			lNeg = 1;
			pStr++;
		}
		t_ptr = pStr;
		t_length = strlen(t_ptr);
		for (k = 0; k < t_length; k++)
		{
			if (!isdigit(*t_ptr))
			{
				printf("Error: invalid decimal operand, %s\n", orig_pStr);
				exit(4);
			}
			t_ptr++;
		}
		lNum = atoi(pStr);
		if (lNeg)
			lNum = -lNum;

		return lNum;
	}
	else if (*pStr == 'x')	/* hex     */
	{
		pStr++;
		if (*pStr == '-')				/* hex is negative */
		{
			lNeg = 1;
			pStr++;
		}
		t_ptr = pStr;
		t_length = strlen(t_ptr);
		for (k = 0; k < t_length; k++)
		{
			if (!isxdigit(*t_ptr))
			{
				printf("Error: invalid hex operand, %s\n", orig_pStr);
				exit(4);
			}
			t_ptr++;
		}
		lNumLong = strtol(pStr, NULL, 16);    /* convert hex string into integer */
		lNum = (lNumLong > INT_MAX) ? INT_MAX : lNumLong;
		if (lNeg)
			lNum = -lNum;
		return lNum;
	}
	else
	{
		printf("Error: invalid operand, %s\n", orig_pStr);
		exit(4);  /* This has been changed from error code 3 to error code 4, see clarification 12 */
	}
}

int isOpcode(char* lPtr)
{
	switch (lPtr[0])
	{
	case 'a':
		if (strcmp(lPtr, "add") == 0)
		{
			return 1;
		}
		if (strcmp(lPtr, "and") == 0)
		{
			return 1;
		}
		return -1;
	case 'b':
		if (strcmp(lPtr, "brnzp") == 0)
		{
			return 1;
		}
		if (strcmp(lPtr, "brnz") == 0)
		{
			return 1;
		}
		if (strcmp(lPtr, "brzp") == 0)
		{
			return 1;
		}
		if (strcmp(lPtr, "brnp") == 0)
		{
			return 1;
		}
		if (strcmp(lPtr, "brn") == 0)
		{
			return 1;
		}
		if (strcmp(lPtr, "brz") == 0)
		{
			return 1;
		}
		if (strcmp(lPtr, "brp") == 0)
		{
			return 1;
		}
		if (strcmp(lPtr, "br") == 0)
		{
			return 1;
		}
		return -1;
	case 'h':
		if (strcmp(lPtr, "halt") == 0)
		{
			return 1;
		}
		return -1;
	case 'j':
		if (strcmp(lPtr, "jmp") == 0)
		{
			return 1;
		}
		if (strcmp(lPtr, "jsr") == 0)
		{
			return 1;
		}
		if (strcmp(lPtr, "jsrr") == 0)
		{
			return 1;
		}
		return -1;
	case 'l':
		if (strcmp(lPtr, "ldb") == 0)
		{
			return 1;
		}
		if (strcmp(lPtr, "ldw") == 0)
		{
			return 1;
		}
		if (strcmp(lPtr, "lea") == 0)
		{
			return 1;
		}
		if (strcmp(lPtr, "lshf") == 0)
		{
			return 1;
		}
		return -1;
	case 'n':
		if (strcmp(lPtr, "nop") == 0)
		{
			return 1;
		}
		if (strcmp(lPtr, "not") == 0)
		{
			return 1;
		}
		return -1;
	case 'r':
		if (strcmp(lPtr, "ret") == 0)
		{
			return 1;
		}
		if (strcmp(lPtr, "rshfl") == 0)
		{
			return 1;
		}
		if (strcmp(lPtr, "rshfa") == 0)
		{
			return 1;
		}
		if (strcmp(lPtr, "rti") == 0)
		{
			return 1;
		}
		return -1;
	case 's':
		if (strcmp(lPtr, "stb") == 0)
		{
			return 1;
		}
		if (strcmp(lPtr, "stw") == 0)
		{
			return 1;
		}
		return -1;
	case 't':
		if (strcmp(lPtr, "trap") == 0)
		{
			return 1;
		}
		return -1;
	case 'x':
		if (strcmp(lPtr, "xor") == 0)
		{
			return 1;
		}
		return -1;
	default:
		return -1;
	}
}

/*Node* createSymbolTable(FILE *input)
{
	Node* head = createLinkedList("bob", "bob");
	head = addLinkedList(head, "susan", "susan");
	head = addLinkedList(head, "brand", "brand");
	head = removeLinkedList(head, "susan");
	printLinkedList(head);
	freeLinkedList(head);

}*/

/*char* createSymbolTable(FILE *input)
{
int lengthOfHex = 7;
int maxNumberOfLines = 50;
int maxNumberOfSymbols = 50;
char labelFinder[MAXLENGTHOFLABEL];
char** symbolTable = (char**)malloc(sizeof(char*)* maxNumberOfSymbols);
if (symbolTable == NULL)
{
printf("Failed to malloc\n");
exit(4);
}
int i = 0;
while (strcmp(labelFinder, ".END") != 0)
{
char temp;
while (fgets(labelFinder, MAXLENGTHOFLABEL, input) != NULL)
{
if (labelFinder[0] == 'x' || startWithNumber(labelFinder) == 1)
{
continue;
}

}*/
/*while ((temp = (char)fgetc(input)) != EOF)
{
if (i == 21)
{
break;
}
if (temp == ' ')
{
continue;
}
if (temp == '\n')
{
labelFinder[i] = '\n';
break;
}
labelFinder[i] = x
i++;
}*/
/*if (labelFinder == NULL)
{
break;
}
i = 0;

}
//printString(labelFinder,i);

/*for (i = 0; i < maxNumberOfSymbols; i++)
{
symbolTable[i] = malloc(lengthOfHex * sizeof(char));
if (symbolTable == NULL)
{
printf("Failed to malloc\n");
free(symbolTable);
exit(4);
}
}*/
/*symbolTable[0] = "bob";
symbolTable[1] = "susa";
printf("%s, %s", symbolTable[0], symbolTable[1]);
return symbolTable;
}*/