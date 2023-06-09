#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);
String masterCardID = "xx xx xx xx"; // Ganti dengan ID kartu master yang Anda miliki
int addr = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Ready to read RFID cards");
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String cardID = getCardID();
    mfrc522.PICC_HaltA();

    if (cardID == masterCardID) {
      Serial.println("Master card detected. Enter registration mode.");
      registerCards();
    } else {
      Serial.print("Access denied for card ID: ");
      Serial.println(cardID);
    }
  }
}

String getCardID() {
  String cardID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
//    cardID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    cardID.concat(String(mfrc522.uid.uidByte[i], DEC));
    if (i < mfrc522.uid.size - 1) {
      cardID.concat(" ");
    }
  }
  return cardID;
}

void registerCards() {
  Serial.println("Place a card to register or remove.");

  while (true) {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      String cardID = getCardID();
      mfrc522.PICC_HaltA();

      if (cardID != masterCardID) {
        if (isCardRegistered(cardID)) {
          removeCard(cardID);
          Serial.print("Card removed: ");
          Serial.println(cardID);
        } else {
          registerCard(cardID);
          Serial.print("Card registered: ");
          Serial.println(cardID);
        }
      }
    }
  }
}

bool isCardRegistered(String cardID) {
  // Implementasikan logika untuk memeriksa apakah kartu sudah terdaftar sebelumnya
  // Anda dapat menggunakan EEPROM atau database eksternal untuk menyimpan daftar ID kartu yang terdaftar
  // Kembalikan true jika kartu sudah terdaftar, false jika tidak
}

void registerCard(String cardID) {
  // Implementasikan logika untuk mendaftarkan kartu baru
  // Anda dapat menggunakan EEPROM atau database eksternal untuk menyimpan daftar ID kartu yang terdaftar

  String tempValue = "";

  for (byte i = 0; i < cardID.length(); i++) {
    char currentCharID = cardID.charAt(i);

    if (currentCharID != ' ') {
      tempValue += currentCharID;
    } else {
      EEPROM.write(addr, tempValue.toInt());
      addr++;
      tempValue = "";
    }
  }

  if (tempValue != "") {
    EEPROM.write(addr, tempValue.toInt());
    addr++;
  }
  addr++;
  EEPROM.write(addr, '\0');
}

void removeCard(String cardID) {
  // Implementasikan logika untuk menghapus kartu dari daftar kartu terdaftar
  // Anda dapat menggunakan EEPROM atau database eksternal untuk menyimpan daftar ID kartu yang terdaftar
}
