#include <simlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


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
            Wait(6); // 6-7 hodin, * 60 ak Wait bere minuty :)

        }
        Release(SolenieL);
        Wait(20); // 20 dni :)
        Wait(1); // 1 rok :rofl:

    }
};

class Formovanie: public Process {

    void Behavior() {

        Seize(FormovanieL);
        // formovanie
        Wait(15); // 15-20 min odpocinok
        Release(FormovanieL);

        (new Formovanie)->Activate();
        (new Solenie)->Activate();
    }
};


class Lisovanie: public Process {

    void Behavior() {

        Seize(LisovanieL);
        Wait(120); // 120-150 min
        Release(LisovanieL);

        (new Lisovanie)->Activate();
        (new Formovanie)->Activate();

    }
};



class Krajanie: public Process {

    void Behavior() {

        Seize(KrajanieL);
        // krajanie
        Wait(5); // 5-10 min odpocinok
        Release(KrajanieL);

        (new Krajanie)->Activate();
        (new Lisovanie)->Activate();
    }
};



class Zrazanie: public Process {

    void Behavior() {

        Seize(ZrazanieL);
        Wait(60); // 60-70 min
        Release(ZrazanieL);

        (new Zrazanie)->Activate();
        (new Krajanie)->Activate();
    }
};


class Lab2: public Process {

    void Behavior() {

        Seize(Laboratorium2);
        Release(Laboratorium2);

        (new Lab2)->Activate();
        (new Zrazanie)->Activate();

    }
};



class Chladenie: public Process {

    void Behavior() {

        Seize(ChladenieL);
        Release(ChladenieL);
        (new Chladenie)->Activate();
        (new Lab2)->Activate();

    }
};


class Pasterizacia: public Process {

    void Behavior() {

        Seize(PasterizaciaL);
        Wait(20); // 20-30 min
        Release(PasterizaciaL);
        (new Pasterizacia)->Activate();
        (new Chladenie)->Activate();
    }
};




class Normalizacia: public Process {

    void Behavior() {

        Seize(NormalizaciaL);
        Release(NormalizaciaL);
        (new Normalizacia)->Activate();
        (new Pasterizacia)->Activate();

    }

};



class Cistenie: public Process {

    void Behavior() {

        Seize(CistenieL);
        Release(CistenieL);
        (new Cistenie)->Activate();
        (new Normalizacia)->Activate();

    }

};



class KontrolaKyslosti: public Process {

    void Behavior() {

        Seize(Laboratorium1);
        Release(Laboratorium1);

        (new KontrolaKyslosti)->Activate();
        (new Cistenie)->Activate();
    }

};