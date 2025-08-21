/*
  The sketch is showing how to parse a file with weather forecast data 
  from the Weather Service Provider Meteomatics.com.
  It is a static parser that parses a specific data structure:
  - ISO8061 encoded timestamp
  - Temperature in degrees Celsius
  - Precipitation in millimeters

  In my final project I received the data from an HTTP Client.
*/

/*
Version Management
21.08.2025 V03 Tutorial version
20.08.2025 V02 improved version
17.08.2025 V01 Initial programming
*/

// --------------------------------------------------------------
// Programm Information
const char *PROGRAM_VERSION = "ESP32 Simple CSV Parser V03";

#define PRINT_CONTENT
#define PROCESS_DELIMITERS
#define PROCESS_HEADER
#define PROCESS_FULL_FILE

// --------------------------------------------------------------
// API data
// The data is from a HTTP Client
String httpResponseString = "validdate;t_2m:C;precip_1h:mm\n2025-08-16T21:45:00Z;16.7;0.00\n2025-08-16T22:45:00Z;15.5;3.50\n";
// validdate;t_2m:C;precip_1h:mm
// 2025-08-16T21:45:00Z;16.7;0.00
// 2025-08-16T22:45:00Z;15.5;0.00
// the lines are separated by a 0x0A character
// last line has 0x0A character as well

// --------------------------------------------------------------
// UI
const char *DIVIDER_LINE = "-----------------------------------";

// --------------------------------------------------------------
// Global variables for parsing
uint16_t payloadLength;
const uint8_t MAXIMUM_FORECASTS = 240; // 1 forecast/hour = 24 forecasts a day, 10 + 2 days is 12 * 24 = 288
String forecastDates[MAXIMUM_FORECASTS];
int temperatures[MAXIMUM_FORECASTS];
int precips[MAXIMUM_FORECASTS];
//float temperatures[MAXIMUM_FORECASTS];
//float precips[MAXIMUM_FORECASTS];
uint16_t numberOfForecasts = 0;

void clearTempPrecipArrays() {
  for (int i = 0; i < MAXIMUM_FORECASTS; i++) {
    forecastDates[i] = "";
    temperatures[i] = 0.0;
    precips[i] = 0.0;
  }
  numberOfForecasts = 0;
}

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println(PROGRAM_VERSION);
  Serial.println(DIVIDER_LINE);

  Serial.println("httpResponseString (raw data):");
  Serial.println(httpResponseString);
  Serial.println(DIVIDER_LINE);
  
#ifdef PRINT_CONTENT  
  Serial.println("Content of the data in ASCII and HEX encoding");
  payloadLength = httpResponseString.length();
  Serial.printf("payloadLength: %d\n", payloadLength);
  Serial.println(DIVIDER_LINE);

  Serial.println("ASCII encoding");
  for (int i = 0; i < payloadLength; i++) {
    char element = httpResponseString.charAt(i);
    Serial.print(element);
    Serial.print(" ");
    if ((i + 1) % 16 == 0) Serial.println();
  }
  Serial.println();
  Serial.println(DIVIDER_LINE);

  Serial.println("HEX encoding");
  for (int i = 0; i < payloadLength; i++) {
    char element = httpResponseString.charAt(i);
    Serial.print(element, HEX);
    Serial.print(" ");
    if ((i + 1) % 16 == 0) Serial.println();
  }
  Serial.println();
  Serial.println(DIVIDER_LINE);
#endif  

  // global variables, are defined in header and not here in setup()
  /*
  const uint8_t MAXIMUM_FORECASTS = 290;  // 1 forecast/hour = 24 forecasts a day, 10 + 2 days is 12 * 24 = 288
  String forecastDates[MAXIMUM_FORECASTS];
  float temperatures[MAXIMUM_FORECASTS];
  float precips[MAXIMUM_FORECASTS];
  uint16_t numberOfForecasts = 0;
  */

  // start parsing
  bool isHeaderLine = true;
  char FIELD_DELIMITER = ';';
  char LINE_DELIMITER = 0x0A;
  String headerField1, headerField2, headerField3;
  String validdate;
  //float temperature, precip;
  uint16_t lastIndex = 0;
  uint16_t newIndex = 0;
  uint8_t fieldIndex = 0;
  uint16_t datasetIndex = 0;
  bool isDatasetComplete = false;

#ifdef PROCESS_DELIMITERS
  Serial.println("Find FIELD_DELIMITER and LINE_DELIMITER");
  for (uint16_t i = 0; i < payloadLength; i++) {
    char element = httpResponseString.charAt(i);
    if (element == FIELD_DELIMITER) {
      Serial.printf("FIELD_DELIMITER found at pos %d\n", i);
    }
    if (element == LINE_DELIMITER) {
      Serial.printf("LINE_DELIMITER found at pos %d\n", i);
    }
  }
  Serial.println(DIVIDER_LINE);
#endif

