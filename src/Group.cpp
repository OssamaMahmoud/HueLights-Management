//
// Created by ubuntu on 11/21/17.
//

#include <err.h>
#include "Group.h"
#include "boost/algorithm/string.hpp"


Group::Group(std::string username, std::string address, std::string port, std::string name, std::string lights) {

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

        this->name = name;
        this->address = address;
        this->port = port;
        this->username = username;
        lightList = lights;
    }

}


std::string Group::vectorToString(vector<std::string> vec){
    std::string result;
    for(int i = 0; i <vec.size(); i++){

        result.append(vec[i]);
        if(i != vec.size() - 1)
            result.append(", ");
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

int Group::setGroupLights( string newLights) {

    string url = "http://" + address + ":" + port + "/api/" + username + "/groups/" + this->id;
    std::cout<< "the URL is:   " << url << std::endl;

    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Group::handleUpdateLights,this,_1,_2));

    string JSON_string = R"({"lights": [)" + (newLights) + R"(])" + R"(})";

    Http::Message body = Http::Message();
    body.addBodyText(JSON_string);

    if (client->put(url, body)){
        //TODO: something i guess
        this->lightList = newLights;
    }

    return 0;
}


void Group::handleUpdateLights(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();
    Wt::Json::Array obj;
    Wt::Json::parse(response.body(),obj);


    if (!err && response.status() == 200) {
        //nothing rlly to update
    } else {
        cerr << "Error handling the http response: " << err << ". Response code: " << response.status() << endl;
    }
}




int Group::addLight(string lightID) {


    this->lightList.append(", \"" + lightID + "\"");
    this->setGroupLights(this->lightList);

    return 0;
}

int Group::removeLight(string lightID) {

    //find a way to parse string and remove the god-damn light....
    lightID = "\"" + lightID.append("\",");
    boost::erase_all(this->lightList, lightID);
    lightID = "\"" + lightID.append("\"");

    boost::erase_all(this->lightList, lightID);
    this->setGroupLights(this->lightList);

    return 0;
}


int Group::changeState(string on, string bri, string hue, string sat) {

    string url = "http://" + address + ":" + port + "/api/" + username + "/groups/" + this->id + "/action";

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
        this->getState();
    } else {
        cerr << "Error handling the http response: " << err << ". Response code: " << response.status() << endl;
    }
}


int Group::deleteGroup(){

    string url = "http://" + address + ":" + port + "/api/" + username + "/groups/" + this->id ;

    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Group::handleChangeState, this,_1,_2));

    Http::Message body = Http::Message();

    if (client->deleteRequest(url, body)){
    //TODO: something i guess
    }

    return 0;
}

void Group::getAllGroups(){
    string url = "http://" + address + ":" + port + "/api/" + username + "/groups" ;

    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Group::handleChangeState, this,_1,_2));

    //just throwing in null for fun, idk what is supposed to in theres
    if (client->get(url)){
        //TODO: something i guess
    }

    return;
}


std::string Group::getState() {
    string url = "http://" + address + ":" + port + "/api/" + username + "/groups/" + this->id ;

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
    Wt::Json::parse(response.body(),obj);


    if (!err && response.status() == 200) {
        //get the json from the response and then extract the id from it
        this->lightList = "";
        Json::Array lightsArray = obj.get("lights");
        //for loop go through the  array and make str of it
        for(int i = 0; i < lightsArray.size(); i++){
            if(i == 0)
                this->lightList.append("\"" + string(lightsArray[i].toString()) + "\"");
            else
                this->lightList.append(", \"" + string(lightsArray[i].toString()) + "\"");

        }

        this->name = "";
        this->on = "";
        this->bri = "";
        this->hue = "";

        this->name.append(obj.get("name").toString());

        if(!obj.get("action").isNull()) {
            Json::Object action = obj.get("action");

            this->on.append(action.get("on").toString());

            this->bri.append(action.get("bri").toString());

            this->sat = "";
            this->sat.append(action.get("sat").toString());

            this->hue.append(action.get("hue").toString());
        }

    } else {
        cerr << "Error handling the http response: " << err << ". Response code: " << response.status() << endl;
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