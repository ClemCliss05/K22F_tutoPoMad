#!/usr/bin/env bash

set -euo pipefail

SERIAL_DEVICE="/dev/serial/by-id/usb-SEGGER_J-Link_000621000000-if00"
BAUDRATE=115200

if [[ ! -e "$SERIAL_DEVICE" ]]; then
    echo "ERROR: J-Link serial device not found."
    echo
    echo "Available serial devices:"
    ls -l /dev/serial/by-id/ 2>/dev/null || true
    exit 1
fi

echo "Opening UART:"
echo "  Device : $SERIAL_DEVICE"
echo "  Baud   : $BAUDRATE"
echo

exec picocom -b "$BAUDRATE" "$SERIAL_DEVICE"