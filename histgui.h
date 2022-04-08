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

		//cout << temp << " deg. Celsius" << endl;
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
	//essentials
	const TGWindow*		window;
	TGMainFrame*		fMain;
	TRootEmbeddedCanvas*	fEcanvas;
	TH1D*			hist;
	TString			path;

	//control values
	Bool_t			isRunning=false;
	Bool_t			isZoomed=false;
	Double_t 		currentTemperature;
	Int_t			smoothing=0;
	
	//widgets
	TGTextButton*		transmission;
	TTimer*			timer;
	TGTextView*		status;
	TGLabel*		labelTransmission;
	TGLabel*		labelTemperature;
	TGHSlider*		sliderSmooth;
	TGRadioButton*		timeRadio;	

public:
	MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h);
	virtual ~MyMainFrame();
	void LoadHistogram();
	void SetHistogram();
	void Refresh();
	void DoDraw();
	void ToggleTransmission();
	void ToggleTimeRange();
	void UpdateLabel(TGLabel*, TString, TString);
	void CheckLabels();
	void Update();
	
};
MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h) {

	window=p;
	path = "";
	// Create a main frame
	fMain = new TGMainFrame(p,w,h);

   	// label + horizontal line (adapted from ROOT tutorials guilabels.C
    	TGGC *fTextGC;
    	const TGFont *font = gClient->GetFont("-*-times-bold-r-*-*-14-*-*-*-*-*-*-*");
    	if (!font) font = gClient->GetResourcePool()->GetDefaultFont();
   	FontStruct_t labelfont = font->GetFontStruct();
   	GCValues_t   gval;
   	gval.fMask = kGCBackground | kGCFont | kGCForeground;
    	gval.fFont = font->GetFontHandle();
    	gClient->GetColorByName("black", gval.fBackground);
    	fTextGC = gClient->GetGC(&gval, kTRUE);

	//get colors 
	ULong_t rcolor, gcolor;
	gClient->GetColorByName("red", rcolor);
	gClient->GetColorByName("grey", gcolor);

	// Create a timer
   	timer = new TTimer(200);
   	timer->SetCommand("Update()");

	// Create a horizontal frame widget with buttons
	TGHorizontalFrame *hframe = new TGHorizontalFrame(fMain,200,20);

	// Create a horizontal frame widget with labels
	TGHorizontalFrame *hframeLabels = new TGHorizontalFrame(fMain,200,20);

	// Create a horizontal frame widget with plotting settings
	TGHorizontalFrame *hframePlotting = new TGHorizontalFrame(fMain,200,20);

	// Create three buttons
	TGTextButton *load = new TGTextButton(hframe,"Select .txt file");
	load->Connect("Clicked()","MyMainFrame",this,"LoadHistogram()");
	hframe->AddFrame(load, new TGLayoutHints(kLHintsCenterX,5,5,3,4));

	transmission = new TGTextButton(hframe,"Transmission status");
	transmission->Connect("Clicked()","MyMainFrame",this,"ToggleTransmission()");
	hframe->AddFrame(transmission, new TGLayoutHints(kLHintsCenterX,5,5,3,4));

	TGTextButton *exit = new TGTextButton(hframe,"Exit","gApplication->Terminate(0)");
	hframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX,5,5,3,4));

	//Add horizontal frame with widgets
	fMain->AddFrame(hframe, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

	//Add labels
	labelTransmission = new TGLabel(hframeLabels, "Transmission status", fTextGC->GetGC(), labelfont, kChildFrame, rcolor);
	hframeLabels->AddFrame(labelTransmission, new TGLayoutHints(kLHintsNormal, 5, 5, 3, 4));

	labelTemperature = new TGLabel(hframeLabels, "Temperature status", fTextGC->GetGC(), labelfont, kChildFrame, rcolor);
	hframeLabels->AddFrame(labelTemperature, new TGLayoutHints(kLHintsNormal, 5, 5, 3, 4));

	//Add horizontal frame with labels
	fMain->AddFrame(hframeLabels, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

	//Create slider label
	TGLabel* labelSmoothing;
	labelSmoothing = new TGLabel(hframePlotting, " Smoothing: ", fTextGC->GetGC(), labelfont, kChildFrame, gcolor);
	hframePlotting->AddFrame(labelSmoothing, new TGLayoutHints(kLHintsNormal, 5, 5, 3, 4));

	//Create slider
	sliderSmooth = new TGHSlider (hframePlotting, 100);
	hframePlotting->AddFrame(sliderSmooth, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

	//Create radio buttons
	timeRadio = new TGRadioButton(hframePlotting, "Zoom to last 48h", "ToggleTimeRange()");
	timeRadio->Connect("Clicked()","MyMainFrame",this,"ToggleTimeRange()");
	hframePlotting->AddFrame(timeRadio, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

	//Add horizontal frame with plotting options
	fMain->AddFrame(hframePlotting, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

	// Create canvas widget
	fEcanvas = new TRootEmbeddedCanvas("Ecanvas",fMain,400,300);
	fMain->AddFrame(fEcanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 10,10,10,1));	

	// Create text viewer for dialog communication
	status = new TGTextView(fMain, 400, 25, "Select .txt file first...");
	status->SetForegroundColor(Pixel_t(kRed));
	fMain->AddFrame(status, new TGLayoutHints(kLHintsCenterX, 5,5,3,4));

	// Set a name to the main frame
	fMain->SetWindowName("HistoDrawer v1.1");

	// Map all subwindows of main frame
	fMain->MapSubwindows();

	// Initialize the layout algorithm
	fMain->Resize(fMain->GetDefaultSize());

	// Map main frame
	fMain->MapWindow();

	// turn on timer
	timer->TurnOn();

	//disabling buttons that have to wait for txt file
	transmission->SetState(kButtonDisabled);
	timeRadio->SetState(kButtonDisabled);
	

}

void MyMainFrame::LoadHistogram() {

	TGFileInfo* fileinfo = new TGFileInfo();
	TGFileDialog *file = new TGFileDialog(window, fMain, kFDOpen, fileinfo);

	path = fileinfo->fFilename;

	if (!(path=="")){
		hist = ParseTxtFile(path);
		SetHistogram();
		DoDraw();

		transmission->SetState(kButtonEngaged);
		transmission->SetState(kButtonUp);
		timeRadio->SetState(kButtonEngaged);	
	}
}

void MyMainFrame::SetHistogram() {
	
	if (!(path=="")) {

		TString xTitle = "t [h]";
		TString yTitle = "T [^{#circ} C]";
	
		TAxis* xaxis = hist->GetXaxis();
		TAxis* yaxis = hist->GetYaxis();

		Int_t Nbins = hist->GetNbinsX();

		xaxis->SetTitle(xTitle);
		yaxis->SetTitle(yTitle);

		xaxis->SetNdivisions(505);
		yaxis->SetNdivisions(510);

		hist->Smooth(smoothing);
		
		Int_t FirstBinToShow = Nbins - 48;
		if (FirstBinToShow<=0) FirstBinToShow = 1;
		if (isZoomed) xaxis->SetRangeUser(FirstBinToShow, Nbins);
	}
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

			//timeRadio->SetState(kButtonDisabled);
			//UpdateLabel(labelTransmission, "TRANSMISSION: STOPPED", "red");

			//ULong_t rcolor;
			//gClient->GetColorByName("red", rcolor);
			//labelTransmission->SetForegroundColor(rcolor);
		}
		else if (!isRunningCopy) {
			isRunning = true;
			status->Clear();
			status->SetText(new TGText("Streaming active!"));
			status->ShowTop();
			status->Update();

			//timeRadio->SetState(kButtonEngaged);
			//UpdateLabel(labelTransmission, "TRANSMISSION: OK", "green");
			//ULong_t gcolor;
			//gClient->GetColorByName("green", gcolor);
			//labelTransmission->SetForegroundColor(gcolor);
			//labelTransmission = new TGLabel(fMain, "Transmission status", fTextGC->GetGC(), labelfont, kChildFrame, gcolor);
		}

		cout << "value of isRunning is now " << isRunning <<endl;
		CheckLabels();
	}
}

void MyMainFrame::ToggleTimeRange() {

	Bool_t isZoomedCopy = isZoomed;

	if (!(path=="")) {

		if (isZoomedCopy) {
			isZoomed = false;
			//status->Clear();
			//status->SetText(new TGText("Standby..."));			
			//status->Update();
			//UpdateLabel(labelTransmission, "TRANSMISSION: STOPPED", "red");

			//ULong_t rcolor;
			//gClient->GetColorByName("red", rcolor);
			//labelTransmission->SetForegroundColor(rcolor);
			timeRadio->SetState(kButtonUp);
		}
		else if (!isZoomedCopy) {
			isZoomed = true;
			//status->Clear();
			//status->SetText(new TGText("Streaming active!"));
			//status->ShowTop();
			//status->Update();
			//UpdateLabel(labelTransmission, "TRANSMISSION: OK", "green");
			//ULong_t gcolor;
			//gClient->GetColorByName("green", gcolor);
			//labelTransmission->SetForegroundColor(gcolor);
			//labelTransmission = new TGLabel(fMain, "Transmission status", fTextGC->GetGC(), labelfont, kChildFrame, gcolor);
		
			timeRadio->SetState(kButtonDown);			
		}

		cout << "value of isZoomed is now " << isRunning <<endl;
		SetHistogram();
	}
}

void MyMainFrame::UpdateLabel(TGLabel* labelToBeUpdated, TString text, TString value) {

	ULong_t color;
	gClient->GetColorByName(value, color);
	labelToBeUpdated->ChangeBackground(color);
	labelToBeUpdated->SetText(text);
	cout << "updating label to "<< text << " with color " << value << endl;

}
void MyMainFrame::CheckLabels() {

	//in this function please list all the labels that should be updated after each tick
	//for each label specify the conditions and results of updating
	//according to the function UpdateLabel - in this case the results are text and background color

	//transmission label

	if (isRunning) UpdateLabel(labelTransmission,  "DAQ RUNNING", "green");
	else if (!(isRunning)) UpdateLabel(labelTransmission, "DAQ STOPPED", "red");

	//temperature label

	Double_t lowLimit = 5.0;
	Double_t optimallowLimit = 15.0;
	Double_t optimalhighLimit = 25.0;
	Double_t highLimit = 32.0;

	cout << currentTemperature << endl;

	if (currentTemperature < lowLimit || currentTemperature >= highLimit) {
		UpdateLabel(labelTemperature, Form("%.1f C", currentTemperature), "red");
	}

	else if (currentTemperature >= lowLimit && currentTemperature < optimallowLimit) {
		UpdateLabel(labelTemperature, Form("%.1f C", currentTemperature), "yellow");
	}

	else if (currentTemperature >= optimallowLimit && currentTemperature < optimalhighLimit) {
		UpdateLabel(labelTemperature, Form("%.1f C", currentTemperature), "green");
	}

	else if (currentTemperature >= optimalhighLimit && currentTemperature < highLimit) {
		UpdateLabel(labelTemperature, Form("%.1f C", currentTemperature), "yellow");
	}

	else UpdateLabel(labelTemperature, "ERROR", "red");

}
void MyMainFrame::Update() {


	if (isRunning){
		hist = ParseTxtFile(path);
		SetHistogram();
		DoDraw();		
		Refresh();	

		//setting the temperature
		currentTemperature = hist->GetBinContent(hist->FindLastBinAbove());
		
		//checking and updating all labels
		CheckLabels();
		smoothing = 0.5 * sliderSmooth->GetPosition();
		cout << smoothing << endl;

	}
	
	//SetHistogram();

}
MyMainFrame::~MyMainFrame() {
	// Clean up used widgets: frames, buttons, layout hints
	fMain->Cleanup();
	delete fMain;
}


