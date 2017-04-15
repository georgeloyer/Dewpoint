# Dewpoint
# project to detect humidity, ambient temperature and object temperature
# and then calculate the dewpoint.
#
#   reports the data to an IoT MQTT database
#   alerts the user on the device and via IoT email alerts when dewpoint is reached
#   actuates an ambient or object heater to heat the object or air above the dewpoint
#
# early versions using Raspberry Pi with python code, now abandoned
# current versions using arduino code and feather m0 with 2.4-inch tft display

# sensor for humidity and ambient temp: DHT-11
# sensor for object temp: TMP007
