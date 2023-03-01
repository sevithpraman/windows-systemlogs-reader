#include "C:/Program Files/Java/jdk1.8.0_333/include/jni.h"
#include "C:/Program Files/Java/jdk1.8.0_333/include/win32/jni_md.h"
#include <windows.h>
#include <stdio.h>
#include "Test.h"

JNIEXPORT jobjectArray JNICALL Java_Test_readSystemLog(JNIEnv *env, jobject obj, jstring logName)
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

// #include "C:/Program Files/Java/jdk1.8.0_333/include/jni.h"
// #include "C:/Program Files/Java/jdk1.8.0_333/include/win32/jni_md.h"
// #include <windows.h>
// #include <stdio.h>
// #include "Test.h"

// JNIEXPORT jobjectArray JNICALL Java_Test_readSystemLog(JNIEnv *env, jobject obj, jstring logName)
// {
//     jobjectArray result;
//     jclass cls;
//     jmethodID constructor;
//     jstring jsource, jdescription;
//     HANDLE hEventLog;
//     EVENTLOGRECORD *pevlr;
//     DWORD dwBytesRead, dwBytesNeeded;
//     char szBuffer[2048];
//     int i = 0, count = 1000;

//     // Convert the Java string to a C string
//     const char *cLogName = (*env)->GetStringUTFChars(env, logName, NULL);
//     if (cLogName == NULL)
//     {
//         return NULL;
//     }

//     // print the log name
//     printf("Log name: %s\n", cLogName);

//     // Open the event log
//     hEventLog = OpenEventLog(NULL, cLogName);
//     if (hEventLog == NULL)
//     {
//         printf("Failed to open the event log (error code %d).\n", GetLastError());
//         return NULL;
//     }

//     // Count the number of event log records
//     // while (ReadEventLog(hEventLog, EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ,
//     //                     0, szBuffer, sizeof(szBuffer), &dwBytesRead, &dwBytesNeeded))
//     // {
//     //     pevlr = (EVENTLOGRECORD *)szBuffer;

//     //     while ((DWORD)((LPBYTE)pevlr - (LPBYTE)szBuffer) < dwBytesRead)
//     //     {
//     //         count++;

//     //         pevlr = (EVENTLOGRECORD *)((LPBYTE)pevlr + pevlr->Length);
//     //     }
//     // }

//     // print the number of records
//     printf("Number of records: %d\n", count);

//     // Allocate the array of objects
//     cls = (*env)->FindClass(env, "SystemLog");
//     if (cls == NULL)
//     {
//         return NULL;
//     }

//     // print the class name
//     printf("Class name: %s\n", cls);

//     // Get the constructor ID
//     constructor = (*env)->GetMethodID(env, cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
//     if (constructor == NULL)
//     {
//         return NULL;
//     }

//     // print the constructor
//     printf("Constructor: %s\n", constructor);

//     // Allocate the array of objects
//     result = (*env)->NewObjectArray(env, count, cls, NULL);
//     if (result == NULL)
//     {
//         return NULL;
//     }

//     // print the result
//     printf("Result: %s\n", result);

//     // print the values
//     printf("Event log opened successfully\n");
//     printf("hEventLog: %d\n", hEventLog);
//     printf("dwBytesRead: %d\n", dwBytesRead);
//     printf("dwBytesNeeded: %d\n", dwBytesNeeded);
//     printf("EVENTLOG_SEQUENTIAL_READ: %d\n", EVENTLOG_SEQUENTIAL_READ);
//     printf("EVENTLOG_BACKWARDS_READ: %d\n", EVENTLOG_BACKWARDS_READ);
//     printf("sizeof(EVENTLOGRECORD): %d\n", sizeof(EVENTLOGRECORD));
//     printf("pevlr: %d\n", pevlr);
//     printf("sizeof(pevlr): %d\n", sizeof(pevlr));

//     // Read the event log records and add them to the array
//     while (ReadEventLog(hEventLog, EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ,
//                         0, szBuffer, sizeof(szBuffer), &dwBytesRead, &dwBytesNeeded))
//     {
//         // move to the first event log record
//         pevlr = (EVENTLOGRECORD *)szBuffer;
//         if (i == 999)
//             break;
//         // print pevlr
//         printf("pevlr: %d\n", pevlr);

