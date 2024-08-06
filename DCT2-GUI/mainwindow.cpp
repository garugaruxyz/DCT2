#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QToolTip>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    lastPath = QDir("/");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selectImageButton_clicked()
{
    QString filter = "Images (*.bmp)";
    QString filePath = QFileDialog::getOpenFileName(this, "Select image", lastPath.path(), filter);

    if (filePath == "")
        return;

    setImagePreview(filePath);
    ui->filePathLineEdit->setText(filePath);
    lastPath = getPath(filePath);
}

void MainWindow::on_runButton_clicked()
{
    ui->resultPreviewLabel->clear();

    QString lineEditContent = ui->filePathLineEdit->text();
    if (lineEditContent.isNull() || lineEditContent.isEmpty())
    {
        QToolTip::showText(ui->filePathLineEdit->mapToGlobal(QPoint()), "Source image not set.", ui->filePathLineEdit);
        qDebug("Source image not set.");
        return;
    }
    dct = DiscreteCosineTransform::DCT2(lineEditContent);

    lineEditContent = ui->blockSizeLineEdit->text();
    if (lineEditContent.isNull() || lineEditContent.isEmpty())
    {
        QToolTip::showText(ui->blockSizeLabel->mapToGlobal(QPoint()), "Block size not set.", ui->blockSizeLabel);
        qDebug("Block size not set.");
        return;
    }
    int F = lineEditContent.toInt();
    if (F <= 0)
    {
        QToolTip::showText(ui->blockSizeLabel->mapToGlobal(QPoint()), "Block size must be greater than 0.", ui->blockSizeLabel);
        qDebug("Block size must be greater than 0.");
        return;
    }
    if (F > dct.getOriginalHeight() || F > dct.getOriginalWidth())
    {
        QToolTip::showText(ui->blockSizeLabel->mapToGlobal(QPoint()), "Block size must be smaller than the image width and height.", ui->blockSizeLabel);
        qDebug("Block size must be smaller than the image width and height.");
        return;
    }

    lineEditContent = ui->thresholdLineEdit->text();
    if (lineEditContent.isNull() || lineEditContent.isEmpty())
    {
        QToolTip::showText(ui->thresholdLineEdit->mapToGlobal(QPoint()), "Threshold not set.", ui->thresholdLineEdit);
        qDebug("Threshold not set.");
        return;
    }
    int d = lineEditContent.toInt();
    int upperLimit = 2 * F - 2;
    if (d < 0 || d > upperLimit)
    {
        QString message = QString("Threshold must be lower than %1 and not lower than 0.").arg(upperLimit);
        QToolTip::showText(ui->thresholdLineEdit->mapToGlobal(QPoint()), message, ui->thresholdLineEdit);
        qDebug("Threshold out of range.");
        return;
    }

    performDCT(F, d);

    QPixmap resultImage(dct.getResultFilePath().c_str());
    setPixmap(ui->resultPreviewLabel, resultImage);
}

void MainWindow::on_filePathLineEdit_editingFinished()
{
    QString filePath = ui->filePathLineEdit->text();
    if (filePath.isNull() || filePath.isEmpty())
        return;

    setImagePreview(filePath);
    ui->filePathLineEdit->setText(filePath);
    lastPath = getPath(filePath);
}

void MainWindow::setImagePreview(const QString& path)
{
    QPixmap pixmap(path);
    if (pixmap.isNull())
    {
        QToolTip::showText(ui->filePathLineEdit->mapToGlobal(QPoint()), "Could not load image.", ui->filePathLineEdit);
        qDebug("Pixmap creation failed.");
        return;
    }

    setPixmap(ui->imagePreviewLabel, pixmap);
}

void MainWindow::setPixmap(QLabel* label, QPixmap pixmap)
{
    int imageWidth = label->width();
    int imageHeight = label->height();
    label->setPixmap(pixmap.scaled(imageWidth, imageHeight, Qt::KeepAspectRatio));
}

void MainWindow::performDCT(const int F, const int d)
{
    dct.reset();
    dct.setBlockSize(F);
    dct.setThreshold(d);
    dct.performDCT2();
    dct.performIDCT2();
}

QDir getPath(const QString& filePath)
{
    QFileInfo file(filePath);
    return file.absoluteDir();
}
