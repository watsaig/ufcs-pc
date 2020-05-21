#include "logger.h"

static Logger* singleton = nullptr;

Logger* Logger::logger()
{
    if (singleton == nullptr)
        singleton = new Logger();
    return singleton;
}

Logger::Logger()
{
    // Initialize log file path
    QString dataLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/logs";
    QDir d;
    if (!d.mkpath(dataLocation))
        fprintf(stderr, "Could not create directory for log file storage\n");

    QString fileName = "log_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".txt";
    mLogFilePath = QDir::cleanPath(dataLocation + "/" + fileName);

    QByteArray path = mLogFilePath.toLocal8Bit();
    fprintf(stdout, "Log file location: %s\n", path.constData());

    connect(this , &Logger::newLogForFile, this, &Logger::logToFile);
    connect(this, &Logger::newLogForFile, this, &Logger::logToTerminal);
}

void Logger::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");

    QString messageType;
    QString message;

    switch (type) {
    case QtDebugMsg:
        messageType  = "Debug";
        message = msg;
        break;
    case QtInfoMsg:
        messageType  = "Info";
        message = msg;
        break;
    case QtWarningMsg:
        messageType  = "Warning";
        message = msg;
        break;
    case QtCriticalMsg:
        messageType  = "Critical error";
        message = QString("%1 (%2:%3, %4)").arg(msg).arg(context.file).arg(context.line).arg(context.function);
        break;
    case QtFatalMsg:
        messageType  = "Fatal error";
        message = QString("%1 (%2:%3, %4)").arg(msg).arg(context.file).arg(context.line).arg(context.function);
        break;
    }

    QString text = date + " " + time + " " + messageType + ": " + message + "\n";

    emit logger()->newLogForFile(text);

    // Logs are stored in a fragmented way to make rich markup easier in QML.
    // Date is omitted since not particularly useful within the app
    QStringList toAdd;
    toAdd << time << messageType << message;
    emit logger()->newLogForGUI(toAdd);

}

void Logger::logToFile(QString message)
{
    QFile logFile(logger()->mLogFilePath);
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream ts(&logFile);
        ts << message;
    }
    else {
        QByteArray path = logger()->mLogFilePath.toLocal8Bit();
        fprintf(stderr, "Could not open log file for writing at %s", path.constData());
        fflush(stderr);
    }
}

void Logger::logToTerminal(QString message)
{
    QByteArray b = message.toLocal8Bit();
    fprintf(stdout, b.constData());
    fflush(stdout); // Force output to be printed right away
}