//         while ((DWORD)((LPBYTE)pevlr - (LPBYTE)szBuffer) < dwBytesRead)
//         {
//             printf("----------------------------------------\n");
//             printf("Event ID: %d\n", pevlr->EventID);
//             printf("Event Type: %d\n", pevlr->EventType);
//             printf("Source: %s\n", (LPSTR)((LPBYTE)pevlr + sizeof(EVENTLOGRECORD)));
//             printf("Category: %d\n", pevlr->EventCategory);
//             printf("Time Generated: %d\n", pevlr->TimeGenerated);
//             // printf("Time Written: %d\n", pevlr->TimeWritten);
//             printf("Record Number: %d\n", pevlr->RecordNumber);
//             printf("Description: %.*s\n", pevlr->StringOffset - sizeof(EVENTLOGRECORD),
//                    (LPSTR)((LPBYTE)pevlr + pevlr->StringOffset));
//             printf("----------------------------------------\n");

//             jsource = (*env)->NewStringUTF(env, (LPSTR)((LPBYTE)pevlr + sizeof(EVENTLOGRECORD)));

//             // print the source
//             // printf("Source: %s\n", jsource);

//             jdescription = (*env)->NewStringUTF(env, (LPSTR)((LPBYTE)pevlr + pevlr->StringOffset));

//             //   jstring jtime = (*env)->NewStringUTF(env, (LPSTR)((LPBYTE)pevlr + pevlr->TimeGenerated));

//             //  jstring juser = (*env)->NewStringUTF(env, (LPSTR)((LPBYTE)pevlr + pevlr->UserSidLength));

//             //   printf("Time: %s\n", jtime);
//             //  printf("User: %s\n", juser);

//             // print the description
//             // printf("Description: %s\n", jdescription);

//             // print i value
//             printf("i: %d\n", i);

//             // store the values in the array
//             jobject eventLogRecord = (*env)->NewObject(env, cls, constructor, jsource, jdescription);
//             (*env)->SetObjectArrayElement(env, result, i, eventLogRecord);
//             if (i == 999)
//                 break;
//             i++;

//             // Release the local references
//             (*env)->DeleteLocalRef(env, jsource);
//             (*env)->DeleteLocalRef(env, jdescription);

//             // Move to the next event log record
//             pevlr = (EVENTLOGRECORD *)((LPBYTE)pevlr + pevlr->Length);
//         }
//     }

//     // Close the event log
//     CloseEventLog(hEventLog);

//     // Release the C string
//     (*env)->ReleaseStringUTFChars(env, logName, cLogName);

//     // Return the array of objects
//     return result;
// }

// #include <windows.h>
// #include <stdio.h>

// JNIEXPORT void JNICALL Java_SystemLogReader_readSystemLog(JNIEnv *env, jobject obj, jobject writerObj)
// {
//     jclass writerClass = (*env)->GetObjectClass(env, writerObj);
//     jmethodID printlnMethod = (*env)->GetMethodID(env, writerClass, "println", "(Ljava/lang/String;)V");

//     HANDLE hEventLog;
//     EVENTLOGRECORD *pEventLogRecord;
//     DWORD dwRead, dwNeeded, dwRecordNum;

//     // Open the System event log
//     hEventLog = OpenEventLog(NULL, "System");

//     if (hEventLog == NULL)
//     {
//         (*env)->CallVoidMethod(env, writerObj, printlnMethod, (*env)->NewStringUTF(env, "Failed to open the System event log"));
//         return;
//     }

//     // Read the event log records
//     dwRecordNum = 0;

//     while (ReadEventLog(hEventLog, EVENTLOG_SEQUENTIAL_READ | EVENTLOG_FORWARDS_READ, dwRecordNum, &pEventLogRecord, sizeof(EVENTLOGRECORD), &dwRead, &dwNeeded))
//     {
//         if (dwRead > 0)
//         {
//             char buffer[1024];
//             sprintf(buffer, "Event ID: %d\nEvent Type: %d\nSource: %s\nDescription: %s\n",
//                     pEventLogRecord->EventID, pEventLogRecord->EventType, pEventLogRecord->EventType, (char *)((BYTE *)pEventLogRecord + pEventLogRecord->StringOffset));
//             (*env)->CallVoidMethod(env, writerObj, printlnMethod, (*env)->NewStringUTF(env, buffer));
//         }

//         dwRecordNum = pEventLogRecord->RecordNumber + 1;

