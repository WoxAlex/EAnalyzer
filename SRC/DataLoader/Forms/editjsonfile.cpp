#include "editjsonfile.h"
#include "ui_editjsonfile.h"
#include <QStandardItemModel>
#include <QResizeEvent>
#include <QMessageBox>
#include <QFileDialog>


EditJsonFile::EditJsonFile(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditJsonFile)
{
    ui->setupUi(this);
//    setupUi(this);
    connect(this->ui->pushButtonAddCandidate, SIGNAL(clicked()),
            SLOT(AddCandidate()));
    connect(this->ui->tableWidgetCandidates, SIGNAL(itemChanged(QTableWidgetItem*)),
            SLOT(DeleteLine( QTableWidgetItem* )));
    connect(this->ui->pushButtonAddResult, SIGNAL(clicked()),
            SLOT(AddData()));
    connect(this->ui->tableWidgetElectionResult, SIGNAL(itemChanged(QTableWidgetItem*)),
            SLOT(DeleteDataLine( QTableWidgetItem* )));
    connect(this->ui->comboBoxElectionType, SIGNAL(currentIndexChanged(int)),
            SLOT(SwitchElectionType( int )));

    connect(this->ui->actionSave_it, SIGNAL(triggered()),
            SLOT(SaveJson()));
    connect(this->ui->actionOpen_json, SIGNAL(triggered()),
            SLOT(OpenJson()));
    ShowElection(election);

}

EditJsonFile::~EditJsonFile()
{
    delete ui;
}

void EditJsonFile::closeEvent(QCloseEvent *event)
{
    QMessageBox box;
    box.setText(QString::fromStdWString(L"Сохранить изменения в JSON"));
    box.setStandardButtons(QMessageBox::Cancel | QMessageBox::Discard | QMessageBox::Save);
    box.setDefaultButton(QMessageBox::Cancel);
    int ret = box.exec();
    if(ret == QMessageBox::Cancel)
    {
        event->ignore();
    }
    if(ret == QMessageBox::Discard)
    {
        event->accept();
    }
    if(ret == QMessageBox::Save)
    {
        if (SaveJson())
            event->accept();
        else
            event->ignore();
    }
}

void EditJsonFile::SetElectionInfo(const EAnalyzer::Election &election)
{
    update_candidate_table = false;
    if (election.info.type == EAnalyzer::ElectionInfo::City)
    {
        ui->comboBoxElectionType->setCurrentIndex(0);
        ui->lineEditElectionCity->setEnabled(true);
        ui->lineEditElectionCity->setText(QString::fromStdWString(election.info.city));
    }
    else
    {
        ui->comboBoxElectionType->setCurrentIndex(1);
        ui->lineEditElectionCity->setEnabled(false);
        ui->lineEditElectionCity->setText(QString::fromStdWString(election.info.city));
    }
    ui->dateEditElectionDate->setDate(election.info.date);
    for(std::vector<EAnalyzer::Person>::const_iterator it = election.info.candidates.begin(),end = election.info.candidates.end(); it != end; ++it)
    {
        int n = ui->tableWidgetCandidates->rowCount();
        ui->tableWidgetCandidates->insertRow(n);
        ui->tableWidgetCandidates->setItem(
                    n,0, new QTableWidgetItem(QString::fromStdWString(it->name)));
        ui->tableWidgetCandidates->setItem(
                    n,1, new QTableWidgetItem(QString::fromStdWString(it->surname)));
        ui->tableWidgetCandidates->setItem(
                    n,2, new QTableWidgetItem(QString::fromStdWString(it->fname)));
    }
    update_candidate_table = true;
}

