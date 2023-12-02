#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <ctype.h>
#include <vector>
#include <simlib.h>
#include <getopt.h>
#include <string.h>
#include <iomanip>

#define day 1440
#define hour 60
#define month 365 / 12
#define year 12 * month

std::pair<double, double> tLab = std::make_pair(15, 30);
std::pair<double, double> tCistenie = std::make_pair(30, 60);
std::pair<double, double> tNormalizacia = std::make_pair(15, 30);
std::pair<double, double> tPasterizacia = std::make_pair(20.0, 30);
std::pair<double, double> tChladenie1 = std::make_pair(30, 60);
std::pair<double, double> tLab2 = std::make_pair(10, 15);
std::pair<double, double> tZrazanie = std::make_pair(60, 70);
std::pair<double, double> tKrajanie = std::make_pair(5, 10);
std::pair<double, double> tLisovanie = std::make_pair(120, 150);
std::pair<double, double> tFormovanie = std::make_pair(15, 20);
std::pair<double, double> tSolenie = std::make_pair(hour * 6, hour * 7);
double tChladenie2 = day * 20;
double tSklad = day * 365;

int numberOfYears = 1;
int monthsOff = 3;
int failProbabilityDays = 1;


#define tObsluhaPoruchy 30

// vyrobne linky
Facility Laboratorium1("Laboratorium pre prijem mlieka");
Facility CistenieL("Cistiaca Linka");
Facility NormalizaciaL("Normalizacna Linka");
Facility PasterizaciaL("Pasterizacna Linka");
Facility ChladenieL("Chladiaca Linka");
Facility Laboratorium2("Mikrobiologicke laboratorium");
Facility ZrazanieL("Zrazacia Linka");
Facility KrajanieL("Krajacia Linka");
Facility LisovanieL("Lisovacia Linka");
Facility FormovanieL("Formovacia Linka");
Facility SolenieL("Soliaca Linka");


bool porucha = false;
bool off = true;

int x = 0;
int bazen = 3;

double fullTime;


// pocitadlo transakci
int transakce = 0;
int transakce2 = 0;
int stopcount = INT32_MAX;

class Porucha : public Event{
    void Behavior() {
        porucha = true;
        (new Porucha)->Activate(Time+Exponential(failProbabilityDays * day));
    }
};
//########################################################################


class Solenie: public Process {

    bool trash;
    

    void Behavior() {

        Seize(SolenieL);
        Porucha();
        for (int i = 0; i < (4 - bazen); i++) {
            Wait(Uniform(tSolenie.first, tSolenie.second));
        }
        
        Release(SolenieL);

        
        if (!trash)
        {
            transakce2++;
        }
        else{
            transakce--;
            trash = false;
        }
            

        if (transakce2 >= stopcount) {
            std::cout << "STOP" << std::endl;
            Stop();
        }

        if (off)
        {
            fullTime = Time;
            off = false;
        }
        

        Wait(tChladenie2);

        transakce++;

        
    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
            trash = true;
        }
    }
};

class Formovanie: public Process {

    bool trash;

    void Behavior() {

        Seize(FormovanieL);
        Porucha();
        Wait(Uniform(tFormovanie.first, tFormovanie.second));
        Release(FormovanieL);

        //(new Formovanie)->Activate();
        
        if (!trash)
        {
            (new Solenie)->Activate();
        }
        else
            trash = false;
    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
            trash = true;
        }
    }
};

class Lisovanie: public Process {

    bool trash;

    void Behavior() {

        Seize(LisovanieL);
        Porucha();
        Wait(Uniform(tLisovanie.first, tLisovanie.second));
        Release(LisovanieL);

        //(new Lisovanie)->Activate();
        
        if (!trash)
        {
            (new Formovanie)->Activate();
        }
        else
            trash = false;

    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
            trash = true;
        }
    }
};

class Krajanie: public Process {
    bool trash;
    void Behavior() {

        Seize(KrajanieL);
        Porucha();
        Wait(Uniform(tKrajanie.first, tKrajanie.second));
        Release(KrajanieL);

        //(new Krajanie)->Activate();
        
        if (!trash)
        {
            (new Lisovanie)->Activate();
        }
        else
            trash = false;
    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
            trash = true;
        }
    }
};

