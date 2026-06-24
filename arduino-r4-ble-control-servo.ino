#include <ArduinoBLE.h>
#include <Servo.h>

// define servo

Servo myServo;

const int servoPin = 9;

// define BLE service and characteristic
BLEService servoService("19B10000-E8F2-537E-4F6C-D104768A1214");

// characteristic
// BLEByteCharacteristic servoWriteChar("19B10001-E8F2-537E-4F6C-D104768A1214", BLEWrite);
BLEByteCharacteristic servoWriteChar("19B10001-E8F2-537E-4F6C-D104768A1214", BLEWrite);


void setup() {
 Serial.begin(115200);

 myServo.attach(servoPin);
 myServo.write(0);

 if(!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
 }

   // Set local name and advertised service
  BLE.setLocalName("R4_Servo_Controller");
  BLE.setAdvertisedService(servoService);

  // Add the characteristic to the service
  servoService.addCharacteristic(servoWriteChar);

  // Add the service
  BLE.addService(servoService);

  // Set an initial value for the characteristic
  servoWriteChar.writeValue(90);


 // Start advertising
  BLE.advertise();
  Serial.println("BLE Servo Peripheral is active, waiting for connections...");

}

void loop() {
  // put your main code here, to run repeatedly:
  // Listen for BLE central devices (like your phone) to connect
  BLEDevice central = BLE.central();
   if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      // If the central wrote to the characteristic, update the servo position
      if (servoWriteChar.written()) {
        uint8_t angleValue = servoWriteChar.value();
        
        // Ensure data is within a valid 0-180 degree window
        if (angleValue >= 0 && angleValue <= 180) {
          Serial.print("Moving servo to: ");
          Serial.print(angleValue);
          Serial.println(" degrees");
          
          myServo.write(angleValue);
        }
      }
    }

        // When the central disconnects, print a message
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }

}
