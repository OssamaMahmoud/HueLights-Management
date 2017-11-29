//
// Created by ubuntu on 11/21/17.
//

#include <err.h>
#include "Group.h"
#include "boost/algorithm/string.hpp"


Group::Group(){};

//makes a god damn group, duhhhhh!
//to know the id of the group u made keep checking the value of groupId, if empty then wait
void Group::makeGroup(std::string username, std::string address, std::string port, std::string name, std::string lights) {

    string url = "http://" + address + ":" + port + "/api/" + username + "/groups";
    std::cout<< "the URL is:   " << url << std::endl;
    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Group::handleMakeGroup,this,_1,_2));
    //create json to send
    string JSON_string = R"({"lights": [)" +
                        (lights) + R"(], "name": ")" + name +
                         R"(", "type": ")" +
                         "LightGroup" + R"("})";

    Http::Message body = Http::Message();
    body.addBodyText(JSON_string);


    if (client->post(url, body)){
        WApplication::instance()->deferRendering();
        //TODO: something i guess
        this->id = "";
//        this->name = name;
//        this->address = address;
//        this->port = port;
//        this->username = username;
//        lightList = lights;
    }

}

void Group::handleMakeGroup(boost::system::error_code err, const Http::Message& response) {
    WApplication::instance()->resumeRendering();

    Wt::Json::Array obj;
    Wt::Json::parse(response.body(),obj);
    cout<< "the response is" << response.body() <<endl;


    if (!err && response.status() == 200) {
        //get the json from the response and then extract the id from it
        this->id = "";
        //cout << "\n\n\nFROM THE HANDLERE WE GOT THIS TING   " << string(obj.get("id").toString());
        Wt::Json::Object x = obj[0];
        Wt::Json::Object y = x.get("success");
        string responseID = string(y.get("id").toString()).substr(8);
        cout << "THE RESPOSNSEID:      " << responseID <<endl;
        this->id.append(responseID);
    } else {
        cerr << "Error handling the http response: " << err << ". Response code: " << response.status() << endl;
    }
}



//takes a string for group id and sets that respective group's lightstring to the light string;
int Group::setGroupLights(string groupId, string newLights) {

    string url = "http://" + address + ":" + port + "/api/" + username + "/groups/" + groupId;
    std::cout<< "the URL is:   " << url << std::endl;

    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Group::handleSetLights,this,_1,_2));

    string JSON_string = R"({"lights": [)" + (newLights) + R"(])" + R"(})";

    Http::Message body = Http::Message();
    body.addBodyText(JSON_string);

    if (client->put(url, body)){
        //TODO: something i guess not
        //this->lightList = newLights;
    }

    return 0;
}


void Group::handleSetLights(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();
    Wt::Json::Array obj;
    Wt::Json::parse(response.body(),obj);


    if (!err && response.status() == 200) {
        //nothing rlly to update
    } else {
        cerr << "Error handling the http response: " << err << ". Response code: " << response.status() << endl;
    }
}


//TODO:
//make group add, to become modify light, which will just take the light list and modify it.


//new method to view all the group features
//name, and all the lights like a double loop
//make everything static accessed by groupID

//int Group::addLight(string lightID) {
//
//    this->lightList.append(", \"" + lightID + "\"");
//    this->setGroupLights(id, this->lightList);
//
//    return 0;
//}
//
//    int Group::removeLight(string lightID) {
//
//        //find a way to parse string and remove the god-damn light....
//        lightID = "\"" + lightID.append("\",");
//        boost::erase_all(this->lightList, lightID);
//        lightID = "\"" + lightID.append("\"");
//
//        boost::erase_all(this->lightList, lightID);
//        this->setGroupLights(this->lightList);
//
//        return 0;
//    }


//given the group id it can change the state of each of the following, on is a string with value "true" or "false"
int Group::changeState(string groupId, string on, string bri, string hue, string sat) {

    string url = "http://" + address + ":" + port + "/api/" + username + "/groups/" + groupId + "/action";

    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Group::handleChangeState, this,_1,_2));

    string JSON_string = R"({"on": ")" + std::string(on) +
            "\", \"bri\": " + bri +
            ", \"hue\": " + hue  +
            ", \"sat\": " + (sat) +
            R"(})";

    Http::Message body = Http::Message();
    body.addBodyText(JSON_string);

    if (client->put(url, body)){
        //TODO: something i guess

        //update the state by getting it again,
    }

    return 0;
}

