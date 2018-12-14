// Farid Khosravi - Student number = 267964 - farid.khosravi@student.tut.fi
// Project 3: Company
// This file contains the codes of the public and private functions and structs
// of the Class Company
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "company.hh"

using namespace std;

// structure and
Company::Company() {
};

//Destructor
Company::~Company() {
    for (auto employee_info: organization_) {
        delete employee_info.second;
        employee_info.second = nullptr;
    }
};

// In this method I add information of new employee.
// There is no output but the inputs are id, department ,
// time and print stream in order to add new employee to
// organization_ map.
// the whole structure also keeps pointers to the data of the data structure.
void Company::addNewEmployee(const std::string &id, const std::string &dep,
                             const double &time, std::ostream &output) {


    // checking by key value to see if the id is in our organization
    // structure or not, if it is then print an error, if it is not
    // building the new map index struct.
    // boss_ is pointer to the information of the boss of this id
    if (organization_.find(id) != organization_.end()){
        output << "Error. Employee already added." << endl;
        return;
    } else {
        Employee* new_employee = new Employee;
        new_employee->id_ = id;
        new_employee->department_ = dep;
        new_employee->time_in_service_ = time;
        new_employee->boss_ = nullptr;

        organization_.insert({id, new_employee});
    }
};

// printing all of the employees in for loop from organization_ map
// no input and output except print stream
void Company::printEmployees(std::ostream &output) const {
    // <ID>, <department>, <time in service>
    for (auto employee_info: organization_){
        output << employee_info.second->id_ << ", "
               << employee_info.second->department_ << ", "
               << employee_info.second->time_in_service_ << endl;
    }
};

// this method adds relation between employees
// inputs are subordinate and boss employee and print stream
// and there is no output
// bool subNotFound is used to detect the employee id which is not
// inserted into the map and printing error by private function printNotFound
// then adding boss and subordinating relation in their struct
void Company::addRelation(const std::string &subordinate,
                          const std::string &boss,
                          std::ostream &output) {
    bool subNotFound = false;

    // Error printing flag handler
    if (organization_.find(subordinate) == organization_.end() and subordinate != EMPTY) {
        subNotFound = true;
        printNotFound(subordinate,subNotFound,output);
        return;
    } else if (organization_.find(boss) == organization_.end() and boss != EMPTY) {
        subNotFound = true;
        return;
    }

    Employee* subordinate_ptr = organization_[subordinate];
    Employee* boss_ptr = nullptr;
    if (boss != EMPTY) {
        boss_ptr = organization_[boss];
    }

    subordinate_ptr->boss_ = boss_ptr;
    if (boss_ptr != nullptr){
        boss_ptr->subordinates_.push_back(subordinate_ptr);
    }
};

// printing subordinates of an input id, there is no output.
// local variables bool idNotFound(error flag printing), Employee* boss_ptr(to keep the id data),
// map<string, string> sub(to keep the name of the subordinates and also sort them), there is no output
// if there is no subordinate we print its related message.
void Company::printSubordinates(const std::string &id, std::ostream &output) const {

    bool idNotFound = false;
    Employee* boss_ptr = nullptr;
    map<string, string> sub;

    // error printing flag
    if (organization_.find(id) == organization_.end()){
        idNotFound = true;
    } else {
        boss_ptr = organization_.at(id);

        // if there is any subordinates
        if (boss_ptr->subordinates_.size() == 0) {
            output << id << " has no subordinates." << endl;
            return;
        }
    }

    // error handling part
    if (idNotFound) {
        printNotFound(id, idNotFound,output);
        return;
    } else {

        // printing the result
        output << id << " has " << boss_ptr->subordinates_.size() << " subordinates:" <<endl;
        for (Employee* subord_ptr: boss_ptr->subordinates_){
            sub.insert({subord_ptr->id_, subord_ptr->id_});
        }

        // printing the result
        for (auto it: sub) {
            output << it.first << endl;
        }
    }

};

// this method prints the boss of the input employee
// the input is id of the employ and there is no output
// if there is no boss related error will print
// local variable bool idNotFound is used as error flag to print the error
void Company::printBoss(const std::string &id, std::ostream &output) const {

    bool idNotFound = false;

    // error printing flag
    if (organization_.find(id) == organization_.end()){
        idNotFound = true;

    // to see is the id is the highest bosses or not
    } else if (organization_.at(id)->boss_ == nullptr) {

        // print the result
        output << id << " has no boss." << endl;
    } else {

        // print the result
        output << id << " has " << 1 << " boss:" << endl;
        output << organization_.at(id)->boss_->id_ << endl;
    }
    if (idNotFound) {
        printNotFound(id, idNotFound,output);
        return;
    }

};

