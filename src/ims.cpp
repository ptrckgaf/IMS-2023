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


#define tLab1 1
#define tCistenie 2
#define tNormalizacia 3.1
#define tPasterizacia 4 
#define tChladenie1 5
#define tLab2 6
#define tZrazanie 7
#define tKrajanie 8
#define tLisovanie 9
#define tFormovanie 10
#define tSolenie 11
#define tChladenie2 12
#define tSklad 13.12

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
int pravdepodobnostporuchy = 50;


// pocitadlo transakci
int transakce = 0;
int stopcount = INT32_MAX;

class Porucha : public Event{
    void Behavior() {

        if (Random() < (pravdepodobnostporuchy / 100))
            porucha = true;
        else
            porucha = false;

        (new Porucha)->Activate(Time+Exponential(1440));
    }
};
//########################################################################


class Solenie: public Process {

    void Behavior() {

        Seize(SolenieL);
        for (int i = 0; i < 3; i++) {
            Wait(tSolenie);
        }
        
        Release(SolenieL);
        Wait(tChladenie2);
        Wait(tSklad);


        if (transakce >= stopcount) {
            std::cout << "STOP" << std::endl;
            Stop();
        }
    }
};

class Formovanie: public Process {

    void Behavior() {

        Seize(FormovanieL);
        Wait(tFormovanie);
        Release(FormovanieL);

        (new Formovanie)->Activate();
        (new Solenie)->Activate();
    }
};

class Lisovanie: public Process {

    void Behavior() {

        Seize(LisovanieL);
        Wait(tLisovanie);
        Release(LisovanieL);

        (new Lisovanie)->Activate();
        (new Formovanie)->Activate();

    }
};

class Krajanie: public Process {

    void Behavior() {

        Seize(KrajanieL);
        Wait(tKrajanie);
        Release(KrajanieL);

        (new Krajanie)->Activate();
        (new Lisovanie)->Activate();
    }
};

class Zrazanie: public Process {

    void Behavior() {

        Seize(ZrazanieL);
        Wait(tZrazanie);
        Release(ZrazanieL);

        (new Zrazanie)->Activate();
        (new Krajanie)->Activate();
    }
};

class Lab2: public Process {

    void Behavior() {

        Seize(Laboratorium2);
        Wait(tLab2);
        Release(Laboratorium2);

        (new Lab2)->Activate();
        (new Zrazanie)->Activate();

    }
};

class Chladenie: public Process {

    void Behavior() {

        Seize(ChladenieL);
        Wait(tChladenie1);
        Release(ChladenieL);
        (new Chladenie)->Activate();
        (new Lab2)->Activate();

    }
};

class Pasterizacia: public Process {

    void Behavior() {

        Seize(PasterizaciaL);
        Wait(tPasterizacia);
        Release(PasterizaciaL);
        (new Pasterizacia)->Activate();
        (new Chladenie)->Activate();
    }
};

class Normalizacia: public Process {

    void Behavior() {

        Seize(NormalizaciaL);
        Wait(tNormalizacia);
        Release(NormalizaciaL);
        (new Normalizacia)->Activate();
        (new Pasterizacia)->Activate();

    }

};

class Cistenie: public Process {

    void Behavior() {

        Seize(CistenieL);
        Porucha();
        Wait(tCistenie);
        Release(CistenieL);
        (new Cistenie)->Activate();
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
        
        Wait(1440); //generator tranzakcii

        Seize(Laboratorium1);
        Porucha();
        Wait(tLab1);
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

    // zahajeni simulace
    Run();

    // print stats
    printStats();

    return EXIT_SUCCESS;
}
