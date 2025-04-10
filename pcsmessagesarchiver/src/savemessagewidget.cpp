#include "savemessagewidget.h"
#include "globals.h"
#include "qdatetime.h"
#include "resourcesmanager.h"
#include "ui_savemessagewidget.h"

#include <QMessageBox>

SaveMessageWidget::SaveMessageWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SaveMessageWidget)
{
    ui->setupUi(this);
    connect(ui->writer, &Writer::save, this, &SaveMessageWidget::saveMessage);
}

SaveMessageWidget::~SaveMessageWidget()
{
    delete ui;
}

void SaveMessageWidget::saveMessage()
{
    ResourcesManager* rm = ResourcesManager::getInstance();
    auto message = ui->writer->getMessage();
    try
    {
        rm->saveMessage(message);
    }
    catch(std::exception e)
    {
        QMessageBox::critical(this, tr("Error"), tr("Couldn't save message"));
    }
}
