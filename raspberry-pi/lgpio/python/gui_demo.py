
from PIL import ImageDraw, Image, ImageFont
import time
from epd_1inch54 import *
from epd_gui import *
from image import *
import gpiozero

'''define pin conection in BCM number
1.54inch_EPD    Raspberry Pi
      VCC   --->   3.3V
      GND   --->   GND
      RST   --->   D17
      BUSY  --->   D24
      D/C   --->   D25
      MOSI  --->   MOSI
      CLK   --->   CLK
      CS    --->   (CE0)D8 
'''
'''
screen coordinates gor gui functions
-----------------> x (0~200)
|
|
|
|
y(0~200)
'''
if __name__ == '__main__':
    gui = EPD_GUI()
    gui.epd.hw_init()

    gui.epd.whitescreen_all(gImage_0,gImage_1)  # Refresh the picture in full screen
    gui.epd.sleep()  # EPD_sleep,Sleep instruction is necessary, please do not delete!!!
    time.sleep(2)  # delay 2s

    gui.epd.hw_init()  # Electronic paper initialization
    gui.epd.whitescreen_all(gImage_2,gImage_3)  # Refresh the picture in full screen
    gui.epd.sleep()  # EPD_sleep,Sleep instruction is necessary, please do not delete!!!
    time.sleep(2)  # delay 2s

    gui.epd.hw_init()  # Electronic paper initialization
    gui.epd.whitescreen_white()
    # Drawing
    gui.epd.hw_init_gui()  # EPD init
    gui.clear(WHITE)

    font_16 = ImageFont.truetype("MiSans-Light.ttf", FONT_SIZE_16)  # read chinese font file
    font_20 = ImageFont.truetype("MiSans-Light.ttf", FONT_SIZE_20)  # read chinese font file
    font_24 = ImageFont.truetype("MiSans-Light.ttf", FONT_SIZE_24)  # read chinese font file
    font_28 = ImageFont.truetype("MiSans-Light.ttf", FONT_SIZE_28)  # read chinese font file
    # Point
    gui.draw_point(5, 10, BLACK, PIXEL_1X1, DOT_STYLE_DFT)
    gui.draw_point(5, 25, BLACK, PIXEL_2X2, DOT_STYLE_DFT)
    gui.draw_point(5, 40, BLACK, PIXEL_3X3, DOT_STYLE_DFT)
    gui.draw_point(5, 55, BLACK, PIXEL_4X4, DOT_STYLE_DFT)
    # Line
    gui.draw_line(15, 10, 55, 50, BLACK, PIXEL_1X1, LINE_SOLID)
    gui.draw_line(55, 10, 15, 50, BLACK, PIXEL_1X1, LINE_SOLID)
    # Rectangle
    gui.draw_rectangle(15, 10, 55, 50, RED, FILL_EMPTY, PIXEL_1X1)
    gui.draw_rectangle(70, 10, 110, 50, BLACK, FILL_FULL, PIXEL_1X1)
    # Circle
    gui.draw_circle(135, 30, 18, BLACK, FILL_EMPTY, PIXEL_1X1)
    gui.draw_circle(176, 30, 18, BLACK, FILL_FULL, PIXEL_1X1)
    gui.draw_str(10, 75, "abcdefg", BLACK, FONT_SIZE_16, font_16)
    gui.draw_str(10, 100, "ABCabc012345", BLACK, FONT_SIZE_20, font_20)
    gui.draw_str(10, 130, "1.54\" E-Paper", RED, FONT_SIZE_24, font_24)
    gui.draw_str(10, 160, "SEENGREAT", RED, FONT_SIZE_28, font_28)
    # TEST_PIN = 1
    gui.epd.display(gui.BWimg,gui.RWimg)  # display image 
    time.sleep(2)  # 2s
    gui.epd.sleep()  # EPD_DeepSleep, Sleep instruction is necessary, please do not delete!!!


    # Clear screen
    gui.epd.hw_init()  # EPD init  initialization
    gui.epd.display_clear()  # Show all white
    gui.epd.sleep()  # Enter deep sleep, Sleep instruction is necessary, please do not delete!!!
    time.sleep(2)
    print("end")
    exit()

