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
    constructor = (*env)->GetMethodID(env, cls, "<init>", "(IILjava/lang/String;IILjava/lang/String;)V");
    if (constructor == NULL)
    {
        return NULL;
    }
    result = (*env)->NewObjectArray(env, count, cls, NULL);
    if (result == NULL)
    {
        return NULL;
    }
    printf("Event log opened successfully\n");
    while (ReadEventLog(hEventLog, EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ,
                        0, szBuffer, sizeof(szBuffer), &dwBytesRead, &dwBytesNeeded))
    {
        pevlr = (EVENTLOGRECORD *)szBuffer;
        while ((DWORD)((LPBYTE)pevlr - (LPBYTE)szBuffer) < dwBytesRead)
        {
            jeventID = pevlr->EventID;
            jeventType = pevlr->EventType;
            jsource = (*env)->NewStringUTF(env, (LPSTR)((LPBYTE)pevlr + sizeof(EVENTLOGRECORD)));
            jcategory = pevlr->EventCategory;
            jtimeGenerated = pevlr->TimeGenerated;
            jdescription = (*env)->NewStringUTF(env, (LPSTR)((LPBYTE)pevlr + pevlr->StringOffset));
            jobject eventLogRecord = (*env)->NewObject(env, cls, constructor, jeventID, jeventType, jsource, jcategory, jtimeGenerated, jdescription);
            (*env)->SetObjectArrayElement(env, result, i, eventLogRecord);
            if (i == count - 1)
                break;
            (*env)->DeleteLocalRef(env, eventLogRecord);
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
