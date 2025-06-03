#include "jsonparser.h"
uint32_t countcount=0;

/* ********* variables ********* */

char *name = "json";

Map_Typedef start = {
    .mptr = NULL,
    .last = NULL,
    .next = NULL
};
Map_Typedef *present = &start;
Map_Typedef *end = &start;
uint32_t count = 0;
void *note;
size_t allSize = 0;

JSONJSON jsonError = {.error=false, .errorCode=NO_ERROR};

// --------------------------------------- {}
Member_Typedef *headNode = NULL;
Addr_Typedef *useText = NULL;
bool hasData = false;
// --------------------------------------- {}

/* ********* End variables ********* */


/* ********* Functions ********* */


/**
 * param: mem, : mem->startAddress, mem->endAddress
 * 
 * 
 * 
*/
bool memberAnalyze(Member_Typedef *node, Addr_Typedef *mem) {
	//colored("running...\n");
	if ( (startWith('{', mem) == True) && (endWith('}', mem) == True) ) {
		node->type = objectType();
		oTypeAnalyze(node, mem);//colored("running...1\n");

	} else if ( (startWith('[', mem) == True) && (endWith(']', mem) == True) ) {
		node->type = arrayType();
		aTypeAnalyze(node, mem);//colored("running...2\n");

	} else if ( isRightString(mem) == True ) {
		node->type = stringType();
		node->value = getEmptyAddress();
		Addr_Typedef* value = node->value;
		value->startAddress = ski(mem->startAddress, mem->endAddress); //mem->startAddress;
		value->endAddress = skiBack(mem->endAddress, mem->startAddress);//mem->endAddress;//colored("running...3\n");

	} else if (isRightInteger(mem) == True) {
		node->type = integerType();
		node->value = getEmptyAddress();
		Addr_Typedef* value = node->value;
		value->startAddress = mem->startAddress;
		value->endAddress = mem->endAddress;//colored("running...4\n");

	} else if (isRightFloating(mem) == True) {
		node->type = floatingType();
		node->value = getEmptyAddress();
		Addr_Typedef* value = node->value;
		value->startAddress = mem->startAddress;
		value->endAddress = mem->endAddress;//colored("running...5\n");

	} else if (isRightBoolean(mem) == True) {
		node->type = booleanType();
		node->value = getEmptyAddress();
		Addr_Typedef* value = node->value;
		value->startAddress = mem->startAddress;
		value->endAddress = mem->endAddress;

	} else if (stringCompare(mem, "null") == True || stringCompare(mem, "NULL") == True || stringCompare(node->value, "Null") == True) {
		node->type = noneType();
		node->value = getEmptyAddress();
		Addr_Typedef* value = node->value;
		value->startAddress = mem->startAddress;
		value->endAddress = mem->endAddress;//colored("running...6\n");

	}//colored("running...7\n");

	return false;
}

Status_Typedef oTypeAnalyze(Member_Typedef *node, Addr_Typedef *mem) {
	Addr_Typedef memNaked = {.startAddress=ski(mem->startAddress, mem->endAddress)+1, .endAddress=skiBack(mem->endAddress, mem->startAddress)-1};
	bool stillLoop = true;
	uint16_t members = 0;

	Member_Typedef *tempnode = getEmptyMember();
	Member_Typedef *record = tempnode;
	Member_Typedef *firstnode = NULL;

	while (stillLoop) {
		if (memNaked.endAddress-memNaked.startAddress <= 0) {			
			if(members > 0) {
				node->value = firstnode;
				mapedMemoryFree(record);
				return True;
			}
			return False;
		}

		Addr_Typedef *memSymbolLeft;

		if (hasSymbolUnNaked(&memNaked, ',') == True) {
			memSymbolLeft = getFirstSymbolLeftUnNaked(&memNaked, ',');
		} else {
			if (memNaked.endAddress-memNaked.startAddress <= 0) {			
				if(members > 0) {
					node->value = firstnode;
					mapedMemoryFree(record);
					return True;
				}
				return False;
			}
			stillLoop = false;
			memSymbolLeft=&memNaked;
		}

		if (memSymbolLeft == NULL) {
			raiseError(ERROR_VALUE);
			return False;
		} 

		Member_Typedef *nnode = getEmptyMember();

		if (members==0) {
			firstnode = nnode;
		}

		if (hasSymbolUnNaked(memSymbolLeft, ':') != True) {
			raiseError(ERROR_VALUE);
			return False;
		}

		Addr_Typedef *key = getFirstSymbolLeftUnNaked(memSymbolLeft, ':');

		if (key != NULL) {
			if (key->endAddress-key->startAddress >= 0 && isRightString(key) == True) {
				nnode->key->startAddress = key->startAddress;
				nnode->key->endAddress = key->endAddress;
				nnode->hasKey = true;
				mapedMemoryFree(key);
			} else {
				nnode->hasKey = false;
				mapedMemoryFree(nnode->key);
				mapedMemoryFree(key);
				raiseError(ERROR_KEY);
				return False;
			}
		}

		mapedMemoryFree(key);
		key = NULL;
		Addr_Typedef *value = getFirstSymbolRightUnNaked(memSymbolLeft, ':');

		if ((value != NULL) && (value->endAddress-value->startAddress >= 0)) {
			memberAnalyze(nnode, value);
			tempnode->bro = nnode;
			tempnode=nnode;
		}

		mapedMemoryFree(value);
		value = NULL;
		members++;
		memNaked.startAddress = findFirstUnNaked(&memNaked, ',') + 1;
	}

	node->value = firstnode;
	mapedMemoryFree(record);
	return True;
}

