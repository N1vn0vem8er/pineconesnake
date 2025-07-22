#include "addnewdialog.h"
#include "settings.h"
#include "ui_addnewdialog.h"

#include <QUuid>
#include <QFileDialog>

AddNewDialog::AddNewDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddNewDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &AddNewDialog::openSelectImage);
}

AddNewDialog::~AddNewDialog()
{
    delete ui;
}

Item AddNewDialog::getItem() const
{
    QString imageName = "";
    QString tmpImagePath = ui->imageLine->text();
    if(!tmpImagePath.isEmpty() && QFileInfo::exists(tmpImagePath))
    {
        QImage image;
        image.load(tmpImagePath);
        if(!image.isNull())
        {
            image = image.scaled(QSize(150, 200));
            imageName = QUuid().createUuid().toString();
            image.save(Settings::imagesPath + "/" + imageName, "JPG");
        }
    }
    Item item(-1, ui->titleLine->text(), ui->description->toPlainText(), 0, imageName, ui->linkLine->text(), 0, ui->chaptersLine->text().toInt());
    return item;
}

void AddNewDialog::openSelectImage()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Select Image"), QDir::homePath(), tr("Images (*.png *.jpg)"));
    if(!path.isEmpty())
    {
        ui->imageLine->setText(path);
    }
}
