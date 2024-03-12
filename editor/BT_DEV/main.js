const { app, BrowserWindow } = require('electron');
//const { checkArduinoConnection } = require('./src/js/usbDetection')



let mainWindow;

function createWindow() {
    // Inicia la verificación de la conexión
    // checkArduinoConnection();
    mainWindow = new BrowserWindow({
        width: 1024,
        height: 800,
        resizable: false, // Hacer que la ventana no sea redimensionable
        webPreferences: {
            nodeIntegration: true
        }
    });

    process.env.ELECTRON_DISABLE_CACHE = 'true';

    mainWindow.loadFile('index.html');

    // Escuchar el evento resize y mantener el tamaño fijo
    mainWindow.on('resize', () => {
        mainWindow.setSize(400, 300); // Mantener el tamaño fijo
    });

    mainWindow.on('closed', function () {
        mainWindow = null;
    });
}





app.whenReady().then(createWindow);
app.commandLine.appendSwitch('disable-http-cache');
app.on('window-all-closed', function () {
    if (process.platform !== 'darwin') app.quit();
});

app.on('activate', function () {
    if (mainWindow === null) createWindow();
});
