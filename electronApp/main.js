const electron = require('electron');
const app = electron.app;
const BrowserWindow = electron.BrowserWindow;
const path = require('path')
const url = require('url')
const bindings = require("bindings")
const whoami = bindings("module")
const ipc = electron.ipcMain;

let mainWindow;

function createWindow () {

  mainWindow = new BrowserWindow({width: 900, height: 800});
  // Open the DevTools.
  mainWindow.webContents.openDevTools();
//  mainWindow.loadURL(`file://${__dirname}/index.html`);

  mainWindow.loadURL(url.format({
        pathname: path.join(__dirname, 'index.html'),
        protocol: 'file:',
        slashes: true
      }))

  mainWindow.on('closed', () => {
    mainWindow = null;
  })
}

app.on('ready', createWindow);

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app.on('activate', () => {
  if (mainWindow === null) {
    createWindow();
  }
});
console.log( whoami.Increment(1) )
//console.log( "init array : " + whoami.initArray(10,10))
ipc.on('bodyClick', () => {
    console.log( whoami.Increment(1) )
});
console.log( "count array : " + whoami.GetArrayLength())
