# ArduinoWireless

ArduinoWireless 868Mhz developed with the ARDUINO microcontroller and a RCQ3-CC1310 shield from the possibility of switching from a minimum of 1 to N° Relay. This remote control system has been designed for the most varied requirements in the field of home automation controls, it can be used to activate all types of lighting, as well as other applications such as heating / cooling systems, electric gates, irrigation systems, automatic doors and industrial systems.

It is possible to set up a point to point configuration (N.1 Arduino - N.1 Actuator) or a point-multipoint configuration (N.1 Arduino - max N° Actuators) up to the possibility of switching 20 Relays. The central unit is composed of a shield (radio section + display + keyboard) which can be purchased on our site and from the software available for free on our site and github:

# Website: https://www.radiocontrolli.com/products/arduino-wireless-868mhz.html

# Source Code: https://github.com/RadioControlli/ArduinoWireless

The actuator units used are powered by a normal 12Volt power supply and can drive normal relay boards or solid state actuator boards (designed and manufactured by RadioControlli).

It is a two-way wireless system (868MHz) that allows the following operations:

1) Send actuation command to the single actuator.
2) At each actuation command the device sends the feedback of the executed operation :
    It is possible at any time to know :
    1) Input State with Button5,
    2) Output State with Button6,
    3) Battery level of the single actuator with the button 7,
    4) Temperature of the single actuator with the button 8,

3) It is possible to send implementation commands to several Actuators (having the same addressing) simultaneously for the implementation of single relays.
4) Possibility of configuring the actuator or actuators available, under "Configure Actuator".
5) Ability to search for new actuators, under "Search Actuator".

# For more information contact us :

mail: sales@radiocontrolli.com
