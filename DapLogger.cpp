#include "DapLogger.h"

DapLogger::DapLogger(QObject *parent, size_t prefix_width)
    : QObject(parent) {
    dap_set_log_tag_width(prefix_width);
    qInstallMessageHandler(messageHandler);
}

inline log_level DapLogger::castQtMsgToDap(QtMsgType type) {
    switch (type) {
    case QtDebugMsg:
        return log_level::L_DEBUG;
    case QtInfoMsg:
        return log_level::L_INFO;
    case QtWarningMsg:
        return log_level::L_WARNING;
    case QtCriticalMsg:
        return log_level::L_ERROR;
    case QtFatalMsg: // Qt FatalMsg as L'CRITICAL interrupt the program by abort() function
        return log_level::L_CRITICAL;
    }
}

bool DapLogger::setLogFile(const QString& filePath) {
    return dap_common_init(filePath.toLatin1().data()) == 0;
}

void DapLogger::messageHandler(QtMsgType type,
                               const QMessageLogContext &ctx,
                               const QString & msg) {
    static char tmpBuf[56];
    strcpy(tmpBuf, strrchr(ctx.file, '/') + 1);

    // delete file extension (.cpp)
    *strrchr(tmpBuf, '.') = '\0';

    _log_it(tmpBuf, castQtMsgToDap(type), msg.toLatin1().data());
}
