#include <iostream>
#include <functional>
#include <iomanip>
#include <queue>
#include <algorithm>
#include <vector>
#include <tr1/memory>
#include <sstream>

using namespace  std::tr1;
enum attackType{
	subAttack = 0,
	fightBack = 1,
};
enum type{
    Dragon = 0,
    Ninja = 1,
    Iceman = 2,
    Lion = 3,
    Wolf = 4
};
enum color{
    none = 0,
    red = 1,
    blue = 2,
};

class character;
class place;
class city;
class dragon;
class base;
std::vector <city> allCitys;
int allForce[5];
int allElements[5];
typedef  shared_ptr<character> c_ptr;
//bool isEnermy(color C, c_ptr hellc);
std::string IntToString(int& T){
	std::stringstream tempSS;
	tempSS << T;
	std::string temp;
	tempSS >> temp;
	return temp;
}
class timer{

    int h, m;
public:
    typedef std::pair<int, int> sysTime;
    timer(){
        h = m = 0;
    }
    void addTime(int i = 10){
        m += i;
        while(m >= 60){
            m -= 60;
            h += 1;
        }
    }
    sysTime getTime(){
        return sysTime(h, m);
    }
    std::ostream& display(std::ostream& cout){
        cout <<std::setfill('0') << std::setw(3)
             << h << ':' << std::setw(2) <<m;
        return cout;
    }
} InnerTimer;
std::ostream& operator << (std::ostream& t, timer A){
    return A.display(t);
}

std::vector<place*> map;
class character{
protected:
    type TYPE;
    color belong;
    int elements;
    int force;
    int No;
    city* Pos;
public:
    virtual void getHurt(c_ptr from, attackType t) {};
    virtual void move() {};
    virtual void fight(c_ptr to, attackType t) {};
    virtual void die(){};
    virtual void getElements(){};
    friend place;
    friend city;
    friend base;
    std::string getTypeName(){
        switch (this->TYPE){
            case 0 : return "dragon";
            case 1 : return "ninja";
            case 2 : return "iceman";
            case 3 : return "lion";
            case 4 : return "wolf";
        }
    }
    std::string getInfo(){
		std::string temp((belong == red) ? "red " : "blue ");
		temp += getTypeName();
		temp += " ";
		temp += IntToString(No);
		return temp;
	}
    friend dragon;
    friend bool isEnermy(color C, c_ptr hellc);
};
class place{
protected:
public:
    std::vector<shared_ptr<character> > population;
    color flag;
public:
	virtual void recieve(c_ptr theC){};
	virtual void debrief(){};
	virtual void genElements(){};

};
class city : public place{
private:
    int no;
    color lastWin;
public:
    void recieve(c_ptr theC){
        population.push_back(shared_ptr<character>(theC));
        std::cout << InnerTimer << ' ';
        std::cout << theC ->getInfo();

        std::cout << " marched to city " << no <<" with "
             << theC->elements << " elements and force " << theC->force
             << std::endl;
        theC -> Pos = this;
    }
    void remove(c_ptr theC){
        std::vector<shared_ptr<character> >::iterator iter = std::find(population.begin(), population.end(),shared_ptr<character>(theC));
        population.erase(iter);
    }
    void recordWining(c_ptr winner){
    	if(winner->belong == lastWin) {
    		flag = winner -> belong;
    		std::cout << InnerTimer << ' '
    			 << ((flag == red)? "red " : "blue ")
    			 << "flag raised in city "
    			 << no
    			 << std::endl;
    	}else{
			lastWin = winner->belong;
		}
    }
    friend character;
    friend dragon;


};
//bool isEnermy(color C ,c_ptr hellc){
//	return hellc->belong != C;
//}
bool isOver = 0;
class base : public place{
public:
	void recieve(c_ptr theC){
		population.push_back(shared_ptr<character>(theC));
		int i = 0;
		std::cout << InnerTimer << ' '
				  << theC -> getInfo() <<" reached "
				  << ((flag == red) ? "red " : "blue ")
				  << "headquarter with "
				  << theC -> elements << " elements and force "
				  << theC -> force << std::endl;
		for(std::vector<c_ptr>::iterator iter = population.begin(), iend = population.end();iter < iend; ++iter){
			if((*iter)->belong != flag) ++i;
		}
		if(i == 2){
			std::cout << InnerTimer << ((flag == red) ? " red " : " blue ")
				 << "headquarter was taken\n";
			isOver = 1;
		}
	}
};
class dragon : public character{
public:
    dragon(){
        TYPE = Dragon ;
    }
    void getHurt(c_ptr from, attackType t) {
    	if(elements <= from -> force)
    		from->elements += elements;
        elements -= from -> force;
        if(elements <= 0) die(),Pos -> recordWining(from);
        else if(t == subAttack){
			fight(from, fightBack);
		}

    };
    void move() {

	};
    void fight(c_ptr to, attackType t) {
    	if(t == subAttack){
			std::cout << InnerTimer << ' '
					  << getInfo() << " attacked "
					  << to->getInfo() <<" in city "
					  << Pos -> no << std::endl;
		}else{
			std::cout << InnerTimer << ' '
					  << getInfo() << " fought back against "
					  << to->getInfo() <<" in city "
					  << Pos -> no << std::endl;

		}
		to -> getHurt(c_ptr(this), t);
		if(elements > 0 && t == subAttack){
			std::cout << InnerTimer << ' '
					  << getInfo() <<" yelled in city "
					  << Pos -> no << std::endl;
		}

	};
    void die(){
        std::cout << InnerTimer << ' '
             << getInfo()
             << " was killed in city " << Pos->no << std::endl;
        Pos -> remove(c_ptr(this));
    }
};


int main(){
    city T;
    InnerTimer.addTime(100000);
    c_ptr a = c_ptr(new dragon), b = c_ptr(new dragon);
    T.recieve(a);
    T.recieve(b);
    T.recordWining(a);
    T.recordWining(a);
    base R;
    R.flag = red;
    a -> fight(b, subAttack);
	R.recieve(a);
    R.recieve(b);

}
