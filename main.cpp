#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <list>

using namespace std;

class Menu;
class ListaOsob;
class ListaWizyt;
class ListaDokumentacji;
class Osoba;

class Aplikacja {
	public:
		Menu *menu;
		ListaOsob *listaOsob;
		Osoba *osobaZalogowana;
		ListaWizyt *listaWizyt;
		ListaDokumentacji *listaDokumentacji;
		
		Aplikacja();
		Run();
		dodajPoczatkoweDane();
};

class Konto {
	public:
		int id;
		string login;
		string haslo;
		string email;
		int type;
};

class Osoba: public Konto {
	public:
		string imie;
		string nazwisko;
};

class Admin: public Osoba {
	public:
		Admin() {
			type = 4;
		}
};

class Personel: public Osoba {
	public:
		Personel() {
			type = 2;
		}
};

class Pacjent: public Osoba {
	public:
		Pacjent() {
			type = 1;
		}
};

class ListaOsob {
	public:
		int lastId = 1;
		list<Osoba*> listaOsob;
		list<Osoba*>::iterator it;
		
		dodajOsobe(Osoba *osoba) {
			osoba->id = this->lastId;
			this->lastId++;
			listaOsob.push_back(osoba);
		}
		
		Osoba* autoryzacja(string login, string haslo) {
			for (it = listaOsob.begin(); it != listaOsob.end(); it++) {
			    if (((*it)->login.compare(login) == 0) && ((*it)->haslo.compare(haslo) == 0)) {
			    	return (*it);
				}
			}
			return NULL;
		}
};

class ListaPersonelu: public ListaOsob {
	
};

class ListaPacjentow: public ListaOsob {
	
};

class Wizyta {
	public:
		int id;
		string data;
		string godzina;
		string rodzajZabiegu;
		int pacjentId;
		int lekarzId;
};

class ListaWizyt {
	public:
		int lastId = 1;
		list<Wizyta*> listaWizyt;
		list<Wizyta*>::iterator it;
		
		dodajWizyte(Wizyta *wizyta) {
			wizyta->id = this->lastId;
			this->lastId++;
			listaWizyt.push_back(wizyta);
		}
		
		usunWizyte(int id) {
			for (it = listaWizyt.begin(); it != listaWizyt.end(); it++) {
				if ((*it)->id == id) {
					listaWizyt.erase(it);
				}
			}
		}
		
		wyswietlListeDla(int osobaId) {
			cout << "ID   ";
			cout << "| Data       ";
			cout << "| Godzina   ";
			cout << "| Rodzaj zabiegu";
	    	cout << endl;	
			for (it = listaWizyt.begin(); it != listaWizyt.end(); it++) {
			    if ((*it)->pacjentId == osobaId) {
			    	cout << (*it)->id;
			    	cout.width(6);
			    	cout << "| ";
			    	cout << (*it)->data;
			    	cout.width(3);
			    	cout << "| ";
			    	cout << (*it)->godzina;
			    	cout.width(7);
			    	cout << "| ";
			    	cout << (*it)->rodzajZabiegu;
			    	cout << endl;
				}
			}
		}
};

class Dokumentacja {
	public:
		int id;
		string data;
		string opis;
		int pacjentId;
		int lekarzId;
};

class ListaDokumentacji {
	public:
		int lastId = 1;
		list<Dokumentacja*> listaDokumentacji;
		list<Dokumentacja*>::iterator it;
		
		dodajDokumentacje(Dokumentacja *dokumentacja) {
			dokumentacja->id = this->lastId;
			this->lastId++;
			listaDokumentacji.push_back(dokumentacja);
		}
		
		wyswietlListeDla(int osobaId) {
			cout << "ID   ";
			cout << "| Data       ";
			cout << "| Opis   ";
	    	cout << endl;	
			for (it = listaDokumentacji.begin(); it != listaDokumentacji.end(); it++) {
			    if ((*it)->pacjentId == osobaId) {
			    	cout << (*it)->id;
			    	cout.width(6);
			    	cout << "| ";
			    	cout << (*it)->data;
			    	cout.width(3);
			    	cout << "| ";
			    	cout << (*it)->opis;
			    	cout << endl;
				}
			}
		}
};

//**************************************************************************
class Polecenie {
	public:
		int *poziom;
		
		Polecenie(int *poziom) {
			this->poziom = poziom;	
		}
		
