                                PNWl2kAir
                                *********


To connect a Winlink RMS station, with a SCS Pactor TNC.
This was made from the Paclink project of N2QZ and N7NIX.
Thanks QRO to N7NIX for providing me with his PacLink code.

It use GTK3 as only lib in the fourth line of the ui file you can cheat if your gtk version is lower. Tarball come from my eclipse directory,
so set in code, path and TX CAT to your need, at the begining of Autre.h set path for the PTC serial port. Then make. 

Normally PNWl2kAir is run from PNPropag. So station and frequency are already set. Just after launch, wait for the ready signal and the change of QRG on the Tx
 to ensure serial connection, before QSO connect.

I use Ubuntu 16, my TNC is a TRX PTC and is connected via Bluetooth.

Still many bug... and with the propagation we have, it is difficult to extensively test the software.

If you experience bug (even those you have solved) please report to FM4PN@laposte.net (i am not very reactive on internet
i live on a boat most of the time i have no connection).

                 73 FM4PN jean Pierre Lathuile
 

TODO :
 
   *For changing color of the connect button, i use 3 Css files there should be a smarter way for doing that.

   *The path for file should be defined for each one installation,  eclipse use different directory for source and binary.
     For debuging path is defined at the top of Autre.h.
     When version come definitive, install ui and css file in the binary directory and change path to ./
     There is a path to set for .ui, .css ,adresse of PTC serial port and mail in /out box.

   *I use a FT897, for different TX the CAT command should be changed in Call back timer / case 0 switch.
   
   *See what to do for Pactor4. May be i will buy one.

   *Internationnalisation, give all string a symbolic name, group them in a .h file and translate. Can be useful some people does not 
    understand French.