void EditJsonFile::SetElectionData(const EAnalyzer::Election &election)
{
    update_data_table = false;
    ui->tableWidgetElectionResult->setRowCount(election.result.size() );

    if(election.info.type == EAnalyzer::ElectionInfo::City)
        ui->tableWidgetElectionResult->setColumnCount(13 + election.info.candidates.size());
    else
        ui->tableWidgetElectionResult->setColumnCount(20 + election.info.candidates.size());

    int n = 0;
    for(std::vector<EAnalyzer::ElectionResult>::const_iterator it = election.result.begin(),end = election.result.end(); it != end; ++it, ++n)
    {
        const EAnalyzer::ElectionResult& t = *it;
        int i = 0;


        bool city = false;
        if (election.info.type == EAnalyzer::ElectionInfo::City)
            city = true;

        ui->tableWidgetElectionResult->setItem(
                    n,i++, new QTableWidgetItem(QString::number(t.N_LEC)));
        ui->tableWidgetElectionResult->setItem(
                    n,i++, new QTableWidgetItem(QString::fromStdWString(t.TEC)));
        ui->tableWidgetElectionResult->setItem(
                    n,i++, new QTableWidgetItem(QString::number(t.VotersAtLEC)));
        ui->tableWidgetElectionResult->setItem(
                    n,i++, new QTableWidgetItem(QString::number(t.VotingBallots)));
        if (!city)
            ui->tableWidgetElectionResult->setItem(
                    n,i++, new QTableWidgetItem(QString::number(t.PeopleVotedInAdvance)));
        ui->tableWidgetElectionResult->setItem(
                    n,i++, new QTableWidgetItem(QString::number(t.PeopleVotedIn)));
        ui->tableWidgetElectionResult->setItem(
                    n,i++, new QTableWidgetItem(QString::number(t.PeopleVotedOut)));
        ui->tableWidgetElectionResult->setItem(
                    n,i++, new QTableWidgetItem(QString::number(t.CanceledBallots)));
        ui->tableWidgetElectionResult->setItem(
                    n,i++, new QTableWidgetItem(QString::number(t.BallotsFoundOut)));
        ui->tableWidgetElectionResult->setItem(
                    n,i++, new QTableWidgetItem(QString::number(t.BallotsFoundIn)));
        ui->tableWidgetElectionResult->setItem(
                    n,i++, new QTableWidgetItem(QString::number(t.InvalidBallots)));
        ui->tableWidgetElectionResult->setItem(
                    n,i++, new QTableWidgetItem(QString::number(t.ValidBallots)));
        if(!city)
        {
            ui->tableWidgetElectionResult->setItem(
                    n,i++, new QTableWidgetItem(QString::number(t.AbsenteeBallots)));
            ui->tableWidgetElectionResult->setItem(
                    n,i++, new QTableWidgetItem(QString::number(t.PeopleTookAbsenteeBallots)));
            ui->tableWidgetElectionResult->setItem(
                    n,i++, new QTableWidgetItem(QString::number(t.PeopleVotedWithAbsenteeBallots)));
            ui->tableWidgetElectionResult->setItem(
                    n,i++, new QTableWidgetItem(QString::number(t.CanceledAbsenteeBallots)));
            ui->tableWidgetElectionResult->setItem(
                    n,i++, new QTableWidgetItem(QString::number(t.PeopleTookAbsenteeBallotsFromTEC)));
            ui->tableWidgetElectionResult->setItem(
                    n,i++, new QTableWidgetItem(QString::number(t.AbsenteeBallotsLost)));
        }
        ui->tableWidgetElectionResult->setItem(
                    n,i++, new QTableWidgetItem(QString::number(t.VotingBallotsLost)));
        ui->tableWidgetElectionResult->setItem(
                    n,i++, new QTableWidgetItem(QString::number(t.VotingBallotsUnaccounted)));
        for(std::vector<int>::const_iterator ic = t.candidates.begin(),cend = t.candidates.end(); ic != cend; ++ic)
        {
            ui->tableWidgetElectionResult->setItem(
                        n,i++, new QTableWidgetItem(QString::number(*ic)));
        }
    }
    update_data_table = true;

}