		virtual void Run(Aplikacja *app) = 0;
};


class Cofniecie: public Polecenie {
	public:
		Cofniecie(int *poziom): Polecenie(poziom) {}
		
		void Run(Aplikacja *app) {
			*poziom = *poziom - 1;
			if (*poziom == 1) {
				app->osobaZalogowana = NULL;
			}
		}
};

class IdzDo: public Polecenie {
	public:
		int doPoziom;
		IdzDo(int *poziom, int doPoziom): Polecenie(poziom) {
			this->doPoziom = doPoziom;
		}
		
		void Run(Aplikacja *app) {
			*poziom = doPoziom;
		}
};

class Rejestracja: public Polecenie {
	public:
		Rejestracja(int *poziom): Polecenie(poziom) {}
		
		void Run(Aplikacja *app) {
			Pacjent *pacjent;
			pacjent = new Pacjent();

			cout << "	REJESTRACJA" << endl;
			cout << "----------------------------------------" << endl;
			cout << "Login: ";
			cin >> pacjent->login;
			cout << "Haslo: ";
			cin >> pacjent->haslo;
			cout << "Email: ";
			cin >> pacjent->email;
			cout << "Imie: ";
			cin >> pacjent->imie;
			cout << "Nazwisko: ";
			cin >> pacjent->nazwisko;
			
			app->listaOsob->dodajOsobe(pacjent);
		}
};

class Logowanie: public Polecenie {
	public:
		Logowanie(int *poziom): Polecenie(poziom) {}
		
		void Run(Aplikacja *app) {
			string login, haslo;

			cout << "	LOGOWANIE" << endl;
			cout << "----------------------------------------" << endl;
			cout << "Login: ";
			cin >> login;
			cout << "Haslo: ";
			cin >> haslo;
			
			app->osobaZalogowana = app->listaOsob->autoryzacja(login, haslo);
			if (app->osobaZalogowana) {
				*poziom = 2;
			}
		}
};

class DodajWizyte: public Polecenie {
	public:
		DodajWizyte(int *poziom): Polecenie(poziom) {}
		
		void Run(Aplikacja *app) {
			Wizyta *wizyta;
			wizyta = new Wizyta();
			wizyta->pacjentId = app->osobaZalogowana->id;

			cout << "	DODAWANIE WIZYTY" << endl;
			cout << "----------------------------------------" << endl;
			cout << "Data: ";
			cin >> wizyta->data;
			cout << "Godzina: ";
			cin >> wizyta->godzina;
			cout << "Cel: ";
			cin >> wizyta->rodzajZabiegu;
			
			app->listaWizyt->dodajWizyte(wizyta);
		}	
};

class UsunWizyte: public Polecenie {
	public:
		UsunWizyte(int *poziom): Polecenie(poziom) {}
		
		void Run(Aplikacja *app) {
			int wizytaId;

			cout << "	USUWANIE WIZYTY" << endl;
			cout << "----------------------------------------" << endl;
			cout << "Podaj ID wizyty do usuniecia: ";
			cin >> wizytaId;
			
			app->listaWizyt->usunWizyte(wizytaId);
		}
};

class HistoriaWizyt: public Polecenie {
	public:
		HistoriaWizyt(int *poziom): Polecenie(poziom) {}
		
		void Run(Aplikacja *app) {
			cout << "	LISTA WIZYT" << endl;
			cout << "----------------------------------------" << endl;
			app->listaWizyt->wyswietlListeDla(app->osobaZalogowana->id);
			getch();
		}
};

class DaneKontaktowe: public Polecenie {
	public:
		DaneKontaktowe(int *poziom): Polecenie(poziom) {}
		
		void Run(Aplikacja *app) {
			cout << "	DANE KONTAKTOWE" << endl;
			cout << "----------------------------------------" << endl;
			cout << "Gabinet ortodontyczny OrtoDent" << endl;
			cout << "ul. Janickiego 67" << endl;
			cout << "71-270 Szczecin" << endl;
			cout << "Tel. 914661735" << endl;
			cout << "E-mail poradniaortodent@gmail.com" << endl;
			cout << "Godziny przyjec: pon-pt, 10:00-18:00" << endl;
			
			getch();
		}
};

class ZmianaHasla: public Polecenie {
	public:
		ZmianaHasla(int *poziom): Polecenie(poziom) {}
		