Status_Typedef aTypeAnalyze(Member_Typedef *node, Addr_Typedef *mem) {
	Addr_Typedef memNaked = {.startAddress=ski(mem->startAddress, mem->endAddress)+1, .endAddress=skiBack(mem->endAddress, mem->startAddress)-1};
	bool stillLoop = true;
	uint16_t members = 0;

	Member_Typedef *tempnode = getEmptyMember();
	Member_Typedef *record = tempnode;
	Member_Typedef *firstnode = NULL;

	while (stillLoop) {
		if (memNaked.endAddress-memNaked.startAddress <= 0) {			
			if(members > 0) {
				node->value = firstnode;
				mapedMemoryFree(record);
				return True;
			}
			return False;
		}

		Addr_Typedef *memSymbolLeft;

		if (hasSymbolUnNaked(&memNaked, ',') == True) {
			memSymbolLeft = getFirstSymbolLeftUnNaked(&memNaked, ',');
		} else {
			if (memNaked.endAddress-memNaked.startAddress <= 0) {			
				if(members > 0) {
					node->value = firstnode;
					mapedMemoryFree(record);
					return True;
				}
				return False;
			}
			stillLoop = false;
			memSymbolLeft=&memNaked;
		}

		if (memSymbolLeft == NULL) {
			raiseError(ERROR_VALUE);
			return False;
		} 

		Member_Typedef *nnode = getEmptyMember();

		if (members==0) {
			firstnode = nnode;
		}

		Addr_Typedef *key;
		Addr_Typedef *value;

		if (hasSymbolUnNaked(memSymbolLeft, ':') == True) {
			key = getFirstSymbolLeftUnNaked(memSymbolLeft, ':');

			if (key != NULL) {
				if (key->endAddress-key->startAddress >= 0) {
					nnode->key->startAddress = key->startAddress;
					nnode->key->endAddress = key->endAddress;
					nnode->hasKey = true;
					mapedMemoryFree(key);
				} else {
					nnode->hasKey = false;
					mapedMemoryFree(nnode->key);
					mapedMemoryFree(key);
				}
			}

			mapedMemoryFree(key);
			key = NULL;
			value = getFirstSymbolRightUnNaked(memSymbolLeft, ':');
		} else {
			value = getEmptyAddress();
			value->startAddress = memSymbolLeft->startAddress;
			value->endAddress = memSymbolLeft->endAddress;
		}

		if ((value != NULL) && (value->endAddress-value->startAddress >= 0)) {
			memberAnalyze(nnode, value);
			tempnode->bro = nnode;
			tempnode=nnode;
		}

		mapedMemoryFree(value);
		value = NULL;
		members++;
		memNaked.startAddress = findFirstUnNaked(&memNaked, ',') + 1;
	}

	node->value = firstnode;
	mapedMemoryFree(record);
	return True;
}


