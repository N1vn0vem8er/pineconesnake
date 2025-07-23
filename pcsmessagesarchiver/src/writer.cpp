#include "writer.h"
#include <QDateTime>
#include "ui_writer.h"
#include "resourcesmanager.h"
#include <QFileDialog>

Writer::Writer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Writer)
{
    ui->setupUi(this);
    ui->fontcomboBox->addItems(QFontDatabase::families());
    ui->fontcomboBox->setCurrentIndex(ui->fontcomboBox->findText(ui->editor->font().family()));

    for(int i = 1; i < 100; i++){
        ui->fontSizecomboBox->addItem(QString::number(i));
    }
    ui->fontSizecomboBox->setCurrentIndex(ui->fontSizecomboBox->findText(QString::number(ui->editor->font().pointSize())));

    connect(ui->fontcomboBox, &QComboBox::currentIndexChanged, this, &Writer::changeFort);
    connect(ui->fontSizecomboBox, &QComboBox::currentIndexChanged, this, &Writer::changeFort);
    connect(ui->saveButton, &QPushButton::clicked, this, [this]{emit save();});
    connect(ui->saveAsButton, &QPushButton::clicked, this, &Writer::saveToFile);
    connect(ui->undoButton, &QPushButton::clicked, this, &Writer::undo);
    connect(ui->redoButton, &QPushButton::clicked, this, &Writer::redo);
    connect(ui->copyAllButton, &QPushButton::clicked, this, &Writer::copyAll);
    QStringList people;
    const auto contacts =  ResourcesManager::getInstance()->getAllContacts();
    for(const auto& i : contacts)
    {
        people << i.name;
    }
    completer = new QCompleter(people, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->toLine->setCompleter(completer);
    ui->fromLine->setCompleter(completer);
}

void Writer::clear()
{
    ui->editor->clear();
}

void Writer::setTitle(const QString &title)
{
    ui->titleLine->setText(title);
}

void Writer::setFrom(const QString &from)
{
    ui->fromLine->setText(from);
}

void Writer::setTo(const QString &to)
{
    ui->toLine->setText(to);
}

void Writer::setContent(const QString &content)
{
    ui->editor->setPlainText(content);
}

QString Writer::getTitle() const
{
    return ui->titleLine->text();
}

QString Writer::getFrom() const
{
    return ui->fromLine->text();
}

QString Writer::getTo() const
{
    return ui->toLine->text();
}

QString Writer::getContent() const
{
    return ui->editor->toPlainText();
}

Globals::Message Writer::getMessage()
{
    QString from = ui->fromLine->text();
    QString content = ui->editor->toPlainText();
    QString title = ui->titleLine->text();
    QString to = ui->toLine->text();
    QDateTime dt = QDateTime::currentDateTime();
    Globals::Message message(-1, title, content, from, to, dt.toString("yyyy-MM-dd HH:mm:ss"), dt.toString("yyyy-MM-dd HH:mm:ss"));
    return message;
}

Writer::~Writer()
{
    delete ui;
    delete completer;
}

void Writer::changeFort()
{
    ui->editor->setFont(QFont(ui->fontcomboBox->currentText(), ui->fontSizecomboBox->currentText().toInt()));
}

void Writer::saveToFile()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Save to file"), QDir::homePath(), "Files (*.txt)");
    if(!path.isEmpty())
    {
        QFile file(path);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        if(file.isOpen())
        {
            QTextStream stream(&file);
            stream << "Title: " << ui->titleLine->text() << "\nFrom: " << ui->fromLine->text() << "\nTo: " << ui->toLine->text() << "\n\n" << ui->editor->toPlainText();
            file.close();
        }
    }
}

void Writer::undo()
{
    ui->editor->undo();
}

void Writer::redo()
{
    ui->editor->redo();
}

void Writer::copyAll()
{
    ui->editor->selectAll();
    ui->editor->copy();
}
