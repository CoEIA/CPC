#include "utilities.h"
#include "fileoperations.h"
#include "constants.h"

#include <QSysInfo>
#include <QDebug>
#include <QDir>
#include <QList>
#include <QProcess>
#include <QApplication>
#include <windows.h>
#include <QMessageBox>

Utilities::Utilities()
{
}

void Utilities::addScheduleInStartup() {
    QString name = "CPC-SCHEDULE";
    QString path = "\"" + QDir::toNativeSeparators(qApp->applicationDirPath() + "\\" + name + ".exe" ) + "\"";

    addToStartup(name, path);
}

void Utilities::lunchScheduleNow() {
    QString processName = "CPC-SCHEDULE.exe";

    //QDir dir;
    //QString path = "\"" + QDir::toNativeSeparators(dir.absolutePath() + "\\" + processName) + "\"";

    if ( ! isProcessRun(processName)) {
        lunchProcess(processName, false);
    }
}

void Utilities::addToStartup(QString name, QString path) {
    HMODULE hModule = GetModuleHandle(NULL);
    TCHAR ModulePath[MAX_PATH];
    TCHAR value[MAX_PATH];

    GetModuleFileName(hModule,ModulePath,sizeof(ModulePath));

    int length = path.toWCharArray(value);
    value[length] = '\0';

    HKEY hNewKey ;
    DWORD Result ;
    RegCreateKeyEx(HKLM,STARTUP,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hNewKey,&Result);
    RegSetValueEx(hNewKey,name.toStdWString().c_str(),0,REG_SZ,(BYTE*)value,sizeof(value));
    RegCloseKey(hNewKey);
}

void Utilities:: lunchProcess (QString path, bool wait){
    QProcess *process = new QProcess;

    process->start(path);
    process->waitForStarted();

    if ( wait)
        process->waitForFinished(-1);
}

bool Utilities:: isProcessRun (QString processName){
    QProcess* process = new QProcess;

    process->start(QString("tasklist /FI \"IMAGENAME eq %1\"").arg(processName));
    process->waitForStarted();
    process->waitForFinished();

    QString output = process->readAll();
    delete process;

    if (output.contains("INFO: No") || output.isEmpty())
        return false;

    return true;
}

void Utilities:: lunchUpdate () {
    QString path =  QApplication::applicationFilePath();
    path = path.replace("/", "\\");

    // get application name
    QString name ;
    QStringList paths =  path.split("\\");
    foreach(QString tmp, paths)
        name = tmp;

    QString processName = "CPC-UPDATE-HELPER.exe";
    QString updaterPath = QDir::toNativeSeparators(qApp->applicationDirPath()) + "\\" + processName ;

    // lunch helper application to kill,delete and lunch new process
    QString app = QString(updaterPath + " %1 %2").arg(name, path);

    QProcess *process = new QProcess;
    process->start(app);

    exit(0);
}

QMultiMap<QString, Plugins> Utilities:: readAllPlugins(const QString& pluginsPath) {
    QDir dir(pluginsPath);
    QStringList filters ;
    filters << "*.xml";

    QString osVersion = Utilities::getOSVersion();
    QMultiMap<QString, Plugins> plugins;

    foreach(QString path,dir.entryList(filters, QDir::Files)) {
        QString pluginPath = QFileInfo(dir,path).absoluteFilePath() ;

        Plugins tmp ;
        PluginsParser parser(&tmp,osVersion);
        parser.readFile(pluginPath);

        plugins.insertMulti(tmp.metaData.category.trimmed(), tmp);
    }

    return plugins;
}

QString Utilities:: getOSVersion () {
    QSysInfo info;

    if ( info.windowsVersion() == QSysInfo::WV_XP )
       return "XP" ;
    else if ( info.windowsVersion() == QSysInfo::WV_WINDOWS7)
        return "SEVEN";

    return "XP";
}

QString Utilities:: getCurrentUserName() {
    TCHAR buffer[100];
    DWORD buffer_size = 100;
    GetUserName(buffer,&buffer_size);
    return QString::fromWCharArray(buffer);
}

