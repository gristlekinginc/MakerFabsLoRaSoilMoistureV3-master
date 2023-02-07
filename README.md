# MakerFabs LoRa Soil Moisture V3 - Platformio
I'll start this by saying I don't know what I'm doing.  I aim to stay close to competent folks who help me.  This journey kicked off when I was looking for a sensor to put on the Helium network for a project in local ephemeral wetlands called vernal pools.

I found the MakerFabs V3 option via @dirkbeer and ordered a few.  
**https://www.makerfabs.com/lora-soil-moisture-sensor-v3.html**

The upside is they are cheap, available, and have a known working sketch on Arduino.  All of those remain true.  The downside is that they're kind of of a PITA to get to work when you're new to all this.

As an entry level geek, I got frustrated at the number of attempts it took to flash the device, which appears to be a combination of not using the MakerFabs suggested USB to serial/TTL converter and the Arduino sketch being right at the capacity limit of the device.

Now, I'm not a code guy.  I looked around and found someone who is, asked @raomin to look at and improve this, and this is what he came up with.  Hopefully it's useful to more than just me.  

## Useful Links
Dirk's Guide: https://github.com/dirkbeer/Helium_MakerfabsSoilMoistureSensorV3

| Item | Link |
| --- | ----------- |
| Sensor | https://www.makerfabs.com/lora-soil-moisture-sensor-v3.html |
| 3.3V USB to TTL / Serial converter | https://www.makerfabs.com/cp2104-usb-to-serial-converter.html |
