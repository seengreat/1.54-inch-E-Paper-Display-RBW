# Seengreat 1.54 Inch E-Paper Display demo
# Author(s):Andy Li from Seengreat

import os
import sys
import spidev
import time
from gpiozero import *
import numpy as np
"""the following pin definiting use BCM"""
PIN_CS     = 8
PIN_DC     = 25
PIN_BUSY   = 24
PIN_RST    = 17
EPD_WIDTH  = 200
EPD_HEIGHT = 200

class EPD_1Inch54():
    def __init__(self):

        # spi init
        self.bus = 0
        self.dev = 0
        self.spi_speed = 8000000
        self.spi = spidev.SpiDev()
        self.spi.open(self.bus, self.dev)
        self.spi.max_speed_hz = self.spi_speed
        self.spi.mode = 0b00
        #self.spi.no_cs = False
        # gpio init
        #self.cs = LED(PIN_CS)
        self.dc = DigitalOutputDevice(PIN_DC,active_high = True,initial_value =False)
        self.rst = DigitalOutputDevice(PIN_RST,active_high = True,initial_value =False)
        self.busy = DigitalInputDevice(PIN_BUSY,pull_up=True,active_state=None)
        self.w = EPD_WIDTH
        self.h = EPD_HEIGHT
        
    def write_cmd(self, cmd):
        """write command"""
        self.dc.off()#wpi.digitalWrite(PIN_DC,wpi.LOW)
        self.spi.writebytes([cmd])
        
    def write_data(self, value):
        """write data"""
        self.dc.on()#wpi.digitalWrite(PIN_DC,wpi.HIGH)
        self.spi.writebytes([value])
        
    def chkstatus(self):
        while self.busy.value==0:
            pass
        
    def reset(self):
        """reset the epd"""
        self.rst.off()#wpi.digitalWrite(PIN_RST,wpi.LOW)
        time.sleep(0.1)
        self.rst.on()#wpi.digitalWrite(PIN_RST,wpi.HIGH)
        time.sleep(0.1)
        
    def hw_init(self):
        """epd init..."""
        self.reset()
        self.chkstatus()
        self.write_cmd(0x12)
        self.chkstatus()
        
        self.write_cmd(0x01) #Driver output control
        self.write_data(0xC7) #199+1=200
        self.write_data(0x00)
        self.write_data(0x00)

        self.write_cmd(0x11) #data entry mode
        self.write_data(0x01)
        
        self.write_cmd(0x44) #set Ram-X address start/end position
        self.write_data(0x00)
        self.write_data(0x18) #0x0F-->(24+1)*8=200
        
        self.write_cmd(0x45) #set Ram-Y address start/end position
        self.write_data(0xC7)
        self.write_data(0x00)
        self.write_data(0x00)
        self.write_data(0x00)
        
        self.write_cmd(0x3C) #BorderWavefrom,
        self.write_data(0x05)

        self.write_cmd(0x21)
        self.write_data(0x00)
        
        self.write_cmd(0x18)
        self.write_data(0x80)
        
        self.write_cmd(0x4E)
        self.write_data(0x00)
        self.write_cmd(0x4F)
        self.write_data(0xC7)
        self.write_data(0x00)
        self.chkstatus()

    def hw_init_gui(self):
        self.reset()
        self.chkstatus()
        self.write_cmd(0x12)  # SWRESET
        self.chkstatus()
        self.write_cmd(0x01)  # Driver output control 
        self.write_data(0xC7)
        self.write_data(0x00)
        self.write_data(0x01)  # 0x00:Show normal 0x01:Show mirror

        self.write_cmd(0x11)  # data entry mode       
        self.write_data(0x01)
        
        self.write_cmd(0x44)  # set Ram-X address start/end position   
        self.write_data(0x00)
        self.write_data(0x18)  # 0x0F-->(24+1)*8=200

        self.write_cmd(0x45)  # set Ram-Y address start/end position        
        self.write_data(0xC7)  # 0xC7-->(199+1)=200
        self.write_data(0x00)
        self.write_data(0x00)
        self.write_data(0x00)

        self.write_cmd(0x3C)  # BorderWavefrom
        self.write_data(0x05)
        
        self.write_cmd(0x21)  # BorderWavefrom
        self.write_data(0x00)

        
        self.write_cmd(0x18)  # Read built-in temperature sensor
        self.write_data(0x80)

        self.write_cmd(0x4E)  # set RAM x address count to 0
        self.write_data(0x00)
        self.write_cmd(0x4F) # set RAM y address count to 0X199
        self.write_data(0xC7)
        self.write_data(0x00)
        self.chkstatus()
        
    def update(self):
        self.write_cmd(0x22)
        self.write_data(0xF7)
        self.write_cmd(0x20)
        self.chkstatus()

    # display
    def whitescreen_all(self,BWdatas,RWdatas):
        self.write_cmd(0x24) #write RAM for black(0)/white (1)
        for i in range(5000):
            self.write_data(BWdatas[i])
        self.write_cmd(0x26) #write RAM for red(0)/white (1)
        for i in range(5000):
            self.write_data(~RWdatas[i])
        self.update()

    def whitescreen_white(self):
        self.write_cmd(0x24) # write RAM for black(0) / white(1)
        for k in range(200):
            for i in range(25):
                self.write_data(0xff)
        self.write_cmd(0x26) 
        for k in range(200):
            for i in range(25):
                self.write_data(0x00)
        self.update()

    def sleep(self):
        self.write_cmd(0x10)
        self.write_data(0x01)
        time.sleep(0.01)

    def display_clear(self):
        self.write_cmd(0x24)
        for i in range(5000):
            self.write_data(0xFF)
        self.write_cmd(0x26)
        for i in range(5000):
            self.write_data(0x00)
        self.update()

    def display(self, BWimage,RWimage):
        if self.w%8 == 0:
            width = self.w//8
        else:
            width = self.w//8+1
        height = self.h

        self.write_cmd(0x24)
        for j in range(height):
            for i in range(width):
               self.write_data(BWimage[i + j * width])
        self.write_cmd(0x26)
        for j in range(height):
            for i in range(width):
               self.write_data(~RWimage[i + j * width])
        self.update()
