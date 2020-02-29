
#include <iostream>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

#include <stdio.h>
#include <signal.h>
#include <syslog.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filewritestream.h>

#include "Config.hpp"
//#include "ActionFactory.hpp"
//#include "actions/Action.hpp"
//#include "Session.hpp"
//#include "SessionManager.hpp"
//#include "AppWebSocketWriter.hpp"
//#include "AppCtx.hpp"

//#include "PCA9685.hpp"
//#include "HcSr04.hpp"

#include <wiringPi.h>

//#define HERTZ 50

#define DAEMON_NAME "simpledaemon"

using namespace std;
using namespace rapidjson;

namespace tamageta {

class Alexo {
	public:
		Alexo();
		void init();
		void daemonize(string rundir, string pidfile);
		void run();

	private:
		static void signal_handler(int sig);
		static void sighandler(int sig);
		static void daemonShutdown();

		int force_exitx;
		static int force_exit;
		static int pidFilehandle;
};

int Alexo::force_exit = 0;
int Alexo::pidFilehandle;

Alexo::Alexo(){
	force_exitx = 0;
}

void Alexo::daemonShutdown(){
//	(pidFilehandle);
}

void Alexo::signal_handler(int sig){
	switch(sig){
		case SIGHUP:
		case SIGINT:
		case SIGTERM:
			daemonShutdown();
			exit(EXIT_SUCCESS);
			break;
		default:
			break;
	}
}

void Alexo::daemonize(string rundir, string pidfile) {
	int pid, sid, i;
	char str[10];
	struct sigaction newSigAction;
	sigset_t newSigSet;

	if (getppid() == 1) {
		return;
	}

	sigemptyset(&newSigSet);
	sigaddset(&newSigSet, SIGCHLD);
	sigaddset(&newSigSet, SIGTSTP);
	sigaddset(&newSigSet, SIGTTOU);
	sigaddset(&newSigSet, SIGTTIN);
	sigprocmask(SIG_BLOCK, &newSigSet, NULL);

	newSigAction.sa_handler = signal_handler;
	sigemptyset(&newSigAction.sa_mask);
	newSigAction.sa_flags = 0;

	sigaction(SIGHUP, &newSigAction, NULL);
	sigaction(SIGTERM, &newSigAction, NULL);
	sigaction(SIGINT, &newSigAction, NULL);

	// fork returns child process id if process is parent or 0 if process is child process
	// this kills the parent process to be independent process and under init/kernel process
	pid = fork();
	if (pid < 0) {
		exit(EXIT_FAILURE);
	}

	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	umask(027);

	sid = setsid();

	if (sid < 0) {
		exit(EXIT_FAILURE);
	}

	for (i = getdtablesize(); i >= 0; --i) {
		close(i);
	}

	i = open("/dev/null", O_RDWR);

	dup(i);
	dup(i);

	chdir(rundir.c_str());

	pidFilehandle = open(pidfile.c_str(), O_RDWR|O_CREAT, 0600);

	if (pidFilehandle == -1 ) {
//		BOOST_LOG_TRIVIAL(info) << "daemonize.pidFilehandle" << pidfile;
		exit(EXIT_FAILURE);
	}

	if (lockf(pidFilehandle,F_TLOCK,0) == -1) {
		exit(EXIT_FAILURE);
	}

	sprintf(str,"%d\n",getpid());

	write(pidFilehandle, str, strlen(str));

	close(pidFilehandle);
}

void Alexo::sighandler(int sig)
{
	force_exit = 1;
}

void Alexo::init(){
	int size = 0;
//	size = Config::getInstance().getI2CSize();
//	for(int i = 0; i < size; i++){
//		shared_ptr<I2CConf> p = Config::getInstance().getI2CConf(i);
//		shared_ptr<PCA9685> pca = shared_ptr<PCA9685>(new PCA9685());
//		pca->Setup(p->address, p->hertz);
//		pca->PWMReset();
//		tamageta::AppCtx::getInstance().add(pca);
//	}
//
//	size = Config::getInstance().getHcSr04ConfSize();
//	for(int i = 0; i < size; i++){
//		shared_ptr<HcSr04Conf> p = Config::getInstance().getHcSr04Conf(i);
//		shared_ptr<HcSr04> hcsr04 = shared_ptr<HcSr04>(new HcSr04(p->pinTrig, p->pinEcho));
//		tamageta::AppCtx::getInstance().add(hcsr04);
//	}
}

void Alexo::run(){
	int opts = 0;
	int n = 0;
	unsigned int ms, oldms = 0;   

	/* Deamonize */
	if(Config::getInstance().isDaemon()){
		cout << "daemonize... " << Config::getInstance().getAppDir() << " : " << Config::getInstance().getPidFile() << endl;
		daemonize(Config::getInstance().getAppDir().c_str(), Config::getInstance().getPidFile().c_str());
		cout << "daemonized!" << endl;
	} else {
		cout << "not daemonized!" << endl;
	}

//	init_log();

	signal(SIGINT, sighandler);
 
}

}


int main(int argc, char **argv)
{
	cout << "Start main..." << endl;

  if ( argc != 2) {
    cerr << "argument count mismatch error." << endl;
    exit(EXIT_FAILURE);
  }
	tamageta::Config::getInstance().load(argv[1]);

	cout << "AppDir:" << tamageta::Config::getInstance().getAppDir() << endl;
	cout << "LogDir:" << tamageta::Config::getInstance().getLogDir() << endl;
	cout << "PidFile:" << tamageta::Config::getInstance().getPidFile() << endl;

	tamageta::Alexo server;

	server.init();

	cout << "after init..." << endl;

	server.run();
 
//	BOOST_LOG_TRIVIAL(info) << "server.run() end...";

	return 0;
} //main

