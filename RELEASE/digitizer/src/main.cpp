/*
 * main.cpp
 *
 *  Created on: Oct 23, 2019
 *      Author: frao
 */
#include <src/digitizer.h>

namespace daq_signals
{
static digitizer* mca_handle = nullptr;

static void relay_signal(int signal)
{
	if (mca_handle)
		mca_handle->handle_termination(signal);
	else
		printf("[!] CAEN MCA not opened yet");
}

static void relay_alarm(int signal)
{
	if (mca_handle)
		mca_handle->handle_alarm();
	else
		printf("[!] CAEN MCA not opened yet");
}

void set_signals()
{
	/* Signals */
	__sighandler_t ret;
	signal(SIGINT, relay_signal);
	signal(SIGTERM, relay_signal);
	signal(SIGQUIT, relay_signal);
	ret = signal(SIGALRM, relay_alarm);
	if (ret == SIG_ERR)
	{
		perror("[!] Unable to catch SIGALRM");
		exit(1);
	}
}
}

int main(int argc, char* argv[])
{
	using namespace daq_signals;
	set_signals();
	mca_handle = new digitizer;
	mca_handle->open_digitizer();
	mca_handle->start_daq();
	/* Data saving protocol goes here */
	delete mca_handle;
	return 0;
}
