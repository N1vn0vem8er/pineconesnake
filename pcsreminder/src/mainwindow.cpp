#include "createeventdialog.h"
#include "mainwindow.h"
#include "repeatedeventwidget.h"
#include "resourcesmanager.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->addReminderButton, &QPushButton::clicked, this, &MainWindow::addReminderPressed);
    refresh();
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
    ResourcesManager* rm = ResourcesManager::getInstance();
    events = rm->getAllEvents();
    repeatedEvents = rm->getAllRepeating();
    for(const auto& i : std::as_const(repeatedEvents))
    {
        RepeatedEventWidget* widget = new RepeatedEventWidget(i, this);
        layout->addWidget(widget);
        widgets.append(widget);
    }
    layout->addStretch();
    ui->scrollAreaWidgetContents->setLayout(layout);
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
    emit requestRefresh();
}

void MainWindow::addRepeating(const EventManager::RepeatedEvent &event)
{
    ResourcesManager::getInstance()->saveRepeating(event);
    refresh();
    emit requestRefresh();
}
