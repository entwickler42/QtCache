#ifndef QTCACHETOOLSETTINGS_H
#define QTCACHETOOLSETTINGS_H

#include <QDir>
#include <QString>
#include <QSettings>

class QtCacheToolSettings
        : public QObject
{
    Q_OBJECT

public:
    QtCacheToolSettings(QObject* parent = 0, const QString& filename = "QtCacheTool.ini");
    ~QtCacheToolSettings();

    QSettings* config() const
    {
        return conf;
    }

    QString preferedUCI() const
    {
        return conf->value("PreferedUCI").toString();
    }

    void setPreferedUCI(const QString& value) const
    {
        conf->setValue("PreferedUCI", value);
    }

    QString QSPEC() const
    {
        return conf->value("QSPEC", "cfkb").toString();
    }

    void setQSPEC(const QString& value)
    {
        return conf->setValue("QSPEC", value);
    }

    QString postImportHook() const
    {
        return conf->value("PostImportHook").toString();
    }

    void setPostImportHook(const QString& value)
    {
        conf->setValue("PostImportHook", value);
    }

    QString preImportHook() const
    {
        return conf->value("PreImportHook").toString();
    }

    void setPreImportHook(const QString& value)
    {
        conf->setValue("PreImportHook", value);
    }

    QString server() const
    {
        return conf->value("Server").toString();
    }

    void setServer(const QString& value)
    {
        conf->setValue("Server", value);
    }


    QString port() const
    {
        return conf->value("Port").toString();
    }

    void setPort(const QString& value)
    {
        conf->setValue("Port", value);
    }

    QString username() const
    {
        return conf->value("user").toString();
    }

    QString password() const
    {
        return conf->value("passwd").toString();
    }

    bool autoConnect() const
    {
        return conf->value("autoConnect", true).toBool();
    }

    void setAutoConnect(bool value)
    {
        conf->setValue("autoConnect", value);
    }

    bool compile() const
    {
        return conf->value("compile", false).toBool();
    }

    void setCompile(bool value)
    {
        conf->setValue("compile", value);
    }

    bool compileEarly() const
    {
        return conf->value("compileEarly", false).toBool();
    }

    void setCompileEarly(bool value)
    {
        conf->setValue("compileEarly", value);
    }

    QString defaultExportDirectory() const
    {
        return conf->value("DefaultExportDirectory", QDir::currentPath()).toString();
    }

    void setDefaultExportDirectory(const QString& value)
    {
        conf->setValue("DefaultExportDirectory", value);
    }

private:
    QSettings* conf;
};

#endif // QTCACHETOOLSETTINGS_H
