#include <ESP8266WiFi.h> // Include the necessary libraries for ESP8266 Wi-Fi and MySQL connection
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <string.h>

const char* ssid = "Galaxy M33 5G9F82";
const char* password = "laki1234";

IPAddress server_addr(185, 214, 124, 4);
char user[] = "u759114105_Laki";
char password_db[] = "Chandu123"; // MySQL password

WiFiClient client; // Create a Wi-Fi client and MySQL connection objects
MySQL_Connection conn((Client *)&client);
MySQL_Cursor cur_mem(&conn);

void setup() {
  Serial.begin(9600); // Start the serial communication for debugging

  WiFi.begin(ssid, password); // Connect to Wi-Fi network

  while (WiFi.status() != WL_CONNECTED) { // Wait until Wi-Fi connection is established
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.print(WiFi.localIP()); // Print the local IP address after connecting

  if (conn.connect(server_addr, 3306, user, password_db)) { // Connect to the MySQL server
    Serial.println("Connection to MySQL server success!");
  } else {
    Serial.println("Connection to MySQL server failed!");
    while (1);
  }

  // Set timezone for the current session
  if (cur_mem.execute("SET time_zone = '+5:30';")) { // Set the timezone to Asia/Kolkata (UTC +5:30)
    Serial.println("Timezone set to Asia/Kolkata!");
  } else {
    Serial.println("Failed to set timezone!");
  }
}

void loop() {
  if (Serial.available() > 0) { // Check if there is data available from the Arduino
    String data = Serial.readStringUntil('\n'); // Read the data from the serial input until a newline character is received
    Serial.print("### Data received from Arduino: ");
    Serial.println(data);

    // Extract sensor values from the received data string using comma as a separator
    int commaIndex1 = data.indexOf(',');
    int commaIndex2 = data.indexOf(',', commaIndex1 + 1);
    int commaIndex3 = data.indexOf(',', commaIndex2 + 1);
    int commaIndex4 = data.indexOf(',', commaIndex3 + 1);
    int commaIndex5 = data.indexOf(',', commaIndex4 + 1);
    int commaIndex6 = data.indexOf(',', commaIndex5 + 1);
    int commaIndex7 = data.indexOf(',', commaIndex6 + 1);
    int commaIndex8 = data.indexOf(',', commaIndex7 + 1);

    float temperature = data.substring(0, commaIndex1).toFloat();
    float humidity = data.substring(commaIndex1 + 1, commaIndex2).toFloat();
    long distance = data.substring(commaIndex2 + 1, commaIndex3).toInt();
    int ldrValue = data.substring(commaIndex3 + 1, commaIndex4).toInt();
    int flameValue = data.substring(commaIndex4 + 1, commaIndex5).toInt(); // Corrected
    int pirValue = data.substring(commaIndex5 + 1, commaIndex6).toInt();
    int soilMoistureValue = data.substring(commaIndex6 + 1, commaIndex7).toInt();
    int rainValue = data.substring(commaIndex7 + 1, commaIndex8).toInt();
    int heartbeatValue = data.substring(commaIndex8 + 1).toInt();

    // Prepare the SQL query to insert the sensor values into the database
    char INSERT_SQL[] = "INSERT INTO u759114105_Sensortable.Sensor_table (temperature, humidity, distance, ldr_value, flame_value, pir_value, soilMoisture_value, rain_value, heartbeat_value,timestamp_column) VALUES (%f,%f,%d,%d,%d,%d,%d,%d,%d,NOW())";
    char query[256];
    sprintf(query, INSERT_SQL, temperature, humidity, distance, ldrValue, flameValue, pirValue, soilMoistureValue, rainValue, heartbeatValue);

    if (cur_mem.execute(query)) { // Execute the SQL query to insert the data into the database
      Serial.println("Data inserted into the database.");
    } else {
      Serial.println("Data insertion failed.");
    }
  }

  delay(5000); // Wait for some time before uploading the next data
}