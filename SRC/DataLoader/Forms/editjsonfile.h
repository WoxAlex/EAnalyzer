#ifndef EDITJSONFILE_H
#define EDITJSONFILE_H

#include <QMainWindow>
#include <QTableWidget>
#include <QMessageBox>
#include "SRC/Analyzer/DataTypes/election.h"
namespace Ui {
    class EditJsonFile;
}

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
