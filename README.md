# ESP32 Flipper Radar (esp32-flipper-radar-luness02)

Bu proje, çevredeki Flipper Zero cihazlarını (veya benzer BLE/Wi-Fi spam ve test sinyallerini) tespit ederek anında Telegram üzerinden bildirim gönderen taşınabilir bir ESP32 tarayıcı aracıdır.

<p align="center">
    <img src="doc/images/logo.png" alt="Logo">
</p>

## Özellikler
- **BLE Tarama:** Çevredeki Bluetooth Low Energy (BLE) cihazlarının isimlerini ve reklam paketlerini dinler.
- **Wi-Fi Tarama:** Çevredeki şüpheli veya test amaçlı Wi-Fi ağlarını (Rogue AP, spam SSID'leri vb.) yakalar.
- **Anlık Telegram Entegrasyonu:** Hedef bir cihaz tespit edildiğinde, ESP32 internet üzerinden (örneğin telefon hotspot'u aracılığıyla) Telegram botuna anında alarm mesajı gönderir.
- **Akıllı Cooldown:** Aynı cihaz için art arda bildirim spam'i yapılmasını önleyen zamanlayıcı koruması içerir.

## Kullanım ve Kurulum
1. Kodu Arduino IDE üzerinden bir ESP32 kartına yükleyin (`.ino` dosyası).
2. Kod içindeki `ssid` ve `password` alanlarını internet sağlayacak bir ağa (örneğin telefonunuzun Mobil Erişim Noktası / Hotspot ayarlarına) göre düzenleyin.
3. Telegram botunuzun token bilgilerini (`botToken`) ve chat ID'nizi (`chatId`) koda ekleyin.
4. Cihazı çalıştırın. Çevrede Flipper tespit edildiği an Telegram cebinize bildirim atacaktır!

## Donanım Gereksinimleri
- Herhangi bir ESP32 geliştirme kartı (ESP32-DevKitC, ESP32-WROOM-32 vb.)
- İnternet bağlantısı için Wi-Fi kaynağı (Mobil Hotspot önerilir)

## Yasal Uyarı (Disclaimer)
Bu proje eğitim ve çevresel analiz amacıyla geliştirilmiştir. Sadece kendi mülkiyetinizdeki veya tarama iznine sahip olduğunuz ortamlarda kullanın.

## Lisans
Bu proje MIT lisansı altında lisanslanmıştır. Detaylar için [LICENSE](LICENSE) dosyasına bakın.