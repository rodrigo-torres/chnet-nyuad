/*
 * main.cpp
 *
 *  Created on: Oct 23, 2019
 *      Author: frao
 */
#include "src/digitizer.h"


static std::unique_ptr<digitizer> mca{nullptr};

extern "C" void relay_signal(int sig)
{
	if (sig == SIGALRM && mca) {
		mca->handle_alarm();
	}
	else {
		printf("[!] Exiting on termination\n");
		mca->exitWithCleanup();
	}
}

void digitizer::set_signals()
{
	auto f = [](int sig){
		auto ret = signal(sig, relay_signal);
		if (ret == SIG_ERR) {
			printf("[!] Unable to catch signal\n");
			exit(1);
		}
	};

	f(SIGINT);
	f(SIGTERM);
	f(SIGQUIT);
	f(SIGALRM);
}

int main(int argc, char* argv[])
{
	mca = std::make_unique<digitizer>();
	mca->open_digitizer();
	mca->start_daq();
	return 0;
}