//         LocalFree(pEventLogRecord);
//     }

//     CloseEventLog(hEventLog);
// }

// HANDLE hEventLog;
// EVENTLOGRECORD *pEventLogRecord;
// DWORD dwRead, dwNeeded, dwRecordNum;

// Open the System event log
// hEventLog = OpenEventLog(NULL, "System");

// if (hEventLog == NULL)
// {
//     printf("Failed to open the System event log (error code: %d)\n", GetLastError());
//     return 1;
// }

// Read the event log records

// dwRecordNum = 0;

//     while (ReadEventLog(hEventLog, EVENTLOG_SEQUENTIAL_READ | EVENTLOG_FORWARDS_READ, dwRecordNum, &pEventLogRecord, sizeof(EVENTLOGRECORD), &dwRead, &dwNeeded))
//     {
//         if (dwRead > 0)
//         {
//             printf("Event ID: %d\n", pEventLogRecord->EventID);
//             printf("Event Type: %d\n", pEventLogRecord->EventType);
//             // printf("Source: %s\n", pEventLogRecord->;SourceName);
//             printf("Category: %d\n", pEventLogRecord->EventCategory);
//             printf("Time Generated: %d\n", pEventLogRecord->TimeGenerated);
//             printf("Time Written: %d\n", pEventLogRecord->TimeWritten);
//             printf("Record Number: %d\n", pEventLogRecord->RecordNumber);
//             // printf("User: %s\n", pEventLogRecord->UserSid);
//             // printf("Computer: %s\n", pEventLogRecord->Computername);
//             printf("Description: %s\n", (char *)((BYTE *)pEventLogRecord + pEventLogRecord->StringOffset));
//         }

//         dwRecordNum = pEventLogRecord->RecordNumber + 1;

//         LocalFree(pEventLogRecord);
//     }
// }

// #include <windows.h>
// #include <stdio.h>

// int main()
// {
//     HANDLE hEventLog;
//     EVENTLOGRECORD *pevlr;
//     DWORD dwBytesRead, dwBytesNeeded;
//     char szBuffer[2048];

//     // welcome message
//     printf("\n************System Log Reader************\n");
//     // Open the event log
//     hEventLog = OpenEventLog(NULL, "System");
//     if (hEventLog == NULL)
//     {
//         printf("Failed to open the event log (error code %d).\n", GetLastError());
//         return 1;
//     }
//     // print the values
//     printf("Event log opened successfully\n");
//     printf("hEventLog: %d\n", hEventLog);
//     printf("dwBytesRead: %d\n", dwBytesRead);
//     printf("dwBytesNeeded: %d\n", dwBytesNeeded);
//     printf("EVENTLOG_SEQUENTIAL_READ: %d\n", EVENTLOG_SEQUENTIAL_READ);
//     printf("EVENTLOG_FORWARDS_READ: %d\n", EVENTLOG_FORWARDS_READ);
//     printf("sizeof(EVENTLOGRECORD): %d\n", sizeof(EVENTLOGRECORD));
//     printf("pevlr: %d\n", pevlr);
//     printf("sizeof(pevlr): %d\n", sizeof(pevlr));

//     // Read the event log records
//     while (ReadEventLog(hEventLog, EVENTLOG_BACKWARDS_READ | EVENTLOG_SEQUENTIAL_READ,
//                         0, szBuffer, sizeof(szBuffer), &dwBytesRead, &dwBytesNeeded))
//     {
//         pevlr = (EVENTLOGRECORD *)szBuffer;

//         while ((DWORD)((LPBYTE)pevlr - (LPBYTE)szBuffer) < dwBytesRead)
//         {
//             printf("----------------------------------------\n");
//             printf("Event ID: %d\n", pevlr->EventID);
//             printf("Event Type: %d\n", pevlr->EventType);
//             printf("Source: %s\n", (LPSTR)((LPBYTE)pevlr + sizeof(EVENTLOGRECORD)));
//             printf("Description: %.*s\n", pevlr->StringOffset - sizeof(EVENTLOGRECORD),
//                    (LPSTR)((LPBYTE)pevlr + pevlr->StringOffset));
//             printf("----------------------------------------\n");
//             pevlr = (EVENTLOGRECORD *)((LPBYTE)pevlr + pevlr->Length);
//         }
//     }

//     // Close the event log
//     CloseEventLog(hEventLog);

//     return 0;
// }
