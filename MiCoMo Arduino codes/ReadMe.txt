These two Arduino files are for the two Arduino Mega 2560s controlling 1) the media transfer pumps+gas sparging 2) the pH probes and pH 
adjustment pumps, respectively. Please note that these work only with Arduino IDE 1.8.9 and below. I believe newer version would give an error when compiling

For running both codes
1. Open Arduino IDE (open one instance of IDE for each piece of code)

2. Go to Tools -> Ports -> select the two Arduino Megas connected, one in each instance. Selecting which one in which IDE instance doesn't matter

3. Upload the two codes into two boards. Make sure the wiring to that particular board matches the code to be uploaded in.
i.e. upload the pH control code to whichever Arduino that is hooked to pH probes. 
*Here, you might need to figure out which Arduino is which port. You can easily do this by unplugging one Arduino and check what's left. The port
numbers never change for one computer.

4. To use the code, for each IDE instance, go to Tools ->Serial Monitor. Input 1001 in the textbox and hit enter, then wait a bit. 
You should start seeing outputs
For media transfer code, you will see outputs as time stamps
For pH code, you will see outputs as pH 

5. Leave the serial monitor open as is. You can now leave the system running by itself
*Closing the serial monitor won't shut the program down, HOWEVER reopening the serial monitor after closing it WILL.
In that case, re-type 1001 in the textbox and hit enter