void Group::handleChangeState(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();
    Wt::Json::Array obj;
    Wt::Json::parse(response.body(),obj);


    if (!err && response.status() == 200) {
        //get the json from the response and then extract the id from it
        //this->lightList = obj.get("lights");
        //this->getState();
    } else {
        cerr << "Error handling the http response: " << err << ". Response code: " << response.status() << endl;
    }
}


//deletes a group
int Group::deleteGroup(string groupId){

    string url = "http://" + address + ":" + port + "/api/" + username + "/groups/" + groupId ;

    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Group::handleChangeState, this,_1,_2));

    Http::Message body = Http::Message();

    if (client->deleteRequest(url, body)){
    //TODO: something i guess
        return 0;
    }

    return 1;
}



//gets all the groups in a comma seperated string, keep checking till the groupIdList is not empty
//like while (empty(this->groupIdList))... wait and then call the get states on each one individually
void Group::getGroups(){
    string url = "http://" + address + ":" + port + "/api/" + username + "/groups" ;

    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Group::handleGetGroups, this,_1,_2));

    //just throwing in null for fun, idk what is supposed to in theres
    if (client->get(url)){
        //TODO: something i guess
        //this->allGroupsString = "";
        this->groupIdList = "";

    }

    return;
}


void Group::handleGetGroups(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();
    Wt::Json::Object obj;
    Wt::Json::parse(response.body(),obj);


    if (!err && response.status() == 200) {
        //get the json from the response and then extract the id from it

        set<string> names = obj.names();
        for( auto id  : names){
            cout << "please wiork   " << string(id) << endl;
            //this->getState(string(id));

            this->groupIdList.append(string(id) + ",");
        }
        //remove the last comma
        this->groupIdList = this->groupIdList.substr(0, groupIdList.length() - 2);


        cout << "our final string is for the groups is:  " << this->groupIdList;

    } else {
        cerr << "Error handling the http response: " << err << ". Response code: " << response.status() << endl;
    }
}


//void Group::handleGetAll(boost::system::error_code err, const Http::Message &response) {
//    WApplication::instance()->resumeRendering();
//    Wt::Json::Object obj;
//    Wt::Json::parse(response.body(),obj);
//
//    set<string> names = obj.names();
//
//    if (!err && response.status() == 200) {
//        //get the json from the response and then extract the id from it
//        //this->lightList = obj.get("lights");
//
//        //ik ik its an infinte loop, im break from it, cause this is hacky... AND?
//        for( auto id  : names){
//            cout << "please wiork   " << string(id) << endl;
//            this->getState(string(id));
//            while(!httpMade)
//                cout<<"sux"<<endl;
//            this->allGroupsString.append("GroupID: " + string(id) + " ");
//            httpMade = false;
//
//        }
//        cout << "our final string is for the groups is:  " << this->allGroupsString;
//
//    } else {
//        cerr << "Error handling the http response: " << err << ". Response code: " << response.status() << endl;
//    }
//}
//


//this beautiful function when given a groupID returns the state of the group, like the name, the light list, as a string "name, lightID1, lightID2"
std::string Group::getState(string groupId) {

    string url = "http://" + address + ":" + port + "/api/" + username + "/groups/" + groupId ;

    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Group::handleGetState, this,_1,_2));

    //just throwing in null for fun, idk what is supposed to in theres
    if (client->get(url)){
        //TODO: something i guess

    }

    return "s";
}

void Group::handleGetState(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();
    Wt::Json::Object obj;
    cout << "ASS   " << string(response.body()) << endl;
    Wt::Json::parse(response.body(), obj);


    if (!err && response.status() == 200) {
        //get the json from the response and then extract the id from it
        //this->lightList = "";
        this->groupState = "";
        this->groupState.append(string(obj.get("name").toString()) + ", ");
        Wt::Json::Array lightsArray = obj.get("lights");
        //for loop go through the  array and make str of it
        for (int i = 0; i < lightsArray.size(); i++) {
            if (i == 0)
                this->groupState.append("\"" + string(lightsArray[i].toString()) + "\"");
            else
                this->groupState.append(", \"" + string(lightsArray[i].toString()) + "\"");

        }
    }
}


