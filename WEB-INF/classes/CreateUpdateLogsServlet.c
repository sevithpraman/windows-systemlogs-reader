#include "C:/Program Files/Java/jdk1.8.0_333/include/jni.h"
#include "C:/Program Files/Java/jdk1.8.0_333/include/win32/jni_md.h"
#include <windows.h>
#include <stdio.h>
#include "CreateUpdateLogsServlet.h"

JNIEXPORT jobjectArray JNICALL Java_CreateUpdateLogsServlet_readSystemLog(JNIEnv *env, jobject obj, jstring logName)
{
    jobjectArray result;
    jclass cls;
    jmethodID constructor;
    jstring jsource, jdescription;
    jint jeventID, jeventType, jcategory, jtimeGenerated;
    HANDLE hEventLog;
    EVENTLOGRECORD *pevlr;
    DWORD dwBytesRead, dwBytesNeeded;
    char szBuffer[2048];
    int i = 0, count = 1000;
    printf("Count: %d\n", count);
    const char *cLogName = (*env)->GetStringUTFChars(env, logName, NULL);
    if (cLogName == NULL)
    {
        return NULL;
    }
    printf("Log name: %s\n", cLogName);
    hEventLog = OpenEventLog(NULL, cLogName);
    if (hEventLog == NULL)
    {
        printf("Failed to open the event log (error code %d).\n", GetLastError());
        return NULL;
    }
    printf("Number of records: %d\n", count);
    cls = (*env)->FindClass(env, "SystemLog");
    if (cls == NULL)
    {
        return NULL;
    }
    printf("Class name: %s\n", cls);
    constructor = (*env)->GetMethodID(env, cls, "<init>", "(IILjava/lang/String;IILjava/lang/String;)V");
    if (constructor == NULL)
    {
        return NULL;
    }
    printf("Constructor: %s\n", constructor);
    result = (*env)->NewObjectArray(env, count, cls, NULL);
    if (result == NULL)
    {
        return NULL;
    }
    printf("Result: %s\n", result);
    printf("Event log opened successfully\n");
    printf("hEventLog: %d\n", hEventLog);
    printf("dwBytesRead: %d\n", dwBytesRead);
    printf("dwBytesNeeded: %d\n", dwBytesNeeded);
    printf("EVENTLOG_SEQUENTIAL_READ: %d\n", EVENTLOG_SEQUENTIAL_READ);
    printf("EVENTLOG_BACKWARDS_READ: %d\n", EVENTLOG_BACKWARDS_READ);
    printf("sizeof(EVENTLOGRECORD): %d\n", sizeof(EVENTLOGRECORD));
    printf("pevlr: %d\n", pevlr);
    printf("sizeof(pevlr): %d\n", sizeof(pevlr));
    while (ReadEventLog(hEventLog, EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ,
                        0, szBuffer, sizeof(szBuffer), &dwBytesRead, &dwBytesNeeded))
    {

        pevlr = (EVENTLOGRECORD *)szBuffer;

        printf("pevlr: %d\n", pevlr);

        while ((DWORD)((LPBYTE)pevlr - (LPBYTE)szBuffer) < dwBytesRead)
        {
            printf("----------------------------------------\n");
            printf("Event ID: %d\n", pevlr->EventID);
            printf("Event Type: %d\n", pevlr->EventType);
            printf("Source: %s\n", (LPSTR)((LPBYTE)pevlr + sizeof(EVENTLOGRECORD)));
            printf("Category: %d\n", pevlr->EventCategory);
            printf("Time Generated: %d\n", pevlr->TimeGenerated);
            // printf("Time Written: %d\n", pevlr->TimeWritten);
            // printf("Record Number: %d\n", pevlr->RecordNumber);
            printf("Description: %.*s\n", pevlr->StringOffset - sizeof(EVENTLOGRECORD),
                   (LPSTR)((LPBYTE)pevlr + pevlr->StringOffset));
            printf("----------------------------------------\n");
            jeventID = pevlr->EventID;
            jeventType = pevlr->EventType;
            jsource = (*env)->NewStringUTF(env, (LPSTR)((LPBYTE)pevlr + sizeof(EVENTLOGRECORD)));
            jcategory = pevlr->EventCategory;
            jtimeGenerated = pevlr->TimeGenerated;
            // jrecordNumber = pevlr->RecordNumber;
            jdescription = (*env)->NewStringUTF(env, (LPSTR)((LPBYTE)pevlr + pevlr->StringOffset));
            printf("i: %d\n", i);
            printf("jeventID: %d\n", jeventID);
            printf("jeventType: %d\n", jeventType);
            printf("jsource: %s\n", jsource);
            printf("jtimeGenerated: %d\n", jtimeGenerated);
            printf("jcategory: %d\n", jcategory);
            // printf("jrecordNumber: %d\n", jrecordNumber);
            printf("jdescription: %s\n", jdescription);
            jobject eventLogRecord = (*env)->NewObject(env, cls, constructor, jeventID, jeventType, jsource, jcategory, jtimeGenerated, jdescription);
            (*env)->SetObjectArrayElement(env, result, i, eventLogRecord);
            if (i == count - 1)
                break;
            // delete local references
            (*env)->DeleteLocalRef(env, jsource);
            (*env)->DeleteLocalRef(env, jdescription);

            pevlr = (EVENTLOGRECORD *)((LPBYTE)pevlr + pevlr->Length);
            i++;
        }
    }
    CloseEventLog(hEventLog);
    (*env)->ReleaseStringUTFChars(env, logName, cLogName);
    return result;
}
