const { WebcastPushConnection } = require('tiktok-live-connector');
const express = require('express');

const app = express();
app.use(express.json());

const PORT = 3000;
const TIKTOK_USERNAME = process.env.TIKTOK_USERNAME || 'adrianamimualice';

// Fila de eventos consumida pelo OpenMP (GET /events).
let eventQueue = [];

const tiktokConnection = new WebcastPushConnection(TIKTOK_USERNAME);

function queueEvent(event) {
    eventQueue.push(event);

    // Proteção simples para não crescer sem limite se o servidor SA:MP estiver offline.
    if (eventQueue.length > 500) {
        eventQueue = eventQueue.slice(-500);
    }
}

function normalizeUser(data) {
    return data?.uniqueId || data?.nickname || 'unknown_user';
}

async function connectTikTok() {
    try {
        await tiktokConnection.connect();
        console.log(`[TIKTOK] Conectado à live de @${TIKTOK_USERNAME}`);
    } catch (err) {
        console.error('[TIKTOK] Erro ao conectar:', err?.message || err);
        console.log('[TIKTOK] Tentando reconectar em 5 segundos...');
        setTimeout(connectTikTok, 5000);
    }
}

// CHAT
tiktokConnection.on('chat', (data) => {
    const event = {
        type: 'chat',
        user: normalizeUser(data),
        message: data?.comment || ''
    };

    if (!event.message) {
        return;
    }

    console.log(`[CHAT] ${event.user}: ${event.message}`);
    queueEvent(event);
});

// PRESENTES (GIFTS)
tiktokConnection.on('gift', (data) => {
    const event = {
        type: 'gift',
        user: normalizeUser(data),
        giftId: data?.giftId || 0,
        giftName: data?.giftName || 'UnknownGift',
        repeatCount: data?.repeatCount || 1
    };

    console.log(`[GIFT] ${event.user} enviou ${event.repeatCount}x ${event.giftName}`);
    queueEvent(event);
});

tiktokConnection.on('disconnected', () => {
    console.warn('[TIKTOK] Conexão encerrada. Reconectando em 5 segundos...');
    setTimeout(connectTikTok, 5000);
});

// OpenMP chama isso para buscar e limpar os eventos pendentes.
app.get('/events', (_, res) => {
    res.json(eventQueue);
    eventQueue = [];
});

app.get('/', (_, res) => {
    res.send('TikTok API Online');
});

app.listen(PORT, () => {
    console.log(`[API] Rodando em http://127.0.0.1:${PORT}`);
    connectTikTok();
});
