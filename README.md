# _Sample project_

# ADC1 + WiFi + Buton

Citește valoarea unui potențiometru conectat pe ADC1 Channel 4 (GPIO5), afișează valoarea doar când se apasă butonul, și aprinde un LED ca feedback.

## Fișiere
- `adc_config.c/.h` – inițializarea ADC
- `gpio_config.c/.h` – inițializare LED și buton
- `main.c` – logica principală

## Exemplu de utilizare
```c
configure_BUTTON_input(BUTTON_GPIO);
configure_LED_output(LED_GPIO);
configure_adc(&adc_handle, ADC_CHANNEL_4);

## Example folder contents

The project **sample_project** contains one source file in C language [main.c](main/main.c). The file is located in folder [main](main).

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both). 

Below is short explanation of remaining files in the project folder.

```
├── CMakeLists.txt
├── main
│   ├── CMakeLists.txt
│   └── main.c
└── README.md                  This is the file you are currently reading
```
Additionally, the sample project contains Makefile and component.mk files, used for the legacy Make based build system. 
They are not used or needed when building with CMake and idf.py.