		void Run(Aplikacja *app) {
			string stareHaslo, noweHaslo, noweHaslo2;
			
			cout << "	ZMIANA HASLA" << endl;
			cout << "----------------------------------------" << endl;
			cout << "Podaj stare haslo: ";
			cin >> stareHaslo;
			cout << "Podaj nowe haslo: ";
			cin >> noweHaslo;
			cout << "Podaj nowe haslo ponownie: ";
			cin >> noweHaslo2;
			
			if ((app->osobaZalogowana->haslo.compare(stareHaslo) == 0) && (noweHaslo.compare(noweHaslo2) == 0)) {
				app->osobaZalogowana->haslo = noweHaslo;
				cout << "Haslo zostalo zmienione";
			} else {
				cout << "Cos poszlo nie tak";
			}
			getch();
		}
};

class ZmianaEmail: public Polecenie {
	public:
		ZmianaEmail(int *poziom): Polecenie(poziom) {}
		
		void Run(Aplikacja *app) {
			cout << "	ZMIANA EMAIL" << endl;
			cout << "----------------------------------------" << endl;
			cout << "Podaj emial: ";
			cin >> app->osobaZalogowana->email;
			cout << "Email zostal zmieniony";		
			getch();
		}
};

class EdycjaDanychOsobowych: public Polecenie {
	public:
		EdycjaDanychOsobowych(int *poziom): Polecenie(poziom) {}
		
		void Run(Aplikacja *app) {
			cout << "	EDYCJA DANYCH OSOBOWYCH" << endl;
			cout << "----------------------------------------" << endl;
			
			getch();
		}
};

class DeaktywacjaKonta: public Polecenie {
	public:
		DeaktywacjaKonta(int *poziom): Polecenie(poziom) {}
		
		void Run(Aplikacja *app) {
			cout << "	DEAKTYWACJA KONTA" << endl;
			cout << "----------------------------------------" << endl;
			
			getch();
		}
};

class WyswietlDokumentacje: public Polecenie {
	public:
		WyswietlDokumentacje(int *poziom): Polecenie(poziom) {}
		
		void Run(Aplikacja *app) {
			cout << "	DOKUMENTACJA MEDYCZNA" << endl;
			cout << "----------------------------------------" << endl;
			app->listaDokumentacji->wyswietlListeDla(app->osobaZalogowana->id);
			getch();
		}
};

class TworzenieDokumentacji: public Polecenie {
	public:
		TworzenieDokumentacji(int *poziom): Polecenie(poziom) {}
		
		void Run(Aplikacja *app) {
			Dokumentacja *dokumentacja;
			dokumentacja = new Dokumentacja();
			cout << "	TWORZENIE DOKUMENTACJI MEDYCZNEJ" << endl;
			cout << "----------------------------------------" << endl;
			cout << "ID pacjenta: ";
			cin >> dokumentacja->pacjentId;
			cout << "Data: ";
			cin >> dokumentacja->data;
			cout << "Opis: ";
			cin >> dokumentacja->opis;
			
			app->listaDokumentacji->dodajDokumentacje(dokumentacja);
		}
};

class EdycjaDokumentacji: public Polecenie {
	public:
		EdycjaDokumentacji(int *poziom): Polecenie(poziom) {}
		
		void Run(Aplikacja *app) {
			cout << "	EDYCJA DOKUMENTACJI MEDYCZNEJ" << endl;
			cout << "----------------------------------------" << endl;
			
			getch();
		}
};

class TworzenieRecept: public Polecenie {
	public:
		TworzenieRecept(int *poziom): Polecenie(poziom) {}
		
		void Run(Aplikacja *app) {
			cout << "	TWORZENIE RECEPT" << endl;
			cout << "----------------------------------------" << endl;
			
			getch();
		}
};
//**************************************************************************
class Opcja {
	public:
	    int poziom;
	    int opcja;
	    int prawo;
	    char nazwa[50];
	    Polecenie *klasa;
};

class Menu {
	static const int ILOSC_OPCJI = 30;
	
	public:
		Aplikacja *app;
		int aktualnyPoziom;
		int aktualnaOpcja;
		int aktualnePrawaUzytkownika;
		
