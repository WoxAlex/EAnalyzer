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

LEC::LEC ():
    N_LEC_(0), 
    TEC_(), 
    N_voters_at_LEC_(0), 
    N_voting_ballots_(0), 
    N_people_voted_in_advance_(0), 
    N_people_voted_in_(0), 
    N_people_voted_out_(0), 
    N_canceled_ballots_(0),
    N_ballots_found_out_(0), 
    N_ballots_found_in_(0), 
    N_invalid_ballots_(0), 
    N_valid_ballots_(0),
    N_absentee_ballots_(0), 
    N_people_took_absentee_ballots_(0), 
    N_people_voted_with_absentee_ballots_(0),
    N_canceled_absentee_ballots_(0), 
    N_people_took_absentee_ballots_from_TEC_(0),
    N_absentee_ballots_lost_(0), 
    N_voting_ballots_lost_(0), 
    N_voting_ballots_unaccounted_(0),
    candidates_ ()

    {
        TEC_[0] = '\0';
        for(int i = 0; i < 25; i++) candidates_[i] = 0;
    }

//==============================================================================

LEC::LEC (int N_LEC, char* TEC, int N_voters_at_LEC):
    N_LEC_(N_LEC), 
    TEC_(), 
    N_voters_at_LEC_(N_voters_at_LEC), 
    N_voting_ballots_(0), 
    N_people_voted_in_advance_(0), 
    N_people_voted_in_(0), 
    N_people_voted_out_(0), 
    N_canceled_ballots_(0),
    N_ballots_found_out_(0),
    N_ballots_found_in_(0), 
    N_invalid_ballots_(0), 
    N_valid_ballots_(0),
    N_absentee_ballots_(0), 
    N_people_took_absentee_ballots_(0), 
    N_people_voted_with_absentee_ballots_(0),
    N_canceled_absentee_ballots_(0), 
    N_people_took_absentee_ballots_from_TEC_(0),
    N_absentee_ballots_lost_(0), 
    N_voting_ballots_lost_(0), 
    N_voting_ballots_unaccounted_(0),
    candidates_ ()
    
    {
        strcpy(TEC_, TEC);
        for(int i = 0; i < 25; i++) candidates_[i] = 0;
    }

//==============================================================================

LEC::LEC (int N_LEC, char* TEC, int N_voters_at_LEC, 
          int N_voting_ballots,
          int N_people_voted_in_advance, int N_people_voted_in, 
          int N_people_voted_out, int N_canceled_ballots,
          int N_ballots_found_out, int N_ballots_found_in,
          int N_invalid_ballots, int N_valid_ballots,
          int N_absentee_ballots, int N_people_took_absentee_ballots, 
          int N_people_voted_with_absentee_ballots, 
          int N_canceled_absentee_ballots, 
          int N_people_took_absentee_ballots_from_TEC,
          int N_absentee_ballots_lost, int N_voting_ballots_lost, 
          int N_voting_ballots_unaccounted,
          int* candidates):
    N_LEC_(N_LEC), 
    TEC_(), 
    N_voters_at_LEC_(N_voters_at_LEC), 
    N_voting_ballots_(N_voting_ballots), 
    N_people_voted_in_advance_(N_people_voted_in_advance), 
    N_people_voted_in_(N_people_voted_in), 
    N_people_voted_out_(N_people_voted_out), 
    N_canceled_ballots_(N_canceled_ballots),
    N_ballots_found_out_(N_ballots_found_out), 
    N_ballots_found_in_(N_ballots_found_in), 
    N_invalid_ballots_(N_invalid_ballots), 
    N_valid_ballots_(N_valid_ballots),
    N_absentee_ballots_(N_absentee_ballots), 
    N_people_took_absentee_ballots_(N_people_took_absentee_ballots), 
    N_people_voted_with_absentee_ballots_(N_people_voted_with_absentee_ballots),
    N_canceled_absentee_ballots_(N_canceled_absentee_ballots), 
    N_people_took_absentee_ballots_from_TEC_(N_people_took_absentee_ballots_from_TEC),
    N_absentee_ballots_lost_(N_absentee_ballots_lost), 
    N_voting_ballots_lost_(N_voting_ballots_lost), 
    N_voting_ballots_unaccounted_(N_voting_ballots_unaccounted)

    {
        for(int i = 0; i < NumberCandidates; i++) candidates_[i] = candidates[i];
        strcpy(TEC_, TEC);
    }

