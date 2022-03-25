#include "histgui.h"


//global variables
MyMainFrame *frame;


void Update() {

	frame->Update();

}

void histgui () {

	cout << "Hello World!" << endl;

	frame = new MyMainFrame(gClient->GetRoot(), 1, 1);
	

}	

