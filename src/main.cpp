#include <simlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


Facility Laboratorium1("Laboratorium pre prijem mlieka");
Facility Cistenie("Cistiaca Linka");
Facility Normalizacia("Normalizacna Linka");
Facility Pasterizacia("Pasterizacna Linka");
Facility Chladenie("Chladiaca Linka");
Facility Laboratorium2("Mikrobiologicke laboratorium");
Facility Zrazanie("Zrazacia Linka");
Facility Krajanie("Krajacia Linka");
Facility Lisovanie("Lisovacia Linka");
Facility Formovanie("Formovacia Linka");
Facility Solenie("Soliaca Linka");










class KontrolaKyslosti: public Process {

    void Behavior() {

        Seize(Laboratorium1);
        Release(Laboratorium1);

        (new KontrolaKyslosti)->Activate();
    }

};

