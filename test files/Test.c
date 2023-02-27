#include <windows.h>
#include <stdio.h>

int main()
{
    HANDLE hEventLog;
    EVENTLOGRECORD *pevlr;
    DWORD dwBytesRead, dwBytesNeeded;
    char szBuffer[2048];

    // welcome message
    printf("\n************System Log Reader************\n");
    // Open the event log
    hEventLog = OpenEventLog(NULL, "System");
    if (hEventLog == NULL)
    {
        printf("Failed to open the event log (error code %d).\n", GetLastError());
        return 1;
    }
    // print the values
    printf("Event log opened successfully\n");
    printf("hEventLog: %d\n", hEventLog);
    printf("dwBytesRead: %d\n", dwBytesRead);
    printf("dwBytesNeeded: %d\n", dwBytesNeeded);
    printf("EVENTLOG_SEQUENTIAL_READ: %d\n", EVENTLOG_SEQUENTIAL_READ);
    printf("EVENTLOG_FORWARDS_READ: %d\n", EVENTLOG_FORWARDS_READ);
    printf("sizeof(EVENTLOGRECORD): %d\n", sizeof(EVENTLOGRECORD));
    printf("pevlr: %d\n", pevlr);
    printf("sizeof(pevlr): %d\n", sizeof(pevlr));

    // Read the event log records
    while (ReadEventLog(hEventLog, EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ,
                        0, szBuffer, sizeof(szBuffer), &dwBytesRead, &dwBytesNeeded))
    {
        pevlr = (EVENTLOGRECORD *)szBuffer;

        while ((DWORD)((LPBYTE)pevlr - (LPBYTE)szBuffer) < dwBytesRead)
        {
            printf("----------------------------------------\n");
            printf("Event ID: %d\n", pevlr->EventID);
            printf("Event Type: %d\n", pevlr->EventType);
            printf("Source: %s\n", (LPSTR)((LPBYTE)pevlr + sizeof(EVENTLOGRECORD)));
            printf("Description: %.*s\n", pevlr->StringOffset - sizeof(EVENTLOGRECORD),
                   (LPSTR)((LPBYTE)pevlr + pevlr->StringOffset));
            printf("----------------------------------------\n");
            pevlr = (EVENTLOGRECORD *)((LPBYTE)pevlr + pevlr->Length);
        }
    }

    // Close the event log
    CloseEventLog(hEventLog);

    return 0;
}