void EditJsonFile::GetElectionInfo(EAnalyzer::Election &election)
{
    if (ui->comboBoxElectionType->currentIndex() == 0)
        election.info.type = EAnalyzer::ElectionInfo::City;
    else
        election.info.type = EAnalyzer::ElectionInfo::Country;

    election.info.city = ui->lineEditElectionCity->text().toStdWString();

    election.info.date = ui->dateEditElectionDate->date();

    election.info.candidates.resize(ui->tableWidgetCandidates->rowCount());

    int i = 0;
    for(std::vector<EAnalyzer::Person>::iterator it = election.info.candidates.begin(),end = election.info.candidates.end(); it != end; ++it, ++i)
    {
        it->name =      ui->tableWidgetCandidates->item(i,0)->text().toStdWString();
        it->surname =   ui->tableWidgetCandidates->item(i,1)->text().toStdWString();
        it->fname =     ui->tableWidgetCandidates->item(i,2)->text().toStdWString();
    }
}

void EditJsonFile::GetElectionData(EAnalyzer::Election &election)
{
    election.result.resize(ui->tableWidgetElectionResult->rowCount());
    bool city = false;
    if (election.info.type == EAnalyzer::ElectionInfo::City)
        city = true;

    int n = 0;
    for(std::vector<EAnalyzer::ElectionResult>::iterator it = election.result.begin(),end = election.result.end(); it != end; ++it, ++n)
    {
        EAnalyzer::ElectionResult& t = *it;
        int i = 0;
        t.N_LEC = ui->tableWidgetElectionResult->item(n,i++)->text().toInt();
        t.TEC = ui->tableWidgetElectionResult->item(n,i++)->text().toStdWString();
        t.VotersAtLEC = ui->tableWidgetElectionResult->item(n,i++)->text().toInt();
        t.VotingBallots = ui->tableWidgetElectionResult->item(n,i++)->text().toInt();
        if (!city)
            t.PeopleVotedInAdvance = ui->tableWidgetElectionResult->item(n,i++)->text().toInt();
        t.PeopleVotedIn = ui->tableWidgetElectionResult->item(n,i++)->text().toInt();
        t.PeopleVotedOut = ui->tableWidgetElectionResult->item(n,i++)->text().toInt();
        t.CanceledBallots = ui->tableWidgetElectionResult->item(n,i++)->text().toInt();
        t.BallotsFoundOut = ui->tableWidgetElectionResult->item(n,i++)->text().toInt();
        t.BallotsFoundIn = ui->tableWidgetElectionResult->item(n,i++)->text().toInt();
        t.InvalidBallots = ui->tableWidgetElectionResult->item(n,i++)->text().toInt();
        t.ValidBallots = ui->tableWidgetElectionResult->item(n,i++)->text().toInt();
        if(!city)
        {
            t.AbsenteeBallots = ui->tableWidgetElectionResult->item(n,i++)->text().toInt();
            t.PeopleTookAbsenteeBallots = ui->tableWidgetElectionResult->item(n,i++)->text().toInt();
            t.PeopleVotedWithAbsenteeBallots = ui->tableWidgetElectionResult->item(n,i++)->text().toInt();
            t.CanceledAbsenteeBallots = ui->tableWidgetElectionResult->item(n,i++)->text().toInt();
            t.PeopleTookAbsenteeBallotsFromTEC = ui->tableWidgetElectionResult->item(n,i++)->text().toInt();
            t.AbsenteeBallotsLost = ui->tableWidgetElectionResult->item(n,i++)->text().toInt();
        }
        t.VotingBallotsLost = ui->tableWidgetElectionResult->item(n,i++)->text().toInt();
        t.VotingBallotsUnaccounted = ui->tableWidgetElectionResult->item(n,i++)->text().toInt();

        for(std::vector<int>::iterator ic = t.candidates.begin(),cend = t.candidates.end(); ic != cend; ++ic)
        {
            *ic = ui->tableWidgetElectionResult->item(n,i++)->text().toInt();
        }
    }
}

