/** @file 
*   @brief
*       База для оперирования данными по выборам в России.
*   @details   
*       Эта база данных была создана для эффективного оперирования табличными 
*   числами, полученными с официального сайта центризбиркома.
*
*   @author Головко Александр
*/

//######################## - LIBRARIES - #######################################

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include "Exception/Exception.h"

using namespace std;

//##################### - MACROS DEFINITIONS - #################################

//##################### - TYPE DEFINITIONS - ###################################

struct person
{
    char name [100];
    char surname [100];
    char fname [100];
};

struct LEC
{
    int N_LEC_;
    char TEC_ [250];
    int N_voters_at_LEC_;

    int N_voting_ballots_;

    int N_people_voted_in_advance_;
    int N_people_voted_in_;
    int N_people_voted_out_;
    int N_canceled_ballots_;

    int N_ballots_found_out_;
    int N_ballots_found_in_;

    int N_invalid_ballots_;
    int N_valid_ballots_;

    int N_absentee_ballots_;
    int N_people_took_absentee_ballots_;
    int N_people_voted_with_absentee_ballots_;
    int N_canceled_absentee_ballots_;
    int N_people_took_absentee_ballots_from_TEC_;

    int N_absentee_ballots_lost_;
    int N_voting_ballots_lost_;
    int N_voting_ballots_unaccounted_;

    int candidates_ [25]; 

    LEC ();
    LEC (int N_LEC, char* TEC, int N_voters_at_LEC);
    LEC (int N_LEC, char* TEC, int N_voters_at_LEC, 
         int N_voting_ballots,
         int N_people_voted_in_advance, int N_people_voted_in, 
         int N_people_voted_out, int N_canceled_ballots,
         int N_ballots_found_out, int N_ballots_found_in,
         int N_invalid_ballots, int N_valid_ballots,
         int N_absentee_ballots, int N_people_took_absentee_ballots, 
         int N_people_voted_with_absentee_ballots, 
         int N_canseled_absentee_ballots, 
         int N_people_took_absentee_ballots_from_TEC,
         int N_absentee_ballots_lost, int N_voting_ballots_lost, 
         int N_voting_ballots_unaccounted, int* candidates);  
    LEC (const LEC& lec);

    void operator = (const LEC& lec);

    friend ostream& operator << (ostream& stream, LEC& lec);
};
//#################### - VARIABLE DEFINITIONS - ################################

int ElectionType = 0;
int BaseSize = 0;
LEC* Base = NULL;
FILE* Datafile = NULL;
int NumberCandidates = 0;
person* candidate;
char date [100] = "\0";
char city [100] = "\0";

int POut = 0;
int bind_control = 0;

//#################### - FUNCTIONS DEFINITIONS - ###############################

LEC scan_LEC ();
void help ();
void bind ();
void unbind ();
void result ();
void analyze ();

//##############################################################################
//==============================================================================
//##############################################################################

int main (int argc, char** argv) try
{
    char com [50] = "help\0";
    for (;;)
    {
        if      (strcmp(com, "quit") == 0) break;
        else if (strcmp(com, "help") == 0) help();
        else if (strcmp(com, "bind") == 0) bind();
        else if (strcmp(com, "unbind") == 0) unbind();
        else if (strcmp(com, "result") == 0) result();
        else if (strcmp(com, "analyze") ==0) analyze();
        else 
            printf("  Нет такой команды. Введите 'help' для помощи.\n");

        printf(">> ");
        cin>>com;
    }

    if (bind_control > 0)
    {
        fclose(Datafile);
        delete[] Base;
        delete[] candidate;
    }

    return 0;
}
catch (Exc& E)
{
    printf("\n >> ERROR: %s\n\n", E.error);
}
catch (int point)
{
    printf("\n >> FATAL ERROR. Point #%i.\n\n", point);
}

//##############################################################################
//==============================================================================
//##############################################################################


//##############################################################################

