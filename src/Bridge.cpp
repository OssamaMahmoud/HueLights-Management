//
// Created by ubuntu on 11/7/17.
//

#include "Bridge.h"

using namespace Wt;
using namespace std;

Bridge::Bridge(WContainerWidget *parent):WContainerWidget(parent){

}

Bridge::~Bridge(){

}


/************************************* http request ****************************************/

void Bridge::defaultConnect(string the_address, string the_port, string the_reference) {
    set_address(the_address);
    set_port(the_port);

    set_userName("newdeveloper");
    set_reference(the_reference);

    string url = "http://" + address + ":" + port + "/api/newdeveloper/config";
    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Bridge::handleDefaultResponse,this,_1,_2));

    if(client->get(url)){
        WApplication::instance()->deferRendering();
    }
    else{

        //print an error message to the screen
        cerr << "can't open url\n\n\n\n\n\n\n\n\n" << endl;
    }
}

void Bridge::newUserConnect(string the_address, string the_port, string devicetype, string the_reference)
         {

    //have to add it to the user in this area
    //ok so im use the session instead of just passing in the username
    set_reference(the_reference);
    string the_username = "newdeveloper";
    set_userName(the_username);

    set_address(the_address);
    set_port(the_port);

    string url = "http://" + the_address + ":" + the_port + "/api";
    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Bridge::handleNewUserResponse,this,_1,_2));

    string JSONformat_info = R"({"devicetype": ")" +devicetype + R"(", "name": ")" + the_username + R"("})";
    Http::Message info = Http::Message();
    info.addBodyText(JSONformat_info);


    if(client->post(url, info)){
        WApplication::instance()->deferRendering();
    }
    else{

        //print an error message to the screen
        cerr << "can't open url" << endl;
    }
}

bool Bridge::modifyBridge(string address, string port, string username, string reference) {
    testBridge(address, port, username);
    string result = get_modifyResponse();
    if(result == R"({"error":{"address":"/","description":"unauthorized user","type":"1"}})"){
        return false;
    }else{
        set_userName(username);
        set_address(address);
        set_port(port);
        set_reference(reference);
        return true;
    }

}

void Bridge::testBridge(string the_address, string the_port, string the_username) {

    string url = "http://" + the_address + ":" + the_port + "api/" + the_username + "/config";
    Http::Client *client = new Http::Client();
    client->setTimeout(15);
    client->setMaximumResponseSize(100*1024);
    client->done().connect(boost::bind(&Bridge::handleModifyResponse,this,_1,_2));

    if(client->get(url)){
        WApplication::instance()->deferRendering();
    }
    else{

        //print an error message to the screen
        cerr << "can't open url" << endl;
    }
}


/************************************* http handle ******************************************/

void Bridge::handleDefaultResponse(boost::system::error_code err, const Http::Message& response) {
    WApplication::instance()->resumeRendering();

    if (!err && response.status() == 200){
        set_defaultResponse(response.body());
    }
    else {
        cerr << "Error handling the http response: "<< err << ". Response code: "<< response.status()<< endl;
    }

}

void Bridge::handleNewUserResponse(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();

    if (!err && response.status() == 200){
        set_newUserResponse(response.body());

    }
    else {
        cerr << "Error handling the http response: "<< err << ". Response code: "<< response.status()<< endl;
    }

}

void Bridge::handleModifyResponse(boost::system::error_code err, const Http::Message &response) {
    WApplication::instance()->resumeRendering();

    if (!err && response.status() == 200){
        set_modifyResponse(response.body());
    }
    else {
        cerr << "Error handling the http response: "<< err << ". Response code: "<< response.status()<< endl;
    }

}


/************************************* getter setter ************************************/
void Bridge::set_userName(string the_name) {
    userName = the_name;
}

string Bridge::get_userName() {
    return userName;
}

void Bridge::set_address(string the_address) {
    address = the_address;
}

string Bridge::get_address() {
    return address;
}

void Bridge::set_port(string the_port) {
    port = the_port;
}

string Bridge::get_port() {
    return port;
}

void Bridge::set_defaultResponse(string response) {
    defaultResponse = response;
}

string Bridge::get_defaultResponse() {
    return defaultResponse;
}

void Bridge::set_newUserResponse(string response) {
    newUserResponse = response;
}

string Bridge::get_newUserResponse() {
    return newUserResponse;
}

void Bridge::set_modifyResponse(string response) {
    modifyResponse = response;
}

string Bridge::get_modifyResponse() {
    return modifyResponse;
}

void Bridge::set_reference(string the_reference) {
    reference = the_reference;
}

string Bridge::get_reference() {
    return reference;
}