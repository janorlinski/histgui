// example.C

#include<vector>
#include<fstream>
#include<string>


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

		TString label = dates[i] + ", " + times[i];

		hTemp->SetBinContent(i+1, temperatures[i]);
		if (TString(time[i]) == "1200") hTemp->GetXaxis()->SetBinLabel(i+1, label); 
	}

	return hTemp;

}


class MyMainFrame {
	RQ_OBJECT("MyMainFrame")
private:
	const TGWindow*		window;
	TGMainFrame*		fMain;
	TRootEmbeddedCanvas*	fEcanvas;
   	TF1*			func;
	TH1D*			hist;
	Bool_t			isZoomed = false;
	TString			path;


public:
	MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h);
	virtual ~MyMainFrame();
	void SetFunction();
	void SetHistogram();	
	void ToggleZoom();
	void DoDraw();
};
MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h) {
	window=p;
	// Create a main frame
	fMain = new TGMainFrame(p,w,h);

	// Create canvas widget
	fEcanvas = new TRootEmbeddedCanvas("Ecanvas",fMain,400,400);
	fMain->AddFrame(fEcanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 10,10,10,1));
	// Create a horizontal frame widget with buttons
	TGHorizontalFrame *hframe = new TGHorizontalFrame(fMain,800,640);

	TGTextButton *load = new TGTextButton(hframe,"Load .txt file");
	load->Connect("Clicked()","MyMainFrame",this,"SetHistogram()");
	hframe->AddFrame(load, new TGLayoutHints(kLHintsCenterX,5,5,3,4));

	TGTextButton *exit = new TGTextButton(hframe,"Exit","gApplication->Terminate(0)");
	hframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX,5,5,3,4));

	fMain->AddFrame(hframe, new TGLayoutHints(kLHintsCenterX,2,2,2,2));
	// Set a name to the main frame
	fMain->SetWindowName("HistoDrawer v0.2");

	// Map all subwindows of main frame
	fMain->MapSubwindows();

	// Initialize the layout algorithm
	fMain->Resize(fMain->GetDefaultSize());

	// Map main frame
	fMain->MapWindow();

	//cout << "Histogram has to be set manually using SetHistogram()!" << endl;

	// Set function and isZoomed
	//SetHistogram(hTemp);
	//isZoomed = false;
	


}
void MyMainFrame::SetFunction() {
	func = new TF1("func", "sin(x)/x", 0, 10);
}
void MyMainFrame::SetHistogram() {

	cout<<"blip"<<endl;
	TGFileInfo* fileinfo = new TGFileInfo();
	//fileinfo->fIniDir("./");
	TGFileDialog *file = new TGFileDialog(window, fMain, kFDOpen, fileinfo);
	//fMain->AddFrame(file);
	//file->CloseWindow();
	//TGTextEntry filename = fileinfo->fName;
	cout <<"blip1"<<endl;
	path = fileinfo->fFilename;

	if (!(path=="")){
		cout<<"blip2"<<endl;
		hist = ParseTxtFile(path);
		DoDraw();
	}
}
void MyMainFrame::ToggleZoom() {

	if (!isZoomed) {
		hist->GetXaxis()->SetRangeUser(0, 1);
		isZoomed = true;
	}

	else if (isZoomed) {
		hist->GetXaxis()->SetRangeUser(0, 1000);
		isZoomed = false;
	}

	DoDraw();
}
void MyMainFrame::DoDraw() {
	// Draws function graphics in randomly chosen interval
	//TF1 *f1 = new TF1("f1","sin(x)/x",0,gRandom->Rndm()*10);
	
	gStyle->SetOptStat(0);	
	hist->SetLineWidth(3);
	hist->Draw("L");
	TCanvas *hCanvas = fEcanvas->GetCanvas();
	hCanvas->cd();
	hCanvas->Update();
}
MyMainFrame::~MyMainFrame() {
	// Clean up used widgets: frames, buttons, layout hints
	fMain->Cleanup();
	delete fMain;
}


