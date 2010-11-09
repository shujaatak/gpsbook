#include "dialogselectsymbol.h"
#include "ui_dialogselectsymbol.h"
#include <QDir>
#include <QFileInfo>

DialogSelectSymbol::DialogSelectSymbol(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSelectSymbol)
{
    ui->setupUi(this);
}

DialogSelectSymbol::~DialogSelectSymbol()
{
    delete ui;
}

QString DialogSelectSymbol::getSelectedSymbolName()
{
    return ui->listWidget->currentItem()->text();
}

void  DialogSelectSymbol::setSelectedIcon(QString name){
    if (QFileInfo( ":/icons/icons/" + name + ".png" ).exists()) {
        source = "icons";
    }
    else if (QFileInfo( ":/icons/google-marker/" +  name + ".png" ).exists()) {
        source = "google-marker";
    }
    else if (QFileInfo( ":/icons/numeric/" +  name + ".png" ).exists()) {
        source = "numeric";
    }
    else if (QFileInfo( ":/icons/geocaching/" +  name + ".png" ).exists()) {
        source = "geocaching";
    }
    else
    {
        source = "icons";
        name   = "cluster4";
    }

    iconName = name;
    on_comboBox_currentIndexChanged(source);
}



void DialogSelectSymbol::on_comboBox_currentIndexChanged(QString text)
{
    QListWidgetItem* itemSelected = NULL;
    ui->comboBox->setCurrentIndex(ui->comboBox->findText(text));
    ui->listWidget->clear();
    source = text;
    QDir resource(":/icons/"+source);
    QStringList fileList = resource.entryList();
    for(int i=0; i<fileList.count(); i++)
    {
        QString str = fileList.at(i);
        QListWidgetItem* item = new QListWidgetItem();
        item->setIcon(QIcon(QString(":/icons/"+source+"/").append(str)));
        item->setText(QFileInfo(str).baseName());
        if ( (QFileInfo(str).baseName() == iconName) ){
            itemSelected = item;
        }
        ui->listWidget->addItem(item);
    }
    if (itemSelected != NULL){
        ui->listWidget->setCurrentItem(itemSelected);
        itemSelected->setSelected(true);
        ui->listWidget->scrollToItem(itemSelected);
    }
    else {
        ui->listWidget->scrollToTop();
    }
}
