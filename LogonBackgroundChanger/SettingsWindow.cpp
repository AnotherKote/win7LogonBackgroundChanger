#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QFileDialog>
#include <QListWidget>
#include <QDir>
#include <QMessageBox>

#include "SettingsWindow.hpp"
#include <QDebug>
SettingsWindow::SettingsWindow(QWidget *parent)
: QWidget(parent)
, m_pcurrentDir(nullptr)
, m_pbrowseButton(nullptr)
, m_paddDir(nullptr)
, m_psave(nullptr)
, m_pcancel(nullptr)
, m_pdirectoryList(nullptr)
{
    this->setWindowFlags(Qt::Dialog);
    ///<@todo QLabel (paths to pictures);
    m_pcurrentDir = new QLineEdit(this);
    m_pbrowseButton = new QPushButton(this); ///<@todo set folder icon for this button
    m_paddDir = new QPushButton(this);
    m_psave = new QPushButton(this);
    m_pcancel = new QPushButton(this);
    m_pdirectoryList = new QListWidget(this);

    m_paddDir->setText("&Add");
    m_psave->setText("&Save");
    m_pcancel->setText("&Cancel");

    connect(m_pcurrentDir, SIGNAL(textChanged(QString)), this, SLOT(setAddButtonEnabled(QString)));
    connect(m_pbrowseButton, SIGNAL(clicked(bool)), this, SLOT(fileDialogOpen()));
    connect(m_paddDir, SIGNAL(clicked(bool)), this, SLOT(addDirToList()));
    connect(m_psave, SIGNAL(clicked(bool)), this, SLOT(saveSettings()));
    connect(m_pcancel, SIGNAL(clicked(bool)), this, SLOT(close()));

    QHBoxLayout *browseLayout = new QHBoxLayout();
    browseLayout->addWidget(m_pcurrentDir);
    browseLayout->addWidget(m_pbrowseButton);
    browseLayout->addWidget(m_paddDir);

    QHBoxLayout *cancelSaveLayout = new QHBoxLayout();
    cancelSaveLayout->addWidget(m_pcancel);
    cancelSaveLayout->addStretch(1);
    cancelSaveLayout->addWidget(m_psave);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(browseLayout);
    mainLayout->addWidget(m_pdirectoryList);
    mainLayout->addLayout(cancelSaveLayout);

    QSize winSize(400, 200);
    this->setFixedSize(winSize);
}

void SettingsWindow::fileDialogOpen()
{
    m_pcurrentDir->setText(QFileDialog::getExistingDirectory());
}

void SettingsWindow::addDirToList()
{
    QDir curDir(m_pcurrentDir->text());
    if(curDir.exists()){
        if(m_pdirectoryList->findItems(m_pcurrentDir->text(), Qt::MatchFixedString).isEmpty()){
            m_pdirectoryList->addItem(m_pcurrentDir->text());
        } else {
            QMessageBox::information(this, "Info", "Already in list.");
        }
    } else {
        QMessageBox::warning(this, "Path error", "Path doesn't exists!");
    }
}

void SettingsWindow::setAddButtonEnabled(QString currentDir)
{
    m_paddDir->setDisabled(currentDir.isEmpty());
}

void SettingsWindow::saveSettings()
{

}