		Opcja opcje[ILOSC_OPCJI] = {
			{1, 1, 0, "1 - Rejestracja do systemu", new Rejestracja(&aktualnyPoziom)},
			{1, 2, 0, "2 - Logowanie do systemu", new Logowanie(&aktualnyPoziom)},
			{1, 0, 0, "0 - Wyjscie", new Cofniecie(&aktualnyPoziom)},
			
			{2, 1, 0, "1 - WIZYTY", new IdzDo(&aktualnyPoziom, 3)},
			{2, 2, 0, "2 - ZARZADZANIE KONTEM", new IdzDo(&aktualnyPoziom, 4)},
			{2, 3, 0, "3 - DOKUMENTACJA MEDYCZNA", new IdzDo(&aktualnyPoziom, 5)},
			{2, 0, 0, "0 - Wyloguj", new Cofniecie(&aktualnyPoziom)},
			
			{3, 1, 0, "1 - Dodaj wizyte", new DodajWizyte(&aktualnyPoziom)},
			{3, 2, 0, "2 - Usun wizyte", new UsunWizyte(&aktualnyPoziom)},
			{3, 3, 0, "3 - Lista wizyt", new HistoriaWizyt(&aktualnyPoziom)},
			{3, 4, 0, "4 - Dane kontaktowe poradni", new DaneKontaktowe(&aktualnyPoziom)},
			{3, 0, 0, "0 - Powrot", new IdzDo(&aktualnyPoziom, 2)},
			
			{4, 1, 0, "1 - Zmiana hasla", new ZmianaHasla(&aktualnyPoziom)},
			{4, 2, 0, "2 - Zmiana e-mail", new ZmianaEmail(&aktualnyPoziom)},
			{4, 3, 0, "3 - Edycja danych osobowych", new EdycjaDanychOsobowych(&aktualnyPoziom)},
			{4, 4, 0, "4 - Dezaktywacja konta", new DeaktywacjaKonta(&aktualnyPoziom)},
			{4, 0, 0, "0 - Powrot", new IdzDo(&aktualnyPoziom, 2)},
			
			{5, 1, 3, "1 - Wyswietl dokumentacje", new WyswietlDokumentacje(&aktualnyPoziom)},
			{5, 2, 2, "2 - Tworzenie dokumentacji medycznej", new TworzenieDokumentacji(&aktualnyPoziom)},
			{5, 3, 2, "3 - Edycja dokumentacji medycznej", new EdycjaDokumentacji(&aktualnyPoziom)},
			{5, 4, 2, "4 - Tworzenie recept", new TworzenieRecept(&aktualnyPoziom)},
			{5, 0, 0, "0 - Powrot", new IdzDo(&aktualnyPoziom, 2)},
		};
		
		Menu(Aplikacja *aplikacja) {
			app = aplikacja;
			aktualnyPoziom = 1;
		}

		void wyswietlMenu(int poziom) {
			system( "cls" );
			cout << "SYSTEM GABINETU ORTODONTYCZNEGO OrtoDent" << endl;
			cout << "----------------------------------------" << endl;
			this->wyswietlLoginInfo();
			
			for (int i=0; i<ILOSC_OPCJI; i++) {
				if ((opcje[i].poziom == poziom) && ((opcje[i].prawo == 0) || (app->osobaZalogowana->type & opcje[i].prawo))) {
					cout << opcje[i].nazwa << endl;
				}
			}
			
			cout << "<< ";
		}
		
		void wykonajPolecenie(int poziom, int opcja) {
			for (int i=0; i<ILOSC_OPCJI; i++) {
				if ((opcje[i].poziom == poziom) && (opcje[i].opcja == opcja)) {
					system( "cls" );
					cout << "SYSTEM GABINETU ORTODONTYCZNEGO OrtoDent" << endl;
					cout << "----------------------------------------" << endl;
					this->wyswietlLoginInfo();
					
					opcje[i].klasa->Run(app);
					return;
				}
			}
		}
		
		void wyswietlLoginInfo() {
			if (app->osobaZalogowana) {
				cout << "Uzytkownik: " << app->osobaZalogowana->imie << " " << app->osobaZalogowana->nazwisko << endl;
				cout << "----------------------------------------" << endl;
			}
		}
};

//**************************************************************************
Aplikacja::Aplikacja() {
	menu = new Menu(this);
	listaOsob = new ListaOsob();
	listaWizyt = new ListaWizyt();
	listaDokumentacji = new ListaDokumentacji();
	this->dodajPoczatkoweDane();
	osobaZalogowana = NULL;
}