// String process, ****************************    group id = 2
Addr_Typedef * getFirstSymbolLeftUnNaked(Addr_Typedef *mem, char vau) {
	if (mem->startAddress > mem->endAddress) {
		return NULL;
	}

	Addr_Typedef *retMem = mapedMemoryAllocate(sizeof(Addr_Typedef));

	char *ptr = findFirstUnNaked(mem, vau);

	if (ptr == NULL) {
		return NULL;
	}

	retMem->startAddress = ski(mem->startAddress, mem->endAddress);
	retMem->endAddress = skiBack(ptr-1, mem->startAddress);

	return retMem;
}

// String process, ****************************    group id = 2
Addr_Typedef * getFirstSymbolRightUnNaked(Addr_Typedef *mem, char vau) {
	if (mem->startAddress > mem->endAddress) {
		return NULL;
	}

	Addr_Typedef *retMem = mapedMemoryAllocate(sizeof(Addr_Typedef));

	char *ptr = findFirstUnNaked(mem, vau);

	if (ptr == NULL) {
		return NULL;
	}

	retMem->startAddress = ski(ptr+1, mem->endAddress);
	retMem->endAddress = skiBack(mem->endAddress, ptr+1);

	return retMem;
}

Status_Typedef isRightBoolean(Addr_Typedef *mem) {
	if (mem->startAddress > mem->endAddress) {
		return False;
	}

	mem->startAddress = ski(mem->startAddress, mem->endAddress);
	mem->endAddress = skiBack(mem->endAddress, mem->startAddress);

	if (stringCompare(mem, "true") == True 	|| \
		stringCompare(mem, "True") == True 	|| \
		stringCompare(mem, "false") == True || \
		stringCompare(mem, "False") == True) {
		return True;
	}

	return False;
}

Status_Typedef isRightFloating(Addr_Typedef *mem) {
	if (mem->startAddress > mem->endAddress) {
		return False;
	}

	mem->startAddress = ski(mem->startAddress, mem->endAddress);
	mem->endAddress = skiBack(mem->endAddress, mem->startAddress);

	uint32_t dotCount = 0;

	char *ptr = mem->startAddress;
	char *end = mem->endAddress;
	while(ptr <= end) {
		if ((*ptr > '9') || (*ptr < '+')) {
			return False;
		}

		if (*ptr == '.') {
			dotCount++;
		}

		if (*ptr == '/') {
			return False;
		}

		if ((*ptr == '+' && ptr != mem->startAddress) || (*ptr == '-' && ptr != mem->startAddress)) {
			return False;
		}

		if (*ptr == ',') {
			return False;
		}

		ptr++;
	}

	if (dotCount > 1) {
		return False;
	}

	return True;
}

Status_Typedef isRightInteger(Addr_Typedef *mem) {
	if (mem->startAddress > mem->endAddress) {
		return False;
	}

	mem->startAddress = ski(mem->startAddress, mem->endAddress);
	mem->endAddress = skiBack(mem->endAddress, mem->startAddress);

	char *ptr = mem->startAddress;
	char *end = mem->endAddress;
	while(ptr <= end) {
		if ((*ptr > '9') || (*ptr < '+')) {
			return False;
		}

		if (*ptr == '.') {
			return False;
		}

		if (*ptr == '/') {
			return False;
		}

		if ((*ptr == '+' && ptr != mem->startAddress) || (*ptr == '-' && ptr != mem->startAddress)) {
			return False;
		}

		if (*ptr == ',') {
			return False;
		}

		ptr++;
	}

	return True;
}

Status_Typedef isRightString(Addr_Typedef *mem) {
	if (mem->startAddress > mem->endAddress) {
		return False;
	}

	uint8_t level = 0;
	bool inside = false;

	if ((startWith('\"', mem) == True) && (endWith('\"', mem) == True)) {
		Addr_Typedef tmem = {.startAddress=ski(mem->startAddress, mem->endAddress)+1, .endAddress=skiBack(mem->endAddress, mem->startAddress)-1};

		if (tmem.startAddress > tmem.endAddress) {
			return False;
		}

		char *ptr = tmem.startAddress;
		while (ptr <= tmem.endAddress) {

			if (isEnter(*ptr)) {
				level++;
			} else if (isExit(*ptr)) {
				level--;
			} else if (*ptr == '\\') {
				ptr++;
				if ( (isSymbolOfEscape(*ptr) == True) || (isSymbolAllowed(*ptr, "\"\\/u") != True) ) {
					raiseError(ERROR_C);
					return False;
				}
			} else if (*ptr == '\"') {
				inside = !inside;
			}

			ptr++;
		}

		return True;
	}

	return False;
}


