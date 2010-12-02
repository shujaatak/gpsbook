/*************************************************************************
*   Copyright (C) 2006 by Christoph Eckert                                *
*   ce@christeck.de                                                       *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#include "MyIOConfigWindow.h"

MyIOConfigWindow::MyIOConfigWindow( QWidget * parent )
    : QDialog( parent )
{
    setupUi( this );

    setupConnections();
}


void MyIOConfigWindow::transmitValues()
{
    cerr << "transmitting values..." << endl;
}


void MyIOConfigWindow::setupConnections()
{
    connect( ButtonCancel, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( ButtonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( BtnSelectFile, SIGNAL( clicked() ), this, SLOT(selectFile() ) );
    connect( ComboBoxType, SIGNAL( editTextChanged( QString ) ), this, SLOT( fillOptionsCombo( QString ) ) );

    connect( ComboBoxOptions, SIGNAL( highlighted( QString ) ), this, SLOT( preserveOptionsEdit() ) );
    connect( ComboBoxOptions, SIGNAL( activated( QString ) ), this, SLOT( updateOptionsEdit( QString ) ) );

    // Attention: Do *not* catch editTextChanged as it also gets emitted when setting the edit text programmatically
    // This would cause a recursion
    // connect( ComboBoxOptions, SIGNAL( editTextChanged( QString ) ), this, SLOT( preserveOptionsEdit() ) );

    connect( ComboBoxOptions, SIGNAL( editTextChanged( QString ) ), this, SLOT( setupHelp() ) );
    connect( ComboBoxCharSet, SIGNAL( editTextChanged( QString ) ), this, SLOT( setupHelp() ) );
}


void MyIOConfigWindow::updateOptionsEdit( QString Selection )
{
    // If the user has chosen an item from the option combo list, don't overwrite the current text
    // in the lineedit but append the selected text to the current contents
    if ( TempOptions != "" && TempOptions.endsWith( "," ) == false )
    {
        TempOptions.append( "," );
    }

    TempOptions.append( Selection );
    ComboBoxOptions->setEditText( TempOptions );

    setupHelp();
}


void MyIOConfigWindow::preserveOptionsEdit()
{
    // If the user has chosen an item from the option combo list, don't overwrite the current text
    // Therefore preserving the text in the edit as soon the user highlights an entry in the combo list
    TempOptions = ComboBoxOptions->currentText();
}


void MyIOConfigWindow::fillOptionsCombo( QString Type )
{
    // Filling the options combo according to the type of this item
    ComboBoxOptions->clear();
    ComboBoxOptions->addItems( SettingsManager::instance()->Options( DataType, Type ) );
    ComboBoxOptions->setEditText( "" );
    // TempOptions = ComboBoxOptions->currentText();
    setupHelp();
}


void MyIOConfigWindow::fillCombos()
{
    // Filling the type combo according to the DataType of this item, preserving the text in the lineedit
    if ( DataType == "Input" || DataType == "Output" )
    {
        // Otherwise the combo should remain empty. Do this for outputs as well
        ComboBoxType->addItems( SettingsManager::instance()->deviceTypes() );
        ComboBoxType->addItems( SettingsManager::instance()->inputTypes() );
        // TODO: Only show USB ports in the combo if one of the USB devices was selected
        // ComboBoxPath->addItems( SettingsManager::instance()->devicePorts() );
        ComboBoxCharSet->addItems( SettingsManager::instance()->characterSets() );
        ComboBoxType->setEditText( "" );
        ComboBoxPath->setEditText( "" );
        ComboBoxCharSet->setEditText( "" );
    }
    else if ( DataType == "Filter" )
    {
        LabelCharSet->hide();
        LabelValue->hide();
        ComboBoxCharSet->hide();
        ComboBoxPath->hide();
        BtnSelectFile->hide();

        ComboBoxType->addItems(  SettingsManager::instance()->filters() );
        ComboBoxType->setEditText( "" );
        ComboBoxPath->setEditText( "" );
    }

    setupHelp();
}


void MyIOConfigWindow::setupHelp()
{
    if ( SettingsManager::instance()->TypeName( DataType, ComboBoxType->currentText() ) == "" )
    {
        if ( DataType == "Input" )
        {
            ComboBoxType->setToolTip( tr( "Edit the type for this input" ) );
            ComboBoxType->setWhatsThis( tr( "Edit the type for this input" ) );
            ComboBoxType->setStatusTip( tr( "Edit the type for this input" ) );
        }
        if ( DataType == "Output" )
        {
            ComboBoxType->setToolTip( tr( "Edit the type for this output" ) );
            ComboBoxType->setWhatsThis( tr( "Edit the type for this output" ) );
            ComboBoxType->setStatusTip( tr( "Edit the type for this output" ) );
        }
        if ( DataType == "Filter" )
        {
            ComboBoxType->setToolTip( tr( "Edit the type for this filter" ) );
            ComboBoxType->setWhatsThis( tr( "Edit the type for this filter" ) );
            ComboBoxType->setStatusTip( tr( "Edit the type for this filter" ) );
        }
    }
    else
    {
        ComboBoxType->setToolTip( SettingsManager::instance()->TypeName( DataType, ComboBoxType->currentText() ) );
        ComboBoxType->setWhatsThis( SettingsManager::instance()->TypeName( DataType, ComboBoxType->currentText() ) );
        ComboBoxType->setStatusTip( SettingsManager::instance()->TypeName( DataType, ComboBoxType->currentText() ) );
    }


    // Setting the tooltip of the options combo.
    if ( SettingsManager::instance()->OptionName( DataType, ComboBoxType->currentText(), ComboBoxOptions->currentText() ) == "" )
    {
        ComboBoxOptions->setToolTip( tr( "If necessary, enter options here" ) );
        ComboBoxOptions->setWhatsThis( tr( "If necessary, enter options here" ) );
        ComboBoxOptions->setStatusTip( tr( "If necessary, enter options here" ) );
    }
    else
    {
        ComboBoxOptions->setToolTip( SettingsManager::instance()->OptionName( DataType, ComboBoxType->currentText(), ComboBoxOptions->currentText() ) );
        ComboBoxOptions->setWhatsThis( SettingsManager::instance()->OptionName( DataType, ComboBoxType->currentText(), ComboBoxOptions->currentText() ) );
        ComboBoxOptions->setStatusTip( SettingsManager::instance()->OptionName( DataType, ComboBoxType->currentText(), ComboBoxOptions->currentText() ) );
    }

    // Setting the tooltip of the charset combo
    if ( SettingsManager::instance()->CharsetName( ComboBoxCharSet->currentText() ) == "" )
    {
        ComboBoxCharSet->setToolTip( tr( "If necessary, enter a character set here" ) );
        ComboBoxCharSet->setWhatsThis( tr( "If necessary, enter a character set here" ) );
        ComboBoxCharSet->setStatusTip( tr( "If necessary, enter a character set here" ) );
    }
    else
    {
        QString TempString = tr( "This character set also is known as:" );
        TempString.append( " " );
        TempString.append( SettingsManager::instance()->CharsetName( ComboBoxCharSet->currentText() ) );
        ComboBoxCharSet->setToolTip( TempString );
        ComboBoxCharSet->setWhatsThis( TempString );
        ComboBoxCharSet->setStatusTip( TempString );
    }


    ComboBoxPath->setToolTip( tr( "Enter the path to a file or port of a device here" ) );
    ComboBoxPath->setWhatsThis( tr( "Enter the path to a file or port of a device here" ) );
    ComboBoxPath->setStatusTip( tr( "Enter the path to a file or port of a device here" ) );

    setHelpText();
    // TODO: Set tooltips for the combo entries if possible?
}


void MyIOConfigWindow::setHelpText()
{
    if ( ComboBoxType->currentText() == "" )
    {
        HelpLabel->setText( "" );
    }
    else if ( ComboBoxOptions->currentText() == "" )
    {
        HelpLabel->setText( ComboBoxType->whatsThis() );
    }
    else
    {
        HelpLabel->setText( ComboBoxOptions->whatsThis() );
    }
}


void MyIOConfigWindow::setDataType( QString Text )
{
    DataType = Text;
    fillCombos();
}


void MyIOConfigWindow::setData( QString Type, QStringList OptionList, QString CharacterSet, QString Value )
{
    // Filling the options combo according to Type, even if it is empty
    fillOptionsCombo( Type );
    // Type: Selecting the passed Type in the combo if it exists
    int Index = ComboBoxType->findText( Type );
    if ( Index == -1 )
    {
        ComboBoxType->setEditText( Type );
    }
    else
    {
        ComboBoxType->setCurrentIndex( Index );
    }
    // Options:
    ComboBoxOptions->setEditText( OptionList.join( "," ) );
    // TempOptions = ComboBoxOptions->currentText();

    // CharSet: Selecting the passed CharSet in the combo if it exists
    Index = ComboBoxCharSet->findText( CharacterSet );
    if ( Index == -1 )
    {
        ComboBoxCharSet->setEditText( CharacterSet );
    }
    else
    {
        ComboBoxCharSet->setCurrentIndex( Index );
    }

    // Value/FilePath: Selecting the passed Value in the combo if it exists
    Index = ComboBoxPath->findText( Value );
    if ( Index == -1 )
    {
        ComboBoxPath->setEditText( Value );
    }
    else
    {
        ComboBoxPath->setCurrentIndex( Index );
    }
}


QString MyIOConfigWindow::Type()
{
    return ComboBoxType->currentText();
}


QString MyIOConfigWindow::Value()
{
    return ComboBoxPath->currentText();
}


QString MyIOConfigWindow::data()
{
    QString Data;
    if ( DataType == "Input" )
    {
        Data.append( " -i " );
    }
    else if ( DataType == "Output" )
    {
        Data.append( " -o " );
    }
    else if ( DataType == "Filter" )
    {
        Data.append( " -x " );
    }

    // The type always needs to be set
    Data.append( ComboBoxType->currentText() );

    if ( ComboBoxOptions->currentText() != "" )
    {
        Data.append( "," );
        Data.append( ComboBoxOptions->currentText() );
    }

    if ( ComboBoxCharSet->currentText() != "" )
    {
        Data.append( " -c " );
        Data.append( ComboBoxCharSet->currentText() );
    }

    if ( ComboBoxPath->currentText() != "" && DataType == "Input" )
    {
        Data.append( " -f " );
        Data.append( ComboBoxPath->currentText() );
        Data.append( " " );
    }
    else if ( ComboBoxPath->currentText() != "" && DataType == "Output" )
    {
        Data.append( " -F " );
        Data.append( ComboBoxPath->currentText() );
        Data.append( " " );
    }

    return Data;
}



void MyIOConfigWindow::selectFile()
{

    QString Path = "";
    QStringList FileTypes = SettingsManager::instance()->fileTypes();
    FileTypes.prepend( tr( "Any filetype" ).append( " ( * )" ) );
    QString Types = FileTypes.join( ";;" );

    // There's another heavily overloaded file select dialog with even more features,
    // but I didn't manage to pass it multiple file filters. Needs further investigation
    if ( DataType == "Input" )
    {
        Path = QFileDialog::getOpenFileName(this, tr("Select Source"), SettingsManager::instance()->pathInput(), Types);
    }
    if ( DataType == "Output" )
    {
        Path = QFileDialog::getSaveFileName(this, tr("Select Destination"), SettingsManager::instance()->pathOutput(), Types);
    }
    if ( DataType == "Filter" )
    {
        Path = QFileDialog::getOpenFileName(this, tr("Select Filter File"), SettingsManager::instance()->pathFilter(), Types);
    }

    if ( Path != "" )
    {
        // In case of a filter dont't setEditText() but append the file path to eventually existing text
        if ( DataType == "Filter" )
        {
            QString TempString = ComboBoxPath->lineEdit()->text();
            TempString.append( "file=" );
            TempString.append( Path );
            ComboBoxPath->setEditText( TempString );
            SettingsManager::instance()->setPathFilter( Path );
        }

        // What about a function which does this job?
        // TODO: In case of input files, autoguess the type according to the input file extension
        else if ( DataType == "Output" )
        {
            // TODO: Append the correct file extension if it hasn't been appended by the user
            // Problem: I have no relation yet between datatypes and extensions
            ComboBoxPath->setEditText( Path );
            SettingsManager::instance()->setPathOutput( Path );
        }
        else if ( DataType == "Input" )
        {
            ComboBoxPath->setEditText( Path );
            SettingsManager::instance()->setPathInput( Path );
        }
    }
}


void MyIOConfigWindow::on_BtnSelectFile_2_clicked()
{
    ComboBoxType->setCurrentIndex(ComboBoxType->findText("gpx"));
    ComboBoxPath->setEditText("gpsbook");
}
