/*
   read a NFC tag and send it to serial
*/

// Adafruit lib to drive the NFC Shield
#include <Adafruit_PN532.h>

// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield

// Or use this line for a breakout or shield with an I2C connection:
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

int inByte = 0; // incoming serial byte

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  // NFC reader initialization
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

  // configure board to read RFID tags
  nfc.SAMConfig();
}

uint8_t currentUid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the current UID

void loop() {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 100);

  if (success) {
    // Display some basic information about the card
    //Serial.print("UID Value: ");
    //nfc.PrintHex(uid, uidLength);
  }

  if (compare(uid, currentUid)) {
    // pas de changement de tag ...
  }
  else {
    //Serial.println("Bim !");
    printTag(uid);
    copy(uid, currentUid);
  }
}

bool compare(uint8_t * uid1, uint8_t * uid2) {
  for (int i = 0; i < 7; i++) {
    if (uid1[i] != uid2[i]) {
      return false;
    }
  }
  return true;
}

void copy(uint8_t * uidSource, uint8_t * uidTarget) {
  for (int i = 0; i < 7; i++) {
    uidTarget[i] = uidSource[i];
  }
}

void printTag(uint8_t * uid) {
  for (int i = 0; i < 7; i++) {
    Serial.print(uid[i], HEX);
  }
  Serial.print("\n");
}