QStringList Utilities:: handleLocation (QString location) {
    QStringList list;
    QString missingPath = "%FILLPATH%";

    if ( location.contains(missingPath) ) {
        QString loc = replaceVariable(location);

        if ( loc.endsWith(missingPath)) {
            QString fullPath = loc.remove(missingPath);
            list.append(FileOperations::showFolders(fullPath));
        }
        else {
            QString leftPath = loc.split(missingPath)[0];
            QString rightPath = loc.split(missingPath)[1];

            QStringList folders = FileOperations::showFolders(leftPath);
            foreach(QString folder, folders){
                QFileInfo file(folder + rightPath);
                if ( file.exists())
                    list.append(file.absoluteFilePath());
            }
        }
    }

    return list;
}


bool Utilities:: promptForUpdate () {
    QMessageBox msg;
    msg.setText(QObject::tr("New Update Found"));
    msg.setInformativeText(QObject::tr("Do you want up update the software now?"));
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Ok);
    msg.setIcon(QMessageBox::Warning);

    int ret = msg.exec();

    if ( ret == QMessageBox::Ok ) {
        return true;
    }

    return false;
}

bool Utilities:: promptForReport () {
    QMessageBox msg;
    msg.setText(QObject::tr("CPC Deleting Report"));
    msg.setInformativeText(QObject::tr("Do you want to see the HTML Report for Items that deleted by CPC?"));
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Ok);
    msg.setIcon(QMessageBox::Information);

    int ret = msg.exec();

    if ( ret == QMessageBox::Ok ) {
        return true;
    }

    return false;
}

void Utilities:: promptForNoUpdate () {
    QMessageBox msg;
    msg.setText(QObject::tr("Your Software is Up to Date"));
    msg.setInformativeText(QObject::tr("No new update files found"));
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setDefaultButton(QMessageBox::Ok);
    msg.setIcon(QMessageBox::Information);

    msg.exec();
}

void Utilities:: promptForRestore () {
    QMessageBox msg;
    msg.setInformativeText(QObject::tr("Plugins Options is Resetting to the Default Configurations"));
    msg.setText(QObject::tr("Resetting Options Done"));
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setDefaultButton(QMessageBox::Ok);
    msg.setIcon(QMessageBox::Information);

    msg.exec();
}

void Utilities:: promptForFreeVersion(){
    QMessageBox msg;
    msg.setText(QObject::tr("Your Software is not registred"));
    msg.setInformativeText(QObject::tr("Please Purchase new licence to use the full features"));
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setDefaultButton(QMessageBox::Ok);
    msg.setIcon(QMessageBox::Critical);

    msg.exec();
}

void Utilities::promptForErrorUpdate() {
    QMessageBox msg;
    msg.setText(QObject::tr("Internet Connection Problem"));
    msg.setInformativeText(QObject::tr("Please make sure that internet connection is active"));
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setDefaultButton(QMessageBox::Ok);
    msg.setIcon(QMessageBox::Critical);

    msg.exec();
}

QString Utilities:: replaceVariable (const QString& path) {
    int index = path.indexOf("\\");
    QString key = path.mid(index+1);

    QString variable = (path.split("\\")[0]).split("%")[1];
    TCHAR buffer[MAX_PATH];

    GetEnvironmentVariable(variable.toStdWString().c_str(), buffer,MAX_PATH);

    QString result = QString::fromWCharArray(buffer);
    result +=  "\\" + key;

    return (result);
}

void Utilities:: printInDebug (const Plugins& plug) {
    qDebug() << "Metadata:"
             << "\n\t Name: " << plug.metaData.pluginsName
             << "\n\t category: " << plug.metaData.category
             << "\n\t version: " << plug.metaData.pluginsVersion ;

    qDebug() << "Data: ";

    foreach(PluginsItem item, plug.pluginsItems) {
        qDebug() << "\t Item: " << item.name;

        foreach(PluginsObject object, item.pluginsObject) {
            qDebug() << "\t Type: " << object.type ;
            qDebug() << "\t Command: " << object.command ;

            qDebug() << "\t Location: " << object.path.location ;
            qDebug() << "\t OS: " << object.path.os ;
            qDebug() << "\t Patterns: " << object.path.patterns ;

            qDebug() << "";
        }

        qDebug() << "" ;
    }
    qDebug() << "";
}

void Utilities:: loadEnglishStyle() {
    QFile file(":/config/EnglishGUI.qss");
    file.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(StyleSheet);
    file.close();
}

void Utilities:: loadArabicStyle() {
    QFile file(":/config/ArabicGUI.qss");
    file.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(StyleSheet);
    file.close();
}
