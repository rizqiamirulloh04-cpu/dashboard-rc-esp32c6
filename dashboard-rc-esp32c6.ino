name: Build ESP32-C6 Firmware

on:
  push:
    branches: [ main ]
  workflow_dispatch:

jobs:
  build:
    runs-on: ubuntu-latest

    steps:
      - name: Checkout repository
        uses: actions/checkout@v4

      - name: Install Arduino CLI
        uses: arduino/setup-arduino-cli@v2

      - name: Update index
        run: arduino-cli core update-index

      - name: Install ESP32 core
        run: |
          arduino-cli core install esp32:esp32

      - name: Install libraries
        run: |
          arduino-cli lib install "Arduino_GFX_Library"

      - name: Compile firmware
        run: |
          mkdir -p build
          arduino-cli compile \
            --fqbn esp32:esp32:esp32c6 \
            --output-dir build \
            dashboard-rc-esp32c6.ino

      - name: Upload artifacts
        uses: actions/upload-artifact@v4
        with:
          name: firmware
          path: build/*