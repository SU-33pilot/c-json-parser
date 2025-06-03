/**
  **************************************************
  *	@brief:		This is a c-file for parsing json string. A new choice to c-parser-json.
  * 				This file has characteristics, they are:
  * 					1) more bugs,
  * 					2) only parse,
  * 					3) a new choice. (for parse)
  *	@author:	su33-pilot
  **************************************************
  *		This is a c-parse-json file.
  **************************************************
  *		I don't know what should be write in here.
  **************************************************
  */
/**************************************************/


/* 	
	See the use functions of this file, goto line 239-262
*/

/* 
	There has a lot of functions no use, but I can't found them all. 
	I had try to find them, but they are seem to be useful, so I give up, just keep it OK.
*/

#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef struct JSONVALUE {
	char *startAddress;
	char *endAddress;
}Addr_Typedef;

typedef struct DUAL {

}DualNode_Typedef;

typedef struct MEMBER {
	Addr_Typedef *key;
	bool hasKey;
	char type;
	void *value;
	uint16_t valueLength;
	struct MEMBER *bro;
}Member_Typedef;

typedef struct Map {
	void *mptr;
	void *last;
	void *next;
	size_t size;
}Map_Typedef;

typedef struct MAPNODE {
	Member_Typedef *node;
	struct MAPNODE *last;
	struct MAPNODE *next;
}Mapnode_Typedef;

typedef struct LEVELNODE {
	Member_Typedef *node;
	struct MAPNODE *last;
	struct MAPNODE *next;
}Levelnode_Typedef;

typedef enum STATUS {
	True = 0,
	False,
	Error,
	Invalid
}Status_Typedef;

typedef enum {
	NO_ERROR=0x00,
	ERROR_KEY=0x01,
	ERROR_VALUE=0x02,
	ERROR_B=0X04,
	ERROR_C=0X08,
	ERROR_D=0X10,
	ERROR_E=0X20
}Error_Typedef;

typedef struct {
	bool error;
	Error_Typedef errorCode;
}JSONJSON;


typedef union FUNCTION {
	char * (*getString)(void);
	char (*getChar)(void);
	int (*getIndeger)(void);
}FunctionU_Typedef;

typedef union DATABACK {
	char character;
	char *string;
	int indeger;
	double floating;
	bool boolean;
}DatabackU_Typedef;

uint8_t hashMod(char *vau, uint8_t modNum);
uint32_t hashAddress(void *addr, uint32_t modNum);

bool memberAnalyze(Member_Typedef *node, Addr_Typedef *mem);
Status_Typedef oTypeAnalyze(Member_Typedef *node, Addr_Typedef *mem);
Status_Typedef aTypeAnalyze(Member_Typedef *node, Addr_Typedef *mem);

Addr_Typedef * getFirstSymbolLeftUnNaked(Addr_Typedef *mem, char vau);
Addr_Typedef * getFirstSymbolRightUnNaked(Addr_Typedef *mem, char vau);

Status_Typedef isRightBoolean(Addr_Typedef *mem);
Status_Typedef isRightFloating(Addr_Typedef *mem);
Status_Typedef isRightInteger(Addr_Typedef *mem);
Status_Typedef isRightString(Addr_Typedef *mem);

char * findFirstUnNaked(Addr_Typedef *mem, char vau);
Status_Typedef hasSymbol(Addr_Typedef *mem, char vau);
Status_Typedef hasSymbolUnNaked(Addr_Typedef *mem, char vau);
char * findFirst(Addr_Typedef *mem, char vau);

bool isEnter(char vau);
bool isExit(char vau);

Status_Typedef startWith(char vau, Addr_Typedef *mem);
Status_Typedef endWith(char vau, Addr_Typedef *mem);
bool isSpecial(char vau);
char *ski(char *ptr, char *end);
char *skiBack(char *ptr, char *end);

