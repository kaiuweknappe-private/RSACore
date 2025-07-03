
#include <systemc.h>
#include "MontgomeryMul.h"

/*
SC_MODULE(Test) {
	SC_CTOR(Test) {
		SC_THREAD(hi);
	}

	void hi() {
		cout << "hi works" << sc_time_stamp() << endl;
	}
};
*/

int sc_main(int argc, char* argv[]){
	MontgomeryMul test("hello");
	sc_start(1, SC_NS);
	return 0;
}