Aplikacja::Run() {
	do {
		menu->wyswietlMenu(menu->aktualnyPoziom);
		cin >> menu->aktualnaOpcja;
		menu->wykonajPolecenie(menu->aktualnyPoziom, menu->aktualnaOpcja);
	} while( menu->aktualnyPoziom != 0 );
	
 	return 0;	
}

Aplikacja::dodajPoczatkoweDane() {
	Admin *admin;
	admin = new Admin();
	admin->login = "admin";
	admin->haslo = "admin";
	admin->email = "admin@gmail.com";
	admin->imie = "Admin";
	admin->nazwisko = "Admin";
	listaOsob->dodajOsobe(admin);
	
	Personel *personel;
	personel = new Personel();
	personel->login = "akozlowska";
	personel->haslo = "ak";
	personel->email = "a.k@gmail.com";
	personel->imie = "Aleksandra";
	personel->nazwisko = "Kozlowska";
	listaOsob->dodajOsobe(personel);
	
	Pacjent *pacjent1;
	pacjent1 = new Pacjent();
	pacjent1->login = "bjaworski";
	pacjent1->haslo = "bj";
	pacjent1->email = "b.j@gmail.com";
	pacjent1->imie = "Boleslaw";
	pacjent1->nazwisko = "Jaworski";
	listaOsob->dodajOsobe(pacjent1);
	
	Pacjent *pacjent2;
	pacjent2 = new Pacjent();
	pacjent2->login = "mkaminska";
	pacjent2->haslo = "mk";
	pacjent2->email = "m.k@gmail.com";
	pacjent2->imie = "Malgorzata";
	pacjent2->nazwisko = "Kaminska";
	listaOsob->dodajOsobe(pacjent2);
	
	Wizyta *wizyta1;
	wizyta1 = new Wizyta();
	wizyta1->data = "05.06.2018";
	wizyta1->godzina = "14:00";
	wizyta1->rodzajZabiegu = "Wizyta kontrolna";
	wizyta1->pacjentId = 3;
	wizyta1->lekarzId = 2;
	listaWizyt->dodajWizyte(wizyta1);
	
	Wizyta *wizyta2;
	wizyta2 = new Wizyta();
	wizyta2->data = "25.06.2018";
	wizyta2->godzina = "13:00";
	wizyta2->rodzajZabiegu = "Wybielanie zebow";
	wizyta2->pacjentId = 3;
	wizyta2->lekarzId = 2;
	listaWizyt->dodajWizyte(wizyta2);
	
	Dokumentacja *dokumentacja1;
	dokumentacja1 = new Dokumentacja();
	dokumentacja1->data = "06.05.2018";
	dokumentacja1->opis = "Wymiana luku gornego i dolnego";
	dokumentacja1->pacjentId = 3;
	dokumentacja1->lekarzId = 2;
	listaDokumentacji->dodajDokumentacje(dokumentacja1);
	
	Dokumentacja *dokumentacja2;
	dokumentacja2 = new Dokumentacja();
	dokumentacja2->data = "06.05.2018";
	dokumentacja2->opis = "Wyciagi 0.4 heavy";
	dokumentacja2->pacjentId = 3;
	dokumentacja2->lekarzId = 2;
	listaDokumentacji->dodajDokumentacje(dokumentacja2);
	
	Dokumentacja *dokumentacja3;
	dokumentacja3 = new Dokumentacja();
	dokumentacja3->data = "04.06.2018";
	dokumentacja3->opis = "Zdjecie aparatu stalego: gora";
	dokumentacja3->pacjentId = 3;
	dokumentacja3->lekarzId = 2;
	listaDokumentacji->dodajDokumentacje(dokumentacja3);
	
	Dokumentacja *dokumentacja4;
	dokumentacja4 = new Dokumentacja();
	dokumentacja4->data = "04.06.2018";
	dokumentacja4->opis = "Aparat retencyjny: gora";
	dokumentacja4->pacjentId = 3;
	dokumentacja4->lekarzId = 2;
	listaDokumentacji->dodajDokumentacje(dokumentacja4);
}

//**************************************************************************
int main() {
	Aplikacja *aplikacja;
	aplikacja = new Aplikacja();
	return aplikacja->Run();
}
//**************************************************************************