// ***********************************************************
Status_Typedef hasSymbol(Addr_Typedef *mem, char vau) {
	if (mem->startAddress > mem->endAddress) {
		return False;
	}

	char *ptr = mem->startAddress;

	while (ptr <= mem->endAddress) {
		if (*ptr == vau) {
			return True;
		}

		ptr++;
	}

	return False;
}

//
Status_Typedef hasSymbolUnNaked(Addr_Typedef *mem, char vau) {
	if (mem->startAddress > mem->endAddress) {
		return False;
	}

	uint8_t level = 0;
	bool inside = false;

	char *ptr = mem->startAddress;

	while (ptr <= mem->endAddress) {
		if (isEnter(*ptr)) {
			level++;
		} else if (isExit(*ptr)) {
			level--;
		} else if (*ptr == '\\') {
			ptr++;
			if ( (isSymbolOfEscape(*ptr) == True) || (isSymbolAllowed(*ptr, "\"\\/u") != True) ) {
				raiseError(ERROR_C);
				return False;
			}
		} else if (*ptr == '\"') {
			inside = !inside;
		}

		if ( (*ptr == vau) && (level == 0) && !inside) {
			return True;
		}

		ptr++;
	}

	return False;
}

// String process, ****************************    group id = 2
char * findFirstUnNaked(Addr_Typedef *mem, char vau) {
	if (mem->startAddress > mem->endAddress) {
		return NULL;
	}

	uint8_t level = 0;
	bool inside = false;

	char *ptr = mem->startAddress;

	while (ptr <= mem->endAddress) {
		if (isEnter(*ptr)) {
			level++;
		} else if (isExit(*ptr)) {
			level--;
		} else if (*ptr == '\\') {
			ptr++;
			if ( (isSymbolOfEscape(*ptr) == True) || (isSymbolAllowed(*ptr, "\"\\/u") != True) ) {
				raiseError(ERROR_C);
				return NULL;
			}
		} else if (*ptr == '\"') {
			inside = !inside;
		}

		if ( (*ptr == vau) && (level == 0) && !inside) {
			return ptr;
		}

		ptr++;
	}

	return NULL;
}

// String process, ****************************    group id = 2
char * findFirst(Addr_Typedef *mem, char vau) {
	if (mem->startAddress > mem->endAddress) {
		return NULL;
	}

	char *ptr = mem->startAddress;

	while (ptr <= mem->endAddress) {
		if (*ptr == vau) {
			return ptr;
		}
		ptr++;
	}

	return NULL;
}

// String process, ****************************    group id = 2
bool isEnter(char vau) {
	char *p = "{[";
	while(*p != '\0') {
		if (vau == *p) {
			return true;
		}
		p++;
	}
	return false;
}

// String process, ****************************    group id = 2
bool isExit(char vau) {
	char *p = "}]";
	while(*p != '\0') {
		if (vau == *p) {
			return true;
		}
		p++;
	}
	return false;
}


// String process, ****************************    group id = 2
Status_Typedef startWith(char vau, Addr_Typedef *mem) {
	char *ptr = ski(mem->startAddress, mem->endAddress);

	if (ptr > mem->endAddress) {
		return Error;
	}

	if (*ptr == vau) {
		return True;
	}

	return False;
}

// String process, ****************************    group id = 2
Status_Typedef endWith(char vau, Addr_Typedef *mem) {
	char *ptr = skiBack(mem->endAddress, mem->startAddress);

	if (ptr <= mem->startAddress) {
		return Error;
	}

	if (*ptr == vau) {
		return True;
	}

	return False;
}

// String process, ****************************    group id = 2
bool isSpecial(char vau) {
	char *p = "\r\n \t\v\e\a\f";
	while(*p != '\0') {
		if (vau == *p) {
			return true;
		}
		p++;
	}
	return false;
}

