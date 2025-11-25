# dimmer_digital_firmware
Firmware to a dimmer digital using AVR Atmega328p 
 hadware: https://github.com/NestorDP/dimmer_digital_hardware


 # Documentation (Doxygen)

This project includes a minimal Doxygen configuration so you can generate HTML documentation from the sources.

How to generate the docs

1. Install Doxygen (on Debian/Ubuntu):

```
sudo apt update
sudo apt install doxygen
```

2. From the project root run:

```
doxygen Doxyfile
```

3. Open the generated HTML in a browser:

```
xdg-open docs/doxygen/html/index.html
```

Notes and tips

- If you want more detailed output (function parameters, private members), adjust `Doxyfile` (e.g. `EXTRACT_PRIVATE = YES`).
- Consider adding `\brief` and `\param`/`\return` style comments to your headers to improve the generated output.
