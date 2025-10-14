#include "mainwindow.h"
#include "codeeditor.h"
#include "previewpage.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QMessageBox>
#include <QSettings>
#include <QWebChannel>

#define VERSION "0.1.0"
#define LICENSELINK "https://www.gnu.org/licenses/gpl-3.0.html"
#define PROJECTLINK "https://github.com/N1vn0vem8er/pineconesnake"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings settings("pcsmarkdowneditor");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
    ui->actionFiles->setChecked(settings.value("filesVisible", false).toBool());
    ui->treeView->setVisible(ui->actionFiles->isChecked());
    ui->actionMarkdown_Preview->setChecked(settings.value("markdownPreviewVisible", true).toBool());
    ui->webEngineView->setVisible(ui->actionMarkdown_Preview->isChecked());
    ui->actionGit->setChecked(settings.value("gitVisible", false).toBool());
    ui->gitWidget->setVisible(ui->actionGit->isChecked());

    ui->splitter->setStretchFactor(2, 3);

    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::closeAllWindows);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->actionOpen_Folder, &QAction::triggered, this, &MainWindow::openDir);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newFile);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    connect(ui->actionUndo, &QAction::triggered, this, &MainWindow::undo);
    connect(ui->actionRedo, &QAction::triggered, this, &MainWindow::redo);
    connect(ui->actionCut, &QAction::triggered, this, &MainWindow::cut);
    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::copy);
    connect(ui->actionPaste, &QAction::triggered, this, &MainWindow::paste);
    connect(ui->actionSelect_All, &QAction::triggered, this, &MainWindow::selectAll);
    connect(ui->actionDelete, &QAction::triggered, this, &MainWindow::deleteText);
    connect(ui->actionDelete_All, &QAction::triggered, this, &MainWindow::deleteAllText);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::tabChanged);
    connect(ui->actionClose, &QAction::triggered, this, [&]{closeTab(ui->tabWidget->currentIndex());});
    connect(ui->actionClose_All, &QAction::triggered, this, [&]{while(ui->tabWidget->count() > 0) closeTab(ui->tabWidget->currentIndex());});
    connect(ui->actionClose_All_But_This, &QAction::triggered, this, [&]{
        for(int i=ui->tabWidget->count()-1; i >= 0;--i)
        {
            if(i != ui->tabWidget->currentIndex())
                closeTab(i);
        }});
    connect(ui->actionFiles, &QAction::triggered, ui->treeView, &FileSystemTree::setVisible);
    connect(ui->actionMarkdown_Preview, &QAction::triggered, ui->webEngineView, &QWidget::setVisible);
    connect(ui->actionFull_Screen, &QAction::triggered, this, [&]{isFullScreen() ? showNormal() : showFullScreen();});
    connect(ui->actionAbout_Qt, &QAction::triggered, this, [&]{QMessageBox::aboutQt(this, tr("About Qt"));});
    connect(ui->actionAbout_Application, &QAction::triggered, this, &MainWindow::showAbout);
    connect(ui->actionGit, &QAction::triggered, ui->gitWidget, &QWidget::setVisible);
    connect(ui->treeView, &FileSystemTree::openFile, this, &MainWindow::open);
    connect(ui->actionIncrease_Font_Size, &QAction::triggered, this, &MainWindow::increaseFontSize);
    connect(ui->actionDecrease_Font_Size, &QAction::triggered, this, &MainWindow::decreaseFontSize);
    connect(ui->actionSet_Font, &QAction::triggered, this, &MainWindow::onSetFont);
    connect(ui->gitWidget, &GitWidget::openInEditor, this, &MainWindow::openInEditor);
    connect(ui->gitWidget, &GitWidget::addTab, this, [&](QWidget* widget, const QString& title){ui->tabWidget->addTab(widget, title);});
    connect(ui->gitWidget, &GitWidget::openFile, this, &MainWindow::open);
    page = new PreviewPage(this);
    connect(page, &PreviewPage::openedFile, this, &MainWindow::openFileInPreview);
    ui->webEngineView->setPage(page);
    QWebChannel* channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("content"), &document);
    page->setWebChannel(channel);
    ui->webEngineView->setUrl(QUrl("qrc:pages/index.html"));
}

MainWindow::~MainWindow()
{
    QSettings settings("pcsmarkdowneditor");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    settings.setValue("filesVisible", ui->actionFiles->isChecked());
    settings.setValue("markdownPreviewVisible", ui->actionMarkdown_Preview->isChecked());
    settings.setValue("gitVisible", ui->actionGit->isChecked());
    delete ui;
}

