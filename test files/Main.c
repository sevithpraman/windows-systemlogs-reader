#include <windows.h>
#include <stdio.h>

int main()
{
    HANDLE hEventLog;
    EVENTLOGRECORD *pEventLogRecord;
    DWORD dwRead, dwNeeded, dwRecordNum;
    char szBuffer[2048];
    // test
    printf("\n************System Log Reader************\n");
    // Open the System event log
    hEventLog = OpenEventLog(NULL, "System");

    if (hEventLog == NULL)
    {
        printf("Failed to open the System event log (error code: %d)\n", GetLastError());
        return 1;
    }

    printf("Event log opened successfully\n");
    printf("%s\n", hEventLog);
    dwRecordNum = 0;
    printf("dwRead: %d\n", dwRead);
    printf("dwNeeded: %d\n", dwNeeded);
    printf("dwRecordNum: %d\n", dwRecordNum);
    printf("EVENTLOG_SEQUENTIAL_READ: %d\n", EVENTLOG_SEQUENTIAL_READ);
    printf("EVENTLOG_FORWARDS_READ: %d\n", EVENTLOG_FORWARDS_READ);
    printf("sizeof(EVENTLOGRECORD): %d\n", sizeof(EVENTLOGRECORD));
    printf("pEventLogRecord: %d\n", pEventLogRecord);
    printf("sizeof(pEventLogRecord): %d\n", sizeof(pEventLogRecord));

    // Read the event log records
    dwRecordNum = 0;

    while (ReadEventLog(hEventLog, EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ, dwRecordNum, szBuffer, sizeof(szBuffer), &dwRead, &dwNeeded))
    {
        pEventLogRecord = (EVENTLOGRECORD *)szBuffer;
        while (dwRead > 0)
        {
            printf("Event ID: %d\n", pEventLogRecord->EventID);
            printf("Event Type: %d\n", pEventLogRecord->EventType);

            WCHAR *SourceName = (WCHAR *)((unsigned char *)pEventLogRecord + sizeof(*pEventLogRecord));
            WCHAR *Computername = (WCHAR *)((unsigned char *)pEventLogRecord + sizeof(*pEventLogRecord) + wcslen(SourceName) * sizeof(WCHAR) + sizeof(WCHAR));
            WCHAR *UserSid = (WCHAR *)((unsigned char *)pEventLogRecord + sizeof(*pEventLogRecord) + wcslen(SourceName) * sizeof(WCHAR) + sizeof(WCHAR) + wcslen(Computername) * sizeof(WCHAR) + sizeof(WCHAR));

            printf("Event ID: %d\n", pEventLogRecord->EventID);
            printf("Event Type: %d\n", pEventLogRecord->EventType);
            printf("Source: %s\n", SourceName);
            printf("Category: %d\n", pEventLogRecord->EventCategory);
            printf("Time Generated: %d\n", pEventLogRecord->TimeGenerated);
            printf("Time Written: %d\n", pEventLogRecord->TimeWritten);
            printf("Record Number: %d\n", pEventLogRecord->RecordNumber);
            printf("User: %s\n", UserSid);
            printf("Computer: %s\n", Computername);
            printf("Description: %s\n", (char *)((BYTE *)pEventLogRecord + pEventLogRecord->StringOffset));
        }

        dwRecordNum = pEventLogRecord->RecordNumber + 1;

        LocalFree(pEventLogRecord);
    }

    CloseEventLog(hEventLog);

    return 0;
}
