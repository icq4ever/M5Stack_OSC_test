#include <Arduino.h>
#include <ArduinoOSC.h>
#include <M5Stack.h>
#include <wifiInfo.h>
// #include <M5Typo/M5Typo.h>
#include <WiFi.h>

// ip, gateway, subnet
const IPAddress ip(192, 168, 0, 151);
const IPAddress gateway(192, 168, 0, 1);
const IPAddress subnet(255, 255, 255, 0);

const int recv_port = 6667;

String translateEncryptionType(wifi_auth_mode_t encryptionType) {
    switch (encryptionType) {
        case (WIFI_AUTH_OPEN):
            return "Open";
        case (WIFI_AUTH_WEP):
            return "WEP";
        case (WIFI_AUTH_WPA_PSK):
            return "WPA_PSK";
        case (WIFI_AUTH_WPA2_PSK):
            return "WPA2_PSK";
        case (WIFI_AUTH_WPA_WPA2_PSK):
            return "WPA_WPA2_PSK";
        case (WIFI_AUTH_WPA2_ENTERPRISE):
            return "WPA2_ENTERPRISE";
		default:
			return "Unknown";
    }
}


void scanNetworks() {
    int numberOfNetworks = WiFi.scanNetworks();

    M5.Lcd.print("Number of networks found: ");
    M5.Lcd.println(numberOfNetworks);

    // for (int i = 0; i < numberOfNetworks; i++) {
    //     M5.Lcd.print("Network name: ");
    //     M5.Lcd.println(WiFi.SSID(i));

    //     M5.Lcd.print("Signal strength: ");
    //     M5.Lcd.println(WiFi.RSSI(i));

    //     M5.Lcd.print("MAC address: ");
    //     M5.Lcd.println(WiFi.BSSIDstr(i));

    //     M5.Lcd.print("Encryption type: ");
    //     String encryptionTypeDescription =
    //         translateEncryptionType(WiFi.encryptionType(i));
    //     M5.Lcd.println(encryptionTypeDescription);
    //     M5.Lcd.println("-----------------------");
    // }
}

// the setup routine runs once when M5Stack starts up
void setup() {
	// Initialize the M5Stack object
	M5.begin();

	/*
	  Power chip connected to gpio21, gpio22, I2C device
	  Set battery charging voltage and current
	  If used battery, please call this function in your project
	*/
	M5.Power.begin();

	WiFi.disconnect(true, true);
	delay(1000);
	scanNetworks();
	delay(1000);
	M5.Lcd.clear();

	M5.Lcd.setTextSize(2);
	M5.Lcd.print("\nconenct to router");
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	WiFi.config(ip, gateway, subnet);

	// M5.Lcd.setTextSize(2);
	// M5.Lcd.print("\nconenct to internet");
	while (WiFi.status() != WL_CONNECTED) { M5.Lcd.print("."); delay(1000); }
    M5.Lcd.print("\nWiFi connected, \nIP = "); M5.Lcd.println(WiFi.localIP());

	delay(1000);
	// M5.Lcd.clear();
	// LCD display
	// M5.Lcd.setTextSize(1);  // 53
	// M5.Lcd.println(
	// 	"012345678901234567890123456789012345678901234567890123456789");
	// M5.Lcd.setTextSize(2);  // 26 하고 반글자
	// M5.Lcd.println("01234567890123456789012345678901234567890123456789");
	// M5.Lcd.setTextSize(3);  // 17 하고 거의 한글자
	// M5.Lcd.println("01234567890123456789012345678901234567890123456789");

	OscWiFi.subscribe(recv_port, "/ID/10", [](const OscMessage &m) {
		M5.Lcd.clear();
		M5.Lcd.setTextSize(2);
		M5.Lcd.setCursor(0, 0);
		M5.Lcd.setTextColor(BLUE);
		M5.Lcd.print(m.remoteIP());
		M5.Lcd.print(":");
		M5.Lcd.println(m.remotePort());
		M5.Lcd.setTextSize(3);
		M5.Lcd.setTextColor(WHITE);
		M5.Lcd.println();
		M5.Lcd.println(m.arg<int>(0));
		M5.Lcd.setTextColor(YELLOW);
		M5.Lcd.println(m.arg<float>(1));
	});
}

// the loop routine runs over and over again forever
void loop() {
	OscWiFi.update();
}