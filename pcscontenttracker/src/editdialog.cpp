#include "editdialog.h"
#include "ui_editdialog.h"

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
}

EditDialog::~EditDialog()
{
    delete ui;
}

Item EditDialog::getIten() const
{
    return Item(item.id, ui->titleLine->text(), ui->description->toPlainText(), item.status, ui->imageLine->text(), ui->linkLine->text(), ui->currentChapterLine->text().toInt(), ui->chaptersLine->text().toInt());
}