// String process, ****************************    group id = 2
char *ski(char *ptr, char *end) {
	if (ptr > end) {
		return ptr;
	}

	while( isSpecial(*ptr) && (ptr < end) ) {
		ptr++;
	}
	return ptr;
}

// String process, ****************************    group id = 2
char *skiBack(char *ptr, char *end) {
	if (ptr < end) {
		return ptr;
	}

	while( isSpecial(*ptr) && (ptr > end) ) {
		ptr--;
	}
	return ptr;
}

// Map empty member node, ****************************    group id = 3
DatabackU_Typedef * getEmptyUData(void) {
	DatabackU_Typedef * du = mapedMemoryAllocate(sizeof(DatabackU_Typedef));
	return du;
}


// Map empty member node, ****************************    group id = 3
Member_Typedef * getEmptyMember(void) {
	Member_Typedef *mb = mapedMemoryAllocate(sizeof(Member_Typedef));
	mb->key = getEmptyAddress();
	mb->hasKey = false;
	mb->type = 'u';
	mb->value = NULL;
	mb->valueLength = 0;
	mb->bro = NULL;

	return mb;
}

//
Addr_Typedef * getEmptyAddress(void) {
	Addr_Typedef *addrNode = mapedMemoryAllocate(sizeof(Addr_Typedef));
	addrNode->startAddress = NULL;
	addrNode->endAddress = NULL;
	return addrNode;
}


// Can be optimized by hash table
// Maped memory *malloc()*, ****************************    group id = 1
void * mapedMemoryAllocate(size_t size) {
	void * j = malloc(size);
	count++;
	Map_Typedef *nf = malloc(sizeof(Map_Typedef));
	nf->last = present;
	present->next = nf;
	present = nf;
	present->mptr = j;
	present->size = size;
	allSize += size;

	return j;
}

// Maped memory *free()*, ****************************    group id = 1
bool mapedMemoryFree(void *ptr) {
	if (ptr == NULL) {
        return false;
    }

	Map_Typedef *now = &start;
	while (now != present) {
		if (now->mptr == ptr) {
			free(now->mptr);
			now->mptr = NULL;
			
			Map_Typedef *last = now->last;
			Map_Typedef *next = now->next;

			if (last != NULL) {
                last->next = next;
            }
            if (next != NULL) {
                next->last = last;
            }

			free(now);
			now = NULL;

			count--;

			return true;
		}

		now = now->next;
	}

	return false;
}

// Maped memory *free()*, ****************************    group id = 1
void freeAllMap(void) {
	Map_Typedef *ptr;
	while(present != &start) {
		free(present->mptr);
		present->mptr = NULL;
		allSize -= present->size;
		ptr = present;
		present = present->last;
		free(ptr);
		count--;
	}
}

void raiseError(Error_Typedef code) {
	jsonError.error = true;
	jsonError.errorCode |= code;
}

Error_Typedef getErrorCode(void) {
	return jsonError.errorCode;
}


//
char * endOfString(char *text) {
	return text + strlen(text) - 1;
}

//
Status_Typedef isSymbolOfEscape(char vau) {
	char *p = "rnveaf";
	while(*p != '\0') {
		if (vau == *p) {

			return True;
		}
		p++;
	}
	return False;
}

Status_Typedef isSymbolAllowed(char vau, char *p) {
	while(*p != '\0') {
		if (vau == *p) {
			return True;
		}
		p++;
	}
	return False;
}

//
char objectType(void) {
	return 'o' ;
}

char arrayType(void) {
	return 'a' ;
}

char stringType(void) {
	return 's' ;
}

char integerType(void) {
	return 'i' ;
}

char floatingType(void) {
	return 'f' ;
}

char booleanType(void) {
	return 'b' ;
}

char noneType(void) {
	return 'n' ;
}

char undefinedType(void) {
	return 'u' ;
};

/* ********* Functions ********* */


//***************************************************
void colored(char *param) {
	printf("\033[91m");
	printf(param);
	printf("\033[0m");
}

void printS(Addr_Typedef *mem) {
	if (mem->startAddress != NULL && mem->endAddress != NULL) {
		char *ptr = mem->startAddress;
		char *end = mem->endAddress;
		while (ptr <= end) {
			printf("%c", *ptr);
			ptr++;
		}
		printf("\n");
	}
}