ostream& operator << (ostream& stream, LEC& lec)
{
    if (POut == 1) stream<<"УИК №";
    stream<<lec.N_LEC_<<", ";
    if (POut == 1) stream<<"ТИК ";
    stream<<lec.TEC_;
    if (POut == 1) stream<<";"<<endl;
    else           stream<<", ";
    stream<<lec.N_voters_at_LEC_<<", ";
    stream<<lec.N_voting_ballots_<<", ";

    if (ElectionType != 1)
    {
        stream<<lec.N_people_voted_in_advance_<<", ";
    }

    stream<<lec.N_people_voted_in_<<", ";
    stream<<lec.N_people_voted_out_<<", ";
    stream<<lec.N_canceled_ballots_<<", ";
    stream<<lec.N_ballots_found_out_<<", ";
    stream<<lec.N_ballots_found_in_<<", ";
    stream<<lec.N_invalid_ballots_<<", ";
    stream<<lec.N_valid_ballots_<<", ";

    if (ElectionType != 2)
    {
        stream<<lec.N_absentee_ballots_<<", ";
        stream<<lec.N_people_took_absentee_ballots_<<", ";
        stream<<lec.N_people_voted_with_absentee_ballots_<<", ";
        stream<<lec.N_canceled_absentee_ballots_<<", ";
        stream<<lec.N_people_took_absentee_ballots_from_TEC_<<", ";
        stream<<lec.N_absentee_ballots_lost_<<", ";
    }

    stream<<lec.N_voting_ballots_lost_<<", ";
    stream<<lec.N_voting_ballots_unaccounted_<<", ";
    for (int i = 0; i < NumberCandidates; i++)
    {
        stream<<lec.candidates_[i];
        if (i < (NumberCandidates - 1))  stream<<", ";
        else                             stream<<";";
    }

    return stream;
}



//##############################################################################

void help ()
{
    printf("\n  Комманды управления:\n");
    printf("      bind    - Связать систему с файлом базы данных.\n");
    printf("      unbind  - Сбросить файл базы данных.\n");
    printf("      quit    - Выйти из программы.\n");
    printf("      help    - Печать этого сообщения.\n");
    printf("      analyze - Проанализировать данные по участкам.\n");
    printf("      result  - Печать результатов выборов по базе.\n\n");
}


void result ()
{
    if (bind_control == 0)
    {
        printf("Программа не связана ни с одним файлом.\n");
    }
    else
    {
        int TV = 0;
        int* V = new int[NumberCandidates];
        int IV = 0;
        int LV = 0;
        for(int i = 0; i < NumberCandidates; i++) V[i] = 0;

        for(int i = 0; i < BaseSize; i++)
        {
            TV += Base[i].N_people_voted_in_ + Base[i].N_people_voted_out_ + Base[i].N_people_voted_in_advance_;
            for(int j = 0; j < NumberCandidates; j++) V[j] += Base[i].candidates_[j];
            IV += Base[i].N_invalid_ballots_;
            LV += Base[i].N_people_voted_in_ + Base[i].N_people_voted_out_ 
                  - Base[i].N_ballots_found_in_ - Base[i].N_ballots_found_out_;
        } 

        printf("\n  Результаты голосования:\n");
        for (int i = 0; i < NumberCandidates; i++)
        {
            printf("    %s %s %s: %6.2f%%\n", 
                    candidate[i].surname, candidate[i].name, candidate[i].fname, 
                    (float)V[i]/TV*100);
        }

        printf("    Недействительные бюллетени: %.2f%%\n", (float)IV/TV*100);
        printf("    Утерянные бюллетени: %.2f%%\n\n", (float)LV/TV*100);

        delete[] V;
    }
}

void analyze ()
{
    if (bind_control == 0)
    {
        printf("Программа не связана ни с одним файлом.\n");
    }
    else
    {
        printf("          Анализ результатов (автогенерация)\n");
        printf("    Выборы ");
        if      (ElectionType == 1) printf("Президента Российской Федерации.\n");
        else if (ElectionType == 2) printf("Главы города %s.\n", city);
        printf ("        %s.\n\n", date);

        printf("Участки со 100%% явкой:\n");
        for (int i = 0; i < BaseSize; i++)
        {
            int TV = Base[i].N_people_voted_in_ + Base[i].N_people_voted_out_ + Base[i].N_people_voted_in_advance_;
            if (Base[i].N_people_voted_in_ + 
                Base[i].N_people_voted_out_ + 
                Base[i].N_people_voted_in_advance_ - 
                Base[i].N_voters_at_LEC_ == 0)
            {
                printf("  - ТИК %s, участок №%i:\n            Избирателей на участке %i, проголосовало %i;\n", 
                        Base[i].TEC_, Base[i].N_LEC_, Base[i].N_voters_at_LEC_, Base[i].N_voters_at_LEC_);
                printf("            Из них %i(%.2f%%) проголосовали на участке, %i(%.2f%%) проголосовали на дому.\n", 
                        Base[i].N_people_voted_in_, (float)Base[i].N_people_voted_in_/TV*100, 
                        Base[i].N_people_voted_out_, (float)Base[i].N_people_voted_out_/TV*100); 
            } 
        }

        printf("\n");
    }
}
