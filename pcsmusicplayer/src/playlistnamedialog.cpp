#include "playlistnamedialog.h"
#include "ui_playlistnamedialog.h"

PlaylistNameDialog::PlaylistNameDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PlaylistNameDialog)
{
    ui->setupUi(this);
    connect(this, &QDialog::accepted, this, [&]{emit playlistName(ui->lineEdit->text());});
}

PlaylistNameDialog::~PlaylistNameDialog()
{
    delete ui;
}

QString PlaylistNameDialog::getName() const
{
    return ui->lineEdit->text();
}
