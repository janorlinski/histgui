# histgui

This is a simple ROOT GUI app, that allows for immediate or pseudo-serial reading of a .txt file and presenting it as TH1D.
Currently in version 1.1, which already does _something_ but nothing particularily impressive. The objective is to eventually create a simple event generator which would serially write data for the GUI. This would allow for quick, GUI-based simulations of typical particle physics distributions. 

### how to use me?

ROOT [1] is required. ROOT6 is recommended. 

1. Download and unpack this repository. 

2. Run histgui.C with 

```
root -l histgui.C
```

3. To initialize data reading from a .txt file, click on "Select .txt file". This repository already includes the exemplary "transmission.txt" file.

4. This activates the "Transmission status" button. If you wish to refresh the .txt file in real time, click the Transmission Status button. You should see the red control button "DAQ STOPPED" change to "DAQ RUNNING" and the dialog box in the bottom change to "Streaming active!".

5. At this point, the GUI refreshes the .txt file in real time. However, this does not provide that the .txt file itself will change. To emulate a simple serial tramsission you can run transmitter.C which will reset the transmission.txt file and add a line of data to it every second.

6. This GUI also includes a "Smoothing" slider which allows to smooth out the data and a "Zoom to last 48h" radio button. 

7. Last interesting widget is the second control button for temperature. It is hard-coded for the temperature example to show whether current outdoor conditions are pleasant. It can be easily adjusted to be color-coded for another parameter of the "slow control", e.g. HV on a detector or working gas pressure. 

8. If you wish to exit, it's best to use the dedicated "Exit" button to avoid crashes. 

### formatting of .txt files

The GUI app only accepts .txt files with data of the following format:

[DATE] [TIMESTAMP] [VALUE]

without any heading rows. 

Moreover, it will assume that datapoints are separated by one hour (yes, this makes the DATE and TIMESTAMP columns redundant - sue me). This is matters only for describing the X axis of the histogram and can be easily tweaked. 

The transmitter.C macro can be easily adjusted to convert different .txt files to the format accepted by histgui.C. Transmitter.C is currently set to read .txt files with climate data attached in this repository, that come from National Centers for Environmental Information of USA [2].

The axis titles can be adjusted by modifying the `SetHistogram()` function.

### common issues / troubleshooting

The "Zoom to last 48h" button seems to not work, unless the DAQ is running. 

When transmission is paused, the TH1D can be easily manipulated by right-clicking on the TCanvas. Also, even if the .txt file itself does not refresh, one can still safely toggle the Tramission status to on. 

No other issues are currently known.

### current objectives

Minor bug fixes, code cleaning and optimisation.
Development of new features for this project is currently suspended.

### footnote

By Jan Orlinski for Computer Tools for Nuclear Physics at FUW [3] (2021/2022).
Readme last updated on 30th November 2023, 10:11.

[1] https://root.cern/

[2] https://www.ncei.noaa.gov/

[3] https://fuw.edu.pl/
