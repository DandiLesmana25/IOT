// DOM Elements
const ipInput = document.getElementById('ip-input');
const btnConnect = document.getElementById('btn-connect');
const statusText = document.getElementById('status-text');
const connectionDot = document.getElementById('connection-dot');

const btnOn = document.getElementById('btn-on');
const btnOff = document.getElementById('btn-off');
const ledIndicator = document.getElementById('led-indicator');
const ledText = document.getElementById('led-text');
const sensorValue = document.getElementById('sensor-value');

let arduinoIP = '';
let pollingInterval = null;

// Coba muat IP terakhir dari LocalStorage (kenyamanan UX)
const savedIP = localStorage.getItem('arduino_last_ip');
if (savedIP) {
    ipInput.value = savedIP;
}

// Fungsi Update Tampilan Koneksi
function setConnectionStatus(isConnected) {
    if (isConnected) {
        statusText.textContent = "Terkoneksi ke API";
        connectionDot.className = "dot connected";
        btnConnect.textContent = "Putuskan";
        btnConnect.style.backgroundColor = "var(--danger)";
        btnConnect.style.boxShadow = "0 4px 15px var(--danger-glow)";
        btnOn.disabled = false;
        btnOff.disabled = false;
    } else {
        statusText.textContent = "Terputus";
        connectionDot.className = "dot disconnected";
        btnConnect.textContent = "Hubungkan";
        btnConnect.style.backgroundColor = "var(--primary)";
        btnConnect.style.boxShadow = "0 4px 15px var(--primary-glow)";
        btnOn.disabled = true;
        btnOff.disabled = true;
        
        // Matikan interval polling
        if (pollingInterval) {
            clearInterval(pollingInterval);
            pollingInterval = null;
        }
    }
}

// Menangani Klik Tombol Hubungkan
btnConnect.addEventListener('click', async () => {
    // Jika sedang terkoneksi, maka putuskan
    if (pollingInterval !== null) {
        setConnectionStatus(false);
        return;
    }

    const ip = ipInput.value.trim();
    if (!ip) {
        alert("Silakan masukkan Alamat IP Arduino terlebih dahulu!");
        return;
    }

    arduinoIP = `http://${ip}`;
    statusText.textContent = "Menghubungi...";

    try {
        // Melakukan Fetch ke Endpoint /api/status untuk tes koneksi
        // (Pakai mode 'cors' dan timeout buatan menggunakan Promise.race)
        const controller = new AbortController();
        const timeoutId = setTimeout(() => controller.abort(), 3000); // 3 detik timeout

        const response = await fetch(`${arduinoIP}/api/status`, {
            method: 'GET',
            signal: controller.signal
        });
        
        clearTimeout(timeoutId);

        if (response.ok) {
            const data = await response.json();
            
            // Simpan IP untuk sesi berikutnya
            localStorage.setItem('arduino_last_ip', ip);
            
            setConnectionStatus(true);
            updateUIWithData(data);

            // Mulai Auto-Polling (AJAX Call berulang) setiap 2 detik
            startPolling();
        } else {
            throw new Error(`HTTP Error: ${response.status}`);
        }
    } catch (error) {
        console.error("Fetch Error:", error);
        setConnectionStatus(false);
        statusText.textContent = "Gagal (Pastikan IP benar & 1 jaringan)";
        alert("Gagal terhubung ke Arduino. Pastikan:\n1. Laptop/HP dan Arduino ada di WiFi yang sama.\n2. Alamat IP benar.\n3. Arduino sudah di-upload kode terbaru.");
    }
});

// Fungsi untuk menarik data berkala (Polling)
function startPolling() {
    pollingInterval = setInterval(async () => {
        try {
            const response = await fetch(`${arduinoIP}/api/status`);
            if (response.ok) {
                const data = await response.json();
                updateUIWithData(data);
            }
        } catch (error) {
            console.warn("Polling terganggu, jaringan mungkin tidak stabil.");
            // Opsi: kita bisa setConnectionStatus(false) di sini jika gagal 3x beruntun
        }
    }, 2000); // 2000 ms = 2 detik
}

// Fungsi Memperbarui Layar dari Data JSON
function updateUIWithData(data) {
    // Update Lampu LED
    if (data.led === true) {
        ledIndicator.className = "led-dot on";
        ledText.textContent = "MENYALA";
        ledText.style.color = "var(--success)";
    } else {
        ledIndicator.className = "led-dot off";
        ledText.textContent = "MATI";
        ledText.style.color = "var(--text-main)";
    }

    // Update Angka Sensor
    if (sensorValue.textContent !== data.sensor.toString()) {
        sensorValue.textContent = data.sensor;
        
        // Animasi agar terlihat data baru masuk
        sensorValue.classList.remove('pop-anim');
        void sensorValue.offsetWidth;
        sensorValue.classList.add('pop-anim');
    }
}

// Fungsi Kontrol Lampu (Mengirim API Request)
async function sendCommand(action) {
    try {
        const response = await fetch(`${arduinoIP}/api/led/${action}`);
        if (response.ok) {
            const data = await response.json();
            updateUIWithData(data);
        }
    } catch (error) {
        console.error("Gagal mengirim perintah:", error);
    }
}

btnOn.addEventListener('click', () => sendCommand('on'));
btnOff.addEventListener('click', () => sendCommand('off'));