// this method prints the colleagues who has a same boss
// the input is id of the employee and there is no output
// local variables, bool idNotFound(error printing flag),
// string boss(keeping name of the boss), int colleagues(number of the colleagues),
// map<string, string> colleague(keeping name of the colleagues in alphabetical order)
void Company::printColleagues(const std::string &id, std::ostream &output) const {

    bool idNotFound = false;
    string boss = EMPTY;
    int colleagues = 0;
    map<string, string> colleague;

    // error printing flag
    if (organization_.find(id) == organization_.end()){
        idNotFound = true;
    } else {

        // checking the id to is it a head boss or not
        // if it is not go through the if
        if (organization_.at(id)->boss_ != nullptr) {
            boss = organization_.at(id)->boss_->id_;
            colleagues = organization_.at(boss)->subordinates_.size() - 1;
        } else {

            //printing the result
            output << id << " has no colleagues." << endl;
            return;
        }
    }

    // error handling
    if (idNotFound) {
        printNotFound(id, idNotFound,output);
        return;
    } else {

        // printing the result
        output << id << " has " << colleagues << " colleagues:"<< endl;
        for (auto employee_info: organization_.at(boss)->subordinates_){
            if (employee_info->id_ != id ) {
                colleague.insert({employee_info->id_ ,employee_info->id_});
            }
        }
        // printing the result
        for (auto colleague_num: colleague) {
            output << colleague_num.first << endl;
        }
    }


};

// printing the members of a team in a department which are in a hierarchy
// the input is id of an employee and the output is print of that team
// local variables are bool idNotFound(error printing flag), string boss(keeping
// name of the highest level boss in that department),
// map<string, double> department(map of storing team member name and their time in service)
// at first I find the highest boss in the related department hierarchy by highestBoss private function and insert
// the bosses in that way to the map and then find the subordinates employee by departmentRec recursive function and
// add them to the map too.
void Company::printDepartment(const std::string &id, std::ostream &output) const {

    bool idNotFound = false;
    string boss = EMPTY;
    map<string, double> department;

    // error printing flag
    if (organization_.find(id) == organization_.end()){
        idNotFound = true;
    } else {

        // finding the highest boss
        highestBoss(id,boss,department);
    }

    // recursive function part
    if (not idNotFound){
        departmentRec(id, boss, department);
    }

    // no colleagues
    if (not idNotFound and department.size() == 0){
        output << id << " has no department colleagues." << endl;
        return;
    }
    // error handling
    if (idNotFound) {
        printNotFound(id, idNotFound,output);
        return;
    }
    // printing results
    output << id << " has " << department.size() << " department colleagues:" << endl;
    for (auto dep: department) {
        output << dep.first << endl;
    }
};

// This function prints the longest time in service of the employee and its subordinates
// the input is employee id and the output is print of the longest time in service and his/her name
// I found the subordinates by recursive function longestRec, it adds the name and time in service
// of the subordinates employees to the longest map.
// local variables are bool idNotFound(error printing flag), double maxTime(using to store maximum
// time in service), string maxName(stores the employee name of the maximu time) and
// map<string, double> longest(stores the name and time in service of each employee)
void Company::printLongestTimeInLineManagement(const std::string &id, std::ostream &output) const {

    bool idNotFound = false;
    double maxTime = 0;
    string maxName = EMPTY;
    map<string, double> longest;

    // error printing flag
    if (organization_.find(id) == organization_.end()){
        idNotFound = true;
    }


    if (not idNotFound){
        auto id1 = id;
        longest.insert({id,organization_.at(id)->time_in_service_});
        // recursive function
        longestRec(id, id1, longest);
    }

    // next 2 ifs are error printing part
    if (not idNotFound and  longest.size() == 0) {
        output << id << " has no longest" << endl;
        return;
    }
    if (idNotFound) {
        printNotFound(id,idNotFound,output);
        return;
    }

    // finding maximum
    auto it = longest.begin();
    maxName = it->first;
    maxTime = it->second;
    for (auto idx: longest) {
        if (maxTime <= idx.second) {
            maxName = idx.first;
            maxTime = idx.second;
        } else {
            continue;
        }
    }

    // printing in a print format
    if (maxName != id){
        output << "With the time of " << maxTime << ", " << maxName << " is the longest-served employee in " << id << "'s line management." << endl;
    } else {
        output << "With the time of " << maxTime << ", " << maxName << " is the longest-served employee in their line management." << endl;
    }
};

