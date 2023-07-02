#include <QGraphicsView>
#include <QApplication>
#include <gamescene.h>
#include "globalvariations.h"
#include "myMainWindow.h"

int main(int argc, char *argv[])
{
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "0");
    QApplication a(argc, argv);

    QSize screen = QGuiApplication::primaryScreen()->size();

    QFile fontFile(":/font/font/Saira/static/Saira_Expanded-Light.ttf");
    if (fontFile.open(QIODevice::ReadOnly)) {
        QByteArray fontData = fontFile.readAll();
        fontFile.close();
        int fontId = QFontDatabase::addApplicationFontFromData(fontData);
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        if (fontFamilies.size() > 0) {
            DefaultFont.setFamily(fontFamilies.at(0));
        }
    }
    DefaultFont.setPointSize(30);

    SCREEN_WIDTH = screen.width();
    SCREEN_HEIGHT = screen.height();
    rootdir = QCoreApplication::applicationDirPath();

    bool flag = 0; int pos = 0;
    for(int i = rootdir.length() - 1; i; i --){
        if(rootdir[i] == '/' && !flag){
            flag = 1;
        }
        else if(rootdir[i] == '/'){
            rootdir = rootdir.left(i);
            break;
        }
    }

    qDebug() << "ROOT: " << rootdir;

    MyMainWindow mainWindow(&a);
    mainWindow.showFullScreen();

    return a.exec();
}
