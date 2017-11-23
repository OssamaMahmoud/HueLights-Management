//
// Created by ubuntu on 11/21/17.
//

#include <err.h>
#include "Group.h"
#include "boost/algorithm/string.hpp"


Group::Group(std::string username, std::string address, std::string port, std::string name, vector<std::string> lights) {

    string url = "http://" + address + ":" + port + "/api/" + username + "/groups";
    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Group::handleMakeGroup,this,_1,_2));
    //create json to send
    string JSON_string = R"({"Lights":
                                [")" + vectorToString(lights) + R"(] ", "name": ")" + name +
                         R"(", "type": ")" +
                         "LightGroup" + R"("})";

    Http::Message body = Http::Message();
    body.addBodyText(JSON_string);

    address = "localhost";
    port = "80";
    if (client->post(url, body)){
        //TODO: something i guess
        this->name = name;
        lightList = vectorToString(lights);
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
    Wt::Json::Object obj;
    Wt::Json::parse(response.body(),obj);


    if (!err && response.status() == 200) {
        //get the json from the response and then extract the id from it
        this->id = "";
        id.append(obj.get("id").toString());
    } else {
        cerr << "Error handling the http response: " << err << ". Response code: " << response.status() << endl;
    }
}

int Group::setGroupLights( string newLights) {

    string url = "http://" + address + ":" + port + "/api/" + username + "/groups/" + this->id;
    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Group::handleUpdateLights,this,_1,_2));

    string JSON_string = R"({"Lights": [")" + (newLights) + R"(])" + R"(})";

    Http::Message body = Http::Message();
    body.addBodyText(JSON_string);

    if (client->post(url, body)){
        //TODO: something i guess
        this->lightList = newLights;
    }

    return 0;
}


void Group::handleUpdateLights(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();
    Wt::Json::Object obj;
    Wt::Json::parse(response.body(),obj);


    if (!err && response.status() == 200) {
        //get the json from the response and then extract the id from it
    } else {
        cerr << "Error handling the http response: " << err << ". Response code: " << response.status() << endl;
    }
}




int Group::addLight(string lightID) {


    this->lightList.append(", " + lightID);
    this->setGroupLights(this->lightList);

    return 0;
}

int Group::removeLight(string lightID) {

    //find a way to parse string and remove the god-damn light....
    lightID = "\"" + lightID.append("\"");
    boost::erase_all(this->lightList, lightID);
    this->setGroupLights(this->lightList);

    return 0;
}


int Group::changeState(bool on, int bri, int hue, int sat) {

    string url = "http://" + address + ":" + port + "/api/" + username + "/groups/" + this->id + "/action";

    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Group::handleChangeState, this,_1,_2));

    string JSON_string = R"({"on": ")" + std::string(on ? "f": "d") +
            ", \"bri\": " + std::to_string(bri) +
            ", \"hue\": " + std::to_string(hue)  +
            ", \"sat\": " + std::to_string(sat) +
            R"(})";

    Http::Message body = Http::Message();
    body.addBodyText(JSON_string);

    if (client->put(url, body)){
        //TODO: something i guess
        //update the state by getting it again,
        this->getState();
    }

    return 0;
}

void Group::handleChangeState(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();
    Wt::Json::Object obj;
    Wt::Json::parse(response.body(),obj);


    if (!err && response.status() == 200) {
        //get the json from the response and then extract the id from it
        //this->lightList = obj.get("lights");
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

    return 0;
}


void Group::handleGetState(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();
    Wt::Json::Object obj;
    Wt::Json::parse(response.body(),obj);


    if (!err && response.status() == 200) {
        //get the json from the response and then extract the id from it
        this->lightList = "";
        this->lightList.append(obj.get("lights").toString());
        this->name = "";
        this->name.append(obj.get("name").toString());
        this->state = "";
        this->state.append(obj.get("action").toString());

    } else {
        cerr << "Error handling the http response: " << err << ". Response code: " << response.status() << endl;
    }
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