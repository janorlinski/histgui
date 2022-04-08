# histgui

This is a simple ROOT GUI app, that allows for immediate or pseudo-serial reading of a .txt file and presenting it as TH1D.
Currently in version 1.1, which already does _something_ but nothing particularily impressive. The objective is to eventually create a simple event generator which would serially write data for the GUI. This would allow for quick, GUI-based simulations of typical particle physics distributions. 

### how to use me?

ROOT is required. ROOT6 is recommended. 

1. Download and unpack this repository. 

2. Run histgui.C with 

```
root -l histgui.C
```

3. To initialize data reading from a .txt file, click on "Select .txt file". This repository already includes the exemplary "transmission.txt" file.

4. This activates the "Transmission status" button. If you wish to refresh the .txt file in real time, click the Transmission Status button. You should see the red control button "DAQ STOPPED" change to "DAQ RUNNING" and the dialog box in the bottom change to "Streaming active!".

5. At this point, the GUI refreshes the .txt file in real time. However, this does not provide that the .txt file itself will change. To emulate a simple serial tramsission you can run transmitter.C which will reset the transmission.txt file and add a line of data to it every second.

6. This GUI also includes a "Smoothing" slider which allows to smooth out the data and a "Zoom to last 48h" button (active only if DAQ has been started once!). 

7. Last interesting widget is the second control button for temperature. It is hard-coded for the current example to show whether the current temperature value is pleasant. It can be easily adjusted to be color-coded for another parameter of the "slow control", e.g. HV on a detector or working gas pressure. 

8. If you wish to exit, it's best to use the dedicated "Exit" button to avoid crashes. 

### formatting of .txt files

The GUI app only accepts .txt files with data of the following format:

[DATE] [TIMESTAMP] [VALUE]

without any heading rows. 

The transmitter.C macro can be easily adjusted to convert different .txt files to the format accepted by histgui.C. Transmitter.C is currently set to read .txt files with climate data attached in this repository, that come from National Centers for Environmental Information of USA [1].

### common issues troubleshooting

No common issues and their troubleshoots are known at the moment.

### current objectives

Further development of this project is currently suspended.

### footnote

By Jan Orlinski for Computer Tools for Nuclear Physics at FUW [2] (2021/2022).
Readme last updated on 7 april 9:59.


[1] https://www.ncei.noaa.gov/

[2] https://fuw.edu.pl/
