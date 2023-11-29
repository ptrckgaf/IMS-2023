#include <simlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


#define tLab1 0
#define tCistenie 0
#define tNormalizacia 0
#define tPasterizacia 0
#define tChladenie 0
#define tLab2 0
#define tZrazanie 0
#define tKrajanie 0
#define tLisovanie 0
#define tFormovanie 0
#define tSolenie 0
#define tChladenie 0
#define tSklad 0



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



class Solenie: public Process {

    void Behavior() {

        Seize(SolenieL);
        for (int i = 0; i < 3; i++) {
            Wait(tSolenie);
        }
        Release(SolenieL);
        Wait(tChladenie);
        Wait(tSklad);

        //TODO STOP
        Stop();

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
        Wait(tChladenie);
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
        Wait(tCistenie);
        Release(CistenieL);
        (new Cistenie)->Activate();
        (new Normalizacia)->Activate();

    }

};

class KontrolaKyslosti: public Process {

    void Behavior() {

        Seize(Laboratorium1);
        Wait(tLab1);
        Release(Laboratorium1);

        (new KontrolaKyslosti)->Activate();
        (new Cistenie)->Activate();
    }

};


int main(int argc, char* argv[]) {

    int timespan = 1440 * 365; // implicitni timespan 1 rok

    // init timespan
    Init(0, timespan);

    // zahajeni vyroby
    (new KontrolaKyslosti)->Activate();

    // zahajeni simulace
    Run();

    return EXIT_SUCCESS;
}
