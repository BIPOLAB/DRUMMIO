const usb = require('usb');
const path = require('path');
const { BrowserWindow } = require('electron');

let popupWindow;

function createPopup() {
    popupWindow = new BrowserWindow({
        width: 400,
        height: 200,
        resizable: false,
        alwaysOnTop: true,
        frame: false,
        webPreferences: {
            nodeIntegration: true
        }
    });

    //popupWindow.loadURL('/src/html/usbpopup.html');
    const popupPath = path.join(__dirname, '../html/usbpopup.html');
    popupWindow.loadURL(`file://${popupPath}`);
}

function checkArduinoConnection() {
    // VID y PID para Arduino Leonardo
    const leonardoVendorId = 0x2341;
    const leonardoProductId = 0x8036;

    const devices = usb.getDeviceList();

    const isArduinoConnected = devices.some(device =>
        device.deviceDescriptor.idVendor === leonardoVendorId &&
        device.deviceDescriptor.idProduct === leonardoProductId
    );

    if (isArduinoConnected) {
        if (popupWindow) {
            popupWindow.close();
            popupWindow = null;
        }
    } else {
        if (!popupWindow) {
            createPopup();
        }
    }
}

// Verifica la conexión cada 2 segundos
setInterval(checkArduinoConnection, 2000);

module.exports = { checkArduinoConnection };
