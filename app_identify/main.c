#include <stdio.h>
#include <pcap.h>
#include <regexParse.h>
#include "session.h"

#define LINE_LEN 16

extern struct dfa_graph_t* load_sig(char *file);
extern void graph_dump(struct dfa_graph_t *graph);
extern void packet_handler(u_char *param, const struct pcap_pkthdr *pkthdr, const u_char *pktdata);

extern struct dfa_graph_t *appgraph;

int resource_init()
{
	load_sig("sig.conf");
	//graph_dump(appgraph);

	create_session_table();
	
	return 0;
}

int main(int argc, char **argv)
{
	pcap_t *fp;
	char errbuf[PCAP_ERRBUF_SIZE];
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	u_int i=0;
	int res;

	if(argc != 2)
	{	
		printf("usage: %s filename", argv[0]);
		return -1;

	}

	resource_init();

	/* Open the capture file */
	if ((fp = pcap_open_offline(argv[1],			// name of the device
						 errbuf					// error buffer
						 )) == NULL)
	{
		fprintf(stderr,"\nUnable to open the file %s.\n", argv[1]);
		return -1;
	}
	
	/* Retrieve the packets from the file */
	while((res = pcap_next_ex(fp, &header, &pkt_data)) >= 0)
	{
		/* print pkt timestamp and pkt len */
		//printf("%ld:%ld (%ld)\n", header->ts.tv_sec, header->ts.tv_usec, header->len);	
		packet_handler(NULL, header, pkt_data);
	}
	
	dump_session_table();

	if (res == -1)
	{
		printf("Error reading the packets: %s\n", pcap_geterr(fp));
	}
	
	pcap_close(fp);
	return 0;
}

