'''

'''

import sys, usb
import Tkinter as tk
import math
import time

class DeviceDescriptor(object):
	
	def __init__(self, vendor_id, product_id, interface_id):
		self.vendor_id = vendor_id
		self.product_id = product_id
		self.interface_id = interface_id
		
	def get_device(self):
		busses = usb.busses()
		for bus in busses:
			for device in bus.devices:
				if device.idVendor == self.vendor_id:
					if device.idProduct == self.product_id:
						return device
		return None

class PlugUSBDevice(object):
	VENDOR_ID = 0x04D8	#Microchip Vendor ID
	PRODUCT_ID = 0x000C	#FS USB Product ID
	INTERFACE_ID = 0
	BULK_IN_EP = 0x81	#Host in address
	BULK_OUT_EP = 0x01	#Host out address
	
	def __init__(self):
		self.device_descriptor = DeviceDescriptor(PlugUSBDevice.VENDOR_ID, 
											PlugUSBDevice.PRODUCT_ID, 
											PlugUSBDevice.INTERFACE_ID)
		self.device = self.device_descriptor.get_device()
		self.handle = None
		
	def open(self):
		self.device = self.device_descriptor.get_device()
		self.handle = self.device.open()
		#if sys.platform == 'darwin': 		#<-- Windows not auto setting config either
		self.handle.setConfiguration(1)
		self.handle.claimInterface(self.device_descriptor.interface_id)
		
	def close(self):
		self.handle.releaseInterface()

	def acquireDataLog(self):
		#send 0x36 command <-- refer to user.c Microchip firmware for commands 
		self.handle.bulkWrite(PlugUSBDevice.BULK_OUT_EP, [0x36], 1000)
		#receive block of data, max rx byte buffer is 64, 1000 ms timeout
		self.readLoggedTempData = self.handle.bulkRead(PlugUSBDevice.BULK_IN_EP, 64, 1000)
		return self.readLoggedTempData
		
	def enumTry(self):
		busses = usb.busses()
		for bus in busses:
			devices = bus.devices
			for dev in devices:
				print "Devices: ", dev.filename
				print " Device Class: ", dev.deviceClass
				print " Device Subclass: ", dev.deviceSubClass
				print " Device Protocol: ", dev.deviceProtocol
				print " Device max packet size: ", dev.maxPacketSize
				print " ID Vendor: ", dev.idVendor
				print " ID Product: ", dev.idProduct
				print " Device Version: ", dev.deviceVersion
				for config in dev.configurations:
					print "  Configuration: ", config.value
					print "   Total Length: ", config.totalLength
					print "   Self powered: ", config.selfPowered
					print "   Remote Wakeup: ", config.remoteWakeup
					print "   Max Power: ", config.maxPower
					for intf in config.interfaces:
						print "    Interface: ", intf[0].interfaceNumber
						for alt in intf:
							print "    Alternate Setting:",alt.alternateSetting
							print "      Interface class:",alt.interfaceClass
							print "      Interface sub class:",alt.interfaceSubClass
							print "      Interface protocol:",alt.interfaceProtocol
							for ep in alt.endpoints:
								print "      Endpoint:",hex(ep.address)
								print "        Type:",ep.type
								print "        Max packet size:",ep.maxPacketSize
								print "        Interval:",ep.interval

	def moreUSBInfo(self):
		for bus in usb.busses():
			for dev in bus.devices:
				try:
					#print "Bus %s Device %s: ID %04x:%04x %s" % (bus.dirname,dev.filename,dev.idVendor,dev.idProduct,dev.open().getString(1,30))
					return True
				except:
					return False
		
