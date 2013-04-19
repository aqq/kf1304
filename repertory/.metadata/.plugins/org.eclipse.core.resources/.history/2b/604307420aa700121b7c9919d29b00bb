#include "server.h"
#include "signal.h"

bool have_signal = false;

void stop_handler(int i)
{
	fprintf(stderr,"service stop manually!\n");
	have_signal = true;
};


int main (int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf (stderr, "Usage: %s [port]\n", argv[0]);
		exit (EXIT_FAILURE);
	}
	signal(SIGTERM,stop_handler);
	signal(SIGINT,stop_handler);
	signal(SIGPIPE,SIG_IGN);
	
	server s;
	bool ret = false;
	do
	{
		if(!s.sopen((argv[1])))
		{
			sleep(1);
			continue;
		}
		ret = s.srun();
		s.sclose();
	}while(!ret&&!have_signal);
	return 0;
};

