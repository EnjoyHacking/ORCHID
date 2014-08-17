#include <regexParse.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
__declspec(dllimport) struct dfa_graph_t *Regex_Parse(struct app_entry_t *apps);
__declspec(dllexport) void Regex_Test(struct app_entry_t *apps, char *Text);
#ifdef __cplusplus
}
#endif

struct dfa_graph_t *graph = NULL;

struct app_entry_t *new_app_sig(int id, char *sig)
{
	struct app_entry_t *entry;

	if (strlen(sig) >= MAX_SIG_LENGTH)
		return NULL;

	entry = (struct app_entry_t *)malloc(sizeof(struct app_entry_t));

	if (entry) {
		entry->appId = id;
		entry->next = NULL;
		strcpy(entry->regex, sig);
	}

	return entry;
}

struct dfa_graph_t* load_sig()
{
	struct app_entry_t *apps = NULL;

	apps = new_app_sig(1, "GET.*youku");
	apps->next = new_app_sig(2, "GET.*tudou");

	Regex_Test(apps, "sdfsdfGETsdf\r\nhskdjfyoukudsflksdjflk");

	return Regex_Parse(apps);
}

void graph_dump(struct dfa_graph_t *graph)
{
	ushort stateNum = graph->stateNum;
	ushort alphabetSize = graph->alphabetSize;
	uchar  *alphabetTable = graph->alphabetTable;
	ushort *nextTable = graph->nextTable;
	uchar i;
	ushort j;

	printf("graph info:\n\t stateNum: %d\n\talphabetSize: %d\n", stateNum, alphabetSize);

	printf("alphabetTable:\n");

	for(i = 'a'; i <= 'z'; i++){
		printf("%3c ", i);
	}
	printf("\n");

	for (i = 'a'; i <= 'z'; i++) {
		printf("%3d ", alphabetTable[i]);
	}
	printf("\n");


	printf("stateTable:\n");
	for (j = 0; j < stateNum; j++) {
		printf("state %d:\n", j);
		for (i = 0; i < alphabetSize; i++)
			printf("%4d ", nextTable[i]);
		
		printf("\n");
		nextTable += alphabetSize;
	}

}