class UsbGui(tk.Tk):
	pud = PlugUSBDevice()
	USBCONNECTION = False
	lastFlag = False
	ta = 0
	tb = 0
	tc = 0
	D4Toggle = False
	D3Toggle = False
	
	def __init__(self):
		tk.Tk.__init__(self)
		self.geometry("625x450")
		self.title("PyUSB and PICDEM(TM) FS USB Demo")
		
		self.temperature = tk.StringVar()
		self.potResistance= tk.StringVar()
		self.boardSelection = tk.StringVar()
		self.dataModeRB = tk.IntVar()
		self.boardSelectionList = [""]
		
		selectionLabel = tk.Label(self, text="Select PICDEM FS USB Board", relief = "flat")
		self.boardSelection.set(self.boardSelectionList[0])
		selectionOptionBox = apply(tk.OptionMenu, (self, self.boardSelection) + tuple(self.boardSelectionList))
		selectionOptionBox.configure(width=25, relief='sunken', bg='white')
		self.connectButton = tk.Button(self, text="Connect", command=self.toggleUSBConnection, width=20)
		
		tempDisplayFrame = tk.LabelFrame(self, text="Temperature Display")
		toggleLedFrame = tk.LabelFrame(self, text="Toggle LEDs", width=10)
		potDisplayFrame = tk.LabelFrame(self, text="Potentiometer Display")
		
		dataModeFrame = tk.LabelFrame(tempDisplayFrame, text="Data Mode", height=150)
		self.tempGraphCanvas = tk.Canvas(tempDisplayFrame, bg='black', width=250, height=175)
		
		self.realTimeRadioButton = tk.Radiobutton(dataModeFrame, text="Real Time", variable=self.dataModeRB, value=0)
		tempDescLabel = tk.Label(dataModeFrame, text="Temperature: ")
		tempDataLabel = tk.Label(dataModeFrame, textvar=self.temperature, bg='black', fg='green', width=10, relief="sunken")
		self.dataLoggingRadioButton = tk.Radiobutton(dataModeFrame, text="Data Logging", variable=self.dataModeRB, value=1)
		self.acquireDataButton = tk.Button(dataModeFrame, text="Acquire Data", state=tk.DISABLED, command=self.getDataFromDev, width=20) #self.pud.acquireDataLog
		
		self.ledD3 = tk.Canvas(toggleLedFrame, bg='forestgreen', width = 20, height=10)
		self.ledD4 = tk.Canvas(toggleLedFrame, bg='forestgreen', width = 20, height=10)
		self.ledD3DescLabel = tk.Label(toggleLedFrame, text="LED D3", state='disabled', relief='raised')
		self.ledD4DescLabel = tk.Label(toggleLedFrame, text="LED D4", state='disabled', relief='raised')
		
		resistanceDescLabel = tk.Label(potDisplayFrame, text="Resistance", relief='flat')
		self.resistanceDataLabel = tk.Label(potDisplayFrame, textvar=self.potResistance, width=10, bg='black', fg='lightgreen', relief = "sunken")
		ohmDescLabel = tk.Label(potDisplayFrame, text="Ohms", relief='flat')
		self.resistanceGageCanvas = tk.Canvas(potDisplayFrame, bg='black', width=150, height=100)
		
		clearScreenButton = tk.Button(self, text="Clear Screen", command=self.clearLogTextArea, width=20)
		self.tempDataLoggingTextArea = tk.Text(self, width=75, height=10)
		
		self.statusBarVersionDescLabel = tk.Label(self,text="Ready", relief='flat')
		
		selectionLabel.grid(row=0, column=0)
		selectionOptionBox.grid(row=0, column=1, sticky=tk.W)
		self.connectButton.grid(row=0, column=2)
		
		tempDisplayFrame.grid(row=1, column=0,rowspan=2, columnspan=2, sticky=tk.N)
		
		dataModeFrame.grid(row=0, column=0, sticky=tk.NW)
		self.realTimeRadioButton.grid(row=0, column=0, sticky=tk.W)
		tempDescLabel.grid(row=1, column=0, sticky=tk.W)
		tempDataLabel.grid(row=1, column=1, sticky=tk.W)
		self.dataLoggingRadioButton.grid(row=2, column=0, sticky=tk.W)
		self.acquireDataButton.grid(row=3, column=0, columnspan=2)
		
		self.tempGraphCanvas.grid(row=0, column=1)
		
		toggleLedFrame.grid(row=1, column=2, ipadx=20, ipady=5, sticky=tk.NW)
		self.ledD3.grid(row=0, column=0)
		self.ledD3DescLabel.grid(row=0, column=1)
		self.ledD4.grid(row=0, column=2)
		self.ledD4DescLabel.grid(row=0, column=3)
		
		potDisplayFrame.grid(row=2, column=2)
		resistanceDescLabel.grid(row=0, column=0)
		self.resistanceDataLabel.grid(row=0, column=1)
		ohmDescLabel.grid(row=0, column=2)
		self.resistanceGageCanvas.grid(row=1, column=0, columnspan=3)
		
		clearScreenButton.grid(row=3, column=0, sticky=tk.W)
		self.tempDataLoggingTextArea.grid(row=4, column=0, columnspan=3, sticky=tk.W)
		
		self.statusBarVersionDescLabel.grid(row=5,column=0, sticky=tk.W)
		statusBarCopyrightDescLabel.grid(row=5, column=1, columnspan=2, sticky=tk.E)
		
		self.drawGages()
		self.updateRGage(5)
		
		self.connected = self.pud.moreUSBInfo()
		
		if self.connected:
			self.boardSelection.set("PICDEM FS USB 0 (Demo)")
			self.connectButton.config(state="normal")
		else:
			self.connectButton.config(state="disabled")
			self.runningFunction()
			
		self.realTimeRadioButton.config(state="disabled")
		self.dataLoggingRadioButton.config(state="disabled")
		self.acquireDataButton.config(state="disabled")

	def clearLogTextArea(self):
		self.tempDataLoggingTextArea.delete(1.0, tk.END)	
		
	def LED3Clicked(self, event):
		if self.D3Toggle == False:
			self.ledD3.config(bg = 'green')
			self.D3Toggle = True
			if self.D4Toggle == True:
				self.pud.handle.bulkWrite(self.pud.BULK_OUT_EP, [0x31, 0x03], 1000) #both on
			else:
				self.pud.handle.bulkWrite(self.pud.BULK_OUT_EP, [0x31, 0x02], 1000) # 3 on, 4 off
		else:
			self.ledD3.config(bg = 'forestgreen')
			self.D3Toggle = False
			if self.D4Toggle == False:
				self.pud.handle.bulkWrite(self.pud.BULK_OUT_EP, [0x31, 0x00], 1000)	# both off
			else:
				self.pud.handle.bulkWrite(self.pud.BULK_OUT_EP, [0x31, 0x01], 1000)	# 4 on, 3 off
		
	def LED4Clicked(self, event):
		if self.D4Toggle == False:
			self.ledD4.config(bg = 'green')
			self.D4Toggle = True
			if self.D3Toggle == True:
				self.pud.handle.bulkWrite(self.pud.BULK_OUT_EP, [0x31, 0x03], 1000)	# both on
			else:
				self.pud.handle.bulkWrite(self.pud.BULK_OUT_EP, [0x31, 0x01], 1000) 	# 4 on, 3 off
		else:
			self.ledD4.config(bg = 'forestgreen')
			self.D4Toggle = False
			if self.D3Toggle == False:
				self.pud.handle.bulkWrite(self.pud.BULK_OUT_EP, [0x31, 0x00], 1000) 	#both off
			else:
				self.pud.handle.bulkWrite(self.pud.BULK_OUT_EP, [0x31, 0x02], 1000) 	#3 on, 4 off
		
	def toggleUSBConnection(self):
		if self.USBCONNECTION == False:
			self.pud.open()
			self.connectButton.config(text = "Disconnect")
			self.USBCONNECTION = True
			self.realTimeRadioButton.config(state="normal")
			self.dataLoggingRadioButton.config(state="normal")
			self.acquireDataButton.config(state="normal")
			self.ledD4DescLabel.config(state='normal')
			self.ledD3DescLabel.config(state='normal')
			self.ledD3DescLabel.bind("<Button-1>", self.LED3Clicked)
			self.ledD4DescLabel.bind("<Button-1>", self.LED4Clicked)
			self.statusBarVersionDescLabel.config(text = "USB Demo Firmware Version 1.0")
			self.runningFunction()
		elif self.USBCONNECTION == True:
			self.pud.close()
			self.connectButton.config(text = "Connect")
			self.realTimeRadioButton.config(state="disabled")
			self.dataLoggingRadioButton.config(state="disabled")
			self.acquireDataButton.config(state="disabled")
			self.ledD3DescLabel.config(state='disabled')
			self.ledD4DescLabel.config(state='disabled')
			self.statusBarVersionDescLabel.config(text = "Ready")
			self.ledD3DescLabel.unbind("<Button-1>")
			self.ledD4DescLabel.unbind("<Button-1>")
			self.USBCONNECTION = False
			
	def drawGages(self):
		self.xScale = [10, 24, 40, 55, 70, 85, 100, 115, 130, 145, 160, 175, 190, 205]
		self.tData = [28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28]
		xT = self.tempGraphCanvas.winfo_reqwidth()
		yT = self.tempGraphCanvas.winfo_reqheight()
		xG = self.resistanceGageCanvas.winfo_reqwidth()
		yG = self.resistanceGageCanvas.winfo_reqheight()
		
		center_xG = xG/2
		center_yG = (yG/2)+20
		radius = 48
		
		self.resistanceGageCanvas.create_oval(center_xG-5, center_yG-5,center_xG+5,center_yG+5, fill='red')
		self.resistanceGageCanvas.create_oval(center_xG-2, center_yG-2,center_xG+2,center_yG+2, fill='forestgreen')
		#total degrees is 220, 24 degrees per 1 kohm, 12 degrees per .5 kohms
		self.resistanceGageCanvas.create_arc(center_xG-50, center_yG-50, center_xG+50, center_yG+50, start=-20, extent=220, outline='forestgreen', style='arc', width=4)
		
		rot = 0
		for rot in range(0, 11):
			z = rot
			cAngle = (-22*z)+200
			x = math.cos(cAngle*math.pi/180)*radius
			y = math.sin(cAngle*math.pi/180)*radius
			xA = math.cos(cAngle*math.pi/180)*3
			yA = math.sin(cAngle*math.pi/180)*3
			xB = math.cos(cAngle*math.pi/180)*(radius+10)
			yB = math.sin(cAngle*math.pi/180)*(radius+10)
			self.resistanceGageCanvas.create_line(center_xG+x, center_yG-y, center_xG+x+xA, center_yG-y-yA, fill='white')
			self.resistanceGageCanvas.create_text(center_xG+xB, center_yG-yB, fill='green', text=str(rot))
			z = rot + .5
			cAngle = (-22*z)+200
			x = math.cos(cAngle*math.pi/180)*radius
			y = math.sin(cAngle*math.pi/180)*radius
			xA = math.cos(cAngle*math.pi/180)*3
			yA = math.sin(cAngle*math.pi/180)*3
			
			if z < 10:
				self.resistanceGageCanvas.create_line(center_xG+x, center_yG-y, center_xG+x+xA, center_yG-y-yA, fill='white')
			rot = rot + 1
			
		self.resistanceGageCanvas.create_text(center_xG, center_yG-5, fill='forestgreen', text="10 K       POT")
		self.tempLine = self.resistanceGageCanvas.create_line(center_xG, center_yG, center_xG+x, center_yG-y, fill='red', width=2)
		
		wT = 10
		hT = 15
		tempText = 36
		
		for x in range (0, 14):
			self.tempGraphCanvas.create_line(wT, 15, wT , yT-15, fill='forestgreen', width=1)
			x=x+1
			wT=wT+15 #10 to 195 in canvas coords
			
		for y in range(0, 11):
			line = self.tempGraphCanvas.create_line(10, hT, xT-45, hT, fill='forestgreen', width=1)
			self.tempGraphCanvas.create_text(xT-30, hT, fill='forestgreen', text=str(tempText)+" C")
			if y==5:
				self.tempGraphCanvas.itemconfig(line, fill='green', width=1)
			y=y+1
			hT=hT+15 #15 to 150 in canvas coords
			tempText = tempText-2 
		
		i = 0
		j = 0
		adjusted = self.tData
		
		for i in range(0, 14):
			d=self.tData[i]
			adjusted[i] = (-7.5*d)+285
			i=i+1
			
		#for j in range(0,13):
		#	self.xl = self.tempGraphCanvas.create_line(self.xScale[j], adjusted[j], self.xScale[j+1], adjusted[j+1], fill='red')
		#	j=j+1	
		
		self.zero = self.tempGraphCanvas.create_line(self.xScale[0], self.tData[0], self.xScale[1], self.tData[1], fill='red')
		self.one = self.tempGraphCanvas.create_line(self.xScale[1], self.tData[1], self.xScale[2], self.tData[2], fill='red')
		self.two = self.tempGraphCanvas.create_line(self.xScale[2], self.tData[2], self.xScale[3], self.tData[3], fill='red')
		self.three = self.tempGraphCanvas.create_line(self.xScale[3], self.tData[3], self.xScale[4], self.tData[4], fill='red')
		self.four = self.tempGraphCanvas.create_line(self.xScale[4], self.tData[4], self.xScale[5], self.tData[5], fill='red')
		self.five = self.tempGraphCanvas.create_line(self.xScale[5], self.tData[5], self.xScale[6], self.tData[6], fill='red')
		self.six = self.tempGraphCanvas.create_line(self.xScale[6], self.tData[6], self.xScale[7], self.tData[7], fill='red')
		self.seven = self.tempGraphCanvas.create_line(self.xScale[7], self.tData[7], self.xScale[8], self.tData[8], fill='red')
		self.eight = self.tempGraphCanvas.create_line(self.xScale[8], self.tData[8], self.xScale[9], self.tData[9], fill='red')
		self.nine = self.tempGraphCanvas.create_line(self.xScale[9], self.tData[9], self.xScale[10], self.tData[10], fill='red')
		self.ten = self.tempGraphCanvas.create_line(self.xScale[10], self.tData[10], self.xScale[11], self.tData[11], fill='red')
		self.eleven = self.tempGraphCanvas.create_line(self.xScale[11], self.tData[11], self.xScale[12], self.tData[12], fill='red')
		self.twelve = self.tempGraphCanvas.create_line(self.xScale[12], self.tData[12], self.xScale[13], self.tData[13], fill='red')
			
	def updateRGage(self, ohms):
		xT = self.tempGraphCanvas.winfo_reqwidth()
		yT = self.tempGraphCanvas.winfo_reqheight()
		xG = self.resistanceGageCanvas.winfo_reqwidth()
		yG = self.resistanceGageCanvas.winfo_reqheight()
		
		center_xG = xG/2
		center_yG = (yG/2)+20
		radius = 48
		cAngle = (-22*ohms)+200
		x = math.cos(cAngle*math.pi/180)*radius
		y = math.sin(cAngle*math.pi/180)*radius
		
		self.resistanceGageCanvas.coords(self.tempLine, center_xG, center_yG, center_xG+x, center_yG-y)	
		
	def updateTGraph(self, aTemperature):
		data = aTemperature
		e = 0
		
		for e in range(0, 13):
			self.tData[e] = self.tData[e+1]	
			e=e+1
		self.tData[13] = (data*-7.5)+285

		self.tempGraphCanvas.coords(self.zero, self.xScale[0], (-7.5*self.tData[0])+285, self.xScale[1], (-7.5*self.tData[1])+285)
		self.tempGraphCanvas.coords(self.one, self.xScale[1], self.tData[1], self.xScale[2], self.tData[2])
		self.tempGraphCanvas.coords(self.two, self.xScale[2], self.tData[2], self.xScale[3], self.tData[3])
		self.tempGraphCanvas.coords(self.three, self.xScale[3], self.tData[3], self.xScale[4], self.tData[4])
		self.tempGraphCanvas.coords(self.four, self.xScale[4], self.tData[4], self.xScale[5], self.tData[5])
		self.tempGraphCanvas.coords(self.five, self.xScale[5], self.tData[5], self.xScale[6], self.tData[6])
		self.tempGraphCanvas.coords(self.six, self.xScale[6], self.tData[6], self.xScale[7], self.tData[7])
		self.tempGraphCanvas.coords(self.seven, self.xScale[7], self.tData[7], self.xScale[8], self.tData[8])
		self.tempGraphCanvas.coords(self.eight, self.xScale[8], self.tData[8], self.xScale[9], self.tData[9])
		self.tempGraphCanvas.coords(self.nine, self.xScale[9], self.tData[9], self.xScale[10], self.tData[10])
		self.tempGraphCanvas.coords(self.ten, self.xScale[10], self.tData[10], self.xScale[11], self.tData[11])
		self.tempGraphCanvas.coords(self.eleven, self.xScale[11], self.tData[11], self.xScale[12], self.tData[12])
		self.tempGraphCanvas.coords(self.twelve, self.xScale[12], self.tData[12], self.xScale[13], self.tData[13])
		
	def getDataFromDev(self):
		if self.dataModeRB.get() == 1:
			data = self.pud.acquireDataLog()
			data.pop(0)
			data.pop(0)
			i = 1

			if len(data) >= 2:
				for i in range(0, len(data)):
					if i%2 == 0:
						self.tb = data[i]*.0078125
					else:
						self.ta = data[i]*2
						self.tc = self.ta+self.tb
					self.tempDataLoggingTextArea.insert(1.0,"MESSAGE  - Temperature Point " + str(i) + " : " + u"+{0:.1f}\xb0C\r\n".format(self.tc)) 
					i = i+1  
			else:
				self.tempDataLoggingTextArea.insert(1.0, "WARNING - No data acquired.\r\n")
				
		
	def runningFunction(self):
		if self.USBCONNECTION == True:
			if self.dataModeRB.get() == 0:
				if self.lastFlag == False:
					self.pud.handle.bulkWrite(self.pud.BULK_OUT_EP, [0x33], 1000)
					self.acquireDataButton.config(state='disabled')
					self.lastFlag = True
				sent_pot_bytes = self.pud.handle.bulkWrite(self.pud.BULK_OUT_EP, [0x37], 1000)

				if sent_pot_bytes:
					read_pot_bytes = self.pud.handle.bulkRead(self.pud.BULK_IN_EP, 64, 1000)
					if len(read_pot_bytes) >=3:
						v = read_pot_bytes[2]*2500
						c = read_pot_bytes[1]*9.804
						pot = (v+c)/1000
						p = str(int(v+c))
						self.potResistance.set(p)
						self.updateRGage(pot)
					
				sent_bytes = self.pud.handle.bulkWrite(self.pud.BULK_OUT_EP, [0x34], 1000)

				if sent_bytes:
					read_bytes = self.pud.handle.bulkRead(self.pud.BULK_IN_EP, 64, 1000)
					ta = read_bytes[2]*2
					tb = read_bytes[1]*.0078125
					tc = ta+tb
					t = u"+{0:.1f} \xb0C".format(tc)
					self.temperature.set(t)
					self.updateTGraph(tc)				
			elif self.dataModeRB.get() == 1:
				if self.lastFlag == True:
					self.acquireDataButton.config(state='normal')
					self.pud.handle.bulkWrite(self.pud.BULK_OUT_EP, [0x35], 1000)
					self.lastFlag = False
		else:
			self.connected = self.pud.moreUSBInfo()
		
			if self.connected:
				self.boardSelection.set("PICDEM FS USB 0 (Demo)")
				self.connectButton.config(state="normal")
			else:
				self.connectButton.config(state="disabled")

		self.after(200, self.runningFunction)

usbGuiApp = UsbGui()
usbGuiApp.mainloop()