//==============================================================================

LEC::LEC (const LEC& lec):
    N_LEC_(lec.N_LEC_), 
    TEC_(), 
    N_voters_at_LEC_(lec.N_voters_at_LEC_), 
    N_voting_ballots_(lec.N_voting_ballots_), 
    N_people_voted_in_advance_(lec.N_people_voted_in_advance_), 
    N_people_voted_in_(lec.N_people_voted_in_), 
    N_people_voted_out_(lec.N_people_voted_out_), 
    N_canceled_ballots_(lec.N_canceled_ballots_),
    N_ballots_found_out_(lec.N_ballots_found_out_), 
    N_ballots_found_in_(lec.N_ballots_found_in_), 
    N_invalid_ballots_(lec.N_invalid_ballots_), 
    N_valid_ballots_(lec.N_valid_ballots_),
    N_absentee_ballots_(lec.N_absentee_ballots_), 
    N_people_took_absentee_ballots_(lec.N_people_took_absentee_ballots_), 
    N_people_voted_with_absentee_ballots_(lec.N_people_voted_with_absentee_ballots_),
    N_canceled_absentee_ballots_(lec.N_canceled_absentee_ballots_), 
    N_people_took_absentee_ballots_from_TEC_(lec.N_people_took_absentee_ballots_from_TEC_),
    N_absentee_ballots_lost_(lec.N_absentee_ballots_lost_), 
    N_voting_ballots_lost_(lec.N_voting_ballots_lost_), 
    N_voting_ballots_unaccounted_(lec.N_voting_ballots_unaccounted_)

    {
        for(int i = 0; i < NumberCandidates; i++) candidates_[i] = lec.candidates_[i];
        strcpy(TEC_, lec.TEC_);
    }

//##############################################################################

void LEC::operator =  (const LEC& lec)
{
    this->N_LEC_ = lec.N_LEC_;
    strcpy(this->TEC_, lec.TEC_);
    this->N_voters_at_LEC_ = lec.N_voters_at_LEC_; 
    this->N_voting_ballots_ = lec.N_voting_ballots_;
    this->N_people_voted_in_advance_ = lec.N_people_voted_in_advance_;
    this->N_people_voted_in_ = lec.N_people_voted_in_;
    this->N_people_voted_out_ = lec.N_people_voted_out_;
    this->N_canceled_ballots_ = lec.N_canceled_ballots_;
    this->N_ballots_found_out_ = lec.N_ballots_found_out_;
    this->N_ballots_found_in_ = lec.N_ballots_found_in_;
    this->N_invalid_ballots_ = lec.N_invalid_ballots_;
    this->N_valid_ballots_ = lec.N_valid_ballots_;
    this->N_absentee_ballots_ = lec.N_absentee_ballots_;
    this->N_people_took_absentee_ballots_ = lec.N_people_took_absentee_ballots_;
    this->N_people_voted_with_absentee_ballots_ = lec.N_people_voted_with_absentee_ballots_;
    this->N_canceled_absentee_ballots_ = lec.N_canceled_absentee_ballots_;
    this->N_people_took_absentee_ballots_from_TEC_ = lec.N_people_took_absentee_ballots_from_TEC_;
    this->N_absentee_ballots_lost_ = lec.N_absentee_ballots_lost_;
    this->N_voting_ballots_lost_ = lec.N_voting_ballots_lost_;
    this->N_voting_ballots_unaccounted_ = lec.N_voting_ballots_unaccounted_;
    for (int i = 0; i < NumberCandidates; i++)  this->candidates_[i] = lec.candidates_[i];
}

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

