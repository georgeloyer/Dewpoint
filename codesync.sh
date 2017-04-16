#!/bin/bash

# codesync.sh
# Syncs the files from a list of source directories to a list
# of destination directories, only updating those files that
# have a difference.

# arduino_code
rsync -azvt /Users/gloyer/Documents/Arduino/DewpointDetector /Users/gloyer/Documents/Projects/Dewpoint/arduino_code
rsync -azvt /Users/gloyer/Documents/Arduino/DewpointCalcTest /Users/gloyer/Documents/Projects/Dewpoint/arduino_code
rsync -azvt /Users/gloyer/Documents/Arduino/FeatherHumTempDew /Users/gloyer/Documents/Projects/Dewpoint/arduino_code

# arduino_libs
rsync -azvt /Users/gloyer/Documents/Arduino/libraries/Adafruit_STMPE610 /Users/gloyer/Documents/Projects/Dewpoint/arduino_libs
rsync -azvt /Users/gloyer/Documents/Arduino/libraries/Adafruit_TMP007 /Users/gloyer/Documents/Projects/Dewpoint/arduino_libs
rsync -azvt /Users/gloyer/Documents/Arduino/libraries/DHT-sensor-library /Users/gloyer/Documents/Projects/Dewpoint/arduino_libs
rsync -azvt /Users/gloyer/Documents/Arduino/libraries/Adafruit_Sensor /Users/gloyer/Documents/Projects/Dewpoint/arduino_libs
rsync -azvt /Users/gloyer/Documents/Arduino/libraries/Adafruit_MQTT_Library /Users/gloyer/Documents/Projects/Dewpoint/arduino_libs
rsync -azvt /Users/gloyer/Documents/Arduino/libraries/Adafruit-_ILI9341 /Users/gloyer/Documents/Projects/Dewpoint/arduino_libs
rsync -azvt /Users/gloyer/Documents/Arduino/libraries/Adafruit-GFX-Library /Users/gloyer/Documents/Projects/Dewpoint/arduino_libs
rsync -azvt /Users/gloyer/Documents/Arduino/libraries/Adafruit_SSD1306 /Users/gloyer/Documents/Projects/Dewpoint/arduino_libs
