#include <open.mp>
#include <requests>

#define LIVE_API_ENDPOINT "http://127.0.0.1:3000"
#define LIVE_API_EVENTS_PATH "/events"
#define LIVE_API_POLL_INTERVAL_MS (1000)

forward OnLiveMessageText(user[], message[]);
forward OnLiveCommandReceive(user[], command[]);
forward OnLiveGiftReceive(user[], giftName[], amount);

forward FetchLiveEvents();
forward OnLiveEventsResponse(Request:req, E_HTTP_STATUS:status, Node:node);

new RequestsClient:gLiveClient = RequestsClient:-1;

main() {}

public OnGameModeInit()
{
    gLiveClient = RequestsClient(LIVE_API_ENDPOINT);

    if (!IsValidRequestsClient(gLiveClient))
    {
        printf("[TIKTOK] Falha ao criar RequestsClient para %s. Client ID: %d", LIVE_API_ENDPOINT, _:gLiveClient);
        return 1;
    }

    printf("[TIKTOK] RequestsClient conectado em %s (id=%d)", LIVE_API_ENDPOINT, _:gLiveClient);

    SetTimer("FetchLiveEvents", LIVE_API_POLL_INTERVAL_MS, true);
    print("[TIKTOK] Polling de eventos iniciado.");

    return 1;
}

public OnRequestFailure(Request:id, errorCode, errorMessage[], len)
{
    printf("[TIKTOK] Request failure | request=%d | code=%d | message=%s", _:id, errorCode, errorMessage);
    return 1;
}

public FetchLiveEvents()
{
    if (!IsValidRequestsClient(gLiveClient))
    {
        print("[TIKTOK] RequestsClient invalido durante FetchLiveEvents.");
        return 1;
    }

    new Request:req = RequestJSON(
        gLiveClient,
        LIVE_API_EVENTS_PATH,
        HTTP_METHOD_GET,
        "OnLiveEventsResponse"
    );

    if (!IsValidRequest(req))
    {
        print("[TIKTOK] Falha ao enviar request para /events.");
    }

    return 1;
}

public OnLiveEventsResponse(Request:req, E_HTTP_STATUS:status, Node:node)
{
    if (status != HTTP_STATUS_OK)
    {
        printf("[TIKTOK] HTTP inesperado em /events: %d", _:status);
        return 1;
    }

    if (JsonNodeType(node) != JSON_NODE_ARRAY)
    {
        print("[TIKTOK] /events nao retornou um JSON array.");
        return 1;
    }

    new length;
    JsonArrayLength(node, length);

    if (length <= 0)
    {
        return 1;
    }

    new user[64], message[144], giftName[64], type[24], repeatCount;

    for (new i = 0; i < length; i++)
    {
        new Node:eventNode;
        JsonArrayObject(node, i, eventNode);

        if (JsonNodeType(eventNode) != JSON_NODE_OBJECT)
        {
            continue;
        }

        type[0] = '\0';
        user[0] = '\0';

        JsonGetString(eventNode, "type", type, sizeof(type));
        JsonGetString(eventNode, "user", user, sizeof(user));

        if (!strcmp(type, "chat", true))
        {
            message[0] = '\0';
            JsonGetString(eventNode, "message", message, sizeof(message));

            if (message[0] == '\0' || user[0] == '\0')
            {
                continue;
            }

            CallLocalFunction("OnLiveMessageText", "ss", user, message);

            if (message[0] == '!')
            {
                CallLocalFunction("OnLiveCommandReceive", "ss", user, message);
            }
        }
        else if (!strcmp(type, "gift", true))
        {
            giftName[0] = '\0';
            repeatCount = 1;

            JsonGetString(eventNode, "giftName", giftName, sizeof(giftName));
            JsonGetInt(eventNode, "repeatCount", repeatCount);

            if (giftName[0] == '\0' || user[0] == '\0')
            {
                continue;
            }

            CallLocalFunction("OnLiveGiftReceive", "ssi", user, giftName, repeatCount);
        }
    }

    return 1;
}

public OnLiveMessageText(user[], message[])
{
    printf("[LIVE CHAT] %s: %s", user, message);
    return 1;
}

public OnLiveCommandReceive(user[], command[])
{
    printf("[LIVE CMD] %s -> %s", user, command);

    if (!strcmp(command, "!car", true))
    {
        printf("[LIVE CMD] %s solicitou um carro!", user);
    }

    return 1;
}

public OnLiveGiftReceive(user[], giftName[], amount)
{
    printf("[LIVE GIFT] %s enviou %d x %s", user, amount, giftName);
    return 1;
}
