//============================================================================
// Name        : Mobille Billing System
// Author      : ovelychko
//============================================================================

#include <iostream>
#include <algorithm>
#include <string>
#include <queue>

using namespace std;

/* In this exercise we want to design a solution for Mobile Billing .
 * Our version of the Mobile Billing contains collection of customer details.
 *
 * Every customer has an id, name, number, balance, callDuration etc.
 * Assume suitable types for each data member
 *     - id: <some unique id>
 *     - name: "Test user"
 *     - phoneNumber: "9800012345"
 *  - balance : 500
 *  - totalCallDuration : 268
 *
 * Customers are classified as Prepaid,Postpaid users with some common operations,
 * e.g. credit, getBalance will be common for both.
 * Customer id must be unique within Prepaid, Postpaid category but need not be
 * unique across both the categories. Phone numbers must be unique across both the categories.
 * For Prepaid customers recharge is considered as credit operation
 * For postpaid customers billPay is considered as credit operation.
 * For both categories, outstanding amount is considered as balance.
 *
 * Our solution should enable us to provide the following functionality
 * E.g.:
 *      display(<id>)
 *      searchByName("Test User")
 *      searchByNumber({"9880012345"})
 *      averagePrepaidBalance()             //among all Prepaid customers
 *      queryPostpaidCustomers(<criteria>), //any criteria, say, beyond some outstanding limit
 *
 * Your code should demonstrate polymorphic behaviour,i.e. dynamic binding of calls
 * search methods should return actual instance(s),find all occurences for non-unique attributes
 * For the sake of simplicity we don't want to do any similiarity or prefix matching when
 * searching for a phone number, Only an exact match should give results.
 * You may go for substring match in case of search by name
 * Also you need not implement operations like call logging
 * which can modify balance/outstanding at this moment
 *
 * The skeleton code below can be used as a starting point but doesn't have to.
 * The comments "fill in" are placeholders where you definitely have to put in some code when
 * you use this skeleton code. But this doesn't mean that you shouldn't or can't put code
 * anywhere else.
 * Also write some simple unit tests to show how you would test the functionality of the
 * MobileBilling.Don't use any testing framework.
 * Simple if-statements are sufficient for this exercise.
 *
 * Hint: Think about performance versus memory tradeoffs in your design, so you can give good
 * reasons for your decision, also clean code with polymorphic design is important.
 */

class Customer {
public:
    float credit() { return 0.0; };

    double enquireBalance() { return balance; };

    bool getIsPrepaid() {
        return isPrepaid;
    }

    string getName() {
        return name;
    }

    string getPhoneNumber() {
        return phoneNumber;
    }

    double getBalance() {
        return balance;
    }

    long getId() {
        return id;
    }

    long setId(long newId) {
        return id = newId;
    }

    ~Customer() {
        cout << "--del " << name << endl;
    }

    Customer() { id = -1; };
protected:

    long id;
    bool isPrepaid = false;
    string name;
    string phoneNumber;
    double balance = 0.0;
    double totalCallDuration = 0.0;
};

class PrepaidCustomer : public Customer {
public:
    PrepaidCustomer(const string &sName, const string &sPhoneNumber, double dBalance = 0.0) {
        isPrepaid = true;
        name = sName;
        phoneNumber = sPhoneNumber;
        balance = dBalance;
    }

    explicit PrepaidCustomer(Customer &templateCustomer) {
        isPrepaid = true;
        name = templateCustomer.getName();
        phoneNumber = templateCustomer.getPhoneNumber();
        balance = templateCustomer.getBalance();
    }
};

class PostpaidCustomer : public Customer {
public:
    PostpaidCustomer(const string &sName, const string &sPhoneNumber, double dBalance = 0.0) {
        isPrepaid = false;
        name = sName;
        phoneNumber = sPhoneNumber;
        balance = dBalance;
    }

    explicit PostpaidCustomer(Customer &templateCustomer) {
        isPrepaid = false;
        name = templateCustomer.getName();
        phoneNumber = templateCustomer.getPhoneNumber();
        balance = templateCustomer.getBalance();
    }
};

//********************************************************
class MobileBilling {

private:
    struct MyClassCompPhoneNumber {
        explicit MyClassCompPhoneNumber(const string &arg) { phoneNumber = arg; };

        inline bool operator()(Customer &item) const { return (phoneNumber == item.getPhoneNumber()); }

    private:
        string phoneNumber;
    };

    struct MyClassCompId {
        explicit MyClassCompId(long arg) { id = arg; };

        inline bool operator()(Customer &item) const { return (id == item.getId()); }

    private:
        long id;
    };

    struct MyClassCompName {
        explicit MyClassCompName(const string &arg1) {
            name = arg1;
        };