void EditJsonFile::ShowElection(const EAnalyzer::Election &election)
{
    ui->tableWidgetCandidates->setRowCount(0);
    ui->tableWidgetElectionResult->setRowCount(0);

    QHeaderView * header = ui->tableWidgetCandidates->horizontalHeader();
    QStandardItemModel* model = new QStandardItemModel( 1, 3, header );
    model->setHeaderData(0,Qt::Horizontal,QVariant(QString::fromStdWString(L"Имя")));
    model->setHeaderData(1,Qt::Horizontal,QVariant(QString::fromStdWString(L"Фамилия")));
    model->setHeaderData(2,Qt::Horizontal,QVariant(QString::fromStdWString(L"Отчество")));
    header->setModel(model);
    header->setResizeMode(QHeaderView::Stretch);
    ui->tableWidgetCandidates->setHorizontalHeader(header);
    SetElectionInfo(election);

    ui->tableWidgetAddCandidate->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidgetAddCandidate->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidgetAddCandidate->setItem(0,0,new QTableWidgetItem(QString::fromStdWString(L"Имя")));
    ui->tableWidgetAddCandidate->setItem(0,1,new QTableWidgetItem(QString::fromStdWString(L"Фамилия")));
    ui->tableWidgetAddCandidate->setItem(0,2,new QTableWidgetItem(QString::fromStdWString(L"Отчество")));

    header = ui->tableWidgetElectionResult->horizontalHeader();
    bool city = true;
    if(election.info.type == EAnalyzer::ElectionInfo::City)
        model = new QStandardItemModel( 1, 13 + election.info.candidates.size(), header );
    else
    {
        model = new QStandardItemModel( 1, 20 + election.info.candidates.size(), header );
        city = false;
    }
    int i = 0;
    model->setHeaderData(i++,Qt::Horizontal,QVariant(QString::fromStdWString(L"УИК")));
    model->setHeaderData(i++,Qt::Horizontal,QVariant(QString::fromStdWString(L"ТИК")));
    model->setHeaderData(i++,Qt::Horizontal,QVariant(QString::fromStdWString(L"VotersAtLEC")));
    model->setHeaderData(i++,Qt::Horizontal,QVariant(QString::fromStdWString(L"VotingBallots")));
    if (!city)
        model->setHeaderData(i++,Qt::Horizontal,QVariant(QString::fromStdWString(L"PeopleVotedInAdvance")));
    model->setHeaderData(i++,Qt::Horizontal,QVariant(QString::fromStdWString(L"PeopleVotedIn")));
    model->setHeaderData(i++,Qt::Horizontal,QVariant(QString::fromStdWString(L"PeopleVotedOut")));
    model->setHeaderData(i++,Qt::Horizontal,QVariant(QString::fromStdWString(L"CanceledBallots")));
    model->setHeaderData(i++,Qt::Horizontal,QVariant(QString::fromStdWString(L"BallotsFoundOut")));
    model->setHeaderData(i++,Qt::Horizontal,QVariant(QString::fromStdWString(L"BallotsFoundIn")));
    model->setHeaderData(i++,Qt::Horizontal,QVariant(QString::fromStdWString(L"InvalidBallots")));
    model->setHeaderData(i++,Qt::Horizontal,QVariant(QString::fromStdWString(L"ValidBallots")));
    if(!city)
    {
        model->setHeaderData(i++,Qt::Horizontal,QVariant(QString::fromStdWString(L"AbsenteeBallots")));
        model->setHeaderData(i++,Qt::Horizontal,QVariant(QString::fromStdWString(L"PeopleTookAbsenteeBallots")));
        model->setHeaderData(i++,Qt::Horizontal,QVariant(QString::fromStdWString(L"PeopleVotedWithAbsenteeBallots")));
        model->setHeaderData(i++,Qt::Horizontal,QVariant(QString::fromStdWString(L"CanceledAbsenteeBallots")));
        model->setHeaderData(i++,Qt::Horizontal,QVariant(QString::fromStdWString(L"PeopleTookAbsenteeBallotsFromTEC")));
        model->setHeaderData(i++,Qt::Horizontal,QVariant(QString::fromStdWString(L"AbsenteeBallotsLost")));
    }
    model->setHeaderData(i++,Qt::Horizontal,QVariant(QString::fromStdWString(L"VotingBallotsLost")));
    model->setHeaderData(i++,Qt::Horizontal,QVariant(QString::fromStdWString(L"VotingBallotsUnaccounted")));

    for(std::vector<EAnalyzer::Person>::const_iterator ic = election.info.candidates.begin(),cend = election.info.candidates.end(); ic != cend; ++ic)
    {
        model->setHeaderData(i++,Qt::Horizontal,QVariant(QString::fromStdWString(ic->surname)));
    }
    header->setModel(model);
    header->setResizeMode(QHeaderView::Stretch);
    ui->tableWidgetElectionResult->setHorizontalHeader(header);

    ui->tableWidgetAddResult->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidgetAddResult->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    if(election.info.type == EAnalyzer::ElectionInfo::City)
        ui->tableWidgetAddResult->setColumnCount(13 + election.info.candidates.size());
    else
    {
        ui->tableWidgetAddResult->setColumnCount(20 + election.info.candidates.size());
        city = false;
    }
    i = 0;
    ui->tableWidgetAddResult->setItem(0,i++,new QTableWidgetItem(QString::fromStdWString(L"УИК")));
    ui->tableWidgetAddResult->setItem(0,i++,new QTableWidgetItem(QString::fromStdWString(L"ТИК")));
    ui->tableWidgetAddResult->setItem(0,i++,new QTableWidgetItem(QString::fromStdWString(L"VotersAtLEC")));
    ui->tableWidgetAddResult->setItem(0,i++,new QTableWidgetItem(QString::fromStdWString(L"VotingBallots")));
    if (!city)
        ui->tableWidgetAddResult->setItem(0,i++,new QTableWidgetItem(QString::fromStdWString(L"PeopleVotedInAdvance")));
    ui->tableWidgetAddResult->setItem(0,i++,new QTableWidgetItem(QString::fromStdWString(L"PeopleVotedIn")));
    ui->tableWidgetAddResult->setItem(0,i++,new QTableWidgetItem(QString::fromStdWString(L"PeopleVotedOut")));
    ui->tableWidgetAddResult->setItem(0,i++,new QTableWidgetItem(QString::fromStdWString(L"CanceledBallots")));
    ui->tableWidgetAddResult->setItem(0,i++,new QTableWidgetItem(QString::fromStdWString(L"BallotsFoundOut")));
    ui->tableWidgetAddResult->setItem(0,i++,new QTableWidgetItem(QString::fromStdWString(L"BallotsFoundIn")));
    ui->tableWidgetAddResult->setItem(0,i++,new QTableWidgetItem(QString::fromStdWString(L"InvalidBallots")));
    ui->tableWidgetAddResult->setItem(0,i++,new QTableWidgetItem(QString::fromStdWString(L"ValidBallots")));
    if(!city)
    {
        ui->tableWidgetAddResult->setItem(0,i++,new QTableWidgetItem(QString::fromStdWString(L"AbsenteeBallots")));
        ui->tableWidgetAddResult->setItem(0,i++,new QTableWidgetItem(QString::fromStdWString(L"PeopleTookAbsenteeBallots")));
        ui->tableWidgetAddResult->setItem(0,i++,new QTableWidgetItem(QString::fromStdWString(L"PeopleVotedWithAbsenteeBallots")));
        ui->tableWidgetAddResult->setItem(0,i++,new QTableWidgetItem(QString::fromStdWString(L"CanceledAbsenteeBallots")));
        ui->tableWidgetAddResult->setItem(0,i++,new QTableWidgetItem(QString::fromStdWString(L"PeopleTookAbsenteeBallotsFromTEC")));
        ui->tableWidgetAddResult->setItem(0,i++,new QTableWidgetItem(QString::fromStdWString(L"AbsenteeBallotsLost")));
    }
    ui->tableWidgetAddResult->setItem(0,i++,new QTableWidgetItem(QString::fromStdWString(L"VotingBallotsLost")));
    ui->tableWidgetAddResult->setItem(0,i++,new QTableWidgetItem(QString::fromStdWString(L"VotingBallotsUnaccounted")));

    for(std::vector<EAnalyzer::Person>::const_iterator ic = election.info.candidates.begin(),cend = election.info.candidates.end(); ic != cend; ++ic)
    {
        ui->tableWidgetAddResult->setItem(0,i++,new QTableWidgetItem(QString::fromStdWString(ic->surname)));
    }


    SetElectionData(election);

}

