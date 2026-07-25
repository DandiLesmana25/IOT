// UUIDs for RFID Scanner Project
const BLE_SERVICE_UUID = '20b10000-e8f2-537e-4f6c-d104768a1214';
const BLE_CHARACTERISTIC_UUID = '20b10002-e8f2-537e-4f6c-d104768a1214';

// DOM Elements
const connectBtn = document.getElementById('connect-btn');
const clearBtn = document.getElementById('clear-btn');
const lastUidText = document.getElementById('last-uid');
const statusText = document.getElementById('status-text');
const connectionDot = document.getElementById('connection-dot');
const tableBody = document.getElementById('table-body');

let bluetoothDevice;
let rfidCharacteristic;

// --- LOCAL STORAGE LOGIC ---
// Kunci penyimpanan di browser
const STORAGE_KEY = 'rfid_history_data';

// Ambil data yang tersimpan, atau buat array kosong jika belum ada
let rfidHistory = JSON.parse(localStorage.getItem(STORAGE_KEY)) || [];

// Render tabel saat pertama kali halaman dimuat
renderTable();

function saveToLocalStorage(uid) {
    const now = new Date();
    // Format tanggal dan waktu
    const timeString = now.toLocaleDateString('id-ID') + ' ' + now.toLocaleTimeString('id-ID');

    // Tambahkan data baru ke paling depan array (unshift)
    rfidHistory.unshift({
        uid: uid,
        timestamp: timeString
    });

    // Simpan array yang diperbarui ke localStorage (harus diubah jadi string JSON)
    localStorage.setItem(STORAGE_KEY, JSON.stringify(rfidHistory));

    // Update tampilan tabel
    renderTable();
}

function clearLocalStorage() {
    if (confirm("Apakah Anda yakin ingin menghapus semua riwayat tap kartu?")) {
        rfidHistory = [];
        localStorage.removeItem(STORAGE_KEY);
        renderTable();
    }
}

function renderTable() {
    tableBody.innerHTML = ''; // Kosongkan tabel saat ini

    if (rfidHistory.length === 0) {
        tableBody.innerHTML = `<tr><td colspan="4" class="empty-state">Belum ada riwayat tap kartu.</td></tr>`;
        return;
    }

    // Loop data dan buat elemen baris tabel
    rfidHistory.forEach((item, index) => {
        const row = document.createElement('tr');
        row.innerHTML = `
            <td>${index + 1}</td>
            <td style="font-family: monospace; font-weight: bold; letter-spacing: 1px;">${item.uid}</td>
            <td>${item.timestamp}</td>
            <td><span class="badge-success">Masuk</span></td>
        `;
        tableBody.appendChild(row);
    });
}

clearBtn.addEventListener('click', clearLocalStorage);


// --- BLUETOOTH LOGIC ---

connectBtn.addEventListener('click', async () => {
    if (bluetoothDevice && bluetoothDevice.gatt.connected) {
        disconnectDevice();
    } else {
        await connectToDevice();
    }
});

async function connectToDevice() {
    try {
        updateStatus('Mencari Scanner...', 'disconnected');

        bluetoothDevice = await navigator.bluetooth.requestDevice({
            filters: [{ name: 'RFID Scanner' }],
            optionalServices: [BLE_SERVICE_UUID]
        });

        bluetoothDevice.addEventListener('gattserverdisconnected', onDisconnected);
        updateStatus('Menghubungkan...', 'disconnected');

        const server = await bluetoothDevice.gatt.connect();
        const service = await server.getPrimaryService(BLE_SERVICE_UUID);

        // Dapatkan characteristic string
        rfidCharacteristic = await service.getCharacteristic(BLE_CHARACTERISTIC_UUID);

        // Mulai listen notifikasi
        await rfidCharacteristic.startNotifications();
        rfidCharacteristic.addEventListener('characteristicvaluechanged', handleRfidTap);

        updateStatus('Terkoneksi', 'connected');
        connectBtn.textContent = 'Putuskan Koneksi';

    } catch (error) {
        console.error('BLE Error:', error);
        updateStatus('Gagal terhubung', 'disconnected');
        if (error.name === 'NotFoundError') {
            updateStatus('Dibatalkan pengguna', 'disconnected');
        }
    }
}

function handleRfidTap(event) {
    const value = event.target.value;

    // Karena kita mengirim String dari Arduino, kita ekstrak pakai TextDecoder
    const decoder = new TextDecoder('utf-8');
    const uidString = decoder.decode(value);

    // Abaikan jika isinya "READY" (Nilai inisialisasi di setup() Arduino)
    if (uidString === "READY") return;

    // 1. Update Kartu Terakhir di Sidebar
    lastUidText.textContent = uidString;

    // Animasi Pop CSS
    lastUidText.classList.remove('pop-anim');
    void lastUidText.offsetWidth; // Force a reflow
    lastUidText.classList.add('pop-anim');

    // 2. Simpan ke LocalStorage dan perbarui tabel riwayat
    saveToLocalStorage(uidString);
}

function disconnectDevice() {
    if (bluetoothDevice && bluetoothDevice.gatt.connected) {
        bluetoothDevice.gatt.disconnect();
    }
}

function onDisconnected() {
    updateStatus('Terputus', 'disconnected');
    connectBtn.textContent = 'Hubungkan ke Scanner';
    lastUidText.textContent = '-- -- -- --';
    lastUidText.classList.remove('pop-anim');
}

function updateStatus(text, stateClass) {
    statusText.textContent = text;
    connectionDot.className = `dot ${stateClass}`;
}
