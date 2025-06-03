#include "jsonparser.h"
#include <windows.h>

void test(Answer *answer, Json *json) {
	if (!answer->OK)
		return;

	showtype(answer->datatype);
	switch (answer->datatype) {
	case STRING:
		printf("%s\n", answer->data.string);
		json->release(answer->data.string);
		break;
	case FLOATING:
		printf("%f\n", answer->data.floating);
		break;
	case INTEGER:
		printf("%d\n", answer->data.integer);
		break;
	case BOOLEANTY:
		if (answer->data.boolean) 
			printf("true\n");
		else 
			printf("false\n");
		break;
	case NULLTY:
		if (answer->data.null != NULL) 
			printS(answer->data.null->value);
		else
			printf("NONE");
		break;
	default: break;
	}
}


int main() {

	char *code = "{        \n\n     \"key\": \"value\" , \"pi\":  3.14, \"new-object\": {\"name\": \"value\" , \"pi\":  3.14, \"object\": {,}, \"array\":[ 199, \"lucky\", \"coming\"]}, \"new-array\":[ null, null]}";

	uint32_t len = strlen(code);

	system("pause");

	Json json = CreateJson();
	json.load(code, len);
	json.parse();

	printf("Error's value is: %d\n", jsonError.errorCode);

	Answer answer;
	char input[100];

	while(strstr(input, "end") == NULL) {
		colored("\nYour can input what you wanna quary: ");
		gets(input);

		answer = json.quary(input);

		test(&answer, &json);
	}

	system("pause");
	return 0;
}