void EditJsonFile::DeleteCandidate(int i)
{
    election.info.candidates.erase(election.info.candidates.begin() + i);
    for(std::vector<EAnalyzer::ElectionResult>::iterator it = election.result.begin(),end = election.result.end(); it != end; ++it)
    {
        it->candidates.erase(it->candidates.begin() + i);
    }
}

bool EditJsonFile::SaveJson()
{
    QString infileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                     "",
                                                     tr("Files (*.json)"));
    if(infileName == "")
        return false;


    GetElectionInfo(election);
    GetElectionData(election);
    EAnalyzer::Election::JsonCoderToFile(election, infileName.toStdString());
    return true;
}

void EditJsonFile::OpenJson()
{
    QString infileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.json)"));
    if(infileName == "")
        return;
    election = EAnalyzer::Election::JsonParserFromFile(infileName.toStdString());
    ShowElection(election);
}

void EditJsonFile::SetJson(const EAnalyzer::Election &election)
{
    this->election = election;
    ShowElection(election);
}

void EditJsonFile::AddCandidate()
{
    EAnalyzer::Person person;
    person.name =      ui->tableWidgetAddCandidate->item(0,0)->text().toStdWString();
    person.surname =   ui->tableWidgetAddCandidate->item(0,1)->text().toStdWString();
    person.fname =     ui->tableWidgetAddCandidate->item(0,2)->text().toStdWString();


    election.info.candidates.push_back(person);

    for(std::vector<EAnalyzer::ElectionResult>::iterator it = election.result.begin(),end = election.result.end(); it != end; ++it)
    {
        it->candidates.push_back(0);
    }
    ShowElection(election);


}

