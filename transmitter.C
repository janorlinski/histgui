#include<vector>
#include<fstream>
#include<string>

using namespace std;

void transmitter() {

	TString path("CRNH0203-2021-GA_Newton_8_W.txt");

	cout << "Opening original txt file at path '" << path << "'..." << endl;
	
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

		//cout << temp << " deg. Celsius" << endl;
		temperatures.push_back(temp);
		dates.push_back(date);
		times.push_back(time);
	}

	cout << "File loaded! Beggining the transmission..." << endl;

	TCondition* cond = new TCondition();
	Int_t filesize = temperatures.size();
	//Int_t filesize = 100;

	for (Int_t h=0; h<filesize; h++) {

		ofstream outputfile;
		outputfile.open("transmission.txt");
		cout << "\n";
		
		for (Int_t i=0; i<h+1; i++) {

			cout <<"[line "<<i<<"]: "<< dates[i] << " " << times[i] << " " << temperatures[i] << "\n";
			outputfile << dates[i] << " " << times[i] << " " << temperatures[i] << "\n";

		}

		outputfile.close();

		cond->TimedWaitRelative(1000);
	}
}
		