CodeEditor *MainWindow::createEditor(const QString &path, const QString &content)
{
    CodeEditor* editor = new CodeEditor(ui->tabWidget);
    connect(editor, &CodeEditor::textChanged, this, [this, editor]{document.setText(editor->toPlainText());});
    connect(editor, &CodeEditor::fontSizeChanged, this, &MainWindow::fontSizeChanged);
    editor->setPath(path);
    editor->setPlainText(content);
    return editor;
}

void MainWindow::newFile()
{
    ui->tabWidget->addTab(createEditor("", ""), tr("New File"));
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
}

void MainWindow::openFile()
{
    const QStringList paths = QFileDialog::getOpenFileNames(this, tr("Open"), QDir::homePath(), tr("*.md"));
    for(const QString& path : paths)
    {
        open(path);
    }
}

void MainWindow::open(const QString &path)
{
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        page->setAbsolutePath(path);
        ui->tabWidget->addTab(createEditor(path, file.readAll()), file.fileName());
        file.close();
    }
}

void MainWindow::openDir()
{
    const QString path = QFileDialog::getExistingDirectory(this, tr("Open Dir"), QDir::homePath());
    if(!path.isEmpty())
    {
        ui->treeView->open(path);
        ui->gitWidget->setRepositoryPath(path);
    }
}

void MainWindow::save()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
    {
        if(!editor->getPath().isEmpty())
        {
            QFile file(editor->getPath());
            if(file.open(QIODevice::WriteOnly))
            {
                file.write(editor->toPlainText().toUtf8());
                file.close();
            }
        }
        else
            saveAs();
    }

}

void MainWindow::saveAs()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
    {
        const QString path = QFileDialog::getSaveFileName(this, tr("Save As"), editor->isEnabled() ? QDir::homePath() : QFileInfo(editor->getPath()).absoluteDir().absolutePath());
        if(!path.isEmpty())
        {
            QFile file(path);
            if(file.open(QIODevice::WriteOnly))
            {
                file.write(editor->toPlainText().toUtf8());
                file.close();
                editor->setPath(path);
            }
        }
    }
}

void MainWindow::closeTab(int index)
{
    QWidget* widget = ui->tabWidget->widget(index);
    CodeEditor* editor = qobject_cast<CodeEditor*>(widget);
    if(editor)
    {
        if(!editor->isSaved())
        {
            if(QMessageBox::question(this, tr("Save"), tr("Save file %1?").arg(editor->getName())) == QMessageBox::StandardButton::Yes)
                save();
        }
    }
    ui->tabWidget->removeTab(index);
    widget->deleteLater();
}

void MainWindow::tabChanged()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
    {
        document.setText(editor->toPlainText());
        fontSizeChanged(editor->font().pointSize());
        page->setAbsolutePath(editor->getPath());
    }
}

void MainWindow::undo()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
        editor->undo();
}

void MainWindow::redo()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
        editor->redo();
}

void MainWindow::cut()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
        editor->cut();
}

void MainWindow::copy()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
        editor->copy();
}

void MainWindow::paste()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
        editor->paste();
}

void MainWindow::deleteText()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
        editor->deleteSelected();
}

void MainWindow::deleteAllText()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
        editor->deleteAll();
}

void MainWindow::selectAll()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
        editor->selectAll();
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, tr("About Application"), tr(R"(
    <html>
        <body>
            <h3>PCS Markdown Editor</h3>
            <p>PCS Markdown Editor is a simple markdown editor with preview. It is a part of the <a href="%1">Pinecone Snake</a> project.</p>
            <p>Version: %2</p>
            <p>License: <a href="%3">GPL 3</a></p>
        </body>
    </html>
)").arg(PROJECTLINK, VERSION, LICENSELINK));
}

void MainWindow::increaseFontSize()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
        editor->increaseFontSize();
}

void MainWindow::decreaseFontSize()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
        editor->decreaseFontSize();
}

void MainWindow::onSetFont()
{
    CodeEditor* editor = qobject_cast<CodeEditor*>(ui->tabWidget->currentWidget());
    if(editor)
    {
        bool ok;
        const QFont font = QFontDialog::getFont(&ok, editor->font(), this);
        if(ok)
            editor->setFont(font);
    }
}

void MainWindow::fontSizeChanged(int size)
{
    ui->statusbar->showMessage(tr("Font Size: %1").arg(size), 3000);
}

void MainWindow::openFileInPreview(const QString &path)
{
    QFile file (path);
    if(file.open(QIODevice::ReadOnly))
    {
        document.setText(file.readAll());
        file.close();
    }
}

void MainWindow::openInEditor(const QString &text, const QString &title, bool readOnly, bool spellChecking, bool disableSaveWarning)
{
    CodeEditor* editor = createEditor("", text);
    editor->setReadOnly(readOnly);
    ui->tabWidget->addTab(editor, title);
}
