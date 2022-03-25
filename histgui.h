// example.C

#include<vector>
#include<fstream>
#include<string>

TH1D* ParseTxtFileLegacy(TString path) {

	
	cout << "Opening txt file at path '" << path << "'..." << endl;
	
	ifstream inputfile;
	inputfile.open(path);
	string value;
	string date;
	string time;
	string dummy;
	vector<double> temperatures;
	vector<string> dates;
	vector<string> times;

	while (inputfile >> dummy >> date >> time >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> value
			 >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy
			 >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy
			 >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy) {

		double temp = stod(value);

		if (temp < -50 || temp > 50) continue;

		cout << temp << " deg. Celsius" << endl;
		temperatures.push_back(temp);
		dates.push_back(date);
		times.push_back(time);


	}

	Int_t size = temperatures.size();

	TH1D *hTemp = new TH1D ("hTemp", "", size, 0.5, size);

	for (Int_t i=0; i<size; i++) {

		TString label = dates[i];

		hTemp->SetBinContent(i+1, temperatures[i]);
		//if ((i+1)%1000 == 0) hTemp->GetXaxis()->SetBinLabel(i+1, label); 
	}

	return hTemp;

}

TH1D* ParseTxtFile(TString path) {

	
	cout << "Opening txt file at path '" << path << "'..." << endl;
	
	ifstream inputfile;
	inputfile.open(path);
	string value;
	string date;
	string time;
	string dummy;
	vector<double> temperatures;
	vector<string> dates;
	vector<string> times;

	while (inputfile >> date >> time >> value) {

		double temp = stod(value);

		if (temp < -50 || temp > 50) continue;

		cout << temp << " deg. Celsius" << endl;
		temperatures.push_back(temp);
		dates.push_back(date);
		times.push_back(time);

	}

	Int_t size = temperatures.size();

	TH1D *hTemp = new TH1D ("hTemp", "", size, 0.5, size);

	for (Int_t i=0; i<size; i++) {

		TString label = dates[i];

		hTemp->SetBinContent(i+1, temperatures[i]);
		//hTemp->GetXaxis()->SetBinLabel(i+1, label); 
		//if ((i+1)%1000 == 0) hTemp->GetXaxis()->SetBinLabel(i+1, label); 
	}

	return hTemp;

}


class MyMainFrame {
	RQ_OBJECT("MyMainFrame")
private:
	const TGWindow*		window;
	TGMainFrame*		fMain;
	TRootEmbeddedCanvas*	fEcanvas;
	TH1D*			hist;
	TString			path;
	Bool_t			isRunning=false;
	TTimer*			timer;
	TGTextView*		status;


public:
	MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h);
	virtual ~MyMainFrame();
	void LoadHistogram();
	void SetHistogram();
	void Refresh();
	void DoDraw();
	void ToggleTransmission();
	void Update();
	
};
MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h) {

	window=p;
	path = "";
	// Create a main frame
	fMain = new TGMainFrame(p,w,h);

	// Create a timer
   	timer = new TTimer(100);
   	timer->SetCommand("Update()");

	// Create a horizontal frame widget with buttons
	TGHorizontalFrame *hframe = new TGHorizontalFrame(fMain,200,20);

	// Create three buttions
	TGTextButton *load = new TGTextButton(hframe,"Select .txt file");
	load->Connect("Clicked()","MyMainFrame",this,"LoadHistogram()");
	hframe->AddFrame(load, new TGLayoutHints(kLHintsCenterX,5,5,3,4));

	TGTextButton *transmission = new TGTextButton(hframe,"Transmission status");
	transmission->Connect("Clicked()","MyMainFrame",this,"ToggleTransmission()");
	hframe->AddFrame(transmission, new TGLayoutHints(kLHintsCenterX,5,5,3,4));

	TGTextButton *exit = new TGTextButton(hframe,"Exit","gApplication->Terminate(0)");
	hframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX,5,5,3,4));

	//Add horizontal frame with widgets
	fMain->AddFrame(hframe, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

	// Create canvas widget
	fEcanvas = new TRootEmbeddedCanvas("Ecanvas",fMain,400,300);
	fMain->AddFrame(fEcanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 10,10,10,1));	

	// Create text viewer for dialog communication
	status = new TGTextView(fMain, 400, 25, "Select .txt file first...");
	status->SetForegroundColor(Pixel_t(kRed));
	fMain->AddFrame(status, new TGLayoutHints(kLHintsCenterX, 5,5,3,4));

	// Set a name to the main frame
	fMain->SetWindowName("HistoDrawer v0.3");

	// Map all subwindows of main frame
	fMain->MapSubwindows();

	// Initialize the layout algorithm
	fMain->Resize(fMain->GetDefaultSize());

	// Map main frame
	fMain->MapWindow();

	// turn on timer
	timer->TurnOn();


}

void MyMainFrame::LoadHistogram() {

	TGFileInfo* fileinfo = new TGFileInfo();
	TGFileDialog *file = new TGFileDialog(window, fMain, kFDOpen, fileinfo);

	path = fileinfo->fFilename;

	if (!(path=="")){
		hist = ParseTxtFile(path);
		SetHistogram();
		DoDraw();
	}
}

void MyMainFrame::SetHistogram() {
	
	TString xTitle = "t [h]";
	TString yTitle = "T [^{#circ} C]";
	
	TAxis* xaxis = hist->GetXaxis();
	TAxis* yaxis = hist->GetYaxis();

	xaxis->SetTitle(xTitle);
	yaxis->SetTitle(yTitle);

	xaxis->SetNdivisions(505);
	yaxis->SetNdivisions(510);

}
void MyMainFrame::DoDraw() {

	gStyle->SetOptStat(0);	
	hist->SetLineWidth(3);
	hist->Draw("L");
	TCanvas *hCanvas = fEcanvas->GetCanvas();
	hCanvas->cd();
	hCanvas->Update();
}
void MyMainFrame::Refresh() {
	TCanvas *hCanvas = fEcanvas->GetCanvas();
	hCanvas->cd();
	hCanvas->Update();
}
void MyMainFrame::ToggleTransmission() {

	Bool_t isRunningCopy = isRunning;

	if (!(path=="")) {

		if (isRunningCopy) {
			isRunning = false;
			status->Clear();
			status->SetText(new TGText("Standby..."));			
			status->Update();
		}
		else if (!isRunningCopy) {
			isRunning = true;
			status->Clear();
			status->SetText(new TGText("Streaming active!"));
			status->ShowTop();
			status->Update();
		}

		cout << "value of isRunning is now " << isRunning <<endl;
	}
}
void MyMainFrame::Update() {

	if (isRunning){
		hist = ParseTxtFile(path);
		SetHistogram();
		DoDraw();		
		Refresh();	
	}

}
MyMainFrame::~MyMainFrame() {
	// Clean up used widgets: frames, buttons, layout hints
	fMain->Cleanup();
	delete fMain;
}


