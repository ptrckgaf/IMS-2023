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


#define tObsluhaPoruchy 10

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
double pravdepodobnostporuchy = 0.1;

int x = 0;


// pocitadlo transakci
int transakce = 0;
int stopcount = INT32_MAX;

class Porucha : public Event{
    void Behavior() {

        
        if (Random() < (pravdepodobnostporuchy))
            porucha = true;
        else
            porucha = false;
        x++;
        (new Porucha)->Activate(Time+Exponential(1));
    }
};
//########################################################################


class Solenie: public Process {

    void Behavior() {

        Seize(SolenieL);
        Porucha();
        for (int i = 0; i < 3; i++) {
            Wait(Uniform(tSolenie.first, tSolenie.second));
        }
        
        Release(SolenieL);
        Wait(tChladenie2);
        Wait(tSklad);


        if (transakce >= stopcount) {
            std::cout << "STOP" << std::endl;
            Stop();
        }
    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
        }
    }
};

class Formovanie: public Process {

    void Behavior() {

        Seize(FormovanieL);
        Porucha();
        Wait(Uniform(tFormovanie.first, tFormovanie.second));
        Release(FormovanieL);

        //(new Formovanie)->Activate();
        (new Solenie)->Activate();
    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
        }
    }
};

class Lisovanie: public Process {

    void Behavior() {

        Seize(LisovanieL);
        Porucha();
        Wait(Uniform(tLisovanie.first, tLisovanie.second));
        Release(LisovanieL);

        //(new Lisovanie)->Activate();
        (new Formovanie)->Activate();

    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
        }
    }
};

class Krajanie: public Process {

    void Behavior() {

        Seize(KrajanieL);
        Porucha();
        Wait(Uniform(tKrajanie.first, tKrajanie.second));
        Release(KrajanieL);

        //(new Krajanie)->Activate();
        (new Lisovanie)->Activate();
    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
        }
    }
};

class Zrazanie: public Process {

    void Behavior() {

        Seize(ZrazanieL);
        Porucha();
        Wait(Uniform(tZrazanie.first, tZrazanie.second));
        Release(ZrazanieL);

        //(new Zrazanie)->Activate();
        (new Krajanie)->Activate();
    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
        }
    }
};

class Lab2: public Process {

    void Behavior() {

        Seize(Laboratorium2);
        Porucha();
        Wait(Uniform(tLab2.first, tLab2.second));
        Release(Laboratorium2);

        //(new Lab2)->Activate();
        (new Zrazanie)->Activate();

    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
        }
    }
};

class Chladenie: public Process {

    void Behavior() {

        Seize(ChladenieL);
        Porucha();
        Wait(Uniform(tChladenie1.first, tChladenie1.second));
        Release(ChladenieL);
        //(new Chladenie)->Activate();
        (new Lab2)->Activate();

    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
        }
    }
};

class Pasterizacia: public Process {

    void Behavior() {

        Seize(PasterizaciaL);
        Porucha();
        Wait(Uniform(tPasterizacia.first, tPasterizacia.second));
        Release(PasterizaciaL);
        //(new Pasterizacia)->Activate();
        (new Chladenie)->Activate();
    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
        }
    }
};

class Normalizacia: public Process {

    void Behavior() {

        Seize(NormalizaciaL);
        Porucha();
        Wait(Uniform(tNormalizacia.first, tNormalizacia.second));
        Release(NormalizaciaL);
        //(new Normalizacia)->Activate();
        (new Pasterizacia)->Activate();
    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
        }
    }

};

class Cistenie: public Process {

    void Behavior() {

        Seize(CistenieL);
        Porucha();
        Wait(Uniform(tCistenie.first, tCistenie.second));
        Release(CistenieL);
        //(new Cistenie)->Activate();
        (new Normalizacia)->Activate();   
    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
        }
    }

};

class KontrolaKyslosti: public Process {

    void Behavior() {
        
        Wait(Exponential(1440)); //generator tranzakcii

        Seize(Laboratorium1);
        Porucha();
        Wait(Uniform(tLab.first, tLab.second));
        Release(Laboratorium1);

        (new KontrolaKyslosti)->Activate();
        (new Cistenie)->Activate();
    }
    void Porucha() {
        if (porucha) {
            Wait(tObsluhaPoruchy);
            porucha = false;
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
}

// -------------------------------------------======== MAIN =========----------------------------------------------------
int main(int argc, char* argv[]) {
    int timespan = 1440 * 365; // implicitni timespan 1 rok

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
