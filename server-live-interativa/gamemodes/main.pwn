#include <open.mp>
#include <requests>
#include <sscanf>

forward OnLiveMessageText(user[], message[]);
forward OnLiveCommandReceive(user[], command[]);
forward OnLiveGiftReceive(user[], giftName[], amount);

main() {}

#define API_URL "http://127.0.0.1:3000/events"

new RequestsClient:liveRequest;

public OnGameModeInit()
{
    liveRequest = RequestsClient("http://127.0.0.1:3000");

    print("RequestsClient criado com sucesso!");
    SetTimer("FetchLiveEvents", 1000, true);
    
    return 1;
}

public OnRequestFailure(Request:req, E_HTTP_STATUS:status)
{
    printf("Falha na requisição: %d", status);
    return 1;
}

public OnLiveMessageText(user[], message[])
{
    printf("[LIVE CHAT] %s: %s", user, message);
}

public OnLiveCommandReceive(user[], command[])
{
    if(strcmp(command, "!car", true) == 0)
    {
        printf("[COMMAND] %s solicitou um carro!", user);
    }
}

public OnLiveGiftReceive(user[], giftName[], amount)
{
    printf("[GIFT] %s enviou %d %s", user, amount, giftName);
}

forward FetchLiveEvents();
public FetchLiveEvents()
{
    Request(
        liveRequest,
        "/events",
        HTTP_GET,
        "OnLiveEventsResponse"
    );
}

forward OnLiveEventsResponse(Request:req, E_HTTP_STATUS:status, const data[], data_size);
public OnLiveEventsResponse(Request:req, E_HTTP_STATUS:status, const data[], data_size)
{
    if(status != HTTP_STATUS_OK) return 1;

    // data = JSON array
    // Exemplo:
    // [
    //   {"type":"chat","user":"abc","message":"oi"},
    //   {"type":"gift","user":"abc","giftName":"Rose","repeatCount":5}
    // ]

    ParseEvents(data);

    return 1;
}

stock ParseEvents(const json[])
{
    new idx = 0;

    while ((idx = strfind(json, "{", true, idx)) != -1)
    {
        new event[256];
        strmid(event, json, idx, idx + 200);

        if (strfind(event, "\"type\":\"chat\"", true) != -1)
        {
            new user[32], message[128];

            ExtractValue(event, "user", user, sizeof(user));
            ExtractValue(event, "message", message, sizeof(message));

            CallLocalFunction("OnLiveMessageText", "ss", user, message);

            // Comando (ex: !spawn)
            if(message[0] == '!')
            {
                CallLocalFunction("OnLiveCommandReceive", "ss", user, message);
            }
        }

        else if (strfind(event, "\"type\":\"gift\"", true) != -1)
        {
            new user[32], gift[32];
            new amount;

            ExtractValue(event, "user", user, sizeof(user));
            ExtractValue(event, "giftName", gift, sizeof(gift));
            amount = ExtractInt(event, "repeatCount");

            CallLocalFunction("OnLiveGiftReceive", "ssi", user, gift, amount);
        }

        idx++;
    }
}

stock ExtractValue(const source[], const key[], dest[], size)
{
    new pattern[32];
    format(pattern, sizeof(pattern), "\"%s\":\"", key);

    new start = strfind(source, pattern, true);
    if(start == -1) return 0;

    start += strlen(pattern);

    new end = strfind(source, "\"", true, start);

    strmid(dest, source, start, end, size);
    return 1;
}

stock ExtractInt(const source[], const key[])
{
    new pattern[32];
    format(pattern, sizeof(pattern), "\"%s\":", key);

    new start = strfind(source, pattern, true);
    if(start == -1) return 0;

    start += strlen(pattern);

    new value[16];
    strmid(value, source, start, start + 10);

    return strval(value);
}