std::string Group::getAllState(string groupId) {
    string url = "http://" + address + ":" + port + "/api/" + username + "/groups/" + groupId ;

    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Group::handleAllGetState, this,_1,_2));

    //just throwing in null for fun, idk what is supposed to in theres
    if (client->get(url)){
        //TODO: something i guess

    }

    return "s";
}

//ITS MESSSSING UP SO BAAAD

void Group::handleAllGetState(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();
    Wt::Json::Object obj;
    cout << "ASS   " << string(response.body()) << endl;
    Wt::Json::parse(response.body(), obj);


    if (!err && response.status() == 200) {
        //get the json from the response and then extract the id from it
        //this->lightList = "";
        this->groupState.append(string(obj.get("name").toString()) + ", Lights: \n");
        Wt::Json::Array lightsArray = obj.get("lights");
        //for loop go through the  array and make str of it
        for (int i = 0; i < lightsArray.size(); i++) {
            if (i == 0)
                this->groupState.append("\"" + string(lightsArray[i].toString()) + "\"");
            else
                this->groupState.append(", \"" + string(lightsArray[i].toString()) + "\"");

        }
        this->groupState.append("|");
    }
}



const string &Group::getPort() const {
    return port;
}

void Group::setPort(const string &port) {
    Group::port = port;
}

const string &Group::getId() const {
    return id;
}

void Group::setId(const string &id) {
    Group::id = id;
}

const string &Group::getName() const {
    return name;
}

void Group::setName(const string &name) {
    Group::name = name;
}


const string &Group::getLightList() const {
    return lightList;
}

void Group::setLightList(const string &lightList) {
    Group::lightList = lightList;
}

const string &Group::getUsername() const {
    return username;
}

void Group::setUsername(const string &username) {
    Group::username = username;
}

void Group::setAddress(const string &address){
    this->address = address;
}

std::string Group::vectorToString(vector<std::string> vec){
    std::string result;
    for(int i = 0; i <vec.size(); i++){

        result.append(vec[i]);
        if(i != vec.size() - 1)
            result.append(", ");
    }
}

const string &Group::getAddress() const {
    return address;
}



void Group::setGroupState(const string &groupState) {
    Group::groupState = groupState;
}

void Group::setGroupIdList(const string &groupIdList) {
    Group::groupIdList = groupIdList;
}


//static int Group::testGroup(){
//
//    Bridge *bridge = new Bridge();
    // address = localhost
    // port what ever
//    string the_address = bridge->get_address();
//    string the_port = bridge->get_port();
//
//
//
//    string url = "http://" + the_address + ":" + the_port + "api/" + user + "/config";
//    Http::Client *client = new Http::Client();
//    client->setTimeout(15);
//    client->setMaximumResponseSize(100*1024);
//    client->done().connect(boost::bind(handleBridgeAdd,_1,_2));
//
//    if(client->post(url)){
//
//    }
//    else{
//
//        //print an error message to the screen
//        cerr << "can't open url" << endl;
//    }
//
//}
//
//void handleBridgeAdd(boost::system::error_code err, const Http::Message& response) {
//
//    Wt::Json::Object obj;
//    Wt::Json::parse(response.body(), obj);
//
//    if (!err && response.status() == 200) {
//        //get the json from the response and then extract the id from it
//        user = obj.get("username");
//    } else {
//        cerr << "Error handling the http response: " << err << ". Response code: " << response.status() << endl;
//    }
//}
//
//    void handleNewUser(boost::system::error_code err, const Http::Message& response){
//
//    Wt::Json::Object obj;
//    Wt::Json::parse(response.body(),obj);
//
//    if (!err && response.status() == 200) {
//        //get the json from the response and then extract the id from it
//        user = obj.get("username");
//    } else {
//        cerr << "Error handling the http response: " << err << ". Response code: " << response.status() << endl;
//    }
//
//}

