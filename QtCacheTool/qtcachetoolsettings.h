#ifndef QTCACHETOOLSETTINGS_H
#define QTCACHETOOLSETTINGS_H

#include <QObject>
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
        return conf->value("QSPEC", "cfk").toString();
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

private:
    QSettings* conf;
};

#endif // QTCACHETOOLSETTINGS_H