void EditJsonFile::DeleteLine(QTableWidgetItem* item)
{
    if(!update_candidate_table)
        return;
    bool update;
    switch (item->column()) {
    case 0:
        update = item->text().toStdWString() !=
                election.info.candidates[item->row()].name;
        break;
    case 1:
        update = item->text().toStdWString()  !=
                election.info.candidates[item->row()].surname;
        break;
    case 2:
        update = item->text().toStdWString()  !=
                election.info.candidates[item->row()].fname;
        break;
    default:
        update = false;
        break;
    }

    if (!update)
        return;

    GetElectionData(election);
    GetElectionInfo(election);

    if(item->text() == "")
    {
        bool f = true;
        int i;
        for(i = 0; i<this->ui->tableWidgetCandidates->columnCount(); i++)
        {
            if(this->ui->tableWidgetCandidates->item(item->row(),i)->text() != "")
            {
                f = false;
                break;
            }
        }
        if (f)
        {
            QMessageBox box;
            box.setText(QString::fromStdWString(L"Удалить кандидата?"));
            box.setInformativeText(QString::fromStdWString(L"Вся информация о голосах за " + election.info.candidates[item->row()].fname + L" будет удалена"));
            box.setStandardButtons(QMessageBox::Cancel | QMessageBox::Discard);
            box.setDefaultButton(QMessageBox::Cancel);
            int ret = box.exec();
            if(ret == QMessageBox::Discard)
                DeleteCandidate(item->row());
        }
    }
    ShowElection(election);
}

