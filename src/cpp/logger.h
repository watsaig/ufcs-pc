#ifndef LOGGER_H
#define LOGGER_H
#include <QObject>
#include <QtCore>

class Logger : public QObject
{
    Q_OBJECT

public:
    static Logger* logger();
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

signals:
    void newLogForGUI(QStringList message);
    void newLogForFile(QString message);

private slots:
    void logToFile(QString message);
    void logToTerminal(QString message);

private:
    Logger();
    QString mLogFilePath;
};

#endif // LOGGER_H
