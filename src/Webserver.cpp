#include "LedManager.h"
#include "Webserver.h"

Webserver::Webserver(LEDManager &ledRef) : LED(ledRef), server(80) {}

void Webserver::begin() {
    server.begin();
}

void signalSuccess(WiFiClient& client) {
    client.println("HTTP/1.1 201 No Content");
    client.println("Server: PotatOS");
    client.println("Connection: close");
    client.println();
}

void Webserver::handleRequest() {
    WiFiClient client = server.available();

    // return if we don't have a client
    if (!client) {
        return;
    }

    String currentLine = "";
    // String lastLine = "";
    // int emptyCounter = 0;
    currentTime = millis();
    previousTime = currentTime;

    while (client.connected() && currentTime - previousTime <= timeoutTime) {
        currentTime = millis();

        // continue if there is no stuff to read
        if (!client.available()) {
            continue;
        }

        // start reading the data stream
        char c = client.read();
        // write it to the console
        // Serial.write(c);

        // new row in the http request
        if (c == '\n') {
            if (currentLine.startsWith("GET")) {
                int currLineLength = currentLine.length() + 1;
                char currLine[currLineLength];
                currentLine.toCharArray(currLine, currLineLength);

                char* command = strtok(currLine, " ");

                while (command != 0) {
                    if (command[0] != '/') {
                        // Find the next command in input string
                        command = strtok(0, " ");
                        continue;
                    }

                    String commandStr(command);

                    if (commandStr.equals("/lights/off")) {
                        LED.set(LED_HORN, false, false);
                        signalSuccess(client);
                    } else if (commandStr.equals("/lights/on")) {
                        LED.set(LED_HORN, true, false);
                        signalSuccess(client);
                    } else {
                        client.println("HTTP/1.1 404 Not Found");
                        client.println("Server: PotatOS");
                        client.println("Connection: close");
                        client.println();
                    }

                    // Find the next command in input string
                    command = strtok(0, " ");
                }

                return;
            }

            currentLine = "";
        } else if (c != '\r') {
            currentLine += c;
        }
    }
}