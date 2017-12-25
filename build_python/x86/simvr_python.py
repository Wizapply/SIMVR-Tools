# -*- coding: cp932 -*-
# Import
from ctypes import *
import time

# Status Define
Initial = 0
CanNotFindUsb = 1
CanNotFindSimvr = 2
CanNotCalibration = 3
TimeoutCalibration = 4
Running = 5
StopActuator = 6
ShutDownActuator = 7
Pause = 8
CanNotCertificate = 9
CalibrationRetry = 10

# Simvr Data Packet
class simvrPacket(Structure):  
    _fields_ = [  
            ("axis1", c_float),  
            ("axis2", c_float),  
            ("axis3", c_float),
            ("axis4", c_float),  
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
            ("commandCount", c_int),  
            ("commandStride", c_int),
            ("command", c_char*256)
            ]  
  
    def __init__(self):  
        self.axis1 = 0.5  #default param
        self.axis2 = 0.5  
        self.axis3 = 0.5
        self.axis4 = 0.5
        self.speedAxis123 = 1.0
        self.accelAxis123 = 0.5
        self.speedAxis4 = 1.0
        self.accelAxis4 = 0.5
        self.roll = 0.0
        self.pitch = 0.0
        self.yaw = 0.0
        self.heave = 0.0
        self.sway = 0.0
        self.surge = 0.0
        self.rotationMotionRatio = 0.3
        self.gravityMotionRatio = 0.7
        self.commandCount = 0 

# Load dll
simvrlib = cdll.LoadLibrary("simvr.dll");
simvrIsOpen = False

# Methods
def simvrAwake(appCode) :
    global simvrIsOpen
    global simvrlib
    if(simvrIsOpen == True) :
        return
    if(simvrlib.simvrOpen(appCode) == 0):
        simvrIsOpen = True
    else:
        print "SIMVR DLL ERROR!"

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
        p = create_string_buffer(size)
        simbuffer = cast(simvrlib.simvrGetBackLog(),c_char_p)
        memmove(p, simbuffer, size)
        simvrbufferString = p.value;
        print simvrbufferString.rstrip("\n")
        simvrlib.simvrClearBackLog();

def simvrUpdateState() :
    global simvrIsOpen
    global simvrlib
    
    stateNo = simvrlib.simvrGetState();
    #State
    if(stateNo != -1 and stateNo != Running and stateNo != StopActuator) :
        return False
    return True
    
def simvrUpdateSIMVR(roll, pitch, yaw) :
    global simvrIsOpen
    global simvrlib
    
    if(simvrIsOpen == False) :
        return

    packet = simvrPacket()
    packet.roll = roll
    packet.pitch = pitch
    packet.yaw = yaw
    
    simvrlib.simvrWrite(byref(packet));

#---------------------------------------------------
# Main Program

simvrAwake("")
print "SIMVR-START..."

time.sleep(1) #wait

simvrlib.simvrSetOriginMode(False)
simvrlib.simvrSetAxisProcessingMode(True)


simvrUpdateBackLog()

print "This program can change ROLL, PITCH, YAW of SIMVR. \nSpecification value [-1.0 to 1.0]. And, this is ended in an [exit] input."

while(simvrUpdateState()) :
    rolldata = raw_input('ROLL >> ')	#python3 = input(*)
    if(rolldata == 'exit') : break
    pitchdata = raw_input('PITCH >> ')
    if(pitchdata == 'exit') : break
    yawdata = raw_input('YAW >> ')
    if(yawdata == 'exit') : break
    
    simvrUpdateSIMVR(float(rolldata), float(pitchdata), float(yawdata))
    print "SIMVR RUN"
    
print "SIMVR-SHUTDOWN"

simvrDestroy()
#---------------------------------------------------
