#include <logging.h>
#include <stdio.h>
#include <iostream>

using namespace std;

FILE* Logger::logger = fopen("logger", "w");

void Logger::LogEvent(const char *event) {
    if (Logger::logger == NULL) {	
        Logger::logger = fopen("logger", "w");
    }
    fprintf(Logger::logger, "%s\n", event);
}

DataFlowException::DataFlowException(const char *type, const char *error) {
    sprintf(msg, "Throwing exception: (%s): %s", type, error);
    Logger::LogEvent(msg);
}

void Logger::Finalize() {
    fclose(Logger::logger);
}
