#include <stdio.h>
#include <time.h>
#include <list>

#include <mosquitto.h>

#include <wiringPi.h>

using namespace std;

#define PIN 7

list<int> medicoes;

int medirLuz(int amostragens)
{
	printf("\n\nLendo sensor...");
	
    int totalCount = 0, count, i, medicao;

    for (i = 0; i < amostragens; ++i)
    {
        pinMode(PIN, OUTPUT);
        digitalWrite(PIN, LOW);
        delay(100);

        pinMode(PIN, INPUT);

        count = 0;

        while (digitalRead(PIN) == LOW)
        {
            delayMicroseconds(100);
            ++count;
        }

        totalCount += count;
    }

    medicao =  totalCount / amostragens;
	
	medicoes.push_back(medicao);
	
	if(medicoes.size() > 10)
		medicoes.pop_front();
	
	
	printf("\nMedicao:\t\t");
	
	for(list<int>::iterator it = medicoes.begin(); it != medicoes.end(); it++)
	{
		printf("%d\t", *it);
	}
	
	list<int> ordenadaBruta(medicoes.begin(), medicoes.end());
	
	ordenadaBruta.sort();
	
	printf("\nBruta:\t\t");
	
	for(list<int>::iterator it = ordenadaBruta.begin(); it != ordenadaBruta.end(); it++)
	{
		printf("%d\t", *it);
	}
	
	int descartar = medicoes.size() / 3;
	
	list<int>::iterator
		begin = ordenadaBruta.begin(),
		end = ordenadaBruta.end();
		
	advance(begin, descartar);
	advance(end, -descartar);
	
	list<int> ordenada(begin, end);
	
	int totalMedia = 0;
	
	printf("\nOrdenada:\t\t");
	
	for(list<int>::iterator it = ordenada.begin(); it != ordenada.end(); it++)
	{
		printf("%d\t", *it);
		totalMedia += *it;
	}
	
	return totalMedia / ( medicoes.size() -  descartar * 2);
}

void getdate(char *output)
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    sprintf(output, "%d-%02d-%02d %02d:%02d:%02d",
            timeinfo->tm_year + 1900,
            timeinfo->tm_mon + 1,
            timeinfo->tm_mday,
            timeinfo->tm_hour,
            timeinfo->tm_min,
            timeinfo->tm_sec);
}

void log_file(const char *str)
{
    char date[32];

    getdate(date);

    FILE *f = fopen("log.txt", "a");
    fprintf(f, "%s\t%s\n", date, str);
    fclose(f);
}

int main(void)
{
    int messageId;
    const char *host = "127.0.0.1";
    int port = 1883;
    int keepalive = 60;
    bool clean_session = true;
    struct mosquitto *mosq = NULL;
    char date[32], payload[8];

    log_file("1 min de atraso...");
    delay(60 * 1000);

    log_file("Iniciando...");

    wiringPiSetup();

    mosquitto_lib_init();

    mosq = mosquitto_new(NULL, clean_session, NULL);

    if (!mosq)
    {
        log_file("Erro: sem memoria");
        return 1;
    }

    if (mosquitto_connect(mosq, host, port, keepalive))
    {
        log_file("Nao foi possivel conectar no broker");
        return 1;
    }

    const char *topic = "/sensor/luz/externo";

    int mqttErr;

    mosquitto_loop_start(mosq);

    for (;;)
    {
        getdate(date);

        // if (mosquitto_loop(mosq, -1, 1) != MOSQ_ERR_SUCCESS)
        // {
        //     log_file("Reconectando...");

        //     if (MOSQ_ERR_SUCCESS != mosquitto_reconnect(mosq))
        //         log_file("Erro ao tentar reconectar no broker");
        // }

        int medicao = medirLuz(10);

        int payloadSize = sprintf(payload, "%d", medicao);

        mqttErr = mosquitto_publish(mosq, &messageId, topic, payloadSize, payload, 2, true);

        if (MOSQ_ERR_SUCCESS != mqttErr)
            log_file("Erro ao publicar");

        FILE *file = fopen("medicao.txt", "a");

        fprintf(file, "%s\t%d\n", date, medicao);

        fclose(file);

        delay(1 * 60 * 1000);
    }

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}
