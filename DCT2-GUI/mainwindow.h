#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QLabel>
#include "DiscreteCosineTransform.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void on_selectImageButton_clicked();

        void on_runButton_clicked();

        void on_filePathLineEdit_editingFinished();

    private:
        void setImagePreview(const QString& path);
        void setPixmap(QLabel* label, QPixmap pixmap);
        void performDCT(int F, int d);
        Ui::MainWindow *ui;
        QDir lastPath;
        DiscreteCosineTransform::DCT2 dct;
};

QDir getPath(const QString& filePath);

#endif // MAINWINDOW_H
