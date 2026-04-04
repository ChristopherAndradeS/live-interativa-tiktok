const { WebcastPushConnection } = require('tiktok-live-connector');
const express = require('express');

const app = express();
app.use(express.json()); 

const PORT = 3000;
const TIKTOK_USERNAME = process.env.TIKTOK_USERNAME || 'freefirebr_oficial';
const MAX_QUEUE_SIZE = 500;

// Fila de eventos consumida pelo OpenMP (GET /events).
let eventQueue = [];

const tiktokConnection = new WebcastPushConnection(TIKTOK_USERNAME);

function removeAccents(value = '') {
    return String(value)
        .normalize('NFD')
        .replace(/[\u0300-\u036f]/g, '');
}

function sanitizeForPawn(value = '', maxLength = 64) {
    const normalized = removeAccents(value)
        .replace(/[^\x20-\x7E]/g, '')
        .replace(/["\\]/g, '')
        .trim();

    return normalized.slice(0, Math.max(0, maxLength));
}

function toInt(value, fallback = 0) {
    const parsed = Number.parseInt(value, 10);
    return Number.isFinite(parsed) ? parsed : fallback;
}

function queueEvent(type, data = {}) {
    eventQueue.push({
        type,
        data,
        timestamp: Date.now()
    });

    // Proteção simples para não crescer sem limite se o servidor OpenMP estiver offline.
    if (eventQueue.length > MAX_QUEUE_SIZE) {
        eventQueue = eventQueue.slice(-MAX_QUEUE_SIZE);
    }
}

function normalizeIdentity(data = {}) {
    const uniqueId = sanitizeForPawn(data?.uniqueId || data?.nickname || 'unknown_user', 64);
    const nickname = sanitizeForPawn(data?.nickname || data?.uniqueId || 'unknown_user', 64);
    const userId = sanitizeForPawn(data?.userId || '', 32);

    return { uniqueId, nickname, userId };
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
    const identity = normalizeIdentity(data);
    const comment = sanitizeForPawn(data?.comment || '', 144);

    if (!comment) {
        return;
    }

    queueEvent('chat', {
        uniqueId: identity.uniqueId,
        nickname: identity.nickname,
        userId: identity.userId,
        comment,
        isModerator: Boolean(data?.isModerator),
        isNewGifter: Boolean(data?.isNewGifter),
        isSubscriber: Boolean(data?.isSubscriber)
    });

    console.log(`[CHAT] ${identity.nickname}: ${comment}`);
});

// PRESENTES (GIFTS)
tiktokConnection.on('gift', (data) => {
    const identity = normalizeIdentity(data);
    const repeatCount = Math.max(1, toInt(data?.repeatCount, 1));
    const diamondCount = Math.max(0, toInt(data?.diamondCount, 0));
    const giftName = sanitizeForPawn(data?.giftName || 'UnknownGift', 64);
    const repeatEnd = Boolean(data?.repeatEnd);

    const payload = {
        uniqueId: identity.uniqueId,
        nickname: identity.nickname,
        giftName,
        repeatCount,
        diamondCount
    };

    if (repeatEnd) {
        queueEvent('giftEnd', payload);
        console.log(`[GIFT-END] ${identity.nickname} finalizou combo ${repeatCount}x ${giftName}`);
    } else {
        queueEvent('gift', payload);
        console.log(`[GIFT] ${identity.nickname} enviou ${repeatCount}x ${giftName}`);
    }
});

// LIKES
tiktokConnection.on('like', (data) => {
    const identity = normalizeIdentity(data);

    queueEvent('like', {
        uniqueId: identity.uniqueId,
        nickname: identity.nickname,
        likeCount: Math.max(0, toInt(data?.likeCount, 0)),
        totalLikeCount: Math.max(0, toInt(data?.totalLikeCount, 0))
    });

    console.log(`[LIKE] ${identity.nickname} deu like : ${data?.likeCount}`);
});

// FOLLOW
tiktokConnection.on('follow', (data) => {
    const identity = normalizeIdentity(data);

    queueEvent('follow', {
        uniqueId: identity.uniqueId,
        nickname: identity.nickname
    });
});

// ENTRADA NA LIVE
tiktokConnection.on('member', (data) => {
    const identity = normalizeIdentity(data);

    queueEvent('member', {
        uniqueId: identity.uniqueId,
        nickname: identity.nickname,
        userId: identity.userId
    });
});

// INFO DE SALA
tiktokConnection.on('roomUser', (data) => {
    queueEvent('roomUser', {
        viewerCount: Math.max(0, toInt(data?.viewerCount, 0)),
        likeCount: Math.max(0, toInt(data?.likeCount, 0))
    });
});

// INSCRIÇÃO
tiktokConnection.on('subscribe', (data) => {
    const identity = normalizeIdentity(data);

    queueEvent('subscribe', {
        uniqueId: identity.uniqueId,
        nickname: identity.nickname,
        subscribeInfo: {
            isSubscriber: Boolean(data?.isSubscriber),
            subscribeType: sanitizeForPawn(data?.subscribeType || '', 32),
            subMonth: Math.max(0, toInt(data?.subMonth, 0)),
            oldSubscribeStatus: Math.max(0, toInt(data?.oldSubscribeStatus, 0)),
            upgrading: Boolean(data?.upgrading)
        }
    });
});

// FIM DA LIVE
tiktokConnection.on('streamEnd', () => {
    queueEvent('streamEnd', {});
    console.warn('[TIKTOK] Live encerrada (streamEnd).');
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