        inline bool operator()(Customer &item) const {
            return (name == item.getName());
        }

    private:
        string name;
    };

public:
    Customer *searchById(int testId) {
        auto it = std::find_if(prepaidCustomers.begin(), prepaidCustomers.end(), MyClassCompId(testId));
        if (it == prepaidCustomers.end()) {
            it = std::find_if(postpaidCustomers.begin(), postpaidCustomers.end(), MyClassCompId(testId));
            if (it == postpaidCustomers.end()) {
                return nullptr;
            } else {
                return it._M_cur;
            }
        } else {
            return it._M_cur;
        }
    }

    auto searchByName(Customer &itemCustomer) {
        if (itemCustomer.getIsPrepaid()) {
            auto it = std::find_if(prepaidCustomers.begin(), prepaidCustomers.end(),
                                   MyClassCompName(itemCustomer.getName()));
            return it;
        } else {
            auto it = std::find_if(postpaidCustomers.begin(), postpaidCustomers.end(),
                                   MyClassCompName(itemCustomer.getName()));
            return it;
        }
    };

    bool checkNameIsExist(Customer &itemCustomer) {
        if (searchByName(itemCustomer) != postpaidCustomers.end())
            return true;
        else
            return false;
    };

    auto searchByPhoneNumber(const string &sPhoneNumber) {
        auto it = std::find_if(prepaidCustomers.begin(), prepaidCustomers.end(), MyClassCompPhoneNumber(sPhoneNumber));
        if (it == prepaidCustomers.end()) {
            it = std::find_if(postpaidCustomers.begin(), postpaidCustomers.end(), MyClassCompPhoneNumber(sPhoneNumber));
        }
        return it;
    };

    bool checkPhoneNumberIsExist(const string &sPhoneNumber) {
        if (searchByPhoneNumber(sPhoneNumber) != postpaidCustomers.end())
            return true;
        else
            return false;
    }

    // Note: functions like this should return operation results (success or error value)
    // But I leave it as is since you did it Void
    void addCustomer(Customer &newCustomer) {
        if (checkNameIsExist(newCustomer)) {
            cout << "can't add customer: this name is already present";
            return;
        }
        if (checkPhoneNumberIsExist(newCustomer.getPhoneNumber())) {
            cout << "can't add customer: this phone number is already present";
            return;
        }

        if (newCustomer.getIsPrepaid()) {
            newCustomer.setId(prepaidLastIndex);
            prepaidCustomers.push_back(newCustomer);
        } else {
            newCustomer.setId(postpaidLastIndex++);
            postpaidCustomers.push_back(newCustomer);
        }
    }

    // Note: functions like this should return operation results (success or error value)
    // But I leave it as is since you did it Void
    void deleteCustomer(Customer &delCustomer) {
        auto it = searchByPhoneNumber(delCustomer.getPhoneNumber());

        if (it != postpaidCustomers.end() &&
            it->getName() == delCustomer.getName() &&
            it->getIsPrepaid() == delCustomer.getIsPrepaid()) {
            // allCustomers.erase(it);
        }
    }

    float getAveragePostPaidCallDuration(Customer customer);

    vector<Customer> queryPostpaidCustomers();

    double getAveragePrePaidBalance(Customer customer);

    vector<Customer> queryPrepaidCustomers();

    void display(long id) {
        auto it = searchById(id);
        if (it != nullptr) {
            cout << "Name: " << it->getName() << " PhoneNumber: " << it->getPhoneNumber() << endl;
        }
    }

    void displayAll() {
        for (auto &s: prepaidCustomers) {
            cout << "prep: Name: " << s.getName() << " PhoneNumber: " << s.getPhoneNumber() << endl;
        }
        for (auto &s: postpaidCustomers) {
            cout << "post: Name: " << s.getName() << " PhoneNumber: " << s.getPhoneNumber() << endl;
        }
    }

private:
    long prepaidLastIndex = 0;
    long postpaidLastIndex = 0;
    deque<Customer> prepaidCustomers;
    deque<Customer> postpaidCustomers;
};

//********************************************************
int main() {
    MobileBilling billing;
    {
        PrepaidCustomer msFirst("msFirst", "0000", 3.14);
        billing.addCustomer(msFirst);
        cout << "==================" << endl;
        PrepaidCustomer msSecond("msSecond", "0001", 0.0);
        billing.addCustomer(msSecond);
        cout << "==================" << endl;
        PostpaidCustomer msNext("msNext", "0002", 0.0);
        billing.addCustomer(msNext);
        cout << "==================" << endl;
        billing.displayAll();

        billing.deleteCustomer(msSecond);
    }

    cout << "==================" << endl;
    billing.displayAll();

    return 0;
}
