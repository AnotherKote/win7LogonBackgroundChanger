#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>

class QLineEdit;
class QPushButton;
class QListWidget;
class SettingsWindow : public QWidget
{
    Q_OBJECT

private:
    QLineEdit *m_pcurrentDir;
    QPushButton *m_pbrowseButton;
    QPushButton *m_paddDir;
    QPushButton *m_psave;
    QPushButton *m_pcancel;
    QListWidget *m_pdirectoryList;
public:
    explicit SettingsWindow(QWidget *parent = 0);

signals:

public slots:
    void fileDialogOpen();
    void addDirToList();
    void setAddButtonEnabled(QString currentDir);
    void saveSettings();
};

#endif // SETTINGSWINDOW_H
