/*
 * dewLog.h
 * History:
 *  2017-05-14  gel Original
 * defines the struct for the dewpoint detector log data
 * 
 */

 struct DewLog {
  String fullPathLogFile;         // full path name of the current log file
  String dateTime;                // date-time string
  float humidity;                 // DHT-22 humidity sensor reading
  float temperature;              // DHT-22 temperature sensor reading
  float mirrorTemp;               // TMP007 object temperature sensor reading
  float dewpointTemp;             // calculated dew point temperature based on humidty and ambient temperature
  int heaterStatus;               // heater status (on = true, off = false)      
 };

