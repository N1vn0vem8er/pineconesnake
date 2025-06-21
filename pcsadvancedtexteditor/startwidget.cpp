#include "startwidget.h"
#include "ui_startwidget.h"

StartWidget::StartWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StartWidget)
{
    ui->setupUi(this);
    connect(ui->newFileButton, &QPushButton::clicked, this, [&]{emit newFile();});
    connect(ui->openFileButton, &QPushButton::clicked, this, [&]{emit openFile();});
    connect(ui->openDirButton, &QPushButton::clicked, this, [&]{emit openDir();});
}

StartWidget::~StartWidget()
{
    delete ui;
}
