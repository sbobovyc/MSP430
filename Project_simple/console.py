
# Get the GUI stuff
import wx

# We're going to be handling files and directories
import os

from threading import Thread
import time
import serial
import struct
import binascii
# Set up some button numbers for the menu

ID_ABOUT=101
ID_OPEN=102
ID_SAVE=103
ID_BUTTON1=300
ID_EXIT=200

class RTCCTimeDate(object):
    def bcd2int(self, value):
        return hex(value)[2:]

    def __str__(self):
        string = ""
        days = ["", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday",
                        "Saturday", "Sunday"]
        self.hour = self.bcd2int(self.hour).zfill(2)
        self.minute = self.bcd2int(self.minute).zfill(2)
        self.second = self.bcd2int(self.second).zfill(2)
        self.month = self.bcd2int(self.month).zfill(2)
        self.date = self.bcd2int(self.date).zfill(2)
        self.year = self.bcd2int(self.year).zfill(2)

        string += "%s:%s:%s, " % (self.hour, self.minute, self.second)
        string += days[self.day]
        string += " %s/%s/%s\n" % (self.month, self.date, self.year)
        return string

def receiving(ser,TextCtrl):
    while True:
        while ser.inWaiting() < 7:
            pass
        data = ser.read(7)
        second, minute, hour, day, date, month, year = struct.unpack("7B", data)
        timeDate = type("RTCCTimeDate", (RTCCTimeDate,), dict(second=second,
                minute=minute, hour=hour, day=day, date=date, month=month, year=year))
        
        print binascii.hexlify(data)        
        TextCtrl.AppendText(timeDate().__str__())
            
class SerialData(object):
    def __init__(self, TextCtrl, init=50):
        try:
            self.ser = ser = serial.Serial(
                port='COM3',
                baudrate=9600,
                bytesize=serial.EIGHTBITS,
                parity=serial.PARITY_NONE,
                stopbits=serial.STOPBITS_ONE,
                timeout=0.1,
                xonxoff=0,
                rtscts=0,
                interCharTimeout=None
            )
        except serial.serialutil.SerialException:
            #no serial connection
            self.ser = None
            print "Failed to open port"
        else:
            Thread(target=receiving, args=(self.ser,TextCtrl)).start()
        
    def __del__(self):
        if self.ser:
            self.ser.close()

class MainWindow(wx.Frame):
    def __init__(self,parent,title):
        # based on a frame, so set up the frame
        wx.Frame.__init__(self,parent,wx.ID_ANY, title)

        # Add a text editor and a status bar
        # Each of these is within the current instance
        # so that we can refer to them later.
        self.control = wx.TextCtrl(self, 1, style=wx.TE_MULTILINE)
        font1 = wx.Font(20, wx.MODERN, wx.NORMAL, wx.NORMAL, False, u'Consolas')
        self.control.SetFont(font1)


        self.CreateStatusBar() # A Statusbar in the bottom of the window

        # Setting up the menu. filemenu is a local variable at this stage.
        filemenu= wx.Menu()
        # use ID_ for future easy reference - much better that "48", "404" etc
        # The & character indicates the short cut key
        filemenu.Append(ID_OPEN, "&Open"," Open a file to edit")
        filemenu.AppendSeparator()
        filemenu.Append(ID_SAVE, "&Save"," Save file")
        filemenu.AppendSeparator()
        filemenu.Append(ID_ABOUT, "&About"," Information about this program")
        filemenu.AppendSeparator()
        filemenu.Append(ID_EXIT,"E&xit"," Terminate the program")

        # Creating the menubar.
        menuBar = wx.MenuBar()
        menuBar.Append(filemenu,"&File") # Adding the "filemenu" to the MenuBar
        self.SetMenuBar(menuBar) # Adding the MenuBar to the Frame content.
        # Note - previous line stores the whole of the menu into the current object

        # Define the code to be run when a menu option is selected
        wx.EVT_MENU(self, ID_ABOUT, self.OnAbout)
        wx.EVT_MENU(self, ID_EXIT, self.OnExit)
        wx.EVT_MENU(self, ID_OPEN, self.OnOpen)
        wx.EVT_MENU(self, ID_SAVE, self.OnSave); # just "pass" in our demo

        # Show it !!!
        self.Show(1)

        # Define widgets early even if they're not going to be seen
        # so that they can come up FAST when someone clicks for them!
        self.aboutme = wx.MessageDialog( self, " A simple \n"
                            " console for an MSP430 Project","About MSP430 Console", wx.OK)
        self.doiexit = wx.MessageDialog( self, " Exit - Are you sure? \n",
                        "", wx.YES_NO)

        # dirname is an APPLICATION variable that we're choosing to store
        # in with the frame - it's the parent directory for any file we
        # choose to edit in this frame
        self.dirname = ''

    def OnAbout(self,e):
        # A modal show will lock out the other windows until it has
        # been dealt with. Very useful in some programming tasks to
        # ensure that things happen in an order that the programmer
        # expects, but can be very frustrating to the user if it is
        # used to excess!
        self.aboutme.ShowModal() # Shows it
        # widget / frame defined earlier so it can come up fast when needed

    def OnExit(self,e):
        # A modal with an "are you sure" check - we don't want to exit
        # unless the user confirms the selection in this case ;-)
        igot = self.doiexit.ShowModal() # Shows it
        if igot == wx.ID_YES:
            self.Close(True) # Closes out this simple application

    def OnOpen(self,e):
        # In this case, the dialog is created within the method because
        # the directory name, etc, may be changed during the running of the
        # application. In theory, you could create one earlier, store it in
        # your frame object and change it when it was called to reflect
        # current parameters / values
        dlg = wx.FileDialog(self, "Choose a file", self.dirname, "", "*.*", wx.OPEN)
        if dlg.ShowModal() == wx.ID_OK:
            self.filename=dlg.GetFilename()
            self.dirname=dlg.GetDirectory()

            # Open the file, read the contents and set them into
            # the text edit window
            filehandle=open(os.path.join(self.dirname, self.filename),'r')
            self.control.SetValue(filehandle.read())
            filehandle.close()

            # Report on name of latest file read
            self.SetTitle("Editing ... "+self.filename)
            # Later - could be enhanced to include a "changed" flag whenever
            # the text is actually changed, could also be altered on "save" ...
        dlg.Destroy()

    def OnSave(self,e):
        # Save away the edited text
        # Open the file, do an RU sure check for an overwrite!
        dlg = wx.FileDialog(self, "Choose a file", self.dirname, "", "*.*", \
                wx.SAVE | wx.OVERWRITE_PROMPT)
        if dlg.ShowModal() == wx.ID_OK:
            # Grab the content to be saved
            itcontains = self.control.GetValue()

            # Open the file for write, write, close
            self.filename=dlg.GetFilename()
            self.dirname=dlg.GetDirectory()
            filehandle=open(os.path.join(self.dirname, self.filename),'w')
            filehandle.write(itcontains)
            filehandle.close()
        # Get rid of the dialog to keep things tidy
        dlg.Destroy()
        
if __name__=='__main__':
    # Set up a window based app, and create a main window in it
    app = wx.PySimpleApp()
    view = MainWindow(None, "MSP430 Console")
    data = SerialData(view.control)
    # Enter event loop
    app.MainLoop()
    data.__del__()
