#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINE_LENGTH_MAX 100
#define TRACE_COUNT 1500
#define TIMESTAMP_SIZE 16

typedef char timestamp[TIMESTAMP_SIZE+1];

typedef enum {
    EVENT_INVALID,
    EVENT_BEGIN_SHIFT,
    EVENT_FALL_ASLEEP,
    EVENT_WAKE_UP,
} event;

typedef struct {
    timestamp ts;
    event event;
    int guardId;
    int minutes;
} trace;


trace parseLogLine(const char logLine[LINE_LENGTH_MAX]) {
    
    trace tracepoint;
    tracepoint.event = EVENT_INVALID;
    
    char logMessage[10];
    char guardIdStr[10];
    
    if(sscanf(logLine, "[%16[^]]] %s #%d", tracepoint.ts, logMessage, &tracepoint.guardId) != 0) {
        
        if(strcmp(logMessage, "Guard") == 0) {
            tracepoint.event = EVENT_BEGIN_SHIFT;
        } else if(strcmp(logMessage, "falls") == 0) {
            tracepoint.event = EVENT_FALL_ASLEEP;
        } else if(strcmp(logMessage, "wakes") == 0) {
            tracepoint.event = EVENT_WAKE_UP;
        }
        
    }
    
    char minuteStr[3];
    memcpy(minuteStr, &tracepoint.ts[14], 2);
    minuteStr[2] = '\0';
    
    sscanf(minuteStr, "%d", &tracepoint.minutes);
    
    return tracepoint;
}

int readLog(trace *tracepoints) {
    FILE *fp;
    
    fp = fopen("input.txt", "r");
    
    char line[LINE_LENGTH_MAX];
    int tracePointsCount = 0;
    
    while(fgets(line, LINE_LENGTH_MAX, fp) != NULL) {
        tracepoints[tracePointsCount] = parseLogLine(line);
        tracePointsCount++;
    }
    
    fclose(fp);
    
    return tracePointsCount;
}

void printTracepoint(trace tracepoint) {
    if(tracepoint.event == EVENT_BEGIN_SHIFT) {
        printf("%s (%2d): EVENT_BEGIN_SHIFT (Guard #%d)\n", tracepoint.ts, tracepoint.minutes, tracepoint.guardId);
    } else if(tracepoint.event == EVENT_FALL_ASLEEP) {
        printf("%s (%2d): EVENT_FALL_ASLEEP\n", tracepoint.ts, tracepoint.minutes);
    } else if(tracepoint.event == EVENT_WAKE_UP) {
        printf("%s (%2d): EVENT_WAKE_UP\n", tracepoint.ts, tracepoint.minutes);
    }
}

int compareTracepoints(const void *a, const void *b) {
    const trace *traceA = (const trace*)a;
    const trace *traceB = (const trace*)b;
    return strcmp(traceA->ts, traceB->ts);
}

int main() {
    trace tracepoints[TRACE_COUNT];
    
    int tracePointsCount = readLog(tracepoints);
    
    qsort(tracepoints, tracePointsCount, sizeof(tracepoints[0]), compareTracepoints);
    
    for(int i = 0; i < tracePointsCount; i++) {
        printTracepoint(tracepoints[i]);
    }
    
    return 0;
}
