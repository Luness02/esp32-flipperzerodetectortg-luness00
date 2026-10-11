#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// --- AYARLAR ---
const char* ssid = "EV_WIFI_ADIN";          // İnternete bağlanıp Telegram'a mesaj atmak için
const char* password = "EV_WIFI_SIFREN";

String botToken = "TELEGRAM_BOT_TOKENINIZ";
String chatId = "TELEGRAM_CHAT_IDINIZ";

BLEScan* pBLEScan;
unsigned long lastTelegramSend = 0;
const long intervalBetweenAlerts = 60000; // Aynı cihaz için 1 dakika içinde tekrar spam atmasın

// BLE Cihaz Tarama Callback Sınıfı
class FlipperScanCallbacks: public BLEAdvertasedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      String deviceName = "";
      if (advertisedDevice.haveName()) {
        deviceName = advertisedDevice.getName().c_str();
      }

      // Flipper Zero'nun yaydığı bilinen isim kalıpları (Flipper, Flipper Zero vb.)
      if (deviceName.indexOf("Flipper") != -1 || deviceName.indexOf("flipper") != -1) {
        Serial.println("[!] FLIPPER TESPİT EDİLDİ (BLE)!");
        
        // Cooldown kontrolü (Sürekli mesaj yağmuruna tutmasın)
        if (millis() - lastTelegramSend > intervalBetweenAlerts) {
          sendTelegramAlert("⚠️ Dikkat! Bulunduğun yerde Flipper Zero tespit edildi! Cihaz Adı: " + deviceName);
          lastTelegramSend = millis();
        }
      }
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("[*] ESP32 Flipper Radarı Başlatılıyor...");

  // Wi-Fi'ı başlangıçta Telegram bildirimi gönderebilmek için STA modunda başlat
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n[+] Wi-Fi Bağlantısı Başarılı!");

  // BLE Tarayıcıyı Başlat
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new FlipperScanCallbacks());
  pBLEScan->setActiveScan(true); // Aktif tarama (cihaz isimlerini almak için önemli)
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
}

void loop() {
  // 1. Adım: BLE Taraması Gerçekleştir (Her tarama 5 saniye sürer)
  Serial.println("[*] BLE taraması yapılıyor...");
  BLEScanResults foundDevices = pBLEScan->start(5, false);
  pBLEScan->clearResults(); // Hafızayı temizle

  // 2. Adım: Wi-Fi Çevresini Tarama (Flipper Marauder / Wi-Fi spam testleri için)
  scanWiFiForFlipper();

  delay(2000); // Taramalar arası kısa bekleme
}

void scanWiFiForFlipper() {
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i) {
    String ssidName = WiFi.SSID(i);
    // Flipper'ın yaygın Wi-Fi spam / Rickroll SSID kalıplarını yakala
    if (ssidName.indexOf("Flipper") != -1 || ssidName.indexOf("bad") != -1 || ssidName.length() > 30) { 
      if (millis() - lastTelegramSend > intervalBetweenAlerts) {
        sendTelegramAlert("⚠️ Wi-Fi Çevresinde Şüpheli Flipper Hareketliliği! SSID: " + ssidName);
        lastTelegramSend = millis();
      }
    }
  }
  WiFi.scanDelete(); // Wi-Fi tarama belleğini boşalt
}

void sendTelegramAlert(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure(); // SSL sertifika doğrulamasını atla

    if (client.connect("api.telegram.org", 443)) {
      String url = "/bot" + botToken + "/sendMessage?chat_id=" + chatId + "&text=" + message;
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: api.telegram.org\r\n" +
                   "User-Agent: ESP32-Radar\r\n" +
                   "Connection: close\r\n\r\n");
      Serial.println("[+] Telegram Alarmı Gönderildi!");
    } else {
      Serial.println("[-] Telegram sunucusuna bağlanılamadı.");
    }
  }
}