// this function is exactly like as the longest one but calculates shortest time in service
void Company::printShortestTimeInLineManagement(const std::string &id, std::ostream &output) const {
    bool idNotFound = false;
    double minTime = 0;
    string minName = EMPTY;
    map<string, double> shortest;

    // input Error flag part
    if (organization_.find(id) == organization_.end()){
        idNotFound = true;
    }

    if (not idNotFound){
        auto id1 = id;
        shortest.insert({id,organization_.at(id)->time_in_service_});
        // recursive function
        longestRec(id, id1, shortest);
    }

    // the next 2 ifs are error printing
    if (not idNotFound and  shortest.size() == 0) {
        output << id << " has no shortest." << endl;
        return;
    }
    if (idNotFound) {
        printNotFound(id,idNotFound,output);
        return;
    }

    // finding minimum amount of time and relative name
    auto it = shortest.begin();
    minName = it->first;
    minTime = it->second;
    for (auto idx: shortest) {
        if (minTime >
                idx.second) {
            minName = idx.first;
            minTime = idx.second;
        } else {
            continue;
        }
    }

    // printing the results
    if (minName != id) {
        output << "With the time of " << minTime << ", " << minName << " is the shortest-served employee in " << id << "'s line management." << endl;
    } else {
        output << "With the time of " << minTime << ", " << minName << " is the shortest-served employee in their line management." << endl;
    }
};

// this function finds the subordinates of the input employee name according to a n level and print them
// I also used subordinatesNRec recursive function for this method to find the subordinates up to level n
// the inputs are the id of the employee and the intended level and the output is printing the list of the subordinates
// local variables are bool idNotFound(error printing flag), int level(it keeps the level passed to compare with allowed level n)
// std::vector<std::string> subvec(a vector of string to keep the names of the subordinates employee),
// size_t it(this variable is used in the recursive function)
void Company::printSubordinatesN(const std::string &id, const int n, std::ostream &output) const {
    bool idNotFound = false;
    int level = 0;
    std::vector<std::string> subvec;
    size_t it = -1;

    // error of bad input n
    if (n < 1) {
        output << "Error. Level can't be less than 1." << endl;
        return;
    }

    // error printing flag
    if (organization_.find(id) == organization_.end()){
        idNotFound = true;
    } else {
        // recursive function
        subordinatesNRec(id,it,level,n,subvec);
    }

    // sorting and priting results
    if (subvec.size() != 0) {
        output << id << " has " << subvec.size() << " subordinates:" << endl;
        sort(subvec.begin(), subvec.end());
        for (auto sub: subvec) {
            output << sub << endl;
        }
      // error printing
    } else if (not idNotFound and subvec.size() == 0) {
        output << id << " has no subordinates." << endl;
        return;
    }
    if (idNotFound) {
        printNotFound(id,idNotFound,output);
        return;
    }
};

// this method finds the higher bosses of the input id according to the distance number
// the inputs are id and distance number n and the output is print of the map of the bosses
// local variables are bool idNotFound(error printing flag), int distance(to keep how much distance we passed)
// string boss(temporary variable to keep name of the bosses in the while loop)
// map<string, int> bosses(keeps the name of the bosses and their distance from the id)
void Company::printBossesN(const std::string &id, const int n, std::ostream &output) const {

    bool idNotFound = false;
    int distance = 0;
    string boss = EMPTY;
    map<string, int> bosses;

    // error for bad input n
    if (n < 1) {
        output << "Error. Level can't be less than 1." << endl;
        return;
    }
    // error printing flag
    if (organization_.find(id) == organization_.end()){
        idNotFound = true;
    } else {

        // the main process has done here to fill out the map of the bosses
        if (organization_.at(id)->boss_ != nullptr) {
            boss = organization_.at(id)->boss_->id_;
            ++distance;
            bosses.insert({boss,distance});
            while (organization_.at(boss)->boss_ != nullptr and distance < n) {
                boss = organization_.at(boss)->boss_->id_;
                bosses.insert({boss,distance});
                ++distance;
            }
        } else {
            boss = id;
            output << id << " has no bosses." << endl;
        }
    }

    // result printing part
    if (distance != 0){
        output << id << " has " << distance << " bosses:" << endl;
        for (auto boss: bosses) {
            output << boss.first << endl;
        }
    }
    // error printing part
    if (idNotFound) {
        printNotFound(id,idNotFound,output);
        return;
    }
};