class Zrazanie: public Process {
    bool trash;
    void Behavior() {

        Seize(ZrazanieL);
        Porucha();
        Wait(Uniform(tZrazanie.first, tZrazanie.second));
        Release(ZrazanieL);

        //(new Zrazanie)->Activate();
        
        if (!trash)
        {
            (new Krajanie)->Activate();
        }
        else
            trash = false;
    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
            trash = true;
        }
    }
};

class Lab2: public Process {
    bool trash;
    void Behavior() {

        Seize(Laboratorium2);
        Porucha();
        Wait(Uniform(tLab2.first, tLab2.second));
        Release(Laboratorium2);

        //(new Lab2)->Activate();
        
        if (!trash)
        {
            (new Zrazanie)->Activate();
        }
        else
            trash = false;

    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
            trash = true;
        }
    }
};

class Chladenie: public Process {
    bool trash;
    void Behavior() {

        Seize(ChladenieL);
        Porucha();
        Wait(Uniform(tChladenie1.first, tChladenie1.second));
        Release(ChladenieL);
        //(new Chladenie)->Activate();
        
        if (!trash)
        {
            (new Lab2)->Activate();
        }
        else
            trash = false;

    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
            trash = true;
        }
    }
};

class Pasterizacia: public Process {
    bool trash;
    void Behavior() {

        Seize(PasterizaciaL);
        Porucha();
        Wait(Uniform(tPasterizacia.first, tPasterizacia.second));
        Release(PasterizaciaL);
        //(new Pasterizacia)->Activate();
        
        if (!trash)
        {
            (new Chladenie)->Activate();
        }
        else
            trash = false;

    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
            trash = true;
        }
    }
};

class Normalizacia: public Process {
    bool trash;
    void Behavior() {

        Seize(NormalizaciaL);
        Porucha();
        Wait(Uniform(tNormalizacia.first, tNormalizacia.second));
        Release(NormalizaciaL);
        //(new Normalizacia)->Activate();
    
        if (!trash)
        {
            (new Pasterizacia)->Activate();
        }
        else
            trash = false;
    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
        }
    }

};

class Cistenie: public Process {

    bool trash;
    void Behavior() {

        Seize(CistenieL);
        Porucha();
        Wait(Uniform(tCistenie.first, tCistenie.second));
        Release(CistenieL);
        //(new Cistenie)->Activate();
        if (!trash)
        {
            (new Normalizacia)->Activate();
        }
        else
            trash = false;
        
           
    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
            trash = true;
        }
    }

};

class KontrolaKyslosti: public Process {

    bool trash;
    void Behavior() {
        x++;
        //Wait(Exponential(1440)); //generator tranzakcii

        Seize(Laboratorium1);
        Porucha();
        Wait(Uniform(tLab.first, tLab.second));
        Release(Laboratorium1);

        (new KontrolaKyslosti)->Activate();

        if (Random() > (0.01) && !trash){
            (new Cistenie)->Activate();
            (new Cistenie)->Activate();
        }  
        else if (trash)
        {
            trash = false;
        }
        
    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
            trash = true;
        }
    }

};

void printStats(){

    Laboratorium1.Output();
    CistenieL.Output();
    NormalizaciaL.Output();
    PasterizaciaL.Output();
    ChladenieL.Output();
    Laboratorium2.Output();
    ZrazanieL.Output();
    KrajanieL.Output();
    LisovanieL.Output();
    FormovanieL.Output();
    SolenieL.Output();

    int hours = static_cast<int>(fullTime / 60);
    int minutes = static_cast<int>(fullTime) % 60;


    std::cout << "Exit transakce: " << transakce <<std::endl;
    std::cout << "Transakce na sklade: " << transakce2 - transakce <<std::endl;
    std::cout << "Full time: " << hours << ":" << minutes << std::endl;
}

// -------------------------------------------======== MAIN =========----------------------------------------------------
int main(int argc, char* argv[]) {

    /*failProbabilityDays;
    numberOfYears;
    monthsOff;*/
    bazen = 3;

    int nod = (numberOfYears * year) - (month * monthsOff);
    int timespan = day * nod; // implicitni timespan 1 rok

     

    // init timespan
    Init(0, timespan);

    // zahajeni vyroby
    (new KontrolaKyslosti)->Activate();
    (new Porucha)->Activate();
    // zahajeni simulace
    Run();

    // print stats
    printStats();


    return EXIT_SUCCESS;
}
