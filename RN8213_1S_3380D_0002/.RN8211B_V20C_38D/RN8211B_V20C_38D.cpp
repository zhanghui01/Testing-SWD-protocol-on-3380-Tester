#ifdef _MSC_VER
#include <winsock2.h>
#endif
#include <uti/stnplan.h>
#include "reslib.h"
#include <awilib/awilib.h>
#pragma warn -8057
#pragma warn -8067
#pragma warn -8065
#pragma warn -8004
#pragma warn -8070
void main(int argc,char *argv[]){
	CITEM::load_pin = LOADPIN;
	Main_Flow(97,TEST_START,TEST_END,START_UP,RELEASE,NULL,NULL,"RN8211B_V20C_38D.PLN","RN8213.dec",false);
}
