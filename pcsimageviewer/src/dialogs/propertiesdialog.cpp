#include "propertiesdialog.h"
#include "ui_propertiesdialog.h"

#include <QFileInfo>

PropertiesDialog::PropertiesDialog(QString filePath, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PropertiesDialog){
    ui->setupUi(this);
    QFileInfo fileInfo(filePath);
    ui->fileNameLabel->setText(fileInfo.fileName());
    ui->fileSizeLabel->setText(QString::number(fileInfo.size()));
    ui->formatLabel->setText(fileInfo.completeSuffix());
    ui->loactionLabel->setText(fileInfo.absoluteFilePath());
    ui->createdLabel->setText(fileInfo.birthTime().toString());
    ui->lastModifiedLabel->setText(fileInfo.lastModified().toString());
    ui->lastOpenedLabel->setText(fileInfo.lastRead().toString());
}

PropertiesDialog::~PropertiesDialog(){
    delete ui;
}
