# histgui

This is a simple ROOT GUI app, that allows for immediate or pseudo-serial reading of a .txt file and presenting it as TH1D.
Currently in version 1.0, i.e. first version that does anything remotely impressive. The objective is to eventually implement a simple event generator for particle physics which would allow for quick, GUI-based simulations of typical particle physics distributions. 

# how to use me?

Download and unpack this repository. Run histogui.C, select the .txt file as "transmission.txt" and set transmission status to "Streaming active!" (see dialog box in the bottom). At this point you should see some data from last stream. Then, open a parallel terminal tab and run "transmitter.C" to start streaming data. When data is streaming - that's pretty much it! Use the "Transmission status" button to play/pause data streaming if needed.

# formatting of .txt files

The GUI app only accepts .txt files with data of the following format:

[DATE] [TIMESTAMP] [VALUE]

without any heading rows. 

The transmitter.C macro can be easily adjusted to convert different .txt files to the format accepted by histgui.C. Transmitter.C is currently set to read .txt files with climate data attached in this repository, that come from National Centers for Environmental Information of USA [1].

[1] https://www.ncei.noaa.gov/

By Jan Orlinski for Computer Tools for Nuclear Physics at FUW (2021/2022).
Readme last updated on 25 march 11:43.