void stringCopy(char *purpose, Addr_Typedef *mem) {
	if (mem->startAddress > mem->endAddress) {
		return;
	}

	char *ptr = mem->startAddress+1;
	char *end = mem->endAddress-1;
	while (ptr <= end) {
		*purpose++ = *ptr++;
	}
	*purpose = '\0';
}

Status_Typedef stringCompare(Addr_Typedef *mem, char *comp) {
	if (mem->startAddress > mem->endAddress) {
		return False;
	}

	if ((uint32_t)(mem->endAddress-mem->startAddress+1) != strlen(comp)) {
		return False;
	}

	char *ptr = mem->startAddress;
	char *end = mem->endAddress;
	while (ptr <= end) {
		if (*ptr++ != *comp++) {
			return False;
		}
	}

	return True;
}

Status_Typedef stringComp(Addr_Typedef *mem1, Addr_Typedef *mem2) {
	if ((mem1->endAddress-mem1->startAddress != mem2->endAddress-mem2->startAddress) || (mem1->startAddress>=mem1->endAddress)) {
		return False;
	}

	char *ptr1 = mem1->startAddress;
	char *ptr2 = mem2->startAddress;
	char *end = mem1->endAddress;
	while (ptr1 <= end) {
		if (*ptr1++ != *ptr2++) {
			return False;
		}
	}

	return True;
}

int stringToInteger(Addr_Typedef *mem) {
	char *ptr = mem->startAddress;
	char *end = mem->endAddress;
	int ret = 0;

	int label = 1;
	if (startWith('+', mem) == True) {
		label = 1;
		ptr++;
		mem->startAddress++;
	} else if (startWith('-', mem) == True) {
		label = -1;
		ptr++;
		mem->startAddress++;
	}

	if (startWith('.', mem) == True) {
		return -9999;
	} else if (hasSymbol(mem, '.') == True) {
		return -9999;
	}

	while (ptr <= end) {
		ret *= 10;
		ret += *ptr - '0';
		ptr++;
	}

	return ret * label;
}

float stringToFloating(Addr_Typedef *mem) {
	char *ptr = mem->startAddress;
	char *end = mem->endAddress;
	float ret = 0;
	float add = 0.0;

	float label = 1.0;
	if (startWith('+', mem) == True) {
		label = 1.0;
		ptr++;
		mem->startAddress++;
	} else if (startWith('-', mem) == True) {
		label = -1.0;
		ptr++;
		mem->startAddress++;
	}

	if (startWith('.', mem) == True) {
		return -9999;
	}

	while (ptr <= end) {
		ret *= 10;
		ret += *ptr - '0';
		ptr++;
		if (*ptr == '.') {
			ptr++;
			break;
		}

		if (*ptr < '0' || *ptr > '9') {
			break;
		}
	}

	while (ptr <= end) {
		if (*ptr < '0' || *ptr > '9') {
			break;
		}
		add += *end - '0';
		add *= 0.1;
		end--;
	}

	return (ret + add) * label;
}


//	# ---------------------------------------------------------------------------------------------------------------
//	# ---------------------------------- hash quary functions -------------------------------------------------------
Member_Typedef * quaryNodeWithStructureLanguage(Member_Typedef *node, char *key) {
	if (node == NULL || strlen(key) < 1) {
		return NULL;
	}

	node = node->value;

	Addr_Typedef *mem = getEmptyAddress();
	mem->startAddress = key;
	mem->endAddress = key + strlen(key) - 1;

	Addr_Typedef *moveptr = getEmptyAddress();
	moveptr->startAddress = key;

	char *ptr = key;

	while (startWith('.', mem) == True && mem->startAddress <= mem->endAddress) {
		ptr++;
		mem->startAddress++;
		moveptr->startAddress++;
	}

	while (endWith('.', mem) == True && mem->startAddress <= mem->endAddress) {
		mem->endAddress--;
	}

	// - | -----------------------------------------------------------
	char *pdot = NULL;
loop:
	pdot = findFirst(mem, '.');

	if (pdot == NULL) {
		if (hasSymbol(mem, '[') == True || hasSymbol(mem, ']') == True) {
			node = getNodeWithArray(node, mem);
			mapedMemoryFree(mem);
			mapedMemoryFree(moveptr);
			return node;
		}

		node = getNodeWithKey(node, mem);
		mapedMemoryFree(mem);
		mapedMemoryFree(moveptr);
		return node;
	}

	moveptr->endAddress = pdot-1;
	mem->startAddress = pdot+1;

	if (hasSymbol(moveptr, '[') == True || hasSymbol(moveptr, ']') == True) {
		node = getNodeWithArray(node, moveptr);
	} else {
		node = getNodeWithKey(node, moveptr);
	}

	if (node == NULL) {
		mapedMemoryFree(mem);
		mapedMemoryFree(moveptr);
		return NULL;
	} else if (node->type != objectType() && node->type != arrayType()) {
		mapedMemoryFree(mem);
		mapedMemoryFree(moveptr);
		return NULL;
	} else {
		node = node->value;
		moveptr->startAddress = pdot+1;
	}

	goto loop;
}