DatabackU_Typedef * getEmptyUData(void);
Member_Typedef * getEmptyMember(void);
Addr_Typedef * getEmptyAddress(void);
void * mapedMemoryAllocate(size_t size);
bool mapedMemoryFree(void *ptr);
void freeAllMap(void);

void raiseError(Error_Typedef code);
Error_Typedef getErrorCode(void);

//
char * endOfString(char *text);



//
Status_Typedef isSymbolOfEscape(char vau);

Status_Typedef isSymbolAllowed(char vau, char *p);


//
char objectType(void);

char arrayType(void);

char stringType(void);

char integerType(void);

char floatingType(void);

char booleanType(void);

char noneType(void);

char undefinedType(void);

extern JSONJSON jsonError;



// # --     
void colored(char *param);
void printS(Addr_Typedef *mem);  

void stringCopy(char *purpose, Addr_Typedef *mem);
Status_Typedef stringCompare(Addr_Typedef *mem, char *comp);
Status_Typedef stringComp(Addr_Typedef *mem1, Addr_Typedef *mem2);
int stringToInteger(Addr_Typedef *mem);
float stringToFloating(Addr_Typedef *mem);

Member_Typedef * quaryNodeWithStructureLanguage(Member_Typedef *node, char *key);
Member_Typedef * getNodeWithArray(Member_Typedef *node, Addr_Typedef *mem);
Member_Typedef * getArrayNode(Member_Typedef *node, uint16_t num);
Member_Typedef * getNodeWithKey(Member_Typedef *node, Addr_Typedef *mem);




// outer
typedef enum DTYPE {
	OBJECT=0,
	ARRAY,
	STRING,
	INTEGER,
	FLOATING,
	BOOLEANTY,
	NULLTY,
	NONE
}Datatype_Typedef;

typedef union DATA {
	char *string;
	int integer;
	float floating;
	bool boolean;
	Member_Typedef *object;
	Member_Typedef *array;
	Member_Typedef *null;
}Data_Typedef;

typedef struct ANSWER {
	bool OK;
	Data_Typedef data;
	Datatype_Typedef datatype;
}Answer;

typedef struct JSON {
	bool (*load)(char *, uint32_t);
	void (*parse)(void);
	Answer (*quary)(char *);
	void (*release)(void *);
	void (*add)(const char *, ...);
}Json;




// This is the major functions.

//
// This function is aim to create a json structure has below functions.
Json CreateJson(void); // (char *addr, uint32_t length)

// This function is aim to load json string like <{"key":"value", "array": [...]>, 
// addr is address of '{' and length is '{' to '}' (include '{' and '}')
bool load(char *addr, uint32_t length);

// This function is aim to parse json string, use it after 'json.load(addr, len)'. 
void parse(void);

// This function is aim to quary value of 'key', for example: Answer answer = json.quary("key") or ...=json.quary("array[0]"). 
Answer quary(char *message);

// This function is aim to release memory which was malloc-ed, 
// if you want to release a single address, use json.release(addr),
// if you want to release all memory of parser used (exit from parser), use json.release(NULL);
/*
   Notice!!! 
   If answer's type is string, 
   you should use json.release(answer.data.string), 
   because Answer.data.string is a pointer of memory-malloc, 
   so you should free it.
*/
void release(void *rel);

// Answer structure has a member "Answer.datatype", Answer.data is a union <string, integer, floating...>,
// So, if you want to get a type's data, you should check if datatype is right. This function can printf datatype, use it at debug time.
void showtype(Datatype_Typedef type);

// I have forget what the use of this function, just keep it here, I think it's no error if you wanna delete it.
void add(const char *format, ...);

/*
		About this file, I write it and no refine it, because it has work-able now, 
	I hope to help people ilke me, who is difficult to understand c-language and 
	need a simple use of parse json string. Thank you.
		💡💡💡With english almost no error, translate it. 💡💡💡
*/

#endif

