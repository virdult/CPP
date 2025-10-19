#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

class Person{
private:
    std::string name;
    std::string surname;

public:
    Person(std::string n = "", std::string s = "") : name(n), surname(s){}

    //getters
    std::string getName() const {return name;}
    std::string getSurname() const {return surname;}

    //setters
    void setName(std::string n){name = n;}
    void setSurname(std::string s){surname = s;}
};

class Passenger : public Person{
private:
    int cash;
    bool ticket;
public:
    Passenger(std::string n = "", std::string s = "", int c = 1250, bool t = false) 
    : Person(n, s), cash(c), ticket(t)
    {
        if(cash < 0){
            std::cout << "Passenger cash can't be negative. It is set to 0." << std::endl;
            cash = 0;
        }
    }
    bool buyTicket(int price){
        if(cash >= price){
            ticket = true;
            cash -= price;
            return true;
        }else{
            return false;
        }
    }
    //getters
    int getCash() const {return cash;}
    bool haveTicket() const {return ticket;}
    //I didn't put setters because we don't want the user to just change any of these's values
};

class Astronaut : public Person{
private:
    int numMissions;
public:
    Astronaut(std::string n = "", std::string s = "", int missionCount = 4)
    : Person(n, s), numMissions(missionCount)
    {}
    void completeMission(){
        numMissions++;
    }
    //getter
    int getNumMissions() const {return numMissions;}
};

class Mission{
private:
    std::string name;
    int missionNumber;
    int cost;
    int faultProbability;
    bool completed;
    
    std::vector<Passenger> passengers;
    std::vector<Astronaut> crew;

    static int numMissions;

    bool isValidName(const std::string& n){
        if(n.size() != 5) return false;

        //Check dash with find
        if(n.find('-') != 2) return false;

        //Extract parts with substr
        std::string letters = n.substr(0, 2);
        std::string digits  = n.substr(3, 2);

        if(!isupper(letters[0]) || !isupper(letters[1]))
            return false;

        if(!isdigit(digits[0]) || !isdigit(digits[1]))
            return false;

        return true;
    }
public:
    Mission(std::string n    = "AA-00", int c = 0, int fProb = 0)
    : cost(c), faultProbability(fProb), completed(false)
    {
        if(isValidName(n)) name = n;
        else{
            std::cout << "Invalid mission name. Setting to AA-00." << std::endl;
            name = "AA-00";
        }
        if(faultProbability < 0) faultProbability = 0;
        if(faultProbability > 100) faultProbability = 100;

        numMissions++;
        missionNumber = numMissions;
    }

    void addSpaceShipStaff(Passenger &p){
        if(p.haveTicket()){
            passengers.push_back(p);
        }else{
            std::cout << "Passenger: " << p.getName() << " couldn't added because no ticket!" << std::endl;
        }
    }
    void addSpaceShipStaff(Astronaut &a){
        crew.push_back(a);
    }

    bool executeMission(){
        int randVal = rand() % 101;
        if(randVal > faultProbability){
            std::cout << "Mission: " << name << " success!" << std::endl;
            completed = true;
            for(auto& a : crew){
                a.completeMission();
                std::cout << a.getName() << " " << a.getSurname() << " now completed " << a.getNumMissions() << " missions!" << std::endl;
            }
        }else{
            std::cout << "Mission: " << name << " failed!" << std::endl;
            completed = false;
        }
        return completed;
    }

    int calculateProfit(int ticketPrice){
        int revenue = 0;
        if(completed){
            revenue = passengers.size() * ticketPrice;
        }
        int profit = revenue - cost;
        return profit;
    }

    int getMissionNumber() const {return missionNumber;}
    std::string getName() const {return name;}
    int getCost() const {return cost;}
};

int Mission::numMissions = 0;

class Agency{
private:
    std::string name;
    int cash;
    int ticketPrice;
    
    std::vector<Mission> completedMissions;
    std::vector<Mission> nextMissions;
public:
    Agency(std::string n = "", int c = 0, int tp = 0)
    : name(n), cash(c), ticketPrice(tp){}

    void addMission(Mission m){
        nextMissions.push_back(m);
    }
    void executeNextMission(){
        if(nextMissions.empty()){
            std::cout << "No missions! Returnin!\n";
            return;
        }
        Mission m = nextMissions.front();
        nextMissions.erase(nextMissions.begin());

        bool success = m.executeMission();

        int profit = m.calculateProfit(ticketPrice);
        cash += profit;

        if(success){
            completedMissions.push_back(m);
        }else{
            nextMissions.push_back(m);
        }
    }
    void agencyInfo(){
        std::cout << "Agency name: " << name << "\n";
        std::cout << "Agency's cash: " << cash << "\n";
        std::cout << "Ticket price for the next flight: " << ticketPrice << "\n";
        std::cout << "Next Missions: \n";
        for(const auto& m : nextMissions){
            std::cout << "Mission number: " << m.getMissionNumber()
            << " Mission name: " << m.getName()
            << " Mission cost: " << m.getCost() << "\n";
        }
        std::cout << "\nCompleted Missions: \n";
        for(const auto& m : completedMissions){
            std::cout << "Mission number: " << m.getMissionNumber()
            << " Mission name: " << m.getName()
            << " Mission cost: " << m.getCost() << "\n";
        }
    }
};

int main(){




    return 0;
}