void EditJsonFile::AddData()
{
    EAnalyzer::ElectionResult result;

    bool city = false;
    if (election.info.type == EAnalyzer::ElectionInfo::City)
        city = true;

    int i = 0;
    result.N_LEC = ui->tableWidgetAddResult->item(0,i++)->text().toInt();
    result.TEC = ui->tableWidgetAddResult->item(0,i++)->text().toStdWString();
    result.VotersAtLEC = ui->tableWidgetAddResult->item(0,i++)->text().toInt();
    result.VotingBallots = ui->tableWidgetAddResult->item(0,i++)->text().toInt();
    if (!city)
        result.PeopleVotedInAdvance = ui->tableWidgetAddResult->item(0,i++)->text().toInt();
    result.PeopleVotedIn = ui->tableWidgetAddResult->item(0,i++)->text().toInt();
    result.PeopleVotedOut = ui->tableWidgetAddResult->item(0,i++)->text().toInt();
    result.CanceledBallots = ui->tableWidgetAddResult->item(0,i++)->text().toInt();
    result.BallotsFoundOut = ui->tableWidgetAddResult->item(0,i++)->text().toInt();
    result.BallotsFoundIn = ui->tableWidgetAddResult->item(0,i++)->text().toInt();
    result.InvalidBallots = ui->tableWidgetAddResult->item(0,i++)->text().toInt();
    result.ValidBallots = ui->tableWidgetAddResult->item(0,i++)->text().toInt();
    if(!city)
    {
        result.AbsenteeBallots = ui->tableWidgetAddResult->item(0,i++)->text().toInt();
        result.PeopleTookAbsenteeBallots = ui->tableWidgetAddResult->item(0,i++)->text().toInt();
        result.PeopleVotedWithAbsenteeBallots = ui->tableWidgetAddResult->item(0,i++)->text().toInt();
        result.CanceledAbsenteeBallots = ui->tableWidgetAddResult->item(0,i++)->text().toInt();
        result.PeopleTookAbsenteeBallotsFromTEC = ui->tableWidgetAddResult->item(0,i++)->text().toInt();
        result.AbsenteeBallotsLost = ui->tableWidgetAddResult->item(0,i++)->text().toInt();
    }
    result.VotingBallotsLost = ui->tableWidgetAddResult->item(0,i++)->text().toInt();
    result.VotingBallotsUnaccounted = ui->tableWidgetAddResult->item(0,i++)->text().toInt();

    result.candidates.clear();
    for(uint j = 0; j< election.info.candidates.size(); ++j)
    {
        result.candidates.push_back(ui->tableWidgetAddResult->item(0,i++)->text().toInt());
    }
    election.result.push_back(result);
    ShowElection(election);
}

void EditJsonFile::DeleteDataLine(QTableWidgetItem *item)
{
    if (!update_data_table)
        return;
    bool update;
    EAnalyzer::Election tmp;
    GetElectionInfo(tmp);
    GetElectionData(tmp);
    update = !(tmp == election);

    if (!update)
        return;

    GetElectionData(election);
    GetElectionInfo(election);

    if(item->text() == "")
    {
        bool f = true;
        int i;
        for(i = 0; i<this->ui->tableWidgetElectionResult->columnCount(); i++)
        {
            if(this->ui->tableWidgetElectionResult->item(item->row(),i)->text() != "")
            {
                f = false;
                break;
            }
        }
        if (f)
        {
            QMessageBox box;
            box.setText(QString::fromStdWString(L"Удалить участок?"));
            box.setInformativeText(QString::fromStdWString(L"Вся информация о участке ") + QString::number(election.result[item->row()].N_LEC) + QString::fromStdWString(L" будет удалена"));
            box.setStandardButtons(QMessageBox::Cancel | QMessageBox::Discard);
            box.setDefaultButton(QMessageBox::Cancel);
            int ret = box.exec();
            if(ret == QMessageBox::Discard)
                election.result.erase(election.result.begin() + item->row());
        }
    }
    ShowElection(election);
}

void EditJsonFile::SwitchElectionType(int i)
{
    if(i != election.info.type)
        return;
    GetElectionData(election);
    GetElectionInfo(election);
    ShowElection(election);
}


