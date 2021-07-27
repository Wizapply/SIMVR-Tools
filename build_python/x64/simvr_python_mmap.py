# -*- coding: cp932 -*-
# Import
from ctypes import *
import struct
import time
import mmap, io

# Status Define
SimvrCanNotFindUsb = 0
SimvrCanNotFindSimvr = 1
SimvrCanNotCalibration = 2
SimvrTimeoutCalibration = 3
SimvrShutDownActuator = 4
SimvrCanNotCertificate = 5
SimvrInitial = 6
SimvrRunning = 7
SimvrStopActuator = 8
SimvrCalibrationRetry = 9

# WRC Data Packet
class wrcPacket(Structure):  
    _fields_ = [  
            ("sequence_number", c_int32),
            ("version", c_int32),
            ("gear", c_int32),
            ("velocity_x", c_float),
            ("velocity_y", c_float),
            ("velocity_z", c_float),
            ("acceleration_x", c_float),
            ("acceleration_y", c_float),
            ("acceleration_z", c_float),
            ("engine_idle_rpm", c_int32),
            ("engine_max_rpm", c_int32),
            ("engine_rpm", c_int32),
            ("suspension_travel_LF", c_float),
            ("suspension_travel_LR", c_float),
            ("suspension_travel_RR", c_float),
            ("suspension_travel_RF", c_float),
            ("suspension_position_LF", c_float),
            ("suspension_position_LR", c_float),
            ("suspension_position_RR", c_float),
            ("suspension_position_RF", c_float),
            ("unknown", c_float*4)
            ]

#Shared Memory
mm = mmap.mmap(0, sizeof(wrcPacket), access=mmap.ACCESS_DEFAULT, tagname="Local\\SAMPLE")
wrcpkt = wrcPacket()

# Simvr Data Packet
class simvrPacket(Structure):  
    _fields_ = [  
            ("axis1", c_float),
            ("axis2", c_float),
            ("axis3", c_float),
            ("axis4", c_float),
            ("axis5", c_float),
            ("axis6", c_float),
            ("speedAxis123", c_float),  
            ("accelAxis123", c_float),
            ("speedAxis4", c_float),  
            ("accelAxis4", c_float),
            ("roll", c_float),  
            ("pitch", c_float),  
            ("yaw", c_float),
            ("heave", c_float),  
            ("sway", c_float),  
            ("surge", c_float),
            ("rotationMotionRatio", c_float),  
            ("gravityMotionRatio", c_float),  
            ("commandCount", c_int32),  
            ("commandStride", c_int32),
            ("command", c_char*256)
            ]

    def __init__(self):  
        self.axis1 = 0.5  #default param
        self.axis2 = 0.5
        self.axis3 = 0.5
        self.axis4 = 0.5
        self.axis5 = 0.5
        self.axis6 = 0.5
        self.speedAxis123 = 1.0
        self.accelAxis123 = 0.7
        self.speedAxis4 = 1.0
        self.accelAxis4 = 0.5
        self.roll = 0.0
        self.pitch = 0.0
        self.yaw = 0.0
        self.heave = 0.0
        self.sway = 0.0
        self.surge = 0.0
        self.rotationMotionRatio = 1.0
        self.gravityMotionRatio = 1.0
        self.commandCount = 0

# Load dll
simvrlib = cdll.LoadLibrary("./simvr.dll");		#Windows
#simvrlib = cdll.LoadLibrary("./libsimvr.dylib");	#Mac
#simvrlib = cdll.LoadLibrary("./libsimvr.so"); 		#Linux
simvrIsOpen = False

# Methods
def simvrAwake(appCode) :
    global simvrIsOpen
    global simvrlib
    if(simvrIsOpen == True) :
        return
    if(simvrlib.simvrOpen(appCode.encode()) == 0):
        simvrIsOpen = True
    else:
        print("SIMVR DLL ERROR!")

def simvrDestroy() :
    global simvrIsOpen
    global simvrlib
    
    if(simvrIsOpen == False) :
        return

    simvrlib.simvrClose()
    simvrUpdateBackLog()
    simvrIsOpen = False

def simvrUpdateBackLog() :
    global simvrIsOpen
    global simvrlib
    
    if(simvrIsOpen == False) :
        return
    
    size = simvrlib.simvrGetBackLogSize();
    if(size > 0) :
        simvrlib.simvrGetBackLog.restype = c_char_p    #for unix
        p = create_string_buffer(size)
        simbuffer = cast(simvrlib.simvrGetBackLog(),c_char_p)
        memmove(p, simbuffer, size)
        simvrbufferString = p.value.decode('utf-8', errors='ignore')
        print(simvrbufferString.rstrip('\n'))
        simvrlib.simvrClearBackLog()

def simvrUpdateState() :
    global simvrIsOpen
    global simvrlib
    
    stateNo = simvrlib.simvrGetState();

    #State
    if(stateNo <= SimvrInitial) :
        return False
    return True
    
def simvrUpdateSIMVR(roll, pitch, yaw, heave) :
    global simvrIsOpen
    global simvrlib
    
    if(simvrIsOpen == False) :
        return

    packet = simvrPacket()
    packet.roll = roll
    packet.pitch = pitch
    packet.yaw = yaw
    packet.heave = heave

    packet.rotationMotionRatio = 1.0
    packet.gravityMotionRatio = 1.0
    
    simvrlib.simvrWrite(byref(packet));

#---------------------------------------------------
# Main Program
simvrAwake("")
print ("SIMVR-START...")

time.sleep(1) #wait

simvrlib.simvrSetOriginMode(False)
simvrlib.simvrSetAxisProcessingMode(True)

simvrUpdateBackLog()

time.sleep(5) #wait
if(simvrUpdateState()) : 
    print("This program can change ROLL, PITCH, YAW of SIMVR from WRC SHARED MEMORY.")
    
while(simvrUpdateState()) :
    rolldata = 0.0
    pitchdata = 0.0
    yawdata = 0.0
    heavedata = 0.0
    
    try:
        mm.seek(0)
        buffer = io.BytesIO(mm.read())
        buffer.readinto(wrcpkt)
        rolldata = round(-wrcpkt.acceleration_x*0.08,4)
        pitchdata = round(wrcpkt.acceleration_z*0.06,4)
        heavedata = round(wrcpkt.acceleration_y*0.05,4)

    except OSError:
        print("MMAP ERROR")
        break
    
    simvrUpdateSIMVR(float(rolldata), float(pitchdata), float(yawdata), float(heavedata))
    time.sleep(0.05) #wait    
print("SIMVR-SHUTDOWN")

simvrDestroy()
mm.close()
#---------------------------------------------------
