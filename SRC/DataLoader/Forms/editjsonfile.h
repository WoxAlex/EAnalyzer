#ifndef EDITJSONFILE_H
#define EDITJSONFILE_H

#include <QMainWindow>
#include <QTableWidget>
#include <QMessageBox>
#include "SRC/Analyzer/DataTypes/election.h"
namespace Ui {
    class EditJsonFile;
}

class IntTableWidgetItem :  public QObject, public QTableWidgetItem {
    Q_OBJECT
    public:
        IntTableWidgetItem(const QString& str)
            :QTableWidgetItem(str)
        {

        }

        bool operator <(const QTableWidgetItem &other ) const
        {
            if( this->text().toInt() < other.text().toInt() )
                return true;
            else
                return false;
        }
};

class EditJsonFile : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditJsonFile(QWidget *parent = 0);

    void onLoad();

    void onClose();

    ~EditJsonFile();

    void closeEvent(QCloseEvent *event);
private:
    Ui::EditJsonFile *ui;
    EAnalyzer::Election election;
    void SetElectionInfo(const EAnalyzer::Election& election);
    void SetElectionData(const EAnalyzer::Election& election);

    void GetElectionInfo(EAnalyzer::Election& election);
    void GetElectionData(EAnalyzer::Election& election);

    void ShowElection(const EAnalyzer::Election& election);

    void DeleteCandidate(int i);


    void SetJson(const EAnalyzer::Election& election);

    void SetRowColor(int row, bool correct_result, bool strange_result);

    bool update_data_table;
    bool update_candidate_table;
public slots:
    bool SaveJson();
    void OpenJson();
    void AddCandidate();
    void DeleteLine(QTableWidgetItem* item);
    void AddData();
    void DeleteDataLine(QTableWidgetItem* item);
    void SwitchElectionType(int i);
};

#endif // EDITJSONFILE_H
