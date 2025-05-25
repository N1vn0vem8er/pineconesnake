#include "createeventdialog.h"
#include "mainwindow.h"
#include "resourcesmanager.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->addReminderButton, &QPushButton::clicked, this, &MainWindow::addReminderPressed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refresh()
{
    if(layout!=nullptr)
    {
        delete layout;
    }
    layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
}

void MainWindow::addReminderPressed()
{
    CreateEventDialog* dialog = new CreateEventDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &CreateEventDialog::addEvent, this, &MainWindow::addReminder);
    connect(dialog, &CreateEventDialog::addRepeating, this, &MainWindow::addRepeating);
    dialog->show();
}

void MainWindow::addReminder(const EventManager::Event &event)
{
    ResourcesManager::getInstance()->saveEvent(event);
    refresh();
}

void MainWindow::addRepeating(const EventManager::RepeatedEvent &event)
{
    ResourcesManager::getInstance()->saveRepeating(event);
    refresh();
}
