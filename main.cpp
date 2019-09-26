#include "mainwindow.h"
#include <QApplication>
#ifdef Q_OS_WIN
#include <windows.h>
#include <dbghelp.h>
#endif
#include <QMessageBox>
#include <QTime>

#ifdef Q_OS_WIN
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{ //程式异常捕获
    //创建 Dump 文件
    QDateTime CurDTime = QDateTime::currentDateTime();
    QString current_date = CurDTime.toString("yyyy_MM_dd_hh_mm_ss");
    //dmp文件的命名
    QString dumpText = "Dump_" + current_date + ".dmp";
    EXCEPTION_RECORD *record = pException->ExceptionRecord;
    QString errCode(QString::number(record->ExceptionCode, 16));
    QString errAddr(QString::number((long long)record->ExceptionAddress, 16));
    QString errFlag(QString::number(record->ExceptionFlags, 16));
    QString errPara(QString::number(record->NumberParameters, 16));
    HANDLE DumpHandle = CreateFile((LPCWSTR)dumpText.utf16(),
                                   GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (DumpHandle != INVALID_HANDLE_VALUE)
    {
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;
        //将dump信息写入dmp文件
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), DumpHandle, MiniDumpNormal, &dumpInfo, NULL, NULL);
        CloseHandle(DumpHandle);
    }
    //创建消息提示
    QMessageBox::warning(NULL, "Dump", QString("ErrorCode%1  ErrorAddr：%2  ErrorFlag:%3 ErrorPara:%4").arg(errCode).arg(errAddr).arg(errFlag).arg(errPara),
                         QMessageBox::Ok);
    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef Q_OS_WIN
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler); //注冊异常捕获函数
#endif
    MainWindow w;
    w.show();

    return a.exec();
}
