#ifndef APPLICATION_H
#define APPLICATION_H

#include <QHash>
#include <QStringList>
#include <QTranslator>
#include <qtsingleapplication.h>

class QDir;
class QTranslator;

typedef QHash<QString, QTranslator*> Translators;

class Application : public QtSingleApplication {
    Q_OBJECT

public:
    explicit Application(int& argc, char* argv[]);
    ~Application();

    static void loadTranslations(const QString& dir);
    static void loadTranslations(const QDir& dir);
    static const QStringList availableLanguages();

public slots:
    static void setLanguage(const QString& locale);

private:
    static QTranslator* current;
    static Translators translators;
};

#endif // APPLICATION_H
