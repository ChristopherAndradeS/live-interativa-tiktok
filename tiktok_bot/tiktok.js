const { WebcastPushConnection } = require('tiktok-live-connector');
const express = require('express');

const app = express();
app.use(express.json());

const PORT = 3000;

// FILA DE EVENTOS (tipo buffer)
let eventQueue = [];

// CONFIG
let tiktokUsername = "adrianamimualice";
let tiktokConnection = new WebcastPushConnection(tiktokUsername);

// Conectar
tiktokConnection.connect()
.then(() => {
    console.log(`Conectado ao TikTok como ${tiktokUsername}`);
})
.catch(err => {
    console.error('Erro ao conectar:', err);
});

// =========================
// EVENTOS DO TIKTOK
// =========================

// CHAT
tiktokConnection.on('chat', data => {
    const event = {
        type: "chat",
        user: data.uniqueId,
        message: data.comment
    };

    console.log(`${data.uniqueId} mandou ${data.comment}`);

    eventQueue.push(event);


});

// PRESENTES (GIFTS)
tiktokConnection.on('gift', data => {
    const event = {
        type: "gift",
        user: data.uniqueId,
        giftId: data.giftId,
        giftName: data.giftName,
        repeatCount: data.repeatCount
    };

    eventQueue.push(event);
});

// =========================
// API ENDPOINT
// =========================

// Pawn vai chamar isso
app.get('/events', (req, res) => {
    res.json(eventQueue);
    
    // Limpa fila após envio
    eventQueue = [];
});

// Health check
app.get('/', (req, res) => {
    res.send("TikTok API Online");
});

// Start server
app.listen(PORT, () => {
    console.log(`API rodando em http://localhost:${PORT}`);
});