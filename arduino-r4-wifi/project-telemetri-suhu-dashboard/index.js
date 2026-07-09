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

// --- CHART.JS SETUP ---
const ctx = document.getElementById('tempChart').getContext('2d');

// Konfigurasi gradien untuk area di bawah kurva grafik
let gradient = ctx.createLinearGradient(0, 0, 0, 400);
gradient.addColorStop(0, 'rgba(14, 165, 233, 0.5)'); // warna cyan semi-transparan
gradient.addColorStop(1, 'rgba(14, 165, 233, 0.0)');

const chartConfig = {
    type: 'line',
    data: {
        labels: [], // Waktu data masuk (timestamp)
        datasets: [{
            label: 'Suhu Ruangan (°C)',
            data: [], // Nilai suhu
            borderColor: '#0ea5e9', // Warna garis utama
            backgroundColor: gradient,
            borderWidth: 3,
            pointBackgroundColor: '#38bdf8',
            pointBorderColor: '#fff',
            pointHoverBackgroundColor: '#fff',
            pointHoverBorderColor: '#38bdf8',
            pointRadius: 4,
            pointHoverRadius: 6,
            fill: true, // Mengisi area di bawah grafik
            tension: 0.4 // Membuat garis melengkung (bezier curve) halus
        }]
    },
    options: {
        responsive: true,
        maintainAspectRatio: false,
        plugins: {
            legend: {
                labels: {
                    color: '#94a3b8',
                    font: { family: "'Outfit', sans-serif" }
                }
            },
            tooltip: {
                backgroundColor: 'rgba(15, 23, 42, 0.9)',
                titleFont: { family: "'Outfit', sans-serif" },
                bodyFont: { family: "'Outfit', sans-serif" },
                padding: 12,
                cornerRadius: 8,
                displayColors: false
            }
        },
        scales: {
            y: {
                min: 20,
                max: 40,
                grid: {
                    color: 'rgba(255, 255, 255, 0.05)',
                    drawBorder: false
                },
                ticks: {
                    color: '#94a3b8',
                    font: { family: "'Outfit', sans-serif" },
                    stepSize: 2
                }
            },
            x: {
                grid: {
                    display: false,
                    drawBorder: false
                },
                ticks: {
                    color: '#94a3b8',
                    font: { family: "'Outfit', sans-serif" },
                    maxTicksLimit: 8
                }
            }
        },
        animation: {
            duration: 400 // Animasi transisi yang lebih cepat saat data baru masuk
        }
    }
};

const tempChart = new Chart(ctx, chartConfig);
const MAX_DATA_POINTS = 20; // Batas maksimal titik yang ditampilkan di grafik

// Fungsi untuk menambah data ke chart
function addDataToChart(temperature) {
    const now = new Date();
    // Format waktu HH:MM:SS
    const timeString = now.toLocaleTimeString('id-ID', { hour12: false });
    
    // Tambahkan label dan data baru
    tempChart.data.labels.push(timeString);
    tempChart.data.datasets[0].data.push(temperature);
    
    // Jika jumlah data melebihi batas maksimal, hapus data paling lama (di awal array)
    if (tempChart.data.labels.length > MAX_DATA_POINTS) {
        tempChart.data.labels.shift();
        tempChart.data.datasets[0].data.shift();
    }
    
    // Perbarui chart
    tempChart.update();
}

// --- BLUETOOTH LOGIC ---

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
        
        if (error.name === 'NotFoundError') {
            updateStatus('Dibatalkan pengguna', 'disconnected');
        }
    }
}

function handleDataUpdate(event) {
    const value = event.target.value;
    
    // Kita membaca 32-bit integer sesuai kode Arduino (BLEIntCharacteristic)
    const temperature = value.getInt32(0, true);
    
    // 1. Update Tampilan Angka Digital
    tempValue.textContent = temperature;
    
    // Trigger the CSS animation
    tempValue.classList.remove('update-anim');
    void tempValue.offsetWidth; // Force a reflow
    tempValue.classList.add('update-anim');

    // 2. Update Tampilan Grafik (Chart.js)
    addDataToChart(temperature);
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
