| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C5 | ESP32-C6 | ESP32-C61 | ESP32-H2 | ESP32-H21 | ESP32-H4 | ESP32-P4 | ESP32-S2 | ESP32-S3 | Linux |
| ----------------- | ----- | -------- | -------- | -------- | -------- | --------- | -------- | --------- | -------- | -------- | -------- | -------- | ----- |
# 📡 ESP32-S3 – ADC, GPIO, WS2812, WiFi Project

Acest proiect este dezvoltat pe platforma **ESP-IDF v6.0.0**, folosind placa **ESP32-S3-SIM7670G-4G**. Proiectul are următoarele funcționalități:

- 📶 Conectare WiFi (mode STA)
- 🔘 Citire analogică de la un potențiometru prin **ADC1 Channel 4** (GPIO 5)
- 👆 Debounce pentru buton (GPIO 4)
- 💡 Aprindere LED (GPIO 46) la apăsarea butonului
- 📊 Afișare tensiune măsurată în terminal (cu calibrare ADC, dacă eFuse permite)
- 🌈 Suport WS2812 (LED RGB adresabil)
- ✅ Arhitectură modulară (fiecare componentă în fișiere separate `.c/.h`)

## 🔧 Structura proiectului

```
ESP32_Project/
│
├── main/
│   ├── main.c                    # Logica principală
│   ├── gpio_config.c/.h         # Configurare GPIO (buton, LED)
│   ├── adc_config.c/.h          # Configurare și citire ADC1
│   ├── wifi_connect.c/.h        # Conectare WiFi (mode STA)
│   ├── ws2812_control.c/.h      # Control LED WS2812 (dacă e inclus)
│
├── CMakeLists.txt
├── sdkconfig
└── README.md                    # [acest fișier]
```

## 🚀 Funcționalități principale

### 🔘 Apăsare buton cu debounce

```c
if (gpio_get_level(BUTTON_GPIO) == 0) {
    vTaskDelay(pdMS_TO_TICKS(50)); // debounce
    if (gpio_get_level(BUTTON_GPIO) == 0) {
        int voltage = read_adc_voltage(adc_handle, channel);
        printf("Tensiune ADC: %d mV\n", voltage);
        gpio_set_level(LED_GPIO, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_set_level(LED_GPIO, 0);
    }
}
```

### 📊 ADC1 (citire tensiune)

- ADC1 Channel 4 (GPIO 5)
- Calibrare automată (curve fitting → fallback la line fitting)
- Conversie manuală dacă nu există eFuse

```c
float voltage = (raw / 4095.0) * 3300;
```

### 📶 WiFi (mode STA)

- Conectare automată la:

```c
#define SSID "myssid"
#define PASSWORD "mypassword"
```

### 🌈 WS2812 (opțional)

- Control WS2812 (LED RGB) cu extensie `led_strip.h` – pregătit pentru implementare.

## 🛠️ Cum rulezi proiectul

```bash
idf.py set-target esp32s3
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

> Înlocuiește portul serial după sistemul tău.

## ⚙️ Setări ADC

| Parametru        | Valoare           |
|------------------|-------------------|
| Canal            | ADC1 Channel 4    |
| GPIO             | 5                 |
| Atenuare         | `ADC_ATTEN_DB_12` |
| Rezoluție        | 12 biți (0 – 4095)|
| Tensiune max     | ~3.6 V            |
| Vref implicit    | 3.3 V             |

## 📂 Extensii viitoare

- Trimitere date prin HTTP/MQTT
- Dashboard Flask local
- Afișare date pe WS2812
- Praguri și alerte

## 📜 Licență

Proiect educațional ESP32 sub licență Apache 2.0.

## ✍️ Autor

**Florian David**  
IoT / Embedded Systems Developer  
Platformă: macOS + ESP32-S3 + ESP-IDF 6.0.0
