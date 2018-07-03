# DigiBadge_Mini
Upgraded and updated version of the [V2 DigiBadge](https://github.com/PhoenixbornTech/DigiBadgeV2). While keeping the same size profile, a few updates and changes have been made to the board and its software. But of course, you're wondering: What are the changes?

 - Added a small flash memory chip
 - Replaced the control "stick" with a set of three buttons
 - Broke out many pins: Analog 0-7 and Digital 2, 3, 4, 11, 12, and 13, along with VCC and GND. Buttons 0, 1, and 2 are assigned to D2, D3, and D4 respectively. I2C is available via A4 and A5, and SPI is available on D11, D12, and D13.
 - The FTDI connector is also broken out. If the FTDI chip isn't present, GND, VCC, RST, D0, and D1 can be accessed. 
 - No power switch! Instead, the device can be put into a super low power mode that effectively doesn't use any power, just like a TV remote.
 - The board has been redesigned so that these can be ordered and manufactured, instead of being entirely hand-assembled.
 - Other minor board tweaks have been made, including making the entire device not hard reset when an SD card is inserted.
 
## Plans for the FUTURE!
After selling these at BABSCon and getting some real-world usage out of them, there are a few things that need to be addressed, largely in the hardware. These fall under the category of [**Revision 2**](https://github.com/PhoenixbornTech/DigiBadge_Mini/milestone/3). Currently, those goals are:

 - Improving the buttons. The current ones are inexpensive, but not the easiest or most intuitive to use.
 - Labeling the FTDI orientation. The R1 board has no indication of pin direction, which can be annoying.
 - Add reverse voltage protection, to ensure nobody fries their board by accidentally putting the batteries in backwards.
 - Adding a voltage regulator. This will allow for consistent backlight brightness and utilizing more of the battery for a longer battery life. This will also allow for LiPo batteries to be used as well.
 
## How it works

With the latest releases, the DigiBadge operates using a Menu system that should be fairly straightforward.

From within a mode, pressing the left or right buttons will change which badge/flag/image is displayed, while pressing the center button will access the menu.

While in the Menu, pressing the left button (When looking at the screen) moves the cursor up, pressing the right button moves the cursor down, and the center button accesses the current selection. If the selection is a mode, the badge goes to that mode. If it is another menu, such as brightness, it goes into that menu.

Bear in mind that while the Slideshow Time menu will automatically return to the main menu on making a selection, **Brightness will not**. This is so you can select a brightness and see the difference. Exiting the menu locks the selection in.
 
The DigiBadge has four modes, in the order that they cycle through: **Color Communication Badge**, **Image Slideshow**, **Static Image**, and **Pride Flags**. Image Slideshow and Static Images are inaccessible (And greyed out) when there is no SD card detected.

The DigiBadge will *automatically* load the SD card when it is inserted. Thanks to moderately recent updates to the default Arduino library, this no longer requires edits to the SD library.
 
## Color Communication Badges
 
Color Communication Badges were *inspired* by BronyCon's use of them, but they were *originated* by the Autism Self-Advocacy Network. You can read what ASAN has to say on the Color Communication Badges on [their website](http://autisticadvocacy.org/2014/02/color-communication-badges/).
 
The badges on the DigiBadge are not identical to ASAN or BronyCon versions, as they have been adapted for viewing on a small screen. In the future, it would be great to allow for users to select a preference for style of these badges, but at the moment it is uncertain if that can be fit onto a device with the limited memory of an ATMega328p.

What you see on the DigiBadge is not an image file - It's programatically created each time it is displayed.
 
## Pride Flags
 
The DigiBadge also includes a variety of Pride Flags for users to display. Only a few flags were omitted, and those were ones that would be difficult to display programatically. As with the Color Communication Badges, these are created each time they are displayed.

There are currently *seven* Pride Flags built in to the DigiBadge Mini, in the order they appear: The **LGBT+ Flag**, the **Bisexual Flag**, the **Transexual Flag**, the **Pansexual Flag**, the **Asexual Flag**, the **Nonbinary Flag** and the **Agender Flag**.

##Image and Slideshow Modes

The Image and Slideshow modes are closely related. Both of them display 24-bit BMP images loaded from the SD card. Both of them will be skipped if no SD card is present. The difference is that the **Static Image** mode will display the image until told to change. The **Slideshow** mode will display a new semi-random image (It won't pick the current or previous image) every few seconds, and will display a "Play" icon in the corner.