Member_Typedef * getNodeWithArray(Member_Typedef *node, Addr_Typedef *mem) {
	if (mem->startAddress > mem->endAddress) {
		return NULL;
	}

	int num = 0;
	bool count = false;
	bool name = true;
	bool inside = false;

	char *ptr = mem->startAddress;
	char *end = mem->endAddress;

	while (ptr <= end) {
		if (*ptr == '[') {
			num = 0;
			count = true;
			inside = true;
			mem->endAddress = ptr-1;
			if (name) {
				node = getNodeWithKey(node, mem);
				if (node == NULL) {
					return NULL;
				}
				name = false;
			}
		} else if (*ptr == ']') {
			if (!inside) {
				return NULL;
			}
			inside = false;
			node = getArrayNode(node, num);
			if (node == NULL) {
				return NULL;
			}
			count = false;
		}

		if (count && (*ptr>='0') && (*ptr<='9')) {
			num *= 10;
			num += *ptr - '0';
		}

		ptr++;
	}

	return node;
}


Member_Typedef * getArrayNode(Member_Typedef *node, uint16_t num) {
	node = node->value;

	for (int i=0; i < num; i++) {
		node = node->bro;
		if (node == NULL) {
			return NULL;
		}
	}

	return node;
}

Member_Typedef * getNodeWithKey(Member_Typedef *node, Addr_Typedef *mem) {
	if (mem->startAddress > mem->endAddress) {
		return NULL;
	}

	Addr_Typedef key;

	while (node != NULL) {
		if (!node->hasKey) {
			node = node->bro;
			continue;
		}

		key.startAddress = node->key->startAddress + 1;
		key.endAddress = node->key->endAddress - 1;

		if (stringComp(&key, mem) == True) {
			return node;
		}
		node = node->bro;
	}
	return NULL;
}


//	# ---------------------------------- hash quary functions end ---------------------------------------------------
//	# ---------------------------------------------------------------------------------------------------------------




//	# ---------------------------------------------------------------------------------------------------------------
//	# -------------------------------------- Analyze to value -------------------------------------------------------
Member_Typedef * anaObject(Member_Typedef *node) {
	if (node->type != objectType()) {
		return NULL;
	}

	return node;
}

Member_Typedef * anaArray(Member_Typedef *node) {
	if (node->type != arrayType()) {
		return NULL;
	}

	return node;
}

char * anaString(Member_Typedef *node) {
	if (node->type != stringType()) {
		return NULL;
	}

	Addr_Typedef *value = node->value;

	char *word = mapedMemoryAllocate((size_t)(value->endAddress-value->startAddress+1 - 1));//(word + '\0' - 2*"\"")
	char *ptr = word;
	char *start = value->startAddress+1;
	char *end = value->endAddress-1;
	//printS(value);
	while (start <= end) {
		//printf("%c", *start);
		*ptr++ = *start++;
	}//printf("\n");
	*ptr = '\0';

	return word;
}

int anaInteger(Member_Typedef *node) {
	if (node->type != integerType()) {
		return -9999;
	}

	int ret = stringToInteger(node->value);
	return ret;
}

float anaFloating(Member_Typedef *node) {
	if (node->type != floatingType()) {
		return -8888.0;
	}

	float ret = stringToFloating(node->value);
	return ret;
}

bool anaBoolean(Member_Typedef *node) {
	if (node->type != booleanType()) {
		return false;
	}

	if (stringCompare(node->value, "true") == True) {
		return true;
	}

	return false;
}