LEC scan_LEC ()
{
    int N_LEC;
    char TEC [100];
    int N_voters_at_LEC;

    int N_voting_ballots;

    int N_people_voted_in_advance = 0;
    int N_people_voted_in;
    int N_people_voted_out;
    int N_canceled_ballots;

    int N_ballots_found_out;
    int N_ballots_found_in;

    int N_invalid_ballots;
    int N_valid_ballots;

    int N_absentee_ballots = 0;
    int N_people_took_absentee_ballots = 0;
    int N_people_voted_with_absentee_ballots = 0;
    int N_canceled_absentee_ballots = 0;
    int N_people_took_absentee_ballots_from_TEC = 0;

    int N_absentee_ballots_lost;
    int N_voting_ballots_lost;
    int N_voting_ballots_unaccounted;

    int candidates [25];

    fscanf (Datafile, "%i", &N_LEC);
    if (POut == 2) printf("Участок №%i, ", N_LEC);

    char a;
    for(a = ' '; a == ' '; fscanf(Datafile, "%c", &a)) {}
    TEC[0] = a;

    for(int i = 1; i < 100; i++)
    {
        fscanf (Datafile, "%c", &TEC[i]);
        if (TEC[i] == ' ')
        {
            TEC[i] = '\0';
            break;
        }
    }
    if (POut == 2) printf("ТИК %s;\n", TEC);

    fscanf (Datafile, "%i", &N_voters_at_LEC);
    if (POut == 2) printf("Число избирателей по спискам: %i\n", N_voters_at_LEC);
    fscanf (Datafile, "%i", &N_voting_ballots);
    if (POut == 2) printf("Число избирательных бюллетеней, полученных УИК: %i\n", N_voting_ballots);
    if (ElectionType != 1)
    {
        fscanf (Datafile, "%i", &N_people_voted_in_advance);
        if (POut == 2) printf("Число избирателей, проголосовавших досрочно: %i\n", N_people_voted_in_advance);
    }
    fscanf (Datafile, "%i", &N_people_voted_in);
    if (POut == 2) printf("Число избирателей, проголосовавших на участке: %i\n", N_people_voted_in);
    fscanf (Datafile, "%i", &N_people_voted_out);
    if (POut == 2) printf("Число избирателей, проголосовавших вне участка: %i\n", N_people_voted_out);
    fscanf (Datafile, "%i", &N_canceled_ballots);
    if (POut == 2) printf("Число погашенных бюллетеней: %i\n", N_canceled_ballots);
    fscanf (Datafile, "%i", &N_ballots_found_out);
    if (POut == 2) printf("Число бюллетеней в урнах для надомного голосования: %i\n", N_ballots_found_out);
    fscanf (Datafile, "%i", &N_ballots_found_in);
    if (POut == 2) printf("Число бюллетеней в стационарных урнах: %i\n", N_ballots_found_in);
    fscanf (Datafile, "%i", &N_invalid_ballots);
    if (POut == 2) printf("Число недействительных бюллетеней: %i\n", N_invalid_ballots);
    fscanf (Datafile, "%i", &N_valid_ballots);
    if (POut == 2) printf("Число действительных бюллетеней: %i\n", N_valid_ballots);
    if (ElectionType != 2)
    {
        fscanf (Datafile, "%i", &N_absentee_ballots);
        if (POut == 2) printf("Число открепительных бюллетеней, полученных УИК: %i\n", N_absentee_ballots);
        fscanf (Datafile, "%i", &N_people_took_absentee_ballots);
        if (POut == 2) printf("Число выданных открепительных удостоверений: %i\n", N_people_took_absentee_ballots);
        fscanf (Datafile, "%i", &N_people_voted_with_absentee_ballots);
        if (POut == 2) printf("Число избирателей, проголосовавших по открепительным удостоверениям: %i\n", N_people_voted_with_absentee_ballots);
        fscanf (Datafile, "%i", &N_canceled_absentee_ballots);
        if (POut == 2) printf("Число погашенных открепительных бюллетеней: %i\n", N_canceled_absentee_ballots);
        fscanf (Datafile, "%i", &N_people_took_absentee_ballots_from_TEC);
        if (POut == 2) printf("Число открепительных бюллетеней, выданных ТИК: %i\n", N_people_took_absentee_ballots_from_TEC);
        fscanf (Datafile, "%i", &N_absentee_ballots_lost);
        if (POut == 2) printf("Число погашенных открепительных бюллетеней: %i\n", N_absentee_ballots_lost);
    }
    fscanf (Datafile, "%i", &N_voting_ballots_lost);
    if (POut == 2) printf("Число погашенных избирательных бюллетеней: %i\n", N_voting_ballots_lost);
    fscanf (Datafile, "%i", &N_voting_ballots_unaccounted);
    if (POut == 2) printf("Число избирательных бюллетеней, не учтенных при получении: %i\n", N_voting_ballots_unaccounted);

    for (int i = 0; i < NumberCandidates; i++)
    {
        fscanf (Datafile, "%i", &candidates[i]);
        if (POut == 2) printf("Кандидат №%i: %i\n", i+1, candidates[i]);
    }

    LEC res (N_LEC, TEC, N_voters_at_LEC, N_voting_ballots, 
             N_people_voted_in_advance, N_people_voted_in, N_people_voted_out,
             N_canceled_ballots, N_ballots_found_out, N_ballots_found_in, 
             N_invalid_ballots, N_valid_ballots, 
             N_absentee_ballots, N_people_took_absentee_ballots, 
             N_people_voted_with_absentee_ballots,
             N_canceled_absentee_ballots,
             N_people_took_absentee_ballots_from_TEC, N_absentee_ballots_lost,
             N_voting_ballots_lost, N_voting_ballots_unaccounted, 
             candidates);

    return res;     
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

void bind ()
{
    if (bind_control == 1)
    {
        printf("  Программа уже связана с неким файлом. Сначала сделайте сброс.\n");
    }
    else
    {
        char filename [150] = "\0";

        printf("  Введите имя файла: \n  >> ");
        cin>>filename;

        if ((Datafile = fopen(filename,"r")) == NULL) printf("  Файл не найден.\n");
        else
        {
            fscanf (Datafile, "%i", &ElectionType);
            for(char a = ' '; a != '\n'; fscanf(Datafile, "%c", &a)) {}

            for(int i = 0; i < 100; i++)
            {
                fscanf (Datafile, "%c", &date[i]);
                if (date[i] == '\n')
                {
                    date[i] = '\0';
                    break;
                }
            }
            if (ElectionType == 2)
            {
                for(int i = 0; i < 100; i++)
                {
                    fscanf (Datafile, "%c", &city[i]);
                    if (city[i] == '\n')
                    {
                        city[i] = '\0';
                        break;
                    }
                }
            }

            fscanf (Datafile, "%i", &NumberCandidates);
            candidate = new person[NumberCandidates];
                    
            for(int n = 0; n < NumberCandidates; n++)
            {
                char a;
                for(a = '\n'; a == '\n' || a == ' '; fscanf(Datafile, "%c", &a)) {}
                candidate[n].surname[0] = a;

                for(int i = 1; i < 100; i++)
                {
                    fscanf (Datafile, "%c", &candidate[n].surname[i]);
                    if (candidate[n].surname[i] == ' ' || candidate[n].surname[i] == '\n')
                    {
                        candidate[n].surname[i] = '\0';
                        break;
                    }
                }
                for(int i = 0; i < 100; i++)
                {
                    fscanf (Datafile, "%c", &candidate[n].name[i]);
                    if (candidate[n].name[i] == ' ' || candidate[n].name[i] == '\n')
                    {
                        candidate[n].name[i] = '\0';
                        break;
                    }
                }
                for(int i = 0; i < 100; i++)
                {
                    fscanf (Datafile, "%c", &candidate[n].fname[i]);
                    if (candidate[n].fname[i] == ' ' || candidate[n].fname[i] == '\n')
                    {
                        candidate[n].fname[i] = '\0';
                        break;
                    }
                }
            }

            fscanf (Datafile, "%i", &BaseSize);

            printf("\n    Тип выборов: %i, ", ElectionType);
            if      (ElectionType == 1) printf("(Президентские)\n");
            else if (ElectionType == 2) printf("(Главы города)\n");
            else if (ElectionType == 3) printf("(Депутатские)\n");
            printf("    Количество кандидатов: %i\n", NumberCandidates);
            for(int i = 0; i < NumberCandidates; i++)
            {
                printf("    -%s %s %s\n", candidate[i].surname, candidate[i].name, candidate[i].fname);
            }
            printf("    Количество участков: %i\n\n", BaseSize);

            Base = new LEC[BaseSize];

            for (int i = 0; i < BaseSize; i++)
            {
                Base[i] = scan_LEC ();
                if (POut == 1) cout<<Base[i]<<endl;
                if (POut > 0)  cout<<endl;
            }
         
            bind_control++;
        }
    }
}

void unbind ()
{
    if (bind_control == 1)
    {
        fclose(Datafile);
        delete[] Base;
        delete[] candidate;
        bind_control--;
    }
    else
    {
        printf("  Программа не связана ни с каким файлом базы данных.\n");
    }
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
