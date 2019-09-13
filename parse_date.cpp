#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stdexcept>
#include <cctype>

using namespace std;

struct Day{
    int value;
    explicit Day(int new_value){
        if(new_value < 1 || new_value > 31)
            throw logic_error("Day value is invalid: " + to_string(new_value));
        value = new_value;
    }
};
struct Month{
    int value;
    explicit Month(int new_value){
        if(new_value < 1 || new_value > 12)
            throw logic_error("Month value is invalid: " + to_string(new_value));
        value = new_value;
    }
};
struct Year{
    int value;
    explicit Year(int new_value){
        value = new_value;
    }
};

class Date{
    int day;
    int month;
    int year;
public:
    Date(Year y, Month m, Day d){
        day = d.value;
        month = m.value;
        year = y.value;
    }
    int GetDay() const{
        return day;
    }
    int GetMonth() const{
        return month;
    }
    int GetYear() const{
        return year;
    }
};
ostream& operator<<(ostream& larg, const Date& rarg){
    larg << setfill('0');
    return larg << setw(4) << to_string(rarg.GetYear()) << "-"
    << setw(2) << to_string(rarg.GetMonth()) << "-"
    << setw(2) << to_string(rarg.GetDay());
}
bool operator<(const Date& larg, const Date& rarg){
    return vector<int>{larg.GetYear(), larg.GetMonth(), larg.GetDay()} <
            vector<int>{rarg.GetYear(), rarg.GetMonth(), rarg.GetDay()};
}

class Database{
    map<Date, set<string>> data;
public:
    void AddEvent(const Date& date, const string& event){
        if(!event.empty())
            data[date].insert(event);
    }
    bool DeleteEvent(const Date& date, const string& event){
        if(data.count(date) > 0 && data[date].count(event) > 0){
            data[date].erase(event);
            return true;
        }else
            return false;
    }
    int  DeleteDate(const Date& date){
        int n = 0;
        if(data.count(date) > 0){
            n = data[date].size();
            data.erase(date);
        }
        return n;
    }
    set<string> Find(const Date& date) const{
        if(data.count(date) > 0)
            return data.at(date);
        return {};
    }
    void Print() const{
        for(const auto& item: data)
            for(const auto& event: item.second)
                cout<<item.first<<" "<<event<<endl;
    }
};
vector<string> split(const string& s, char delim = ' ', bool allow = true){
    vector<string> splited_vector;
    string token;
    for(auto c:s)
        if(c != delim)
            token += c;
        else if(token.size()){
            splited_vector.push_back(token);
            token.clear();
        }else if(!allow)
            throw invalid_argument("Wrong data format: " + s);
    if(token.size()) splited_vector.push_back(token);
    return splited_vector;
}
Date ParseDate(const string& data){
    bool ok = true;
    vector<string> s_data;
    string token;
    for(auto c: data)
        if(isdigit(c) || ((c == '-' || c == '+') && token.empty()))//!!
            token += c;
        else if(c == '-' && token.size() && token != "-" && token != "+"){
            s_data.push_back(token);
            token.clear();
        }else{
            ok = false;
            break;
        }
    if(token.size() && token != "-" && token != "+")//!!
        s_data.push_back(token);
    else
        ok = false;
    if(s_data.size() != 3)
        ok = false;
    if(!ok)
        throw logic_error("Wrong date format: " + data);

    Year year(stoi(s_data[0]));
    Month month(stoi(s_data[1]));
    Day day(stoi(s_data[2]));
    return Date(year, month, day);
}

int main()
{
    Database db;

    string line;
    try{
        while(getline(cin, line)){
            vector<string> s_line = split(line);
            string cmd, date, event;
            if(s_line.size() > 2)
                event = s_line[2];
            if(s_line.size() > 1)
                date = s_line[1];
            if(!s_line.empty()){
                string cmd = s_line[0];
                if(cmd == "Print")
                    db.Print();
                else if(cmd == "Add"){
                    db.AddEvent(ParseDate(date), event);
                }else if(cmd == "Find"){
                    for(const auto& event: db.Find(ParseDate(date)))
                        cout<<event<<endl;
                }else if(cmd == "Del"){
                    Date p_date(ParseDate(date));
                    if(event.empty())
                        cout<<"Deleted "<<db.DeleteDate(p_date)<<" events"<<endl;//last_error was here:)
                    else{
                        cout<<(db.DeleteEvent(p_date, event)?
                            "Deleted successfully":
                            "Event not found")<<endl;
                    }
                }else
                    throw logic_error("Unknown command: " + cmd);
            }
        }
    }catch(exception& ex){
        cout<<ex.what()<<endl;
    }
    return 0;
}