void * anaNull(Member_Typedef *node) {
	if (node->type != noneType()) {
		return NULL;
	}

	if (stringCompare(node->value, "null") != True ||	\
		stringCompare(node->value, "NULL") != True ||	\
		stringCompare(node->value, "Null") != True) {
		return node;
	}

	return NULL;
}


//	# ------------------------------------ Analyze to value end -----------------------------------------------------
//	# ---------------------------------------------------------------------------------------------------------------


// # --
// outer functions //(char *addr, uint32_t length)
Json CreateJson(void) {
	Json j = {
		.load=load,
		.parse=parse,
		.quary=quary,
		.release=release,

		.add=add
	};

	return j;
}

bool load(char *addr, uint32_t length) {
	if (addr == NULL || length <= 0) {
		return false;
	}

	headNode = getEmptyMember();
	headNode->key->startAddress = name;
	headNode->key->endAddress = name + strlen(name) - 1;
	headNode->hasKey = true;
	headNode->type = objectType();

	useText = getEmptyAddress();
	useText->startAddress = addr;
	useText->endAddress = addr + length - 1;

	hasData = true;

	return true;
}

void parse(void) {
	if (!hasData) {
		return;
	}

	memberAnalyze(headNode, useText);
}

Answer quary(char *message) {
	Member_Typedef *node = quaryNodeWithStructureLanguage(headNode, message);
	Answer ret = {.OK=false};

	if (node == NULL) {
		ret.OK = false;
		return ret;
	}

	if (node->type == objectType()) {
		Member_Typedef *objectAnswer = anaObject(node);
		if (objectAnswer == NULL) {
			ret.OK = false;
			return ret;
		}

		ret.OK = true;
		ret.data.object = objectAnswer;
		ret.datatype = OBJECT;
		return ret;

	} else if (node->type == arrayType()) {
		Member_Typedef *arrayAnswer = anaArray(node);
		if (arrayAnswer == NULL) {
			ret.OK = false;
			return ret;
		}

		ret.OK = true;
		ret.data.array = arrayAnswer;
		ret.datatype = ARRAY;
		return ret;

	} else if (node->type == stringType()) {
		char *stringAnswer = anaString(node);
		if (stringAnswer == NULL) {
			ret.OK = false;
			return ret;
		}

		ret.OK = true;
		ret.data.string = stringAnswer;
		ret.datatype = STRING;
		return ret;

	} else if (node->type == integerType()) {
		int integerAnswer = anaInteger(node);
		ret.OK = true;
		ret.data.integer = integerAnswer;
		ret.datatype = INTEGER;
		return ret;

	} else if (node->type == floatingType()) {
		float floatingAnswer = anaFloating(node);
		ret.OK = true;
		ret.data.floating = floatingAnswer;
		ret.datatype = FLOATING;
		return ret;

	} else if (node->type == booleanType()) {
		bool booleanAnswer = anaBoolean(node);
		ret.OK = true;
		ret.data.boolean = booleanAnswer;
		ret.datatype = BOOLEANTY;
		return ret;

	} else if (node->type == noneType()) {
		void * nullAnswer = anaNull(node);
		ret.OK = true;
		ret.data.null = nullAnswer;
		ret.datatype = NULLTY;
		return ret;
	}

	ret.OK = false;
	ret.datatype = NONE;
	return ret;

}


void release(void *rel) {
	if (rel != NULL) {
		mapedMemoryFree(rel);
	} else {
		freeAllMap();
	}
}

void showtype(Datatype_Typedef type) {

	switch (type) {
	case OBJECT:	printf("object\n"); 	break;
	case ARRAY:		printf("array\n"); 		break;
	case STRING:	printf("srting\n");		break;
	case INTEGER:	printf("integer\n"); 	break;
	case FLOATING:	printf("floating\n");	break;
	case BOOLEANTY:	printf("boolean\n");	break;
	case NULLTY:	printf("null\n");		break;
	default:		printf("none\n");		break;
	}

}

void add(const char *format, ...) {
	va_list args;
	va_start(args, format);

	Member_Typedef *node = va_arg(args, void*);
	while (node != NULL) {
		node = va_arg(args, void*);
		printf("add a node\n");
	} ;
}