#ifdef PROCESS_HEADER
  Serial.println("Get CSV Header data");
  lastIndex = 0;
  newIndex = 0;
  fieldIndex = 0;
  datasetIndex = 0;
  for (uint16_t i = 0; i < payloadLength; i++) {
    char element = httpResponseString.charAt(i);
    if (element == FIELD_DELIMITER) {
      //Serial.printf("FIELD_DELIMITER found at pos %d\n", i);
      if (fieldIndex == 0) {
        newIndex = i;
        fieldIndex++;
        headerField1 = httpResponseString.substring(lastIndex, newIndex);
        lastIndex = newIndex + 1;
      } else if (fieldIndex == 1) {
        newIndex = i;
        fieldIndex++;
        headerField2 = httpResponseString.substring(lastIndex, newIndex);
        lastIndex = newIndex + 1;
      } else if (fieldIndex == 2) {
        // note: this is never reached as there is no field delimiter but a line delimiter
        Serial.println("### fieldIndex2 should never reached by FIELD_DELIMITER - Error in dataset !");
        newIndex = i;
        fieldIndex++;
        headerField3 = httpResponseString.substring(lastIndex, newIndex);
        lastIndex = newIndex;
      }
    }
    if (element == LINE_DELIMITER) {
      //Serial.printf("LINE_DELIMITER found at pos %d\n", i);
      if (fieldIndex == 2) {
        newIndex = i;
        fieldIndex = 0;  // start with a new one
        headerField3 = httpResponseString.substring(lastIndex, newIndex);
        lastIndex = newIndex + 1;
        datasetIndex++;
      }
    }

    // stop the workflow after the header
    if (datasetIndex > 0) break;
  }
  // print the column names
  Serial.printf("headerField1: %s\nheaderField2: %s\nheaderField3: %s\n", headerField1, headerField2, headerField3);
  Serial.println(DIVIDER_LINE);
#endif  

#ifdef PROCESS_FULL_FILE
  Serial.println("Parse the complete file");

  lastIndex = 0;
  newIndex = 0;
  fieldIndex = 0;
  datasetIndex = 0;
  isHeaderLine = true;
  clearTempPrecipArrays();
  for (int i = 0; i < payloadLength; i++) {
    char element = httpResponseString.charAt(i);
    if (isHeaderLine) {
      if (element == FIELD_DELIMITER) {
        //Serial.printf("FIELD_DELIMITER found at pos %d\n", i);
        if (fieldIndex == 0) {
          newIndex = i;
          fieldIndex++;
          headerField1 = (String) httpResponseString.substring(lastIndex, newIndex);
          lastIndex = newIndex + 1;
        } else if (fieldIndex == 1) {
          newIndex = i;
          fieldIndex++;
          headerField2 = httpResponseString.substring(lastIndex, newIndex);
          lastIndex = newIndex + 1;
        } else if (fieldIndex == 2) {
          // note: this is never reached as there is no field delimiter but a line delimiter
          Serial.println("### fieldIndex2 should never reached by FIELD_DELIMITER - Error in dataset !");
          newIndex = i;
          fieldIndex++;
          headerField3 = httpResponseString.substring(lastIndex, newIndex);
          lastIndex = newIndex;
        }
      }
      if (element == LINE_DELIMITER) {
        //Serial.printf("LINE_DELIMITER found at pos %d\n", i);
        if (fieldIndex == 2) {
          newIndex = i;
          fieldIndex = 0;  // start with a new one
          headerField3 = httpResponseString.substring(lastIndex, newIndex);
          lastIndex = newIndex + 1;
          isHeaderLine = false;
        }
      }
      // end of isHeaderLine
    } else {
      // this is the workflow for the data section of the csv data
      if (element == FIELD_DELIMITER) {
        //Serial.printf("FIELD_DELIMITER found at pos %d\n", i);
        if (fieldIndex == 0) {
          // this is the String encoded field 'validdate' = date & time of data
          newIndex = i;
          fieldIndex++;
          // limiting to MAXIMUM_FORECASTS
          if (datasetIndex < MAXIMUM_FORECASTS) {
            forecastDates[datasetIndex] = httpResponseString.substring(lastIndex, newIndex);
          }
          lastIndex = newIndex + 1;
        } else if (fieldIndex == 1) {
          // this is the float encoded field 't _ 2 m : C' = temperature
          newIndex = i;
          fieldIndex++;
          // limiting to MAXIMUM_FORECASTS
          if (datasetIndex < MAXIMUM_FORECASTS) {
            temperatures[datasetIndex] = (httpResponseString.substring(lastIndex, newIndex).toFloat() * 100);
          }
          lastIndex = newIndex + 1;
        } else if (fieldIndex == 2) {
          // note: this is never reached as there is no field delimiter but a line delimiter
          Serial.println("### fieldIndex2 should never reached by FIELD_DELIMITER - Error in dataset !");
        }
      }
      if (element == LINE_DELIMITER) {
        if (fieldIndex == 2) {
          // this is the float encoded field 'p r e c i p _ 1 h : m m' = precipitation
          newIndex = i;
          fieldIndex = 0;  // start with a new one
          if (datasetIndex < MAXIMUM_FORECASTS) {
            precips[datasetIndex] = (httpResponseString.substring(lastIndex, newIndex).toFloat() * 100);
          }
          lastIndex = newIndex + 1;
          datasetIndex++;
        }
      }
    }
  }
  // limiting to MAXIMUM_FORECASTS
  if (datasetIndex < MAXIMUM_FORECASTS) {
    numberOfForecasts = datasetIndex;
  } else {
    numberOfForecasts = MAXIMUM_FORECASTS;
  }
  Serial.println(DIVIDER_LINE);
  Serial.println("Parsing result");
  Serial.println("-= Header section data =-");
  Serial.printf("headerField1: %s\nheaderField2: %s\nheaderField3: %s\n", headerField1, headerField2, headerField3);
  Serial.println(DIVIDER_LINE);

  Serial.println("-= Data section data =-");
  Serial.printf("Number of forecasts: %d\n", numberOfForecasts);
  for (int i = 0; i < numberOfForecasts; i++) {
    Serial.printf("%03d: %s %1.2f C %1.2f mm\n", (i + 1), forecastDates[i].c_str(), temperatures[i] / 100.0, precips[i] / 100.0);
  }
  Serial.println(DIVIDER_LINE);
#endif  
}

void loop() {
}