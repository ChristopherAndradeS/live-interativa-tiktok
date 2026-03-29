
stock TimestampToDate(Timestamp, &year, &month, &day, &hour, &minute, &second, HourGMT, MinuteGMT = 0)
{
    // Ajuste de fuso horário
    Timestamp += (HourGMT * 3600) + (MinuteGMT * 60);

    second = Timestamp % 60;
    new t_minute = Timestamp / 60;
    minute = t_minute % 60;
    new t_hour = t_minute / 60;
    hour = t_hour % 24;

    // Algoritmo de Howard Hinnant para dias
    new days = t_hour / 24;
    
    // Ajuste de época: Hinnant utiliza 01/03/0000 como base
    // Dias entre 01/01/1970 e 01/03/0000 = 719468
    days += 719468;

    new era = (days >= 0 ? days : days - 146096) / 146097;
    new doe = days - era * 146097;                                  // Dia da era (0-146096)
    new yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;      // Ano da era (0-399)
    new y = yoe + era * 400;
    new doy = doe - (365 * yoe + yoe/4 - yoe/100);                  // Dia do ano (0-365)
    new mp = (5 * doy + 2) / 153;                                   // Mês (0-11, sendo 0 = Março)
    
    day = doy - (153 * mp + 2) / 5 + 1;
    month = mp + (mp < 10 ? 3 : -9);
    year = y + (month <= 2 ? 1 : 0);
}