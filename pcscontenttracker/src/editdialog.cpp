#include "editdialog.h"
#include "qdir.h"
#include "qfileinfo.h"
#include "quuid.h"
#include "settings.h"
#include "ui_editdialog.h"

#include <QFileDialog>

EditDialog::EditDialog(Item item, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditDialog)
{
    ui->setupUi(this);
    this->item = item;
    ui->titleLine->setText(item.title);
    ui->description->setPlainText(item.description);
    ui->chaptersLine->setText(QString::number(item.chapters));
    ui->currentChapterLine->setText(QString::number(item.chapter));
    ui->linkLine->setText(item.link);
    ui->imageLine->setText(item.image);
    connect(ui->selectButton, &QPushButton::clicked, this, &EditDialog::openSelectImage);
}

EditDialog::~EditDialog()
{
    delete ui;
}

Item EditDialog::getIten() const
{

    QString tmpImagePath = ui->imageLine->text();
    QString imageName = item.image;
    if(tmpImagePath != item.image && !tmpImagePath.isEmpty() && QFileInfo(tmpImagePath).exists())
    {
        if(!item.image.isEmpty() && tmpImagePath != item.image && QFileInfo(Settings::imagesPath + "/" + item.image).exists())
        {
            QFile file(Settings::imagesPath + "/" + item.image);
            file.remove();
        }
        QImage image;
        image.load(tmpImagePath);
        if(!image.isNull())
        {
            image = image.scaled(QSize(150, 200));
            imageName = QUuid().createUuid().toString();
            image.save(Settings::imagesPath + "/" + imageName, "JPG");
        }
    }
    return Item(item.id, ui->titleLine->text(), ui->description->toPlainText(), 0, imageName, ui->linkLine->text(), 0, ui->chaptersLine->text().toInt());
}

void EditDialog::openSelectImage()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Select Image"), QDir::homePath(), tr("Images (*.png *.jpg)"));
    if(!path.isEmpty())
    {
        ui->imageLine->setText(path);
    }
}
