// UUIDs must match the Arduino sketch exactly
const BLE_SERVICE_UUID = '19b10000-e8f2-537e-4f6c-d104768a1214';
const BLE_CHARACTERISTIC_UUID = '19b10002-e8f2-537e-4f6c-d104768a1214';

// DOM Elements
const connectBtn = document.getElementById('connect-btn');
const tempValue = document.getElementById('temp-value');
const statusText = document.getElementById('status-text');
const connectionDot = document.getElementById('connection-dot');

let bluetoothDevice;
let sensorCharacteristic;

// Event Listener for the connect button
connectBtn.addEventListener('click', async () => {
    if (bluetoothDevice && bluetoothDevice.gatt.connected) {
        disconnectDevice();
    } else {
        await connectToDevice();
    }
});

async function connectToDevice() {
    try {
        updateStatus('Meminta izin Bluetooth...', 'disconnected');

        // Request the Bluetooth device through browser UI
        bluetoothDevice = await navigator.bluetooth.requestDevice({
            filters: [{ name: 'Monitor Suhu' }],
            optionalServices: [BLE_SERVICE_UUID]
        });

        // Set up event listener for when device gets disconnected
        bluetoothDevice.addEventListener('gattserverdisconnected', onDisconnected);

        updateStatus('Menghubungkan ke Arduino...', 'disconnected');

        // Connect to the GATT server
        const server = await bluetoothDevice.gatt.connect();

        // Get the specific service
        const service = await server.getPrimaryService(BLE_SERVICE_UUID);

        // Get the specific characteristic
        sensorCharacteristic = await service.getCharacteristic(BLE_CHARACTERISTIC_UUID);

        // Start receiving notifications
        await sensorCharacteristic.startNotifications();

        // Listen for characteristic value changes (telemetry data)
        sensorCharacteristic.addEventListener('characteristicvaluechanged', handleDataUpdate);

        updateStatus('Terkoneksi', 'connected');
        connectBtn.textContent = 'Putuskan Koneksi';

    } catch (error) {
        console.error('Error:', error);
        updateStatus('Gagal terhubung', 'disconnected');

        // Reset state if user cancelled or error occurred
        if (error.name === 'NotFoundError') {
            updateStatus('Dibatalkan oleh pengguna', 'disconnected');
        }
    }
}

function handleDataUpdate(event) {
    const value = event.target.value;

    // In Arduino, BLEIntCharacteristic sends a 32-bit integer.
    // We read it as a 32-bit integer (little endian).
    const temperature = value.getInt32(0, true);

    // Update the UI
    tempValue.textContent = temperature;

    // Trigger the CSS animation
    tempValue.classList.remove('update-anim');

    // Force a reflow to restart the animation
    void tempValue.offsetWidth;

    tempValue.classList.add('update-anim');
}

function disconnectDevice() {
    if (bluetoothDevice && bluetoothDevice.gatt.connected) {
        bluetoothDevice.gatt.disconnect();
    }
}

function onDisconnected() {
    updateStatus('Terputus', 'disconnected');
    connectBtn.textContent = 'Hubungkan ke Arduino';
    tempValue.textContent = '--';
    tempValue.classList.remove('update-anim');
}

function updateStatus(text, stateClass) {
    statusText.textContent = text;
    connectionDot.className = `dot ${stateClass}`;
}
