#ifndef DIALOGLINKEDITION_H
#define DIALOGLINKEDITION_H

#include <QDialog>
#include <QHash>

namespace Ui {
    class DialogLinkEdition;
}

class DialogLinkEdition : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLinkEdition(QWidget *parent = 0);
    ~DialogLinkEdition();
    void init(QString text, QString url, QString mimetype);
    QString text;
    QString url;
    QString mimetype;

private:
    Ui::DialogLinkEdition *ui;
    QHash<QString,QString>  mimeType;

private slots:
    void on_toolButtonAutoCompleteMimeType_clicked();
    void on_buttonBox_accepted();
};

#endif // DIALOGLINKEDITION_H