// this private function finds the highest boss in the id department hierarchy
// the inputs are id and the reference boss variable name and the department
// reference map from the main function which this function is a part of it.
void Company::highestBoss(const string &id, string &boss, std::map<string, double> &department) const {
    // id_department to compare bosses department
    string id_department = organization_.at(id)->department_;
    if (organization_.at(id)->boss_ != nullptr) {
        boss = organization_.at(id)->boss_->id_;
        if (department.find(boss) == department.end()){
            department.insert({boss,organization_.at(id)->boss_->time_in_service_});
        }

        // finding higher bosses until a boss with nullptr or different department name
        while (organization_.at(boss)->boss_ != nullptr and
               organization_.at(boss)->boss_->department_ == id_department) {
            boss = organization_.at(boss)->boss_->id_;
            if (department.find(boss) == department.end()){
                department.insert({boss,organization_.at(boss)->boss_->time_in_service_});
            }
        }
    } else {
        // there is no boss
        boss = id;
    }
};

// using this function to print error. inputs are id and print flag bid(bool id)
void Company::printNotFound(const std::string &id, const bool &bid, std::ostream &output) const {
    if (bid) {
        output << "Error. " << id << " not found." << endl;
    }
};

// This recursive function finds all the team member who works under a highest same boss in the specific department
// inputs are reference id and a boss variable and a reference department map to keep the department colleagues name and
// their time in service information
void Company::departmentRec(const std::string &id,string boss, std::map<string, double> &department) const {
    if (boss != id and department.find(boss) == department.end()
            and organization_.at(boss)->department_ == organization_.at(id)->department_) {
        department.insert({boss,organization_.at(boss)->time_in_service_});
    } else {}
    if (organization_.at(boss)->subordinates_.size() != 0) {
        for (auto employ: organization_.at(boss)->subordinates_) {
            boss = employ->id_;
            departmentRec(id, boss,department);
        }
    } else {
        return;
    }
}

// this private function is used in shortest and longest finder method
// what it does is just finding the employees in the id hierarchy and store their name and time in service
// in the department map
void Company::longestRec(const string &id, const string &id1, std::map<string, double> &department) const {

    if (organization_.at(id)->subordinates_.size() != 0) {
        for (auto employ: organization_.at(id)->subordinates_) {
            string tempId = employ->id_;
            if (organization_.at(tempId)->department_ == organization_.at(id1)->department_ and department.find(tempId) == department.end()){
                department.insert({tempId,organization_.at(tempId)->time_in_service_});
            }
            longestRec(tempId, id1, department);
        }
    } else {
        return;
    }
}

// this recursive function was the most challenging part of this project in my beleive
// this private function finds the all subordinates of an input id
// the inputs are id the size_t it which used to address the next vector member to use as id
// the int &level to keep the level of the subordinate in comparison with the first id, the int n which is the
// maximum distance from the id and the string vector &subvec which stores the subordinate employees.
void Company::subordinatesNRec(const string &id, size_t it, int &level, const int n, std::vector<string> &subvec) const {

    // check is ther any subordinate or not
    if (organization_.at(id)->subordinates_.size() != 0){

        // loops over the subordinates and push back them to the vector
        for (auto sub: organization_.at(id)->subordinates_) {
            if (level < n) {

                subvec.push_back(sub->id_);
            } else {
                break;
            }
        }
        // increasing level to control the distance from id
        ++level;
        // increasing it to move to the next index of the vector
        ++it;
        // recursive part
        if (it < subvec.size() and level < n) {
            subordinatesNRec(subvec[it],it,level, n, subvec);
        } else {
            return;
        }


    } else {
        // if there is no subordinates the function comes here and
        // increase the it to go to the next index of the vector to check its subordinates
        ++it;
        if (it < subvec.size() and level < n) {
            subordinatesNRec(subvec[it],it,level, n, subvec);
        } else {
            return;
        }
    }
};
