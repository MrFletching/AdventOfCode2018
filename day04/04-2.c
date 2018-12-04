#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINE_LENGTH_MAX 100
#define TRACE_COUNT 1500
#define TIMESTAMP_SIZE 16
#define SLEEP_LOG_SIZE 1000
#define GUARD_SIZE 100

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

typedef struct {
    int guardId;
    int fellAsleepAt;
    int wakeUpAt;
    int minutesAsleep;
} sleepLog;

typedef struct {
    int guardId;
    sleepLog sleepLog[SLEEP_LOG_SIZE];
    int sleepLogSize;
} guard;

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

guard* findGuard(guard *guards, int guardCount, int guardId) {
    for(int i = 0; i < guardCount; i++) {
        if(guards[i].guardId == guardId) {
            return &guards[i];
        }
    }
    
    return NULL;
}

int main() {
    trace tracepoints[TRACE_COUNT];
    
    int tracePointsCount = readLog(tracepoints);
    
    qsort(tracepoints, tracePointsCount, sizeof(tracepoints[0]), compareTracepoints);
    
    sleepLog sl[SLEEP_LOG_SIZE];
    int sleepLogCount = 0;
    int currentGuard = -1;
    
    for(int i = 0; i < tracePointsCount; i++) {
        trace currentTrace = tracepoints[i];
        
        if(currentTrace.event == EVENT_BEGIN_SHIFT) {
            currentGuard = currentTrace.guardId;
            sl[sleepLogCount].guardId = currentGuard;
        } else if(currentTrace.event == EVENT_FALL_ASLEEP) {
            sl[sleepLogCount].fellAsleepAt = currentTrace.minutes;
        } else if(currentTrace.event == EVENT_WAKE_UP) {
            sl[sleepLogCount].wakeUpAt = currentTrace.minutes;
            sl[sleepLogCount].minutesAsleep = sl[sleepLogCount].wakeUpAt - sl[sleepLogCount].fellAsleepAt;
            sleepLogCount++;
            sl[sleepLogCount].guardId = currentGuard;
        }
    }
    
    int guardCount = 0;
    guard guards[GUARD_SIZE];
    
    for(int i = 0; i < sleepLogCount; i++) {
        // printf("Guard #%d was asleep for %d minutes\n", sl[i].guardId, sl[i].minutesAsleep);
        
        // Look for the guard in existing guards
        guard *g;
        g = findGuard(guards, guardCount, sl[i].guardId);
        
        if(g == NULL) {
            // Guard not found, create a new guard
            g = &guards[guardCount];
            g->guardId = sl[i].guardId;
            g->sleepLogSize = 0;
            guardCount++;
        }
        
        g->sleepLog[g->sleepLogSize] = sl[i];
        g->sleepLogSize++;
    }
    
    // Find out guard that slept the most on a particular minute
    
    int asleepGuardId = -1;
    int asleepMinute = -1;
    int asleepCount = -1;
    
    
    for(int i = 0; i < guardCount; i++) {
        guard *g;
        g = &guards[i];
        
        int minutes[60] = {0};
        
        for(int j = 0; j < g->sleepLogSize; j++) {
            sleepLog *sl;
            sl = &g->sleepLog[j];
            
            for(int m = sl->fellAsleepAt; m < sl->wakeUpAt; m++) {
                minutes[m]++;
            }
        }
        
        int maxMinute = -1;
        int maxMinuteCount = -1;
        
        for(int m = 0; m < 60; m++) {
            if(minutes[m] > maxMinuteCount) {
                maxMinuteCount = minutes[m];
                maxMinute = m;
            }
        }
        
        if(maxMinuteCount > asleepCount) {
            asleepCount = maxMinuteCount;
            asleepMinute = maxMinute;
            asleepGuardId = g->guardId;
        }
        
    }
    
    printf("Guard #%d was asleep %d times on minute %d\n", asleepGuardId, asleepCount, asleepMinute);
    printf("%d * %d = %d\n", asleepGuardId, asleepMinute, asleepGuardId * asleepMinute);
    
    
    return 0;
}
